#include "stdafx.h"
#include "GraphicsClass.h"
#include "D3DClass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "ColorShaderClass.h"
#include "TextureShaderClass.h"

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

    if (!Model->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), "Textures/stone01.tga"))
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

    return true;
}


void GraphicsClass::Shutdown()
{
    //if (ColorShader)
    //{
    //    ColorShader->Shutdown();
    //    DX_DELETE(ColorShader);
    //}
	if (TextureShader)
	{
        TextureShader->Shutdown();
		DX_DELETE(TextureShader);
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
    return Render();
}


bool GraphicsClass::Render()
{
    Direct3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

    Camera->Render();

    XMMATRIX WorldMatrix, ViewMatrix, ProjectionMatrix;
    Direct3D->GetWorldMatrix(WorldMatrix);
    Camera->GetViewMatrix(ViewMatrix);
    Direct3D->GetProjectionMatrix(ProjectionMatrix);

    Model->Bind(Direct3D->GetDeviceContext());
    //ColorShader->Render(Direct3D->GetDeviceContext(), Model->GetIndexCount(), WorldMatrix, ViewMatrix, ProjectionMatrix);
    TextureShader->Render(Direct3D->GetDeviceContext(), Model->GetIndexCount(), WorldMatrix, ViewMatrix, ProjectionMatrix, Model->GetTexture());

    Direct3D->EndScene();
    return true;
}