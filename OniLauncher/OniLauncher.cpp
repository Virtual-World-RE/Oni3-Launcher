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
TCHAR szTitle[MAX_LOADSTRING];                  // Windows main title.
TCHAR szWindowClass[MAX_LOADSTRING];            // Main class name.

OniLauncher oniLauncher;

int APIENTRY WINMAIN(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PTSTR pCmdLine, _In_ int nCmdShow)
{
    MSG msg;
    HACCEL hAccelTable;

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(pCmdLine);

    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDS_ONILAUNCHER, szWindowClass, MAX_LOADSTRING);

    if (!MainWindowRegisterClass(hInstance))
        return false;

    if (!InitInstance(hInstance, nCmdShow))
        return false;

    hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDS_ONILAUNCHER));

    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    DestroyAcceleratorTable(hAccelTable);
    UnregisterClass(szWindowClass, hInstance);

    return (int) msg.wParam;
}

bool MainWindowRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = 0;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ONILAUNCHER));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCE(IDS_ONILAUNCHER);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    if (!RegisterClassEx(&wcex))
    {
        showWinApiErrorMB(NULL, TEXT("Class Registration Error"));
        return false;
    }

    return true;
}

bool InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    RECT rect;
    HWND hWnd;

    SecureZeroMemory(&rect, sizeof(RECT));
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
    TCHAR loadedString[MAX_LOADSTRING];
    HWND debugModeButton;
    HWND monitorComboBox = CreateWindow(WC_COMBOBOX, TEXT(""), DROPDOWN_COMBO_BOX | WS_DISABLED, 5, 130, 240, 0, hWnd, (HMENU)ID_MONITOR, hInst, NULL);
    HWND resolutionComboBox = CreateWindow(WC_COMBOBOX, TEXT(""), DROPDOWN_COMBO_BOX | WS_DISABLED, 5, 180, 130, 0, hWnd, (HMENU)ID_RESOLUTION, hInst, NULL);
    HWND refreshRateComboBox = CreateWindow(WC_COMBOBOX, TEXT(""), DROPDOWN_COMBO_BOX | WS_DISABLED, 140, 180, 105, 0, hWnd, (HMENU)ID_REFRESH_RATE, hInst, NULL);
    HWND displayModeComboBox = CreateWindow(WC_COMBOBOX, TEXT(""), DROPDOWN_COMBO_BOX | WS_DISABLED, 5, 230, 240, 0, hWnd, (HMENU)ID_DISPLAY_MODE, hInst, NULL);
    //HWND fpsComboBox = CreateWindow(WC_COMBOBOX, TEXT(""), DROPDOWN_COMBO_BOX, 5, 250, 240, 200, hWnd, (HMENU)ID_FRAME_PER_SECONDS, hInst, NULL);

    LoadString(::hInst, IDS_MONITOR_DESC, loadedString, MAX_LOADSTRING);
    CreateWindow(WC_STATIC, loadedString, DEFAULT_STYLE, 5, 110, 240, 16, hWnd, NULL, hInst, NULL);
    LoadString(::hInst, IDS_RESOLUTION_DESC, loadedString, MAX_LOADSTRING);
    CreateWindow(WC_STATIC, loadedString, DEFAULT_STYLE, 5, 160, 130, 16, hWnd, NULL, hInst, NULL);
    LoadString(::hInst, IDS_REFRESH_DESC, loadedString, MAX_LOADSTRING);
    CreateWindow(WC_STATIC, loadedString, DEFAULT_STYLE, 140, 160, 105, 16, hWnd, NULL, hInst, NULL);
    LoadString(::hInst, IDS_DISPLAY_MODE_DESC, loadedString, MAX_LOADSTRING);
    CreateWindow(WC_STATIC, loadedString, DEFAULT_STYLE, 5, 210, 240, 16, hWnd, NULL, hInst, NULL);

    LoadString(::hInst, IDS_DEBUG, loadedString, MAX_LOADSTRING);
    debugModeButton = CreateWindow(WC_BUTTON, loadedString, DEFAULT_STYLE | BS_AUTOCHECKBOX, 5, 295, 240, 20, hWnd, (HMENU)ID_DEBUG, hInst, NULL);
    LoadString(::hInst, IDS_SAVE, loadedString, MAX_LOADSTRING);
    CreateWindow(WC_BUTTON, loadedString, DEFAULT_STYLE, 5, 320, 240, 30, hWnd, (HMENU)ID_SAVE, hInst, NULL);
    LoadString(::hInst, IDS_PLAY, loadedString, MAX_LOADSTRING);
    CreateWindow(WC_BUTTON, loadedString, DEFAULT_STYLE, 5, 355, 240, 40, hWnd, (HMENU)ID_PLAY, hInst, NULL);

    EnumChildWindows(hWnd, (WNDENUMPROC)SetFont, (LPARAM)GetStockObject(DEFAULT_GUI_FONT));

    oniLauncher.setHandlers(hWnd, monitorComboBox, resolutionComboBox, refreshRateComboBox, displayModeComboBox, debugModeButton);
    oniLauncher.fillMonitorComboBox();
    oniLauncher.fillDisplayModeComboBox();

    if (oniLauncher.isConfigLoaded())
        oniLauncher.preFillSettingsFromConfig();

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
        if (!oniLauncher.checkSettings())
            return 0;
        if (!oniLauncher.saveConfigFile())
            showWinApiErrorMB(hWnd, TEXT("Error saving the configuration file."));
        //oriLauncher.pseudoWinMain(hWnd);
        PostQuitMessage(0);
        return 0;
    }
    case ID_SAVE:
    {
        if (!oniLauncher.checkSettings())
            return 0;
        if (!oniLauncher.saveConfigFile())
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

        d3dDisplayModes[i] = (D3DDISPLAYMODE *)calloc(modes + 1U, sizeof(D3DDISPLAYMODE));

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
    oni3GameExeFileHandler = CreateFile(oni3GameExePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);

    if (oni3GameExeFileHandler == INVALID_HANDLE_VALUE) {
        showWinApiErrorMB(hWnd, TEXT("Error opening game executable"));
        return false;
    }

    oni3ProcessCreated = CreateProcess(oni3GameExePath, NULL, NULL, NULL, FALSE, DETACHED_PROCESS, NULL, oni3GamePath, &oni3GameSI, &oni3GamePI);
    WaitForSingleObject(oni3GamePI.hProcess, INFINITE);

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

    currentMonitor = settings.value<UINT>("monitor", -1);
    currentResolution.height = settings["resolution"].value("height", 0);
    currentResolution.width = settings["resolution"].value("width", 0);
    currentRefreshRate = settings.value("refreshRate", 0);
    currentDisplayMode = DISPLAYMODE::NONE;
    if (settings.value("borderless", false) == true)
        currentDisplayMode = DISPLAYMODE::BORDERLESS;
    if (settings.value("fullscreen", false) == true)
        currentDisplayMode = DISPLAYMODE::FULLSCREEN;
    if (settings.value("borderless", false) == false && settings.value("fullscreen", false) == false)
        currentDisplayMode = DISPLAYMODE::WINDOWED;
    debugEnabled = settings.value("debugEnabled", false);

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

D3DDISPLAYMODE *OniLauncher::getMonitorDisplayMode(UINT width, UINT height)
{
    for (int i = 0; &d3dDisplayModes[currentMonitor][i]; i++) {
        if (d3dDisplayModes[currentMonitor][i].Width == width && d3dDisplayModes[currentMonitor][i].Height == height)
            return &d3dDisplayModes[currentMonitor][i];
    }

    return NULL;
}

VOID OniLauncher::setHandlers(HWND hWnd, HWND monitorComboBox, HWND resolutionComboBox, HWND refreshRateComboBox, HWND displayModeComboBox, HWND debugModeButton)
{
    this->hWnd = hWnd;
    this->monitorComboBox = monitorComboBox;
    this->resolutionComboBox = resolutionComboBox;
    this->refreshRateComboBox = refreshRateComboBox;
    this->displayModeComboBox = displayModeComboBox;
    this->debugModeButton = debugModeButton;
}

bool OniLauncher::isConfigLoaded()
{
    return configLoaded;
}

bool OniLauncher::preFillSettingsFromConfig()
{
    if (ComboBox_SetCurSel(displayModeComboBox, ComboBox_SelectItemData(displayModeComboBox, 0, (LPARAM)currentDisplayMode)) == CB_ERR)
        return false;
    if (ComboBox_SetCurSel(monitorComboBox, ComboBox_SelectItemData(monitorComboBox, 0, (LPARAM)currentMonitor)) == CB_ERR)
        return false;
    fillResolutionComboBox();
    if (ComboBox_SetCurSel(resolutionComboBox, ComboBox_SelectItemData(resolutionComboBox, 0, (LPARAM)getMonitorDisplayMode(currentResolution.width, currentResolution.height))) == CB_ERR)
        return false;
    fillRefreshRateComboBox();
    if (ComboBox_SetCurSel(refreshRateComboBox, ComboBox_SelectItemData(refreshRateComboBox, 0, (LPARAM)currentRefreshRate)) == CB_ERR)
        return false;
    if (debugEnabled)
        Button_SetCheck(debugModeButton, BST_CHECKED);
    return true;
}

VOID OniLauncher::resetResolution()
{
    ComboBox_Enable(resolutionComboBox, FALSE);
    ComboBox_ResetContent(resolutionComboBox);
    currentResolution.width = 0;
    currentResolution.height = 0;
}

VOID OniLauncher::resetRefreshRate()
{
    ComboBox_Enable(refreshRateComboBox, FALSE);
    ComboBox_ResetContent(refreshRateComboBox);
    currentRefreshRate = 0;
}

bool OniLauncher::fillMonitorComboBox()
{
    INT comboBoxIndex;
    TCHAR buffer[MAX_LOADSTRING + 1 + UINT_DIGITS];
    TCHAR monitorStr[MAX_LOADSTRING];
    LoadString(::hInst, IDS_MONITOR, monitorStr, MAX_LOADSTRING);

    for (UINT i = 0; d3dDisplayModes[i]; i++) {
        _stprintf_s(buffer, MAX_LOADSTRING + 1 + UINT_DIGITS, TEXT("%s %u"), monitorStr, i);
        comboBoxIndex = ComboBox_AddString(monitorComboBox, buffer);
        if (comboBoxIndex == CB_ERR || ComboBox_SetItemData(monitorComboBox, comboBoxIndex, (LPARAM)i) == CB_ERR)
            return false;
    }

    ComboBox_Enable(monitorComboBox, TRUE);
    return true;
}

// TODO: Improve me !
bool OniLauncher::fillResolutionComboBox()
{
    INT comboBoxIndex;
    TCHAR buffer[UINT_DIGITS * 2 + 4];
    UINT modes = d3d->GetAdapterModeCount(currentMonitor, D3DFMT_MODE);

    for (int i = 0; i < modes; i++) {
        if (i > 0 && !(d3dDisplayModes[currentMonitor][i - 1].Width == d3dDisplayModes[currentMonitor][i].Width && d3dDisplayModes[currentMonitor][i - 1].Height == d3dDisplayModes[currentMonitor][i].Height)) {
            _stprintf_s(buffer, UINT_DIGITS * 2 + 4, TEXT("%u x %u"), d3dDisplayModes[currentMonitor][i].Width, d3dDisplayModes[currentMonitor][i].Height);
            comboBoxIndex = ComboBox_AddString(resolutionComboBox, buffer);
            if (comboBoxIndex == CB_ERR || ComboBox_SetItemData(resolutionComboBox, comboBoxIndex, (LPARAM)&d3dDisplayModes[currentMonitor][i]) == CB_ERR)
                return false;
        } else if (i==0)  {
            _stprintf_s(buffer, UINT_DIGITS * 2 + 4, TEXT("%u x %u"), d3dDisplayModes[currentMonitor][i].Width, d3dDisplayModes[currentMonitor][i].Height);
            comboBoxIndex = ComboBox_AddString(resolutionComboBox, buffer);
            if (comboBoxIndex == CB_ERR || ComboBox_SetItemData(resolutionComboBox, comboBoxIndex, (LPARAM)&d3dDisplayModes[currentMonitor][i]) == CB_ERR)
                return false;
        }
    }

    ComboBox_Enable(resolutionComboBox, TRUE);
    return true;
}

bool OniLauncher::fillRefreshRateComboBox()
{
    INT comboBoxIndex;
    TCHAR buffer[UINT_DIGITS + 4];
    UINT modes = d3d->GetAdapterModeCount(currentMonitor, D3DFMT_MODE);

    for (int i = 0; i < modes; i++) {
        if (currentResolution.width == d3dDisplayModes[currentMonitor][i].Width && currentResolution.height == d3dDisplayModes[currentMonitor][i].Height) {
            _stprintf_s(buffer, UINT_DIGITS + 4, TEXT("%u Hz"), d3dDisplayModes[currentMonitor][i].RefreshRate);
            comboBoxIndex = ComboBox_AddString(refreshRateComboBox, buffer);
            if (comboBoxIndex == CB_ERR || ComboBox_SetItemData(refreshRateComboBox, comboBoxIndex, (LPARAM)d3dDisplayModes[currentMonitor][i].RefreshRate) == CB_ERR)
                return false;
        }
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

bool OniLauncher::fetchSelectedMonitor()
{
    LRESULT getItemDataStatus = ComboBox_GetItemData(monitorComboBox, ComboBox_GetCurSel(monitorComboBox));

    if (getItemDataStatus == CB_ERR)
        return false;

    currentMonitor = (UINT)getItemDataStatus;

    #ifdef _DEBUG
    TCHAR debugBuffer[256];
    _stprintf_s(debugBuffer, TEXT("Selected monitor is %u\n"), currentMonitor);
    OutputDebugString(debugBuffer);
    #endif

    return true;
}

bool OniLauncher::fetchSelectedResolution()
{
    D3DDISPLAYMODE *d3dDisplayMode;
    LRESULT getItemDataStatus = ComboBox_GetItemData(resolutionComboBox, ComboBox_GetCurSel(resolutionComboBox));

    if (getItemDataStatus == CB_ERR)
        return false;

    d3dDisplayMode = (D3DDISPLAYMODE *)getItemDataStatus;

    currentResolution.width = d3dDisplayMode->Width;
    currentResolution.height = d3dDisplayMode->Height;

    #ifdef _DEBUG
    TCHAR debugBuffer[256];
    _stprintf_s(debugBuffer, TEXT("Selected resolution is %d x %d\n"), currentResolution.width, currentResolution.height);
    OutputDebugString(debugBuffer);
    #endif

    return true;
}

bool OniLauncher::fetchSelectedRefreshRate()
{
    LRESULT getItemDataStatus = ComboBox_GetItemData(refreshRateComboBox, ComboBox_GetCurSel(refreshRateComboBox));

    if (getItemDataStatus == CB_ERR)
        return false;

    currentRefreshRate = (UINT)getItemDataStatus;

    #ifdef _DEBUG
    TCHAR debugBuffer[256];
    _stprintf_s(debugBuffer, TEXT("Selected refresh rate is %d\n"), currentRefreshRate);
    OutputDebugString(debugBuffer);
    #endif

    return true;
}

bool OniLauncher::fetchSelectedDisplayMode()
{
    LRESULT getItemDataStatus = ComboBox_GetItemData(displayModeComboBox, ComboBox_GetCurSel(displayModeComboBox));

    if (getItemDataStatus == CB_ERR)
        return false;

    currentDisplayMode = (DISPLAYMODE)getItemDataStatus;

    #ifdef _DEBUG
    TCHAR debugBuffer[256];
    _stprintf_s(debugBuffer, TEXT("Selected display mode is %s\n"), currentDisplayMode == DISPLAYMODE::BORDERLESS ? TEXT("Borderless") : currentDisplayMode == DISPLAYMODE::FULLSCREEN ? TEXT("Fullscreen") : currentDisplayMode == DISPLAYMODE::WINDOWED ? TEXT("Windowed") : TEXT("Unknown"));
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

// TODO: Find a way to check if currentMonitor was selected. Or maybe select first monitor by default ?
bool OniLauncher::checkSettings()
{
    TCHAR loadedString[2][MAX_LOADSTRING];

    if (currentMonitor < 0U) {
        LoadString(::hInst, IDS_NO_MONITOR_SELECTED_TITLE, loadedString[0], MAX_LOADSTRING);
        LoadString(::hInst, IDS_NO_MONITOR_SELECTED_MSG, loadedString[1], MAX_LOADSTRING);
        MessageBox(hWnd, loadedString[1], loadedString[0], MB_OK | MB_ICONERROR);
        return false;
    }
    if (currentResolution.width == 0 || currentResolution.height == 0) {
        LoadString(::hInst, IDS_NO_RESOLUTION_SELECTED_TITLE, loadedString[0], MAX_LOADSTRING);
        LoadString(::hInst, IDS_NO_RESOLUTION_SELECTED_MSG, loadedString[1], MAX_LOADSTRING);
        MessageBox(hWnd, loadedString[1], loadedString[0], MB_OK | MB_ICONERROR);
        return false;
    }
    if (currentRefreshRate == 0) {
        LoadString(::hInst, IDS_NO_REFRESH_RATE_SELECTED_TITLE, loadedString[0], MAX_LOADSTRING);
        LoadString(::hInst, IDS_NO_REFRESH_RATE_SELECTED_MSG, loadedString[1], MAX_LOADSTRING);
        MessageBox(hWnd, loadedString[1], loadedString[0], MB_OK | MB_ICONERROR);
        return false;
    }
    if (currentDisplayMode == DISPLAYMODE::NONE) {
        LoadString(::hInst, IDS_NO_DISPLAY_MODE_SELECTED_TITLE, loadedString[0], MAX_LOADSTRING);
        LoadString(::hInst, IDS_NO_DISPLAY_MODE_SELECTED_MSG, loadedString[1], MAX_LOADSTRING);
        MessageBox(hWnd, loadedString[1], loadedString[0], MB_OK | MB_ICONERROR);
        return false;
    }
    return true;
}

bool OniLauncher::saveConfigFile()
{
    DWORD dwBytesWritten = 0;
    CHAR settingsDump[2048];
    TCHAR szPath[MAX_PATH];
    HANDLE saveFile;
    json settings = json{
        {"monitor", currentMonitor},
        {"resolution", {
            {"width", currentResolution.width},
        {"height", currentResolution.height}}},
        {"refreshRate", currentRefreshRate},
        {"fullscreen", currentDisplayMode == DISPLAYMODE::FULLSCREEN ? true : false},
        {"borderless", currentDisplayMode == DISPLAYMODE::BORDERLESS ? true : false},
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
    //settings.dump(4).size();
    strcpy_s(settingsDump, 2047, settings.dump(4).c_str());
    WriteFile(saveFile, settingsDump, strlen(settingsDump), &dwBytesWritten, NULL);

    CloseHandle(saveFile);

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
