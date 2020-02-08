#pragma once

class ColorShaderClass : public AlignedAllocationPolicy<16>
{
private:
	struct MatrixBufferType
	{
		XMMATRIX World;
		XMMATRIX View;
		XMMATRIX Projection;
	};
public:
	ColorShaderClass();
	ColorShaderClass(const ColorShaderClass& InClass);
	~ColorShaderClass();

	bool Initialize(ID3D11Device* InDevice, HWND InHwnd);
	void Shutdown();
	bool Render(ID3D11DeviceContext* InDeviceContext, int InIndexCount, const XMMATRIX& InWorldMatrix, const XMMATRIX& InViewMatrix, const XMMATRIX& InProjectionMatrix);

private:
	bool InitializeShader(ID3D11Device* InDevice, HWND InHwnd, const WCHAR* InVSFilename, const WCHAR* InPSFilename);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* InErrorMessage, HWND InHwnd, const WCHAR* InShaderFilename);
	bool SetShaderParameters(ID3D11DeviceContext* InDeviceContext, const XMMATRIX& InWorldMatrix, const XMMATRIX& InViewMatrix, const XMMATRIX& InProjectionMatrix);
	void RenderShader(ID3D11DeviceContext* InDeviceContext, int InIndexCount);

private:
	ID3D11VertexShader* VertexShader = nullptr;
	ID3D11PixelShader* PixelShader = nullptr;
	ID3D11InputLayout* Layout = nullptr;
	ID3D11Buffer* MatrixBuffer = nullptr;
};