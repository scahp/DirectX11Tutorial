#include "stdafx.h"
#include "MayaModelConverter.h"
#include <conio.h>

void MayaModelConverter::Do()
{
	OpenConsole();

	bool Result;
	char Filename[256];
	int VertexCount, TextureCount, NormalCount, FaceCount;

	GetModelFilename(Filename);

	Result = ReadFileCounts(Filename, VertexCount, TextureCount, NormalCount, FaceCount);
	if (!Result)
		return;

	std::cout << std::endl;
	std::cout << "Vertices:\t" << VertexCount << std::endl;
	std::cout << "UVs:\t" << TextureCount << std::endl;
	std::cout << "Normals:\t" << NormalCount << std::endl;
	std::cout << "Faces:\t" << FaceCount << std::endl;

	Result = LoadDataStructures(Filename, VertexCount, TextureCount, NormalCount, FaceCount);
	if (!Result)
		return;

	std::cout << "\nFile has been converted." << std::endl;
	std::cout << "\nPress any key to close console window." << std::endl;
	_getch();

	CloseConsole();

	return;
}

void MayaModelConverter::OpenConsole()
{
	AllocConsole();
	SetConsoleTitle(TEXT("테스트용 콘솔"));

	FILE* FPDebugOut = NULL;
	FILE* FPDebugIn = NULL;
	_tfreopen_s(&FPDebugOut, _T("CONOUT$"), _T("w"), stdout);
	_tfreopen_s(&FPDebugIn, _T("CONIN$"), _T("r"), stdin);
	_tsetlocale(LC_ALL, _T(""));
}

void MayaModelConverter::CloseConsole()
{
	FreeConsole();
}

void MayaModelConverter::GetModelFilename(char* InFilename)
{
	bool Done;
	std::ifstream FIn;

	Done = false;
	while (!Done)
	{
		std::cout << "Enter model filename: ";

		std::cin >> InFilename;
		FIn.open(InFilename);

		if (FIn.good())
		{
			Done = true;
		}
		else
		{
			FIn.clear();
			std::cout << std::endl;
			std::cout << "File " << InFilename << " could not be opened." << std::endl << std::endl;
		}
	}
}

bool MayaModelConverter::ReadFileCounts(const char* InFilename, int& OutVertexCount, int& OutTextureCount, int& OutNormalCount, int& OutFaceCount)
{
	std::ifstream FIn;

	char Input;

	OutVertexCount = 0;
	OutTextureCount = 0;
	OutNormalCount = 0;
	OutFaceCount = 0;

	FIn.open(InFilename);

	if (FIn.fail())
		return false;

	FIn.get(Input);
	while (!FIn.eof())
	{
		if (Input == 'v')
		{
			FIn.get(Input);
			if (Input == ' ') { ++OutVertexCount; }
			if (Input == 't') { ++OutTextureCount; }
			if (Input == 'n') { ++OutNormalCount; }
		}

		if (Input == 'f')
		{
			FIn.get(Input);
			if (Input == ' ') { ++OutFaceCount; }
		}

		while (Input != '\n')
			FIn.get(Input);

		FIn.get(Input);
	}

	FIn.close();

	return true;
}

