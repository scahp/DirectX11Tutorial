#pragma once

class MultiTextureShaderClass
{
private:
	struct MatrixBufferType
	{
		XMMATRIX World;
		XMMATRIX View;
		XMMATRIX Projection;
	};

public:
	MultiTextureShaderClass() {}
	MultiTextureShaderClass(const MultiTextureShaderClass& InOhter) {}
	~MultiTextureShaderClass() {}

	bool Initialize(ID3D11Device* InDevice, HWND InHwnd);
	void Shutdown();
	bool Render(ID3D11DeviceContext* InDeviceContext, int InIndexCount, XMMATRIX InWorldMatrix, XMMATRIX InViewMatrix, XMMATRIX InProjectionMatrix, ID3D11ShaderResourceView** InTextureArray);

private:
	bool InitializeShader(ID3D11Device* InDevice, HWND InHwnd, const WCHAR* InVSFilename, const WCHAR* InPSFilename);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* InErrorMessage, HWND InHwnd, const WCHAR* InShaderFilename);
	bool SetShaderParameters(ID3D11DeviceContext* InDeviceContext, XMMATRIX InWorldMatrix, XMMATRIX InViewMatrix, XMMATRIX InProjectionMatrix, ID3D11ShaderResourceView** InTextureArray);
	void RenderShader(ID3D11DeviceContext* InDeviceContext, int InIndexCount);

public:
	ID3D11VertexShader* VertexShader = nullptr;
	ID3D11PixelShader* PixelShader = nullptr;
	ID3D11InputLayout* Layout = nullptr;
	ID3D11Buffer* MatrixBuffer = nullptr;
	ID3D11SamplerState* SampleState = nullptr;
};

