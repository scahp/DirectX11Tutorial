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
class LightClass;
class LightShaderClass;
class BitmapClass;
class ModelListClass;
class FrustumClass;
class MultiTextureShaderClass;
class BumpMapShaderClass;
class DebugWindowClass;
class RenderTextureClass;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass& InClass);
	~GraphicsClass();

	bool Initialize(int InScreenWidth, int InScreenHeight, HWND InHwnd);
	void Shutdown();
	bool Frame(float RotationY);

private:
	bool Render(float InRotation);
	bool RenderToTexture(float InRotation);
	bool RenderScene();

private:
	D3DClass* Direct3D = nullptr;
	CameraClass* Camera = nullptr;
	ModelClass* Model = nullptr;
	ColorShaderClass* ColorShader = nullptr;
	TextureShaderClass* TextureShader = nullptr;
	LightShaderClass* LightShader = nullptr;
	LightClass* Light = nullptr;
	BitmapClass* Bitmap = nullptr;
	ModelListClass* ModelList = nullptr;
	FrustumClass* Frustum = nullptr;
	MultiTextureShaderClass* MultiTextureShader = nullptr;
	BumpMapShaderClass* BumpMapShader = nullptr;
	DebugWindowClass* DebugWindow = nullptr;
	RenderTextureClass* RenderTexture = nullptr;
};