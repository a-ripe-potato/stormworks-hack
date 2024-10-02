#pragma once
#include <iostream>
#include "vector"
#include <Windows.h>
#include <TlHelp32.h>
#pragma comment(lib, "advapi32.lib")

DWORD getProcID(wchar_t* exeName);

MODULEENTRY32 getModule(DWORD procID, wchar_t* moduleName);

uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets);