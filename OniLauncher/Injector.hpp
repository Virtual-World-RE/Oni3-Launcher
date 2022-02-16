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

#pragma once

#include "pch.h"
#include "error.hpp"

class Injector
{
private:
    LPCTSTR path;
    LPCTSTR exeName;
    LPCSTR dllName;

private:
    PROCESS_INFORMATION RunProcess();
    bool InjectDll(HANDLE hProc);

public:
    Injector(LPCTSTR path, LPCTSTR exeName, LPCSTR dllName);
    bool RunAndInject();
};

