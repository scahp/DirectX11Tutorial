#include "stdafx.h"
#include "ModelClass.h"
#include "TextureClass.h"
#include <fstream>

ModelClass::ModelClass()
{

}

ModelClass::ModelClass(const ModelClass& InClass)
{

}

ModelClass::~ModelClass()
{

}

bool ModelClass::Initialize(ID3D11Device* InDevice, ID3D11DeviceContext* InDeviceContext, const char* InModelFilename
	, const WCHAR* InTextureFilename1, const WCHAR* InTextureFilename2)
{
	if (!LoadModel(InModelFilename))
		return false;

	if (!InitializeBuffers(InDevice))
		return false;

	return LoadTextures(InDevice, InTextureFilename1, InTextureFilename2);
}

void ModelClass::Shutdown()
{
	ReleaseTextures();
	ShutdownBuffers();
	ReleaseModel();
}

void ModelClass::Bind(ID3D11DeviceContext* InDeviceContext)
{
	BindBuffers(InDeviceContext);
}

int ModelClass::GetIndexCount()
{
	return IndexCount;
}

ID3D11ShaderResourceView** ModelClass::GetTextureArray()
{
	return TextureArray->GetTextureArray();
}

bool ModelClass::LoadTextures(ID3D11Device* InDevice, const WCHAR* InFilename1, const WCHAR* InFilename2)
{
	TextureArray = new TextureArrayClass();
	if (!TextureArray)
		return false;

	return TextureArray->Initialize(InDevice, InFilename1, InFilename2);
}

void ModelClass::ReleaseTextures()
{
	if (TextureArray)
	{
		TextureArray->Shutdown();
		DX_DELETE(TextureArray);
	}
}

bool ModelClass::InitializeBuffers(ID3D11Device* InDevice)
{
	VertexType* Vertices = new VertexType[VertexCount];
	if (!Vertices)
		return false;

	unsigned long* Indices = new unsigned long[IndexCount];
	if (!Indices)
	{
		DX_DELETE_ARRAY(Vertices);
		return false;
	}

	for (int i = 0; i < VertexCount; ++i)
	{
		Vertices[i].Position = XMFLOAT3(Model[i].X, Model[i].Y, Model[i].Z);
		Vertices[i].TexCoord = XMFLOAT2(Model[i].TU, Model[i].TV);
		Vertices[i].Normal = XMFLOAT3(Model[i].NX, Model[i].NY, Model[i].NZ);

		Indices[i] = i;
	}

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

bool ModelClass::LoadModel(const char* InFilename)
{
	std::ifstream FIn;
	FIn.open(InFilename);

	if (FIn.fail())
		return false;

	char Input = 0;
	FIn.get(Input);
	while (Input != ':')
		FIn.get(Input);

	FIn >> VertexCount;
	IndexCount = VertexCount;

	Model = new ModelType[VertexCount];
	if (!Model)
		return false;

	FIn.get(Input);
	while (Input != ':')
		FIn.get(Input);
	FIn.get(Input);
	FIn.get(Input);

	for (int i = 0; i < VertexCount; ++i)
	{
		FIn >> Model[i].X >> Model[i].Y >> Model[i].Z;
		FIn >> Model[i].TU >> Model[i].TV;
		FIn >> Model[i].NX >> Model[i].NY >> Model[i].NZ;
	}

	FIn.close();
	return true;
}

void ModelClass::ReleaseModel()
{
	DX_DELETE_ARRAY(Model);
}
