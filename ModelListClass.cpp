#include "stdafx.h"
#include "ModelListClass.h"
#include <time.h>

ModelListClass::ModelListClass()
{

}

ModelListClass::ModelListClass(const ModelListClass& InOther)
{

}

ModelListClass::~ModelListClass()
{

}

bool ModelListClass::Initialize(int InNumModels)
{
	float Red = 0.0f;
	float Green = 0.0f;
	float Blue = 0.0f;

	ModelCount = InNumModels;

	ModelInfoList = new ModelInfoType[ModelCount];
	if (!ModelInfoList)
		return false;

	srand((unsigned int)time(nullptr));

	// 모든 모델을 거쳐 무작위로 모델 색상과 위치를 생성
	for (int i = 0; i < ModelCount; ++i)
	{
		Red = (float)rand() / RAND_MAX;
		Green = (float)rand() / RAND_MAX;
		Blue = (float)rand() / RAND_MAX;

		ModelInfoList[i].Color = XMFLOAT4(Red, Green, Blue, 1.0f);

		ModelInfoList[i].PositionX = (((float)rand() - (float)rand()) / RAND_MAX) * 10.0f;
		ModelInfoList[i].PositionY = (((float)rand() - (float)rand()) / RAND_MAX) * 10.0f;
		ModelInfoList[i].PositionZ = (((float)rand() - (float)rand()) / RAND_MAX) * 10.0f + 5.0f;
	}
	return true;
}

void ModelListClass::Shutdown()
{
	DX_DELETE_ARRAY(ModelInfoList);
}

int ModelListClass::GetModelCount()
{
	return ModelCount;
}

void ModelListClass::GetData(int InIndex, float& OutPositionX, float& OutPositionY, float& OutPositionZ, XMFLOAT4& OutColor)
{
	OutPositionX = ModelInfoList[InIndex].PositionX;
	OutPositionY = ModelInfoList[InIndex].PositionY;
	OutPositionZ = ModelInfoList[InIndex].PositionZ;

	OutColor = ModelInfoList[InIndex].Color;
}