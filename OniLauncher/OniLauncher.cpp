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
OriLauncher oriLauncher;

//
// Application main function
//
int APIENTRY WINMAIN(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PTSTR pCmdLine, _In_ int nCmdShow)
{
    MSG msg;
    HACCEL hAccelTable;

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(pCmdLine);

    // Initializing our global string
    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDS_ONILAUNCHER, szWindowClass, MAX_LOADSTRING);

    // Registering windows class
    if (!MyRegisterClass(hInstance))
        return FALSE;

    // Initializing our app
    if (!InitInstance(hInstance, nCmdShow))
        return FALSE;

    hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDS_ONILAUNCHER));

    // Main message loop:
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  OBJECTIF : Register Windows class.
//
BOOL MyRegisterClass(HINSTANCE hInstance)
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
        MessageBox(NULL, TEXT("Call to RegisterClassEx failed!"), TEXT("Class Registration Error"), MB_ICONERROR);
        return FALSE;
    }

    return TRUE;
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   OBJECTIF : Save instance handle and create a main window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    RECT rect;
    HWND hWnd;

    SecureZeroMemory(&rect, sizeof(RECT));
    rect.bottom = MAIN_WINDOW_HEIGHT;
    rect.right = MAIN_WINDOW_WIDTH;

    ::hInst = hInstance; // Stocke le handle d'instance dans la variable globale

    AdjustWindowRect(&rect, WINDOWS_STYLE, true);
    hWnd = CreateWindow(szWindowClass, szTitle, WINDOWS_STYLE, CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, NULL);

    if (!hWnd) {
        MessageBox(NULL, TEXT("Call to CreateWindow failed!"), TEXT("Error"), MB_ICONERROR);
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

bool CALLBACK SetFont(HWND child, LPARAM font)
{
    SendMessage(child, WM_SETFONT, font, true);
    return true;
}

LRESULT wndProcCreate(HWND hWnd)
{
    TCHAR loadedString[MAX_SMALL_STRING];
    HFONT defaultFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
    HWND monitorComboBox = CreateWindow(WC_COMBOBOX, TEXT(""), DROPDOWN_COMBO_BOX | WS_DISABLED, 5, 150, 240, 200, hWnd, (HMENU)ID_MONITOR, hInst, NULL);
    HWND resolutionComboBox = CreateWindow(WC_COMBOBOX, TEXT(""), DROPDOWN_COMBO_BOX | WS_DISABLED, 5, 200, 130, 200, hWnd, (HMENU)ID_RESOLUTION, hInst, NULL);
    HWND refreshRateComboBox = CreateWindow(WC_COMBOBOX, TEXT(""), DROPDOWN_COMBO_BOX | WS_DISABLED, 140, 200, 105, 200, hWnd, (HMENU)ID_REFRESH_RATE, hInst, NULL);
    HWND displayModeComboBox = CreateWindow(WC_COMBOBOX, TEXT(""), DROPDOWN_COMBO_BOX, 5, 250, 240, 200, hWnd, (HMENU)ID_DISPLAY_MODE, hInst, NULL);
    //HWND fpsComboBox = CreateWindow(WC_COMBOBOX, TEXT(""), DROPDOWN_COMBO_BOX, 5, 250, 240, 200, hWnd, (HMENU)ID_FRAME_PER_SECONDS, hInst, NULL);

    LoadString(::hInst, IDS_MONITOR_DESC, loadedString, MAX_SMALL_STRING);
    CreateWindow(WC_STATIC, loadedString, DEFAULT_STYLE, 5, 130, 240, 16, hWnd, NULL, hInst, NULL);
    LoadString(::hInst, IDS_RESOLUTION_DESC, loadedString, MAX_SMALL_STRING);
    CreateWindow(WC_STATIC, loadedString, DEFAULT_STYLE, 5, 180, 130, 16, hWnd, NULL, hInst, NULL);
    LoadString(::hInst, IDS_REFRESH_DESC, loadedString, MAX_SMALL_STRING);
    CreateWindow(WC_STATIC, loadedString, DEFAULT_STYLE, 140, 180, 105, 16, hWnd, NULL, hInst, NULL);
    LoadString(::hInst, IDS_DISPLAY_MODE_DESC, loadedString, MAX_SMALL_STRING);
    CreateWindow(WC_STATIC, loadedString, DEFAULT_STYLE, 5, 230, 240, 16, hWnd, NULL, hInst, NULL);

    LoadString(::hInst, IDS_DEBUG, loadedString, MAX_SMALL_STRING);
    CreateWindow(WC_BUTTON, loadedString, DEFAULT_STYLE | BS_AUTOCHECKBOX, 5, 295, 240, 20, hWnd, (HMENU)ID_DEBUG, hInst, NULL);
    LoadString(::hInst, IDS_SAVE, loadedString, MAX_SMALL_STRING);
    CreateWindow(WC_BUTTON, loadedString, DEFAULT_STYLE, 5, 320, 240, 30, hWnd, (HMENU)ID_SAVE, hInst, NULL);
    LoadString(::hInst, IDS_PLAY, loadedString, MAX_SMALL_STRING);
    CreateWindow(WC_BUTTON, loadedString, DEFAULT_STYLE, 5, 355, 240, 40, hWnd, (HMENU)ID_PLAY, hInst, NULL);

    EnumChildWindows(hWnd, (WNDENUMPROC)SetFont, (LPARAM)GetStockObject(DEFAULT_GUI_FONT));

    oniLauncher.setHandlers(hWnd, monitorComboBox, resolutionComboBox, refreshRateComboBox, displayModeComboBox);
    oniLauncher.fillMonitorComboBox();

    return 0;
}

LRESULT wndProcCommandScreenSettings(HWND hWnd, UINT wmId, UINT iCode)
{
    if (iCode == CBN_SELENDOK) {
        switch (wmId) {
        case ID_MONITOR:
        {
            oniLauncher.resetResolution();
            oniLauncher.resetRefreshRate();

            oniLauncher.fetchCurrentMonitor();
            oniLauncher.fillResolutionComboBox();
            break;
        }
        case ID_RESOLUTION:
        {
            oniLauncher.resetRefreshRate();

            oniLauncher.fetchCurrentResolution();
            oniLauncher.fillRefreshComboBox();
            break;
        }
        case ID_REFRESH_RATE:
        {
            oniLauncher.fetchCurrentRefreshRate();
            break;
        }
        }
    }

    return 0;
}

LRESULT wndProcCommandButtonAction(HWND hWnd, UINT wmId, UINT iCode)
{
    oniLauncher.saveConfigFile();

    /*switch (wmId) {
    case ID_PLAY:
    {
        //pseudoWinMain(hWnd);
        break;
    }
    }*/

    return 0;
}

LRESULT wndProcCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    UINT wmId = LOWORD(wParam);
    UINT iCode = HIWORD(wParam);
    HWND hCtl = (HWND)lParam;

    // Analyse les sélections de menu :
    switch (wmId) {
    case IDM_ABOUT:
        DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
        break;
    case IDM_EXIT:
        return !DestroyWindow(hWnd);
    case ID_REFRESH_RATE:
    case ID_RESOLUTION:
    case ID_MONITOR:
        return wndProcCommandScreenSettings(hWnd, wmId, iCode);
    case ID_SAVE:
    case ID_PLAY:
        return wndProcCommandButtonAction(hWnd, wmId, iCode);
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

//
//  FONCTION : WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  OBJECTIF : Traite les messages pour la fenêtre principale.
//
//  WM_COMMAND  - traite le menu de l'application
//  WM_PAINT    - Dessine la fenêtre principale
//  WM_DESTROY  - génère un message d'arrêt et retourne
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case WM_CREATE:
        return wndProcCreate(hWnd);
    case WM_CTLCOLORSTATIC:
    {
        HDC hdcStatic = (HDC)wParam;
        HBRUSH BGColorBrush = CreateSolidBrush(RGB(255, 255, 255)); // 230 230

        SetTextColor(hdcStatic, RGB(0, 0, 0));
        SetBkMode(hdcStatic, TRANSPARENT);
        return (LRESULT)BGColorBrush;
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_COMMAND:
        return wndProcCommand(hWnd, message, wParam, lParam);
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Gestionnaire de messages pour la boîte de dialogue "À propos de".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message) {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

LPUINT getNumbers(LPTSTR str)
{
    INT occurrences = 0;
    INT j = 0;
    LPUINT realloced = FALSE;
    BOOL trigger = FALSE;
    LPUINT result = NULL;
    LPTSTR nbrBuffer = (LPTSTR)calloc(_tcslen(str), sizeof(TCHAR));

    if (nbrBuffer == NULL)
        return NULL;

    for (UINT i = 0; str[i]; i++) {
        if (_istdigit(str[i])) {
            nbrBuffer[j++] = str[i];
            trigger = TRUE;
        } else if (trigger) {
            trigger = FALSE;
            ++occurrences;
            realloced = (LPUINT)realloc(result, sizeof(INT) * occurrences);
            if (realloced == NULL) {
                if (result) free(result);
                return NULL;
            } else
                result = realloced;
            result[occurrences - 1] = _tstoi(nbrBuffer);
            SecureZeroMemory(nbrBuffer, sizeof(TCHAR) * j);
            j = 0;
        }
    }

    if (trigger) {
        ++occurrences;
        realloced = (LPUINT)realloc(result, sizeof(INT) * occurrences);
        if (realloced == NULL) {
            if (result) free(result);
            return NULL;
        } else
            result = realloced;
        result[occurrences - 1] = (UINT)_tstoi(nbrBuffer);
    }

    free(nbrBuffer);

    return result;
}

BOOL OniLauncher::jsonLoadConfig()
{
    return TRUE;
}

BOOL OniLauncher::jsonConfigExists()
{
    TCHAR szPath[MAX_PATH];

    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, szPath))) {
        PathCchAppend(szPath, MAX_PATH, TEXT("OniLauncher"));
        PathCchAppend(szPath, MAX_PATH, TEXT("config.json"));
        if (PathFileExists(szPath))
            if (!PathIsDirectory(szPath)) {
                #ifdef _DEBUG
                OutputDebugString(TEXT("Found JSON config file: "));
                OutputDebugString(szPath);
                OutputDebugString(TEXT("\n"));
                #endif
                return TRUE;
            }
    }

    return FALSE;
}

