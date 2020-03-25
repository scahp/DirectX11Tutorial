#pragma once

class BumpMapShaderClass
{
private:
	struct MatrixBufferType
	{
		XMMATRIX World;
		XMMATRIX View;
		XMMATRIX Projection;
	};

	struct LightBufferType
	{
		XMFLOAT4 DiffuseColor;
		XMFLOAT3 LightDirection;
		float Padding;
	};

public:
	bool Initialize(ID3D11Device* InDevice, HWND InHwnd);
	bool InitializeShader(ID3D11Device* InDevice, HWND InHwnd, const WCHAR* InVSFilename, const WCHAR* InPSFilename);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* InErrorMessage, HWND InHwnd, const WCHAR* InShaderFilename);
	bool SetShaderParameters(ID3D11DeviceContext* InDeviceContext, XMMATRIX InWorldMatrix, XMMATRIX InViewMatrix, XMMATRIX InProjectionMatrix, ID3D11ShaderResourceView** InTextureArray, XMFLOAT3 InLightDirection, XMFLOAT4 InDiffuseColor);
	void RenderShader(ID3D11DeviceContext* InDeviceContext, int InIndexCount);
	void Shutdown();
	bool Render(ID3D11DeviceContext* InDeviceContext, int InIndexCount, XMMATRIX InWorldMatrix, XMMATRIX InViewMatrix, XMMATRIX InProjectionMatrix, ID3D11ShaderResourceView** InTextureArray, XMFLOAT3 InLightDirection, XMFLOAT4 InDiffuseColor);
private:
	ID3D11VertexShader* VertexShader = nullptr;
	ID3D11PixelShader* PixelShader = nullptr;
	ID3D11InputLayout* Layout = nullptr;
	ID3D11Buffer* MatrixBuffer = nullptr;
	ID3D11SamplerState* SamplerState = nullptr;
	ID3D11Buffer* LightBuffer = nullptr;
};

