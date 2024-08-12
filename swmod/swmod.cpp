#pragma once
#include "swmod.h"
#include "build_flags.h"



//#define INTEGRITY_CHECK
//#define EXPIRE
struct allowList al;
struct modList ml;
struct cmd_flags flags;
struct addresses addr; 
constexpr struct CommandAllowList cal;
MODULEENTRY32 Module;
HANDLE hProcess;
std::thread tAutoloadout;
bool bActionThread = false;
bool bKeepActive = false;
DWORD processID;
bool bPlrScanThread = false;
constexpr bool bIntegrityChecks = true;
constexpr int build_time = __DATE_TIME_UNIX__ + 21600;
constexpr int expire_build_time = build_time + 60 * 60 * 24 * 14;    //14 days
wchar_t* dllPath = (wchar_t*)L"C:\\swmodV2.dll";
constexpr int PS_IDLE_SCANS = 100;
//wchar_t* dllPath = (wchar_t*)L"C:\\Users\\joe\\source\\repos\\swmodV2\\x64\\Release\\swmodV2.dll";



BOOL WINAPI HandlerRoutine(
    _In_ DWORD dwCtrlType) {
    if (dwCtrlType == CTRL_CLOSE_EVENT || dwCtrlType == CTRL_C_EVENT) {
        cleanup();
        Sleep(500);
        return true;
    }
    if (dwCtrlType == CTRL_BREAK_EVENT || dwCtrlType == CTRL_LOGOFF_EVENT || dwCtrlType == CTRL_SHUTDOWN_EVENT) {
        cleanup();
        Sleep(500);
    }
    return false;
}


