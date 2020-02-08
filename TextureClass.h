#pragma once

class TextureClass
{
private:
	struct TargaHeader
	{
		unsigned char Data1[12];
		unsigned short Width;
		unsigned short Height;
		unsigned char BPP;
		unsigned char Data2;
	};

public:
	TextureClass();
	TextureClass(const TextureClass& InClass);
	~TextureClass();

	bool Initialize(ID3D11Device* InDevice, ID3D11DeviceContext* InDeivceContext, const char* InFilename);
	void Shutdown();
	bool LoadTarga(const char* InFilename, int& Width, int& Height);
	ID3D11ShaderResourceView* GetTexture() const { return TextureView; }
private:
	unsigned char* TargaData = nullptr;
	ID3D11Texture2D* Texture = nullptr;
	ID3D11ShaderResourceView* TextureView = nullptr;
};