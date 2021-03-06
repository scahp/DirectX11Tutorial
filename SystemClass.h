﻿#pragma once
#include <Windows.h>

class InputClass;
class GraphicsClass;
class PositionClass;
class FPSClass;
class CPUClass;
class TimerClass;

class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass& InClass);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND InHWnd, UINT InMsg, WPARAM InWParam, LPARAM InLParam);

private:
	bool Frame();
	void InitializeWindow(int& OutScreenWidth, int& OutScreenHeight);
	void ShutdownWindows();

private:
	LPCWSTR ApplicationName = TEXT("DirectX11Tutorial");
	HINSTANCE Instance;
	HWND Hwnd;

	InputClass* Input = nullptr;
	GraphicsClass* Graphics = nullptr;
	PositionClass* Position = nullptr;
	FPSClass* FPS = nullptr;
	CPUClass* CPU = nullptr;
	TimerClass* Timer = nullptr;
};

static SystemClass* ApplicationHandle = 0;
