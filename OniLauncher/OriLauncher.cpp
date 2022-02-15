/*
 ** This file is part of the Onimusha 3:RE Project !
 **
 ** You should have received a copy of the LICENSE along with this program.
 **
 ** Author: Gabriel GRONDIN (GGLinnk) <gglinnk@protonmail.com>
 ** Contributors (See CONTRIBUTION.md):
 */

#include "OriLauncher.hpp"

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

BOOL OriLauncher::checkGamePaths()
{
    if (!PathFileExists(oni3GamePath) || !PathFileExists(oni3GameExePath) || !PathIsDirectory(oni3GamePath) || PathIsDirectory(oni3GameExePath))
        return FALSE;
    return TRUE;
}

BOOL OriLauncher::resetGlobalHKEY()
{
    if (currentHKey && currentHKey != HKEY_CLASSES_ROOT && currentHKey != HKEY_CURRENT_CONFIG && currentHKey != HKEY_CURRENT_USER && currentHKey != HKEY_LOCAL_MACHINE && currentHKey != HKEY_USERS)
        RegCloseKey(currentHKey);

    currentHKey = 0;
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
    if (currentHKey && lpValue && lpData)
        result = RegGetValue(currentHKey, NULL, lpValue, RRF_RT_REG_SZ, NULL, lpData, lpcbData) == ERROR_SUCCESS;
    setCurrentHKEY(0);

    return result;
}

BOOL OriLauncher::lookForRegKey(LPCTSTR lpSubKey)
{
    HKEY phkResult = 0;

    if (RegOpenKeyEx(currentHKey, lpSubKey, 0, KEY_READ | KEY_WOW64_32KEY, &phkResult) != ERROR_SUCCESS)
        return FALSE;

    resetGlobalHKEY();
    currentHKey = phkResult;

    return TRUE;
}

OriLauncher::~OriLauncher()
{
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
}
