#include "DirectInput.h"
#include <dwrite.h>
#include <dinput.h>


DirectInput::DirectInput()
{
    
}


DirectInput::~DirectInput()
{
	if (DIMouse)
		DIMouse->Unacquire();
	if (DIKeyboard)
		DIKeyboard->Unacquire();
    if (DirectInputObject)
    {
        DirectInputObject->Release();
    }
}

bool DirectInput::InitDirectInput(HINSTANCE hInstance,HWND hWnd)
{
	//Create direct input object
	HRESULT hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)& DirectInputObject, NULL);

	//Create devices for keyboard and mouse
	hr = DirectInputObject->CreateDevice(GUID_SysKeyboard,
		&DIKeyboard,
		NULL);

	hr = DirectInputObject->CreateDevice(GUID_SysMouse,
		&DIMouse,
		NULL);

	//Set data format for the direct input devices
	hr = DIKeyboard->SetDataFormat(&c_dfDIKeyboard);
	hr = DIKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	hr = DIMouse->SetDataFormat(&c_dfDIMouse);
	hr = DIMouse->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);

	return true;

}

void DirectInput::DetectInput(double time, Camera* camera,HWND hwnd)
{
        DIMOUSESTATE mouseCurrState;

        BYTE keyboardState[256];

        DIKeyboard->Acquire();


        DIKeyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);

        if (keyboardState[DIK_O] & 0x80)
        {
            DIMouse->Unacquire();
            m_bIsMouseAcquired = false;
        }

        if (keyboardState[DIK_I] & 0x80)
        {
            DIMouse->Acquire();
            m_bIsMouseAcquired = true;
        }

        DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);

        if (keyboardState[DIK_ESCAPE] & 0x80)
            PostMessage(hwnd, WM_DESTROY, 0, 0);

        float speed = 0.1f * time;

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
            if ((mouseCurrState.lX != mouseLastState.lX) || (mouseCurrState.lY != mouseLastState.lY))
            {
                camera->RotateLeftRight(mouseCurrState.lX * 0.001f);

                camera->RotateUpDown(mouseCurrState.lY * 0.001f);

                mouseLastState = mouseCurrState;
            }

        }

     camera->UpdateCamera();

        return;

}


bool DirectInput::is_left_Button_Pressed(DIMOUSESTATE mouseCurrState)
{
    if (mouseCurrState.rgbButtons[0])
    {
        return true;
    }

    return false;
}
