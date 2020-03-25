#pragma once
#include "TextureArrayClass.h"

class TextureClass;

class ModelClass : public AlignedAllocationPolicy<16>
{
private:
	struct VertexType
	{
		XMFLOAT3 Position;
		XMFLOAT2 Texture;
		XMFLOAT3 Normal;
		XMFLOAT3 Tangent;
		XMFLOAT3 Binormal;
	};

	struct ModelType
	{
		float X, Y, Z;
		float TU, TV;
		float NX, NY, NZ;
		float TX, TY, TZ;
		float BX, BY, BZ;
	};

	struct TempVertexType
	{
		float X, Y, Z;
		float TU, TV;
		float NX, NY, NZ;
	};

	struct VectorType
	{
		float X, Y, Z;
	};

public:
	ModelClass();
	ModelClass(const ModelClass& InClass);
	~ModelClass();

	bool Initialize(ID3D11Device* InDevice, ID3D11DeviceContext* InDeviceContext, const char* InModelFilename
		, const WCHAR* InTextureFilename1, const WCHAR* InTextureFilename2);
	void Shutdown();
	void Bind(ID3D11DeviceContext* InDeviceContext);

	int GetIndexCount();
	ID3D11ShaderResourceView** GetTextureArray();
	bool LoadTextures(ID3D11Device* InDevice, const WCHAR* InFilename1, const WCHAR* InFilename2);

	void ReleaseTextures();
private:
	bool InitializeBuffers(ID3D11Device* InDevice);
	void ShutdownBuffers();
	void BindBuffers(ID3D11DeviceContext* InDeviceContext);

	bool LoadModel(const char* InFilename);
	void ReleaseModel();

	void CalculateModelVectors();
	void CalculateTangentBinormal(TempVertexType InVertex1, TempVertexType InVertex2, TempVertexType InVertex3, VectorType& OutTangent, VectorType& OutBinormal);
	void CalculateNormal(VectorType InTangent, VectorType InBinormal, VectorType& OutNormal);
private:
	ID3D11Buffer* VertexBuffer = nullptr;
	ID3D11Buffer* IndexBuffer = nullptr;
	int VertexCount = 0;
	int IndexCount = 0;
	ModelType* Model = nullptr;
	TextureArrayClass* TextureArray = nullptr;
};