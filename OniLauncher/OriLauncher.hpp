/*
 ** This file is part of the Onimusha 3:RE Project !
 **
 ** You should have received a copy of the LICENSE along with this program.
 **
 ** Author: Gabriel GRONDIN (GGLinnk) <gglinnk@protonmail.com>
 ** Contributors (See CONTRIBUTION.md):
 */

#pragma once
#include "pch.h"
#include "error.h"

class OriLauncher
{
private:
    // Default variables
    HKEY    hKey =                      NULL;
    HWND    hWnd =                      NULL;
    BOOL    oni3ProcessCreated =        FALSE;
    DWORD   oni3GamePathLength =        0UL;
    HANDLE  oni3GameExeFileHandler =    NULL;
                                        
    STARTUPINFO         oni3GameSI;
    PROCESS_INFORMATION oni3GamePI;

    TCHAR   oni3GameExePath[MAX_PATH];
    TCHAR   oni3GamePath[MAX_PATH];

    BOOL    resetGlobalHKEY();
    BOOL    setCurrentHKEY(HKEY hKeyId);
    BOOL    getRegKeyValue(LPCTSTR lpValue, LPTSTR lpData, LPDWORD lpcbData);
    BOOL    lookForRegKey(LPCTSTR lpSubKey);
    BOOL    checkGamePaths();
public:
            OriLauncher();
    BOOL    pseudoWinMain(HWND hWnd);
    BOOL    checkError();
            ~OriLauncher();
};
