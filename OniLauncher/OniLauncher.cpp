/*
 ** This file is part of the Onimusha 3:RE Project !
 **
 ** You should have received a copy of the LICENSE along with this program.
 **
 ** Author: Gabriel GRONDIN (GGLinnk) <gglinnk@protonmail.com>
 ** Contributors (See CONTRIBUTION.md):
 */

#include "OniLauncher.hpp"

 // Global variables:
HINSTANCE hInst;                                // Global instance.

OniLauncher oniLauncher;

int APIENTRY WINMAIN(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PTSTR pCmdLine, _In_ int nCmdShow)
{
    MSG msg;
    HACCEL hAccelTable;
    TCHAR szTitle[MAX_LOADSTRING];
    TCHAR szWindowClass[MAX_LOADSTRING];

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(pCmdLine);

    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDS_ONILAUNCHER, szWindowClass, MAX_LOADSTRING);

    if (!WindowRegisterClass(hInstance, WndProc, MAKEINTRESOURCE(IDS_ONILAUNCHER), szWindowClass))
        return false;

    if (!InitInstance(hInstance, nCmdShow, szTitle, szWindowClass))
        return false;

    hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDS_ONILAUNCHER));

    while (GetMessage(&msg, NULL, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    DestroyAcceleratorTable(hAccelTable);
    UnregisterClass(szWindowClass, hInstance);

    return (int)msg.wParam;
}

bool WindowRegisterClass(HINSTANCE hInstance, WNDPROC lpfnWndProc, LPCTSTR lpszMenuName, LPCTSTR szWindowClass)
{
    WNDCLASSEX wcex = { 0 };

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = 0;
    wcex.lpfnWndProc = lpfnWndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ONILAUNCHER));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = lpszMenuName;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    if (!RegisterClassEx(&wcex)) {
        showWinApiErrorMB(NULL, TEXT("Class Registration Error"));
        return false;
    }

    return true;
}

bool InitInstance(HINSTANCE hInstance, int nCmdShow, LPCTSTR szTitle, LPCTSTR szWindowClass)
{
    RECT rect = { 0 };
    HWND hWnd;

    rect.bottom = MAIN_WINDOW_HEIGHT;
    rect.right = MAIN_WINDOW_WIDTH;

    ::hInst = hInstance;

    if (!AdjustWindowRect(&rect, WINDOWS_STYLE, TRUE)) {
        showWinApiErrorMB(NULL, TEXT("Unable to ajust window size"));
        return false;
    }

    hWnd = CreateWindow(szWindowClass, szTitle, WINDOWS_STYLE, CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, NULL);

    if (!hWnd) {
        showWinApiErrorMB(NULL, TEXT("CreateWindow Error"));
        return false;
    }

    ShowWindow(hWnd, nCmdShow);

    return true;
}

BOOL CALLBACK SetFont(HWND child, LPARAM font)
{
    SendMessage(child, WM_SETFONT, font, TRUE);
    return TRUE;
}

LRESULT wndProcCreate(HWND hWnd)
{
    oniLauncher.init(hWnd);

    EnumChildWindows(hWnd, (WNDENUMPROC)SetFont, (LPARAM)GetStockObject(DEFAULT_GUI_FONT));

    return 0;
}

LRESULT wndProcCommandScreenSettings(HWND hWnd, UINT wmId, UINT iCode)
{
    if (iCode != CBN_SELENDOK)
        return 0;

    switch (wmId) {
    case ID_MONITOR:
    {
        oniLauncher.resetResolution();
        oniLauncher.resetRefreshRate();

        oniLauncher.fetchSelectedMonitor();
        oniLauncher.fillResolutionComboBox();
        return 0;
    }
    case ID_RESOLUTION:
    {
        oniLauncher.resetRefreshRate();

        oniLauncher.fetchSelectedResolution();
        oniLauncher.fillRefreshRateComboBox();
        return 0;
    }
    case ID_REFRESH_RATE:
    {
        oniLauncher.fetchSelectedRefreshRate();
        return 0;
    }
    case ID_DISPLAY_MODE:
    {
        oniLauncher.fetchSelectedDisplayMode();
        return 0;
    }
    case ID_GAME_LANG:
    {
        oniLauncher.fetchSelectedLanguage();
        return 0;
    }
    }

    return 0;
}

