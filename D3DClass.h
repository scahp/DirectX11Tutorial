#pragma once

class D3DClass
{
public:
	D3DClass();
	D3DClass(const D3DClass& InClass);
	~D3DClass();

	bool Initialize(int InWidth, int InHeight, bool InEnableVsync, HWND InHwnd
				, bool InFullScreen, float InDepthFar, float InDepthNear);
	void Shutdown();

	void BeginScene(float InClearR, float InClearG, float InClearB, float InClearA);
	void EndScene();

	ID3D11Device* GetDevice() { return Device; }
	ID3D11DeviceContext* GetDeviceContext() { return DeviceContext; }

	void GetProjectionMatrix(XMMATRIX& OutMatrix) { OutMatrix = ProjectionMatrix; }
	void GetWorldMatrix(XMMATRIX& OutMatrix) { OutMatrix = WorldMatrix; }
	void GetOrthoMatrix(XMMATRIX& OutMatrix) { OutMatrix = OrthoMatrix; }

	void GetVideoCardInfo(char* OutCardName, int& OutMemory);

	void TurnZBufferOn();
	void TurnZBufferOff();

private:
	bool EnableVsync = false;
	int VideoCardMemory = 0;
	char VideoCardDescription[128] = { 0, };
	IDXGISwapChain* SwapChain = nullptr;
	ID3D11Device* Device = nullptr;
	ID3D11DeviceContext* DeviceContext = nullptr;
	ID3D11RenderTargetView* RenderTargetView = nullptr;
	ID3D11Texture2D* DepthStencilBuffer = nullptr;
	ID3D11DepthStencilState* DepthStencilState = nullptr;
	ID3D11DepthStencilView* DepthStencilView = nullptr;
	ID3D11RasterizerState* RasterState = nullptr;
	XMMATRIX ProjectionMatrix;
	XMMATRIX WorldMatrix;
	XMMATRIX OrthoMatrix;
	ID3D11DepthStencilState* DepthDisabledStencilState = nullptr;
};