#include "stdafx.h"
#include "TextureClass.h"

TextureClass::TextureClass()
{

}

TextureClass::TextureClass(const TextureClass& InClass)
{

}

TextureClass::~TextureClass()
{

}

bool TextureClass::Initialize(ID3D11Device* InDevice, ID3D11DeviceContext* InDeivceContext, const char* InFilename)
{
	int Width = 0;
	int Height = 0;

	if (!LoadTarga(InFilename, Width, Height))
		return false;

	D3D11_TEXTURE2D_DESC TextureDesc;
	TextureDesc.Width = Width;
	TextureDesc.Height = Height;
	TextureDesc.MipLevels = 0;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	if (FAILED(InDevice->CreateTexture2D(&TextureDesc, nullptr, &Texture)))
	{
		DX_DELETE_ARRAY(TargaData);
		return false;
	}

	UINT RowPitch = (Width * 4) * sizeof(unsigned char);		// Targa 이미지 데이터 너비 사이즈 구함
	InDeivceContext->UpdateSubresource(Texture, 0, nullptr, TargaData, RowPitch, 0);	// Targa 이미지 데이터를 텍스쳐에 복사

	// 쉐이더 리소스 뷰 생성
	D3D11_SHADER_RESOURCE_VIEW_DESC SrvDesc;
	SrvDesc.Format = TextureDesc.Format;
	SrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SrvDesc.Texture2D.MostDetailedMip = 0;
	SrvDesc.Texture2D.MipLevels = -1;

	if (FAILED(InDevice->CreateShaderResourceView(Texture, &SrvDesc, &TextureView)))
	{
		DX_DELETE_ARRAY(TargaData);
		DX_RELEASE(Texture);
		return false;
	}

	InDeivceContext->GenerateMips(TextureView);

	DX_DELETE_ARRAY(TargaData);

	return true;
}

void TextureClass::Shutdown()
{
	DX_RELEASE(TextureView);
	DX_RELEASE(Texture);
	DX_DELETE_ARRAY(TargaData);
}

bool TextureClass::LoadTarga(const char* InFilename, int& Width, int& Height)
{
	// targa 파일을 바이너리 모드로 파일을 엽니다.
	FILE* FilePtr;
	if (fopen_s(&FilePtr, InFilename, "rb") != 0)
	{
		return false;
	}

	// 파일 헤더를 읽어옵니다.
	TargaHeader TargaFileHeader;
	unsigned int Count = (unsigned int)fread(&TargaFileHeader, sizeof(TargaHeader), 1, FilePtr);
	if (Count != 1)
	{
		fclose(FilePtr);
		return false;
	}

	// 파일헤더에서 중요 정보를 얻어옵니다.
	Height = (int)TargaFileHeader.Height;
	Width = (int)TargaFileHeader.Width;
	int bpp = (int)TargaFileHeader.BPP;

	// 파일이 32bit 인지 24bit인지 체크합니다.
	if (bpp != 32)
	{
		fclose(FilePtr);
		return false;
	}

	// 32 비트 이미지 데이터의 크기를 계산합니다.
	int ImageSize = Width * Height * 4;

	//  targa 이미지 데이터 용 메모리를 할당합니다.
	unsigned char* TargaImage = new unsigned char[ImageSize];
	if (!TargaImage)
	{
		fclose(FilePtr);
		return false;
	}

	// targa 이미지 데이터를 읽습니다.
	Count = (unsigned int)fread(TargaImage, 1, ImageSize, FilePtr);
	if (Count != ImageSize)
	{
		DX_DELETE_ARRAY(TargaImage);
		fclose(FilePtr);
		return false;
	}

	// 파일을 닫습니다.
	if (fclose(FilePtr) != 0)
	{
		DX_DELETE_ARRAY(TargaImage);
		return false;
	}

	// targa 대상 데이터에 대한 메모리를 할당합니다.
	TargaData = new unsigned char[ImageSize];
	if (!TargaData)
	{
		DX_DELETE_ARRAY(TargaImage);
		return false;
	}

	// targa 대상 데이터 배열에 인덱스를 초기화합니다.
	int index = 0;

	// targa 이미지 데이터에 인덱스를 초기화합니다.
	int k = (Width * Height * 4) - (Width * 4);

	// 이제 targa 형식이 거꾸로 저장되었으므로 올바른 순서로 targa 이미지 데이터를 targa 대상 배열에 복사합니다.
	for (int j = 0; j < Height; j++)
	{
		for (int i = 0; i < Width; i++)
		{
			TargaData[index + 0] = TargaImage[k + 2];  // 빨강
			TargaData[index + 1] = TargaImage[k + 1];  // 녹색
			TargaData[index + 2] = TargaImage[k + 0];  // 파랑
			TargaData[index + 3] = TargaImage[k + 3];  // 알파

														 // 인덱스를 targa 데이터로 증가시킵니다.
			k += 4;
			index += 4;
		}

		// targa 이미지 데이터 인덱스를 역순으로 읽은 후 열의 시작 부분에서 이전 행으로 다시 설정합니다.
		k -= (Width * 8);
	}

	// 대상 배열에 복사 된 targa 이미지 데이터를 해제합니다.
	DX_DELETE_ARRAY(TargaImage);

	return true;
}