int main()
{
    
    SetConsoleTitleW(L"Silly Stormworks Mod");
    printf("Build ID: %d", build_time - 0x420);

    #ifdef INTEGRITY_CHECK
        printf("-S");
        if (CheckProcessFileName()) {
            printf("\nA fatal error has occured and the program must exit!\n");
            Sleep(5000);
            ExitProcess(-1);
        }
        adbg_NtSetInformationThread();
        std::thread tIntegrityThread(IntegrityThread);
        tIntegrityThread.detach();
    #endif
        printf("\n");
    #ifdef EXPIRE
        if (isExpired()) {
            printf("This build has expired!\n");
            Sleep(5000);
            ExitProcess(-1);
        }
        else{
            printf("This build has been flagged to expire.\n");
        }
    #endif
    MSG msg = { };
    processID = getProcID((wchar_t *)L"stormworks64.exe");
    if (processID != 0)
    {
        hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, processID);
        Module = getModule(processID, (wchar_t*)(L"stormworks64.exe"));
        printf("%smodule base addr 0x%p\n",prefix.c_str(), Module.modBaseAddr);
        getPlayerObjWhenAvailable();
        al.playerobj = verifyPlrObjAddress();
        if (!al.playerobj) {
            printf("%sFailed to locate player object, waiting for init.\n", prefix.c_str());
        }
        else {
            addr.PlrSlotAddr = (char*)addr.PlrObjAddr + 0x240;
        }
        addr.LockSettAddr = (BYTE*)Module.modBaseAddr + 0xBFAD12;
        addr.ConfigLockAddr = (BYTE*)addr.LockSettAddr - 0x28;
        addr.InfElecAddr = (BYTE*)addr.LockSettAddr + 0x1;
        addr.InfFuelAddr = (BYTE*)addr.LockSettAddr + 0x2;
        addr.DisableWeaponsAddr = (BYTE*)addr.LockSettAddr + 0x4;
        addr.InfAmmoAddr = (BYTE*)addr.LockSettAddr + 0x5;
        addr.NoClipAddr = (BYTE*)addr.LockSettAddr + 0x6;
        addr.MapTpAddr = (BYTE*)addr.LockSettAddr + 0x7;
        addr.VehLockAddr = (BYTE*)addr.LockSettAddr + 0x8;
        addr.VehCleanAddr = (BYTE*)addr.LockSettAddr + 0xB;
        //addr.VehDamageAddr = (BYTE*)addr.LockSettAddr + 0x18;
        addr.VehTpAddr = (BYTE*)addr.LockSettAddr + 0x1E;
        addr.MapPlayersAddr = (BYTE*)addr.LockSettAddr + 0x26;
        addr.VehMapAddr = (BYTE*)addr.LockSettAddr + 0x27;
        
    }
    else {
        std::cout << exeNotFound;
        Sleep(500);
        return -1;
    }
    
    addr.EnvHealthDecAddr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char *)"\xF3\x0F\x11\x80\xEC\x03\x00\x00\x48\x8B\x87\x58\x02\x00\x00\xF3\x45", (char*)"xxxxxxxxxxxxxxxxx", (char*)"EnvHealthDecAddr");
    addr.PlrHealthDecAddr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\xF3\x0F\x11\x80\xEC\x03\x00\x00\xF3\x44\x0F\x5C\xA7\xF0\x06\x00\x00\xF3\x44\x0F", (char*)"xxxxxxxxxxxxxxxxxxxx",(char*)"PlrHealthDecAddr");
    addr.DecPrimarySmgAmmoAddr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\x41\xFF\x4F\x08\xC7\x85\x28\x03\x00\x00\x33\x33\xB3\x3E", (char*)"xxxxxxxxxxxxxx", (char*)"DecPrimarySmgAmmoAddr");
    addr.DecPrimaryRifleAmmoAddr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\x41\xFF\x4F\x08\xC7\x85\x34\x03\x00\x00\x66\x66\xE6\x3E", (char*)"xxxxxxxxxxxxxx", (char*)"DecPrimaryRifleAmmoAddr");
    addr.DecPistolAmmoAddr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\xFF\xC8\x41\x89\x46\x08\x8B\x44\x24", (char*)"xxxxxxxxx", (char*)"DecPistolAmmoAddr");
    addr.DecC4Addr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\x41\xFF\x4E\x08\xE9\xAA\xAA\xFF\xFF\xE8", (char*)"xxxxx??xxx", (char*)"DecC4Addr");
    addr.DecGrenadeAddr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\x41\x89\x46\x08\x40\xB7\x01\x45\x38\xBD", (char*)"xxxxxxxxxx", (char*)"DecGrenadeAddr");
    //DecPrimaryWeldingTorchAddr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\xF3\x44\x0F\x5C\xC7\x41\x0F\x28\xFA\xF3\x41\x0F\x5F\xF8", (char*)"xxxxxxxxxxxxxx", (char*)"DecPrimaryWeldingTorchAddr");
    //DecFlaregunAddr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\xFF\xC8\x41\x89\x46\x08\x45\x38\xBD\x08\x24\x00\x00\x0F\x84\xC8", (char*)"xxxxxxxxxxxxxxxx", (char*)"DecFlaregunAddr");
    //DecFlareAddr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\x41\x89\x46\x08\x45\x38\xBD\x08\x24\x00\x00\x0F\x84\xFB\xFC\xFF\xFF", (char*)"xxxxxxxxxxxxxxxxx", (char*)"DecFlareAddr");
    //DecMedKitAddr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\x41\x89\x46\x08\x48\x8B\xBD\xF0", (char*)"xxxxxxxx", (char*)"DecMedKitAddr");
    //DecPrimaryFireExtAddr = (char*)PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\x41\x80\xBE\x08\x24\x00\x00\x00\x74\x23", (char*)"xxxxxxxxxx", (char*)"DecFireExtIDPatternAddr") + 0x10;
    //RifleNoSpreadAddr = (char*)PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\xF3\x0F\x59\x15\xBE\x51\x41", (char*)"xxxxxxx", (char*)"RifleNoSpreadIDPatternAddr") + 0xD;
    addr.RifleProjIDAddr = (char*)PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\xC7\x44\x24\x38\x05\x00\x00\x00\xF3\x0F", (char*)"xxxxxxxxxx", (char*)"RifleProjIDAddr") + 4;
    //RifleNoSpreadAddr = Module.modBaseAddr + 0x688F8B; you also got to add the offset to .text to use this
    addr.DecFlashlightAddr = (char*)PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\x0F\x84\xC4\xF5\xFF\xFF\xF3\x41\x0F\x10\x47\x04", (char*)"xx??xxxxxxxx", (char*)"FlashlightIDPatternAddr") + 0xC;
    addr.RifleRapidFireAddr = (char*)addr.DecPrimaryRifleAmmoAddr - 4;
    printf("%sRifleProjIDAddr = 0x%p\n", prefix.c_str(), addr.RifleProjIDAddr);
    printf("%sRifleNoSpreadAddr = 0x%p\n", prefix.c_str(), addr.RifleNoSpreadAddr);
    printf("%sDecFlashlightAddr = 0x%p\n", prefix.c_str(), addr.DecFlashlightAddr);
    //printf("%sDecFireExtAddr = 0x%p\n", prefix.c_str(), DecPrimaryFireExtAddr);
    
    if (!SetConsoleCtrlHandler(HandlerRoutine, TRUE))
    {
        std::cout << failedSetCH;
    }

    if (addr.RifleProjIDAddr != (void*)4 ) {
        al.projID = true;
    }

    if (addr.RifleNoSpreadAddr != NULL ) {
        al.noSpread = true;
    }

    if (addr.RifleRapidFireAddr != NULL ) {
        al.rapidFire = true;
    }

    if (addr.EnvHealthDecAddr != NULL
        && addr.PlrHealthDecAddr != NULL)
    {
        al.god = true;
    }

    if (
        addr.DecGrenadeAddr != NULL &&
        addr.DecC4Addr != NULL  &&
        addr.DecPrimarySmgAmmoAddr != NULL  &&
        addr.DecPistolAmmoAddr != NULL &&
        addr.DecPrimaryRifleAmmoAddr != NULL)
    {
        al.infAmmo = true; 
    }

    //if (DecPrimaryWeldingTorchAddr != NULL &&
    //    DecMedKitAddr != NULL  &&
    //    DecFlashlightAddr != NULL  &&
    //    DecPrimaryFireExtAddr != NULL  &&
    //    DecFlareAddr != NULL && 
    //    DecFlaregunAddr != NULL)
    //{
    //    al.infUtil = true;
    //}
    if (addr.DecFlashlightAddr != NULL) {
        al.infUtil = true;
    }

    
    printHelpMessage();
    while (true)
    {
        readyCmdLine();
    }
    
    cleanup();
    Sleep(500);
    return 0;
}

