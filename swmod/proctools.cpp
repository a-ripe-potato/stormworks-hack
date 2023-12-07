#include <iostream>
#include "proctools.h"

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