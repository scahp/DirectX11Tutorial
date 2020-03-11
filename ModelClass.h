#pragma once

class TextureClass;

class ModelClass : public AlignedAllocationPolicy<16>
{
private:
	//struct VertexType
	//{
	//	XMFLOAT3 Position;
	//	XMFLOAT4 Color;
	//};

	struct VertexType
	{
		XMFLOAT3 Position;
		XMFLOAT2 TexCoord;
		XMFLOAT3 Normal;
	};

	struct ModelType
	{
		float X, Y, Z;
		float TU, TV;
		float NX, NY, NZ;
	};

public:
	ModelClass();
	ModelClass(const ModelClass& InClass);
	~ModelClass();

	bool Initialize(ID3D11Device* InDevice, ID3D11DeviceContext* InDeviceContext, const char* InModelFilename, const char* InTextureFilename);
	void Shutdown();
	void Bind(ID3D11DeviceContext* InDeviceContext);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture() const;	

private:
	bool InitializeBuffers(ID3D11Device* InDevice);
	void ShutdownBuffers();
	void BindBuffers(ID3D11DeviceContext* InDeviceContext);

	bool LoadTexture(ID3D11Device* InDevice, ID3D11DeviceContext* InDeviceContext, const char* InFilename);
	void ReleaseTexture();

	bool LoadModel(const char* InFilename);
	void ReleaseModel();

private:
	ID3D11Buffer* VertexBuffer = nullptr;
	ID3D11Buffer* IndexBuffer = nullptr;
	int VertexCount = 0;
	int IndexCount = 0;
	TextureClass* Texture = nullptr;
	ModelType* Model = nullptr;
};