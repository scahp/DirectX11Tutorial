#include "stdafx.h"
#include "RenderTextureClass.h"

bool RenderTextureClass::Initialize(ID3D11Device* InDevice, int InTextureWidth, int InTextureHeight)
{
	D3D11_TEXTURE2D_DESC TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(TextureDesc));

	TextureDesc.Width = InTextureWidth;
	TextureDesc.Height = InTextureHeight;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	HRESULT Result = InDevice->CreateTexture2D(&TextureDesc, nullptr, &RenderTargetTexture);
	if (FAILED(Result))
		return false;

	D3D11_RENDER_TARGET_VIEW_DESC RenderTargetViewDesc;
	RenderTargetViewDesc.Format = TextureDesc.Format;
	RenderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	RenderTargetViewDesc.Texture2D.MipSlice = 0;

	Result = InDevice->CreateRenderTargetView(RenderTargetTexture, &RenderTargetViewDesc, &RenderTargetView);
	if (FAILED(Result))
		return false;

	D3D11_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc;
	ShaderResourceViewDesc.Format = TextureDesc.Format;
	ShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	ShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	ShaderResourceViewDesc.Texture2D.MipLevels = 1;

	Result = InDevice->CreateShaderResourceView(RenderTargetTexture, &ShaderResourceViewDesc, &ShaderResourceView);
	if (FAILED(Result))
		return false;

	return true;
}

void RenderTextureClass::Shutdown()
{
	DX_RELEASE(ShaderResourceView);
	DX_RELEASE(RenderTargetView);
	DX_RELEASE(RenderTargetTexture);
}

void RenderTextureClass::SetRenderTarget(ID3D11DeviceContext* InDeviceContext, ID3D11DepthStencilView* InDepthStencilView)
{
	InDeviceContext->OMSetRenderTargets(1, &RenderTargetView, InDepthStencilView);
}

void RenderTextureClass::ClearRenderTarget(ID3D11DeviceContext* InDeviceContext, ID3D11DepthStencilView* InDepthStencilView,
	float InRed, float InGreen, float InBlue, float InAlpha)
{
	float Color[4] = { InRed, InGreen, InBlue, InAlpha };
	InDeviceContext->ClearRenderTargetView(RenderTargetView, Color);
	InDeviceContext->ClearDepthStencilView(InDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

ID3D11ShaderResourceView* RenderTextureClass::GetShaderResourceView()
{
	return ShaderResourceView;
}