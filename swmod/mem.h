#pragma once
#include <Windows.h>


//internal patch
void Patch(void* dst, void* src, unsigned int size);

//external
void PatchEX(HANDLE hProcess, void* dst, void* src, unsigned int size);

//internal nop
void Nop(HANDLE hProcess, void* dst, unsigned int size);

//external
void NopEX(HANDLE hProcess, void* dst, unsigned int size);

void* ProtectedRead(HANDLE hprocess, void* dst, unsigned int size);

