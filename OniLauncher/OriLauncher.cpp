/*
 ** This file is part of the Onimusha 3:RE Project !
 **
 ** You should have received a copy of the LICENSE along with this program.
 **
 ** Author: Gabriel GRONDIN (GGLinnk) <gglinnk@protonmail.com>
 ** Contributors (See CONTRIBUTION.md):
 */

#include "OriLauncher.hpp"

/// <summary>
/// 
/// </summary>
OriLauncher::OriLauncher()
{
    SecureZeroMemory(&oni3GameSI, sizeof(STARTUPINFO));
    SecureZeroMemory(&oni3GamePI, sizeof(PROCESS_INFORMATION));
    SecureZeroMemory(oni3GamePath, sizeof(TCHAR) * MAX_PATH);
    SecureZeroMemory(oni3GameExePath, sizeof(TCHAR) * MAX_PATH);

    oni3GameSI.cb = sizeof(STARTUPINFO);
    getRegKeyValue(TEXT("InstallPath"), oni3GamePath, &oni3GamePathLength);
    StringCchCopy(oni3GameExePath, MAX_PATH, oni3GamePath);
    PathCchAppend(oni3GameExePath, MAX_PATH, TEXT(R"(\oni3.exe)"));

    checkGamePaths();
}


/// <summary>
/// 
/// </summary>
/// <returns></returns>
BOOL OriLauncher::checkGamePaths()
{
    if (!PathFileExists(oni3GamePath) || !PathFileExists(oni3GameExePath) || !PathIsDirectory(oni3GamePath) || PathIsDirectory(oni3GameExePath))
        return FALSE;
    return TRUE;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
BOOL OriLauncher::pseudoWinMain(HWND hWnd)
{
    oni3GameExeFileHandler = CreateFile(oni3GameExePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);

    this->hWnd = hWnd;
    if (oni3GameExeFileHandler == INVALID_HANDLE_VALUE) {
        showWinApiErrorMB(hWnd, MB_OK, TEXT("Error opening game executable"));
        return FALSE;
    }

    oni3ProcessCreated = CreateProcess(oni3GameExePath, NULL, NULL, NULL, FALSE, DETACHED_PROCESS, NULL, oni3GamePath, &oni3GameSI, &oni3GamePI);
    //WaitForSingleObject(oni3GamePI.hProcess, INFINITE);

    return TRUE;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
BOOL OriLauncher::resetGlobalHKEY()
{
    if (hKey && hKey != HKEY_CLASSES_ROOT && hKey != HKEY_CURRENT_CONFIG && hKey != HKEY_CURRENT_USER && hKey != HKEY_LOCAL_MACHINE && hKey != HKEY_USERS)
        RegCloseKey(hKey);

    hKey = 0;
    return FALSE;
}

/// <summary>
/// 
/// </summary>
/// <param name="hKey"></param>
/// <returns></returns>
BOOL OriLauncher::setCurrentHKEY(HKEY hKey)
{
    if (hKey)
        resetGlobalHKEY();

    hKey = (hKey) ? hKey : 0;
    return((BOOL)hKey);
}

/// <summary>
/// 
/// </summary>
/// <param name="lpValue"></param>
/// <param name="lpData"></param>
/// <param name="lpcbData"></param>
/// <returns></returns>
BOOL OriLauncher::getRegKeyValue(LPCTSTR lpValue, LPTSTR lpData, LPDWORD lpcbData)
{
    BOOL result = FALSE;

    setCurrentHKEY(HKEY_LOCAL_MACHINE);
    if (!lookForRegKey(TEXT(R"(SOFTWARE\CAPCOM\ONIMUSHA3 PC\1.00.000)")))
        return FALSE;
    if (hKey && lpValue && lpData)
        result = RegGetValue(hKey, NULL, lpValue, RRF_RT_REG_SZ, NULL, lpData, lpcbData) == ERROR_SUCCESS;
    setCurrentHKEY(0);

    return result;
}

/// <summary>
/// 
/// </summary>
/// <param name="lpSubKey"></param>
/// <returns></returns>
BOOL OriLauncher::lookForRegKey(LPCTSTR lpSubKey)
{
    HKEY phkResult = 0;

    if (RegOpenKeyEx(hKey, lpSubKey, 0, KEY_READ | KEY_WOW64_32KEY, &phkResult) != ERROR_SUCCESS)
        return FALSE;

    resetGlobalHKEY();
    hKey = phkResult;

    return TRUE;
}

/// <summary>
/// 
/// </summary>
OriLauncher::~OriLauncher()
{
    if (oni3GamePI.hProcess != NULL) {
        if (!CloseHandle(oni3GamePI.hProcess))
            showWinApiErrorMB(NULL, MB_OK, TEXT("CloseHandle ProcessInfo hProcess Error"));
        else
            oni3GamePI.hProcess = NULL;
    }
    if (oni3GamePI.hThread != NULL) {
        if (!CloseHandle(oni3GamePI.hThread))
            showWinApiErrorMB(NULL, MB_OK, TEXT("CloseHandle ProcessInfo hThread Error"));
        else
            oni3GamePI.hThread = NULL;
    }
    if (oni3GameExeFileHandler != NULL) {
        if (!CloseHandle(oni3GameExeFileHandler))
            showWinApiErrorMB(NULL, MB_OK, TEXT("CloseHandle Game Exe File Error"));
        else
            oni3GameExeFileHandler = NULL;
    }
}
