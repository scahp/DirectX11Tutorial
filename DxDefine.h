#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
using namespace DirectX;

///////////////////////////
//  warning C4316 처리용  //
///////////////////////////
#include "AlignedAllocationPolicy.h"

#define DX_RELEASE(x) { if (x) { x->Release(); x = nullptr; } }
#define DX_DELETE(x) { if (x) { delete x; x = nullptr; } }
#define DX_DELETE_ARRAY(x) { if (x) { delete[] x; x = nullptr; } }