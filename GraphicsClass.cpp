#include "stdafx.h"
#include "GraphicsClass.h"
#include "D3DClass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "ColorShaderClass.h"
#include "TextureShaderClass.h"
#include "LightClass.h"
#include "LightShaderClass.h"
#include "BitmapClass.h"
#include "ModelListClass.h"
#include "FrustumClass.h"
#include "MultiTextureShaderClass.h"
#include "BumpMapShaderClass.h"
#include "RenderTextureClass.h"
#include "DebugWindowClass.h"

GraphicsClass::GraphicsClass()
{
}


GraphicsClass::GraphicsClass(const GraphicsClass& InClass)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int InScreenWidth, int InScreenHeight, HWND InHwnd)
{
    Direct3D = (D3DClass*)_aligned_malloc(sizeof(D3DClass), 16);
    if (!Direct3D)
        return false;

    if (!Direct3D->Initialize(InScreenWidth, InScreenHeight, VSYNC_ENABLED,
        InHwnd, FULL_SCREEN, SCREEN_DEPTH_FAR, SCREEN_DEPTH_NEAR))
    {
        MessageBox(InHwnd, TEXT("Could not initialize Direct3D"), L"Error", MB_OK);
        return false;
    }

    Camera = new CameraClass();
    if (!Camera)
    {
        DX_DELETE(Direct3D);
        return false;
    }

    Camera->SetPosition(0.0f, 0.0f, -5.0f);

    Model = new ModelClass();
    if (!Model)
    {
        DX_DELETE(Direct3D);
        DX_DELETE(Camera);
        return false;
    }

    if (!Model->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), "Model/cube.txt", TEXT("Textures/stone01.dds"), TEXT("Textures/bump01.dds")))
    {
		DX_DELETE(Direct3D);
		DX_DELETE(Camera);
        DX_DELETE(Model);
        MessageBox(InHwnd, TEXT("Could not initialize the model object"), TEXT("Error"), MB_OK);
        return false;
    }

  //  ColorShader = new ColorShaderClass();
  //  if (!ColorShader)
  //  {
		//DX_DELETE(Direct3D);
		//DX_DELETE(Camera);
		//DX_DELETE(Model);
  //      return false;
  //  }


  //  if (!ColorShader->Initialize(Direct3D->GetDevice(), InHwnd))
  //  {
		//DX_DELETE(Direct3D);
		//DX_DELETE(Camera);
		//DX_DELETE(Model);
  //      DX_DELETE(ColorShader);
  //      MessageBox(InHwnd, TEXT("Could not initialize the color shader object"), TEXT("Error"), MB_OK);
  //      return false;
  //  }

	TextureShader = new TextureShaderClass();
	if (!TextureShader)
	{
		DX_DELETE(Direct3D);
		DX_DELETE(Camera);
		DX_DELETE(Model);
		return false;
	}

	if (!TextureShader->Initialize(Direct3D->GetDevice(), InHwnd))
	{
		DX_DELETE(Direct3D);
		DX_DELETE(Camera);
		DX_DELETE(Model);
		DX_DELETE(TextureShader);
		MessageBox(InHwnd, TEXT("Could not initialize the texture shader object"), TEXT("Error"), MB_OK);
		return false;
	}

 //   Bitmap = new BitmapClass();
 //   if (!Bitmap)
 //       return false;
 //   
 //   if (!Bitmap->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), InScreenWidth, InScreenHeight, "Textures/stone01.tga", 256, 256))
 //   {
 //       MessageBox(InHwnd, TEXT("Could not initlaize the bitmap object."), L"Error", MB_OK);
 //       return false;
 //   }

 //   LightShader = new LightShaderClass();
 //   if (!LightShader)
 //       return false;

 //   if (!LightShader->Initialize(Direct3D->GetDevice(), InHwnd))
 //   {
 //       MessageBox(InHwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
 //       return false;
 //   }

    Light = new LightClass();
    if (!Light)
        return false;

    //Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
    Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
    Light->SetDirection(0.0f, 0.0f, 1.0f);
 //   Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
 //   Light->SetSpecularPower(32.0f);

 //   ModelList = new ModelListClass();
 //   if (!ModelList)
 //       return false;

	//if (!ModelList->Initialize(25))
 //   {
 //       MessageBox(InHwnd, TEXT("Could not initliaze the model list object."), TEXT("Error"), MB_OK);
 //       return false;
 //   }

    //Frustum = new FrustumClass();
    //if (!Frustum)
    //    return false;

    BumpMapShader = new BumpMapShaderClass();
    if (!BumpMapShader)
        return false;

    if (!BumpMapShader->Initialize(Direct3D->GetDevice(), InHwnd))
    {
        MessageBox(InHwnd, TEXT("Could not initialize the bump map shader object."), TEXT("Error"), MB_OK);
        return false;
    }

    //MultiTextureShader = new MultiTextureShaderClass();
    //if (!MultiTextureShader)
    //    return false;

    //if (!MultiTextureShader->Initialize(Direct3D->GetDevice(), InHwnd))
    //{
    //    MessageBox(InHwnd, TEXT("Could not initialize the multitexture shader object"), TEXT("Error"), MB_OK);
    //    return false;
    //}

	// 렌더링 텍스처 객체를 생성한다.
	RenderTexture = new RenderTextureClass();
	if (!RenderTexture)
		return false;

	// 렌더링 텍스처 객체를 초기화한다.
	if (!RenderTexture->Initialize(Direct3D->GetDevice(), InScreenWidth, InScreenHeight))
		return false;

	// 디버그 창 객체를 만듭니다.
	DebugWindow = new DebugWindowClass();
	if (!DebugWindow)
		return false;

	// 디버그 창 객체를 초기화 합니다.
	if (!DebugWindow->Initialize(Direct3D->GetDevice(), InScreenWidth, InScreenHeight, 100, 100))
	{
		MessageBox(InHwnd, L"Could not initialize the debug window object.", L"Error", MB_OK);
		return false;
	}

    return true;
}


