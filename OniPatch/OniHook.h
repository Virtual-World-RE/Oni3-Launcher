#pragma once

#include "pch.h"
#include "Utils.h"
#include "Trace.h"
#include "MyIDirect3D9.h"

using json = nlohmann::json;

// Enums
enum class DISPLAYMODE : UINT
{
	NONE,
	BORDERLESS,
	FULLSCREEN,
	WINDOWED,
};

// Type
typedef struct
{
	UINT width;
	UINT height;
} RESOLUTION;

typedef HWND(WINAPI *CreateWindowExWType) (
	_In_ DWORD dwExStyle,
	_In_opt_ LPCWSTR lpClassName,
	_In_opt_ LPCWSTR lpWindowName,
	_In_ DWORD dwStyle,
	_In_ int X,
	_In_ int Y,
	_In_ int nWidth,
	_In_ int nHeight,
	_In_opt_ HWND hWndParent,
	_In_opt_ HMENU hMenu,
	_In_opt_ HINSTANCE hInstance,
	_In_opt_ LPVOID lpParam);

typedef IDirect3D9 *(WINAPI *Direct3DCreate9Type) (UINT SDKVersion);

class OniHook
{
private:
	static OniHook *instance;

	bool		configLoaded = false;
	UINT        selectedMonitor = 0U;
	RESOLUTION  selectedResolution = { 0U, 0U };
	UINT        selectedRefreshRate = 0U;
	DISPLAYMODE selectedDisplayMode = DISPLAYMODE::NONE;
	bool        debugEnabled = false;
public:
	static CreateWindowExWType TrueCreateWindowExW;
	static Direct3DCreate9Type TrueDirect3DCreate9;

	bool jsonLoadConfig();
	// Hooks
	static HWND WINAPI MyCreateWindowExW(_In_ DWORD dwExStyle, _In_opt_ LPCWSTR lpClassName, _In_opt_ LPCWSTR lpWindowName, _In_ DWORD dwStyle, _In_ int X, _In_ int Y, _In_ int nWidth, _In_ int nHeight, _In_opt_ HWND hWndParent, _In_opt_ HMENU hMenu, _In_opt_ HINSTANCE hInstance, _In_opt_ LPVOID lpParam);
	static IDirect3D9 *WINAPI MyDirect3DCreate9(UINT SDKVersion);
	void LoadEntryPoints();
	static OniHook *GetInstance();
	void Hook();
	void UnHook();
};