LRESULT wndProcCommandButtonAction(HWND hWnd, UINT wmId, UINT iCode)
{
    if (iCode != BN_CLICKED)
        return 0;

    switch (wmId) {
    case ID_DEBUG:
    {
        oniLauncher.fetchSelectedDebugMode();
        return 0;
    }
    case ID_PLAY:
    {
        if (!oniLauncher.checkSettings() || !oniLauncher.checkGame())
            return 0;
        if (!oniLauncher.save()) {
            showWinApiErrorMB(hWnd, TEXT("Error saving the configuration file.\nTry again !"));
            return 0;
        }
        //oriLauncher.pseudoWinMain(hWnd);
        PostQuitMessage(0);
        return 0;
    }
    case ID_SAVE:
    {
        if (!oniLauncher.checkSettings())
            return 0;
        if (!oniLauncher.save())
            showWinApiErrorMB(hWnd, TEXT("Error saving the configuration file...\nTry again !"));
        return 0;
    }
    }

    return 0;
}

LRESULT wndProcCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    UINT wmId = LOWORD(wParam);
    UINT iCode = HIWORD(wParam);

    // Analyse les sélections de menu :
    switch (wmId) {
    case IDM_ABOUT:
        DialogBox(::hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
        return 0;
    case IDM_EXIT:
        return !DestroyWindow(hWnd);
    case ID_DISPLAY_MODE:
    case ID_REFRESH_RATE:
    case ID_RESOLUTION:
    case ID_MONITOR:
    case ID_GAME_LANG:
        return wndProcCommandScreenSettings(hWnd, wmId, iCode);
    case ID_DEBUG:
    case ID_SAVE:
    case ID_PLAY:
        return wndProcCommandButtonAction(hWnd, wmId, iCode);
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT wndProcCtlColorStatic(WPARAM wParam)
{
    HDC hdcStatic = (HDC)wParam;
    HBRUSH BGColorBrush = CreateSolidBrush(RGB(255, 255, 255));

    SetTextColor(hdcStatic, RGB(0, 0, 0));
    SetBkMode(hdcStatic, TRANSPARENT);

    return (LRESULT)BGColorBrush;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case WM_CREATE:
        return wndProcCreate(hWnd);
    case WM_CTLCOLORSTATIC:
        return wndProcCtlColorStatic(wParam);
    case WM_COMMAND:
        return wndProcCommand(hWnd, message, wParam, lParam);
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR aboutProcCommand(HWND hDlg, UINT message, WPARAM wParam)
{
    UINT wmId = LOWORD(wParam);
    UINT iCode = HIWORD(wParam);

    if (wmId == IDOK || wmId == IDCANCEL) {
        EndDialog(hDlg, wmId);
        return (INT_PTR)TRUE;
    }
    return (INT_PTR)TRUE;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);

    switch (message) {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;
    case WM_COMMAND:
        return aboutProcCommand(hDlg, message, wParam);
    }
    return (INT_PTR)FALSE;
}

int d3dDisplayModeCmp(const void *firstD3dDisplayMode, const void *secondD3dDisplayMode)
{
    D3DDISPLAYMODE firstD3dDisplayModeT = *(const D3DDISPLAYMODE *)firstD3dDisplayMode;
    D3DDISPLAYMODE secondD3dDisplayModeT = *(const D3DDISPLAYMODE *)secondD3dDisplayMode;

    INT64 wDiff = (INT64)firstD3dDisplayModeT.Width - (INT64)secondD3dDisplayModeT.Width;

    if (wDiff == 0) {
        INT64 hDiff = (INT64)firstD3dDisplayModeT.Height - (INT64)secondD3dDisplayModeT.Height;

        if (hDiff == 0) {
            INT64 rDiff = (INT64)firstD3dDisplayModeT.RefreshRate - (INT64)secondD3dDisplayModeT.RefreshRate;

            if (rDiff > 0)
                return 4;
            if (rDiff < 0)
                return -4;
            return 0;
        }

        if (hDiff > 0)
            return 2;
        if (hDiff < 0)
            return -2;
    }

    if (wDiff > 0)
        return 1;
    if (wDiff < 0)
        return -1;

    return 0;
}

// https://stackoverflow.com/a/8196291/12876357
BOOL isElevated()
{
    BOOL fRet = FALSE;
    HANDLE hToken = NULL;
    TOKEN_ELEVATION Elevation = { 0 };
    DWORD cbSize = sizeof(TOKEN_ELEVATION);

    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
        if (GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof(Elevation), &cbSize))
            fRet = Elevation.TokenIsElevated;

    if (hToken)
        CloseHandle(hToken);

    return fRet;
}

OniLauncher::OniLauncher()
{
    this->initD3D();
    this->initMonitorDisplayModes();
    if (this->jsonConfigExists())
        configLoaded = this->jsonLoadConfig();
}

VOID OniLauncher::initD3D()
{
    d3d = Direct3DCreate9(D3D_SDK_VERSION);

    if (d3d == NULL)
        throw TEXT("D3D Failed to initialize...");
}

bool OniLauncher::initMonitorDisplayModes()
{
    UINT adapters = d3d->GetAdapterCount();

    if (adapters <= 0 || adapters == UINT_MAX)
        return false;

    d3dDisplayModes = (D3DDISPLAYMODE **)calloc(adapters + 1U, sizeof(D3DDISPLAYMODE *));

    if (d3dDisplayModes == NULL)
        return false;

    // TODO: Free monitorDisplayModes and all previous monitorDisplayModes[i] of the last monitorDisplayModes[i] if it failed te be malloc'ed.
    for (UINT i = 0; i < adapters; i++) {
        UINT modes = d3d->GetAdapterModeCount(i, D3DFMT_MODE);

        d3dDisplayModes[i] = (D3DDISPLAYMODE *)calloc(modes, sizeof(D3DDISPLAYMODE));

        if (d3dDisplayModes[i] == NULL)
            return false;

        for (UINT j = 0; j < modes; j++) {
            SecureZeroMemory(&d3dDisplayModes[i][j], sizeof(D3DDISPLAYMODE));
            d3d->EnumAdapterModes(i, D3DFMT_MODE, j, &d3dDisplayModes[i][j]);
        }

        qsort(d3dDisplayModes[i], modes, sizeof(D3DDISPLAYMODE), d3dDisplayModeCmp);
    }

    return true;
}

bool OniLauncher::startGameWithPatch()
{
    HANDLE oni3GameExeFileHandler = NULL;
    bool oni3ProcessCreated = false;
    STARTUPINFO oni3GameSI = { 0 };
    PROCESS_INFORMATION oni3GamePI = { 0 };

    SecureZeroMemory(&oni3GameSI, sizeof(STARTUPINFO));
    SecureZeroMemory(&oni3GamePI, sizeof(PROCESS_INFORMATION));

    oni3GameSI.cb = sizeof(STARTUPINFO);

    oni3GameExeFileHandler = CreateFile(oni3GameExePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);

    if (oni3GameExeFileHandler == INVALID_HANDLE_VALUE) {
        showWinApiErrorMB(hWnd, TEXT("Error opening game executable"));
        return false;
    }

    oni3ProcessCreated = CreateProcess(oni3GameExePath, NULL, NULL, NULL, FALSE, DETACHED_PROCESS, NULL, oni3GamePath, &oni3GameSI, &oni3GamePI);
    WaitForSingleObject(oni3GamePI.hProcess, INFINITE);

    if (oni3GamePI.hProcess != NULL) {
        if (!CloseHandle(oni3GamePI.hProcess))
            showWinApiErrorMB(NULL, TEXT("CloseHandle ProcessInfo hProcess Error"));
        else
            oni3GamePI.hProcess = NULL;
    }
    if (oni3GamePI.hThread != NULL) {
        if (!CloseHandle(oni3GamePI.hThread))
            showWinApiErrorMB(NULL, TEXT("CloseHandle ProcessInfo hThread Error"));
        else
            oni3GamePI.hThread = NULL;
    }
    if (oni3GameExeFileHandler != NULL) {
        if (!CloseHandle(oni3GameExeFileHandler))
            showWinApiErrorMB(NULL, TEXT("CloseHandle Game Exe File Error"));
        else
            oni3GameExeFileHandler = NULL;
    }

    return true;
}

bool OniLauncher::jsonLoadConfig()
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
        MessageBoxA(hWnd, buffer, "Invalid JSON file", MB_OK | MB_ICONERROR);
    }

    selectedMonitor = settings.value<UINT>("monitor", -1);
    selectedResolution.height = settings["resolution"].value<UINT>("height", 0);
    selectedResolution.width = settings["resolution"].value<UINT>("width", 0);
    selectedMode = getMonitorDisplayMode(selectedResolution.width, selectedResolution.height);
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

