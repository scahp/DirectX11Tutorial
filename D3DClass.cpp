#include "stdafx.h"
#include "D3DClass.h"

D3DClass::D3DClass()
{
}


D3DClass::D3DClass(const D3DClass& InClass)
{
}


D3DClass::~D3DClass()
{
}


bool D3DClass::Initialize(int InWidth, int InHeight, bool InEnableVsync, HWND InHwnd
	, bool InFullScreen, float InDepthFar, float InDepthNear)
{
	EnableVsync = InEnableVsync;

	// Create DX Graphic Interface Factory
	IDXGIFactory* Factory = nullptr;
	if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&Factory)))
		return false;

	// Create GraphicCard Interface Adapter by using Factory
	IDXGIAdapter* Adapter = nullptr;
	if (FAILED(Factory->EnumAdapters(0, &Adapter)))
		return false;

	// 출력(모니터)에 대한 첫번째 어뎁터를 지정합니다.
	IDXGIOutput* AdapterOutput = nullptr;
	if (FAILED(Adapter->EnumOutputs(0, &AdapterOutput)))
		return false;

	// 출력(모니터)에 대한 DXGI_FORMAT_R8G8B8A8_UNORM 표시 형식에 맞는 모드 수를 가져옴
	unsigned int NumModes = 0;
	if (FAILED(AdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED,
		&NumModes, NULL)))
	{
		return false;
	}

	// 가능한 모든 모니터와 그래픽카드 조합을 저장할 리스트를 생성합니다.
	DXGI_MODE_DESC* DisplayModeList = new DXGI_MODE_DESC[NumModes];
	if (!DisplayModeList)
		return false;

	// 이제 디스플레이 모드에 대한 리스트를 채웁니다.
	if (FAILED(AdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM
		, DXGI_ENUM_MODES_INTERLACED, &NumModes, DisplayModeList)))
	{
		return false;
	}

	// 이제 모든 디스플레이 모드에 대해 화면 너비/높이에 맞는 디스플레이 모드를 찾습니다.
	// 적합한 것을 찾으면 모니터의 새로고침 비율의 분모와 분자 값을 저장합니다.
	unsigned int Numerator = 0;
	unsigned int Denominator = 0;
	for (unsigned int i = 0; i < NumModes; ++i)
	{
		const auto& CurDisplayMode = DisplayModeList[i];
		if ((CurDisplayMode.Width == (unsigned int)InWidth)
			&& (CurDisplayMode.Height == (unsigned int)InHeight))
		{
			Numerator = CurDisplayMode.RefreshRate.Numerator;
			Denominator = CurDisplayMode.RefreshRate.Denominator;
		}
	}

	// 비디오카드의 구조체를 얻습니다.
	DXGI_ADAPTER_DESC AdapterDesc;
	if (FAILED(Adapter->GetDesc(&AdapterDesc)))
		return false;

	// 비디오카드 메모리 용량 단위를 MB 단위로 저장합니다.
	VideoCardMemory = (int)(AdapterDesc.DedicatedVideoMemory / (1024 * 1024));

	// 비디오카드 이름을 저장합니다.
	size_t StrLen = 0;
	if (wcstombs_s(&StrLen, VideoCardDescription, sizeof(VideoCardDescription), AdapterDesc.Description, sizeof(AdapterDesc.Description)))
		return false;

	// 디스플레이 모드 리스트를 해제합니다.
	delete[] DisplayModeList;
	DisplayModeList = nullptr;

	// 출력 어뎁터를 해제합니다.
	AdapterOutput->Release();
	AdapterOutput = nullptr;

	// 어댑터를 해제합니다.
	Adapter->Release();
	Adapter = nullptr;

	// 팩토리 객체를 해제합니다.
	Factory->Release();
	Factory = nullptr;

	// 스왑체인 구조체를 초기화합니다.
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));

	SwapChainDesc.BufferCount = 1;	// 백버퍼 1개만 사용하도록 지정
	SwapChainDesc.BufferDesc.Width = InWidth;
	SwapChainDesc.BufferDesc.Height = InHeight;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// 32Bit 서페이스를 설정
	if (EnableVsync)
	{
		SwapChainDesc.BufferDesc.RefreshRate.Numerator = Numerator;
		SwapChainDesc.BufferDesc.RefreshRate.Denominator = Denominator;
	}
	else
	{
		SwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.OutputWindow = InHwnd;	// 렌더링에 사용될 윈도우 핸들 지정
	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.SampleDesc.Quality = 0;
	SwapChainDesc.Windowed = !InFullScreen;

	// 스캔라인 순서 및 크기를 지정하지 않음으로 설정
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;	// 출력된 다음 백버퍼를 비우도록 지정
	SwapChainDesc.Flags = 0;

	// 피쳐레벨을 DX11로 설정
	D3D_FEATURE_LEVEL FeatureLevel = D3D_FEATURE_LEVEL_11_0;

	// 스왑체인, Direct3D 장치 그리고 Direct3D 장치 컨텍스트를 만듭니다.
	if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, &FeatureLevel, 1
		, D3D11_SDK_VERSION, &SwapChainDesc, &SwapChain, &Device, nullptr, &DeviceContext)))
	{
		return false;
	}

	// 백버퍼 포인터를 얻어옵니다.
	ID3D11Texture2D* Backbuffer = nullptr;
	if (FAILED(SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&Backbuffer)))
		return false;

	// 백버퍼 포인터로 렌더타겟 뷰를 생성
	if (FAILED(Device->CreateRenderTargetView(Backbuffer, nullptr, &RenderTargetView)))
		return false;

	// 백버퍼 포인터를 해제합니다.
	Backbuffer->Release();
	Backbuffer = nullptr;

	// 깊이버퍼 구조체를 초기화 합니다.
	D3D11_TEXTURE2D_DESC DepthBufferDesc;
	ZeroMemory(&DepthBufferDesc, sizeof(DepthBufferDesc));

	DepthBufferDesc.Width = InWidth;
	DepthBufferDesc.Height = InHeight;
	DepthBufferDesc.MipLevels = 1;
	DepthBufferDesc.ArraySize = 1;
	DepthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthBufferDesc.SampleDesc.Count = 1;
	DepthBufferDesc.SampleDesc.Quality = 0;
	DepthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	DepthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DepthBufferDesc.CPUAccessFlags = 0;
	DepthBufferDesc.MiscFlags = 0;

	// 깊이버퍼 텍스쳐 생성
	if (Device->CreateTexture2D(&DepthBufferDesc, nullptr, &DepthStencilBuffer))
	return false;

	// 스텐실상태 구조체를 초기화합니다.
	D3D11_DEPTH_STENCIL_DESC DepthStencilDesc;
	ZeroMemory(&DepthStencilDesc, sizeof(DepthStencilDesc));

	DepthStencilDesc.DepthEnable = true;
	DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	DepthStencilDesc.StencilEnable = true;
	DepthStencilDesc.StencilReadMask = 0xFF;
	DepthStencilDesc.StencilWriteMask = 0xFF;

	DepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	DepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	DepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	DepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	if (FAILED(Device->CreateDepthStencilState(&DepthStencilDesc, &DepthStencilState)))
		return false;

	// Set DepthStencilState
	DeviceContext->OMSetDepthStencilState(DepthStencilState, 1);

	D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc;
	ZeroMemory(&DepthStencilViewDesc, sizeof(DepthStencilViewDesc));
	DepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DepthStencilViewDesc.Texture2D.MipSlice = 0;

	// 깊이 스텐실 뷰를 생성
	if (FAILED(Device->CreateDepthStencilView(DepthStencilBuffer, &DepthStencilViewDesc, &DepthStencilView)))
		return false;

	// Set RenderTargetView and DepthStencilView
	DeviceContext->OMSetRenderTargets(1, &RenderTargetView, DepthStencilView);

	D3D11_RASTERIZER_DESC RasterDesc;
	RasterDesc.AntialiasedLineEnable = false;
	RasterDesc.CullMode = D3D11_CULL_BACK;
	RasterDesc.DepthBias = 0;
	RasterDesc.DepthBiasClamp = 0.0f;
	RasterDesc.DepthClipEnable = true;
	RasterDesc.FillMode = D3D11_FILL_SOLID;
	RasterDesc.FrontCounterClockwise = false;
	RasterDesc.MultisampleEnable = false;
	RasterDesc.ScissorEnable = false;
	RasterDesc.SlopeScaledDepthBias = 0.0f;

	if (FAILED(Device->CreateRasterizerState(&RasterDesc, &RasterState)))
		return false;

	// Set RasterizerState
	DeviceContext->RSSetState(RasterState);

	D3D11_VIEWPORT Viewport;
	Viewport.Width = (float)InWidth;
	Viewport.Height = (float)InHeight;
	Viewport.MinDepth = 0.0f;
	Viewport.MaxDepth = 1.0f;
	Viewport.TopLeftX = 0.0f;
	Viewport.TopLeftY = 0.0f;

	DeviceContext->RSSetViewports(1, &Viewport);

	float FieldOfView = 3.14f / 4.0f;
	float ScreenAspect = (float)InWidth / (float)InHeight;

	ProjectionMatrix = XMMatrixPerspectiveFovLH(FieldOfView, ScreenAspect, InDepthNear, InDepthFar);
	WorldMatrix = XMMatrixIdentity();
	OrthoMatrix = XMMatrixOrthographicLH((float)InWidth, (float)InHeight, InDepthNear, InDepthFar);

	return true;
}

