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
	SetConsoleTitle(TEXT("�׽�Ʈ�� �ܼ�"));

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

	// ����, �ؽ��� ��ǥ �� ������ ������ ������ �о���Դϴ�.
	// �߿� : Maya�� ������ ��ǥ�踦 ����ϱ� ������ �޼� ��ǥ��� ��ȯ�մϴ�.
	FIn.get(Input);
	while (!FIn.eof())
	{
		if (Input == 'v')
		{
			FIn.get(Input);

			// ������ ����
			if (Input == ' ')
			{
				FIn >> Vertices[VertexIndex].X >> Vertices[VertexIndex].Y >> Vertices[VertexIndex].Z;

				// Z ������ ������ �޼� �ý������� ����
				Vertices[VertexIndex].Z *= -1.0f;
				++VertexIndex;
			}

			// �ؽ��� UV ��ǥ�� ����
			if (Input == 't')
			{
				FIn >> TexCoords[TexCoordIndex].X >> TexCoords[TexCoordIndex].Y;

				// V �ؽ��� ��ǥ�� �޼� �ý������� ����
				TexCoords[TexCoordIndex].Y = 1.0f - TexCoords[TexCoordIndex].Y;
				++TexCoordIndex;
			}

			// ������ ����
			if (Input == 'n')
			{
				FIn >> Normals[NormalIndex].X >> Normals[NormalIndex].Y >> Normals[NormalIndex].Z;

				// Z ������ �������� �޼� �ý������� ����
				Normals[NormalIndex].Z *= -1.0f;
				++NormalIndex;
			}
		}

		// Face�� ����
		if (Input == 'f')
		{
			FIn.get(Input);
			if (Input == ' ')
			{
				// ������ �ý��ۿ��� �޼� �ý������� ��ȯ�ϱ� ���ؼ� ǥ�� �����͸� �Ųٷ� ����
				FIn >> Faces[FaceIndex].VIndex3 >> Input2 >> Faces[FaceIndex].TIndex3 >> Input2 >> Faces[FaceIndex].NIndex3
					>> Faces[FaceIndex].VIndex2 >> Input2 >> Faces[FaceIndex].TIndex2 >> Input2 >> Faces[FaceIndex].NIndex2
					>> Faces[FaceIndex].VIndex1 >> Input2 >> Faces[FaceIndex].TIndex1 >> Input2 >> Faces[FaceIndex].NIndex1;
				++FaceIndex;
			}
		}

		// ������ ���� ����
		while (Input != '\n')
			FIn.get(Input);

		// ���� ���� ���� �κ��� �б� ����
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