OniLauncher::OniLauncher()
{
    this->initD3D();
    this->initMonitorDisplayModes();
    if (this->jsonConfigExists())
        this->jsonLoadConfig();
}

VOID OniLauncher::initD3D()
{
    d3d = Direct3DCreate9(D3D_SDK_VERSION);

    if (d3d == NULL)
        throw TEXT("D3D Failed to initialize...");
}

BOOL OniLauncher::initMonitorDisplayModes()
{
    UINT adapters = d3d->GetAdapterCount();

    monitorDisplayModes = (MONITORDISPLAYMODES **)calloc(adapters + 1U, sizeof(MONITORDISPLAYMODES *));

    if (adapters <= 0 || monitorDisplayModes == NULL)
        return FALSE;

    for (UINT i = 0; i < adapters; i++) {
        MONITORDISPLAYMODES *currentDisplayMode = NULL;
        UINT modes = d3d->GetAdapterModeCount(i, D3DFMT_MODE);

        monitorDisplayModes[i] = (MONITORDISPLAYMODES *)malloc(sizeof(MONITORDISPLAYMODES));
        currentDisplayMode = monitorDisplayModes[i];

        if (monitorDisplayModes[i] == NULL)
            return FALSE;

        for (UINT j = 0; j < modes; j++) {
            d3d->EnumAdapterModes(i, D3DFMT_MODE, j, &currentDisplayMode->displayMode);

            if (j < modes - 1U) {
                currentDisplayMode->next = (MONITORDISPLAYMODES *)malloc(sizeof(MONITORDISPLAYMODES));

                if (currentDisplayMode->next == NULL)
                    return FALSE;

                currentDisplayMode = currentDisplayMode->next;
            }

            currentDisplayMode->next = NULL;
        }
    }

    return TRUE;
}