bool OniLauncher::jsonConfigExists()
{
    TCHAR szPath[MAX_PATH];

    if (!SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, szPath)))
        return false;

    PathCchAppend(szPath, MAX_PATH, TEXT("OniLauncher"));
    PathCchAppend(szPath, MAX_PATH, TEXT("config.json"));
    if (PathFileExists(szPath))
        if (!PathIsDirectory(szPath)) {
            #ifdef _DEBUG
            OutputDebugString(TEXT("Found JSON config file: "));
            OutputDebugString(szPath);
            OutputDebugString(TEXT("\n"));
            #endif
            return true;
        }

    return false;
}

UINT OniLauncher::getMonitorDisplayMode(UINT width, UINT height)
{
    for (int i = 0; &d3dDisplayModes[selectedMonitor][i]; i++)
        if (d3dDisplayModes[selectedMonitor][i].Width == width && d3dDisplayModes[selectedMonitor][i].Height == height)
            return i;
    return 0;
}

VOID OniLauncher::init(HWND hWnd)
{
    DWORD oni3GamePathLength = MAX_PATH;
    TCHAR loadedString[2][MAX_LOADSTRING] = { 0 };

    debugModeButton = NULL;
    monitorComboBox = createComboBoxWithLabel(hWnd, IDS_MONITOR_DESC, ID_MONITOR, 5, 110, 240);
    resolutionComboBox = createComboBoxWithLabel(hWnd, IDS_RESOLUTION_DESC, ID_RESOLUTION, 5, 154, 130);
    refreshRateComboBox = createComboBoxWithLabel(hWnd, IDS_REFRESH_DESC, ID_REFRESH_RATE, 140, 154, 105);
    displayModeComboBox = createComboBoxWithLabel(hWnd, IDS_DISPLAY_MODE_DESC, ID_DISPLAY_MODE, 5, 198, 240);
    languageComboBox = createComboBoxWithLabel(hWnd, IDS_GAME_LANG_DESC, ID_GAME_LANG, 5, 242, 240);

    this->hWnd = hWnd;

    LoadString(::hInst, IDS_DEBUG, loadedString[0], MAX_LOADSTRING);
    debugModeButton = CreateWindow(WC_BUTTON, loadedString[0], DEFAULT_STYLE | BS_AUTOCHECKBOX, 5, 300, 240, 20, hWnd, (HMENU)ID_DEBUG, hInst, NULL);
    LoadString(::hInst, IDS_SAVE, loadedString[0], MAX_LOADSTRING);
    CreateWindow(WC_BUTTON, loadedString[0], DEFAULT_STYLE, 5, 320, 240, 30, hWnd, (HMENU)ID_SAVE, hInst, NULL);
    LoadString(::hInst, IDS_PLAY, loadedString[0], MAX_LOADSTRING);
    CreateWindow(WC_BUTTON, loadedString[0], DEFAULT_STYLE, 5, 355, 240, 40, hWnd, (HMENU)ID_PLAY, hInst, NULL);

    fillLanguageComboBox();
    fillDisplayModeComboBox();
    fillMonitorComboBox();

    ComboBox_SetCurSel(monitorComboBox, 0);

    fillResolutionComboBox();
    prefillSettings();

    getRegKeyValue(TEXT("InstallPath"), oni3GamePath, &oni3GamePathLength);

    StringCchCopy(oni3GameExePath, MAX_PATH, oni3GamePath);
    PathCchAppend(oni3GameExePath, MAX_PATH, TEXT(R"(\oni3.exe)"));

    OutputDebugString(oni3GamePath);
    OutputDebugString(TEXT("\n"));
    OutputDebugString(oni3GameExePath);
    OutputDebugString(TEXT("\n\n"));
}

