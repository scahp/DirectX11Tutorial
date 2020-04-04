#pragma once
class DebugWindowClass
{
private:
	struct VertexType
	{
		XMFLOAT3 Position;
		XMFLOAT2 Texture;
	};

public:
	bool Initialize(ID3D11Device* InDevice, int InScreenWidth, int InScreenHeight, int InBitmapWidth, int InBitmapHeight);
	void Shutdown();
	bool Bind(ID3D11DeviceContext* InDeviceContext, int InPositionX, int InPositionY);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device* InDevice);
	void ShutdownBuffers();
	bool UpdateBuffers(ID3D11DeviceContext* InDeviceContext, int InPositionX, int InPositionY);
	void BindBuffers(ID3D11DeviceContext* InDeviceContext);

private:
	ID3D11Buffer* VertexBuffer = nullptr;
	ID3D11Buffer* IndexBuffer = nullptr;
	int VertexCount = 0;
	int IndexCount = 0;
	int ScreenWidth = 0;
	int ScreenHeight = 0;
	int BitmapWidth = 0;
	int BitmapHeight = 0;
	int PreviousPosX = 0;
	int PreviousPosY = 0;
};

