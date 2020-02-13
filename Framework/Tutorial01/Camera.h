#pragma once

#include <DirectXMath.h>
#include "structures.h"


class Camera
{
public:
	Camera();
	~Camera();

	XMVECTOR camPosition = XMVectorSet(0.0f, 0.0f, -0.5f, 0.0f);
	XMVECTOR camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX WVP;
	XMMATRIX World;
	XMMATRIX camView;
	XMMATRIX camProjection;

	const XMVECTOR DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	const XMVECTOR camForward = XMVectorSet(0.0f,0.0f,1.0f,0.0f);
	const XMVECTOR camRight = XMVectorSet(1.0f,0.0f,0.0f,0.0f);

	XMMATRIX camRotationMatrix;

	float moveLeftRight = 0.0f;
	float moveBackForward = 0.0f;

	//Rotation left/right. Y axis
	float camYaw = 0.0f;

	//Rotation Up/Down. X axis.
	float camPitch = 0.0f;

	void UpdateCamera();

};

