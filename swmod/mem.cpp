#include "mem.h"
#include "string"

//internal patch
void Patch(void* dst, void* src, unsigned int size)
{
	DWORD trash;
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	memcpy(dst, src, size);
	VirtualProtect(dst, size, oldprotect, &trash);
}

//external
void PatchEX(HANDLE hProcess, void* dst, void* src, unsigned int size)
{
	DWORD trash;
	DWORD oldprotect;
	VirtualProtectEx(hProcess, dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	if (!WriteProcessMemory(hProcess, dst, src, size, NULL)) {
		if (GetLastError() != 0x3E6) {
			printf("Failed to write memory 0x%X\n", GetLastError());
		}
		
	}

	VirtualProtectEx(hProcess, dst, size, oldprotect, &trash);
}

//internal nop
void Nop(HANDLE hProcess, void* dst, unsigned int size)
{
	DWORD trash;
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	memset(dst, 0x90, size);
	VirtualProtect(dst, size, oldprotect, &trash);
}

//external
void NopEX(HANDLE hProcess, void* dst, unsigned int size)
{
	DWORD trash;
	DWORD oldprotect;
	byte* nopArray = new byte[size];
	memset(nopArray, 0x90, size);
	VirtualProtectEx(hProcess, dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	//std::cout << "page protection was:" << oldprotect << "\n";
	WriteProcessMemory(hProcess, dst, nopArray, size, NULL);
	VirtualProtectEx(hProcess, dst, size, oldprotect, &trash);
	delete[] nopArray;
}

void* ProtectedRead(HANDLE hProcess, void* dst, unsigned int size)
{
	if (dst)
	{
		DWORD trash;
		DWORD oldprotect;
		void* buffer;
		VirtualProtectEx(hProcess, dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
		ReadProcessMemory(hProcess, dst, &buffer, size, NULL);
		VirtualProtectEx(hProcess, dst, size, oldprotect, &trash);
		return buffer;
	}
	return nullptr;
}