bool MayaModelConverter::LoadDataStructures(const char* InFilename, int InVertexCount, int InTextureCount, int InNormalCount, int InFaceCount)
{
	VertexType *Vertices, *TexCoords, *Normals;
	FaceType* Faces;
	std::ifstream FIn;
	int VertexIndex, TexCoordIndex, NormalIndex, FaceIndex, VIndex, TIndex, NIndex;
	char Input, Input2;
	std::ofstream FOut;

	Vertices = new VertexType[InVertexCount];
	if (!Vertices)
		return false;

	TexCoords = new VertexType[InTextureCount];
	if (!TexCoords)
		return false;

	Normals = new VertexType[InNormalCount];
	if (!Normals)
		return false;

	Faces = new FaceType[InFaceCount];
	if (!Faces)
		return false;

	VertexIndex = 0;
	TexCoordIndex = 0;
	NormalIndex = 0;
	FaceIndex = 0;

	FIn.open(InFilename);

	if (FIn.fail())
		return false;

	// 정점, 텍스쳐 좌표 및 법선을 데이터 구조로 읽어들입니다.
	// 중요 : Maya는 오른손 좌표계를 사용하기 때문에 왼손 좌표계로 변환합니다.
	FIn.get(Input);
	while (!FIn.eof())
	{
		if (Input == 'v')
		{
			FIn.get(Input);

			// 정점을 읽음
			if (Input == ' ')
			{
				FIn >> Vertices[VertexIndex].X >> Vertices[VertexIndex].Y >> Vertices[VertexIndex].Z;

				// Z 정점을 뒤집어 왼손 시스템으로 변경
				Vertices[VertexIndex].Z *= -1.0f;
				++VertexIndex;
			}

			// 텍스쳐 UV 좌표를 읽음
			if (Input == 't')
			{
				FIn >> TexCoords[TexCoordIndex].X >> TexCoords[TexCoordIndex].Y;

				// V 텍스쳐 좌표를 왼손 시스템으로 변경
				TexCoords[TexCoordIndex].Y = 1.0f - TexCoords[TexCoordIndex].Y;
				++TexCoordIndex;
			}

			// 법선을 읽음
			if (Input == 'n')
			{
				FIn >> Normals[NormalIndex].X >> Normals[NormalIndex].Y >> Normals[NormalIndex].Z;

				// Z 법선을 반전시켜 왼손 시스템으로 변경
				Normals[NormalIndex].Z *= -1.0f;
				++NormalIndex;
			}
		}

		// Face를 읽음
		if (Input == 'f')
		{
			FIn.get(Input);
			if (Input == ' ')
			{
				// 오른손 시스템에서 왼손 시스템으로 변환하기 위해서 표면 데이터를 거꾸로 읽음
				FIn >> Faces[FaceIndex].VIndex3 >> Input2 >> Faces[FaceIndex].TIndex3 >> Input2 >> Faces[FaceIndex].NIndex3
					>> Faces[FaceIndex].VIndex2 >> Input2 >> Faces[FaceIndex].TIndex2 >> Input2 >> Faces[FaceIndex].NIndex2
					>> Faces[FaceIndex].VIndex1 >> Input2 >> Faces[FaceIndex].TIndex1 >> Input2 >> Faces[FaceIndex].NIndex1;
				++FaceIndex;
			}
		}

		// 나머지 행을 읽음
		while (Input != '\n')
			FIn.get(Input);

		// 다음 줄의 시작 부분을 읽기 시작
		FIn.get(Input);
	}

	FIn.close();

	FOut.open("Model/Model.txt");

	FOut << "Vertex Count:\t" << (InFaceCount * 3) << std::endl;
	FOut << std::endl;
	FOut << "Data:\t" << std::endl;
	FOut << std::endl;

	for (int i = 0; i < FaceIndex; i++)
	{
		VIndex = Faces[i].VIndex1 - 1;
		TIndex = Faces[i].TIndex1 - 1;
		NIndex = Faces[i].NIndex1 - 1;

		FOut << Vertices[VIndex].X << ' ' << Vertices[VIndex].Y << ' ' << Vertices[VIndex].Z << ' '
			<< TexCoords[TIndex].X << ' ' << TexCoords[TIndex].Y << ' '
			<< Normals[NIndex].X << ' ' << Normals[NIndex].Y << ' ' << Normals[NIndex].Z << std::endl;

		VIndex = Faces[i].VIndex2 - 1;
		TIndex = Faces[i].TIndex2 - 1;
		NIndex = Faces[i].NIndex2 - 1;

		FOut << Vertices[VIndex].X << ' ' << Vertices[VIndex].Y << ' ' << Vertices[VIndex].Z << ' '
			<< TexCoords[TIndex].X << ' ' << TexCoords[TIndex].Y << ' '
			<< Normals[NIndex].X << ' ' << Normals[NIndex].Y << ' ' << Normals[NIndex].Z << std::endl;

		VIndex = Faces[i].VIndex3 - 1;
		TIndex = Faces[i].TIndex3 - 1;
		NIndex = Faces[i].NIndex3 - 1;

		FOut << Vertices[VIndex].X << ' ' << Vertices[VIndex].Y << ' ' << Vertices[VIndex].Z << ' '
			<< TexCoords[TIndex].X << ' ' << TexCoords[TIndex].Y << ' '
			<< Normals[NIndex].X << ' ' << Normals[NIndex].Y << ' ' << Normals[NIndex].Z << std::endl;
	}

	FOut.close();

	DX_DELETE_ARRAY(Vertices);
	DX_DELETE_ARRAY(TexCoords);
	DX_DELETE_ARRAY(Normals);
	DX_DELETE_ARRAY(Faces);
	return true;
}
