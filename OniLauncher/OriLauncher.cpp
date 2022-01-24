/*
 ** This file is part of the Onimusha 3:RE Project !
 **
 ** You should have received a copy of the LICENSE along with this program.
 **
 ** Author: Gabriel GRONDIN (GGLinnk) <gglinnk@protonmail.com>
 ** Contributors (See CONTRIBUTION.md):
 */

#include "OriLauncher.h"

OriLauncher::OriLauncher(HWND originHWnd)
{
    SecureZeroMemory(&oni3GameSI, sizeof(STARTUPINFO));
    SecureZeroMemory(&oni3GamePI, sizeof(PROCESS_INFORMATION));
    SecureZeroMemory(oni3GamePath, sizeof(TCHAR) * MAX_PATH);
    SecureZeroMemory(oni3GameExePath, sizeof(TCHAR) * MAX_PATH);

    oni3GameSI.cb = sizeof(STARTUPINFO);
    hWnd = originHWnd;
    getRegKeyValue(TEXT("InstallPath"), oni3GamePath, &oni3GamePathLength);
    StringCchCopy(oni3GameExePath, MAX_PATH, oni3GamePath);
    StringCchCat(oni3GameExePath, MAX_PATH, TEXT(R"(\oni3.exe)"));
}

/// <summary>
/// 
/// </summary>
/// <returns>
///  BOOLEAN: TRUE if an error is detected.
/// </returns>
BOOL OriLauncher::checkError()
{
    BOOL status = FALSE;
    checkGamePath();
    return status;
}

BOOL OriLauncher::checkGamePath()
{
    return TRUE;
}

BOOL OriLauncher::pseudoWinMain()
{
    oni3GameExeFileHandler = CreateFile(oni3GameExePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);

    if (oni3GameExeFileHandler == INVALID_HANDLE_VALUE) {
        TCHAR sbuffer[256];
        StringCchPrintf(sbuffer, 256, TEXT("Cannot Find the Game [%s]"), oni3GameExePath);
        MessageBox(NULL, sbuffer, TEXT("Error"), MB_ICONERROR);
    } else {
        oni3ProcessCreated = CreateProcess(oni3GameExePath, NULL, NULL, NULL, FALSE, DETACHED_PROCESS, NULL, oni3GamePath, &oni3GameSI, &oni3GamePI);
        //WaitForSingleObject(oni3GamePI.hProcess, INFINITE);
    }

    return 0;
}

BOOL OriLauncher::resetGlobalHKEY()
{
    if (hKey && hKey != HKEY_CLASSES_ROOT && hKey != HKEY_CURRENT_CONFIG && hKey != HKEY_CURRENT_USER && hKey != HKEY_LOCAL_MACHINE && hKey != HKEY_USERS)
        RegCloseKey(hKey);

    hKey = 0;
    return FALSE;
}

BOOL OriLauncher::setCurrentHKEY(HKEY hKey)
{
    if (hKey)
        resetGlobalHKEY();

    hKey = (hKey) ? hKey : 0;
    return((BOOL)hKey);
}

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

BOOL OriLauncher::lookForRegKey(LPCTSTR lpSubKey)
{
    HKEY phkResult = 0;

    if (RegOpenKeyEx(hKey, lpSubKey, 0, KEY_READ | KEY_WOW64_32KEY, &phkResult) != ERROR_SUCCESS)
        return FALSE;

    resetGlobalHKEY();
    hKey = phkResult;

    return TRUE;
}

OriLauncher::~OriLauncher()
{
    int status = 0;
    BOOL handleError = FALSE;

    do {
        if (oni3GamePI.hProcess != NULL) {
            handleError = (CloseHandle(oni3GamePI.hProcess) == TRUE) ? TRUE : handleError;
            oni3GamePI.hProcess = NULL;
        }
        if (oni3GamePI.hThread != NULL) {
            handleError = (CloseHandle(oni3GamePI.hThread) == TRUE) ? TRUE : handleError;
            oni3GamePI.hThread = NULL;
        }
        if (oni3GameExeFileHandler != NULL) {
            handleError = (CloseHandle(oni3GameExeFileHandler) == TRUE) ? TRUE : handleError;
            oni3GameExeFileHandler = NULL;
        }
        if (handleError)
            status = showWinApiError(hWnd, MB_RETRYCANCEL, TEXT("Error closing handle"));
    } while (status == IDRETRY);

    if (status == IDCANCEL)
        MessageBox(hWnd, TEXT("An error occured while closing handles"), TEXT("Error"), MB_ICONERROR | MB_OK);
}
