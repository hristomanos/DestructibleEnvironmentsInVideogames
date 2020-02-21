#include "Camera.h"



Camera::Camera(UINT WindowWidth,UINT WindowHeight)
{
	camPosition = XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f);
	camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	//Set the View matrix
	camView = XMMatrixLookAtLH(camPosition, camTarget, camUp);

	//Set the Projection matrix
	camProjection = XMMatrixPerspectiveFovLH(0.4f * 3.14f, (float)WindowWidth / WindowHeight, 1.0f, 1000.0f);
}


Camera::~Camera()
{
}


void Camera::MoveForward(float speed)
{
	moveBackForward += speed;
}

void Camera::MoveBackward(float speed)
{
	moveBackForward -= speed;
}

void Camera::StrafeRight(float speed)
{
	moveLeftRight += speed;
}

void Camera::StrafeLeft(float speed)
{
	moveLeftRight -= speed;
}

void Camera::RotateLeftRight(float value)
{
	camYaw += value;
}

void Camera::RotateUpDown(float value)
{
	camPitch += value;
}

void Camera::UpdateCamera()
{
	camRotationMatrix = XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0);
	camTarget = XMVector3TransformCoord(DefaultForward, camRotationMatrix);
	camTarget = XMVector3Normalize(camTarget);

	XMMATRIX RotateYTemptMatrix;
	RotateYTemptMatrix = XMMatrixRotationY(camYaw);

	camRight = XMVector3TransformCoord(DefaultRight, camRotationMatrix);
	camForward = XMVector3TransformCoord(DefaultForward, camRotationMatrix);
	camUp = XMVector3Cross(camForward, camRight);

	camPosition += moveLeftRight * camRight;
	camPosition += moveBackForward * camForward;

	moveLeftRight = 0.0f;
	moveBackForward = 0.0f;

	camTarget = camPosition + camTarget;

	camView = XMMatrixLookAtLH(camPosition, camTarget, camUp);

}