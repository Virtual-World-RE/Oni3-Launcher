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

#include "Resource.h"

#define MAX_LOADSTRING 100
#define MAXPATH_LENGTH 1024
#define MAX_STRERROR_S_LENGTH 94
#define MAX_SMALL_STRING 100

#define MAIN_WINDOW_WIDTH 300
#define MAIN_WINDOW_HEIGHT 500

#define D3DFMT_MODE D3DFMT_X8R8G8B8

#define DEFAULT_STYLE WS_CHILD | WS_VISIBLE
#define DROPDOWN_COMBO_BOX DEFAULT_STYLE | CBS_DROPDOWNLIST | WS_VSCROLL | WS_TABSTOP

#ifdef UNICODE
  #define WINMAIN wWinMain
#else
  #define WINMAIN WinMain
#endif

// Sub-function for WndProc()
LRESULT          wndProcCreate(HWND hWnd);
LRESULT          wndProcCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// Prototype of OniLauncher.cpp functions:
BOOL             MyRegisterClass(HINSTANCE hInstance);
BOOL             InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

// Reversed functions
LRESULT          pseudoWinMain(HWND hWnd);
BOOL             resetGlobalHKEY();
BOOL             setCurrentHKEY(HKEY hKeyId);
BOOL             getRegKeyValue(LPCTSTR lpValue, LPTSTR lpData, LPDWORD lpcbData);
BOOL             lookForRegKey(LPCTSTR lpSubKey);

// DirectX 9 related functions
LONG             initD3D();
VOID             destroyD3D();
BOOL             initDisplaysModes();
VOID             destroyDisplaysModes();