#include "stdafx.h"
#include "ColorShaderClass.h"

ColorShaderClass::ColorShaderClass()
{
}

ColorShaderClass::ColorShaderClass(const ColorShaderClass& InClass)
{
}


ColorShaderClass::~ColorShaderClass()
{
}

bool ColorShaderClass::Initialize(ID3D11Device* InDevice, HWND InHwnd)
{
	return InitializeShader(InDevice, InHwnd, TEXT("Shaders/Color.vs"), TEXT("Shaders/Color.ps"));
}

void ColorShaderClass::Shutdown()
{
	ShutdownShader();
}

bool ColorShaderClass::Render(ID3D11DeviceContext* InDeviceContext, int InIndexCount
	, const XMMATRIX& InWorldMatrix, const XMMATRIX& InViewMatrix, const XMMATRIX& InProjectionMatrix)
{
	if (!SetShaderParameters(InDeviceContext, InWorldMatrix, InViewMatrix, InProjectionMatrix))
		return false;

	RenderShader(InDeviceContext, InIndexCount);
	return true;
}

bool ColorShaderClass::InitializeShader(ID3D11Device* InDevice, HWND InHwnd, const WCHAR* InVSFilename, const WCHAR* InPSFilename)
{
	ID3D10Blob* ErrorMessage = nullptr;

	// Compile VS
	ID3D10Blob* VertexShaderBuffer = nullptr;
	if (FAILED(D3DCompileFromFile(InVSFilename, nullptr, nullptr, "ColorVertexShader", "vs_5_0"
		, D3D10_SHADER_ENABLE_STRICTNESS, 0, &VertexShaderBuffer, &ErrorMessage)))
	{
		if (ErrorMessage)
			OutputShaderErrorMessage(ErrorMessage, InHwnd, InVSFilename);
		else
			MessageBox(InHwnd, InVSFilename, TEXT("Missing Shader File"), MB_OK);

		return false;
	}

	ID3D10Blob* PixelShaderBuffer = nullptr;
	if (FAILED(D3DCompileFromFile(InPSFilename, nullptr, nullptr, "ColorPixelShader", "ps_5_0"
		, D3D10_SHADER_ENABLE_STRICTNESS, 0, &PixelShaderBuffer, &ErrorMessage)))
	{
		if (ErrorMessage)
			OutputShaderErrorMessage(ErrorMessage, InHwnd, InVSFilename);
		else
			MessageBox(InHwnd, InVSFilename, TEXT("Missing Shader File"), MB_OK);

		return false;
	}

	if (FAILED(InDevice->CreateVertexShader(VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), nullptr, &VertexShader)))
		return false;

	if (FAILED(InDevice->CreatePixelShader(PixelShaderBuffer->GetBufferPointer(), PixelShaderBuffer->GetBufferSize(), nullptr, &PixelShader)))
		return false;

	// 정점 입력 레이아웃 구조체 설정
	// 이 설정은 ModelClass와 쉐이더의 VertexType 구조와 일치해야 합니다.
	D3D11_INPUT_ELEMENT_DESC PolygonLayout[2];
	PolygonLayout[0].SemanticName = "POSITION";
	PolygonLayout[0].SemanticIndex = 0;
	PolygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	PolygonLayout[0].InputSlot = 0;
	PolygonLayout[0].AlignedByteOffset = 0;
	PolygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	PolygonLayout[0].InstanceDataStepRate = 0;

	PolygonLayout[1].SemanticName = "COLOR";
	PolygonLayout[1].SemanticIndex = 0;
	PolygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	PolygonLayout[1].InputSlot = 0;
	PolygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	PolygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	PolygonLayout[1].InstanceDataStepRate = 0;

	unsigned int NumElements = sizeof(PolygonLayout) / sizeof(PolygonLayout[0]);

	// 정점 입력 레이아웃을 만듬
	if (FAILED(InDevice->CreateInputLayout(PolygonLayout, NumElements, VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), &Layout)))
		return false;

	// 더이상 사용하지 않는 버퍼들 제거
	DX_RELEASE(VertexShaderBuffer);
	DX_RELEASE(PixelShaderBuffer);

	// 정점 쉐이더에 있는 행렬 상수 버퍼의 구조체 작성
	D3D11_BUFFER_DESC MatrixBufferDesc;
	MatrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	MatrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	MatrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	MatrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	MatrixBufferDesc.MiscFlags = 0;
	MatrixBufferDesc.StructureByteStride = 0;

	if (FAILED(InDevice->CreateBuffer(&MatrixBufferDesc, nullptr, &MatrixBuffer)))
		return false;

	return true;
}

void ColorShaderClass::ShutdownShader()
{
	DX_RELEASE(MatrixBuffer);
	DX_RELEASE(Layout);
	DX_RELEASE(PixelShader);
	DX_RELEASE(VertexShader);
}

void ColorShaderClass::OutputShaderErrorMessage(ID3D10Blob* InErrorMessage, HWND InHwnd, const WCHAR* InShaderFilename)
{
	OutputDebugStringA(reinterpret_cast<const char*>(InErrorMessage->GetBufferPointer()));

	DX_RELEASE(InErrorMessage);

	MessageBox(InHwnd, TEXT("Error Compiling Shader"), InShaderFilename, MB_OK);
}

bool ColorShaderClass::SetShaderParameters(ID3D11DeviceContext* InDeviceContext, const XMMATRIX& InWorldMatrix
	, const XMMATRIX& InViewMatrix, const XMMATRIX& InProjectionMatrix)
{
	// 행렬을 Transpose 하여 쉐이더에서 사용할 수 있게 합니다.
	auto WorldMatrix = XMMatrixTranspose(InWorldMatrix);
	auto ViewMatrix = XMMatrixTranspose(InViewMatrix);
	auto ProjectionMatrix = XMMatrixTranspose(InProjectionMatrix);

	D3D11_MAPPED_SUBRESOURCE MappedResource;
	if (FAILED(InDeviceContext->Map(MatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource)))
		return false;

	MatrixBufferType* DataPtr = (MatrixBufferType*)MappedResource.pData;
	DataPtr->World = WorldMatrix;
	DataPtr->View = ViewMatrix;
	DataPtr->Projection = ProjectionMatrix;

	InDeviceContext->Unmap(MatrixBuffer, 0);

	unsigned int BufferNumber = 0;
	InDeviceContext->VSSetConstantBuffers(BufferNumber, 1, &MatrixBuffer);	// 정점쉐이더에서의 상수 버퍼 위치 설정

	return true;
}

void ColorShaderClass::RenderShader(ID3D11DeviceContext* InDeviceContext, int InIndexCount)
{
	InDeviceContext->IASetInputLayout(Layout);

	InDeviceContext->VSSetShader(VertexShader, nullptr, 0);
	InDeviceContext->PSSetShader(PixelShader, nullptr, 0);

	InDeviceContext->DrawIndexed(InIndexCount, 0, 0);
}
