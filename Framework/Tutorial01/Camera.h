#pragma once

#include <DirectXMath.h>
#include "structures.h"
#include <windows.h>

class Camera
{
public:
	Camera(UINT WindowWidth, UINT WindowHeight);
	~Camera();

	XMVECTOR camPosition = XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f);
	XMVECTOR camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX WVP;
	XMMATRIX World;
	XMMATRIX camView;
	XMMATRIX camProjection;

	 XMVECTOR DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	 XMVECTOR DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	 XMVECTOR camForward = XMVectorSet(0.0f,0.0f,1.0f,0.0f);
	 XMVECTOR camRight = XMVectorSet(1.0f,0.0f,0.0f,0.0f);

	XMMATRIX camRotationMatrix;
	XMMATRIX  groundWorld;

	void MoveForward(float speed);
	void MoveBackward(float speed);
	void StrafeRight(float speed);
	void StrafeLeft(float speed);

	

	void RotateLeftRight(float value);
	void RotateUpDown(float value);

	//Rotation left/right. Y axis
	float camYaw = 0.0f;

	//Rotation Up/Down. X axis.
	float camPitch = 0.0f;

	void UpdateCamera();
private:

	float moveBackForward = 0.0f;
	float moveLeftRight = 0.0f;



};

