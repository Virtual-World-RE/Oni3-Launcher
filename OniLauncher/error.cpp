#include "pch.h"
#include "error.h"

int showWinApiError(HWND hWnd = NULL, DWORD messageBoxFlags = MB_OK, LPCTSTR messageBoxTitle = TEXT("Error"))
{
    int status = -1;
    DWORD lastError = 0UL;
    LPTSTR messageBuffer = NULL;
    DWORD messageFlags = FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS;

    messageBoxFlags |= MB_ICONERROR;
    lastError = GetLastError();
    //if (lastError == ERROR_SUCCESS) return status;

    if (FormatMessage(messageFlags, NULL, lastError, 0, (LPTSTR)&messageBuffer, 0, NULL) == 0UL)
        showWinApiError(hWnd, MB_OK, TEXT("Error formating error message"));
    else
        status = MessageBox(hWnd, messageBuffer, messageBoxTitle, messageBoxFlags);

    if (LocalFree(messageBuffer) != NULL)
        showWinApiError(hWnd, MB_OK, TEXT("Error freeing message buffer memory"));

    return status;
}
