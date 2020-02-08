#pragma once

class CameraClass : public AlignedAllocationPolicy<16>
{
public:
	CameraClass();
	CameraClass(const CameraClass& InClass);
	~CameraClass();

	void SetPosition(float InX, float InY, float InZ);
	void SetRotation(float InX, float InY, float InZ);

	void Render();

	XMFLOAT3 GetPosition() const { return Position; }
	XMFLOAT3 GetRotation() const { return Rotation; }
	void GetViewMatrix(XMMATRIX& OutMatrix) const { OutMatrix = ViewMatrix; }

private:
	XMFLOAT3 Position;
	XMFLOAT3 Rotation;
	XMMATRIX ViewMatrix;
};