void readyCmdLine()
{
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
    #ifdef EXPIRE
        if(isExpired()){
            printf("This build has expired!\n");
            Sleep(5000);
            cleanup();
            ExitProcess(-1);
        }
    #endif

    std::string cmd;
    //char cmd[16];
    std::cout << prefix;
    std::getline(std::cin, cmd);
    if (cmd == "")
    {
        std::cout << "skipped command\n";
        return;
    }
    ProcessCommand(cmd);
}

void ProcessCommand(std::string cmd)
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

        if (checkDebugger()) {
            ExitProcess(-1);
    }
    #endif

        

    std::string command[8];
    splitString(command, cmd);

    //version
    if (command[0] == "version" || command[0] == "ver")
    {
        std::cout << MODVERSION;
        return;
    }
    
    //auto loadout (needs fix)
    if (command[0] == "al")     
    {
        if (!cal.auto_loadout) {
            std::cout << actionUnavailableStr;
            return;
        }
        al.playerobj = verifyPlrObjAddress();
        if (al.playerobj)
        {
            if (!ml.autoLoadout)
            {
                ml.autoLoadout = true;
                StartActionThread();
                std::cout << enableAutoloadoutStr;
            }
            else
            {
                ml.autoLoadout = false;
                std::cout << disableAutoloadoutStr;
            }
        }
        else {
            std::cout << invalidPlrObjStr;
        }
        return;
    }

    //god mode
    if (command[0] == "g" || command[0] == "god")
    {
        if (!cal.plrobj_req) {
            std::cout << actionUnavailableStr;
            return;
        }
        ml.god = !ml.god;
        if (ml.god) 
        {
            if (al.god) {
                NopEX(hProcess, addr.EnvHealthDecAddr, 8);
                NopEX(hProcess, addr.PlrHealthDecAddr, 8);
                std::cout << enableGodmodeStr;
            }
            else
            {
                std::cout << actionUnavailableStr;
            }
        }
        else
        {
            if (al.god) {
                void* healthDecInstruction = (void*)"\xF3\x0F\x11\x80\xEC\x03\x00\x00";
                PatchEX(hProcess, addr.EnvHealthDecAddr, healthDecInstruction, 8);
                PatchEX(hProcess, addr.PlrHealthDecAddr, healthDecInstruction, 8);
                std::cout << disableGodmodeStr;
            }
            else
            {
                std::cout << actionUnavailableStr;
            }
        }
        return;
    }

    if (command[0] == "heal") {
        if (!cal.plrobj_req) {
            std::cout << actionUnavailableStr;
            return;
        }
        al.playerobj = verifyPlrObjAddress();
        if (!al.playerobj)
        {
            addr.PlrObjAddr = tryGetPlrObj();
            al.playerobj = verifyPlrObjAddress();
            if (!al.playerobj) {
                std::cout << invalidPlrObjStr;
                return;
            }
        }
        PatchEX(hProcess, (BYTE*)addr.PlrObjAddr + 0x3C4, (void*)"\x00\x00\xC8\x42", 4);
        printf("%sPlayer healed!\n", prefix.c_str());
        return;
    }

    if (command[0] == "die" || command[0] == "kill") {
        if (!cal.plrobj_req) {
            std::cout << actionUnavailableStr;
            return;
        }
        al.playerobj = verifyPlrObjAddress();
        if (!al.playerobj)
        {
            addr.PlrObjAddr = tryGetPlrObj();
            al.playerobj = verifyPlrObjAddress();
            if (!al.playerobj) {
                std::cout << invalidPlrObjStr;
                return;
            }
        }
        PatchEX(hProcess, (BYTE*)addr.PlrObjAddr + 0x3C4, (void*)"\x00\x00\x00\x00", 4);
        printf("%sPlayer killed!\n", prefix.c_str());
        return;
    }

    if (command[0] == "ps") { //player scanner
        if (!cal.plrobj_req) {
            std::cout << actionUnavailableStr;
            return;
        }
        al.playerobj = verifyPlrObjAddress();
        if (!al.playerobj) {
            printf("%sStarting player object scanner\n", prefix.c_str());
            getPlayerObjWhenAvailable();
        }
        else {
            printf("%sPlayer object has already been found, if this is not true, use \"flushplr\" command.\n",prefix.c_str());
        }
        return;
    }

    if (command[0] == "flushplr") {
        addr.PlrObjAddr = nullptr;
        al.playerobj = false;
        return;
    }


    //infinite page
    if (cmd[0] == 'i')
    {
        if (cmd[1] == NULL)
        {
            std::cout << invalidArgumentStr;
            return;
        }
        for (int i = 1; i < sizeof(cmd) - 1; i++)
        {
            //infinite ammo
            if (cmd[i] == 'a')
            {
                if (!cal.inf_ammo) {
                    std::cout << actionUnavailableStr;
                    return;
                }
                if (cmd[i + 1] == 'g')
                {
                    if (!ml.infAmmoG) {
                        ml.infAmmoG = true;
                        StartActionThread();
                        std::cout << enableGInfammoStr;
                    }
                    else {
                        std::cout << disableGInfammoStr;
                        ml.infAmmoG = false;
                        
                    }
                    return;
                }
                else
                {
                    if (command[1] == "-lazy" || command[1] == "-l") {
                        if (command[2].empty()) {
                            flags.method = "lazy";
                            StartActionThread();
                        }

                    }
                    if (al.infAmmo && flags.method == "default") //al.infAmmo
                    {
                        if (!ml.infAmmo) {
                            EnableInfAmmo(addr.DecPrimarySmgAmmoAddr, addr.DecPrimaryRifleAmmoAddr, addr.DecPistolAmmoAddr, addr.DecC4Addr, addr.DecGrenadeAddr);
                            printf(enableInfammoStr.c_str());
                        }
                        else {
                            printf(disableInfammoStr.c_str());
                            flags.method = "default";
                            DisableInfAmmo(addr.DecPrimarySmgAmmoAddr, addr.DecPrimaryRifleAmmoAddr, addr.DecPistolAmmoAddr, addr.DecC4Addr, addr.DecGrenadeAddr);
                        }
                        ml.infAmmo = !ml.infAmmo;
                    }
                    else {
                        if (ml.infAmmo) {
                            printf(disableInfammoStr.c_str());
                        }
                        else {
                            printf(enableInfammoStr.c_str());
                        }
                        ml.infAmmo = !ml.infAmmo;
                        return;
                    }
                }
            }
                //infinite util (depricated)
                if (cmd[i] == 'u')
                {
                    if (!cal.inf_util) {
                        std::cout << actionUnavailableStr;
                        return;
                    }
                    if (al.infUtil)
                    {
                        ml.infUtil = !ml.infUtil;
                        if (ml.infUtil)
                        {
                            //std::cout << enableInfutilStr;
                            //EnableInfUtil();
                            EnableInfFlashlight(addr.DecFlashlightAddr);
                            printf(enableInfFlashlightStr.c_str());
                        }
                        else
                        {
                            //std::cout << disableInfutilStr;
                            //DisableInfUtil();
                            DisableInfFlashlight(addr.DecFlashlightAddr);
                            printf(disableInfFlashlightStr.c_str());
                        }
                    }
                    else
                    {
                        std::cout << actionUnavailableStr;

                    }
                }
        }
        return;
    }

    //keep action thread active
    if (command[0] == "ka")
    {
        if (!bKeepActive)
        {
            std::cout << enableKeepActiveStr;
        }
        else {
            std::cout << disableKeepActiveStr;
        }
        bKeepActive = !bKeepActive;
        return;
    }
    
    /*if (command[0] == "dll") {
        if (GetFileAttributes(dllPath) == INVALID_FILE_ATTRIBUTES) {
            printf("%sDll file doesn't exist\n",prefix.c_str());
            return;
        }

        std::ifstream File(dllPath, std::ios::binary | std::ios::ate);

        if (File.fail()) {
            printf("%sOpening the file failed: %X\n",prefix.c_str(), (DWORD)File.rdstate());
            File.close();
            return;
        }

        auto FileSize = File.tellg();
        if (FileSize < 0x1000) {
            printf("%sFilesize invalid.\n",prefix.c_str());
            File.close();
            return;
        }

        BYTE* pSrcData = new BYTE[(UINT_PTR)FileSize];
        if (!pSrcData) {
            printf("%sCan't allocate dll file.\n",prefix.c_str());
            File.close();
            return;
        }

        File.seekg(0, std::ios::beg);
        File.read((char*)(pSrcData), FileSize);
        File.close();

        printf("%sMapping...\n",prefix.c_str());
        if (!ManualMapDll(hProcess, pSrcData, FileSize)) {
            delete[] pSrcData;
            printf("%sError while mapping.\n",prefix.c_str());
            return;
        }
        delete[] pSrcData;
        printf("%sdone!\n",prefix.c_str());
        return;
    }*/


    //admin menu
    if (command[0] == "am" || command[0] == "cm")
    {
        if (!cal.custom_menu) {
            std::cout << actionUnavailableStr;
            return;
        }
        if (command[1] == "-bypass" || command[1] == "-b") {
            flags.bypass = true;
        }

        if (!ml.forceAdminMenu) {
            ml.forceAdminMenu = true;
            StartActionThread();
            std::cout << enableAdminMenuStr;
            
        }
        else {
            std::cout << disableAdminMenuStr;
            ml.forceAdminMenu = false;
            flags.bypass = false;
        }
        return;
    }
    
    //show map players
    if (command[0] == "sp" || command[0] == "mp")
    {
        if (!cal.custom_menu) {
            std::cout << actionUnavailableStr;
            return;
        }
        if (!ml.mapPlrs) {
            ml.mapPlrs = true;
            StartActionThread();
            std::cout << enableMapPlrsStr;

        }
        else {
            std::cout << disableMapPlrsStr;
            ml.mapPlrs = false;
        }
        return;
    }
  
    //noclip
    if (command[0] == "nc" || command[0] == "noclip")
    {
        if (!cal.custom_menu) {
            std::cout << actionUnavailableStr;
            return;
        }
        if (!ml.forceNoClip) {
            ml.forceNoClip = true;
            StartActionThread();
            std::cout << enableNoclipStr;
        }
        else {
            std::cout << disableNoclipStr;
            ml.forceNoClip = false;
        }
        return;
    }

    //kill thread
    if (command[0] == "kt")
    {
        bPlrScanThread = false;
        bActionThread = false;
        return;
    }
    //workbench
    if (command[0] == "wb") {
        if (!cal.workbench) {
            std::cout << actionUnavailableStr;
            return;
        }
        addr.WorkbenchLockAddr = tryGetWorkbenchLock();
        printf("Workbench lock address: 0x%p\n", addr.WorkbenchLockAddr);
        return;
    }

    if (command[0] == "auth") {
        if (!cal.workbench) {
            std::cout << actionUnavailableStr;
            return;
        }
        addr.WorkbenchLockAddr = tryGetWorkbenchLock();
        if (addr.WorkbenchLockAddr != NULL) {
            PatchEX(hProcess, addr.WorkbenchLockAddr, (BYTE*)"\x01", 1);
            printf(authWorkbenchStr.c_str());
            return;
        }
        printf("Somthing went wrong! WorkbenchLockAddr: 0x%p\n", addr.WorkbenchLockAddr);
        return;
    }

    if (command[0] == "deauth") {
        if (!cal.workbench) {
            std::cout << actionUnavailableStr;
            return;
        }
        addr.WorkbenchLockAddr = tryGetWorkbenchLock();
        if (addr.WorkbenchLockAddr != NULL) {
            PatchEX(hProcess, addr.WorkbenchLockAddr, (BYTE*)"\x00", 1);
            printf(deauthWorkbenchStr.c_str());
            return;
        }
        printf("Somthing went wrong! WorkbenchLockAddr: 0x%p\n", addr.WorkbenchLockAddr);
        return;
    }

    //give command
    if (command[0] == "give") {
        if (!cal.plrobj_req || !cal.give_item) {
            std::cout << actionUnavailableStr;
            return;
        }
        al.playerobj = verifyPlrObjAddress();
        if (!al.playerobj)
        {
            addr.PlrObjAddr = tryGetPlrObj();
            al.playerobj = verifyPlrObjAddress();
            if (!al.playerobj) {
                std::cout << invalidPlrObjStr;
                return;
            }
        }
        if (command[1].empty() || command[2].empty()) {
            printf("%s\n",invalidArgumentStr.c_str());
            return;
        }

        FLOAT optCharge = 100;
        UINT optAmmo = 30;

        if (!command[3].empty())
        {
            optCharge = stof(command[3]);
        }
        if (!command[4].empty())
        {
            optAmmo = stoi(command[4]);
        }


        UINT item = stoi(command[1]);
        UINT slot = stoi(command[2]);
        if (slot < 0 || slot > 10) {
            printf("%s\n", invalidArgumentStr.c_str());
            return;
        }
        GiveItem(hProcess, slot,item, optCharge, optAmmo);
        return;
    }



    //gives flashlight
    if (command[0] == "flash" || command[0] == "flashlight")
    {
        if (!cal.plrobj_req || !cal.give_item) {
            std::cout << actionUnavailableStr;
            return;
        }
        al.playerobj = verifyPlrObjAddress();
        if (!al.playerobj)
        {
            addr.PlrObjAddr = tryGetPlrObj();
            al.playerobj = verifyPlrObjAddress();
            if (!al.playerobj) {
                std::cout << invalidPlrObjStr;
                return;
            }
        }
        
        UINT arg = 1;
        if (!command[1].empty()) {
            arg = stoi(command[1]);
        }
        if (arg < 1 || arg > 9) {
            printf("%s\n",invalidArgumentStr.c_str());
            return;
        }

        //int i = *(reinterpret_cast<int*>(&f));
        //printf("%08x\n", i);
        GiveItem(hProcess, arg, SWITEM::flashlightID, 100, 0);
        printf("%sLet there be light!\n", prefix.c_str());
        return;
    }

    //reloads primary
    if (command[0] == "reload" || command[0] == "rl")
    {
        if (!cal.plrobj_req) {
            std::cout << actionUnavailableStr;
            return;
        }
        al.playerobj = verifyPlrObjAddress();
        if (!al.playerobj)
        {
            addr.PlrObjAddr = tryGetPlrObj();
            al.playerobj = verifyPlrObjAddress();
            if (!al.playerobj) {
                std::cout << invalidPlrObjStr;
                return;
            }
        }
        UINT arg = 0;
        if (!command[1].empty()) {
            arg = stoi(command[1]);
        }
        if (arg < 0 || arg > 9) {
            printf("%s\n", invalidArgumentStr.c_str());
            return;
        }
        setAmmo(arg, 1000);
        printf("%sWhats the point of this?\n", prefix.c_str());
        return;
    }

    //fixes integers
    if (command[0] == "fix" || command[0] == "fx")
    {
        if (!cal.plrobj_req) {
            std::cout << actionUnavailableStr;
            return;
        }
        al.playerobj = verifyPlrObjAddress();
        if (!al.playerobj)
        {
            addr.PlrObjAddr = tryGetPlrObj();
            al.playerobj = verifyPlrObjAddress();
            if (!al.playerobj) {
                std::cout << invalidPlrObjStr;
                return;
            }
        }
        UINT arg = 0;
        if (!command[1].empty()) {
            arg = stoi(command[1]);
        }
        if (arg < 0 || arg > 9) {
            printf("%s\n", invalidArgumentStr.c_str());
            return;
        }
        
        setCharge(arg, 100);
        setAmmo(arg, 100);
        printf("%shehe\n", prefix.c_str());
        return;
    }


 
    //test command 
    if (command[0] == "dbg") {

        return;
    }

    if (command[0] == "getslot" || command[0] == "getitem" || command[0] == "qslot") {
        if (!cal.plrobj_req) {
            std::cout << actionUnavailableStr;
            return;
        }
        al.playerobj = verifyPlrObjAddress();
        if (!al.playerobj)
        {
            addr.PlrObjAddr = tryGetPlrObj();
            al.playerobj = verifyPlrObjAddress();
            if (!al.playerobj) {
                std::cout << invalidPlrObjStr;
                return;
            }
        }
        UINT pos = 0;
        if (stoi(command[1]) >= 0 && stoi(command[1]) < 10) {
            pos = stoi(command[1]);
        }
        uint16_t item = GetItem(hProcess, pos);
        printf("%sItem at slot %d has id %d\n",prefix.c_str(),pos,item);
        return;
    }

    if (command[0] == "projid") {
        if (!cal.plrobj_req || !cal.change_projid) {
            std::cout << actionUnavailableStr;
            return;
        }
        if (!al.playerobj)
        {
            addr.PlrObjAddr = tryGetPlrObj();
            al.playerobj = verifyPlrObjAddress();
            if (!al.playerobj) {
                std::cout << invalidPlrObjStr;
                return;
            }
        }
        if (command[1] == "reset" || command[1] == "r") {
            changeProjId(5, addr.RifleProjIDAddr);
            printf("%sRestored rifle projectile id.\n", prefix.c_str());
            ml.projidchanged = false;
            return;
        }
        if (command[1] == "b" || command[1] == "bertha") {
            ml.projidchanged = true;
            changeProjId(14, addr.RifleProjIDAddr);
            printf(updateRifleProjidStr.c_str());
            return;
        }
        if (command[1].empty() || stoi(command[1]) < 1) {
            printf(invalidArgumentStr.c_str());
            return;
        }

        
        #ifndef DEBUG
            if (stoi(command[1]) > 15) {
                printf(invalidArgumentStr.c_str());
                return;
            }
        #endif 

        ml.projidchanged = true;
        changeProjId(stoi(command[1]), addr.RifleProjIDAddr);
        printf(updateRifleProjidStr.c_str());
        return;
    }

    if (command[0] == "sip" || command[0] == "setinvalidproj") {
        if (!cal.plrobj_req || !cal.change_projid || !cal.set_invalid_projid) {
            std::cout << actionUnavailableStr;
            return;
        }
        ml.projidchanged = true;
        changeProjId(20, addr.RifleProjIDAddr);
        printf("%sInvalid projectile id set.\n", prefix.c_str());
        return;
    }



    if (command[0] == "ns" || command[0] == "nospread") {
        if (!cal.no_spread) {
            std::cout << actionUnavailableStr;
            return;
        }
        if (!al.noSpread) {
            printf("%s",actionUnavailableStr.c_str());
            return;
        }
        if (!ml.noSpread) {
            EnableNoSpread(addr.RifleNoSpreadAddr);
            printf("%s",enableNoSpreadStr.c_str());
        }
        else {
            DisableNoSpread(addr.RifleNoSpreadAddr);
            printf("%s", disableNoSpreadStr.c_str());
        }
        ml.noSpread = !ml.noSpread;
        return;
    }

    if (command[0] == "rf" || command[0] == "rapidfire") {
        if (!cal.rapid_fire) {
            std::cout << actionUnavailableStr;
            return;
        }
        if (!al.rapidFire) {
            printf("%s", actionUnavailableStr.c_str());
            return;
        }
        if (!ml.rapidFire) {
            EnableRapidFire(addr.RifleRapidFireAddr);
            printf("%s", enableRapidFireStr.c_str());
        }
        else {
            DisableRapidFire(addr.RifleRapidFireAddr);
            printf("%s", disableRapidFireStr.c_str());
        }
        ml.rapidFire = !ml.rapidFire;
        return;
    }

    //rifle
    if (command[0] == "rifle")
    {
        if (!cal.plrobj_req || !cal.give_item) {
            std::cout << actionUnavailableStr;
            return;
        }
        al.playerobj = verifyPlrObjAddress();
        char* PrimaryItemslotAddr = (char*)addr.PlrObjAddr + 0x268;
        if (!al.playerobj)
        {
            addr.PlrObjAddr = tryGetPlrObj();
            al.playerobj = verifyPlrObjAddress();
            if (!al.playerobj) {
                std::cout << invalidPlrObjStr;
                return;
            }
        }
        GiveItem(hProcess, 0, SWITEM::rifleID, 0, 30);
        printf("%sRifle given!\n", prefix.c_str());
        return;
    }
    //torch
    if (command[0] == "torch")
    {
        if (!cal.plrobj_req || !cal.give_item) {
            std::cout << actionUnavailableStr;
            return;
        }
        al.playerobj = verifyPlrObjAddress();
        char* PrimaryItemslotAddr = (char*)addr.PlrObjAddr + 0x268;
        if (!al.playerobj)
        {
            addr.PlrObjAddr = tryGetPlrObj();
            al.playerobj = verifyPlrObjAddress();
            if (!al.playerobj) {
                std::cout << invalidPlrObjStr;
                return;
            }
        }
        GiveItem(hProcess, 0, SWITEM::welding_torchID, 400, 0);
        return;
    }
    //vehicle spawning
    if (command[0] == "vs" || command[0] == "vehiclespawning") {
        ml.forceVehicleSpawning = !ml.forceVehicleSpawning;
        if (ml.forceVehicleSpawning) {
            StartActionThread();
            printf(enableVehSpawningStr.c_str());
        }else{
            printf(disableVehSpawningStr.c_str());
        }
        return;
    }
    
    if (command[0] == "grenade")
    {
        if (!cal.plrobj_req || !cal.give_item) {
            std::cout << actionUnavailableStr;
            return;
        }
        al.playerobj = verifyPlrObjAddress();
        if (!al.playerobj)
        {
            addr.PlrObjAddr = tryGetPlrObj();
            al.playerobj = verifyPlrObjAddress();
            if (!al.playerobj) {
                std::cout << invalidPlrObjStr;
                return;
            }
        }
        char* PrimaryItemslotAddr = (char*)addr.PlrObjAddr + 0x268;
        GiveItem(hProcess, 3, SWITEM::grenadeID, 0, 1);
        return;
    }

    //loadout
    if (command[0] == "loa")
    {
        if (!cal.plrobj_req || !cal.give_item) {
            std::cout << actionUnavailableStr;
            return;
        }
        giveLoadout();
        return;
    }

    //help 
    if (command[0] == "?" || command[0] == "help")
    {
        printHelpMessage();
        return;
    }
    //exit
    if (command[0] == "x")
    {
        cleanup();
        Sleep(500);
        ExitProcess(-1);
    }
    std::cout << prefix << "Invalid command!\n";
}

