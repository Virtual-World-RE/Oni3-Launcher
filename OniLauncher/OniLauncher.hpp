/*
 ** This file is part of the Onimusha 3:RE Project !
 **
 ** You should have received a copy of the LICENSE along with this program.
 **
 ** Author: Gabriel GRONDIN (GGLinnk) <gglinnk@protonmail.com>
 ** Contributors (See CONTRIBUTION.md):
 */

#pragma once

#ifdef UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#include "pch.h"
#include "error.h"
#include "Resource.h"
#include "OriLauncher.hpp"

#define MAX_LOADSTRING 100
#define MAX_STRERROR_S_LENGTH 94
#define MAX_SMALL_STRING 100

#define MAIN_WINDOW_WIDTH 250
#define MAIN_WINDOW_HEIGHT 400

#define D3DFMT_MODE D3DFMT_X8R8G8B8

#define WINDOWS_STYLE WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX
#define DEFAULT_STYLE WS_CHILD | WS_VISIBLE
#define DROPDOWN_COMBO_BOX DEFAULT_STYLE | CBS_DROPDOWNLIST | WS_VSCROLL | WS_TABSTOP

#ifdef UNICODE
  #define WINMAIN wWinMain
#else
  #define WINMAIN WinMain
#endif

using json = nlohmann::json;

typedef struct MONITORDISPLAYMODES
{
    D3DDISPLAYMODE displayMode;
    struct MONITORDISPLAYMODES *next;
} MONITORDISPLAYMODES;

typedef struct
{
    UINT width;
    UINT height;
} RESOLUTION;

// Prototype of OniLauncher.cpp windows related functions
BOOL             MyRegisterClass(HINSTANCE hInstance);
BOOL             InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

/// Sub-function for main window process WndProc()
LRESULT          wndProcCreate(HWND hWnd);
LRESULT          wndProcCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

LPUINT           getNumbers(LPTSTR str);

class OniLauncher
{
private:
    BOOL        configured = FALSE;
    LPDIRECT3D9 d3d = NULL;

    UINT        currentMonitor = 0;
    RESOLUTION  currentResolution = { 0, 0 };
    UINT        currentRefreshRate = 0;
    BOOL        debugEnabled = FALSE;

    HWND        hWnd = NULL;
    HWND        monitorComboBox = NULL;
    HWND        resolutionComboBox = NULL;
    HWND        refreshRateComboBox = NULL;

    VOID        initD3D();
    BOOL        initMonitorDisplayModes();

    BOOL        jsonLoadConfig();
    BOOL        jsonConfigExists();

    VOID        destroyMonitorDisplayModes();
    VOID        destroyD3D();
public:
    MONITORDISPLAYMODES **monitorDisplayModes = NULL;

    OniLauncher();

    VOID        setHandlers(HWND hWnd, HWND monitorComboBox, HWND resolutionComboBox, HWND refreshRateComboBox);

    UINT        fetchCurrentMonitor();
    RESOLUTION  fetchCurrentResolution();
    UINT        fetchCurrentRefreshRate();
                
    VOID        resetMonitor();
    VOID        resetResolution();
    VOID        resetRefreshRate();
                
    UINT        getCurrentMonitor();
    RESOLUTION  getCurrentResolution();
    UINT        getCurrentRefreshRate();
                
    VOID        fillMonitorComboBox();
    VOID        fillResolutionComboBox();
    VOID        fillRefreshComboBox();
                
    LRESULT     saveConfigFile();

    ~OniLauncher();
};
