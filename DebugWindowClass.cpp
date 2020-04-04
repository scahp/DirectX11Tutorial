#include "stdafx.h"
#include "DebugWindowClass.h"

bool DebugWindowClass::Initialize(ID3D11Device* InDevice, int InScreenWidth, int InScreenHeight, int InBitmapWidth, int InBitmapHeight)
{
	// 화면 크기를 저장하십시오.
	ScreenWidth = InScreenWidth;
	ScreenHeight = InScreenHeight;

	// 이 비트맵을 렌더링 할 픽셀의 크기를 저장합니다.
	BitmapWidth = InBitmapWidth;
	BitmapHeight = InBitmapHeight;

	// 이전 렌더링 위치를 음수로 초기화합니다.
	PreviousPosX = -1;
	PreviousPosY = -1;

	// 정점 및 인덱스 버퍼를 초기화합니다.
	return InitializeBuffers(InDevice);
}

void DebugWindowClass::Shutdown()
{
	// 버텍스 및 인덱스 버퍼를 종료합니다.
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
	// 정점 배열의 정점 수를 설정합니다.
	VertexCount = 6;

	// 인덱스 배열의 인덱스 수를 설정합니다.
	IndexCount = VertexCount;

	// 정점 배열을 만듭니다.
	VertexType* Vertices = new VertexType[VertexCount];
	if (!Vertices)
		return false;

	// 인덱스 배열을 만듭니다.
	unsigned long* Indices = new unsigned long[IndexCount];
	if (!Indices)
		return false;

	// 처음에는 정점 배열을 0으로 초기화합니다.
	memset(Vertices, 0, (sizeof(VertexType) * VertexCount));

	// 데이터로 인덱스 배열을로드합니다.
	for (int i = 0; i < IndexCount; i++)
		Indices[i] = i;

	// 정적 정점 버퍼의 설명을 설정한다.
	D3D11_BUFFER_DESC VertexBufferDesc;
	VertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	VertexBufferDesc.ByteWidth = sizeof(VertexType) * VertexCount;
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;

	// subresource 구조에 정점 데이터에 대한 포인터를 제공합니다.
	D3D11_SUBRESOURCE_DATA VertexData;
	VertexData.pSysMem = Vertices;
	VertexData.SysMemPitch = 0;
	VertexData.SysMemSlicePitch = 0;

	// 이제 정점 버퍼를 만듭니다.
	if (FAILED(InDevice->CreateBuffer(&VertexBufferDesc, &VertexData, &VertexBuffer)))
		return false;

	// 정적 인덱스 버퍼의 설명을 설정합니다.
	D3D11_BUFFER_DESC IndexBufferDesc;
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth = sizeof(unsigned long) * IndexCount;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;

	// 하위 리소스 구조에 인덱스 데이터에 대한 포인터를 제공합니다.
	D3D11_SUBRESOURCE_DATA IndexData;
	IndexData.pSysMem = Indices;
	IndexData.SysMemPitch = 0;
	IndexData.SysMemSlicePitch = 0;

	// 인덱스 버퍼를 만듭니다.
	if (FAILED(InDevice->CreateBuffer(&IndexBufferDesc, &IndexData, &IndexBuffer)))
		return false;

	// 이제 버텍스와 인덱스 버퍼가 생성되고로드 된 배열을 해제하십시오.
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
	//이 비트 맵을 렌더링 할 위치가 변경되지 않은 경우 정점 버퍼를 업데이트하지 마십시오.
	// 현재 올바른 매개 변수가 있습니다.
	if ((InPositionX == PreviousPosX) && (InPositionY == PreviousPosY))
		return true;

	// 변경된 경우 렌더링되는 위치를 업데이트합니다.
	PreviousPosX = InPositionX;
	PreviousPosY = InPositionY;

	// 비트 맵 왼쪽의 화면 좌표를 계산합니다.
	float Left = (float)((ScreenWidth / 2) * -1) + (float)InPositionX;

	// 비트 맵 오른쪽의 화면 좌표를 계산합니다.
	float Right = Left + (float)BitmapWidth;

	// 비트 맵 상단의 화면 좌표를 계산합니다.
	float Top = (float)(ScreenHeight / 2) - (float)InPositionY;

	// 비트 맵 아래쪽의 화면 좌표를 계산합니다.
	float Bottom = Top - (float)BitmapHeight;

	// 정점 배열을 만듭니다.
	VertexType* Vertices = new VertexType[VertexCount];
	if (!Vertices)
		return false;

	// 정점 배열에 데이터를로드합니다.
	// 첫 번째 삼각형.
	Vertices[0].Position = XMFLOAT3(Left, Top, 0.0f);  // Top Left.
	Vertices[0].Texture = XMFLOAT2(0.0f, 0.0f);

	Vertices[1].Position = XMFLOAT3(Right, Bottom, 0.0f);  // Bottom Right.
	Vertices[1].Texture = XMFLOAT2(1.0f, 1.0f);

	Vertices[2].Position = XMFLOAT3(Left, Bottom, 0.0f);  // Bottom Left.
	Vertices[2].Texture = XMFLOAT2(0.0f, 1.0f);

	// 두 번째 삼각형.
	Vertices[3].Position = XMFLOAT3(Left, Top, 0.0f);  // Top Left.
	Vertices[3].Texture = XMFLOAT2(0.0f, 0.0f);

	Vertices[4].Position = XMFLOAT3(Right, Top, 0.0f);  // Top Right.
	Vertices[4].Texture = XMFLOAT2(1.0f, 0.0f);

	Vertices[5].Position = XMFLOAT3(Right, Bottom, 0.0f);  // Bottom Right.
	Vertices[5].Texture = XMFLOAT2(1.0f, 1.0f);

	// 버텍스 버퍼를 쓸 수 있도록 잠급니다.
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	if (FAILED(InDeviceContext->Map(VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource)))
		return false;

	// 정점 버퍼의 데이터를 가리키는 포인터를 얻는다.
	VertexType* VerticesPtr = (VertexType*)MappedResource.pData;

	// 데이터를 정점 버퍼에 복사합니다.
	memcpy(VerticesPtr, (void*)Vertices, (sizeof(VertexType) * VertexCount));

	// 정점 버퍼의 잠금을 해제합니다.
	InDeviceContext->Unmap(VertexBuffer, 0);

	// 더 이상 필요하지 않은 꼭지점 배열을 해제합니다.
	delete[] Vertices;
	Vertices = 0;

	return true;
}

void DebugWindowClass::BindBuffers(ID3D11DeviceContext* InDeviceContext)
{
	// 정점 버퍼 보폭 및 오프셋을 설정합니다.
	unsigned int Stride = sizeof(VertexType);
	unsigned int Offset = 0;

	// 렌더링 할 수 있도록 입력 어셈블러에서 정점 버퍼를 활성으로 설정합니다.
	InDeviceContext->IASetVertexBuffers(0, 1, &VertexBuffer, &Stride, &Offset);

	// 렌더링 할 수 있도록 입력 어셈블러에서 인덱스 버퍼를 활성으로 설정합니다.
	InDeviceContext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// 이 꼭지점 버퍼에서 렌더링되어야하는 프리미티브 유형을 설정합니다.이 경우에는 삼각형입니다.
	InDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
