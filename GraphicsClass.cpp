#include "stdafx.h"
#include "GraphicsClass.h"
#include "D3DClass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "ColorShaderClass.h"
#include "TextureShaderClass.h"
#include "LightClass.h"
#include "LightShaderClass.h"

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

    if (!Model->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), "Model/Cube.txt", "Textures/stone01.tga"))
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

	//TextureShader = new TextureShaderClass();
	//if (!TextureShader)
	//{
	//	DX_DELETE(Direct3D);
	//	DX_DELETE(Camera);
	//	DX_DELETE(Model);
	//	return false;
	//}

	//if (!TextureShader->Initialize(Direct3D->GetDevice(), InHwnd))
	//{
	//	DX_DELETE(Direct3D);
	//	DX_DELETE(Camera);
	//	DX_DELETE(Model);
	//	DX_DELETE(TextureShader);
	//	MessageBox(InHwnd, TEXT("Could not initialize the texture shader object"), TEXT("Error"), MB_OK);
	//	return false;
	//}

    LightShader = new LightShaderClass();
    if (!LightShader)
        return false;

    if (!LightShader->Initialize(Direct3D->GetDevice(), InHwnd))
    {
        MessageBox(InHwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
        return false;
    }

    Light = new LightClass();
    if (!Light)
        return false;

    Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
    Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
    Light->SetDirection(0.0f, 0.0f, 1.0f);

    return true;
}


void GraphicsClass::Shutdown()
{
    //if (ColorShader)
    //{
    //    ColorShader->Shutdown();
    //    DX_DELETE(ColorShader);
    //}
	//if (TextureShader)
	//{
 //       TextureShader->Shutdown();
	//	DX_DELETE(TextureShader);
	//}
    
    DX_DELETE(Light);
    if (LightShader)
    {
        LightShader->Shutdown();
        DX_DELETE(LightShader);
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


bool GraphicsClass::Frame()
{
    static float Rotation = 0.0f;

    Rotation += (float)XM_PI * 0.01f;
    if (Rotation > 360.0f)
        Rotation -= 360.0f;

    return Render(Rotation);
}


bool GraphicsClass::Render(float InRotation)
{
    Direct3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

    Camera->Render();

    XMMATRIX WorldMatrix, ViewMatrix, ProjectionMatrix;
    Direct3D->GetWorldMatrix(WorldMatrix);
    Camera->GetViewMatrix(ViewMatrix);
    Direct3D->GetProjectionMatrix(ProjectionMatrix);

    WorldMatrix = XMMatrixRotationY(InRotation);

    Model->Bind(Direct3D->GetDeviceContext());
    //ColorShader->Render(Direct3D->GetDeviceContext(), Model->GetIndexCount(), WorldMatrix, ViewMatrix, ProjectionMatrix);
    //TextureShader->Render(Direct3D->GetDeviceContext(), Model->GetIndexCount(), WorldMatrix, ViewMatrix, ProjectionMatrix, Model->GetTexture());
    if (!LightShader->Render(Direct3D->GetDeviceContext(), Model->GetIndexCount(), WorldMatrix, ViewMatrix, ProjectionMatrix
        , Model->GetTexture(), Light->GetDirection(), Light->GetAmbientColor(), Light->GetDiffuseColor()))
    {
        return false;
    }

    Direct3D->EndScene();
    return true;
}