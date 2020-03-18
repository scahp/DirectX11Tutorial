#include "stdafx.h"
#include "InputClass.h"

InputClass::InputClass()
{
}


InputClass::InputClass(const InputClass& InClass)
{
}


InputClass::~InputClass()
{
}


bool InputClass::Initialize(HINSTANCE InHinstance, HWND InHwnd, int InScreenWidth, int InScreenHeight)
{
	ScreenWidth = InScreenWidth;
	ScreenHeight = InScreenHeight;

	HRESULT Result = DirectInput8Create(InHinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&DirectInput, NULL);
	if (FAILED(Result))
		return false;

	Result = DirectInput->CreateDevice(GUID_SysKeyboard, &Keyboard, NULL);
	if (FAILED(Result))
		return false;

	// 데이터 형식을 설정하십시오. 이 경우 키보드이므로 사전 정의 된 데이터 형식을 사용할 수 있습니다.
	Result = Keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(Result))
		return false;

	// 다른 프로그램과 공유하지 않도록 키보드의 협조 수준을 설정합니다
	Result = Keyboard->SetCooperativeLevel(InHwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(Result))
		return false;

	// 키보드를 할당받는다
	Result = Keyboard->Acquire();
	if (FAILED(Result))
		return false;

	// 마우스 Direct Input 인터페이스를 생성합니다.
	Result = DirectInput->CreateDevice(GUID_SysMouse, &Mouse, NULL);
	if (FAILED(Result))
		return false;

	// 미리 정의 된 마우스 데이터 형식을 사용하여 마우스의 데이터 형식을 설정합니다.
	Result = Mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(Result))
		return false;

	// 다른 프로그램과 공유 할 수 있도록 마우스의 협력 수준을 설정합니다.
	Result = Mouse->SetCooperativeLevel(InHwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(Result))
		return false;

	// 마우스를 할당받는다
	Result = Mouse->Acquire();
	if (FAILED(Result))
		return false;

	return true;
}


void InputClass::Shutdown()
{
	if (Mouse)
	{
		Mouse->Unacquire();
		DX_RELEASE(Mouse);
	}

	if (Keyboard)
	{
		Keyboard->Unacquire();
		DX_RELEASE(Keyboard);
	}

	DX_RELEASE(DirectInput);
}

bool InputClass::Frame()
{
	if (!ReadKeyboard())
		return false;

	if (!ReadMouse())
		return false;

	ProcessInput();

	return true;
}

bool InputClass::IsEscapePressed()
{
	if (KeyboardState[DIK_ESCAPE] & 0x80)
		return true;

	return false;
}

void InputClass::GetMouseLocation(int& OutX, int& OutY)
{
	OutX = MouseX;
	OutY = MouseY;
}

bool InputClass::ReadKeyboard()
{
	HRESULT Result = Keyboard->GetDeviceState(sizeof(KeyboardState), (LPVOID)&KeyboardState);
	if (FAILED(Result))
	{
		// 키보드가 포커스를 잃었거나 획득되지 않은 경우 컨트롤을 다시 가져 온다
		if ((Result == DIERR_INPUTLOST) || (Result == DIERR_NOTACQUIRED))
			Keyboard->Acquire();
		else
			return false;
	}

	return true;
}

bool InputClass::ReadMouse()
{
	HRESULT Result = Mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&MouseState);
	if (FAILED(Result))
	{
		// 마우스가 포커스를 잃었거나 획득되지 않은 경우 컨트롤을 다시 가져 온다
		if ((Result == DIERR_INPUTLOST) || (Result == DIERR_NOTACQUIRED))
			Mouse->Acquire();
		else
			return false;
	}

	return true;
}

void InputClass::ProcessInput()
{
	MouseX += MouseState.lX;
	MouseY += MouseState.lY;

	// 마우스 위치가 화면 너비 또는 높이를 초과하지 않는지 확인한다.
	if (MouseX < 0) { MouseX = 0; }
	if (MouseY < 0) { MouseY = 0; }

	if (MouseX > ScreenWidth) { MouseX = ScreenWidth; }
	if (MouseY > ScreenHeight) { MouseY = ScreenHeight; }
}
