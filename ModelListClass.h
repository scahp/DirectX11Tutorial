#pragma once

class ModelListClass
{
private:
	struct ModelInfoType
	{
		XMFLOAT4 Color;
		float PositionX, PositionY, PositionZ;
	};

public:
	ModelListClass();
	ModelListClass(const ModelListClass& InOther);
	~ModelListClass();

	bool Initialize(int InNumModels);
	void Shutdown();
	int GetModelCount();
	void GetData(int InIndex, float& OutPositionX, float& OutPositionY, float& OutPositionZ, XMFLOAT4& OutColor);
private:
	int ModelCount = 0;
	ModelInfoType* ModelInfoList = nullptr;
};

