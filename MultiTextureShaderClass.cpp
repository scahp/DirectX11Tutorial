#include "stdafx.h"
#include "MultiTextureShaderClass.h"

#include <fstream>

bool MultiTextureShaderClass::Initialize(ID3D11Device* InDevice, HWND InHwnd)
{
	return InitializeShader(InDevice, InHwnd, TEXT("Shaders/MultiTexture.vs"), TEXT("Shaders/MultiTexture.ps"));
}

void MultiTextureShaderClass::Shutdown()
{
	ShutdownShader();
}

bool MultiTextureShaderClass::Render(ID3D11DeviceContext* InDeviceContext, int InIndexCount, XMMATRIX InWorldMatrix
	, XMMATRIX InViewMatrix, XMMATRIX InProjectionMatrix, ID3D11ShaderResourceView** InTextureArray)
{
	if (!SetShaderParameters(InDeviceContext, InWorldMatrix, InViewMatrix, InProjectionMatrix, InTextureArray))
		return false;

	RenderShader(InDeviceContext, InIndexCount);

	return true;
}

bool MultiTextureShaderClass::InitializeShader(ID3D11Device* InDevice, HWND InHwnd, const WCHAR* InVSFilename, const WCHAR* InPSFilename)
{
	ID3D10Blob* ErrorMessage = nullptr;

	ID3D10Blob* VertexShaderBuffer = nullptr;
	HRESULT Result = D3DCompileFromFile(InVSFilename, nullptr, nullptr, "MultiTextureVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0
		, &VertexShaderBuffer, &ErrorMessage);

	if (FAILED(Result))
	{
		if (ErrorMessage)
			OutputShaderErrorMessage(ErrorMessage, InHwnd, InVSFilename);
		else
			MessageBox(InHwnd, InVSFilename, TEXT("Missing Shader File"), MB_OK);

		return false;
	}

	ID3D10Blob* PixelShaderBuffer = nullptr;
	Result = D3DCompileFromFile(InPSFilename, nullptr, nullptr, "MultiTexturePixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0
		, &PixelShaderBuffer, &ErrorMessage);
	if (FAILED(Result))
	{
		if (ErrorMessage)
			OutputShaderErrorMessage(ErrorMessage, InHwnd, InPSFilename);
		else
			MessageBox(InHwnd, InPSFilename, TEXT("Missing Shader File"), MB_OK);

		return false;
	}

	Result = InDevice->CreateVertexShader(VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), nullptr, &VertexShader);
	if (FAILED(Result))
		return false;

	Result = InDevice->CreatePixelShader(PixelShaderBuffer->GetBufferPointer(), PixelShaderBuffer->GetBufferSize(), nullptr, &PixelShader);
	if (FAILED(Result))
		return false;

	D3D11_INPUT_ELEMENT_DESC PolygonLyaout[2];
	PolygonLyaout[0].SemanticName = "POSITION";
	PolygonLyaout[0].SemanticIndex = 0;
	PolygonLyaout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	PolygonLyaout[0].InputSlot = 0;
	PolygonLyaout[0].AlignedByteOffset = 0;
	PolygonLyaout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	PolygonLyaout[0].InstanceDataStepRate = 0;

	PolygonLyaout[1].SemanticName = "TEXCOORD";
	PolygonLyaout[1].SemanticIndex = 0;
	PolygonLyaout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	PolygonLyaout[1].InputSlot = 0;
	PolygonLyaout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	PolygonLyaout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	PolygonLyaout[1].InstanceDataStepRate = 0;

	unsigned int NumElements = sizeof(PolygonLyaout) / sizeof(PolygonLyaout[0]);

	Result = InDevice->CreateInputLayout(PolygonLyaout, NumElements, VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), &Layout);
	if (FAILED(Result))
		return false;

	VertexShaderBuffer->Release();
	VertexShaderBuffer = nullptr;

	PixelShaderBuffer->Release();
	PixelShaderBuffer = nullptr;

	D3D11_BUFFER_DESC MatrixBufferDesc;
	MatrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	MatrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	MatrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	MatrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	MatrixBufferDesc.MiscFlags = 0;
	MatrixBufferDesc.StructureByteStride = 0;

	Result = InDevice->CreateBuffer(&MatrixBufferDesc, nullptr, &MatrixBuffer);
	if (FAILED(Result))
		return false;

	D3D11_SAMPLER_DESC SamplerDesc;
	SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.MipLODBias = 0.0f;
	SamplerDesc.MaxAnisotropy = 1;
	SamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	SamplerDesc.BorderColor[0] = 0;
	SamplerDesc.BorderColor[1] = 0;
	SamplerDesc.BorderColor[2] = 0;
	SamplerDesc.BorderColor[3] = 0;
	SamplerDesc.MinLOD = 0;
	SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	Result = InDevice->CreateSamplerState(&SamplerDesc, &SampleState);
	if (FAILED(Result))
		return false;

	return true;
}

void MultiTextureShaderClass::ShutdownShader()
{
	DX_RELEASE(SampleState);
	DX_RELEASE(MatrixBuffer);
	DX_RELEASE(Layout);
	DX_RELEASE(PixelShader);
	DX_RELEASE(VertexShader);
}

void MultiTextureShaderClass::OutputShaderErrorMessage(ID3D10Blob* InErrorMessage, HWND InHwnd, const WCHAR* InShaderFilename)
{
	OutputDebugStringA(reinterpret_cast<const char*>(InErrorMessage->GetBufferPointer()));

	DX_RELEASE(InErrorMessage);

	MessageBox(InHwnd, TEXT("Error Compile Shader."), InShaderFilename, MB_OK);
}

bool MultiTextureShaderClass::SetShaderParameters(ID3D11DeviceContext* InDeviceContext, XMMATRIX InWorldMatrix, XMMATRIX InViewMatrix
	, XMMATRIX InProjectionMatrix, ID3D11ShaderResourceView** InTextureArray)
{
	InWorldMatrix = XMMatrixTranspose(InWorldMatrix);
	InViewMatrix = XMMatrixTranspose(InViewMatrix);
	InProjectionMatrix = XMMatrixTranspose(InProjectionMatrix);

	D3D11_MAPPED_SUBRESOURCE MappedResource;
	if (FAILED(InDeviceContext->Map(MatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource)))
		return false;

	MatrixBufferType* DataPtr = (MatrixBufferType*)MappedResource.pData;

	DataPtr->World = InWorldMatrix;
	DataPtr->View = InViewMatrix;
	DataPtr->Projection = InProjectionMatrix;

	InDeviceContext->Unmap(MatrixBuffer, 0);

	unsigned int BufferNumber = 0;

	InDeviceContext->VSSetConstantBuffers(BufferNumber, 1, &MatrixBuffer);
	InDeviceContext->PSSetShaderResources(0, 2, InTextureArray);
	return true;
}

void MultiTextureShaderClass::RenderShader(ID3D11DeviceContext* InDeviceContext, int InIndexCount)
{
	InDeviceContext->IASetInputLayout(Layout);
	InDeviceContext->VSSetShader(VertexShader, nullptr, 0);
	InDeviceContext->PSSetShader(PixelShader, nullptr, 0);
	InDeviceContext->PSSetSamplers(0, 1, &SampleState);
	InDeviceContext->DrawIndexed(InIndexCount, 0, 0);
}