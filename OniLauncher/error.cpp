#include "pch.h"
#include "error.h"

/// <summary>
///   <para>This will open a MessageBox containing the last Win API error.</para>
/// </summary>
/// <remarks>
///   <para>Uses Win API <see cref="GetLastError"/> function.</para>
///   <para>This function has to be executed if the previous function returned an error.</para>
///   <para>This is only compatible with the Windows API functions.</para>
/// </remarks>
/// <param name="messageBoxHWnd">(optionnal) MessageBox parent window (default: null)</param>
/// <param name="messageBoxFlags">(optionnal) MessageBox flags (default: MB_OK)</param>
/// <param name="messageBoxTitle">(optionnal) MessageBox title bar name (default: Error)</param>
/// <returns>
///   <para>0 if succeed.</para>
///   <para>1 if an error occured.</para>
///   <para>2 if no error is detected.</para>
/// </returns>
INT showWinApiErrorMB(HWND hWnd, DWORD messageBoxFlags, LPCTSTR messageBoxTitle)
{
    INT result = 0;
    DWORD lastError = GetLastError();
    LPTSTR messageBuffer = NULL;

    messageBoxFlags |= MB_ICONERROR;

    if (lastError == ERROR_SUCCESS) return 2;

    if (FormatMessage(MESSAGE_FLAGS, NULL, lastError, 0, (LPTSTR)&messageBuffer, 0, NULL) == 0UL)
        result = showWinApiErrorMB(hWnd, MB_OK, TEXT("Error formating error message"));
    else if (MessageBox(hWnd, messageBuffer, messageBoxTitle, messageBoxFlags))
        result = 1;

    if (LocalFree(messageBuffer) != NULL)
        result = showWinApiErrorMB(hWnd, MB_OK, TEXT("Error freeing message buffer memory"));

    return result;
}
