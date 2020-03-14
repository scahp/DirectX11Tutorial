#include "stdafx.h"
#include "FrustumClass.h"

FrustumClass::FrustumClass()
{

}

FrustumClass::FrustumClass(const FrustumClass& InOther)
{

}

FrustumClass::~FrustumClass()
{

}

void FrustumClass::ConstructFrustum(float InScreenDepth, XMMATRIX InProjectionMatrix, XMMATRIX InViewMatrix)
{
	XMFLOAT4X4 PMatrix;
	XMStoreFloat4x4(&PMatrix, InProjectionMatrix);

	float ZMin = -PMatrix._43 / PMatrix._33;			// Zn 구함.
	float R = InScreenDepth / (InScreenDepth - ZMin);

	PMatrix._33 = R;
	PMatrix._43 = -R * ZMin;
	
	XMMATRIX FinalMatrix = XMMatrixMultiply(InViewMatrix, InProjectionMatrix);

	XMFLOAT4X4 Matrix;
	XMStoreFloat4x4(&Matrix, FinalMatrix);

	// 절두체에 가장 가까운 평면
	float X = (float)(Matrix._14 + Matrix._13);
	float Y = (float)(Matrix._24 + Matrix._23);
	float Z = (float)(Matrix._34 + Matrix._33);
	float W = (float)(Matrix._44 + Matrix._43);
	Plane[0] = XMVectorSet(X, Y, Z, W);
	Plane[0] = XMPlaneNormalize(Plane[0]);

	// 절두체의 가장 먼 평면
	X = (float)(Matrix._14 - Matrix._13);
	Y = (float)(Matrix._24 - Matrix._23);
	Z = (float)(Matrix._34 - Matrix._33);
	W = (float)(Matrix._44 - Matrix._43);
	Plane[1] = XMVectorSet(X, Y, Z, W);
	Plane[1] = XMPlaneNormalize(Plane[1]);

	// 절두체의 왼쪽 평면
	X = (float)(Matrix._14 + Matrix._11);
	Y = (float)(Matrix._24 + Matrix._21);
	Z = (float)(Matrix._34 + Matrix._31);
	W = (float)(Matrix._44 + Matrix._41);
	Plane[2] = XMVectorSet(X, Y, Z, W);
	Plane[2] = XMPlaneNormalize(Plane[2]);

	// 절두체의 오른쪽 평면
	X = (float)(Matrix._14 - Matrix._11);
	Y = (float)(Matrix._24 - Matrix._21);
	Z = (float)(Matrix._34 - Matrix._31);
	W = (float)(Matrix._44 - Matrix._41);
	Plane[3] = XMVectorSet(X, Y, Z, W);
	Plane[3] = XMPlaneNormalize(Plane[3]);

	// 절두체의 윗 평면
	X = (float)(Matrix._14 - Matrix._12);
	Y = (float)(Matrix._24 - Matrix._22);
	Z = (float)(Matrix._34 - Matrix._32);
	W = (float)(Matrix._44 - Matrix._42);
	Plane[4] = XMVectorSet(X, Y, Z, W);
	Plane[4] = XMPlaneNormalize(Plane[4]);

	// 절두체의 아래 평면
	X = (float)(Matrix._14 + Matrix._12);
	Y = (float)(Matrix._24 + Matrix._22);
	Z = (float)(Matrix._34 + Matrix._32);
	W = (float)(Matrix._44 + Matrix._42);
	Plane[5] = XMVectorSet(X, Y, Z, W);
	Plane[5] = XMPlaneNormalize(Plane[5]);
}

bool FrustumClass::CheckPoint(float InX, float InY, float InZ)
{
	for (int i = 0; i < 6; ++i)
	{
		if (XMVectorGetX(XMPlaneDotCoord(Plane[i], XMVectorSet(InX, InY, InZ, 1.0f))) < 0.0f)
			return false;
	}
	return true;
}

