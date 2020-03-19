#pragma once

class TextureArrayClass
{
public:
	bool Initialize(ID3D11Device* InDevice, const WCHAR* InFilename1, const WCHAR* InFilename2);
	void Shutdown();

	ID3D11ShaderResourceView** GetTextureArray();

private:
	ID3D11ShaderResourceView* Textures[2] = { nullptr, nullptr };
};

