#include "stdafx.h"
#include "TextureArrayClass.h"

bool TextureArrayClass::Initialize(ID3D11Device* InDevice, const WCHAR* InFilename1, const WCHAR* InFilename2)
{
	if (FAILED(CreateDDSTextureFromFile(InDevice, InFilename1, nullptr, &Textures[0])))
		return false;

	if (FAILED(CreateDDSTextureFromFile(InDevice, InFilename2, nullptr, &Textures[1])))
		return false;

	return true;
}

void TextureArrayClass::Shutdown()
{
	DX_RELEASE(Textures[0]);
	DX_RELEASE(Textures[1]);
}

ID3D11ShaderResourceView** TextureArrayClass::GetTextureArray()
{
	return Textures;
}