bool FrustumClass::CheckCube(float InXCenter, float InYCenter, float InZCenter, float InRadius)
{
	for (int i = 0; i < 6; ++i)
	{
		if (XMVectorGetX(XMPlaneDotCoord(Plane[i]
			, XMVectorSet((InXCenter - InRadius), (InYCenter - InRadius), (InZCenter - InRadius), 1.0f))) >= 0.0f)
		{
			continue;
		}
		if (XMVectorGetX(XMPlaneDotCoord(Plane[i]
			, XMVectorSet((InXCenter + InRadius), (InYCenter - InRadius), (InZCenter - InRadius), 1.0f))) >= 0.0f)
		{
			continue;
		}
		if (XMVectorGetX(XMPlaneDotCoord(Plane[i]
			, XMVectorSet((InXCenter - InRadius), (InYCenter + InRadius), (InZCenter - InRadius), 1.0f))) >= 0.0f)
		{
			continue;
		}

		if (XMVectorGetX(XMPlaneDotCoord(Plane[i]
			, XMVectorSet((InXCenter + InRadius), (InYCenter + InRadius), (InZCenter - InRadius), 1.0f))) >= 0.0f)
		{
			continue;
		}
		if (XMVectorGetX(XMPlaneDotCoord(Plane[i]
			, XMVectorSet((InXCenter - InRadius), (InYCenter - InRadius), (InZCenter + InRadius), 1.0f))) >= 0.0f)
		{
			continue;
		}

		if (XMVectorGetX(XMPlaneDotCoord(Plane[i]
			, XMVectorSet((InXCenter + InRadius), (InYCenter - InRadius), (InZCenter + InRadius), 1.0f))) >= 0.0f)
		{
			continue;
		}
		if (XMVectorGetX(XMPlaneDotCoord(Plane[i]
			, XMVectorSet((InXCenter - InRadius), (InYCenter + InRadius), (InZCenter + InRadius), 1.0f))) >= 0.0f)
		{
			continue;
		}
		if (XMVectorGetX(XMPlaneDotCoord(Plane[i]
			, XMVectorSet((InXCenter + InRadius), (InYCenter + InRadius), (InZCenter + InRadius), 1.0f))) >= 0.0f)
		{
			continue;
		}

		return false;
	}

	return true;
}

bool FrustumClass::CheckSphere(float InXCenter, float InYCenter, float InZCenter, float InRadius)
{
	for (int i = 0; i < 6; ++i)
	{
		if (XMVectorGetX(XMPlaneDotCoord(Plane[i], XMVectorSet(InXCenter, InYCenter, InZCenter, 1.0f))) < -InRadius)
			return false;
	}
	return true;
}

bool FrustumClass::CheckRectangle(float InXCenter, float InYCenter, float InZCenter, float InXSize, float InYSize, float InZSize)
{
	for (int i = 0; i < 6; i++)
	{
		if (XMVectorGetX(XMPlaneDotCoord(Plane[i], XMVectorSet((InXCenter - InXSize), (InYCenter - InYSize), (InZCenter - InZSize), 1.0f))) >= 0.0f)
			continue;

		if (XMVectorGetX(XMPlaneDotCoord(Plane[i], XMVectorSet((InXCenter + InXSize), (InYCenter - InYSize), (InZCenter - InZSize), 1.0f))) >= 0.0f)
			continue;

		if (XMVectorGetX(XMPlaneDotCoord(Plane[i], XMVectorSet((InXCenter - InXSize), (InYCenter + InYSize), (InZCenter - InZSize), 1.0f))) >= 0.0f)
			continue;

		if (XMVectorGetX(XMPlaneDotCoord(Plane[i], XMVectorSet((InXCenter - InXSize), (InYCenter - InYSize), (InZCenter + InZSize), 1.0f))) >= 0.0f)
			continue;

		if (XMVectorGetX(XMPlaneDotCoord(Plane[i], XMVectorSet((InXCenter + InXSize), (InYCenter + InYSize), (InZCenter - InZSize), 1.0f))) >= 0.0f)
			continue;

		if (XMVectorGetX(XMPlaneDotCoord(Plane[i], XMVectorSet((InXCenter + InXSize), (InYCenter - InYSize), (InZCenter + InZSize), 1.0f))) >= 0.0f)
			continue;

		if (XMVectorGetX(XMPlaneDotCoord(Plane[i], XMVectorSet((InXCenter - InXSize), (InYCenter + InYSize), (InZCenter + InZSize), 1.0f))) >= 0.0f)
			continue;

		if (XMVectorGetX(XMPlaneDotCoord(Plane[i], XMVectorSet((InXCenter + InXSize), (InYCenter + InYSize), (InZCenter + InZSize), 1.0f))) >= 0.0f)
			continue;

		return false;
	}

	return true;
}