VOID OniLauncher::setHandlers(HWND hWnd, HWND monitorComboBox, HWND resolutionComboBox, HWND refreshRateComboBox, HWND displayModeComboBox)
{
    this->hWnd = hWnd;
    this->monitorComboBox = monitorComboBox;
    this->resolutionComboBox = resolutionComboBox;
    this->refreshRateComboBox = refreshRateComboBox;
    this->displayModeComboBox = displayModeComboBox;
}


UINT OniLauncher::fetchCurrentMonitor()
{
    TCHAR lpText[256];
    LPUINT numbers = NULL;

    GetWindowText(monitorComboBox, lpText, 256);
    numbers = getNumbers(lpText);
    currentMonitor = numbers[0];
    free(numbers);

    #ifdef _DEBUG
    TCHAR debugBuffer[256];
    _stprintf_s(debugBuffer, TEXT("Current monitor is %d\n"), currentMonitor);
    OutputDebugString(debugBuffer);
    #endif

    return currentMonitor;
}

RESOLUTION OniLauncher::fetchCurrentResolution()
{
    TCHAR lpText[256];
    LPUINT numbers = NULL;

    GetWindowText(resolutionComboBox, lpText, 256);
    numbers = getNumbers(lpText);
    currentResolution.width = numbers[0];
    currentResolution.height = numbers[1];
    free(numbers);

    #ifdef _DEBUG
    TCHAR debugBuffer[256];
    _stprintf_s(debugBuffer, TEXT("Current resolution set to %d x %d\n"), currentResolution.width, currentResolution.height);
    OutputDebugString(debugBuffer);
    #endif

    return currentResolution;
}

