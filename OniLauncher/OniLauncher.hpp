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
#include "error.hpp"
#include "Resource.h"
#include "windowsx_bugfix.hpp"
#include "Injector.hpp"

#define MAX_LANG_LENGTH 32
#define MAX_LOADSTRING 200
#define MAX_STRERROR_S_LENGTH 94
#define UINT_DIGITS 10

#define MAIN_WINDOW_WIDTH 250
#define MAIN_WINDOW_HEIGHT 400

#define D3DFMT_MODE D3DFMT_X8R8G8B8

#define WINDOWS_STYLE WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX
#define DEFAULT_STYLE WS_CHILD | WS_VISIBLE
#define DROPDOWN_COMBO_BOX DEFAULT_STYLE | CBS_DROPDOWNLIST | WS_VSCROLL

using json = nlohmann::json;

typedef struct
{
    UINT width;
    UINT height;
} RESOLUTION;

#ifdef UNICODE
#define WINMAIN wWinMain
#else
#define WINMAIN WinMain
#endif

/// <summary>
/// The Application Entry Point
/// </summary>
/// <param name="hInstance">is something called a "handle to an instance" or "handle to a module". The operating system uses this value to identify the executable (EXE) when it is loaded in memory. The instance handle is needed for certain Windows functions�for example, to load icons or bitmaps.</param>
/// <param name="hPrevInstance">has no meaning. It was used in 16-bit Windows, but is now always zero.</param>
/// <param name="pCmdLine">contains the command-line arguments as a Unicode string.</param>
/// <param name="nCmdShow">is a flag that says whether the main application window will be minimized, maximized, or shown normally.</param>
/// <returns>The return value is not used by the operating system, but you can use the return value to convey a status code to some other program that you write.</returns>
int APIENTRY WINMAIN(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PTSTR pCmdLine, _In_ int nCmdShow);

// Prototype of OniLauncher.cpp windows related functions

/// <summary>
///   Register Main Window Class
/// </summary>
/// <param name="hInstance"></param>
/// <param name="lpfnWndProc"></param>
/// <returns>
///   <para>FALSE if failed.</para>
///   <para>TRUE if succeed.</para>
/// </returns>
bool                WindowRegisterClass(HINSTANCE hInstance, WNDPROC lpfnWndProc, LPCTSTR lpszMenuName, LPCTSTR szWindowClass);

/// <summary>
///   <para>Create a main window.</para>
///   <para>Initialize global hInst from hInstance.</para>
/// </summary>
/// <param name="hInstance"></param>
/// <param name="nCmdShow">is a flag that says whether the main application window will be minimized, maximized, or shown normally.</param>
/// <returns>
///   <para>FALSE if failed.</para>
///   <para>TRUE if succeed.</para>
/// </returns>
bool                InitInstance(HINSTANCE hinstance, int nCmdShow, LPCTSTR szTitle, LPCTSTR szWindowClass);

/// <summary>
///   <para>Set a given font for a given handler.</para>
///   <para>This has to be used with EnumChildWindows.</para>
/// </summary>
/// <param name="child">: Handler to child window.</param>
/// <param name="font">: Font to be used.</param>
/// <returns>Always returns TRUE.</returns>
BOOL CALLBACK       SetFont(HWND child, LPARAM font);

/// <summary>
///   Processes messages for the main window.
/// </summary>
/// <param name="hWnd"></param>
/// <param name="message"></param>
/// <param name="wParam"></param>
/// <param name="lParam"></param>
/// <remarks>
///   Message can be one of the following one:
///   <para>WM_COMMAND - Manages application menu.</para>
///   <para>WM_PAINT - Draw the main window.</para>
///   <para>WM_DESTROY - Send a message then returns.</para>
/// </remarks>
/// <returns></returns>
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

/// Sub-function for main window process WndProc()
LRESULT             wndProcCreate(HWND hWnd);
LRESULT             wndProcCtlColorStatic(WPARAM wParam);
LRESULT             wndProcCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT             wndProcCommandButtonAction(HWND hWnd, UINT wmId, UINT iCode);
LRESULT             wndProcCommandScreenSettings(HWND hWnd, UINT wmId, UINT iCode);

/// Sub-function for about dialog window About()
INT_PTR             aboutProcCommand(HWND hDlg, UINT message, WPARAM wParam);

static int __cdecl SortModesCallback(const void *arg1, const void *arg2);

enum class DISPLAYMODE : UINT
{
    NONE,
    BORDERLESS,
    FULLSCREEN,
    WINDOWED,
};

class OniLauncher
{
private:
    bool            configLoaded = false;
    LPDIRECT3D9     d3d = NULL;
    D3DDISPLAYMODE **d3dDisplayModes = NULL;

    TCHAR       oni3GamePath[MAX_PATH] = { 0 };
    TCHAR       oni3GameExePath[MAX_PATH] = { 0 };

    UINT        selectedMonitor = 0U;
    UINT        selectedMode = 0U;
    RESOLUTION  selectedResolution = { 0U, 0U };
    UINT        selectedRefreshRate = 0U;
    DISPLAYMODE selectedDisplayMode = DISPLAYMODE::NONE;
    TCHAR       selectedLanguage[MAX_LANG_LENGTH] = { 0 };
    bool        debugEnabled = false;

    HWND        hWnd = NULL;
    HWND        monitorComboBox = NULL;
    HWND        resolutionComboBox = NULL;
    HWND        refreshRateComboBox = NULL;
    HWND        displayModeComboBox = NULL;
    HWND        languageComboBox = NULL;
    HWND        debugModeButton = NULL;

    HKEY        currentHKey = NULL;

    VOID        initD3D();
    bool        initMonitorDisplayModes();

    HWND        createComboBoxWithLabel(HWND hWnd, UINT textUID, UINT menuId, UINT x, UINT y, UINT w);
    bool        prefillSettings();

    bool        setCurrentHKEY(HKEY hKey);
    bool        resetHKEY();
    bool        openRegKey(LPCTSTR lpSubKey, bool write = false);
    bool        getRegKeyValue(LPCTSTR lpValue, LPTSTR lpData, LPDWORD lpcbData);
    bool        setRegKeyValue(LPCTSTR lpValue, LPTSTR lpData);

    bool        jsonLoadConfig();
    bool        jsonConfigExists();

    VOID        destroyMonitorDisplayModes();
    VOID        destroyD3D();
public:
    OniLauncher();
    VOID        init(HWND hWnd);

    UINT        getMonitorDisplayMode(UINT width, UINT height);
    bool        startGameWithPatch();

    VOID        resetResolution();
    VOID        resetRefreshRate();

    bool        fillMonitorComboBox();
    bool        fillResolutionComboBox();
    bool        fillRefreshRateComboBox();
    bool        fillDisplayModeComboBox();
    bool        fillLanguageComboBox();

    bool        fetchSelectedMonitor();
    bool        fetchSelectedResolution();
    bool        fetchSelectedRefreshRate();
    bool        fetchSelectedDisplayMode();
    bool        fetchSelectedDebugMode();
    bool        fetchSelectedLanguage();

    bool        checkGamePaths();
    bool        checkSettings();
    bool        save();

    ~OniLauncher();
};