void GraphicsClass::Shutdown()
{
    //DX_DELETE(Frustum);
    //if (ModelList)
    //{
    //    ModelList->Shutdown();
    //    DX_DELETE(ModelList);
    //}

    //if (ColorShader)
    //{
    //    ColorShader->Shutdown();
    //    DX_DELETE(ColorShader);
    //}

	//if (Bitmap)
	//{
	//	Bitmap->Shutdown();
	//	DX_DELETE(Bitmap);
	//}
	
    if (TextureShader)
	{
        TextureShader->Shutdown();
		DX_DELETE(TextureShader);
	}
   
	if (DebugWindow)
	{
		DebugWindow->Shutdown();
		DX_DELETE(DebugWindow);
	}

	if (RenderTexture)
	{
		RenderTexture->Shutdown();
		DX_DELETE(RenderTexture);
	}

    DX_DELETE(Light);

    if (BumpMapShader)
    {
        BumpMapShader->Shutdown();
        DX_DELETE(BumpMapShader);
    }

    //if (LightShader)
    //{
    //    LightShader->Shutdown();
    //    DX_DELETE(LightShader);
    //}    

    if (MultiTextureShader)
    {
        MultiTextureShader->Shutdown();
        DX_DELETE(MultiTextureShader);
    }

    if (Model)
    {
        Model->Shutdown();
        DX_DELETE(Model);
    }

    DX_DELETE(Camera);

    if (Direct3D)
    {
        Direct3D->Shutdown();
        _aligned_free(Direct3D);
        Direct3D = nullptr;
    }
}


bool GraphicsClass::Frame(float RotationY)
{
    return Render(RotationY);
}

bool GraphicsClass::RenderToTexture(float InRotation)
{
	// 렌더링 대상을 렌더링에 맞게 설정합니다.
	RenderTexture->SetRenderTarget(Direct3D->GetDeviceContext(), Direct3D->GetDepthStencilView());

	// 렌더링을 텍스처에 지웁니다.
	RenderTexture->ClearRenderTarget(Direct3D->GetDeviceContext(), Direct3D->GetDepthStencilView(), 0.0f, 0.0f, 1.0f, 1.0f);

	// 이제 장면을 렌더링하면 백 버퍼 대신 텍스처로 렌더링됩니다.
	if (!RenderScene())
		return false;

	// 렌더링 대상을 원래의 백 버퍼로 다시 설정하고 렌더링에 대한 렌더링을 더 이상 다시 설정하지 않습니다.
	Direct3D->SetBackBufferRenderTarget();

	return true;
}