void cleanup()
{

    bPlrScanThread = false;
    if (getProcID((wchar_t*)L"stormworks64.exe") == 0) {
        CloseHandle(hProcess);
        return;
    }
    std::cout << "\n";
    
    if (ml.projidchanged) {
        changeProjId(5, addr.RifleProjIDAddr);
    }
    if (ml.god) 
    {
        std::cout << disableGodmodeStr;
        void* healthDecInstruction = (void*)"\xF3\x0F\x11\x80\xC4\x03\x00\x00";
        PatchEX(hProcess, addr.EnvHealthDecAddr, healthDecInstruction, 8);
        PatchEX(hProcess, addr.PlrHealthDecAddr, healthDecInstruction, 8);
    }
    if (ml.noSpread) {
        DisableNoSpread(addr.RifleNoSpreadAddr);
        printf(disableNoSpreadStr.c_str());
    }
    if (ml.rapidFire) {
        DisableRapidFire(addr.RifleRapidFireAddr);
        printf(disableRapidFireStr.c_str());
    }
    if (ml.infAmmo)
    {
        DisableInfAmmo(addr.DecPrimarySmgAmmoAddr, addr.DecPrimaryRifleAmmoAddr, addr.DecPistolAmmoAddr, addr.DecC4Addr, addr.DecGrenadeAddr);
        printf(disableInfammoStr.c_str());
    }
    if (ml.infUtil)
    {
        //DisableInfUtil();
        DisableInfFlashlight(addr.DecFlashlightAddr);
        printf(disableInfutilStr.c_str());
    }
    if (bActionThread)
    {
        bActionThread = false;
        printf("%sShutting down action thread.\n",prefix.c_str());
        Sleep(1000);
    }
    CloseHandle(hProcess);
}



