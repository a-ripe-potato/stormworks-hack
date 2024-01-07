#include <iostream>
#include "proctools.h"
#include <windows.h>
#pragma comment(lib, "advapi32.lib")


DWORD getProcID(wchar_t * exeName)
{
	PROCESSENTRY32 procEntry{ 0 };
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (!hSnapshot) 
	{
		std::cout << "Unable to take a process snapshot\n";
		return 0;
	}
	procEntry.dwSize = sizeof(procEntry);
	if (!Process32First(hSnapshot, &procEntry)) {
		std::cout << "Unable to process first snapshot\n";
		return 0;
	}

	do
	{
		if (!wcscmp(procEntry.szExeFile, exeName)) 
		{
			CloseHandle(hSnapshot);
			return procEntry.th32ProcessID;
		}
	} while (Process32Next(hSnapshot, &procEntry));

	CloseHandle(hSnapshot);
	return 0;

}

MODULEENTRY32 getModule(DWORD procID, wchar_t* moduleName)
{
	MODULEENTRY32 modEntry = { 0 };

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE, procID);

	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		modEntry.dwSize = sizeof(MODULEENTRY32);
		if (Module32First(hSnapshot, &modEntry))
		{
			do
			{
				if (!wcscmp(modEntry.szModule, moduleName))
				{
					break;
				}
			} while (Module32Next(hSnapshot, &modEntry));
		}
		CloseHandle(hSnapshot);
	}
	return modEntry;
}

uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		if (!ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0) && GetLastError() != 0x12B) {
			if (GetLastError() == 0x3E6) {
				printf("Failed to read memory. Invalid pointer.\n");
			}
			else {
				printf("Failed in FindDMAAddy() 0x%X\n", GetLastError());
			}
		}
		addr += offsets[i];
	}
	return addr;
}



