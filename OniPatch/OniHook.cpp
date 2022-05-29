#include "pch.h"
#include "OniHook.h"

CreateWindowExWType OniHook::TrueCreateWindowExW = nullptr;
Direct3DCreate9Type OniHook::TrueDirect3DCreate9 = nullptr;
OniHook *OniHook::instance = nullptr;

bool OniHook::jsonLoadConfig()
{
    DWORD dwByteReaded = 0;
    CHAR settingsDump[2048] = { 0 };
    TCHAR szPath[MAX_PATH];
    HANDLE saveFile;
    json settings;

    if (!SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, szPath)))
        return false;

    PathCchAppend(szPath, MAX_PATH, TEXT("OniLauncher"));
    PathCchAppend(szPath, MAX_PATH, TEXT("config.json"));

    saveFile = CreateFile(szPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (saveFile == INVALID_HANDLE_VALUE)
        return false;

    if (!ReadFile(saveFile, settingsDump, 2047, &dwByteReaded, NULL))
        return false;

    CloseHandle(saveFile);
    try {
        settings = json::parse(settingsDump);
    } catch (json::parse_error &e) {
        CHAR buffer[200];
        sprintf_s(buffer, 200, "nlohmann JSON failed to parse:\n%s", e.what());
        MessageBoxA(NULL, buffer, "Invalid JSON file", MB_OK | MB_ICONERROR);
    }

    selectedMonitor = settings.value<UINT>("monitor", -1);
    selectedResolution.height = settings["resolution"].value<UINT>("height", 0);
    selectedResolution.width = settings["resolution"].value<UINT>("width", 0);
    selectedRefreshRate = settings.value<UINT>("refreshRate", 0);
    if (settings.value<bool>("borderless", false) == true)
        selectedDisplayMode = DISPLAYMODE::BORDERLESS;
    if (settings.value<bool>("fullscreen", false) == true)
        selectedDisplayMode = DISPLAYMODE::FULLSCREEN;
    if (settings.value<bool>("borderless", false) == false && settings.value<bool>("fullscreen", false) == false)
        selectedDisplayMode = DISPLAYMODE::WINDOWED;
    debugEnabled = settings.value<bool>("debugEnabled", false);

    return true;
}

OniHook::OniHook()
{
    if (jsonLoadConfig())
        configLoaded = true;
}

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
            RECT rect = { 0 };
            TraceMsg("Onimusha 3 window found");

            // AERO__BORDERLESS: WS_POPUP | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX
            // BASIC_BORDERLESS: WS_POPUP | WS_THICKFRAME |              WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX
            // FULLSCREEN

            X = CW_USEDEFAULT;
            Y = CW_USEDEFAULT;

            if (configLoaded) {
                rect.bottom = selectedResolution.height;
                rect.right = selectedResolution.width;
                AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
            }

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