UINT OniLauncher::fetchCurrentRefreshRate()
{
    TCHAR lpText[256];
    LPUINT numbers = NULL;

    GetWindowText(refreshRateComboBox, lpText, 256);
    numbers = getNumbers(lpText);
    currentRefreshRate = numbers[0];
    free(numbers);

    #ifdef _DEBUG
    TCHAR debugBuffer[256];
    _stprintf_s(debugBuffer, TEXT("Current refresh rate is %d\n"), currentRefreshRate);
    OutputDebugString(debugBuffer);
    #endif

    return currentRefreshRate;
}

VOID OniLauncher::resetMonitor()
{
    EnableWindow(monitorComboBox, false);
    SendMessage(monitorComboBox, CB_RESETCONTENT, NULL, NULL);
    currentMonitor = 0;
}

VOID OniLauncher::resetResolution()
{
    EnableWindow(resolutionComboBox, false);
    SendMessage(resolutionComboBox, CB_RESETCONTENT, NULL, NULL);
    currentResolution.width = 0;
    currentResolution.height = 0;
}

VOID OniLauncher::resetRefreshRate()
{
    EnableWindow(refreshRateComboBox, false);
    SendMessage(refreshRateComboBox, CB_RESETCONTENT, NULL, NULL);
    currentRefreshRate = 0;
}

UINT OniLauncher::getCurrentMonitor()
{
    return currentMonitor;
}

RESOLUTION OniLauncher::getCurrentResolution()
{
    return currentResolution;
}

UINT OniLauncher::getCurrentRefreshRate()
{
    return currentRefreshRate;
}

VOID OniLauncher::fillMonitorComboBox()
{
    TCHAR buffer[MAX_SMALL_STRING];
    TCHAR monitorStr[MAX_SMALL_STRING];
    LoadString(::hInst, IDS_MONITOR, monitorStr, MAX_SMALL_STRING);
    
    for (UINT i = 0; oniLauncher.monitorDisplayModes[i]; i++) {
        _stprintf_s(buffer, MAX_SMALL_STRING, TEXT("%s %d"), monitorStr, i);
        SendMessage(monitorComboBox, CB_ADDSTRING, 0, (LPARAM)buffer);
    }

    EnableWindow(monitorComboBox, TRUE);
}

