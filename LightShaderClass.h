#pragma once

class LightShaderClass : public AlignedAllocationPolicy<16>
{
private:
	struct MatrixBufferType
	{
		XMMATRIX World;
		XMMATRIX View;
		XMMATRIX Projection;
	};

	struct CameraBufferType
	{
		XMFLOAT3 CameraPosition;
		float Padding;
	};

	struct LightBufferType
	{
		XMFLOAT4 AmbientColor;
		XMFLOAT4 DiffuseColor;
		XMFLOAT3 LightDirection;
		float SpecularPower;
		XMFLOAT4 SpecularColor;
	};

public:
	LightShaderClass();
	LightShaderClass(const LightShaderClass&);
	~LightShaderClass();

	bool Initialize(ID3D11Device* InDevice, HWND InHwnd);
	void Shutdown();
	bool Render(ID3D11DeviceContext * InDeviceContext, int InIndexCount, XMMATRIX InWorldMatrix, XMMATRIX InViewMatrix
		, XMMATRIX InProjectionMatrix, ID3D11ShaderResourceView * InTexture, XMFLOAT3 InLightDirection
		, XMFLOAT4 InAmbientColor, XMFLOAT4 InDiffuseColor, XMFLOAT3 InCameraPosition, XMFLOAT4 InSpecularColor, float InSpecularPower);

private:
	bool InitializeShader(ID3D11Device* InDevice, HWND InHwnd, const WCHAR* InVSFilename, const WCHAR* InPSFilename);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* InErrorMessage, HWND InHwnd, const WCHAR* InShaderFilename);
	bool SetShaderParameters(ID3D11DeviceContext* InDeviceContext, XMMATRIX InWorldMatrix, XMMATRIX InViewMatrix
		, XMMATRIX InProjectionMatrix, ID3D11ShaderResourceView* InTexture, XMFLOAT3 InLightDirection
		, XMFLOAT4 inAmbientColor, XMFLOAT4 InDiffuseColor, XMFLOAT3 InCameraPosition, XMFLOAT4 InSpecularColor, float InSpecularPower);
	void RenderShader(ID3D11DeviceContext* InDeviceContext, int InIndexCount);

private:
	ID3D11VertexShader* VertexShader = nullptr;
	ID3D11PixelShader* PixelShader = nullptr;
	ID3D11InputLayout* Layout = nullptr;
	ID3D11SamplerState* SamplerState = nullptr;
	ID3D11Buffer* MatrixBuffer = nullptr;
	ID3D11Buffer* CameraBuffer = nullptr;
	ID3D11Buffer* LightBuffer = nullptr;
};

