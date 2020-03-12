#include "stdafx.h"
#include "LightShaderClass.h"

LightShaderClass::LightShaderClass()
{

}

LightShaderClass::LightShaderClass(const LightShaderClass&)
{

}

LightShaderClass::~LightShaderClass()
{

}

bool LightShaderClass::Initialize(ID3D11Device* InDevice, HWND InHwnd)
{
	return InitializeShader(InDevice, InHwnd, TEXT("Shaders/Light.vs"), TEXT("Shaders/Light.ps"));
}

void LightShaderClass::Shutdown()
{
	ShutdownShader();
}

bool LightShaderClass::Render(ID3D11DeviceContext* InDeviceContext, int InIndexCount, XMMATRIX InWorldMatrix
	, XMMATRIX InViewMatrix, XMMATRIX InProjectionMatrix, ID3D11ShaderResourceView* InTexture, XMFLOAT3 InLightDirection
	, XMFLOAT4 InAmbientColor, XMFLOAT4 InDiffuseColor, XMFLOAT3 InCameraPosition, XMFLOAT4 InSpecularColor, float InSpecularPower)
{
	if (!SetShaderParameters(InDeviceContext, InWorldMatrix, InViewMatrix, InProjectionMatrix, InTexture
		, InLightDirection, InAmbientColor, InDiffuseColor, InCameraPosition, InSpecularColor, InSpecularPower))
	{
		return false;
	}

	RenderShader(InDeviceContext, InIndexCount);
	return true;
}

