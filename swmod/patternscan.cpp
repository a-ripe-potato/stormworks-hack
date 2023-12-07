#include "patternscan.h"
#include <iostream>
#include "mem.h"
#include "strings.h"
char* MINPLROBJADDR = (char*)0x0000001000000000;
char* MAXPLROBJADDR = (char*)0x0000040000000000;


void * PatternScan(char* base, size_t size, char* pattern, char* mask)
{
	size_t patternLength = strlen(mask);
	for (unsigned int i = 0; i < size - patternLength; i++)
	{
		bool f = true;
			for (unsigned int j = 0; j < patternLength; j++)
			{
				if (mask[j] != '?' && pattern[j] != *(base + i + j))
				{
					f = false;
					break;
				}
			}
			if (f)
			{
				return (void*)(base + i);
			}
	}
	return nullptr;
}


void * PatternScanEX(HANDLE hProcess, char* begin, char* end, char* pattern, char* mask)
{
	char* currentChunk = begin;
	SIZE_T bytesRead;
	//void* internalAddress;
	while (currentChunk < end)
	{
		char buffer[4096];
		DWORD oldprotect;
		VirtualProtectEx(hProcess, (void*)currentChunk, sizeof(buffer), PROCESS_ALL_ACCESS, &oldprotect);
		ReadProcessMemory(hProcess, (void*)currentChunk, &buffer, sizeof(buffer), &bytesRead);
		VirtualProtectEx(hProcess, (void*)currentChunk, sizeof(buffer), oldprotect, NULL);
		if (bytesRead == 0)
		{
			return nullptr;
		}
		void* internalAddress = PatternScan((char*)&buffer, bytesRead, pattern, mask);

		if (internalAddress != nullptr)
		{
			uintptr_t offsetFromBuffer = (uintptr_t)internalAddress - (uintptr_t)&buffer;
			return (void*)(currentChunk + offsetFromBuffer);
		}
		else
		{
			currentChunk = currentChunk + bytesRead;
		}
	}
	std::cout << failFindPatternGen;
	return nullptr;
}

void * PatternScanExModule(HANDLE hProcess,wchar_t * exeName, wchar_t* module, char* pattern, char* mask, char* patternName)
{
	DWORD procID = getProcID(exeName);
	MODULEENTRY32 modEntry = getModule(procID, module);
	char* begin = (char *)modEntry.modBaseAddr;
	char* end = (char*)begin + modEntry.modBaseSize;
	void* result = PatternScanEX(hProcess, begin, end, pattern, mask);
	if (result == NULL) {
		std::cout << failFindPatternArg << patternName << "\n";
	}
	else
	{
		std::cout << foundPattern << patternName << " (0x" << result << ")\n";
	}
	return result;
}

void* getPlayerObjectFromFlashlight(HANDLE hProcess)
{
	wchar_t* exeAndModule = (wchar_t*)L"stormworks64.exe";
	DWORD procID = getProcID(exeAndModule);
	MODULEENTRY32 modEntry = getModule(procID, exeAndModule);
	char* begin = (char*)modEntry.modBaseAddr;
	char* end = (char*)modEntry.modBaseSize;
	std::cout << "preparing to scan 0x" << (uintptr_t)begin << " to 0x" << (uintptr_t)end << "\n";
	void* result = pattenScanForFlashlight(hProcess, begin, end);
	return result;
}

void* pattenScanForFlashlight(HANDLE hProcess, char* begin, char* end)
{
	char* pattern = (char*)"\xFF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0F\x00\x00\x00\xE4\xEE\xC7\x42";
	char* mask = (char*)"xxxxxxxxxxxxxxxxxxxx";
	char* currentChunk = begin;
	SIZE_T bytesRead;
	//void* internalAddress;
	while (currentChunk < end)
	{
		char buffer[4096];
		DWORD oldprotect;
		VirtualProtectEx(hProcess, (void*)currentChunk, sizeof(buffer), PROCESS_ALL_ACCESS, &oldprotect);
		ReadProcessMemory(hProcess, (void*)currentChunk, &buffer, sizeof(buffer), &bytesRead);
		VirtualProtectEx(hProcess, (void*)currentChunk, sizeof(buffer), oldprotect, NULL);
		if (bytesRead == 0)
		{
			return nullptr;
		}
		void* internalAddress = PatternScan((char*)&buffer, bytesRead, pattern, mask);

		if (internalAddress != nullptr)
		{
			if ((DWORD)ProtectedRead(hProcess, (char*)internalAddress - 0x278, 4) == 4)
			{
				uintptr_t offsetFromBuffer = (uintptr_t)internalAddress - (uintptr_t)&buffer;
				return (char*)(currentChunk + offsetFromBuffer - 0x278);
			}
			else 
			{
				std::cout << "found pattern but ignored because no player object was found";
				currentChunk = currentChunk + bytesRead;
			}

		}
		else
		{
			currentChunk = currentChunk + bytesRead;
		}
	}
	std::cout << failFindPatternGen;
	return nullptr;
}
