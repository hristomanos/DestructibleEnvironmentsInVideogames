#pragma once
#pragma comment (lib,"dinput8.lib")
#pragma comment (lib,"dxguid.lib")

#define _XM_NO_INTRINSICS_

#include <dinput.h>
#include"Camera.h"

#include "Utilities/Helper.h"
#include <vector>

#include "DrawableGameObject.h"

class DirectInput
{
public:
	DirectInput();
	~DirectInput();
	

	bool InitDirectInput(HINSTANCE hInstance,HWND hWnd);
	void DetectInput(double time,Camera* camera, HWND hWnd,std::vector<XMFLOAT3>& vertPosArray, std::vector<WORD>& indexPosArray, XMMATRIX& worldSpace);

	bool is_left_Button_Pressed(DIMOUSESTATE mouseCurrState);

	bool rayHit = false;

private:
	bool m_bIsMouseAcquired = false;

	float pickedDist = 0.0f;
	bool  isShoot = false;

	//COM interface. Represent a physical object to get our input.
	IDirectInputDevice8* m_pDIKeyboard;
	IDirectInputDevice8* m_pDIMouse;

	//Holds the last state of the mouse
	DIMOUSESTATE m_MouseLastState;

	//Structure that gets input from keyboard
	LPDIRECTINPUT8 m_DirectInputObject;

	void PickRayVector(float mouseX, float mouseY, XMVECTOR& pickRayInWorldSpacePos, XMVECTOR& pickRayInWorldSpaceDir,Camera* camera);
	bool PointInTriangle(XMVECTOR& triV1, XMVECTOR& triV2, XMVECTOR& triV3, XMVECTOR& point);
	float Pick(XMVECTOR pickRayInWorldSpacePos, XMVECTOR pickRayInWorldSpaceDir, std::vector<XMFLOAT3>& vertPosArray, std::vector<WORD>& indexPosArray, XMMATRIX& worldSpace);

	int* cubeHit = new int[5];

};



