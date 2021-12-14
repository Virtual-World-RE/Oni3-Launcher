/*
 ** This file is part of the Onimusha 3:RE Project !
 **
 ** You should have received a copy of the LICENSE along with this program.
 **
 ** Author: Gabriel GRONDIN (GGLinnk) <gglinnk@protonmail.com>
 ** Contributors (See CONTRIBUTION.md):
 */

#include "OniLauncher.h"

// Variables globales :
HINSTANCE hInst;                                // instance actuelle
TCHAR szTitle[MAX_LOADSTRING];                  // Texte de la barre de titre
TCHAR szWindowClass[MAX_LOADSTRING];            // nom de la classe de fenêtre principale

D3DDISPLAYMODE ***displaysModes = NULL;
LPDIRECT3D9 d3d = NULL;

int APIENTRY WINMAIN(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PTSTR pCmdLine, _In_ int nCmdShow)
{
    MSG msg;
    HACCEL hAccelTable;

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(pCmdLine);

    initD3D();
    initDisplaysModes();

    // Initialise les chaînes globales
    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_ONILAUNCHER, szWindowClass, MAX_LOADSTRING);

    // Insription de la classe de fenêtre
    if (!MyRegisterClass(hInstance))
        return FALSE;

    // Effectue l'initialisation de l'application :
    if (!InitInstance(hInstance, nCmdShow))
        return FALSE;

    hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ONILAUNCHER));

    // Boucle de messages principale :
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    destroyDisplaysModes();
    destroyD3D();

    return (int) msg.wParam;
}

