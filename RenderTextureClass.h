#pragma once

class RenderTextureClass
{
public:
	bool Initialize(ID3D11Device* InDevice, int InTextureWidth, int InTextureHeight);
	void Shutdown();
	void SetRenderTarget(ID3D11DeviceContext* InDeviceContext, ID3D11DepthStencilView* InDepthStencilView);
	void ClearRenderTarget(ID3D11DeviceContext* InDeviceContext, ID3D11DepthStencilView* InDepthStencilView
		, float InRed, float InGreen, float InBlue, float InAlpha);
	ID3D11ShaderResourceView* GetShaderResourceView();

private:
	ID3D11Texture2D* RenderTargetTexture = nullptr;
	ID3D11RenderTargetView* RenderTargetView = nullptr;
	ID3D11ShaderResourceView* ShaderResourceView = nullptr;
};