HWND OniLauncher::createComboBoxWithLabel(HWND hWnd, UINT textUID, UINT menuId, UINT x, UINT y, UINT w)
{
    TCHAR lStr[MAX_LOADSTRING];
    HWND hCtl = NULL;
    HWND hTxt = NULL;

    LoadString(NULL, textUID, lStr, MAX_LOADSTRING);
    hTxt = CreateWindow(WC_STATIC, lStr, DEFAULT_STYLE, x, y, w, 30, hWnd, NULL, NULL, NULL);

    if (hTxt == NULL)
        return NULL;

    hCtl = CreateWindow(WC_COMBOBOX, TEXT(""), DROPDOWN_COMBO_BOX | WS_DISABLED, x, y + 16, w, 0, hWnd, (HMENU)menuId, hInst, NULL);

    if (hCtl == NULL)
        return NULL;

    return hCtl;
}

bool OniLauncher::prefillSettings()
{
    DWORD vlength = MAX_LANG_LENGTH;
    TCHAR reg[MAX_PATH] = { 0 };

    if (getRegKeyValue(TEXT("Language"), reg, &vlength) == true) {
        if (ComboBox_SelectString(languageComboBox, 0, reg) == CB_ERR)
            return false;
    } else {
        if (ComboBox_SelectString(languageComboBox, 0, TEXT("Japanese")) == CB_ERR)
            return false;
    }

    if (!configLoaded)
        return false;

    if (ComboBox_SetCurSel(monitorComboBox, ComboBox_SelectItemData(monitorComboBox, 0, (LPARAM)selectedMonitor)) == CB_ERR)
        return false;
    fillResolutionComboBox();
    if (ComboBox_SetCurSel(resolutionComboBox, ComboBox_SelectItemData(resolutionComboBox, 0, (LPARAM)selectedMode)) == CB_ERR)
        return false;
    fillRefreshRateComboBox();
    if (ComboBox_SetCurSel(refreshRateComboBox, ComboBox_SelectItemData(refreshRateComboBox, 0, (LPARAM)selectedRefreshRate)) == CB_ERR)
        return false;
    if (ComboBox_SetCurSel(displayModeComboBox, ComboBox_SelectItemData(displayModeComboBox, 0, (LPARAM)selectedDisplayMode)) == CB_ERR)
        return false;
    if (debugEnabled)
        Button_SetCheck(debugModeButton, BST_CHECKED);

    return true;
}