bool LightShaderClass::InitializeShader(ID3D11Device* InDevice, HWND InHwnd, const WCHAR* InVSFilename, const WCHAR* InPSFilename)
{
	HRESULT Result;
	ID3D10Blob* ErrorMessage = nullptr;

	ID3D10Blob* VertexShaderBuffer = nullptr;
	Result = D3DCompileFromFile(InVSFilename, nullptr, nullptr, "LightVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0
							, &VertexShaderBuffer, &ErrorMessage);

	if (FAILED(Result))
	{
		if (ErrorMessage)
		{
			OutputShaderErrorMessage(ErrorMessage, InHwnd, InVSFilename);
		}
		else
		{
			MessageBox(InHwnd, InVSFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	ID3D10Blob* PixelShaderBuffer = nullptr;
	Result = D3DCompileFromFile(InPSFilename, nullptr, nullptr, "LightPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0
							, &PixelShaderBuffer, &ErrorMessage);

	if (FAILED(Result))
	{
		if (ErrorMessage)
		{
			OutputShaderErrorMessage(ErrorMessage, InHwnd, InPSFilename);
		}
		else
		{
			MessageBox(InHwnd, InPSFilename, L"Missing Shader File", MB_OK);
		}
		return false;
	}

	Result = InDevice->CreateVertexShader(VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize()
									, nullptr, &VertexShader);
	if (FAILED(Result))
		return false;

	Result = InDevice->CreatePixelShader(PixelShaderBuffer->GetBufferPointer(), PixelShaderBuffer->GetBufferSize()
									, nullptr, &PixelShader);
	if (FAILED(Result))
		return false;

	D3D11_INPUT_ELEMENT_DESC PolygonLayout[3];
	PolygonLayout[0].SemanticName = "POSITION";
	PolygonLayout[0].SemanticIndex = 0;
	PolygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	PolygonLayout[0].InputSlot = 0;
	PolygonLayout[0].AlignedByteOffset = 0;
	PolygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	PolygonLayout[0].InstanceDataStepRate = 0;

	PolygonLayout[1].SemanticName = "TEXCOORD";
	PolygonLayout[1].SemanticIndex = 0;
	PolygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	PolygonLayout[1].InputSlot = 0;
	PolygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	PolygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	PolygonLayout[1].InstanceDataStepRate = 0;

	PolygonLayout[2].SemanticName = "NORMAL";
	PolygonLayout[2].SemanticIndex = 0;
	PolygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	PolygonLayout[2].InputSlot = 0;
	PolygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	PolygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	PolygonLayout[2].InstanceDataStepRate = 0;

	UINT NumElements = sizeof(PolygonLayout) / sizeof(PolygonLayout[0]);

	Result = InDevice->CreateInputLayout(PolygonLayout, NumElements, VertexShaderBuffer->GetBufferPointer()
								, VertexShaderBuffer->GetBufferSize(), &Layout);

	VertexShaderBuffer->Release();
	VertexShaderBuffer = 0;

	PixelShaderBuffer->Release();
	PixelShaderBuffer = 0;

	D3D11_SAMPLER_DESC SamplerDesc;
	SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.MipLODBias = 0;
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

	D3D11_BUFFER_DESC CameraBufferDesc;
	CameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	CameraBufferDesc.ByteWidth = sizeof(CameraBufferType);
	CameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	CameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	CameraBufferDesc.MiscFlags = 0;
	CameraBufferDesc.StructureByteStride = 0;

	Result = InDevice->CreateBuffer(&CameraBufferDesc, nullptr, &CameraBuffer);
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

void LightShaderClass::ShutdownShader()
{
	DX_RELEASE(LightBuffer);
	DX_RELEASE(CameraBuffer);
	DX_RELEASE(MatrixBuffer);
	DX_RELEASE(SamplerState);
	DX_RELEASE(Layout);
	DX_RELEASE(PixelShader);
	DX_RELEASE(VertexShader);
}

void LightShaderClass::OutputShaderErrorMessage(ID3D10Blob* InErrorMessage, HWND InHwnd, const WCHAR* InShaderFilename)
{
	OutputDebugStringA(reinterpret_cast<const char*>(InErrorMessage->GetBufferPointer()));

	InErrorMessage->Release();
	InErrorMessage = 0;

	MessageBox(InHwnd, L"Error compiling shader.", InShaderFilename, MB_OK);
}

bool LightShaderClass::SetShaderParameters(ID3D11DeviceContext* InDeviceContext, XMMATRIX InWorldMatrix, XMMATRIX InViewMatrix
	, XMMATRIX InProjectionMatrix, ID3D11ShaderResourceView* InTexture, XMFLOAT3 InLightDirection, XMFLOAT4 InAmbientColor, XMFLOAT4 InDiffuseColor
	, XMFLOAT3 InCameraPosition, XMFLOAT4 InSpecularColor, float InSpecularPower)
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

	InDeviceContext->PSSetShaderResources(0, 1, &InTexture);

	if (FAILED(InDeviceContext->Map(CameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource)))
		return false;

	CameraBufferType* DataPtr3 = (CameraBufferType*)MappedResource.pData;
	DataPtr3->CameraPosition = InCameraPosition;
	DataPtr3->Padding = 0;

	InDeviceContext->Unmap(CameraBuffer, 0);

	BufferNumber = 1;

	InDeviceContext->VSSetConstantBuffers(BufferNumber, 1, &CameraBuffer);

	if (FAILED(InDeviceContext->Map(LightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource)))
		return false;

	LightBufferType* DataPtr2 = (LightBufferType*)MappedResource.pData;

	DataPtr2->AmbientColor = InAmbientColor;
	DataPtr2->DiffuseColor = InDiffuseColor;
	DataPtr2->LightDirection = InLightDirection;
	DataPtr2->SpecularPower = InSpecularPower;
	DataPtr2->SpecularColor = InSpecularColor;

	InDeviceContext->Unmap(LightBuffer, 0);

	BufferNumber = 0;
	InDeviceContext->PSSetConstantBuffers(BufferNumber, 1, &LightBuffer);
	return true;
}

void LightShaderClass::RenderShader(ID3D11DeviceContext* InDeviceContext, int InIndexCount)
{
	InDeviceContext->IASetInputLayout(Layout);
	InDeviceContext->VSSetShader(VertexShader, nullptr, 0);
	InDeviceContext->PSSetShader(PixelShader, nullptr, 0);
	InDeviceContext->DrawIndexed(InIndexCount, 0, 0);
}


