#pragma once

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x600
#endif // _WIN32_WINNT

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN		// 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

// Windows 헤더 파일
#include <Windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include "DxDefine.h"