
#include "swmod.h"
#include <iostream>
#include "windows.h"
#include <psapi.h>
#include "mem.h"
#include "proctools.h"
#include "patternscan.h"
#include <thread>
#include "strings.h"
#include "items.h"
#include <Psapi.h>




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
void* RifleNoSpreadAddr;
void* RifleRapidFireAddr;
void* RifleProjIDAddr;
void* PlrObjAddr;
void* PlrSlotAddr;
char* MAXSEARCHADDR = (char*)0xFFFFFF00;
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
        LockSettAddr = (BYTE*)Module.modBaseAddr + 0xBD7EE2;
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
    DecPrimarySmgAmmoAddr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\x41\xFF\x4D\x08\xC7\x85\x68\x02\x00\x00\x33\x33\xB3\x3E", (char*)"xxxxxxxxxxxxxx", (char*)"DecPrimarySmgAmmoAddr");
    DecPrimaryRifleAmmoAddr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\x41\xFF\x4D\x08\xC7\x85\x74\x02\x00\x00\x66\x66\xE6\x3E", (char*)"xxxxxxxxxxxxxx", (char*)"DecPrimaryRifleAmmoAddr");
    DecPistolAmmoAddr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\xFF\xC8\x41\x89\x46\x08\x8B\x44\x24\x68", (char*)"xxxxxxxxxx", (char*)"DecPistolAmmoAddr");
    DecC4Addr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\x41\xFF\x4E\x08\xE9\xDA\xED\xFF\xFF", (char*)"xxxxxxxxx", (char*)"DecC4Addr");
    DecGrenadeAddr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\x41\x89\x46\x08\x40\xB7\x01\x45\x38\xBD", (char*)"xxxxxxxxxx", (char*)"DecGrenadeAddr");
    DecPrimaryWeldingTorchAddr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\xF3\x44\x0F\x5C\xC7\x41\x0F\x28\xFA\xF3\x41\x0F\x5F\xF8", (char*)"xxxxxxxxxxxxxx", (char*)"DecPrimaryWeldingTorchAddr");
    DecFlaregunAddr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\xFF\xC8\x41\x89\x46\x08\x45\x38\xBD\x08\x24\x00\x00\x0F\x84\xC8", (char*)"xxxxxxxxxxxxxxxx", (char*)"DecFlaregunAddr");
    DecFlareAddr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\x41\x89\x46\x08\x45\x38\xBD\x08\x24\x00\x00\x0F\x84\xFB\xFC\xFF\xFF", (char*)"xxxxxxxxxxxxxxxxx", (char*)"DecFlareAddr");
    DecMedKitAddr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\x41\x89\x46\x08\x48\x8B\xBD\xF0", (char*)"xxxxxxxx", (char*)"DecMedKitAddr");
    DecPrimaryFireExtAddr = (char*)PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\x41\x80\xBE\x08\x24\x00\x00\x00\x74\x23", (char*)"xxxxxxxxxx", (char*)"DecFireExtIDPatternAddr") + 0x10;
    RifleNoSpreadAddr = (char*)PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\xF3\x0F\x59\x15\xBE\x51\x41", (char*)"xxxxxxx", (char*)"RifleNoSpreadIDPatternAddr") + 0xD;
    RifleProjIDAddr = (char*)PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\xC7\x44\x24\x38\x05\x00\x00\x00\xF3\x0F", (char*)"xxxxxxxxxx", (char*)"RifleProjIDAddr") + 4;
    //RifleNoSpreadAddr = Module.modBaseAddr + 0x688F8B; you also got to add the offset to .text to use this
    DecFlashlightAddr = (char*)PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\x0F\x84\x60\xF6\xFF\xFF\xF3\x41\x0F", (char*)"xxxxxxxxx", (char*)"FlashlightIDPatternAddr") + 0xC;
    RifleRapidFireAddr = (char*)DecPrimaryRifleAmmoAddr - 4;
    printf("%sRifleProjIDAddr = 0x%p\n", prefix.c_str(), RifleProjIDAddr);
    printf("%sRifleNoSpreadAddr = 0x%p\n", prefix.c_str(), RifleNoSpreadAddr);
    printf("%sDecFlashlightAddr = 0x%p\n", prefix.c_str(), DecFlashlightAddr);
    printf("%sDecFireExtAddr = 0x%p\n", prefix.c_str(), DecPrimaryFireExtAddr);
    if (!SetConsoleCtrlHandler(HandlerRoutine, TRUE))
    {
        std::cout << failedSetCH;
    }

    if (RifleProjIDAddr != (void*)4 ) {
        al.projID = true;
    }

    if (RifleNoSpreadAddr != NULL ) {
        al.noSpread = true;
    }

    if (RifleRapidFireAddr != NULL ) {
        al.rapidFire = true;
    }

    if (EnvHealthDecAddr != NULL
        && PlrHealthDecAddr != NULL)
    {
        al.god = true;
    }

    if (
       DecGrenadeAddr != NULL &&
       DecC4Addr != NULL  &&
       DecPrimarySmgAmmoAddr != NULL  &&
       DecPistolAmmoAddr != NULL &&
       DecPrimaryRifleAmmoAddr != NULL)
    {
        al.infAmmo = true; 
    }

    if (DecPrimaryWeldingTorchAddr != NULL &&
        DecMedKitAddr != NULL  &&
        DecFlashlightAddr != NULL  &&
        DecPrimaryFireExtAddr != NULL  &&
        DecFlareAddr != NULL && 
        DecFlaregunAddr != NULL)
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

void ProcessCommand(std::string cmd)
 
{
    std::string command[8];
    splitString(command, cmd);

    //version
    if (command[0] == "version" || command[0] == "ver")
    {
        std::cout << MODVERSION;
        return;
    }
    
    //auto loadout
    if (command[0] == "al")
    {
        std::cout << actionUnavailableStr; //need to fix this
        return;

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
    if (command[0] == "g" || command[0] == "god")
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

    if (command[0] == "heal") {
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

    if (command[0] == "die" || command[0] == "kill") {
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

    if (command[0] == "ps") { //player scanner
        if (!verifyPlrObjAddress()) {
            printf("%sStarting player object scanner\n", prefix.c_str());
            getPlayerObjWhenAvailable();
        }
        else {
            printf("%sPlayer object has already been found, if this is not true, use \"flushplr\" command.\n",prefix.c_str());
        }
        return;
    }

    if (command[0] == "flushplr") {
        PlrObjAddr = nullptr;
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
                if (cmd[i] == 'u')
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
                if (cmd[i] == 'e')
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
                if (cmd[i] == 'f')
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
    
    //admin menu
    if (command[0] == "am")
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
    if (command[0] == "sp" || command[0] == "mp")
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
    if (command[0] == "nc" || command[0] == "noclip")
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
    if (command[0] == "kt")
    {
        bPlrScanThread = false;
        bActionThread = false;
        return;
    }

    //gives fire ext
    if (command[0] == "ext")
    {
        if (!verifyPlrObjAddress()) {
            getPlayerObjWhenAvailable();
            printf("%sPlease try again\n", prefix.c_str());
            return;
        }
        else {
            
        }
        GiveItem(0, SWITEM::fire_extID, 100, 0);
        printf("%sTime to fly\n",prefix.c_str());
        return;
    }

    //give command
    if (command[0] == "give") {
        
        if (!verifyPlrObjAddress()) {
            getPlayerObjWhenAvailable();
            printf("%sPlease try again\n", prefix.c_str());
            return;
        }
        if (command[1].empty() || command[2].empty()) {
            printf("%s\n",invalidArgumentStr.c_str());
            return;
        }

        UINT item = stoi(command[1]);
        UINT slot = stoi(command[2]);
        if (slot < 0 || slot > 10) {
            printf("%s\n", invalidArgumentStr.c_str());
            return;
        }
        GiveItem(slot,item, 100, 100);
        return;
    }



    //gives flashlight
    if (command[0] == "flash" || command[0] == "flashlight")
    {
        if (!verifyPlrObjAddress()) {
            getPlayerObjWhenAvailable();
            printf("%sPlease try again\n", prefix.c_str());
            return;
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
        GiveItem(arg, SWITEM::flashlightID, 100, 0);
        printf("%sLet there be light!\n", prefix.c_str());
        return;
    }

    //reloads primary
    if (command[0] == "reload" || command[0] == "rl")
    {
        if (!verifyPlrObjAddress()) {
            getPlayerObjWhenAvailable();
            printf("%sPlease try again\n", prefix.c_str());
            return;
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
        if (!verifyPlrObjAddress()) {
            getPlayerObjWhenAvailable();
            printf("%sPlease try again\n", prefix.c_str());
            return;
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

    if (command[0] == "projid") {
        if (!al.projID) {
            printf("%s", actionUnavailableStr.c_str());
            return;
        }
        if (command[1].empty() || stoi(command[1]) < 1) {
            printf("%s", invalidArgumentStr.c_str());
            return;
        }
        ml.projidchanged = true;
        changeProjId(stoi(command[1]));
        printf("%sUpdated rifle projectile id.\n",prefix.c_str());
        return;
    }


    if (command[0] == "ns" || command[0] == "nospread") {
        if (!al.noSpread) {
            printf("%s",actionUnavailableStr.c_str());
            return;
        }
        if (!ml.noSpread) {
            EnableNoSpread();
            printf("%s",enableNoSpreadStr.c_str());
        }
        else {
            DisableNoSpread();
            printf("%s", disableNoSpreadStr.c_str());
        }
        ml.noSpread = !ml.noSpread;
        return;
    }

    if (command[0] == "rf" || command[0] == "rapidfire") {
        if (!al.rapidFire) {
            printf("%s", actionUnavailableStr.c_str());
            return;
        }
        if (!ml.rapidFire) {
            EnableRapidFire();
            printf("%s", enableRapidFireStr.c_str());
        }
        else {
            DisableRapidFire();
            printf("%s", disableRapidFireStr.c_str());
        }
        ml.rapidFire = !ml.rapidFire;
        return;
    }

    if (command[0] == "vd")
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

    //rifle
    if (command[0] == "rifle")
    {
        if (verifyPlrObjAddress())
        {
            char* PrimaryItemslotAddr = (char*)PlrObjAddr + 0x268;
            GiveItem(0,SWITEM::rifleID,0,30);
        }
        else
        {
            getPlayerObjWhenAvailable();
            std::cout << invalidPlrObjStr;
        }
        return;
    }
    
    if (command[0] == "gre")
    {
        if (verifyPlrObjAddress())
        {
            char* PrimaryItemslotAddr = (char*)PlrObjAddr + 0x268;
            GiveItem(3, SWITEM::grenadeID, 0, 1);
        }
        else
        {
            getPlayerObjWhenAvailable();
            std::cout << invalidPlrObjStr;
        }
        return;
    }

    //loadout
    if (command[0] == "loa")
    {
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
    
    if (ml.projidchanged) {
        changeProjId(5);
    }
    if (ml.god) 
    {
        std::cout << disableGodmodeStr;
        void* healthDecInstruction = (void*)"\xF3\x0F\x11\x80\xC4\x03\x00\x00";
        PatchEX(hProcess, EnvHealthDecAddr, healthDecInstruction, 8);
        PatchEX(hProcess, PlrHealthDecAddr, healthDecInstruction, 8);
    }
    if (ml.noSpread) {
        DisableNoSpread();
        printf("%s",disableNoSpreadStr.c_str());
    }
    if (ml.rapidFire) {
        DisableRapidFire();
        printf("%s", disableRapidFireStr.c_str());
    }
    if (ml.infAmmo)
    {
        DisableInfAmmo();
        printf("%s", disableInfammoStr.c_str());
    }
    if (ml.infUtil)
    {
        DisableInfUtil();
        printf("%s", disableInfutilStr.c_str());
    }
    if (bActionThread)
    {
        bActionThread = false;
        printf("%sShutting down action thread.\n",prefix.c_str());
        Sleep(1000);
    }
    CloseHandle(hProcess);
}

void GiveItem(DWORD itemSlot, int itemID, float charge, int ammo)
{
    if (!verifyPlrObjAddress())
    {
        printf("%s",invalidPlrObjStr.c_str());
        return;
    }
        
        DWORD* plrAddr = (DWORD*)PlrObjAddr;
        DWORD* slotAddr = plrAddr + 0x95 + 7 * itemSlot;
        DWORD* itemAddr = slotAddr + 5;
        DWORD* chargeAddr = slotAddr + 6;
        DWORD* ammoAddr = slotAddr + 7;
        PatchEX(hProcess, itemAddr, reinterpret_cast<int*>(&itemID), 4);
        PatchEX(hProcess, chargeAddr, reinterpret_cast<int*>(&charge), 4);
        PatchEX(hProcess, ammoAddr, reinterpret_cast<int*>(&ammo), 4);
        //std::cout << ">> Item given!" << "\n";
    

}

void EnableInfAmmo()
{
    NopEX(hProcess, DecPrimarySmgAmmoAddr, 4);
    NopEX(hProcess, DecPrimaryRifleAmmoAddr, 4);
    NopEX(hProcess, DecPistolAmmoAddr, 2);
    NopEX(hProcess, DecC4Addr, 4);
    NopEX(hProcess, DecGrenadeAddr, 4);
}

void EnableRapidFire() {
    int i = 0;
    PatchEX(hProcess, RifleRapidFireAddr, reinterpret_cast<int*>(&i), 4);
}

void DisableRapidFire() {
    int i = 7;
    PatchEX(hProcess, RifleRapidFireAddr, reinterpret_cast<int*>(&i), 4);
}

void EnableNoSpread() {
    void* patch = (void*)"\x69\xC1\x00\x00\x00\x00";
    PatchEX(hProcess, RifleNoSpreadAddr, patch, 6);
}

void DisableNoSpread() {
    void* patch = (void*)"\x69\xC1\xFD\x43\x03\x00";
    PatchEX(hProcess, RifleNoSpreadAddr, patch, 6);
}


void DisableInfAmmo()
{
    void* ammoDecInstruction = (void*)"\x41\xFF\x4D\x08";
    void* PisGrenAmmoDecInstruction = (void*)"\x41\x89\x46\x08";
    void* C4DecInstruction = (void*)"\x41\xFF\x4E\x08";
    void* pistoldec = (void*)"\xFF\xC8";
    PatchEX(hProcess, DecPrimarySmgAmmoAddr, ammoDecInstruction, 4);
    PatchEX(hProcess, DecPrimaryRifleAmmoAddr, ammoDecInstruction, 4);
    PatchEX(hProcess, DecPistolAmmoAddr, pistoldec, 2);
    PatchEX(hProcess, DecC4Addr, C4DecInstruction, 4);
    PatchEX(hProcess, DecGrenadeAddr, PisGrenAmmoDecInstruction, 4);
}

void EnableInfUtil()
{
    void* addIns = (void*)"\xF3\x44\x0F\x58\xC7";
    void* addIns2 = (void*)"\xF3\x0F\x58\xC7";

    PatchEX(hProcess, DecFlashlightAddr, addIns2, 4);
    NopEX(hProcess, DecFlareAddr, 4);
    NopEX(hProcess, DecFlaregunAddr, 2);
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
    void* FGdecIns = (void*)"\xFF\xC8";
    PatchEX(hProcess, DecFlashlightAddr, subIns2, 4);
    PatchEX(hProcess, DecFlareAddr, FlareDecInstruction, 4);
    PatchEX(hProcess, DecFlaregunAddr, FGdecIns, 2);
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

void setCharge(UINT itemSlot, float charge)
{

    if (!verifyPlrObjAddress())
    {
        printf("%s\n", prefix.c_str());
        return;
    }
    DWORD* plrAddr = (DWORD*)PlrObjAddr;
    DWORD* slotAddr = plrAddr + 0x95 + 7 * itemSlot;
    DWORD* ammoAddr = slotAddr + 6;
    PatchEX(hProcess, ammoAddr, reinterpret_cast<int*>(&charge), 4);
}

void setAmmo(UINT itemSlot, int ammo)
{

    if (!verifyPlrObjAddress())
    {
        printf("%s\n",prefix.c_str());
        return;
    }
        DWORD* plrAddr = (DWORD*)PlrObjAddr;
        DWORD* slotAddr = plrAddr + 0x95 + 7 * itemSlot;
        DWORD* ammoAddr = slotAddr + 7;
        PatchEX(hProcess, ammoAddr, reinterpret_cast<int*>(&ammo), 4);
}

void changeProjId(UINT projID) {
    PatchEX(hProcess, RifleProjIDAddr, reinterpret_cast<int*>(&projID), 4);
}


void giveLoadout()
{
    if (verifyPlrObjAddress())
    {
        char* PrimaryItemslotAddr = (char*)PlrObjAddr + 0x268;
        GiveItem(0, SWITEM::smgID, 0, 40); 
        GiveItem(1, SWITEM::flashlightID, 100, 0);
        GiveItem(2, SWITEM::medkitID, 0, 4);
        GiveItem(3, SWITEM::C4ID, 0, 1); 
        GiveItem(4, SWITEM::C4_detID, 0, 0); 
        GiveItem(7, SWITEM::grenadeID, 0, 1);
        GiveItem(8, SWITEM::binocularsID, 0, 0); 
        GiveItem(9, SWITEM::arcticID, 0, 0);
    }
    else
    {
        std::cout << " \n" << invalidPlrObjStr;
        al.playerobj = false;
        getPlayerObjWhenAvailable();
    }
}

void getPlayerObjWhenAvailable() {
    PlrObjAddr = tryGetPlrObj();
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
        PlrObjAddr = tryGetPlrObj();
        c++;
        Sleep(2000);
    } while (!verifyPlrObjAddress() && bPlrScanThread);
    PlrSlotAddr = (char*)PlrObjAddr + 0x240;
    printf("Player object found after %u scans\n%s", c, prefix.c_str());
}

BYTE* tryGetPlrObj() {
    uintptr_t plrPtrChainBase = (uintptr_t)Module.modBaseAddr + 0xBEBC68;
    return (BYTE*)FindDMAAddy(hProcess, plrPtrChainBase, { 0x1F0, 0x20, 0x270, 0x50, 0x48, 0x370, 0x0 });
}

bool verifyPlrObjAddress()
{
    if (PlrObjAddr == NULL) { return false; }
    al.playerobj = PlrObjAddr == tryGetPlrObj() && (DWORD)ProtectedRead(hProcess, PlrObjAddr, 4) == 4;
    //al.playerobj = (DWORD)ProtectedRead(hProcess, PlrObjAddr, 4) == 4;
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








