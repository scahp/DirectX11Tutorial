#include "stdafx.h"
#include "BumpMapShaderClass.h"

bool BumpMapShaderClass::Initialize(ID3D11Device* InDevice, HWND InHwnd)
{
	return InitializeShader(InDevice, InHwnd, TEXT("Shaders/Bumpmap.vs"), TEXT("Shaders/Bumpmap.ps"));
}

void BumpMapShaderClass::Shutdown()
{
	ShutdownShader();
}

bool BumpMapShaderClass::Render(ID3D11DeviceContext* InDeviceContext, int InIndexCount, XMMATRIX InWorldMatrix,
	XMMATRIX InViewMatrix, XMMATRIX InProjectionMatrix, ID3D11ShaderResourceView** InTextureArray,
	XMFLOAT3 InLightDirection, XMFLOAT4 InDiffuseColor)
{
	if (!SetShaderParameters(InDeviceContext, InWorldMatrix, InViewMatrix, InProjectionMatrix, InTextureArray, InLightDirection, InDiffuseColor))
		return false;

	RenderShader(InDeviceContext, InIndexCount);

	return true;
}

bool BumpMapShaderClass::InitializeShader(ID3D11Device* InDevice, HWND InHwnd, const WCHAR* InVSFilename, const WCHAR* InPSFilename)
{
	HRESULT Result;
	ID3D10Blob* ErrorMessage = nullptr;

	ID3D10Blob* VertexShaderBuffer = nullptr;
	Result = D3DCompileFromFile(InVSFilename, nullptr, nullptr, "BumpMapVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS,
		0, &VertexShaderBuffer, &ErrorMessage);

	if (FAILED(Result))
	{
		if (ErrorMessage)
			OutputShaderErrorMessage(ErrorMessage, InHwnd, InVSFilename);
		else
			MessageBox(InHwnd, InVSFilename, TEXT("Missing Shader File"), MB_OK);

		return false;
	}

	ID3D10Blob* PixelShaderBuffer = nullptr;
	Result = D3DCompileFromFile(InPSFilename, nullptr, nullptr, "BumpMapPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS,
		0, &PixelShaderBuffer, &ErrorMessage);

	if (FAILED(Result))
	{
		if (ErrorMessage)
			OutputShaderErrorMessage(ErrorMessage, InHwnd, InPSFilename);
		else
			MessageBox(InHwnd, InPSFilename, TEXT("Missing Shader File"), MB_OK);

		return false;
	}

	Result = InDevice->CreateVertexShader(VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), nullptr,
		&VertexShader);
	if (FAILED(Result))
		return false;

	Result = InDevice->CreatePixelShader(PixelShaderBuffer->GetBufferPointer(), PixelShaderBuffer->GetBufferSize(), nullptr,
		&PixelShader);
	if (FAILED(Result))
		return false;

	D3D11_INPUT_ELEMENT_DESC PolygonLyaout[5];
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

	PolygonLyaout[2].SemanticName = "NORMAL";
	PolygonLyaout[2].SemanticIndex = 0;
	PolygonLyaout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	PolygonLyaout[2].InputSlot = 0;
	PolygonLyaout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	PolygonLyaout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	PolygonLyaout[2].InstanceDataStepRate = 0;

	PolygonLyaout[3].SemanticName = "TANGENT";
	PolygonLyaout[3].SemanticIndex = 0;
	PolygonLyaout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	PolygonLyaout[3].InputSlot = 0;
	PolygonLyaout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	PolygonLyaout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	PolygonLyaout[3].InstanceDataStepRate = 0;

	PolygonLyaout[4].SemanticName = "BINORMAL";
	PolygonLyaout[4].SemanticIndex = 0;
	PolygonLyaout[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	PolygonLyaout[4].InputSlot = 0;
	PolygonLyaout[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	PolygonLyaout[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	PolygonLyaout[4].InstanceDataStepRate = 0;

	UINT NumElement = sizeof(PolygonLyaout) / sizeof(PolygonLyaout[0]);

	Result = InDevice->CreateInputLayout(PolygonLyaout, NumElement, VertexShaderBuffer->GetBufferPointer(),
		VertexShaderBuffer->GetBufferSize(), &Layout);
	if (FAILED(Result))
		return false;

	DX_RELEASE(VertexShaderBuffer);
	DX_RELEASE(PixelShaderBuffer);

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

	Result = InDevice->CreateSamplerState(&SamplerDesc, &SamplerState);
	if (FAILED(Result))
		return false;

	D3D11_BUFFER_DESC LightBufferDesc;
	LightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	LightBufferDesc.ByteWidth = sizeof(LightBufferType);
	LightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	LightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	LightBufferDesc.MiscFlags = 0;
	LightBufferDesc.StructureByteStride = 0;

	Result = InDevice->CreateBuffer(&LightBufferDesc, nullptr, &LightBuffer);
	if (FAILED(Result))
		return false;

	return true;
}

void BumpMapShaderClass::ShutdownShader()
{
	DX_RELEASE(LightBuffer);
	DX_RELEASE(SamplerState);
	DX_RELEASE(MatrixBuffer);
	DX_RELEASE(Layout);
	DX_RELEASE(PixelShader);
	DX_RELEASE(VertexShader);
}

void BumpMapShaderClass::OutputShaderErrorMessage(ID3D10Blob* InErrorMessage, HWND InHwnd, const WCHAR* InShaderFilename)
{
	OutputDebugStringA(reinterpret_cast<const char*>(InErrorMessage->GetBufferPointer()));

	DX_RELEASE(InErrorMessage);

	MessageBox(InHwnd, TEXT("Error Compiling Shader."), InShaderFilename, MB_OK);
}

bool BumpMapShaderClass::SetShaderParameters(ID3D11DeviceContext* InDeviceContext, XMMATRIX InWorldMatrix, XMMATRIX InViewMatrix,
	XMMATRIX InProjectionMatrix, ID3D11ShaderResourceView** InTextureArray, XMFLOAT3 InLightDirection, XMFLOAT4 InDiffuseColor)
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

	if (FAILED(InDeviceContext->Map(LightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource)))
		return false;

	LightBufferType* DataPtr2 = (LightBufferType*)MappedResource.pData;

	DataPtr2->DiffuseColor = InDiffuseColor;
	DataPtr2->LightDirection = InLightDirection;

	InDeviceContext->Unmap(LightBuffer, 0);

	BufferNumber = 0;

	InDeviceContext->PSSetConstantBuffers(BufferNumber, 1, &LightBuffer);

	return true;
}

void BumpMapShaderClass::RenderShader(ID3D11DeviceContext* InDeviceContext, int InIndexCount)
{
	InDeviceContext->IASetInputLayout(Layout);

	InDeviceContext->VSSetShader(VertexShader, nullptr, 0);
	InDeviceContext->PSSetShader(PixelShader, nullptr, 0);

	InDeviceContext->PSSetSamplers(0, 1, &SamplerState);

	InDeviceContext->DrawIndexed(InIndexCount, 0, 0);
}