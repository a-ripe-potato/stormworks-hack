#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include "proctools.h"
#include "swmod.h"

void * PatternScan(char* base, size_t size, char* pattern, char* mask);

void * PatternScanEX(HANDLE hProcess, char* begin, char* end, char* pattern, char* mask);

void * PatternScanExModule(HANDLE hProcess, wchar_t* exeName, wchar_t* module, char* pattern, char* mask, char* patternName);

void* getPlayerObjectFromFlashlight(HANDLE hProcess);

void* pattenScanForFlashlight(HANDLE hProcess, char* begin, char* end);