VOID OniLauncher::resetResolution()
{
    ComboBox_Enable(resolutionComboBox, FALSE);
    ComboBox_ResetContent(resolutionComboBox);
    selectedResolution.width = 0;
    selectedResolution.height = 0;
}

VOID OniLauncher::resetRefreshRate()
{
    ComboBox_Enable(refreshRateComboBox, FALSE);
    ComboBox_ResetContent(refreshRateComboBox);
    selectedRefreshRate = 0;
}

bool OniLauncher::fillMonitorComboBox()
{
    INT comboBoxIndex;
    TCHAR buffer[MAX_LOADSTRING + 1 + UINT_DIGITS];
    TCHAR monitorStr[MAX_LOADSTRING];
    UINT monitors = d3d->GetAdapterCount();

    ComboBox_Enable(monitorComboBox, FALSE);
    ComboBox_ResetContent(monitorComboBox);

    LoadString(::hInst, IDS_MONITOR, monitorStr, MAX_LOADSTRING);

    for (UINT i = 0; i < monitors; i++) {
        _stprintf_s(buffer, MAX_LOADSTRING + 1 + UINT_DIGITS, TEXT("%s %u"), monitorStr, i);
        comboBoxIndex = ComboBox_AddString(monitorComboBox, buffer);

        if (comboBoxIndex == CB_ERR || ComboBox_SetItemData(monitorComboBox, comboBoxIndex, (LPARAM)i) == CB_ERR)
            return false;
    }

    ComboBox_Enable(monitorComboBox, TRUE);
    return true;
}

bool OniLauncher::fillResolutionComboBox()
{
    INT comboBoxIndex;
    TCHAR buffer[UINT_DIGITS * 2 + 4];
    UINT modes = d3d->GetAdapterModeCount(selectedMonitor, D3DFMT_MODE);

    ComboBox_Enable(resolutionComboBox, FALSE);
    ComboBox_ResetContent(resolutionComboBox);

    for (UINT i = 0; i < modes; i++) {
        if (i != 0 && (d3dDisplayModes[selectedMonitor][i - 1].Width == d3dDisplayModes[selectedMonitor][i].Width && d3dDisplayModes[selectedMonitor][i - 1].Height == d3dDisplayModes[selectedMonitor][i].Height))
            continue;

        _stprintf_s(buffer, UINT_DIGITS * 2 + 4, TEXT("%u x %u"), d3dDisplayModes[selectedMonitor][i].Width, d3dDisplayModes[selectedMonitor][i].Height);
        comboBoxIndex = ComboBox_AddString(resolutionComboBox, buffer);

        if (comboBoxIndex == CB_ERR || ComboBox_SetItemData(resolutionComboBox, comboBoxIndex, (LPARAM)i) == CB_ERR)
            return false;
    }

    ComboBox_Enable(resolutionComboBox, TRUE);

    return true;
}

bool OniLauncher::fillRefreshRateComboBox()
{
    INT comboBoxIndex;
    TCHAR buffer[UINT_DIGITS + 4];
    UINT modes = d3d->GetAdapterModeCount(selectedMonitor, D3DFMT_MODE);

    ComboBox_Enable(refreshRateComboBox, FALSE);
    ComboBox_ResetContent(refreshRateComboBox);

    for (UINT i = selectedMode; i < modes; i++) {
        if (selectedResolution.width != d3dDisplayModes[selectedMonitor][i].Width || selectedResolution.height != d3dDisplayModes[selectedMonitor][i].Height)
            break;

        _stprintf_s(buffer, UINT_DIGITS + 4, TEXT("%u Hz"), d3dDisplayModes[selectedMonitor][i].RefreshRate);
        comboBoxIndex = ComboBox_AddString(refreshRateComboBox, buffer);

        if (comboBoxIndex == CB_ERR || ComboBox_SetItemData(refreshRateComboBox, comboBoxIndex, (LPARAM)d3dDisplayModes[selectedMonitor][i].RefreshRate) == CB_ERR)
            return false;
    }

    ComboBox_Enable(refreshRateComboBox, TRUE);
    return true;
}