void StartActionThread()
{
    if (!bActionThread)
    {
        bActionThread = true;
        std::thread tActionThread(ActionThread);
        tActionThread.detach();
        //std::cout << startActionThreadStr;
    }
}

void IntegrityThread() {
    adbg_NtSetInformationThread();
    const int build_copy = __DATE_TIME_UNIX__;
    while (true) {
        if (checkDebugger()) {
            ExitProcess(-1);
        }
        if (build_copy + 21600 != build_time) {
            ExitProcess(-1);
        }
        Sleep(20000);
    }
}



void ActionThread()
{
    bool hasDied = false;
    //FLOAT* health;
    adbg_NtSetInformationThread();
    while (bActionThread)
    {
        //auto loadout
        if (ml.autoLoadout)
        {
            //health = ProtectedFloatRead(hProcess, (char*)PlrObjAddr + 0x3C4, 4);
            //std::cout << health << "\n";
            //if (health == (void*)0xFFFFFFFF)
            //{
            //    std::cout << invalidPlrObjStr + prefix;
            //   ml.autoLoadout = false;
            //    continue;
            //}
            //
            //if (health == 0 && hasDied == false) {
            //    hasDied = true;
            //    std::cout << ">> Detected local player death\n";
            //}
            //if (hasDied && health == (FLOAT*)0x42C80000)
            //{
            //    std::cout << ">> Detected local player respawn\n";
            //    hasDied = false;
            //    giveLoadout();
            //}
        }
        //inf ammo
        if (ml.infAmmoG) {
            PatchEX(hProcess, addr.InfAmmoAddr, (BYTE*)"\x01", 1);

        }
        if (ml.infAmmo && flags.method == "lazy") {
            setAmmo(0, 100);
        }
        //show map players
        if (ml.mapPlrs) {
            PatchEX(hProcess, addr.MapPlayersAddr, (BYTE*)"\x01", 1);
        }
        //inf electric
        if (ml.infElec) {
            PatchEX(hProcess, addr.InfElecAddr, (BYTE*)"\x01", 1);

        }
        //inf fuel
        if (ml.infFuel) {
            PatchEX(hProcess, addr.InfFuelAddr, (BYTE*)"\x01", 1);

        }
        //noclip
        if (ml.forceNoClip) {
            PatchEX(hProcess, addr.NoClipAddr, (BYTE*)"\x01", 1);
            PatchEX(hProcess, addr.MapTpAddr, (BYTE*)"\x01", 1);
        }
        if (ml.forceVehicleSpawning) {
            PatchEX(hProcess, addr.VehLockAddr, (BYTE*)"\x01", 1); //VehLock
        }
        //Force admin menu
        if (ml.forceAdminMenu) {
            PatchEX(hProcess, addr.VehTpAddr, (BYTE*)"\x01", 1); //VehTp
            PatchEX(hProcess, addr.VehMapAddr, (BYTE*)"\x01", 1); //VehMap
            PatchEX(hProcess, addr.VehCleanAddr, (BYTE*)"\x01", 1);//VehClean
            PatchEX(hProcess, addr.LockSettAddr, (BYTE*)"\x00", 1); //LockSett

            if (flags.bypass) {
                PatchEX(hProcess, addr.ConfigLockAddr, (BYTE*)"\x01", 1);
            }
        }

        if (!ml.forceNoClip && !ml.forceAdminMenu && !ml.autoLoadout && !ml.mapPlrs &&
            !ml.infElec && !ml.infFuel && !ml.forceVehicleSpawning &&
            !ml.infAmmoG && !bKeepActive && !ml.infAmmo)
        {
            bActionThread = false;
            break;
        }
        Sleep(750);
    }
    //std::cout << stopActionThreadStr + "\n" + prefix;
}



