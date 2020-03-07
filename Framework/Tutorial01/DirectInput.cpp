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
	hr = m_pDIKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	hr = m_pDIMouse->SetDataFormat(&c_dfDIMouse);
	hr = m_pDIMouse->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);

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


bool DirectInput::is_left_Button_Pressed(DIMOUSESTATE mouseCurrState)
{
    if (mouseCurrState.rgbButtons[0])
    {
        return true;
    }

    return false;
}
