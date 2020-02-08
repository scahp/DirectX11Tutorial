#include "stdafx.h"
//#include "ColorShaderClass.h"
//
//ColorShaderClass::ColorShaderClass()
//{
//}
//
//ColorShaderClass::ColorShaderClass(const ColorShaderClass& InClass)
//{
//}
//
//
//ColorShaderClass::~ColorShaderClass()
//{
//}
//
//bool ColorShaderClass::Initialize(ID3D11Device* InDevice, HWND InHwnd)
//{
//	return InitializeShader(InDevice, InHwnd, TEXT("Shaders/Color.vs"), TEXT("Shaders/Color.ps"));
//}
//
//void ColorShaderClass::Shutdown()
//{
//	ShutdownShader();
//}
//
//bool ColorShaderClass::Render(ID3D11DeviceContext* InDeviceContext, int InIndexCount
//	, const XMMATRIX& InWorldMatrix, const XMMATRIX& InViewMatrix, const XMMATRIX& InProjectionMatrix)
//{
//	if (!SetShaderParameters(InDeviceContext, InWorldMatrix, InViewMatrix, InProjectionMatrix))
//		return false;
//
//	RenderShader(InDeviceContext, InIndexCount);
//	return true;
//}
//
//bool ColorShaderClass::InitializeShader(ID3D11Device* InDevice, HWND InHwnd, WCHAR* InVSFilename, WCHAR* InPSFilename)
//{
//	ID3D10Blob* ErrorMessage = nullptr;
//
//	// Compile VS
//	ID3D10Blob* VertexShaderBuffer = nullptr;
//	if (FAILED(D3DCompileFromFile(InVSFilename, nullptr, nullptr, "ColorVertexShader", "vs_5_0"
//		, D3D10_SHADER_ENABLE_STRICTNESS, 0, &VertexShaderBuffer, &ErrorMessage)))
//	{
//		if (ErrorMessage)
//			OutputShaderErrorMessage(ErrorMessage, InHwnd, InVSFilename);
//		else
//			MessageBox(InHwnd, InVSFilename, TEXT("Missing Shader File"), MB_OK);
//
//		return false;
//	}
//
//	ID3D10Blob* PixelShaderBuffer = nullptr;
//	if (FAILED(D3DCompileFromFile(InPSFilename, nullptr, nullptr, "ColorPixelShader", "ps_5_0"
//		, D3D10_SHADER_ENABLE_STRICTNESS, 0, &PixelShaderBuffer, &ErrorMessage)))
//	{
//		if (ErrorMessage)
//			OutputShaderErrorMessage(ErrorMessage, InHwnd, InVSFilename);
//		else
//			MessageBox(InHwnd, InVSFilename, TEXT("Missing Shader File"), MB_OK);
//
//		return false;
//	}
//
//	if (FAILED(InDevice->CreateVertexShader(VertexShaderBuffer->GetBufferPointer(), )))
//
//	if (FAILED(InDevice->CreatePixelShader(PixelShaderBuffer)))
//}
//
