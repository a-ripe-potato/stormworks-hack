
#include "swmod.h"
#include <iostream>
#include <windows.h> 
#include <psapi.h>
#include "mem.h"
#include "proctools.h"
#include "patternscan.h"
#include <thread>
#include "strings.h"


struct allowList al;
struct modList ml;
BYTE* NoClipAddr;
BYTE* VehTpAddr;
BYTE* VehMapAddr;
BYTE* VehCleanAddr;
BYTE* LockSettAddr;
BYTE* MapTpAddr;
BYTE* VehDamageAddr;
BYTE* MapPlayersAddr;
BYTE* InfElecAddr;
BYTE* InfFuelAddr;
BYTE* DisableWeaponsAddr;
BYTE* InfAmmoAddr;
char* token;
void* EnvHealthDecAddr;
void* PlrHealthDecAddr;
void* DecPrimarySmgAmmoAddr;
void* DecPrimaryRifleAmmoAddr;
void* DecPrimaryWeldingTorchAddr;
void* DecPrimaryFireExtAddr;
void* DecPistolAmmoAddr;
void* DecC4Addr;
void* DecGrenadeAddr;
void* DecFlaregunAddr;
void* DecFlareAddr;
void* DecMedKitAddr;
void* DecFlashlightAddr;
void* PlrObjAddr;
void* PlrSlotAddr;
char* MAXSEARCHADDR = (char*)0xFFFFFFFFFFFF0000;
MODULEENTRY32 Module;
HANDLE hProcess;
std::thread tAutoloadout;
bool bActionThread = false;
bool bKeepActive = false;
DWORD processID;
bool bPlrScanThread = false;


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
    MSG msg = { };
    processID = getProcID((wchar_t *)L"stormworks64.exe");
    if (processID != 0)
    {
        hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, processID);
        Module = getModule(processID, (wchar_t*)(L"stormworks64.exe"));
        printf("%smodule base addr 0x%p\n",prefix.c_str(), Module.modBaseAddr);
        getPlayerObjWhenAvailable();
        if (!verifyPlrObjAddress()) {
            printf("%sFailed to locate player object, waiting for init.\n", prefix.c_str());
        }
        else {
            PlrSlotAddr = (char*)PlrObjAddr + 0x240;
        }
        LockSettAddr = (BYTE*)Module.modBaseAddr + 0xBD5EB2;
        InfElecAddr = (BYTE*)LockSettAddr + 0x1;
        InfFuelAddr = (BYTE*)LockSettAddr + 0x2;
        DisableWeaponsAddr = (BYTE*)LockSettAddr + 0x4;
        InfAmmoAddr = (BYTE*)LockSettAddr + 0x5;
        NoClipAddr = (BYTE*)LockSettAddr + 0x6;
        MapTpAddr = (BYTE*)LockSettAddr + 0x7;
        VehCleanAddr = (BYTE*)LockSettAddr + 0xB;
        VehDamageAddr = (BYTE*)LockSettAddr + 0x18;
        VehTpAddr = (BYTE*)LockSettAddr + 0x1E;
        MapPlayersAddr = (BYTE*)LockSettAddr + 0x26;
        VehMapAddr = (BYTE*)LockSettAddr + 0x27;
        
    }
    else {
        std::cout << exeNotFound;
        Sleep(500);
        return -1;
    }
    
    EnvHealthDecAddr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char *)"\xF3\x0F\x11\x80\xC4\x03\x00\x00\x48\x8B\x87\x58\x02\x00\x00\xF3\x45\x0F\x59\xD7", (char*)"xxxxxxxxxxxxxxxxxxxx", (char*)"EnvHealthDecAddr");
    PlrHealthDecAddr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\xF3\x0F\x11\x80\xC4\x03\x00\x00\xF3\x44\x0F\x5C\xA7\xE8\x06\x00\x00", (char*)"xxxxxxxxxxxxxxxxx",(char*)"PlrHealthDecAddr");
    DecPrimarySmgAmmoAddr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\x41\xFF\x4D\x08\xC7\x85\x50\x02\x00\x00\x33\x33\xB3\x3E", (char*)"xxxxxxxxxxxxxx", (char*)"DecPrimarySmgAmmoAddr");
    DecPrimaryRifleAmmoAddr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\x41\xFF\x4D\x08\xC7\x85\x5C\x02\x00\x00\x66\x66\xE6\x3E", (char*)"xxxxxxxxxxxxxx", (char*)"DecPrimaryRifleAmmoAddr");
    DecPistolAmmoAddr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\x41\x89\x46\x08\x8B\x44\x24\x48\x89\x44\x24\x68", (char*)"xxxxxxxxxxxx", (char*)"DecPistolAmmoAddr");
    DecC4Addr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\x41\xFF\x4E\x08\xE9\x06\xEE\xFF\xFF", (char*)"xxxxxxxxx", (char*)"DecC4Addr");
    DecGrenadeAddr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\x41\x89\x46\x08\x40\xB7\x01\x45\x38\xBD", (char*)"xxxxxxxxxx", (char*)"DecGrenadeAddr");
    DecPrimaryWeldingTorchAddr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\xF3\x44\x0F\x5C\xC7\x41\x0F\x28\xFA\xF3\x41\x0F\x5F\xF8", (char*)"xxxxxxxxxxxxxx", (char*)"DecPrimaryWeldingTorchAddr");
    DecPrimaryFireExtAddr = (char*)PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\x41\x80\xBE\x08\x24\x00\x00\x00\x74\x23", (char*)"xxxxxxxxxx", (char*)"DecFireExtIDPatternAddr") + 0x10;
    DecFlaregunAddr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\x41\x89\x46\x08\x45\x38\xBD\x08\x24\x00\x00\x0F\x84\xC6\xFA\xFF\xFF", (char*)"xxxxxxxxxxxxxxxxx", (char*)"DecFlaregunAddr");
    DecFlareAddr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\x41\x89\x46\x08\x45\x38\xBD\x08\x24\x00\x00\x0F\x84\xFB\xFC\xFF\xFF", (char*)"xxxxxxxxxxxxxxxxx", (char*)"DecFlareAddr");
    DecMedKitAddr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\x41\x89\x46\x08\x48\x8B\xBD\x00\x01\x00\x00\x8B\x9E\x98", (char*)"xxxxxxxxxxxxxx", (char*)"DecMedKitAddr");
    DecFlashlightAddr = (char*)PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\x0F\x84\x50\xF7\xFF\xFF\x45\x84\xD2", (char*)"xxxxxxxxx", (char*)"FlashlightIDPatternAddr") + 0x15;
    std::cout << ">> DecFlashlightAddr = 0x" << DecFlashlightAddr << "\n";
    std::cout << ">> DecFireExtAddr = 0x" << DecPrimaryFireExtAddr << "\n";
    if (!SetConsoleCtrlHandler(HandlerRoutine, TRUE))
    {
        std::cout << failedSetCH;
    }

    if (EnvHealthDecAddr != NULL && EnvHealthDecAddr < MAXSEARCHADDR
        && PlrHealthDecAddr != NULL && PlrHealthDecAddr < MAXSEARCHADDR)
    {
        al.god = true;
    }

    if (
       DecGrenadeAddr != NULL && DecGrenadeAddr < MAXSEARCHADDR &&
       DecC4Addr != NULL && DecC4Addr < MAXSEARCHADDR &&
       DecPrimarySmgAmmoAddr != NULL && DecPrimarySmgAmmoAddr < MAXSEARCHADDR &&
       DecPistolAmmoAddr != NULL && DecPistolAmmoAddr < MAXSEARCHADDR &&
       DecPrimaryRifleAmmoAddr != NULL && DecPrimaryRifleAmmoAddr < MAXSEARCHADDR)
    {
        al.infAmmo = true; 
    }

    if (DecPrimaryWeldingTorchAddr != NULL && DecPrimaryWeldingTorchAddr < MAXSEARCHADDR &&
        DecMedKitAddr != NULL && DecMedKitAddr < MAXSEARCHADDR &&
        DecFlashlightAddr != NULL && DecFlashlightAddr < MAXSEARCHADDR &&
        DecPrimaryFireExtAddr != NULL && DecPrimaryFireExtAddr < MAXSEARCHADDR &&
        DecFlareAddr != NULL && DecFlareAddr < MAXSEARCHADDR &&
        DecFlaregunAddr != NULL && DecFlaregunAddr < MAXSEARCHADDR)
    {
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

void ProcessCommand(std::string command)
 
{
    int  page = 0; //0 is main, 1 is infinite 

    char cmd = command[0];

    //version
    if (command == "version" || command == "ver")
    {
        std::cout << MODVERSION;
        return;
    }
    
    //auto loadout
    if (command == "al")
    {
        //std::cout << actionUnavailableStr; //need to fix this
        //return;

        if (verifyPlrObjAddress())
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
    if (cmd == 'g' && command[1] == NULL || command == "god")
    {
        ml.god = !ml.god;
        if (ml.god) 
        {
            if (al.god) {
                NopEX(hProcess, EnvHealthDecAddr, 8);
                NopEX(hProcess, PlrHealthDecAddr, 8);
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
                void* healthDecInstruction = (void*)"\xF3\x0F\x11\x80\xC4\x03\x00\x00";
                PatchEX(hProcess, EnvHealthDecAddr, healthDecInstruction, 8);
                PatchEX(hProcess, PlrHealthDecAddr, healthDecInstruction, 8);
                std::cout << disableGodmodeStr;
            }
            else
            {
                std::cout << actionUnavailableStr;
            }
        }
        return;
    }

    if (command == "heal") {
        if (verifyPlrObjAddress()) {
            PatchEX(hProcess, (BYTE*)PlrObjAddr + 0x3C4, (void*)"\x00\x00\xC8\x42", 4);
            printf("%sPlayer healed!\n",prefix.c_str());
        }
        else {
            getPlayerObjWhenAvailable();
            printf("%s\n", actionUnavailableStr.c_str());
        }
        return;
    }

    if (command == "die" || command == "kill") {
        if (verifyPlrObjAddress()) {
            PatchEX(hProcess, (BYTE*)PlrObjAddr + 0x3C4, (void*)"\x00\x00\x00\x00", 4);
            printf("%sPlayer killed!\n",prefix.c_str());
        }
        else {
            getPlayerObjWhenAvailable();
            printf("%s\n", actionUnavailableStr.c_str());
        }
        return;
    }

    if (command == "start ps") {
        if (!verifyPlrObjAddress()) {
            getPlayerObjWhenAvailable();
            printf("%sStarted player object scanner\n",prefix.c_str());
        }
        else {
            printf("%sPlayer object has already been found\n",prefix.c_str());
        }
        return;
    }

    if (command[0] == 'g' && command[1] == 'i') {
        return;
        std::string a[5];
        splitString(a, command);
        //GiveItem(PlrObjAddr, (DWORD)std::stoi(a[1]), std::stoi(a[2]),);
   
    }




    //infinite page
    if (cmd == 'i')
    {
        if (command[1] == NULL)
        {
            std::cout << invalidArgumentStr;
            return;
        }
        for (int i = 1; i < sizeof(command) - 1; i++)
        {
            //infinite ammo
            if (command[i] == 'a')
            {
                if (command[i + 1] == 'g')
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
                }
                else
                {
                    if (al.infAmmo) //al.infAmmo
                    {
                        if (!ml.infAmmo) {
                            EnableInfAmmo();
                            std::cout << enableInfammoStr;
                        }
                        else {
                            std::cout << disableInfammoStr;
                            DisableInfAmmo();
                        }
                        ml.infAmmo = !ml.infAmmo;
                    }
                }
            }
                //infinite util
                if (command[i] == 'u')
                {
                    if (al.infUtil)
                    {
                        ml.infUtil = !ml.infUtil;
                        if (ml.infUtil)
                        {
                            std::cout << enableInfutilStr;
                            EnableInfUtil();
                        }
                        else
                        {
                            std::cout << disableInfutilStr;
                            DisableInfUtil();
                        }
                    }
                    else
                    {
                        std::cout << actionUnavailableStr;

                    }
                }
                //infinite electricity
                if (command[i] == 'e')
                {
                    if (!ml.infElec) {
                        ml.infElec = true;
                        StartActionThread();
                        std::cout << enableInfElecStr;

                    }
                    else {
                        std::cout << disableInfElecStr;
                        ml.infElec = false;
                    }
                }
                //infinite fuel
                if (command[i] == 'f')
                {
                    if (!ml.infFuel) {
                        ml.infFuel = true;
                        StartActionThread();
                        std::cout << enableInfFuelStr;

                    }
                    else {
                        std::cout << disableInfFuelStr;
                        ml.infFuel = false;
                    }
                }
            
        }
        return;
    }

    //keep action thread active
    if (command == "ka") 
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
    
    //admin menu
    if (command == "am") 
    {
        if (!ml.forceAdminMenu) {
            ml.forceAdminMenu = true;
            StartActionThread();
            std::cout << enableAdminMenuStr;
            
        }
        else {
            std::cout << disableAdminMenuStr;
            ml.forceAdminMenu = false;
        }
        return;
    }
    
    //show map players
    if (command == "sp" || command == "mp")
    {
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
    if (command == "nc" || command == "noclip") 
    {
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
    if (command == "kt") 
    {
        bPlrScanThread = false;
        bActionThread = false;
        return;
    }

    if (command == "ext")
    {
        if (!verifyPlrObjAddress()) {
            getPlayerObjWhenAvailable();
            printf("%sPlease try again\n", prefix.c_str());
            return;
        }
        else {
            
        }
        GiveItem(0, (void*)"\x0A\x00\x00\x00", (void*)"\x00\x00\x20\x41", (void*)"\x00\x00\x00\x00");
        printf("%sTime to fly\n",prefix.c_str());
        return;
    }

    if (command[0] == 'f' && command[1] == 'l' && command[2] == 'a' && command[3] == 's' && command[4] == 'h')
    {
        if (!verifyPlrObjAddress()) {
            getPlayerObjWhenAvailable();
            printf("%sPlease try again\n", prefix.c_str());
            return;
        }
        std::string a[5];
        splitString(a, command);
        UINT arg = 1;
        if (!a[1].empty()) {
            arg = stoi(a[1]);
        }
        if (arg < 1 || arg > 9) {
            printf("%s\n",invalidArgumentStr.c_str());
            return;
        }
        GiveItem(arg, (void*)"\x0F\x00\x00\x00", (void*)"\x00\x00\xC8\x42", (void*)"\x00\x00\x00\x00");
        printf("%sLet there be light!\n", prefix.c_str());
        return;
    }

    if (command == "reload" || command == "rl")
    {
        if (!verifyPlrObjAddress()) {
            getPlayerObjWhenAvailable();
            printf("%sPlease try again\n", prefix.c_str());
            return;
        }
        std::string a[5];
        splitString(a, command);
        UINT arg = 0;
        if (!a[1].empty()) {
            arg = stoi(a[1]);
        }
        if (arg < 0 || arg > 9) {
            printf("%s\n", invalidArgumentStr.c_str());
            return;
        }
        setChargeAndAmmo(arg, (void*)"\x00\x00\x00\x00", (void*)"\x00\x00\x00\x28");
        printf("%sWhats the point of this?\n", prefix.c_str());
        return;
    }

    if (command == "fix" || command == "fx")
    {
        if (!verifyPlrObjAddress()) {
            getPlayerObjWhenAvailable();
            printf("%sPlease try again\n", prefix.c_str());
            return;
        }
        std::string a[5];
        splitString(a, command);
        UINT arg = 0;
        if (!a[1].empty()) {
            arg = stoi(a[1]);
        }
        if (arg < 0 || arg > 9) {
            printf("%s\n", invalidArgumentStr.c_str());
            return;
        }
        setChargeAndAmmo(arg, (void*)"\x00\x00\x7A\x44", (void*)"\x00\x00\x00\x00");
        printf("%shehe\n", prefix.c_str());
        return;
    }

    if (command == "vd")
    {
        if (!ml.vehDamage) {
            ml.vehDamage = true;
            StartActionThread();
            std::cout << disableVehDmgStr;

        }
        else {
            std::cout << enableVehDmgStr;
            ml.vehDamage = false;
        }
        return;
    }

    //smg 
    if (command == "smg")
    {
        if (verifyPlrObjAddress())
        {
            char* PrimaryItemslotAddr = (char*)PlrObjAddr + 0x268;
            GiveItem(0,(DWORD*)"\x25\x00\x00\x00",(DWORD*)"\x00\x00\x00\x00",(DWORD*)"\x28\x00\x00\x00");
        }
        else
        {
            getPlayerObjWhenAvailable();
            std::cout << invalidPlrObjStr;
        }
        return;
    }
    
    if (command == "gre")
    {
        if (verifyPlrObjAddress())
        {
            char* PrimaryItemslotAddr = (char*)PlrObjAddr + 0x268;
            GiveItem(3, (DWORD*)"\x29\x00\x00\x00", (DWORD*)"\x00\x00\x00\x00", (DWORD*)"\x01\x00\x00\x00");
        }
        else
        {
            getPlayerObjWhenAvailable();
            std::cout << invalidPlrObjStr;
        }
        return;
    }

    //loadout
    if (command == "loa")
    {
        giveLoadout();
        return;
    }

    //help 
    if (cmd == '?' || command == "help")
    {
        printHelpMessage();
        return;
    }
    //exit
    if (cmd == 'x')
    {
        cleanup();
        Sleep(500);
        exit(0);
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
    if (ml.god) 
    {
        std::cout << disableGodmodeStr;
        void* healthDecInstruction = (void*)"\xF3\x0F\x11\x80\xC4\x03\x00\x00";
        PatchEX(hProcess, EnvHealthDecAddr, healthDecInstruction, 8);
        PatchEX(hProcess, PlrHealthDecAddr, healthDecInstruction, 8);
    }
    if (ml.infAmmo)
    {
        std::cout << disableInfammoStr;
        DisableInfAmmo();
    }
    if (ml.infUtil)
    {
        std::cout << disableInfutilStr;
        DisableInfUtil();
    }
    if (bActionThread)
    {
        bActionThread = false;
        Sleep(1000);
    }
    CloseHandle(hProcess);
}

void GiveItem(DWORD itemSlot, void* itemID, void* DWcharge, void* DWammo)
{
    if (verifyPlrObjAddress())
    {
        DWORD* plr = (DWORD*)PlrObjAddr;
        DWORD* slot = plr + 0x95 + 7 * itemSlot;
        DWORD* item = slot + 5;
        DWORD* charge = slot + 6;
        DWORD* ammo = slot + 7;
        PatchEX(hProcess, item, itemID, 4);
        PatchEX(hProcess, charge, DWcharge, 4);
        PatchEX(hProcess, ammo, DWammo, 4);
        //std::cout << ">> Item given!" << "\n";
    }
    else 
    {
        std::cout << invalidPlrObjStr << "\n";
    }

}

void EnableInfAmmo()
{
    NopEX(hProcess, DecPrimarySmgAmmoAddr, 4);
    NopEX(hProcess, DecPrimaryRifleAmmoAddr, 4);
    NopEX(hProcess, DecPistolAmmoAddr, 4);
    NopEX(hProcess, DecC4Addr, 4);
    NopEX(hProcess, DecGrenadeAddr, 4);
}

void DisableInfAmmo()
{
    void* ammoDecInstruction = (void*)"\x41\xFF\x4D\x08";
    void* PisGrenAmmoDecInstruction = (void*)"\x41\x89\x46\x08";
    void* C4DecInstruction = (void*)"\x41\xFF\x4E\x08";
    PatchEX(hProcess, DecPrimarySmgAmmoAddr, ammoDecInstruction, 4);
    PatchEX(hProcess, DecPrimaryRifleAmmoAddr, ammoDecInstruction, 4);
    PatchEX(hProcess, DecPistolAmmoAddr, PisGrenAmmoDecInstruction, 4);
    PatchEX(hProcess, DecC4Addr, C4DecInstruction, 4);
    PatchEX(hProcess, DecGrenadeAddr, PisGrenAmmoDecInstruction, 4);
}

void EnableInfUtil()
{
    void* addIns = (void*)"\xF3\x44\x0F\x58\xC7";
    void* addIns2 = (void*)"\xF3\x0F\x58\xC7";

    PatchEX(hProcess, DecFlashlightAddr, addIns2, 4);
    NopEX(hProcess, DecFlareAddr, 4);
    NopEX(hProcess, DecFlaregunAddr, 4);
    PatchEX(hProcess, DecPrimaryWeldingTorchAddr, addIns, 5);
    PatchEX(hProcess, DecPrimaryFireExtAddr, addIns2, 4);
    NopEX(hProcess, DecMedKitAddr, 4);
}

void DisableInfUtil()
{ 
    void* subIns = (void*)"\xF3\x44\x0F\x5C\xC7";
    void* utilDecInstruction = (void*)"\xF3\x41\x0F\x11\x7D\x04";
    void* FlareDecInstruction = (void*)"\x41\x89\x46\x08";
    void* subIns2 = (void*)"\xF3\x0F\x5C\xC7";
    PatchEX(hProcess, DecFlashlightAddr, subIns2, 4);
    PatchEX(hProcess, DecFlareAddr, FlareDecInstruction, 4);
    PatchEX(hProcess, DecFlaregunAddr, FlareDecInstruction, 4);
    PatchEX(hProcess, DecMedKitAddr, FlareDecInstruction, 4);
    PatchEX(hProcess, DecPrimaryWeldingTorchAddr, subIns, 5);
    PatchEX(hProcess, DecPrimaryFireExtAddr, subIns2, 4);
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


void ActionThread()
{
    bool hasDied = false;
    FLOAT* health;

    while (bActionThread)
    {
        //auto loadout
        if (ml.autoLoadout)
        {
            health = ProtectedFloatRead(hProcess, (char*)PlrObjAddr + 0x3C4, 4);
            std::cout << health << "\n";
            if (health == (void*)0xFFFFFFFF)
            {
                std::cout << invalidPlrObjStr + prefix;
                ml.autoLoadout = false;
                continue;
            }

            if (health == 0 && hasDied == false) {
                hasDied = true;
                std::cout << ">> Detected local player death\n";
            }
            if (hasDied && health == (FLOAT*)0x42C80000)
            {
                std::cout << ">> Detected local player respawn\n";
                hasDied = false;
                giveLoadout();
            }
        }
        //vehicle damage
        if (ml.vehDamage) 
        {
            PatchEX(hProcess, VehDamageAddr, (BYTE*)"\x00", 1);
        }
        //inf ammo
        if (ml.infAmmoG) {
            PatchEX(hProcess, InfAmmoAddr, (BYTE*)"\x01", 1);

        }
        //show map players
        if (ml.mapPlrs) {
            PatchEX(hProcess, MapPlayersAddr, (BYTE*)"\x01", 1);
        }
        //inf electric
        if (ml.infElec) {
            PatchEX(hProcess, InfElecAddr, (BYTE*)"\x01", 1);

        }
        //inf fuel
        if (ml.infFuel) {
            PatchEX(hProcess, InfFuelAddr, (BYTE*)"\x01", 1);

        }
        //noclip
        if (ml.forceNoClip) {
            PatchEX(hProcess, NoClipAddr, (BYTE*)"\x01", 1);
            PatchEX(hProcess, MapTpAddr, (BYTE*)"\x01", 1);
        }
        //Force admin menu
        if (ml.forceAdminMenu) {
            PatchEX(hProcess, VehTpAddr, (BYTE*)"\x01", 1); //VehTp
            PatchEX(hProcess, VehMapAddr, (BYTE*)"\x01", 1); //VehMap
            PatchEX(hProcess, VehCleanAddr, (BYTE*)"\x01", 1);//VehClean
            PatchEX(hProcess, LockSettAddr, (BYTE*)"\x00", 1); //LockSett
        }

        if (!ml.forceNoClip && !ml.forceAdminMenu && !ml.autoLoadout && !ml.mapPlrs &&
            !ml.infElec && !ml.infFuel && !ml.vehDamage &&
            !ml.infAmmoG && !bKeepActive)
        {
            bActionThread = false;
            break;
        }
        Sleep(750);
    }
    //std::cout << stopActionThreadStr + "\n" + prefix;
}

void setChargeAndAmmo(DWORD itemSlot, void* DWcharge, void* DWammo)
{
    if (verifyPlrObjAddress())
    {
        DWORD* plr = (DWORD*)PlrObjAddr;
        DWORD* slot = plr + 0x95 + 7 * itemSlot;
        DWORD* charge = slot + 6;
        DWORD* ammo = slot + 7;
        PatchEX(hProcess, charge, DWcharge, 4);
        PatchEX(hProcess, ammo, DWammo, 4);
        //std::cout << ">> Item given!" << "\n";
    }
    else
    {
        std::cout << invalidPlrObjStr << "\n";
    }

}


void giveLoadout()
{
    if (verifyPlrObjAddress())
    {
        char* PrimaryItemslotAddr = (char*)PlrObjAddr + 0x268;
        GiveItem( 0, (DWORD*)"\x25\x00\x00\x00", (DWORD*)"\x00\x00\x00\x00", (DWORD*)"\x28\x00\x00\x00"); //smg
        GiveItem( 1, (DWORD*)"\x29\x00\x00\x00", (DWORD*)"\x00\x00\x00\x00", (DWORD*)"\x01\x00\x00\x00"); //grenade
        GiveItem( 2, (DWORD*)"\x0D\x00\x00\x00", (DWORD*)"\x00\x00\x00\x00", (DWORD*)"\x01\x00\x00\x00"); //flaregun
        GiveItem( 3, (DWORD*)"\x1F\x00\x00\x00", (DWORD*)"\x00\x00\x00\x00", (DWORD*)"\x01\x00\x00\x00"); //c4
        GiveItem( 4, (DWORD*)"\x20\x00\x00\x00", (DWORD*)"\x00\x00\x00\x00", (DWORD*)"\x00\x00\x00\x00"); //c4 det
        GiveItem( 6, (DWORD*)"\x0B\x00\x00\x00", (FLOAT*)"\x00\x00\x00\x00", (DWORD*)"\x04\x00\x00\x00"); //med kit
        GiveItem( 7, (DWORD*)"\x0F\x00\x00\x00", (FLOAT*)"\x00\x00\xC8\x42", (DWORD*)"\x00\x00\x00\x00"); //flashlight
        GiveItem( 8, (DWORD*)"\x06\x00\x00\x00", (DWORD*)"\x00\x00\x00\x00", (DWORD*)"\x00\x00\x00\x00"); //binoculars
        GiveItem( 9, (DWORD*)"\x05\x00\x00\x00", (DWORD*)"\x00\x00\x00\x00", (DWORD*)"\x00\x00\x00\x00"); //artic
    }
    else
    {
        std::cout << " \n" << invalidPlrObjStr;
        al.playerobj = false;
        getPlayerObjWhenAvailable();
    }
}

void getPlayerObjWhenAvailable() {
    tryGetPlrObj();
    if (verifyPlrObjAddress()) {
        printf("%sPlayer object found: 0x%p\n", prefix.c_str(), PlrObjAddr);
    }
    else {
        std::thread tPlrObjWait(waitForPlrObj);
        tPlrObjWait.detach();
    }
}

std::vector<std::string> splitStringBySpace(std::string str)
{
    std::string word = "";
    std::vector<std::string> v;
    unsigned int pos = 0;
    for (auto x : str)
    {
        if (x == ' ')
        {
            //std::cout << word << std::endl;
            v.insert(v.begin() + pos, word);
            word = "";
            pos++;
        }
        else {
            word = word + x;
        }
    }
    return v;
    //std::cout << word << std::endl;
}

void waitForPlrObj() {
    bPlrScanThread = true;
    unsigned int c = 1;
    do {
        if (c % 60 == 0) {
            printf("Plr obj not found in %u scans, possibly idle.\n%s",c, prefix.c_str());
        }
        tryGetPlrObj();
        c++;
        Sleep(2000);
    } while (!verifyPlrObjAddress() && bPlrScanThread);
    PlrSlotAddr = (char*)PlrObjAddr + 0x240;
    printf("Player object found after %u scans\n%s", c, prefix.c_str());
}

void tryGetPlrObj() {
    uintptr_t plrPtrChainBase = (uintptr_t)Module.modBaseAddr + 0xBC9770;
    PlrObjAddr = (BYTE*)FindDMAAddy(hProcess, plrPtrChainBase, { 0x368, 0x4D0, 0xC08, 0x9A8, 0x98, 0x0 });
}

bool verifyPlrObjAddress()
{
    if (PlrObjAddr == NULL) { return false; }
    al.playerobj = (DWORD)ProtectedRead(hProcess, PlrObjAddr, 4) == 4;
    return al.playerobj;
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


std::string ieee_float_to_hex(float f)
{
    static_assert(std::numeric_limits<float>::is_iec559,
        "native float must be an IEEE float");

    union { float fval; std::uint32_t ival; };
    fval = f;

    std::ostringstream stm;
    stm << std::hex << std::uppercase << ival;

    return stm.str();
}






