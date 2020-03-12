#pragma once

class LightClass
{
public:
	LightClass();
	LightClass(const LightClass& InOther);

	void SetAmbientColor(float InRed, float InGreen, float InBlue, float InAlpha);
	void SetDiffuseColor(float InRed, float InGreen, float InBlue, float InAlpha);
	void SetDirection(float InX, float InY, float InZ);

	XMFLOAT4 GetAmbientColor();
	XMFLOAT4 GetDiffuseColor();
	XMFLOAT3 GetDirection();

private:
	XMFLOAT4 AmbientColor;
	XMFLOAT4 DiffuseColor;
	XMFLOAT3 Direction;
};