bool GraphicsClass::RenderScene()
{
	Camera->Render();

	XMMATRIX WorldMatrix, ViewMatrix, ProjectionMatrix, OrthoMatrix;
	Direct3D->GetWorldMatrix(WorldMatrix);
	Camera->GetViewMatrix(ViewMatrix);
	Direct3D->GetProjectionMatrix(ProjectionMatrix);
	Direct3D->GetOrthoMatrix(OrthoMatrix);

	static float Rotation = 0.0f;
	Rotation += (float)XM_PI * 0.0025f;
	if (Rotation > 360.0f)
	{
		Rotation -= 360.0f;
	}
	WorldMatrix = XMMatrixRotationY(Rotation);

	Model->Bind(Direct3D->GetDeviceContext());

	return BumpMapShader->Render(Direct3D->GetDeviceContext(), Model->GetIndexCount(), WorldMatrix, ViewMatrix, ProjectionMatrix, Model->GetTextureArray(),
		Light->GetDirection(), Light->GetDiffuseColor());

	//MultiTextureShader->Render(Direct3D->GetDeviceContext(), Model->GetIndexCount()
	//    , WorldMatrix, ViewMatrix, ProjectionMatrix, Model->GetTextureArray());

 //   Frustum->ConstructFrustum(SCREEN_DEPTH_FAR, ProjectionMatrix, ViewMatrix);

 //   int ModelCount = ModelList->GetModelCount();

 //   int RenderCount = 0;
	//float PositionX = 0;
	//float PositionY = 0;
	//float PositionZ = 0;
	//float Radius = 1.0f;
	//XMFLOAT4 Color;

 //   for (int Index = 0; Index < ModelCount; ++Index)
 //   {
 //       ModelList->GetData(Index, PositionX, PositionY, PositionZ, Color);

 //       if (Frustum->CheckSphere(PositionX, PositionY, PositionZ, Radius))
 //       {
 //           WorldMatrix = XMMatrixTranslation(PositionX, PositionY, PositionZ);

 //           Model->Bind(Direct3D->GetDeviceContext());

 //           LightShader->Render(Direct3D->GetDeviceContext(), Model->GetIndexCount(), WorldMatrix, ViewMatrix, ProjectionMatrix
 //               , Model->GetTexture(), Light->GetDirection(), Light->GetAmbientColor(), Color, Camera->GetPosition()
 //               , Light->GetSpecularColor(), Light->GetSpecularPower());

 //           Direct3D->GetWorldMatrix(WorldMatrix);
 //           ++RenderCount;
 //       }
 //   }
}

bool GraphicsClass::Render(float InRotation)
{
    if (!RenderToTexture(InRotation))
        return false;

    Direct3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

    if (!RenderScene())
        return false;

	// 모든 2D 렌더링을 시작하려면 Z 버퍼를 끕니다.
	Direct3D->TurnZBufferOff();

	// 카메라 및 d3d 객체에서 월드, 뷰 및 투영 행렬을 가져옵니다
	XMMATRIX worldMatrix, viewMatrix, orthoMatrix;

	Camera->GetViewMatrix(viewMatrix);
	Direct3D->GetWorldMatrix(worldMatrix);
	Direct3D->GetOrthoMatrix(orthoMatrix);

	// 디버그 윈도우 버텍스와 인덱스 버퍼를 그래픽 파이프 라인에 배치하여 그리기를 준비합니다.
	if (!DebugWindow->Bind(Direct3D->GetDeviceContext(), 50, 50))
	{
		return false;
	}

	// 텍스처 셰이더를 사용해 디버그 윈도우를 렌더링한다.
	if (!TextureShader->Render(Direct3D->GetDeviceContext(), DebugWindow->GetIndexCount(), worldMatrix, viewMatrix,
		orthoMatrix, RenderTexture->GetShaderResourceView()))
	{
		return false;
	}

	// 모든 2D 렌더링이 완료되었으므로 Z 버퍼를 다시 켜십시오.
	Direct3D->TurnZBufferOn();

	// 버퍼의 내용을 화면에 출력합니다
	Direct3D->EndScene();

    //////////////////////////////////////////////////////////////////////////

    Direct3D->EndScene();
    return true;
}