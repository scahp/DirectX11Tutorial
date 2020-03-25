#include "stdafx.h"
#include "ModelClass.h"
#include "TextureClass.h"
#include <fstream>

ModelClass::ModelClass()
{

}

ModelClass::ModelClass(const ModelClass& InClass)
{

}

ModelClass::~ModelClass()
{

}

bool ModelClass::Initialize(ID3D11Device* InDevice, ID3D11DeviceContext* InDeviceContext, const char* InModelFilename
	, const WCHAR* InTextureFilename1, const WCHAR* InTextureFilename2)
{
	if (!LoadModel(InModelFilename))
		return false;

	CalculateModelVectors();

	if (!InitializeBuffers(InDevice))
		return false;

	return LoadTextures(InDevice, InTextureFilename1, InTextureFilename2);
}

void ModelClass::Shutdown()
{
	ReleaseTextures();
	ShutdownBuffers();
	ReleaseModel();
}

void ModelClass::Bind(ID3D11DeviceContext* InDeviceContext)
{
	BindBuffers(InDeviceContext);
}

int ModelClass::GetIndexCount()
{
	return IndexCount;
}

ID3D11ShaderResourceView** ModelClass::GetTextureArray()
{
	return TextureArray->GetTextureArray();
}

bool ModelClass::LoadTextures(ID3D11Device* InDevice, const WCHAR* InFilename1, const WCHAR* InFilename2)
{
	TextureArray = new TextureArrayClass();
	if (!TextureArray)
		return false;

	return TextureArray->Initialize(InDevice, InFilename1, InFilename2);
}

void ModelClass::ReleaseTextures()
{
	if (TextureArray)
	{
		TextureArray->Shutdown();
		DX_DELETE(TextureArray);
	}
}

bool ModelClass::InitializeBuffers(ID3D11Device* InDevice)
{
	VertexType* Vertices = new VertexType[VertexCount];
	if (!Vertices)
		return false;

	unsigned long* Indices = new unsigned long[IndexCount];
	if (!Indices)
	{
		DX_DELETE_ARRAY(Vertices);
		return false;
	}

	for (int i = 0; i < VertexCount; ++i)
	{
		Vertices[i].Position = XMFLOAT3(Model[i].X, Model[i].Y, Model[i].Z);
		Vertices[i].Texture = XMFLOAT2(Model[i].TU, Model[i].TV);
		Vertices[i].Normal = XMFLOAT3(Model[i].NX, Model[i].NY, Model[i].NZ);
		Vertices[i].Tangent = XMFLOAT3(Model[i].TX, Model[i].TY, Model[i].TZ);
		Vertices[i].Binormal = XMFLOAT3(Model[i].BX, Model[i].BY, Model[i].BZ);

		Indices[i] = i;
	}

	D3D11_BUFFER_DESC VertexBufferDesc;
	VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	VertexBufferDesc.ByteWidth = sizeof(VertexType) * VertexCount;
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = 0;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;

	// Subresource 구조에 정점 데이터에 대한 포인터 제공
	D3D11_SUBRESOURCE_DATA VertexData;
	VertexData.pSysMem = Vertices;
	VertexData.SysMemPitch = 0;
	VertexData.SysMemSlicePitch = 0;

	if (FAILED(InDevice->CreateBuffer(&VertexBufferDesc, &VertexData, &VertexBuffer)))
	{
		DX_DELETE_ARRAY(Vertices);
		DX_DELETE_ARRAY(Indices);
		return false;
	}

	D3D11_BUFFER_DESC IndexBufferDesc;
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth = sizeof(unsigned long) * IndexCount;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;

	// Subresource 구조에 인덱스 데이터에 대한 포인터 제공
	D3D11_SUBRESOURCE_DATA IndexData;
	IndexData.pSysMem = Indices;
	IndexData.SysMemPitch = 0;
	IndexData.SysMemSlicePitch = 0;

	if (FAILED(InDevice->CreateBuffer(&IndexBufferDesc, &IndexData, &IndexBuffer)))
	{
		DX_DELETE_ARRAY(Vertices);
		DX_DELETE_ARRAY(Indices);
		DX_RELEASE(VertexBuffer);
		return false;
	}

	DX_DELETE_ARRAY(Vertices);
	DX_DELETE_ARRAY(Indices);

	return true;
}

