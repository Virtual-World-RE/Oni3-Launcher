#pragma once
#include "pch.h"

#define ERR_FMT_MSG_FLAGS FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS

/// <summary>
///   This will open a MessageBox containing the last Win API error.
/// </summary>
/// <remarks>
///   <para>Uses Win API <see cref="GetLastError"/> function.</para>
///   <para>This function has to be executed if the previous function returned an error.</para>
///   <para>This is only compatible with the Windows API functions.</para>
/// </remarks>
/// <param name="messageBoxHWnd">(optionnal) MessageBox parent window (default: null)</param>
/// <param name="messageBoxTitle">(optionnal) MessageBox title bar name (default: Error)</param>
/// <returns>
///   <para>0 if successful.</para>
///   <para>1 if no error is detected.</para>
///   <para>2 if an error occured executing FormatMessage.</para>
///   <para>3 if an error occured executing MessageBox.</para>
///   <para>4 if an error occured freeing message buffer using LocalFree.</para>
/// </returns>
INT showWinApiErrorMB(HWND hWnd = NULL, LPCTSTR messageBoxTitle = TEXT("Error"));
