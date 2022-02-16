/*
 ** This file is part of the Onimusha 3:RE Project !
 **
 ** You should have received a copy of the LICENSE along with this program.
 **
 ** Author: Xavier Monin (xaviermonin)
 ** Edited by: Gabriel GRONDIN (GGLinnk) <gglinnk@protonmail.com>
 ** Contributors (See CONTRIBUTION.md):
 **
 ** Source: https://github.com/xaviermonin/Onimusha3Patch/blob/master/src/OniInjector/Injector.h
 */

#include "Injector.hpp"

PROCESS_INFORMATION Injector::RunProcess()
{
    TCHAR fullExePath[MAX_PATH] = { 0 };
    STARTUPINFO startupInfo = { 0 };
    PROCESS_INFORMATION processInfo = { 0 };

    SecureZeroMemory(&startupInfo, sizeof(STARTUPINFO));
    SecureZeroMemory(&processInfo, sizeof(PROCESS_INFORMATION));
    startupInfo.cb = sizeof(STARTUPINFO);

    StringCchCopy(fullExePath, MAX_PATH, path);
    PathCchAppend(fullExePath, MAX_PATH, exeName);

    if (!CreateProcess(fullExePath, NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, path, &startupInfo, &processInfo)) {
        showWinApiErrorMB();
        return { 0 };
    }

    return processInfo;
}

bool Injector::InjectDll(HANDLE hProc)
{
    SIZE_T dllNameSize = strlen(dllName) + 1;
    HMODULE moduleHandle = NULL;
    LPVOID loadLibraryAddr = NULL;
    LPVOID allocAddr = NULL;
    BOOL processMemmoryWrited = FALSE;
    HANDLE hRemoteThread = NULL;

    moduleHandle = LoadLibraryA("kernel32.dll");
    if (moduleHandle == NULL) {
        showWinApiErrorMB();
        return false;
    }

    loadLibraryAddr = (LPVOID)GetProcAddress(moduleHandle, "LoadLibraryA");
    if (loadLibraryAddr == NULL) {
        showWinApiErrorMB();
        return false;
    }

    allocAddr = (LPVOID)VirtualAllocEx(hProc, NULL, dllNameSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    if (allocAddr == NULL) {
        showWinApiErrorMB();
        return false;
    }

    processMemmoryWrited = WriteProcessMemory(hProc, allocAddr, dllName, dllNameSize, nullptr);
    if (processMemmoryWrited == FALSE) {
        showWinApiErrorMB();
        return false;
    }

    hRemoteThread = CreateRemoteThread(hProc, NULL, 0, (LPTHREAD_START_ROUTINE)loadLibraryAddr, allocAddr, 0, NULL);
    if (hRemoteThread == NULL) {
        showWinApiErrorMB();
        return false;
    }

    WaitForSingleObject(hRemoteThread, INFINITE);

    return true;
}

Injector::Injector(LPCTSTR path, LPCTSTR exeName, LPCSTR dllName)
{
    this->path = path;
    this->exeName = exeName;
    this->dllName = dllName;
}

bool Injector::RunAndInject()
{
    bool status = true;
    PROCESS_INFORMATION procInfo = RunProcess();

    if (!(InjectDll(procInfo.hProcess) && ResumeThread(procInfo.hThread) != (DWORD)-1))
        status = false;
    if (!CloseHandle(procInfo.hProcess))
        status = false;
    if (!CloseHandle(procInfo.hThread))
        status = false;

    return status;
}