bool OniLauncher::fillDisplayModeComboBox()
{
    INT comboBoxIndex;
    TCHAR loadedString[MAX_LOADSTRING];

    LoadString(::hInst, IDS_DISPLAY_MODE_BORD, loadedString, MAX_LOADSTRING);
    comboBoxIndex = ComboBox_AddString(displayModeComboBox, loadedString);
    if (comboBoxIndex == CB_ERR || ComboBox_SetItemData(displayModeComboBox, comboBoxIndex, (LPARAM)DISPLAYMODE::BORDERLESS) == CB_ERR)
        return false;
    LoadString(::hInst, IDS_DISPLAY_MODE_FULL, loadedString, MAX_LOADSTRING);
    comboBoxIndex = ComboBox_AddString(displayModeComboBox, loadedString);
    if (comboBoxIndex == CB_ERR || ComboBox_SetItemData(displayModeComboBox, comboBoxIndex, (LPARAM)DISPLAYMODE::FULLSCREEN) == CB_ERR)
        return false;
    LoadString(::hInst, IDS_DISPLAY_MODE_WIND, loadedString, MAX_LOADSTRING);
    comboBoxIndex = ComboBox_AddString(displayModeComboBox, loadedString);
    if (comboBoxIndex == CB_ERR || ComboBox_SetItemData(displayModeComboBox, comboBoxIndex, (LPARAM)DISPLAYMODE::WINDOWED) == CB_ERR)
        return false;

    ComboBox_Enable(displayModeComboBox, TRUE);

    return true;
}

bool OniLauncher::fillLanguageComboBox()
{
    ComboBox_AddString(languageComboBox, TEXT("Traditional Chinese"));
    ComboBox_AddString(languageComboBox, TEXT("Simplified Chinese"));
    ComboBox_AddString(languageComboBox, TEXT("Korean"));
    ComboBox_AddString(languageComboBox, TEXT("Japanese"));
    ComboBox_AddString(languageComboBox, TEXT("English"));
    ComboBox_AddString(languageComboBox, TEXT("French"));
    ComboBox_AddString(languageComboBox, TEXT("Spanish"));
    ComboBox_AddString(languageComboBox, TEXT("German"));
    ComboBox_AddString(languageComboBox, TEXT("Italian"));

    if (isElevated())
        ComboBox_Enable(languageComboBox, TRUE);

    return true;
}

bool OniLauncher::fetchSelectedMonitor()
{
    LRESULT getItemDataStatus = ComboBox_GetItemData(monitorComboBox, ComboBox_GetCurSel(monitorComboBox));

    if (getItemDataStatus == CB_ERR)
        return false;

    selectedMonitor = (UINT)getItemDataStatus;

    #ifdef _DEBUG
    TCHAR debugBuffer[256];
    _stprintf_s(debugBuffer, TEXT("Selected monitor is %u\n"), selectedMonitor);
    OutputDebugString(debugBuffer);
    #endif

    return true;
}

bool OniLauncher::fetchSelectedResolution()
{
    UINT dmIndex;
    LRESULT getItemDataStatus = ComboBox_GetItemData(resolutionComboBox, ComboBox_GetCurSel(resolutionComboBox));

    if (getItemDataStatus == CB_ERR)
        return false;

    dmIndex = (UINT)getItemDataStatus;

    selectedResolution.width = d3dDisplayModes[selectedMonitor][dmIndex].Width;
    selectedResolution.height = d3dDisplayModes[selectedMonitor][dmIndex].Height;
    selectedMode = dmIndex;

    #ifdef _DEBUG
    TCHAR debugBuffer[256];
    _stprintf_s(debugBuffer, TEXT("Selected resolution is %d x %d\n"), selectedResolution.width, selectedResolution.height);
    OutputDebugString(debugBuffer);
    #endif

    return true;
}

bool OniLauncher::fetchSelectedRefreshRate()
{
    LRESULT getItemDataStatus = ComboBox_GetItemData(refreshRateComboBox, ComboBox_GetCurSel(refreshRateComboBox));

    if (getItemDataStatus == CB_ERR)
        return false;

    selectedRefreshRate = (UINT)getItemDataStatus;

    #ifdef _DEBUG
    TCHAR debugBuffer[256];
    _stprintf_s(debugBuffer, TEXT("Selected refresh rate is %d\n"), selectedRefreshRate);
    OutputDebugString(debugBuffer);
    #endif

    return true;
}

