#pragma once

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH_FAR = 1000.0f;
const float SCREEN_DEPTH_NEAR = 0.1f;

class D3DClass;
class CameraClass;
class ModelClass;
class ColorShaderClass;
class TextureShaderClass;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass& InClass);
	~GraphicsClass();

	bool Initialize(int InScreenWidth, int InScreenHeight, HWND InHwnd);
	void Shutdown();
	bool Frame();

private:
	bool Render();

private:
	D3DClass* Direct3D = nullptr;
	CameraClass* Camera = nullptr;
	ModelClass* Model = nullptr;
	ColorShaderClass* ColorShader = nullptr;
	TextureShaderClass* TextureShader = nullptr;
};