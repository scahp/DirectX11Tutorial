#pragma once

class TextureClass;

class BitmapClass
{
private:
	struct VertexType
	{
		XMFLOAT3 Position;
		XMFLOAT2 Texture;
	};

public:
	BitmapClass();
	BitmapClass(const BitmapClass& InOther);
	~BitmapClass();

	bool Initialize(ID3D11Device* InDevice, ID3D11DeviceContext* InDeviceContext, int InScreenWidth, int InScreenHeight, const char* InTextureFilename,
		int InBitmapWidth, int InBitmapHeight);
	void Shutdown();
	bool Render(ID3D11DeviceContext* InDeviceContext, int InPositionX, int InPositionY);
	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device* InDevice);
	void ShutdownBuffers();
	bool UpdateBuffers(ID3D11DeviceContext* InDeviceContext, int InPositionX, int InPositionY);
	void RenderBuffers(ID3D11DeviceContext* InDeviceContext);
	bool LoadTexture(ID3D11Device* InDevice, ID3D11DeviceContext* InDeviceContext, const char* InFilename);
	void ReleaseTexture();

private:
	ID3D11Buffer* VertexBuffer = nullptr;
	ID3D11Buffer* IndexBuffer = nullptr;
	int VertexCount = 0;
	int IndexCount = 0;
	TextureClass* Texture = nullptr;
	int ScreenWidth = 0;
	int ScreenHeight = 0;
	int BitmapWidth = 0;
	int BitmapHeight = 0;
	int PreviousPosX = 0;
	int PreviousPosY = 0;
};

