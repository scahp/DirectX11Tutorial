#include "stdafx.h"
#include "DebugWindowClass.h"

bool DebugWindowClass::Initialize(ID3D11Device* InDevice, int InScreenWidth, int InScreenHeight, int InBitmapWidth, int InBitmapHeight)
{
	// ȭ�� ũ�⸦ �����Ͻʽÿ�.
	ScreenWidth = InScreenWidth;
	ScreenHeight = InScreenHeight;

	// �� ��Ʈ���� ������ �� �ȼ��� ũ�⸦ �����մϴ�.
	BitmapWidth = InBitmapWidth;
	BitmapHeight = InBitmapHeight;

	// ���� ������ ��ġ�� ������ �ʱ�ȭ�մϴ�.
	PreviousPosX = -1;
	PreviousPosY = -1;

	// ���� �� �ε��� ���۸� �ʱ�ȭ�մϴ�.
	return InitializeBuffers(InDevice);
}

void DebugWindowClass::Shutdown()
{
	// ���ؽ� �� �ε��� ���۸� �����մϴ�.
	ShutdownBuffers();
}

bool DebugWindowClass::Bind(ID3D11DeviceContext* InDeviceContext, int InPositionX, int InPositionY)
{
	if (!UpdateBuffers(InDeviceContext, InPositionX, InPositionY))
		return false;

	BindBuffers(InDeviceContext);

	return true;
}

int DebugWindowClass::GetIndexCount()
{
	return IndexCount;
}

bool DebugWindowClass::InitializeBuffers(ID3D11Device* InDevice)
{
	// ���� �迭�� ���� ���� �����մϴ�.
	VertexCount = 6;

	// �ε��� �迭�� �ε��� ���� �����մϴ�.
	IndexCount = VertexCount;

	// ���� �迭�� ����ϴ�.
	VertexType* Vertices = new VertexType[VertexCount];
	if (!Vertices)
		return false;

	// �ε��� �迭�� ����ϴ�.
	unsigned long* Indices = new unsigned long[IndexCount];
	if (!Indices)
		return false;

	// ó������ ���� �迭�� 0���� �ʱ�ȭ�մϴ�.
	memset(Vertices, 0, (sizeof(VertexType) * VertexCount));

	// �����ͷ� �ε��� �迭���ε��մϴ�.
	for (int i = 0; i < IndexCount; i++)
		Indices[i] = i;

	// ���� ���� ������ ������ �����Ѵ�.
	D3D11_BUFFER_DESC VertexBufferDesc;
	VertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	VertexBufferDesc.ByteWidth = sizeof(VertexType) * VertexCount;
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;

	// subresource ������ ���� �����Ϳ� ���� �����͸� �����մϴ�.
	D3D11_SUBRESOURCE_DATA VertexData;
	VertexData.pSysMem = Vertices;
	VertexData.SysMemPitch = 0;
	VertexData.SysMemSlicePitch = 0;

	// ���� ���� ���۸� ����ϴ�.
	if (FAILED(InDevice->CreateBuffer(&VertexBufferDesc, &VertexData, &VertexBuffer)))
		return false;

	// ���� �ε��� ������ ������ �����մϴ�.
	D3D11_BUFFER_DESC IndexBufferDesc;
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth = sizeof(unsigned long) * IndexCount;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;

	// ���� ���ҽ� ������ �ε��� �����Ϳ� ���� �����͸� �����մϴ�.
	D3D11_SUBRESOURCE_DATA IndexData;
	IndexData.pSysMem = Indices;
	IndexData.SysMemPitch = 0;
	IndexData.SysMemSlicePitch = 0;

	// �ε��� ���۸� ����ϴ�.
	if (FAILED(InDevice->CreateBuffer(&IndexBufferDesc, &IndexData, &IndexBuffer)))
		return false;

	// ���� ���ؽ��� �ε��� ���۰� �����ǰ�ε� �� �迭�� �����Ͻʽÿ�.
	delete[] Vertices;
	Vertices = 0;

	delete[] Indices;
	Indices = 0;

	return true;
}

void DebugWindowClass::ShutdownBuffers()
{
	DX_RELEASE(VertexBuffer);
	DX_RELEASE(IndexBuffer);
}

