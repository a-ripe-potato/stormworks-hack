#include "antidbg.h"
#include "build_flags.h"





bool checkDebugger() { //returns true if a debugger is present.
    //corrupt breakpoints

#ifdef INTEGRITY_CHECK
    PVOID pRetAddress = _ReturnAddress();
    if (*(PBYTE)pRetAddress == 0xCC) // int 3
    {
        DWORD dwOldProtect;
        if (VirtualProtect(pRetAddress, 1, PAGE_EXECUTE_READWRITE, &dwOldProtect))
        {
            *(PBYTE)pRetAddress = 0x90; // nop
            VirtualProtect(pRetAddress, 1, dwOldProtect, &dwOldProtect);
        }
    }
#endif

    //debugger check
    bool bDebuggerCheckA = CheckRemoteDebuggerPresent();
    //printf("check a\n");
    bool bDebuggerCheckB = IsDebuggerPresent();
    //printf("check b\n");
    bool bDebuggerCheckC = CheckHeap();
    //printf("check c\n");
    bool bDebuggerCheckD = CheckDebugRegisters();
    //printf("check d\n");
    bool bDebuggerCheckE = CheckAPIPatch();
    //printf("check e\n");
    bool bDebuggerCheckF = CheckRaiseCTRLCException();
    bool bDebuggerCheckG = CheckDbgPrintException();
    bool bDebuggerCheckH = CheckProcessFileName();
    bool bDebuggerCheckI = CheckCloseHandleException();
    //printf("check f\n");
    //printf("\nIntegrity check complete: A:%b,B:%b,C:%b,D:%b,E:%b,F:%b,G:%b\n",bDebuggerCheckA, bDebuggerCheckB, bDebuggerCheckC, bDebuggerCheckD, bDebuggerCheckE, bDebuggerCheckF, bDebuggerCheckG);
    return bDebuggerCheckA || bDebuggerCheckB || bDebuggerCheckC || bDebuggerCheckD || bDebuggerCheckE || bDebuggerCheckF || bDebuggerCheckG || bDebuggerCheckH || bDebuggerCheckI;
}

inline bool CheckRemoteDebuggerPresent()
{
    HANDLE hProcess = INVALID_HANDLE_VALUE;
    BOOL found = FALSE;

    hProcess = GetCurrentProcess();
    CheckRemoteDebuggerPresent(hProcess, &found);

    return found;
}

inline bool CheckHeap()
{
#ifdef INTEGRITY_CHECK
    PVOID pRetAddress = _ReturnAddress();
    if (*(PBYTE)pRetAddress == 0xCC) // int 3
    {
        DWORD dwOldProtect;
        if (VirtualProtect(pRetAddress, 1, PAGE_EXECUTE_READWRITE, &dwOldProtect))
        {
            *(PBYTE)pRetAddress = 0x90; // nop
            VirtualProtect(pRetAddress, 1, dwOldProtect, &dwOldProtect);
        }
    }
#endif

    PROCESS_HEAP_ENTRY HeapEntry = { 0 };
    do
    {
        if (!HeapWalk(GetProcessHeap(), &HeapEntry))
            return false;
    } while (HeapEntry.wFlags != PROCESS_HEAP_ENTRY_BUSY);

    PVOID pOverlapped = (PBYTE)HeapEntry.lpData + HeapEntry.cbData;
    return ((DWORD)(*(PDWORD)pOverlapped) == 0xABABABAB);
}

inline bool CheckDebugRegisters()
{
#ifdef INTEGRITY_CHECK
    PVOID pRetAddress = _ReturnAddress();
    if (*(PBYTE)pRetAddress == 0xCC) // int 3
    {
        DWORD dwOldProtect;
        if (VirtualProtect(pRetAddress, 1, PAGE_EXECUTE_READWRITE, &dwOldProtect))
        {
            *(PBYTE)pRetAddress = 0x90; // nop
            VirtualProtect(pRetAddress, 1, dwOldProtect, &dwOldProtect);
        }
    }
#endif
    CONTEXT ctx;
    ZeroMemory(&ctx, sizeof(CONTEXT));
    ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;

    if (!GetThreadContext(GetCurrentThread(), &ctx))
        return false;

    return (ctx.Dr0 != 0x00) || (ctx.Dr1 != 0x00) || (ctx.Dr2 != 0x00) || (ctx.Dr3 != 0x00) || (ctx.Dr6 != 0x00) || (ctx.Dr7 != 0x00);
}

