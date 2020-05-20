#include "DirectInput.h"
#include <dwrite.h>
#include <dinput.h>

DirectInput::DirectInput()
{
  
}


DirectInput::~DirectInput()
{
	if (m_pDIMouse)
		m_pDIMouse->Unacquire();
	if (m_pDIKeyboard)
		m_pDIKeyboard->Unacquire();
    if (m_DirectInputObject)
    {
        m_DirectInputObject->Release();
    }
}

bool DirectInput::InitDirectInput(HINSTANCE hInstance,HWND hWnd)
{
	//Create direct input object
	HRESULT hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)& m_DirectInputObject, NULL);

	//Create devices for keyboard and mouse
	hr = m_DirectInputObject->CreateDevice(GUID_SysKeyboard,
		&m_pDIKeyboard,
		NULL);

	hr = m_DirectInputObject->CreateDevice(GUID_SysMouse,
		&m_pDIMouse,
		NULL);

	//Set data format for the direct input devices
	hr = m_pDIKeyboard->SetDataFormat(&c_dfDIKeyboard);
	hr = m_pDIKeyboard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	hr = m_pDIMouse->SetDataFormat(&c_dfDIMouse);
	hr = m_pDIMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_NOWINKEY | DISCL_BACKGROUND);

    for (int i = 0; i < 5; i++)
    {
        cubeHit[i] = 0;
    }


	return true;

}

void DirectInput::DetectInput(double time, Camera* camera,HWND hwnd)
{
        DIMOUSESTATE mouseCurrState;

        BYTE keyboardState[256];

        m_pDIKeyboard->Acquire();


        m_pDIKeyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);

        if (keyboardState[DIK_O] & 0x80)
        {
            m_pDIMouse->Unacquire();
            m_bIsMouseAcquired = false;
        }

        if (keyboardState[DIK_I] & 0x80)
        {
            m_pDIMouse->Acquire();
            m_bIsMouseAcquired = true;
        }

        m_pDIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);

        if (keyboardState[DIK_ESCAPE] & 0x80)
            PostMessage(hwnd, WM_DESTROY, 0, 0);

        float speed = camera->m_MovementSpeed * time;

        if (keyboardState[DIK_A] & 0x80)
        {  
            camera->StrafeLeft(speed);
        }
        if (keyboardState[DIK_D] & 0x80)
        {
            camera->StrafeRight(speed);
        }
        if (keyboardState[DIK_W] & 0x80)
        {
            camera->MoveForward(speed);
        }
        if (keyboardState[DIK_S] & 0x80)
        {
            camera->MoveBackward(speed);
        }





        if (m_bIsMouseAcquired == true)
        {
			////Left mouse button was pressed
			if (mouseCurrState.rgbButtons[0])
			{
				if (isShoot == false)
				{
                    //Get curson position on 2D screen space
					POINT mousePos;
					GetCursorPos(&mousePos);

                    //Translate position to the client windows position 
                    //(In windowed mode, the client window might 
                    //not align exactly with the monitors screen)
					ScreenToClient(hwnd, &mousePos); 

                    //X and Y coordinates of the cursor
					int mouseX = mousePos.x;
					int mouseY = mousePos.y;

					float tempDist; //Will store the picked distance to the current object being checked
					float closestDist = FLT_MAX; //Disance to closest object.
					int hitIndex; // Will store the index value of the closest picked object.

					XMVECTOR pickRayInWorldSpacePos, pickRayInWorldSpaceDir; //World space picking ray position and direction

                    //TRANSFORM from 2D projection space to 3D world space
					PickRayVector(mouseX, mouseY, pickRayInWorldSpacePos, pickRayInWorldSpaceDir,camera); 

					if (closestDist < FLT_MAX)
					{
						cubeHit[hitIndex] = 1;
						pickedDist = closestDist;
						
					}

					isShoot = true;
				}
			}
				if (!mouseCurrState.rgbButtons[0])
				{
					isShoot = false;
                    rayHit = false;
				}

				if ((mouseCurrState.lX != m_MouseLastState.lX) || (mouseCurrState.lY != m_MouseLastState.lY))
				{
					camera->RotateLeftRight(mouseCurrState.lX * 0.001f);

					camera->RotateUpDown(mouseCurrState.lY * 0.001f);

					m_MouseLastState = mouseCurrState;
				}
        }

     camera->UpdateCamera();

        return;

}

