#include "stdafx.h"
#include "LightClass.h"

LightClass::LightClass()
{

}

LightClass::LightClass(const LightClass& InOther)
{

}

void LightClass::SetAmbientColor(float InRed, float InGreen, float InBlue, float InAlpha)
{
	AmbientColor = XMFLOAT4(InRed, InGreen, InBlue, InAlpha);
}

void LightClass::SetDiffuseColor(float InRed, float InGreen, float InBlue, float InAlpha)
{
	DiffuseColor = XMFLOAT4(InRed, InGreen, InBlue, InAlpha);
}

void LightClass::SetDirection(float InX, float InY, float InZ)
{
	Direction = XMFLOAT3(InX, InY, InZ);
}

DirectX::XMFLOAT4 LightClass::GetAmbientColor()
{
	return AmbientColor;
}

XMFLOAT4 LightClass::GetDiffuseColor()
{
	return DiffuseColor;
}

XMFLOAT3 LightClass::GetDirection()
{
	return Direction;
}
