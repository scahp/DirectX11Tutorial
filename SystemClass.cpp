#include "SystemClass.h"

// SystemClass.cpp
#include "stdafx.h"
#include "InputClass.h"
#include "GraphicsClass.h"
#include "SystemClass.h"
#include "PositionClass.h"
#include "FPSClass.h"
#include "CPUClass.h"
#include "TimerClass.h"

LRESULT CALLBACK SystemClassWndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

SystemClass::SystemClass()
{
}

SystemClass::SystemClass(const SystemClass& InClass)
{
}

SystemClass::~SystemClass()
{
}

bool SystemClass::Initialize()
{
	int ScreenWidth = 0;
	int ScreenHeight = 0;

	InitializeWindow(ScreenWidth, ScreenHeight);

	Input = new InputClass();
	if (!Input)
		return false;

	if (!Input->Initialize(Instance, Hwnd, ScreenWidth, ScreenHeight))
	{
		MessageBox(Hwnd, TEXT("Could not initialize the input object."), TEXT("Error"), MB_OK);
		return false;
	}

	Graphics = new GraphicsClass();
	if (!Graphics)
		return false;

	Position = new PositionClass();
	if (!Position)
		return false;

	FPS = new FPSClass();
	if (!FPS)
		return false;
	FPS->Initialize();

	CPU = new CPUClass();
	if (!CPU)
		return false;
	CPU->Initialize();

	Timer = new TimerClass();
	if (!Timer)
		return false;
	Timer->Initialize();

	return Graphics->Initialize(ScreenWidth, ScreenHeight, Hwnd);
}

void SystemClass::Shutdown()
{
	DX_DELETE(FPS);
	DX_DELETE(CPU);
	DX_DELETE(Timer);
	DX_DELETE(Position);

	if (Graphics)
	{
		Graphics->Shutdown();
		DX_DELETE(Graphics);
	}

	if (Input)
	{
		Input->Shutdown();
		DX_DELETE(Input);
	}

	ShutdownWindows();
}

void SystemClass::Run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	char szPerf[128] = { 0, };

	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (!Frame())
				break;

			static float DeltaTime = 0;
			DeltaTime += Timer->GetTime();
			
			if (DeltaTime > 1000.0f)
			{
				sprintf_s(szPerf, sizeof(szPerf) - 1, "FPS : %d, CPU : %d%%\n", FPS->GetFPS(), CPU->GetCPUPercentage());
				OutputDebugStringA(szPerf);
				DeltaTime = 0.0f;
			}
		}

		if (Input->IsEscapePressed())
			break;
	}
}

bool SystemClass::Frame()
{
	Timer->Frame();
	FPS->Frame();
	CPU->Frame();

	if (!Input->Frame())
		return false;

	int MouseX = 0, MouseY = 0;
	Input->GetMouseLocation(MouseX, MouseY);

	float RotationY = 0.0f;
	Position->GetRotation(RotationY);

	return Graphics->Frame(RotationY);
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND InHwnd, UINT InMsg, WPARAM InWparam, LPARAM InLparam)
{
	return DefWindowProc(InHwnd, InMsg, InWparam, InLparam);
}

void SystemClass::InitializeWindow(int& OutScreenWidth, int& OutScreenHeight)
{
	ApplicationHandle = this;
	Instance = GetModuleHandle(NULL);
	ApplicationName = TEXT("DX11Tutorial");

	int PosX = 0;
	int PosY = 0;

	// 윈도우 만들고
	// windows 클래스를 아래와 같이 설정합니다.
	WNDCLASSEX wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = SystemClassWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = Instance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = ApplicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// windows class를 등록합니다
	RegisterClassEx(&wc);

	OutScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	OutScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (FULL_SCREEN)
	{
		DEVMODE ScreenSetting;
		memset(&ScreenSetting, 0, sizeof(ScreenSetting));
		ScreenSetting.dmSize = sizeof(ScreenSetting);
		ScreenSetting.dmPelsWidth = (unsigned long)OutScreenWidth;
		ScreenSetting.dmPelsHeight = (unsigned long)OutScreenHeight;
		ScreenSetting.dmBitsPerPel = 32;
		ScreenSetting.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// 풀스크린으로 디스플레이를 설정합니다.
		ChangeDisplaySettings(&ScreenSetting, CDS_FULLSCREEN);
	}
	else
	{
		OutScreenWidth = 800;
		OutScreenHeight = 600;

		// 윈도우 창을 가로, 세로의 정 가운데 오도록 함
		PosX = (GetSystemMetrics(SM_CXSCREEN) - OutScreenWidth) / 2;
		PosY = (GetSystemMetrics(SM_CYSCREEN) - OutScreenHeight) / 2;
	}

	Hwnd = CreateWindowEx(WS_EX_APPWINDOW, ApplicationName, ApplicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		PosX, PosY, OutScreenWidth, OutScreenHeight, NULL, NULL, Instance, NULL);

	// 윈도우를 화면에 표시하고 포커스를 지정합니다
	ShowWindow(Hwnd, SW_SHOW);
	SetForegroundWindow(Hwnd);
	SetFocus(Hwnd);
}

void SystemClass::ShutdownWindows()
{
	if (FULL_SCREEN)
		ChangeDisplaySettings(NULL, 0);

	DestroyWindow(Hwnd);
	Hwnd = NULL;

	UnregisterClass(ApplicationName, Instance);
	Instance = NULL;

	ApplicationHandle = NULL;
}

LRESULT CALLBACK SystemClassWndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
		default:
		{
			if (ApplicationHandle)
				return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
			return DefWindowProc(hwnd, umessage, wparam, lparam);
		}
	}
}