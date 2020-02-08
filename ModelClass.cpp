#include "stdafx.h"
#include "ModelClass.h"

ModelClass::ModelClass()
{

}

ModelClass::ModelClass(const ModelClass& InClass)
{

}

ModelClass::~ModelClass()
{

}

bool ModelClass::Initialize(ID3D11Device* InDevice)
{
	return InitializeBuffers(InDevice);
}

void ModelClass::Shutdown()
{
	ShutdownBuffers();
}

void ModelClass::Bind(ID3D11DeviceContext* InDeviceContext)
{
	BindBuffers(InDeviceContext);
}

int ModelClass::GetIndexCount()
{
	return IndexCount;
}

bool ModelClass::InitializeBuffers(ID3D11Device* InDevice)
{
	VertexCount = 3;
	IndexCount = 3;

	VertexType* Vertices = new VertexType[VertexCount];
	if (!Vertices)
		return false;

	unsigned long* Indices = new unsigned long[IndexCount];
	if (!Indices)
	{
		DX_DELETE_ARRAY(Vertices);
		return false;
	}

	// 정점 배열에 데이터를 설정합니다.
	Vertices[0].Position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	Vertices[0].Color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	Vertices[1].Position = XMFLOAT3(0.0f, 1.0f, 0.0f);  // Top middle.
	Vertices[1].Color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	Vertices[2].Position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
	Vertices[2].Color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	Indices[0] = 0;
	Indices[1] = 1;
	Indices[2] = 2;

	D3D11_BUFFER_DESC VertexBufferDesc;
	VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	VertexBufferDesc.ByteWidth = sizeof(VertexType) * VertexCount;
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = 0;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;

	// Subresource 구조에 정점 데이터에 대한 포인터 제공
	D3D11_SUBRESOURCE_DATA VertexData;
	VertexData.pSysMem = Vertices;
	VertexData.SysMemPitch = 0;
	VertexData.SysMemSlicePitch = 0;

	if (FAILED(InDevice->CreateBuffer(&VertexBufferDesc, &VertexData, &VertexBuffer)))
	{
		DX_DELETE_ARRAY(Vertices);
		DX_DELETE_ARRAY(Indices);
		return false;
	}

	D3D11_BUFFER_DESC IndexBufferDesc;
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth = sizeof(unsigned long) * IndexCount;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;

	// Subresource 구조에 인덱스 데이터에 대한 포인터 제공
	D3D11_SUBRESOURCE_DATA IndexData;
	IndexData.pSysMem = Indices;
	IndexData.SysMemPitch = 0;
	IndexData.SysMemSlicePitch = 0;

	if (FAILED(InDevice->CreateBuffer(&IndexBufferDesc, &IndexData, &IndexBuffer)))
	{
		DX_DELETE_ARRAY(Vertices);
		DX_DELETE_ARRAY(Indices);
		DX_RELEASE(VertexBuffer);
		return false;
	}

	DX_DELETE_ARRAY(Vertices);
	DX_DELETE_ARRAY(Indices);

	return true;
}

void ModelClass::ShutdownBuffers()
{
	DX_RELEASE(IndexBuffer);
	DX_RELEASE(VertexBuffer);
}

void ModelClass::BindBuffers(ID3D11DeviceContext* InDeviceContext)
{
	unsigned int Stride = sizeof(VertexType);
	unsigned int Offset = 0;

	InDeviceContext->IASetVertexBuffers(0, 1, &VertexBuffer, &Stride, &Offset);
	InDeviceContext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	InDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
