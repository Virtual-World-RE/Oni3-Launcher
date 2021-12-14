#include "pch.h"
#include "OniHook.h"

CreateWindowExWType OniHook::TrueCreateWindowExW = nullptr;
Direct3DCreate9Type OniHook::TrueDirect3DCreate9 = nullptr;
OniHook *OniHook::instance = nullptr;

HWND WINAPI OniHook::MyCreateWindowExW(
    _In_     DWORD     dwExStyle,
    _In_opt_ LPCWSTR   lpClassName,
    _In_opt_ LPCWSTR   lpWindowName,
    _In_     DWORD     dwStyle,
    _In_     int       X,
    _In_     int       Y,
    _In_     int       nWidth,
    _In_     int       nHeight,
    _In_opt_ HWND      hWndParent,
    _In_opt_ HMENU     hMenu,
    _In_opt_ HINSTANCE hInstance,
    _In_opt_ LPVOID    lpParam
)
{
    TraceFunc();

    if (lpWindowName != NULL) {
        TraceW("Window class", lpClassName);
        TraceW("Window name", lpWindowName);
        TraceStyle(dwStyle);
        TraceParam("X", X);
        TraceParam("Y", Y);
        TraceParam("Width", nWidth);
        TraceParam("Height", nHeight);

        if (!wcscmp(lpWindowName, L"oni3")) {
            RECT rect;
            TraceMsg("Onimusha 3 window found");

            // BORDERLESS_STYLE: WS_POPUP | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX

            X = CW_USEDEFAULT;
            Y = CW_USEDEFAULT;

            SecureZeroMemory(&rect, sizeof(RECT));
            rect.bottom = 1080;
            rect.right = 1920;
            AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

            // TODO: Read from config
            nWidth = rect.right - rect.left;
            nHeight = rect.bottom - rect.top;
            dwExStyle = WS_EX_APPWINDOW;
            dwStyle = WS_OVERLAPPEDWINDOW;

            TraceStyle(dwStyle);
            TraceParam("New X", X);
            TraceParam("New Y", Y);
            TraceParam("New Width", nWidth);
            TraceParam("New Height", nHeight);
        }

        //exit(302);
    }

    HWND result = TrueCreateWindowExW(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);

    if (result == NULL) {
        DWORD errorCode = GetLastError();
        std::cerr << "CreateWindowExW failed with error code " << errorCode << std::endl;
    }

    return result;
}

IDirect3D9 *WINAPI OniHook::MyDirect3DCreate9(UINT SDKVersion)
{
    TraceFunc();
    IDirect3D9 *d3d9 = TrueDirect3DCreate9(SDKVersion);

    return new MyIDirect3D9(d3d9);
}

void OniHook::LoadEntryPoints()
{
    TraceFunc();

    TrueCreateWindowExW = reinterpret_cast<CreateWindowExWType>(Utils::LoadProcAddress("user32", "CreateWindowExW"));
    TrueDirect3DCreate9 = reinterpret_cast<Direct3DCreate9Type>(Utils::LoadProcAddress("d3d9", "Direct3DCreate9"));
}

OniHook *OniHook::GetInstance()
{
    if (OniHook::instance == nullptr) {
        OniHook::instance = new OniHook();
    }

    return OniHook::instance;
}

void OniHook::Hook()
{
    TraceFunc();

    LoadEntryPoints();

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    DetourAttach((PVOID *)&TrueDirect3DCreate9, MyDirect3DCreate9);
    DetourAttach((PVOID *)&TrueCreateWindowExW, MyCreateWindowExW);

    if (DetourTransactionCommit() != NO_ERROR) {
        std::cerr << "Detour failed";
    }
}

void OniHook::UnHook()
{
}