void ModelClass::ShutdownBuffers()
{
	DX_RELEASE(IndexBuffer);
	DX_RELEASE(VertexBuffer);
}

void ModelClass::BindBuffers(ID3D11DeviceContext* InDeviceContext)
{
	unsigned int Stride = sizeof(VertexType);
	unsigned int Offset = 0;

	InDeviceContext->IASetVertexBuffers(0, 1, &VertexBuffer, &Stride, &Offset);
	InDeviceContext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	InDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool ModelClass::LoadModel(const char* InFilename)
{
	std::ifstream FIn;
	FIn.open(InFilename);

	if (FIn.fail())
		return false;

	char Input = 0;
	FIn.get(Input);
	while (Input != ':')
		FIn.get(Input);

	FIn >> VertexCount;
	IndexCount = VertexCount;

	Model = new ModelType[VertexCount];
	if (!Model)
		return false;

	FIn.get(Input);
	while (Input != ':')
		FIn.get(Input);
	FIn.get(Input);
	FIn.get(Input);

	for (int i = 0; i < VertexCount; ++i)
	{
		FIn >> Model[i].X >> Model[i].Y >> Model[i].Z;
		FIn >> Model[i].TU >> Model[i].TV;
		FIn >> Model[i].NX >> Model[i].NY >> Model[i].NZ;
	}

	FIn.close();
	return true;
}

void ModelClass::ReleaseModel()
{
	DX_DELETE_ARRAY(Model);
}

void ModelClass::CalculateModelVectors()
{
	TempVertexType Vertex1, Vertex2, Vertex3;
	VectorType Tangent, Binormal, Normal;

	int FaceCount = VertexCount / 3;

	int Index = 0;

	for (int i = 0; i < FaceCount; ++i)
	{
		Vertex1.X = Model[Index].X;
		Vertex1.Y = Model[Index].Y;
		Vertex1.Z = Model[Index].Z;
		Vertex1.TU = Model[Index].TU;
		Vertex1.TV = Model[Index].TV;
		Vertex1.NX = Model[Index].NX;
		Vertex1.NY = Model[Index].NY;
		Vertex1.NZ = Model[Index].NZ;
		Index++;

		Vertex2.X = Model[Index].X;
		Vertex2.Y = Model[Index].Y;
		Vertex2.Z = Model[Index].Z;
		Vertex2.TU = Model[Index].TU;
		Vertex2.TV = Model[Index].TV;
		Vertex2.NX = Model[Index].NX;
		Vertex2.NY = Model[Index].NY;
		Vertex2.NZ = Model[Index].NZ;
		Index++;

		Vertex3.X = Model[Index].X;
		Vertex3.Y = Model[Index].Y;
		Vertex3.Z = Model[Index].Z;
		Vertex3.TU = Model[Index].TU;
		Vertex3.TV = Model[Index].TV;
		Vertex3.NX = Model[Index].NX;
		Vertex3.NY = Model[Index].NY;
		Vertex3.NZ = Model[Index].NZ;
		Index++;

		CalculateTangentBinormal(Vertex1, Vertex2, Vertex3, Tangent, Binormal);

		CalculateNormal(Tangent, Binormal, Normal);

		Model[Index - 1].NX = Normal.X;
		Model[Index - 1].NY = Normal.Y;
		Model[Index - 1].NZ = Normal.Z;
		Model[Index - 1].TX = Tangent.X;
		Model[Index - 1].TY = Tangent.Y;
		Model[Index - 1].TZ = Tangent.Z;
		Model[Index - 1].BX = Binormal.X;
		Model[Index - 1].BY = Binormal.Y;
		Model[Index - 1].BZ = Binormal.Z;

		Model[Index - 2].NX = Normal.X;
		Model[Index - 2].NY = Normal.Y;
		Model[Index - 2].NZ = Normal.Z;
		Model[Index - 2].TX = Tangent.X;
		Model[Index - 2].TY = Tangent.Y;
		Model[Index - 2].TZ = Tangent.Z;
		Model[Index - 2].BX = Binormal.X;
		Model[Index - 2].BY = Binormal.Y;
		Model[Index - 2].BZ = Binormal.Z;

		Model[Index - 3].NX = Normal.X;
		Model[Index - 3].NY = Normal.Y;
		Model[Index - 3].NZ = Normal.Z;
		Model[Index - 3].TX = Tangent.X;
		Model[Index - 3].TY = Tangent.Y;
		Model[Index - 3].TZ = Tangent.Z;
		Model[Index - 3].BX = Binormal.X;
		Model[Index - 3].BY = Binormal.Y;
		Model[Index - 3].BZ = Binormal.Z;
	}
}

void ModelClass::CalculateTangentBinormal(TempVertexType InVertex1, TempVertexType InVertex2, TempVertexType InVertex3,
	VectorType& OutTangent, VectorType& OutBinormal)
{
	float Vector1[3], Vector2[3];
	float TUVector[2], TVVector[2];

	// 현재 표면의 두 벡터를 계산합니다.
	Vector1[0] = InVertex2.X - InVertex1.X;
	Vector1[1] = InVertex2.Y - InVertex1.Y;
	Vector1[2] = InVertex2.Z - InVertex1.Z;

	Vector2[0] = InVertex3.X - InVertex1.X;
	Vector2[1] = InVertex3.Y - InVertex1.Y;
	Vector2[2] = InVertex3.Z - InVertex1.Z;

	// tu 및 tv 텍스처 공간 벡터를 계산합니다.
	TUVector[0] = InVertex2.TU - InVertex1.TU;
	TVVector[0] = InVertex2.TV - InVertex1.TV;

	TUVector[1] = InVertex3.TU - InVertex1.TU;
	TVVector[1] = InVertex3.TV - InVertex1.TV;

	// 탄젠트 / 바이 노멀 방정식의 분모를 계산합니다.
	float den = 1.0f / (TUVector[0] * TVVector[1] - TUVector[1] * TVVector[0]);

	// 교차 곱을 계산하고 계수로 곱하여 접선과 비 구식을 얻습니다.
	OutTangent.X = (TVVector[1] * Vector1[0] - TVVector[0] * Vector2[0]) * den;
	OutTangent.Y = (TVVector[1] * Vector1[1] - TVVector[0] * Vector2[1]) * den;
	OutTangent.Z = (TVVector[1] * Vector1[2] - TVVector[0] * Vector2[2]) * den;

	OutBinormal.X = (TUVector[0] * Vector2[0] - TUVector[1] * Vector1[0]) * den;
	OutBinormal.Y = (TUVector[0] * Vector2[1] - TUVector[1] * Vector1[1]) * den;
	OutBinormal.Z = (TUVector[0] * Vector2[2] - TUVector[1] * Vector1[2]) * den;

	// 이 법선의 길이를 계산합니다.
	float length = sqrt((OutTangent.X * OutTangent.X) + (OutTangent.Y * OutTangent.Y) + (OutTangent.Z * OutTangent.Z));

	// 법선을 표준화 한 다음 저장합니다.
	OutTangent.X = OutTangent.X / length;
	OutTangent.Y = OutTangent.Y / length;
	OutTangent.Z = OutTangent.Z / length;

	// 이 법선의 길이를 계산합니다.
	length = sqrt((OutBinormal.X * OutBinormal.X) + (OutBinormal.Y * OutBinormal.Y) + (OutBinormal.Z * OutBinormal.Z));

	// 법선을 표준화 한 다음 저장합니다.
	OutBinormal.X = OutBinormal.X / length;
	OutBinormal.Y = OutBinormal.Y / length;
	OutBinormal.Z = OutBinormal.Z / length;
}


void ModelClass::CalculateNormal(VectorType InTangent, VectorType InBinormal, VectorType& OutNormal)
{
	// 법선 벡터를 줄 수있는 접선과 binormal의 외적을 계산합니다.
	OutNormal.X = (InTangent.Y * InBinormal.Z) - (InTangent.Z * InBinormal.Y);
	OutNormal.Y = (InTangent.Z * InBinormal.X) - (InTangent.X * InBinormal.Z);
	OutNormal.Z = (InTangent.X * InBinormal.Y) - (InTangent.Y * InBinormal.X);

	// 법선의 길이를 계산합니다.
	float length = sqrt((OutNormal.X * OutNormal.X) + (OutNormal.Y * OutNormal.Y) + (OutNormal.Z * OutNormal.Z));

	// 법선을 표준화합니다.
	OutNormal.X = OutNormal.X / length;
	OutNormal.Y = OutNormal.Y / length;
	OutNormal.Z = OutNormal.Z / length;
}