bool DebugWindowClass::UpdateBuffers(ID3D11DeviceContext* InDeviceContext, int InPositionX, int InPositionY)
{
	//�� ��Ʈ ���� ������ �� ��ġ�� ������� ���� ��� ���� ���۸� ������Ʈ���� ���ʽÿ�.
	// ���� �ùٸ� �Ű� ������ �ֽ��ϴ�.
	if ((InPositionX == PreviousPosX) && (InPositionY == PreviousPosY))
		return true;

	// ����� ��� �������Ǵ� ��ġ�� ������Ʈ�մϴ�.
	PreviousPosX = InPositionX;
	PreviousPosY = InPositionY;

	// ��Ʈ �� ������ ȭ�� ��ǥ�� ����մϴ�.
	float Left = (float)((ScreenWidth / 2) * -1) + (float)InPositionX;

	// ��Ʈ �� �������� ȭ�� ��ǥ�� ����մϴ�.
	float Right = Left + (float)BitmapWidth;

	// ��Ʈ �� ����� ȭ�� ��ǥ�� ����մϴ�.
	float Top = (float)(ScreenHeight / 2) - (float)InPositionY;

	// ��Ʈ �� �Ʒ����� ȭ�� ��ǥ�� ����մϴ�.
	float Bottom = Top - (float)BitmapHeight;

	// ���� �迭�� ����ϴ�.
	VertexType* Vertices = new VertexType[VertexCount];
	if (!Vertices)
		return false;

	// ���� �迭�� �����͸��ε��մϴ�.
	// ù ��° �ﰢ��.
	Vertices[0].Position = XMFLOAT3(Left, Top, 0.0f);  // Top Left.
	Vertices[0].Texture = XMFLOAT2(0.0f, 0.0f);

	Vertices[1].Position = XMFLOAT3(Right, Bottom, 0.0f);  // Bottom Right.
	Vertices[1].Texture = XMFLOAT2(1.0f, 1.0f);

	Vertices[2].Position = XMFLOAT3(Left, Bottom, 0.0f);  // Bottom Left.
	Vertices[2].Texture = XMFLOAT2(0.0f, 1.0f);

	// �� ��° �ﰢ��.
	Vertices[3].Position = XMFLOAT3(Left, Top, 0.0f);  // Top Left.
	Vertices[3].Texture = XMFLOAT2(0.0f, 0.0f);

	Vertices[4].Position = XMFLOAT3(Right, Top, 0.0f);  // Top Right.
	Vertices[4].Texture = XMFLOAT2(1.0f, 0.0f);

	Vertices[5].Position = XMFLOAT3(Right, Bottom, 0.0f);  // Bottom Right.
	Vertices[5].Texture = XMFLOAT2(1.0f, 1.0f);

	// ���ؽ� ���۸� �� �� �ֵ��� ��޴ϴ�.
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	if (FAILED(InDeviceContext->Map(VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource)))
		return false;

	// ���� ������ �����͸� ����Ű�� �����͸� ��´�.
	VertexType* VerticesPtr = (VertexType*)MappedResource.pData;

	// �����͸� ���� ���ۿ� �����մϴ�.
	memcpy(VerticesPtr, (void*)Vertices, (sizeof(VertexType) * VertexCount));

	// ���� ������ ����� �����մϴ�.
	InDeviceContext->Unmap(VertexBuffer, 0);

	// �� �̻� �ʿ����� ���� ������ �迭�� �����մϴ�.
	delete[] Vertices;
	Vertices = 0;

	return true;
}

void DebugWindowClass::BindBuffers(ID3D11DeviceContext* InDeviceContext)
{
	// ���� ���� ���� �� �������� �����մϴ�.
	unsigned int Stride = sizeof(VertexType);
	unsigned int Offset = 0;

	// ������ �� �� �ֵ��� �Է� ��������� ���� ���۸� Ȱ������ �����մϴ�.
	InDeviceContext->IASetVertexBuffers(0, 1, &VertexBuffer, &Stride, &Offset);

	// ������ �� �� �ֵ��� �Է� ��������� �ε��� ���۸� Ȱ������ �����մϴ�.
	InDeviceContext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// �� ������ ���ۿ��� �������Ǿ���ϴ� ������Ƽ�� ������ �����մϴ�.�� ��쿡�� �ﰢ���Դϴ�.
	InDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
