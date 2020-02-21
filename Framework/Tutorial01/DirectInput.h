#pragma once
#pragma comment (lib,"dinput8.lib")
#pragma comment (lib,"dxguid.lib")
#include <dinput.h>
#include"Camera.h"

class DirectInput
{
public:
	DirectInput();
	~DirectInput();

	//COM interface. Represent a physical object to get our input.
	IDirectInputDevice8* DIKeyboard;
	IDirectInputDevice8* DIMouse;

	//Holds the last state of the mouse
	DIMOUSESTATE mouseLastState;

	//Structure that gets input from keyboard
	LPDIRECTINPUT8 DirectInputObject;

	bool InitDirectInput(HINSTANCE hInstance,HWND hWnd);
	void DetectInput(double time,Camera* camera, HWND hWnd);

	bool is_left_Button_Pressed(DIMOUSESTATE mouseCurrState);

private:
	bool m_bIsMouseAcquired = false;
};



