#pragma once
#pragma intrinsic(_ReturnAddress)
#include <windows.h> 
#include <iostream>
#include <TlHelp32.h>
#include "WinBase.h"




typedef struct timeKeeper {
    uint64_t timeUpperA;
    uint64_t timeLowerA;
    uint64_t timeUpperB;
    uint64_t timeLowerB;
} TimeKeeper;


typedef NTSTATUS(__stdcall* _NtQueryInformationProcess)(_In_ HANDLE, _In_  unsigned int, _Out_ PVOID, _In_ ULONG, _Out_ PULONG);
typedef NTSTATUS(__stdcall* _NtSetInformationThread)(_In_ HANDLE, _In_ THREAD_INFORMATION_CLASS, _In_ PVOID, _In_ ULONG);


bool checkDebugger();


inline bool CheckRemoteDebuggerPresent();

inline bool CheckHeap();

inline bool CheckDebugRegisters();

inline bool CheckAPIPatch();

inline bool CheckRaiseCTRLCException();

inline bool CheckDbgPrintException();

inline bool CheckProcessFileName();

inline bool CheckCloseHandleException();

void adbg_NtSetInformationThread();