inline bool CheckAPIPatch()
{
#ifdef INTEGRITY_CHECK
    PVOID pRetAddress = _ReturnAddress();
    if (*(PBYTE)pRetAddress == 0xCC) // int 3
    {
        DWORD dwOldProtect;
        if (VirtualProtect(pRetAddress, 1, PAGE_EXECUTE_READWRITE, &dwOldProtect))
        {
            *(PBYTE)pRetAddress = 0x90; // nop
            VirtualProtect(pRetAddress, 1, dwOldProtect, &dwOldProtect);
        }
    }
#endif
    HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");
    if (!hKernel32)
        return false;

    FARPROC pIsDebuggerPresent = GetProcAddress(hKernel32, "IsDebuggerPresent");
    if (!pIsDebuggerPresent)
        return false;

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (INVALID_HANDLE_VALUE == hSnapshot)
        return false;

    PROCESSENTRY32W ProcessEntry;
    ProcessEntry.dwSize = sizeof(PROCESSENTRY32W);

    if (!Process32FirstW(hSnapshot, &ProcessEntry))
        return false;

    bool bDebuggerPresent = false;
    HANDLE hProcess = NULL;
    DWORD dwFuncBytes = 0;
    const DWORD dwCurrentPID = GetCurrentProcessId();
    do
    {
        __try
        {
            if (dwCurrentPID == ProcessEntry.th32ProcessID)
                //continue;
                __leave;
            hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessEntry.th32ProcessID);
            if (NULL == hProcess)
                //continue;
                __leave;

            if (!ReadProcessMemory(hProcess, pIsDebuggerPresent, &dwFuncBytes, sizeof(DWORD), NULL))
                //continue;
                __leave;

            if (dwFuncBytes != *(PDWORD)pIsDebuggerPresent)
            {
                bDebuggerPresent = true;
                //break;
                __leave;
            }
        }
        __finally
        {
            if (hProcess)
                CloseHandle(hProcess);
        }
    } while (Process32NextW(hSnapshot, &ProcessEntry));

    if (hSnapshot)
        CloseHandle(hSnapshot);
    return bDebuggerPresent;
}

inline bool CheckRaiseCTRLCException()
{
#ifdef INTEGRITY_CHECK
    PVOID pRetAddress = _ReturnAddress();
    if (*(PBYTE)pRetAddress == 0xCC) // int 3
    {
        DWORD dwOldProtect;
        if (VirtualProtect(pRetAddress, 1, PAGE_EXECUTE_READWRITE, &dwOldProtect))
        {
            *(PBYTE)pRetAddress = 0x90; // nop
            VirtualProtect(pRetAddress, 1, dwOldProtect, &dwOldProtect);
        }
    }
#endif
    __try
    {
        RaiseException(DBG_CONTROL_C, 0, 0, NULL);
        return true;
    }
    __except (DBG_CONTROL_C == GetExceptionCode()
        ? EXCEPTION_EXECUTE_HANDLER
        : EXCEPTION_CONTINUE_SEARCH)
    {
        return false;
    }
}

inline bool CheckDbgPrintException()
{
#ifdef INTEGRITY_CHECK
    PVOID pRetAddress = _ReturnAddress();
    if (*(PBYTE)pRetAddress == 0xCC) // int 3
    {
        DWORD dwOldProtect;
        if (VirtualProtect(pRetAddress, 1, PAGE_EXECUTE_READWRITE, &dwOldProtect))
        {
            *(PBYTE)pRetAddress = 0x90; // nop
            VirtualProtect(pRetAddress, 1, dwOldProtect, &dwOldProtect);
        }
    }
#endif
    __try
    {
        RaiseException(DBG_PRINTEXCEPTION_C, 0, 0, 0);
    }
    __except (GetExceptionCode() == DBG_PRINTEXCEPTION_C)
    {
        return false;
    }

    return true;
}

