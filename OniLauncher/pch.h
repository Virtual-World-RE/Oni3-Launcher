// pch.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclure les en-têtes Windows rarement utilisés
// Windows Header Files
#include <windows.h>
#include <windowsx.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// Additional C headers
#include <tlhelp32.h>
#include <process.h>
#include <string.h>
#include <stdio.h>
#include <locale.h>
#include <shlwapi.h>
#include <strsafe.h>
#include <uxtheme.h>
#include <winbase.h>
#include <pathcch.h>
#include <shlobj_core.h>

#include <nlohmann/json.hpp>

// DirectX 9 headers
//#include <d3d9.h>
#include <dxgi.h>
#include <d3d9helper.h>


// reference additional headers your program requires here
#include <unknwn.h>
#include <winrt/base.h>
