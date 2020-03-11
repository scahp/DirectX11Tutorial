#pragma once

#include <iostream>
#include <fstream>

struct VertexType
{
	float X, Y, Z;
};

struct FaceType
{
	int VIndex1, VIndex2, VIndex3;
	int TIndex1, TIndex2, TIndex3;
	int NIndex1, NIndex2, NIndex3;
};

class MayaModelConverter
{
public:
	void Do();

private:
	void OpenConsole();
	void CloseConsole();
	void GetModelFilename(char* InFilename);
	bool ReadFileCounts(const char* InFilename, int& OutVertexCount, int& OutTextureCount, int& OutNormalCount, int& OutFaceCount);
	bool LoadDataStructures(const char* InFilename, int InVertexCount, int InTextureCount, int InNormalCount, int InFaceCount);
};