//
//  FONCTION : MyRegisterClass()
//
//  OBJECTIF : Inscrit la classe de fenêtre.
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
    wcex.lpszMenuName = MAKEINTRESOURCE(IDC_ONILAUNCHER);
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
//   FONCTION : InitInstance(HINSTANCE, int)
//
//   OBJECTIF : enregistre le handle d'instance et crée une fenêtre principale
//
//   COMMENTAIRES :
//
//        Dans cette fonction, nous enregistrons le handle de l'instance dans une variable globale, puis
//        nous créons et affichons la fenêtre principale du programme.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    RECT rect;
    HWND hWnd;

    SecureZeroMemory(&rect, sizeof(RECT));
    rect.bottom = MAIN_WINDOW_HEIGHT;
    rect.right = MAIN_WINDOW_WIDTH;

    ::hInst = hInstance; // Stocke le handle d'instance dans la variable globale

    AdjustWindowRect(&rect, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, true);
    hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, NULL);

    if (!hWnd) {
        MessageBox(NULL, TEXT("Call to CreateWindow failed!"), TEXT("Error"), MB_ICONERROR);
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

void fillMonitorComboBox(HWND monitorComboBox)
{
    TCHAR buffer[MAX_SMALL_STRING];
    TCHAR monitorStr[MAX_SMALL_STRING];
    LoadString(::hInst, IDS_MONITOR, monitorStr, MAX_SMALL_STRING);
    
    for (UINT i = 0; ::displaysModes[i]; i++) {
        _stprintf_s(buffer, MAX_SMALL_STRING, TEXT("%s %d"), monitorStr, i);
        SendMessage(monitorComboBox, CB_ADDSTRING, 0, (LPARAM)buffer);
    }
}

void fillResolutionComboBox(UINT monitor, HWND resultionComboBox)
{
    TCHAR buffer[MAX_SMALL_STRING];

    for (UINT i = 0; ::displaysModes[monitor][i]; i++) {
        _stprintf_s(buffer, MAX_SMALL_STRING, TEXT("%d x %d"), ::displaysModes[monitor][i]->Width, ::displaysModes[monitor][i]->Height);
        if (i > 0 && ::displaysModes[monitor][i - 1]->Width == ::displaysModes[monitor][i]->Width && ::displaysModes[monitor][i - 1]->Height == ::displaysModes[monitor][i]->Height)
            continue;
        SendMessage(resultionComboBox, CB_ADDSTRING, 0, (LPARAM)buffer);
    }
}

LRESULT wndProcCreate(HWND hWnd)
{
    HWND monitorComboBox = CreateWindow(TEXT("COMBOBOX"), NULL, DROPDOWN_COMBO_BOX, 5, 150, 290, 200, hWnd, (HMENU)ID_MONITOR, hInst, NULL);
    HWND resolutionComboBox = CreateWindow(TEXT("COMBOBOX"), NULL, DROPDOWN_COMBO_BOX, 5, 200, 180, 200, hWnd, (HMENU)ID_RESOLUTION, hInst, NULL);
    HWND frequencyComboBox = CreateWindow(TEXT("COMBOBOX"), NULL, DROPDOWN_COMBO_BOX, 190, 200, 105, 200, hWnd, (HMENU)ID_REFRESH_RATE, hInst, NULL);
    HWND enableDebugCheckBox = CreateWindow(TEXT("BUTTON"), TEXT("Enable debug menu"), DEFAULT_STYLE | BS_AUTOCHECKBOX, 5, 395, 290, 20, hWnd, (HMENU)ID_DEBUG, hInst, NULL);
    HWND saveButton = CreateWindow(TEXT("BUTTON"), TEXT("Save settings"), DEFAULT_STYLE, 5, 420, 290, 30, hWnd, (HMENU)ID_SAVE, hInst, NULL);
    HWND playButton = CreateWindow(TEXT("BUTTON"), TEXT("Play now"), DEFAULT_STYLE, 5, 455, 290, 40, hWnd, (HMENU)ID_PLAY, hInst, NULL);

    EnableWindow(resolutionComboBox, false);
    EnableWindow(frequencyComboBox, false);

    fillMonitorComboBox(monitorComboBox);

    return 0;
}

LRESULT saveConfigFile(HWND hWnd)
{
    HWND monitorComboBox = GetDlgItem(hWnd, ID_MONITOR);
    HWND resolutionComboBox = GetDlgItem(hWnd, ID_RESOLUTION);
    HWND frequencyComboBox = GetDlgItem(hWnd, ID_REFRESH_RATE);
    HWND enableDebugCheckBox = GetDlgItem(hWnd, ID_DEBUG);
    TCHAR lpText[256];

    GetWindowText(monitorComboBox, lpText, 256);
    MessageBox(hWnd, lpText, TEXT("Info"), MB_ICONINFORMATION);

    return 0;
}

UINT parseNumber(LPTSTR str)
{
    UINT j = 0;
    UINT result = 0;
    LPTSTR number = (LPTSTR)calloc(_tcslen(str), sizeof(CHAR));

    if (number == NULL)
        return -1;

    for (INT i = 0; str[i]; i++) {
        if (_istdigit(str[i])) {
            number[j++] = str[i];
        }
    }

    result = _tstoi(number);
    free(number);

    return result;
}

LRESULT wndProcCommandMonitor(HWND hWnd, UINT iCode)
{
    TCHAR lpText[256];
    HWND monitorComboBox = NULL;
    HWND resolutionComboBox = NULL;

    if (iCode == CBN_SELENDOK) {
        monitorComboBox = GetDlgItem(hWnd, ID_MONITOR);
        resolutionComboBox = GetDlgItem(hWnd, ID_RESOLUTION);

        GetWindowText(monitorComboBox, lpText, 256);
        SendMessage(resolutionComboBox, CB_RESETCONTENT, NULL, NULL);
        //MessageBox(hWnd, lpText, TEXT("Info"), MB_ICONINFORMATION);
        fillResolutionComboBox(parseNumber(lpText), resolutionComboBox);
        EnableWindow(resolutionComboBox, true);
    }

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
    case ID_MONITOR:
        return wndProcCommandMonitor(hWnd, iCode);
    case ID_SAVE:
        return saveConfigFile(hWnd);
    case ID_PLAY:
    {
        saveConfigFile(hWnd);
        //pseudoWinMain(hWnd);
        MessageBox(hWnd, TEXT("Play button pressed"), TEXT("Info"), MB_ICONINFORMATION);
    }
    break;
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
    switch (message)
    {
    case WM_CREATE:
        return wndProcCreate(hWnd);
    case WM_PAINT:
    {
        TCHAR loadedString[MAX_SMALL_STRING];
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        LoadString(::hInst, IDS_MONITOR_DESC, loadedString, MAX_SMALL_STRING);
        TextOut(hdc, 5, 130, loadedString, _tcslen(loadedString));
        TextOut(hdc, 5, 180, TEXT("Resolution:"), _tcslen(TEXT("Resolution:")));
        TextOut(hdc, 190, 180, TEXT("Refresh rate:"), _tcslen(TEXT("Refresh rate:")));

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
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR) TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

LONG initD3D()
{
    ::d3d = Direct3DCreate9(D3D_SDK_VERSION);

    if (::d3d == NULL)
        return E_FAIL;
    return S_OK;
}

VOID destroyD3D()
{
    if (::d3d != NULL)
        ::d3d->Release();
}

BOOL initDisplaysModes()
{
    UINT adapters = ::d3d->GetAdapterCount();
    ::displaysModes = (D3DDISPLAYMODE ***)calloc(adapters + 1U, sizeof(D3DDISPLAYMODE **));

    if (!adapters || ::displaysModes == NULL)
        return FALSE;

    for (UINT i = 0; i < adapters; i++) {
        UINT modes = ::d3d->GetAdapterModeCount(i, D3DFMT_MODE);
        ::displaysModes[i] = (D3DDISPLAYMODE **)calloc(modes + 1U, sizeof(D3DDISPLAYMODE *));

        if (::displaysModes[i] == NULL)
            return FALSE;

        for (UINT j = 0; j < modes; j++) {
            ::displaysModes[i][j] = (D3DDISPLAYMODE *)calloc(1, sizeof(D3DDISPLAYMODE));
            ::d3d->EnumAdapterModes(i, D3DFMT_MODE, j, ::displaysModes[i][j]);
            ::displaysModes[i][j + 1] = NULL;
        }
    }

    return TRUE;
}

VOID destroyDisplaysModes()
{
    if (::displaysModes == NULL)
        return;

    for (UINT i = 0; ::displaysModes[i]; i++) {
        for (UINT j = 0; ::displaysModes[i][j]; j++) {
            free(::displaysModes[i][j]);
            ::displaysModes[i][j] = NULL;
        }
        free(::displaysModes[i]);
        ::displaysModes[i] = NULL;
    }
    free(::displaysModes);
    ::displaysModes = NULL;
}
