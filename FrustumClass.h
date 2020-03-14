#pragma once

class FrustumClass : public AlignedAllocationPolicy<16>
{
public:
	FrustumClass();
	FrustumClass(const FrustumClass& InOther);
	~FrustumClass();

	void ConstructFrustum(float InScreenDepth, XMMATRIX InProjectionMatrix, XMMATRIX InViewMatrix);
	bool CheckPoint(float InX, float InY, float InZ);
	bool CheckCube(float InXCenter, float InYCenter, float InZCenter, float InRadius);
	bool CheckSphere(float InXCenter, float InYCenter, float InZCenter, float InRadius);
	bool CheckRectangle(float InXCenter, float InYCenter, float InZCenter, float InXSize, float InYSize, float InZSize);
private:
	XMVECTOR Plane[6];
};