void D3DClass::Shutdown()
{
	// 종료전 윈도우 모드로 설정하지 않으면 스왑체인 해제시 예외 발생
	if (SwapChain)
		SwapChain->SetFullscreenState(false, nullptr);

	if (RasterState)
	{
		RasterState->Release();
		RasterState = nullptr;
	}

	if (DepthStencilView)
	{
		DepthStencilView->Release();
		DepthStencilView = nullptr;
	}

	if (DepthStencilState)
	{
		DepthStencilState->Release();
		DepthStencilState = nullptr;
	}

	if (DepthStencilBuffer)
	{
		DepthStencilBuffer->Release();
		DepthStencilBuffer = nullptr;
	}

	if (RenderTargetView)
	{
		RenderTargetView->Release();
		RenderTargetView = nullptr;
	}

	if (DeviceContext)
	{
		DeviceContext->Release();
		DeviceContext = nullptr;
	}

	if (Device)
	{
		Device->Release();
		Device = nullptr;
	}

	if (SwapChain)
	{
		SwapChain->Release();
		SwapChain = nullptr;
	}
}

void D3DClass::BeginScene(float InClearR, float InClearG, float InClearB, float InClearA)
{
	const float ClearColor[4] = { InClearR, InClearG, InClearB, InClearA };
	
	DeviceContext->ClearRenderTargetView(RenderTargetView, ClearColor);
	DeviceContext->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void D3DClass::EndScene()
{
	if (EnableVsync)
		SwapChain->Present(1, 0);	// 화면 새로고침 비율을 고정
	else
		SwapChain->Present(0, 0);	// 가능한 빠르게 출력
}

void D3DClass::GetVideoCardInfo(char* OutCardName, int& OutMemory)
{
	strcpy_s(OutCardName, sizeof(OutCardName), VideoCardDescription);
	OutMemory = VideoCardMemory;
}

