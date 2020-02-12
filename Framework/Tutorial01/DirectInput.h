#pragma once
#pragma comment (lib,"dinput8.lib")
#pragma comment (lib,"dxguid.lib")
#include <dinput.h>


class DirectInput
{
public:
	DirectInput();
	~DirectInput();

	//COM interface. Represent a physical object to get our input.
	IDirectInputDevice8* DIKeyboard;
	IDirectInputDevice8* DIMouse;

	//Hold the last state of the mouse
	DIMOUSESTATE mouseLastState;
	//Structure that gets input from keyboard
	LPDIRECTINPUT8 DirectInputObject;

	bool InitDirectInput(HINSTANCE hInstance,HWND hWnd);
	void DetectInput(double time);



};