bool OniLauncher::fetchSelectedDisplayMode()
{
    LRESULT getItemDataStatus = ComboBox_GetItemData(displayModeComboBox, ComboBox_GetCurSel(displayModeComboBox));

    if (getItemDataStatus == CB_ERR)
        return false;

    selectedDisplayMode = (DISPLAYMODE)getItemDataStatus;

    #ifdef _DEBUG
    TCHAR debugBuffer[256];
    _stprintf_s(debugBuffer, TEXT("Selected display mode is %s\n"), selectedDisplayMode == DISPLAYMODE::BORDERLESS ? TEXT("Borderless") : selectedDisplayMode == DISPLAYMODE::FULLSCREEN ? TEXT("Fullscreen") : selectedDisplayMode == DISPLAYMODE::WINDOWED ? TEXT("Windowed") : TEXT("Unknown"));
    OutputDebugString(debugBuffer);
    #endif

    return true;
}

bool OniLauncher::fetchSelectedDebugMode()
{
    debugEnabled = Button_GetCheck(debugModeButton) ? true : false;

    #ifdef _DEBUG
    TCHAR debugBuffer[256];
    _stprintf_s(debugBuffer, TEXT("Debug mode %s\n"), debugEnabled ? TEXT("enabled") : TEXT("disabled"));
    OutputDebugString(debugBuffer);
    #endif

    return true;
}

bool OniLauncher::fetchSelectedLanguage()
{
    int getTextStatus = ComboBox_GetText(languageComboBox, selectedLanguage, MAX_LANG_LENGTH);

    if (getTextStatus == 0)
        return false;

    #ifdef _DEBUG
    TCHAR debugBuffer[256];
    _stprintf_s(debugBuffer, TEXT("Selected language %s\n"), selectedLanguage);
    OutputDebugString(debugBuffer);
    #endif

    return true;
}

bool OniLauncher::setCurrentHKEY(HKEY hKey)
{
    if (currentHKey)
        resetHKEY();

    currentHKey = hKey;

    return((bool)hKey);
}

bool OniLauncher::resetHKEY()
{
    if (currentHKey && currentHKey != HKEY_CLASSES_ROOT && currentHKey != HKEY_CURRENT_CONFIG && currentHKey != HKEY_CURRENT_USER && currentHKey != HKEY_LOCAL_MACHINE && currentHKey != HKEY_USERS)
        RegCloseKey(currentHKey);

    currentHKey = 0;
    return false;
}

bool OniLauncher::openRegKey(LPCTSTR lpSubKey, bool write)
{
    HKEY phkResult = 0;

    LSTATUS regOpenKeyStatus = RegOpenKeyEx(currentHKey, lpSubKey, 0, KEY_WOW64_32KEY | (write ? KEY_WRITE : KEY_READ), &phkResult);
    SetLastError(regOpenKeyStatus);
    if (regOpenKeyStatus == ERROR_ACCESS_DENIED)
        showWinApiErrorMB();
    if (regOpenKeyStatus != ERROR_SUCCESS)
        return false;

    resetHKEY();
    currentHKey = phkResult;

    return true;
}

bool OniLauncher::getRegKeyValue(LPCTSTR lpValue, LPTSTR lpData, LPDWORD lpcbData)
{
    bool result = false;

    setCurrentHKEY(HKEY_LOCAL_MACHINE);

    if (!openRegKey(TEXT(R"(SOFTWARE\CAPCOM\ONIMUSHA3 PC\1.00.000)")))
        return false;
    if (currentHKey && lpValue && lpData)
        result = (RegGetValue(currentHKey, NULL, lpValue, RRF_RT_REG_SZ, NULL, lpData, lpcbData) == ERROR_SUCCESS);

    setCurrentHKEY(0);

    return result;
}

bool OniLauncher::setRegKeyValue(LPCTSTR lpValue, LPTSTR lpData)
{
    bool result = false;

    setCurrentHKEY(HKEY_LOCAL_MACHINE);

    if (!openRegKey(TEXT(R"(SOFTWARE\CAPCOM\ONIMUSHA3 PC\1.00.000)"), true))
        return false;
    if (currentHKey && lpValue && lpData)
        result = (RegSetValueEx(currentHKey, lpValue, 0L, REG_SZ, (BYTE *)lpData, sizeof(TCHAR) * (_tcslen(lpData) + 1)));

    setCurrentHKEY(0);

    return result;
}

