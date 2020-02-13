#include "DirectInput.h"



DirectInput::DirectInput()
{
}


DirectInput::~DirectInput()
{
	if (DIMouse)
		DIMouse->Release();
	if (DIKeyboard)
		DIKeyboard->Release();
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

void DirectInput::DetectInput(double time)
{
	//Current mouse state variable
	DIMOUSESTATE mouseCurrState;

	//Hold possible keyboard keys to be pressed
	BYTE keyboardState[256];

	//Acquire device
	DIKeyboard->Acquire();

	DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);

	DIKeyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);

	//if (keyboardState[DIK_ESCAPE] & 0x80)
		//	PostMessage(hWnd,WM_DESTROY,0,0);

	if (mouseCurrState.lX != mouseLastState.lX)
	{

	}



	mouseLastState = mouseCurrState;

}



