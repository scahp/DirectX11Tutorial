#include "stdafx.h"
#include "BitmapClass.h"
#include "TextureClass.h"

BitmapClass::BitmapClass()
{

}

BitmapClass::BitmapClass(const BitmapClass& InOther)
{

}

BitmapClass::~BitmapClass()
{

}

bool BitmapClass::Initialize(ID3D11Device* InDevice, ID3D11DeviceContext* InDeviceContext, int InScreenWidth, int InScreenHeight, const char* InTextureFilename, int InBitmapWidth, int InBitmapHeight)
{
	ScreenWidth = InScreenWidth;
	ScreenHeight = InScreenHeight;

	BitmapWidth = InBitmapWidth;
	BitmapHeight = InBitmapHeight;

	PreviousPosX = -1;
	PreviousPosY = -1;

	if (!InitializeBuffers(InDevice))
		return false;

	return LoadTexture(InDevice, InDeviceContext, InTextureFilename);
}

void BitmapClass::Shutdown()
{
	ReleaseTexture();
	ShutdownBuffers();
}

bool BitmapClass::Render(ID3D11DeviceContext* InDeviceContext, int InPositionX, int InPositionY)
{
	if (!UpdateBuffers(InDeviceContext, InPositionX, InPositionY))
		return false;

	RenderBuffers(InDeviceContext);

	return true;
}

int BitmapClass::GetIndexCount()
{
	return IndexCount;
}

ID3D11ShaderResourceView* BitmapClass::GetTexture()
{
	return Texture->GetTexture();
}

bool BitmapClass::InitializeBuffers(ID3D11Device* InDevice)
{
	IndexCount = VertexCount = 6;

	VertexType* Vertices = new VertexType[VertexCount];
	if (!Vertices)
		return false;

	memset(Vertices, 0, sizeof(VertexType) * VertexCount);

	unsigned long* Indices = new unsigned long[IndexCount];
	if (!Indices)
		return false;

	for (int i = 0; i < IndexCount; ++i)
		Indices[i] = i;

	D3D11_BUFFER_DESC VertexBufferDesc;
	VertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	VertexBufferDesc.ByteWidth = sizeof(VertexType) * VertexCount;
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA VertexData;
	VertexData.pSysMem = Vertices;
	VertexData.SysMemPitch = 0;
	VertexData.SysMemSlicePitch = 0;

	if (FAILED(InDevice->CreateBuffer(&VertexBufferDesc, &VertexData, &VertexBuffer)))
		return false;

	D3D11_BUFFER_DESC IndexBufferDesc;
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth = sizeof(unsigned long) * IndexCount;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA IndexData;
	IndexData.pSysMem = Indices;
	IndexData.SysMemPitch = 0;
	IndexData.SysMemSlicePitch = 0;

	if (FAILED(InDevice->CreateBuffer(&IndexBufferDesc, &IndexData, &IndexBuffer)))
		return false;

	DX_DELETE_ARRAY(Vertices);
	DX_DELETE_ARRAY(Indices);
	
	return true;
}

void BitmapClass::ShutdownBuffers()
{
	DX_RELEASE(IndexBuffer);
	DX_RELEASE(VertexBuffer);
}

bool BitmapClass::UpdateBuffers(ID3D11DeviceContext* InDeviceContext, int InPositionX, int InPositionY)
{
	float Left, Right, Top, Bottom;
	VertexType* Vertices = nullptr;
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	VertexType* VerticesPtr;
	HRESULT Result;

	// 이 비트맵을 렌더링 할 위치가 변경되지 않은 경우 정점 버퍼를 업데이트 하지 마십시오.
	if ((InPositionX == PreviousPosX) && (InPositionY == PreviousPosY))
		return true;

	PreviousPosX = InPositionX;
	PreviousPosY = InPositionY;

	Left = (float)((ScreenWidth / 2) * -1) + (float)InPositionX;
	Right = Left + (float)BitmapWidth;
	Top = (float)(ScreenHeight / 2) - (float)InPositionY;
	Bottom = Top - (float)BitmapHeight;

	Vertices = new VertexType[VertexCount];
	if (!Vertices)
		return false;

	Vertices[0].Position = XMFLOAT3(Left, Top, 0.0f);  // Top left.
	Vertices[0].Texture = XMFLOAT2(0.0f, 0.0f);

	Vertices[1].Position = XMFLOAT3(Right, Bottom, 0.0f);  // Bottom right.
	Vertices[1].Texture = XMFLOAT2(1.0f, 1.0f);

	Vertices[2].Position = XMFLOAT3(Left, Bottom, 0.0f);  // Bottom left.
	Vertices[2].Texture = XMFLOAT2(0.0f, 1.0f);

	Vertices[3].Position = XMFLOAT3(Left, Top, 0.0f);  // Top left.
	Vertices[3].Texture = XMFLOAT2(0.0f, 0.0f);

	Vertices[4].Position = XMFLOAT3(Right, Top, 0.0f);  // Top right.
	Vertices[4].Texture = XMFLOAT2(1.0f, 0.0f);

	Vertices[5].Position = XMFLOAT3(Right, Bottom, 0.0f);  // Bottom right.
	Vertices[5].Texture = XMFLOAT2(1.0f, 1.0f);

	Result = InDeviceContext->Map(VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	if (FAILED(Result))
		return false;

	VerticesPtr = (VertexType*)MappedResource.pData;
	memcpy(VerticesPtr, (void*)Vertices, sizeof(VertexType) * VertexCount);

	InDeviceContext->Unmap(VertexBuffer, 0);

	DX_DELETE_ARRAY(Vertices);
	return true;
}

void BitmapClass::RenderBuffers(ID3D11DeviceContext* InDeviceContext)
{
	UINT Stride = sizeof(VertexType);
	UINT Offset = 0;

	InDeviceContext->IASetVertexBuffers(0, 1, &VertexBuffer, &Stride, &Offset);
	InDeviceContext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	InDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool BitmapClass::LoadTexture(ID3D11Device* InDevice, ID3D11DeviceContext* InDeviceContext, const char* InFilename)
{
	Texture = new TextureClass();
	if (!Texture)
		return false;

	return Texture->Initialize(InDevice, InDeviceContext, InFilename);
}

void BitmapClass::ReleaseTexture()
{
	if (Texture)
	{
		Texture->Shutdown();
		DX_DELETE(Texture);
	}
}