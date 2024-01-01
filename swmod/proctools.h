#pragma once
#include "vector"
#include <Windows.h>
#include <TlHelp32.h>

DWORD getProcID(wchar_t* exeName);

MODULEENTRY32 getModule(DWORD procID, wchar_t* moduleName);

uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets);