void DirectInput::PickRayVector(float mouseX, float mouseY, XMVECTOR& pickRayInWorldSpacePos,XMVECTOR& pickRayInWorldSpaceDir, Camera* camera)
{
	XMVECTOR pickRayInViewSpaceDir = XMVectorSet(0.0f,0.0f,0.0f,0.0f);
	XMVECTOR pickRayInViewSpacePos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

    //2D SCREEN SPACE -> 3D VIEW SPACE
	//Transform 2D pick position on screen space to 3D ray in View space
    float PRVecX, PRVecY, PRVecZ;
	XMMATRIX camProjection = camera->GetProjectionMatrix();

	PRVecX = (((2.0 * mouseX) / 1280 )  - 1 ) / camProjection(0, 0);
	PRVecY = -(((2.0 * mouseY) / 720)   - 1)  / camProjection(1, 1);
	PRVecZ = 1.0f; //View space's Z direction ranges from 0 to 1, so we set 1 since the ray goes "into" the screen

	pickRayInViewSpaceDir = XMVectorSet(PRVecX, PRVecY, PRVecZ, 0.0f);

    //2D VIEW SPACE -> 3D WORLD SPACE
	//Transform 3D Ray from View space to 3D ray in World space
	XMMATRIX pickRayToWorldSpaceMatrix;
	XMVECTOR matInvDeter; 

	pickRayToWorldSpaceMatrix = XMMatrixInverse(&matInvDeter, camera->GetViewMatrix());


	pickRayInWorldSpacePos = XMVector3TransformCoord(pickRayInViewSpacePos, pickRayToWorldSpaceMatrix);
	pickRayInWorldSpaceDir = XMVector3TransformNormal(pickRayInViewSpaceDir, pickRayToWorldSpaceMatrix);

}


bool DirectInput::PointInTriangle(XMVECTOR& triV1, XMVECTOR& triV2, XMVECTOR& triV3, XMVECTOR& point)
{
    //To find out if the point is inside the triangle, we check to see if the point
    //is on the correct side of each of the triangles edges.

    XMVECTOR cp1 = XMVector3Cross((triV3 - triV2), (point - triV2));
    XMVECTOR cp2 = XMVector3Cross((triV3 - triV2), (triV1 - triV2));
    if (XMVectorGetX(XMVector3Dot(cp1, cp2)) >= 0)
    {
        cp1 = XMVector3Cross((triV3 - triV1), (point - triV1));
        cp2 = XMVector3Cross((triV3 - triV1), (triV2 - triV1));
        if (XMVectorGetX(XMVector3Dot(cp1, cp2)) >= 0)
        {
            cp1 = XMVector3Cross((triV2 - triV1), (point - triV1));
            cp2 = XMVector3Cross((triV2 - triV1), (triV3 - triV1));
            if (XMVectorGetX(XMVector3Dot(cp1, cp2)) >= 0)
            {
                return true;
            }
            else
                return false;
        }
        else
            return false;
    }
    return false;
}


