//#pragma once
//
//class ColorShaderClass
//{
//private:
//	struct MatrixBufferType
//	{
//		XMMATRIX World;
//		XMMATRIX View;
//		XMMATRIX Projection;
//	};
//public:
//	ColorShaderClass();
//	ColorShaderClass(const ColorShaderClass& InClass);
//	~ColorShaderClass();
//
//	bool Initialize(ID3D11Device* InDevice, HWND InHwnd);
//	void Shutdown();
//private:
//	ID3D11VertexShader* VertexShader = nullptr;
//	ID3D11PixelShader* PixelShader = nullptr;
//	ID3D11InputLayout* Layout = nullptr;
//	ID3D11Buffer* MatrixBuffer = nullptr;
//};