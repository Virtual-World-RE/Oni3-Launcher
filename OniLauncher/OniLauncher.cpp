// OniLauncher.cpp : Définit le point d'entrée de l'application.
//

/*
 ** This file is part of the Onimusha O3:RE Project !
 **
 ** You should have received a copy of the LICENSE along with this program.
 **
 ** Author: Gabriel GRONDIN (GGLinnk) <gglinnk@protonmail.com>
 ** Contributors (See CONTRIBUTION.md):
 **  AlgoFlash
 **  Rigodron
 */

#include "framework.h"
#include "OniLauncher.h"

#define MAX_LOADSTRING 100
#define SPAWNLP_ERROR -1

// Variables globales :
HINSTANCE hInst;                                // instance actuelle
WCHAR szTitle[MAX_LOADSTRING];                  // Texte de la barre de titre
WCHAR szWindowClass[MAX_LOADSTRING];            // nom de la classe de fenêtre principale

// Variables globales identifiées :
DWORD hKeyCurrentId;
HKEY hKey;
LPSTR lpClass;

// Variables globales a identifier :
CHAR char_409428[260U];
DWORD dword_408054;

// Déclarations anticipées des fonctions incluses dans ce module de code :
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// Déclarations anticipées des fonctions a identifier dans ce module de code :
BOOL                sub_401450(LPCSTR lpSubKey);
BOOL                sub_4014F0(LPCSTR regKeyPath, LPBYTE lpDataBuffer, LPDWORD lpcbData);

// Reversed functions
int                 pseudoWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);
int                 checkPrintError(intptr_t processHandle);
BOOL                isEhshellRunning();
BOOL                resetGlobalHKEY();
BOOL                checkRegKeyType(LPCSTR regKeyPath, DWORD regKeyExpectedValue);
BOOL                setCurrentHKEY(DWORD hKeyId);
BOOL                getInstallPathRegKey(LPBYTE lpData, LPDWORD lpcbData);
BOOL                checkRegKeyExist(LPCSTR lpSubKey);

BOOL checkRegKeyExist(LPCSTR lpSubKey) // NAWAK
{
	signed int i = 0;
	CHAR i_shift;
	size_t subKeyLen;
	signed int j = 0;
	LPDWORD v6;
	char v7;
	unsigned int lpSubKeyLength;
	char* v9;
	int k = 0;
	char k_shift;
	HKEY hKey;
	HKEY phkResult = 0;
	unsigned int subKeyLength; 
	DWORD dwDisposition;
	CHAR SubKey[256]; 
	__int16 v19 = 0; 
	char v20 = 0; 

	memset(SubKey, 0, sizeof(SubKey));

	switch (hKeyCurrentId)
	{
	case 1:
		hKey = HKEY_CLASSES_ROOT;
		goto LABEL_8;
	case 2:
		hKey = HKEY_CURRENT_USER;
		goto LABEL_8;
	case 3:
		hKey = HKEY_LOCAL_MACHINE;
		goto LABEL_8;
	case 4:
		hKey = HKEY_USERS;
	LABEL_8:
		if (!strcmp(lpSubKey, ".."))
		{
			subKeyLen = strlen(SubKey);
			if (!subKeyLen)
				return 0;

			for (j = subKeyLen - 1; j >= 0; --j)
				if (SubKey[j] == '\\')
					break;

			if (j == -1)
			{
				setCurrentHKEY(hKeyCurrentId);
				return 1;
			}
			SubKey[j] = 0;
			subKeyLength = strlen(SubKey);
			if (!subKeyLength)
				return 0;
		}
		else
		{
			if (!sub_401450(lpSubKey))
				return 0;
			subKeyLength = strlen(SubKey);
			if (subKeyLength && (dwDisposition + subKeyLength + 3) != '\\') // FIXME
			{
				//v6 = dwDisposition + 3;// FIXME
				do
				{
					v7 = v6 + 1; // FIXME
					v6++; // FIXME
				} while (v7);
				*v6 = '\\';
			}
			lpSubKeyLength = strlen(lpSubKey) + 1;
			v9 = (char*)&dwDisposition + 3;

			while (*++v9);

			memcpy(v9, lpSubKey, lpSubKeyLength);
		}

		if (RegCreateKeyExA(hKey, SubKey, 0, lpClass, 0, KEY_READ | KEY_WRITE, 0, &phkResult, &dwDisposition))
			return 0;

		resetGlobalHKEY();
		::hKey = phkResult;

		do
		{
			k_shift = SubKey[k];
			char_409428[k++] = k_shift;
		} while (k_shift);

		return 1;
	default:
		return 0;
	}
}

int pseudoWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	BOOL ehshellWasRunning = FALSE;  // TRUE if Windows Media Center Shell is running.
	HWND wmcHandler;                 // Windows Media Center window handler.
	int oni3GameHandler;             // Onimusha 3 oni3.exe process handler.
	int bufferErrorCode;             // Unknown process handler.
	CHAR oni3GameExePath[256];       // Store oni3.exe executable path.
	CHAR windirPath[256];
	OFSTRUCT ReOpenBuff[128];
	CHAR unknVar1[256];
	DWORD unknVar2[256];

	GetEnvironmentVariableA("windir", windirPath, sizeof(windirPath));

	strcpy_s(unknVar1, R"(\ehome\ehshell.exe)");
	getInstallPathRegKey((LPBYTE)oni3GameExePath, unknVar2);

	strcpy_s(oni3GameExePath, R"(\oni3.exe)");
	if (OpenFile(oni3GameExePath, ReOpenBuff, OF_EXIST) == HFILE_ERROR)
		MessageBoxA(0, "Cannot Find the Game", 0, 0);
	else
	{
		// Close the Windows Media Center if running (Windows XP)
		if (isEhshellRunning())
		{
			wmcHandler = FindWindowA(0, "Media Center");
			if (CloseWindow(wmcHandler)) {
				ehshellWasRunning = TRUE;
				Sleep(200U);
			}
		}
		// Executing Onimusha 3
		oni3GameHandler = _spawnlp(_P_WAIT, oni3GameExePath, oni3GameExePath, "-fullscreen", NULL);
		checkPrintError(oni3GameHandler);

		if (ehshellWasRunning)
		{
			bufferErrorCode = _spawnlp(_P_NOWAIT, windirPath, windirPath, NULL);
			checkPrintError(bufferErrorCode);
		}
	}
	return 0;
}

BOOL sub_401450(LPCSTR lpSubKey)
{
	HKEY phkResult = 0;

	if (!hKey || !lpSubKey || RegOpenKeyExA(hKey, lpSubKey, 0, 0x2001Fu, &phkResult))
		return FALSE;
	RegCloseKey(phkResult);
	return TRUE;
}

BOOL sub_4014F0(LPCSTR regKeyPath, LPBYTE lpDataBuffer, LPDWORD lpcbData)
{
	if (hKey && regKeyPath && lpDataBuffer && checkRegKeyType(regKeyPath, 1))
		return RegQueryValueExA(hKey, regKeyPath, 0, 0, lpDataBuffer, lpcbData) == 0;
	return FALSE;
}

BOOL getInstallPathRegKey(LPBYTE lpData, LPDWORD lpcbData)
{
	BOOL result;

	if (!setCurrentHKEY(3) || !checkRegKeyExist	(R"(SOFTWARE\CAPCOM\ONIMUSHA3 PC\1.00.000)"))
		return -1;
	result = sub_4014F0("InstallPath", lpData, lpcbData);
	setCurrentHKEY(0);

	return result;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Placez le code ici.

	// Initialise les chaînes globales
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_ONILAUNCHER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Effectue l'initialisation de l'application :
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ONILAUNCHER));

	// Boucle de messages principale :
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

//
//  FONCTION : MyRegisterClass()
//
//  OBJECTIF : Inscrit la classe de fenêtre.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ONILAUNCHER));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_ONILAUNCHER);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
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
	HWND hWnd;

	hInst = hInstance; // Stocke le handle d'instance dans la variable globale
	hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
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
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Analyse les sélections de menu :
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Ajoutez ici le code de dessin qui utilise hdc...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Gestionnaire de messages pour la boîte de dialogue À propos de.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

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

BOOL checkRegKeyType(LPCSTR lpValueName, DWORD regKeyExpectedValue)
{
	DWORD regType;

	if (hKey && lpValueName && !RegQueryValueExA(hKey, lpValueName, NULL, &regType, NULL, NULL))
		return regKeyExpectedValue == regType;
	return FALSE;
}

BOOL setCurrentHKEY(DWORD hKeyId)
{
	if (hKey)
		resetGlobalHKEY();

	switch (hKeyId)
	{
	case 1:
		hKeyCurrentId = hKeyId;
		hKey = HKEY_CLASSES_ROOT;
		return TRUE;
	case 2:
		hKeyCurrentId = hKeyId;
		hKey = HKEY_CURRENT_USER;
		return TRUE;
	case 3:
		hKeyCurrentId = hKeyId;
		hKey = HKEY_LOCAL_MACHINE;
		return TRUE;
	case 4:
		hKeyCurrentId = hKeyId;
		hKey = HKEY_USERS;
		return TRUE;
	default:
		hKeyCurrentId = 0;
		hKey = 0;
		return FALSE;
	}
}

BOOL resetGlobalHKEY()
{
	if (hKey && hKey != HKEY_CLASSES_ROOT && hKey != HKEY_CURRENT_USER && hKey != HKEY_LOCAL_MACHINE && hKey != HKEY_USERS)
		RegCloseKey(hKey);

	memset(char_409428, 0, sizeof(char_409428));
	hKey = 0;

	return FALSE;
}

int checkPrintError(intptr_t processHandle)
{
	CHAR errorMessage[256U] = {0};
	if (processHandle == SPAWNLP_ERROR && errno)
	{
		strerror_s(errorMessage, errno);
		return MessageBoxA(0, (LPCSTR)errorMessage, NULL, MB_OK | MB_ICONERROR);
	}
	return 0;

}

BOOL isEhshellRunning()
{
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;

	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
		return FALSE;

	// Set the size of the structure before using it.
	pe32.dwSize = sizeof(PROCESSENTRY32);

	// Retrieve information about the first process, and exit if unsuccessful
	if (!Process32First(hProcessSnap, &pe32))
	{
		CloseHandle(hProcessSnap);
		return FALSE;
	}

	while (wcscmp(pe32.szExeFile, TEXT("ehshell.exe")))
	{
		if (!Process32Next(hProcessSnap, &pe32))
		{
			CloseHandle(hProcessSnap);
			return 0;
		}
	}
	CloseHandle(hProcessSnap);
	return 1;
}