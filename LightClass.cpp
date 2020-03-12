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

void LightClass::SetSpecularColor(float InRed, float InGreen, float InBlue, float InAlpha)
{
	SpecularColor = XMFLOAT4(InRed, InGreen, InBlue, InAlpha);
}

void LightClass::SetSpecularPower(float InPower)
{
	SpecularPower = InPower;
}

XMFLOAT4 LightClass::GetAmbientColor()
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

XMFLOAT4 LightClass::GetSpecularColor()
{
	return SpecularColor;
}

float LightClass::GetSpecularPower()
{
	return SpecularPower;
}
