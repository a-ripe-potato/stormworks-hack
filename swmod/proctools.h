#pragma once

#include <Windows.h>
#include <TlHelp32.h>

DWORD getProcID(wchar_t* exeName);

MODULEENTRY32 getModule(DWORD procID, wchar_t* moduleName);