VOID OniLauncher::fillResolutionComboBox()
{
    TCHAR buffer[MAX_SMALL_STRING];
    MONITORDISPLAYMODES *currentMonitorDisplayMode = monitorDisplayModes[currentMonitor];

    while (currentMonitorDisplayMode) {
        if (!(currentMonitorDisplayMode->next && currentMonitorDisplayMode->displayMode.Width == currentMonitorDisplayMode->next->displayMode.Width && currentMonitorDisplayMode->displayMode.Height == currentMonitorDisplayMode->next->displayMode.Height)) {
            _stprintf_s(buffer, MAX_SMALL_STRING, TEXT("%d x %d"), currentMonitorDisplayMode->displayMode.Width, currentMonitorDisplayMode->displayMode.Height);
            SendMessage(resolutionComboBox, CB_ADDSTRING, 0, (LPARAM)buffer);
        }
        currentMonitorDisplayMode = currentMonitorDisplayMode->next;
    }

    EnableWindow(resolutionComboBox, TRUE);
}

VOID OniLauncher::fillRefreshComboBox()
{
    TCHAR buffer[MAX_SMALL_STRING];
    MONITORDISPLAYMODES *currentMonitorDisplayMode = monitorDisplayModes[currentMonitor];

    while (currentMonitorDisplayMode) {
        if (currentResolution.width == currentMonitorDisplayMode->displayMode.Width && currentResolution.height == currentMonitorDisplayMode->displayMode.Height) {
            _stprintf_s(buffer, MAX_SMALL_STRING, TEXT("%d Hz"), currentMonitorDisplayMode->displayMode.RefreshRate);
            SendMessage(refreshRateComboBox, CB_ADDSTRING, 0, (LPARAM)buffer);
        }
        currentMonitorDisplayMode = currentMonitorDisplayMode->next;
    }

    EnableWindow(refreshRateComboBox, TRUE);
}

LRESULT OniLauncher::saveConfigFile()
{
    debugEnabled = IsDlgButtonChecked(hWnd, ID_DEBUG) == BST_CHECKED;

    #ifdef _DEBUG
    TCHAR message[128];

    OutputDebugString(TEXT("INFO: Save button pressed\n\n"));

    _stprintf_s(message, TEXT("Monitor: %u\n"), currentMonitor);
    OutputDebugString(message);
    _stprintf_s(message, TEXT("Resolution: %u x %u\n"), currentResolution.width, currentResolution.height);
    OutputDebugString(message);
    _stprintf_s(message, TEXT("Refresh rate: %u\n"), currentRefreshRate);
    OutputDebugString(message);
    _stprintf_s(message, TEXT("Debug: %s\n"), debugEnabled ? TEXT("true") : TEXT("false"));
    OutputDebugString(message);
    #endif

    if (currentMonitor == 0)
        return MessageBox(hWnd, TEXT("You should choose a monitor to continue."), TEXT("No monitor selected !"), MB_OK | MB_ICONERROR);
    if (currentResolution.width == 0 || currentResolution.height == 0)
        return MessageBox(hWnd, TEXT("You should choose a resolution to continue."), TEXT("No resolution selected !"), MB_OK | MB_ICONERROR);
    if (currentRefreshRate == 0)
        return MessageBox(hWnd, TEXT("You should choose a refresh rate to continue."), TEXT("No refresh rate selected !"), MB_OK | MB_ICONERROR);

    json settings = json{ {"monitor", currentMonitor}, {"resolution", {{"width", currentResolution.width}, {"height", currentResolution.height}}}, {"refreshRate", currentRefreshRate}, {"debugEnabled", debugEnabled}};

    OutputDebugStringA(settings.dump(4).c_str());

    return 0;
}

VOID OniLauncher::destroyMonitorDisplayModes()
{
    MONITORDISPLAYMODES *currentDisplayMode = NULL;
    MONITORDISPLAYMODES *nextDisplayMode = NULL;

    if (monitorDisplayModes == NULL)
        return;

    for (UINT i = 0; monitorDisplayModes[i]; i++) {
        nextDisplayMode = monitorDisplayModes[i];
        while (currentDisplayMode) {
            currentDisplayMode = nextDisplayMode;
            nextDisplayMode = currentDisplayMode->next;
            free(currentDisplayMode);
        }
        free(monitorDisplayModes[i]);
        monitorDisplayModes[i] = NULL;
    }
    free(monitorDisplayModes);
    monitorDisplayModes = NULL;
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
