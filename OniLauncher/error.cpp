#include "pch.h"
#include "error.h"

INT showWinApiErrorMB(HWND hWnd, LPCTSTR messageBoxTitle)
{
    DWORD lastError = GetLastError();
    LPTSTR messageBuffer = NULL;

    if (lastError == ERROR_SUCCESS)
        return 1;
    if (!FormatMessage(ERR_FMT_MSG_FLAGS, NULL, lastError, 0, (LPTSTR)&messageBuffer, 0, NULL))
        return 2;
    if (!MessageBox(hWnd, messageBuffer, messageBoxTitle, MB_OK | MB_ICONERROR))
        return 3;
    if (LocalFree(messageBuffer))
        return 4;

    return 0;
}