inline bool CheckProcessFileName()
{
#ifdef INTEGRITY_CHECK
    PVOID pRetAddress = _ReturnAddress();
    if (*(PBYTE)pRetAddress == 0xCC) // int 3
    {
        DWORD dwOldProtect;
        if (VirtualProtect(pRetAddress, 1, PAGE_EXECUTE_READWRITE, &dwOldProtect))
        {
            *(PBYTE)pRetAddress = 0x90; // nop
            VirtualProtect(pRetAddress, 1, dwOldProtect, &dwOldProtect);
        }
    }
#endif
    bool found = false;
    // detect debugger by process file (for example: ollydbg.exe)
    #ifdef IGNORE_CHEAT_ENGINE
    const wchar_t* debuggersFilename[7] = {
        L"ollydbg.exe",
        L"ida.exe",
        L"ida64.exe",
        L"radare2.exe",
        L"x64dbg.exe",
        L"EngHost.exe",
        L"DbgX.Shell.exe"
    };
    #else
    const wchar_t* debuggersFilename[9] = {
        L"cheatengine-x86_64.exe",
        L"ollydbg.exe",
        L"ida.exe",
        L"ida64.exe",
        L"radare2.exe",
        L"x64dbg.exe",
        L"cheatengine-x86_64-SSE4-AVX2.exe",
        L"EngHost.exe",
        L"DbgX.Shell.exe"
};
    #endif // DEBUG

    

    wchar_t* processName;
    PROCESSENTRY32W processInformation{ sizeof(PROCESSENTRY32W) };
    HANDLE processList;

    processList = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    processInformation = { sizeof(PROCESSENTRY32W) };
    if ((Process32FirstW(processList, &processInformation)))
    {
        do
        {
            for (const wchar_t* debugger : debuggersFilename)
            {
                processName = processInformation.szExeFile;
                if (_wcsicmp(debugger, processName) == 0) {
                    found = true;
                }
            }
        } while (Process32NextW(processList, &processInformation));
    }
    CloseHandle(processList);
    return found;
}

inline bool CheckCloseHandleException()
{
#ifdef INTEGRITY_CHECK
    PVOID pRetAddress = _ReturnAddress();
    if (*(PBYTE)pRetAddress == 0xCC) // int 3
    {
        DWORD dwOldProtect;
        if (VirtualProtect(pRetAddress, 1, PAGE_EXECUTE_READWRITE, &dwOldProtect))
        {
            *(PBYTE)pRetAddress = 0x90; // nop
            VirtualProtect(pRetAddress, 1, dwOldProtect, &dwOldProtect);
        }
    }
#endif
    HANDLE hInvalid = (HANDLE)0xBEEF; // an invalid handle
    DWORD found = FALSE;

    __try
    {
        CloseHandle(hInvalid);
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        found = TRUE;
    }

    return found;
}

void adbg_NtSetInformationThread()
{
#ifdef INTEGRITY_CHECK
    PVOID pRetAddress = _ReturnAddress();
    if (*(PBYTE)pRetAddress == 0xCC) // int 3
    {
        DWORD dwOldProtect;
        if (VirtualProtect(pRetAddress, 1, PAGE_EXECUTE_READWRITE, &dwOldProtect))
        {
            *(PBYTE)pRetAddress = 0x90; // nop
            VirtualProtect(pRetAddress, 1, dwOldProtect, &dwOldProtect);
        }
    }
#endif
    THREAD_INFORMATION_CLASS ThreadHideFromDebugger = (THREAD_INFORMATION_CLASS)0x11;

    // Get a handle to ntdll.dll so we can import NtSetInformationThread
    HMODULE hNtdll = LoadLibraryW(L"ntdll.dll");
    if (hNtdll == INVALID_HANDLE_VALUE || hNtdll == NULL)
    {
        return;
    }

    // Dynamically acquire the addres of NtSetInformationThread and NtQueryInformationThread
    _NtSetInformationThread NtSetInformationThread = NULL;
    NtSetInformationThread = (_NtSetInformationThread)GetProcAddress(hNtdll, "NtSetInformationThread");

    if (NtSetInformationThread == NULL)
    {
        return;
    }

    // There is nothing to check here after this call.
    NtSetInformationThread(GetCurrentThread(), ThreadHideFromDebugger, 0, 0);
}


