#pragma once

class ModelClass : public AlignedAllocationPolicy<16>
{
private:
	struct VertexType
	{
		XMFLOAT3 Position;
		XMFLOAT4 Color;
	};

public:
	ModelClass();
	ModelClass(const ModelClass& InClass);
	~ModelClass();

	bool Initialize(ID3D11Device* InDevice);
	void Shutdown();
	void Bind(ID3D11DeviceContext* InDeviceContext);
	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device* InDevice);
	void ShutdownBuffers();
	void BindBuffers(ID3D11DeviceContext* InDeviceContext);

private:
	ID3D11Buffer* VertexBuffer = nullptr;
	ID3D11Buffer* IndexBuffer = nullptr;
	int VertexCount = 0;
	int IndexCount = 0;
};