#include "mem.h"
#include "string"

//internal patch
void Patch(void* dst, void* src, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	memcpy(dst, src, size);
	VirtualProtect(dst, size, oldprotect, NULL);
}

//external
void PatchEX(HANDLE hProcess, void* dst, void* src, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtectEx(hProcess, dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	WriteProcessMemory(hProcess, dst, src, size, NULL);
	VirtualProtectEx(hProcess, dst, size, oldprotect, NULL);
}

//internal nop
void Nop(HANDLE hProcess, void* dst, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	memset(dst, 0x90, size);
	VirtualProtect(dst, size, oldprotect, NULL);
}

//external
void NopEX(HANDLE hProcess, void* dst, unsigned int size)
{
	DWORD oldprotect;
	byte* nopArray = new byte[size];
	memset(nopArray, 0x90, size);

	
	VirtualProtectEx(hProcess, dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	//std::cout << "page protection was:" << oldprotect << "\n";
	WriteProcessMemory(hProcess, dst, nopArray, size, NULL);
	VirtualProtectEx(hProcess, dst, size, oldprotect, NULL);
	delete[] nopArray;
}

void* ProtectedRead(HANDLE hProcess, void* dst, unsigned int size)
{
	if (dst)
	{
		DWORD oldprotect;
		void* buffer;
		VirtualProtectEx(hProcess, dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
		ReadProcessMemory(hProcess, dst, &buffer, size, NULL);
		VirtualProtectEx(hProcess, dst, size, oldprotect, NULL);
		return buffer;
	}
	return (void*)0xFFFFFFFF;
}

FLOAT* ProtectedFloatRead(HANDLE hProcess, void* dst, unsigned int size)
{
	if (dst) 
	{
		DWORD oldprotect;
		FLOAT* buffer;
		VirtualProtectEx(hProcess, dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
		ReadProcessMemory(hProcess, dst, &buffer, size, NULL);
		VirtualProtectEx(hProcess, dst, size, oldprotect, NULL);
		return buffer;
	}
	return (FLOAT*)0xFFFFFFFF;
}