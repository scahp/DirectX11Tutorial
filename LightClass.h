#pragma once

class LightClass
{
public:
	LightClass();
	LightClass(const LightClass& InOther);

	void SetAmbientColor(float InRed, float InGreen, float InBlue, float InAlpha);
	void SetDiffuseColor(float InRed, float InGreen, float InBlue, float InAlpha);
	void SetDirection(float InX, float InY, float InZ);
	void SetSpecularColor(float InRed, float InGreen, float InBlue, float InAlpha);
	void SetSpecularPower(float InPower);

	XMFLOAT4 GetAmbientColor();
	XMFLOAT4 GetDiffuseColor();
	XMFLOAT3 GetDirection();
	XMFLOAT4 GetSpecularColor();
	float GetSpecularPower();

private:
	XMFLOAT4 AmbientColor;
	XMFLOAT4 DiffuseColor;
	XMFLOAT3 Direction;
	XMFLOAT4 SpecularColor;
	float SpecularPower;
};

