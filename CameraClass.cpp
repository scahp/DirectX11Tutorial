#include "stdafx.h"
#include "CameraClass.h"

CameraClass::CameraClass()
{
	Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

CameraClass::CameraClass(const CameraClass& InClass)
{

}

CameraClass::~CameraClass()
{

}

void CameraClass::SetPosition(float InX, float InY, float InZ)
{
	Position.x = InX;
	Position.y = InY;
	Position.z = InZ;
}

void CameraClass::SetRotation(float InX, float InY, float InZ)
{
	Rotation.x = InX;
	Rotation.y = InY;
	Rotation.z = InZ;
}

void CameraClass::Render()
{
	XMFLOAT3 Up, Pos, LookAt;
	XMVECTOR UpVector, PositionVector, LookAtVector;
	float Yaw, Pitch, Roll;
	XMMATRIX RotationMatrix;

	Up.x = 0.0f;
	Up.y = 1.0f;
	Up.x = 0.0f;
	UpVector = XMLoadFloat3(&Up);

	Pos = Position;
	PositionVector = XMLoadFloat3(&Pos);

	LookAt.x = 0.0f;
	LookAt.y = 0.0f;
	LookAt.z = 1.0f;
	LookAtVector = XMLoadFloat3(&LookAt);

	static float temp = 0.0174532925f;
	Pitch = Rotation.x * temp;
	Yaw = Rotation.y * temp;
	Roll = Rotation.z * temp;

	RotationMatrix = XMMatrixRotationRollPitchYaw(Pitch, Yaw, Roll);

	LookAtVector = XMVector3TransformCoord(LookAtVector, RotationMatrix);
	UpVector = XMVector3TransformCoord(UpVector, RotationMatrix);
	LookAtVector = XMVectorAdd(PositionVector, LookAtVector);
	ViewMatrix = XMMatrixLookAtLH(PositionVector, LookAtVector, UpVector);
}