bool OniLauncher::checkGame()
{
    TCHAR loadedString[2][MAX_LOADSTRING] = { 0 };

    if (!PathFileExists(oni3GamePath) || !PathIsDirectory(oni3GamePath)) {
        LoadString(::hInst, IDSE_GAME_NOT_FOUND_TITLE, loadedString[0], MAX_LOADSTRING);
        LoadString(::hInst, IDSE_GAME_NOT_FOUND_MSG, loadedString[1], MAX_LOADSTRING);
        MessageBox(hWnd, loadedString[1], loadedString[0], MB_OK | MB_ICONERROR);
        return false;
    }

    if (!PathFileExists(oni3GameExePath) || PathIsDirectory(oni3GameExePath)) {
        LoadString(::hInst, IDSE_GAME_NOT_FOUND_TITLE, loadedString[0], MAX_LOADSTRING);
        LoadString(::hInst, IDSE_GAME_NOT_FOUND_MSG, loadedString[1], MAX_LOADSTRING);
        MessageBox(hWnd, loadedString[1], loadedString[0], MB_OK | MB_ICONERROR);
        return false;
    }

    return true;
}

bool OniLauncher::checkSettings()
{
    TCHAR loadedString[2][MAX_LOADSTRING] = { 0 };

    if (selectedResolution.width == 0 || selectedResolution.height == 0) {
        LoadString(::hInst, IDSE_NO_RESOLUTION_SELECTED_TITLE, loadedString[0], MAX_LOADSTRING);
        LoadString(::hInst, IDSE_NO_RESOLUTION_SELECTED_MSG, loadedString[1], MAX_LOADSTRING);
        MessageBox(hWnd, loadedString[1], loadedString[0], MB_OK | MB_ICONERROR);
        return false;
    }
    if (selectedRefreshRate == 0) {
        LoadString(::hInst, IDSE_NO_REFRESH_RATE_SELECTED_TITLE, loadedString[0], MAX_LOADSTRING);
        LoadString(::hInst, IDSE_NO_REFRESH_RATE_SELECTED_MSG, loadedString[1], MAX_LOADSTRING);
        MessageBox(hWnd, loadedString[1], loadedString[0], MB_OK | MB_ICONERROR);
        return false;
    }
    if (selectedDisplayMode == DISPLAYMODE::NONE) {
        LoadString(::hInst, IDSE_NO_DISPLAY_MODE_SELECTED_TITLE, loadedString[0], MAX_LOADSTRING);
        LoadString(::hInst, IDSE_NO_DISPLAY_MODE_SELECTED_MSG, loadedString[1], MAX_LOADSTRING);
        MessageBox(hWnd, loadedString[1], loadedString[0], MB_OK | MB_ICONERROR);
        return false;
    }
    return true;
}

bool OniLauncher::save()
{
    DWORD dwBytesWritten = 0;
    CHAR settingsDump[2048];
    TCHAR szPath[MAX_PATH];
    HANDLE saveFile;
    json settings = json{
        {"monitor", selectedMonitor},
        {"resolution", {
            {"width", selectedResolution.width},
        {"height", selectedResolution.height}}},
        {"refreshRate", selectedRefreshRate},
        {"fullscreen", selectedDisplayMode == DISPLAYMODE::FULLSCREEN ? true : false},
        {"borderless", selectedDisplayMode == DISPLAYMODE::BORDERLESS ? true : false},
        {"debugEnabled", debugEnabled}
    };

    if (!SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, szPath)))
        return false;
    PathCchAppend(szPath, MAX_PATH, TEXT("OniLauncher"));
    if (CreateDirectory(szPath, NULL) == ERROR_PATH_NOT_FOUND)
        return false;
    PathCchAppend(szPath, MAX_PATH, TEXT("config.json"));
    saveFile = CreateFile(szPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (saveFile == INVALID_HANDLE_VALUE)
        return false;

    strcpy_s(settingsDump, 2047, settings.dump(4).c_str());
    WriteFile(saveFile, settingsDump, strlen(settingsDump), &dwBytesWritten, NULL);

    CloseHandle(saveFile);

    if (isElevated())
        setRegKeyValue(TEXT("Language"), selectedLanguage);

    #ifdef _DEBUG
    OutputDebugString(TEXT("INFO: Settings are saved\n\n"));
    #endif

    return true;
}

VOID OniLauncher::destroyMonitorDisplayModes()
{
    if (d3dDisplayModes == NULL)
        return;

    for (UINT i = 0; d3dDisplayModes[i]; i++) {
        free(d3dDisplayModes[i]);
        d3dDisplayModes[i] = NULL;
    }
    free(d3dDisplayModes);
    d3dDisplayModes = NULL;
}

VOID OniLauncher::destroyD3D()
{
    if (d3d != NULL)
        d3d->Release();
}

OniLauncher::~OniLauncher()
{
    destroyMonitorDisplayModes();
    destroyD3D();
}
