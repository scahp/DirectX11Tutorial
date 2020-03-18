#pragma once

#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>

class InputClass
{
public:
	InputClass();
	InputClass(const InputClass& InClass);
	~InputClass();

	bool Initialize(HINSTANCE InHinstance, HWND InHwnd, int InScreenWidth, int InScreenHeight);
	void Shutdown();
	bool Frame();

	bool IsEscapePressed();
	void GetMouseLocation(int& OutX, int& OutY);

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

private:
	IDirectInput8* DirectInput = nullptr;
	IDirectInputDevice8* Keyboard = nullptr;
	IDirectInputDevice8* Mouse = nullptr;

	unsigned char KeyboardState[256] = { 0, };
	DIMOUSESTATE MouseState;

	int ScreenWidth = 0;
	int ScreenHeight = 0;
	int MouseX = 0;
	int MouseY = 0;
};