void getPlayerObjWhenAvailable() {
    addr.PlrObjAddr = tryGetPlrObj();
    al.playerobj = verifyPlrObjAddress();
    if (al.playerobj) {
        printf("%sPlayer object found: 0x%p\n", prefix.c_str(), addr.PlrObjAddr);
    }
    else {
        std::thread tPlrObjWait(waitForPlrObj);
        tPlrObjWait.detach();
    }
}

void waitForPlrObj() {
    bPlrScanThread = true;
    unsigned int c = 1;
    do {
        if (c % PS_IDLE_SCANS == 0) {
            printf("Plr obj not found in %u scans, possibly idle.\n%s",c, prefix.c_str());
        }
        addr.PlrObjAddr = tryGetPlrObj();
        c++;
        Sleep(2000);
        al.playerobj = verifyPlrObjAddress();
    } while (!al.playerobj && bPlrScanThread);
    addr.PlrSlotAddr = (char*)addr.PlrObjAddr + 0x240;
    printf("Player object found after %u scans\n%s", c, prefix.c_str());
}


//auto v{ split(str, " ") };
template <size_t N>
void splitString(std::string(&arr)[N], std::string str)
{
    int n = 0;
    std::istringstream iss(str);
    for (auto it = std::istream_iterator<std::string>(iss); it != std::istream_iterator<std::string>() && n < N; ++it, ++n)
        arr[n] = *it;
}

bool isExpired() {
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

    #ifdef OFFLINE
        time_t current_time = time(NULL);
    #else
        time_t current_time = getNTPTime();
    #endif

        
    #ifdef DEBUG
        printf("local Time: %lld\n", current_time);
    #endif 

    static const int expire_after = expire_build_time; // 7 days
    //static const int expire_after = build_time + 60 * 10;
    return current_time >= expire_after && current_time != NULL;
}

MODULEENTRY32 getSWModule() {
    return Module;
}

HANDLE gethProcess() {
    return hProcess;
}

void* getPlayerObjAddr() {
    return addr.PlrObjAddr;
}

bool isPlrScannerRunning() {
    return bPlrScanThread;
}

struct addresses getAddresses() {
    return addr;
}

struct allowList getAllowList() {
	return al;
}

struct modList getModList() {
	return ml;
}

struct cmd_flags getFlags() {
	return flags;
}

bool isActionThreadRunning() {
	return bActionThread;
}