//Checks our object to see if our ray intersects with it. 
//It returns a float which is the distance to the picked object.
//Otherwise: float_max
float DirectInput::Pick(XMVECTOR pickRayInWorldSpacePos,
    XMVECTOR pickRayInWorldSpaceDir,
    std::vector<XMFLOAT3>& vertPosArray,
    std::vector<WORD>& indexPosArray,
    XMMATRIX& worldSpace)
{
    //Loop through each triangle in the object
    for (int i = 0; i < indexPosArray.size() / 3; i++)
    {
        //Fill in the plane equation to identify the plane for intersection checking
        //Triangle's vertices V1, V2, V3
        XMVECTOR tri1V1 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
        XMVECTOR tri1V2 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
        XMVECTOR tri1V3 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

        //Temporary 3d floats for each vertex
        XMFLOAT3 tV1, tV2, tV3;

        //Get triangle 
        tV1 = vertPosArray[indexPosArray[(i * 3) + 0]];
        tV2 = vertPosArray[indexPosArray[(i * 3) + 1]]; 
        tV3 = vertPosArray[indexPosArray[(i * 3) + 2]]; 

        tri1V1 = XMVectorSet(tV1.x, tV1.y, tV1.z, 0.0f);
        tri1V2 = XMVectorSet(tV2.x, tV2.y, tV2.z, 0.0f);
        tri1V3 = XMVectorSet(tV3.x, tV3.y, tV3.z, 0.0f);

        //Transform the vertices to world space
        tri1V1 = XMVector3TransformCoord(tri1V1, worldSpace);
        tri1V2 = XMVector3TransformCoord(tri1V2, worldSpace);
        tri1V3 = XMVector3TransformCoord(tri1V3, worldSpace);

        //Find the normal using U, V coordinates (two edges)
        XMVECTOR U = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
        XMVECTOR V = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
        XMVECTOR faceNormal = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

        //Get two edges
        U = tri1V2 - tri1V1;
        V = tri1V3 - tri1V1;

        //Compute face normal by crossing U, V
        faceNormal = XMVector3Cross(U, V);

        faceNormal = XMVector3Normalize(faceNormal);

        //Calculate a point on the triangle for the plane equation
        XMVECTOR triPoint = tri1V1;

        //Get plane equation ("Ax + By + Cz + D = 0") Variables
        float tri1A = XMVectorGetX(faceNormal);
        float tri1B = XMVectorGetY(faceNormal);
        float tri1C = XMVectorGetZ(faceNormal);
        float tri1D = (-tri1A * XMVectorGetX(triPoint) - tri1B * XMVectorGetY(triPoint) - tri1C * XMVectorGetZ(triPoint));

        //Now we find where (on the ray) the ray intersects with the triangles plane
        float ep1, ep2, t = 0.0f;
        float planeIntersectX, planeIntersectY, planeIntersectZ = 0.0f;
        XMVECTOR pointInPlane = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

        ep1 = (XMVectorGetX(pickRayInWorldSpacePos) * tri1A) + (XMVectorGetY(pickRayInWorldSpacePos) * tri1B) + (XMVectorGetZ(pickRayInWorldSpacePos) * tri1C);
        ep2 = (XMVectorGetX(pickRayInWorldSpaceDir) * tri1A) + (XMVectorGetY(pickRayInWorldSpaceDir) * tri1B) + (XMVectorGetZ(pickRayInWorldSpaceDir) * tri1C);

        //Make sure there are no divide-by-zeros
        if (ep2 != 0.0f)
            t = -(ep1 + tri1D) / (ep2);

        if (t > 0.0f)    //Make sure you don't pick objects behind the camera
        {
            //Get the point on the plane
            planeIntersectX = XMVectorGetX(pickRayInWorldSpacePos) + XMVectorGetX(pickRayInWorldSpaceDir) * t;
            planeIntersectY = XMVectorGetY(pickRayInWorldSpacePos) + XMVectorGetY(pickRayInWorldSpaceDir) * t;
            planeIntersectZ = XMVectorGetZ(pickRayInWorldSpacePos) + XMVectorGetZ(pickRayInWorldSpaceDir) * t;

            pointInPlane = XMVectorSet(planeIntersectX, planeIntersectY, planeIntersectZ, 0.0f);

            //Call function to check if point is in the triangle
            if (PointInTriangle(tri1V1, tri1V2, tri1V3, pointInPlane))
            {
                //Return the distance to the hit, so you can check all the other pickable objects in your scene
                //and choose whichever object is closest to the camera
                return t / 2.0f;
            }
        }
    }
    //return the max float value (near infinity) if an object was not picked
    return FLT_MAX;
}