// swmod.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
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
void* DecUWWeldingTorchAddr;
void* DecFlaregunAddr;
void* DecFlareAddr;
void* DecMedKitAddr;
void* DecFlashlightAddr;
void* PlrObjAddr;
char* MAXSEARCHADDR = (char*)0xFFFFFFFFFFFF0000;
MODULEENTRY32 Module;
HANDLE hProcess;
std::thread tAutoloadout;
bool bActionThread = false;
bool bKeepActive = false;

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
    MSG msg = { };
    DWORD processID = getProcID((wchar_t *)L"stormworks64.exe");
    
    if (processID != 0)
    {
        Module = getModule(processID, (wchar_t*)(L"stormworks64.exe"));
        NoClipAddr = (BYTE*)Module.modBaseAddr + 0xBD3D88;
        VehTpAddr = (BYTE*)Module.modBaseAddr + 0xBD3DA0;
        VehMapAddr = (BYTE*)Module.modBaseAddr + 0xBD3DA9;
        VehCleanAddr = (BYTE*)Module.modBaseAddr + 0xBD3D8D;
        LockSettAddr = (BYTE*)Module.modBaseAddr + 0xBD3D82;
        MapTpAddr = (BYTE*)Module.modBaseAddr + 0xBD3D89;
        hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, processID);
    }
    else {
        std::cout << exeNotFound;
        Sleep(500);
        return -1;
    }
    
    EnvHealthDecAddr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char *)"\xF3\x0F\x11\x80\xC4\x03\x00\x00\x48\x8B\x87\x58\x02\x00\x00\xF3\x45\x0F\x59\xD7", (char*)"xxxxxxxxxxxxxxxxxxxx", (char*)"EnvHealthDecAddr");
    PlrHealthDecAddr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\xF3\x0F\x11\x80\xC4\x03\x00\x00\xF3\x44\x0F\x5C\xA7\xE8\x06\x00\x00\xF3\x44\x0F\x11\x64\x24\x68", (char*)"xxxxxxxxxxxxxxxxxxxxxxxx",(char*)"PlrHealthDecAddr");
    DecPrimarySmgAmmoAddr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\x41\xFF\x4D\x08\xC7\x85\x38\x02\x00\x00\x33\x33\xB3\x3E\x41\x0F\x28\xC2", (char*)"xxxxxxxxxxxxxxxxxx", (char*)"DecPrimarySmgAmmoAddr");
    DecPrimaryRifleAmmoAddr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\x41\xFF\x4D\x08\xC7\x85\x44\x02\x00\x00\x66\x66\xE6\x3E\x41\x0F\x28\xC2", (char*)"xxxxxxxxxxxxxxxxxx", (char*)"DecPrimaryRifleAmmoAddr");
    DecPistolAmmoAddr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\x41\x89\x46\x08\x8B\x44\x24\x48\x89\x44\x24\x68", (char*)"xxxxxxxxxxxx", (char*)"DecPistolAmmoAddr");
    DecC4Addr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\x41\xFF\x4E\x08\xE9\x27\xEE\xFF\xFF\xE8\x0F\xAF\xB8\xFF", (char*)"xxxxxxxxxxxxxx", (char*)"DecC4Addr");
    DecGrenadeAddr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\x41\x89\x46\x08\x40\xB7\x01\x45\x38\xBD\x08\x24\x00\x00", (char*)"xxxxxxxxxxxxxx", (char*)"DecGrenadeAddr");
    DecPrimaryWeldingTorchAddr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\xF3\x41\x0F\x11\x7D\x04\x44\x0F\x28\xC7\x0F\x28\xC7", (char*)"xxxxxxxxxxxxx", (char*)"DecPrimaryWeldingTorchAddr");
    DecPrimaryFireExtAddr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\xF3\x41\x0F\x11\x4D\x04\x44\x0F\x2F\xD1\x72\x05\x41\xC6\x45\x0C\x00\xC6\x45\x89\x01\x49\x8D\x96\x30\x1F\x00\x00", (char*)"xxxxxxxxxxxxxxxxxxxxxxxxxxxx", (char*)"DecPrimaryFireExtAddr");
    DecUWWeldingTorchAddr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\xF3\x41\x0F\x11\x7D\x04\x44\x0F\x28\xC7\x0F\x28\xC7", (char*)"xxxxxxxxxxxxx", (char*)"DecUWWeldingTorchAddr");
    DecFlaregunAddr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\x41\x89\x46\x08\x45\x38\xBD\x08\x24\x00\x00\x0F\x84\xC6\xFA\xFF\xFF", (char*)"xxxxxxxxxxxxxxxxx", (char*)"DecFlaregunAddr");
    DecFlareAddr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\x41\x89\x46\x08\x45\x38\xBD\x08\x24\x00\x00\x0F\x84\xFB\xFC\xFF\xFF", (char*)"xxxxxxxxxxxxxxxxx", (char*)"DecFlareAddr");
    DecMedKitAddr = PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\x41\x89\x46\x08\x48\x8B\xBD\x00\x01\x00\x00\x8B\x9E\x98\x00\x00\x00", (char*)"xxxxxxxxxxxxxxxxx", (char*)"DecMedKitAddr");
    DecFlashlightAddr = (char*)PatternScanExModule(hProcess, (wchar_t*)L"stormworks64.exe", (wchar_t*)L"stormworks64.exe", (char*)"\x0F\x84\xB0\x00\x00\x00\x48\x8B\x7D\x08\x0F\xB6\x45\xAC", (char*)"xxxxxxxxxxxxxx", (char*)"FlashlightIDPatternAddr") - 0x33;
    std::cout << ">> DecFlashlightAddr = 0x" << DecFlashlightAddr << "\n";

    if (!SetConsoleCtrlHandler(HandlerRoutine, TRUE))
    {
        std::cout << failedSetCH;
    }

    if (EnvHealthDecAddr != NULL && EnvHealthDecAddr < MAXSEARCHADDR
        && PlrHealthDecAddr != NULL && PlrHealthDecAddr < MAXSEARCHADDR)
    {
        al.god = true;
    }

    if (DecPrimarySmgAmmoAddr != NULL && DecPrimarySmgAmmoAddr < MAXSEARCHADDR
        && DecPrimaryRifleAmmoAddr != NULL && DecPrimaryRifleAmmoAddr < MAXSEARCHADDR
        && DecPistolAmmoAddr != NULL && DecPistolAmmoAddr < MAXSEARCHADDR
        && DecC4Addr != NULL  && DecC4Addr < MAXSEARCHADDR
        && DecGrenadeAddr != NULL && DecGrenadeAddr < MAXSEARCHADDR)
    {
        al.infAmmo = true; 
    }

    if (DecPrimaryWeldingTorchAddr != NULL && DecPrimaryWeldingTorchAddr < MAXSEARCHADDR
        && DecPrimaryFireExtAddr != NULL && DecPrimaryFireExtAddr < MAXSEARCHADDR
        && DecUWWeldingTorchAddr != NULL && DecUWWeldingTorchAddr < MAXSEARCHADDR
        && DecFlaregunAddr != NULL && DecFlaregunAddr < MAXSEARCHADDR
        && DecFlareAddr != NULL && DecFlareAddr < MAXSEARCHADDR
        && DecMedKitAddr != NULL && DecMedKitAddr < MAXSEARCHADDR
        && DecFlashlightAddr != NULL && DecFlashlightAddr < MAXSEARCHADDR)
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
    std::cin >> cmd;
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

    if (command == "version" || command == "ver")
    {
        std::cout << MODVERSION;
        return;
    }
    
    if (command == "al")
    {
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

    //infinite page
    if (cmd == 'i')
    {
        if (command[1] == NULL)
        {
            std::cout << invalidArgumentStr;
            return;
        }
        if (command[1] == 'a' || command[2] == 'a')
        {
            if (al.infAmmo)
            {
                ml.infAmmo = !ml.infAmmo;
                if (ml.infAmmo)
                {
                    EnableInfAmmo();
                    std::cout << enableInfammoStr;
                }
                else
                {
                    DisableInfAmmo();
                    std::cout << disableInfammoStr;
                }
            }
            else
            {
                std::cout << actionUnavailableStr;
            }
        }
        if (command[1] == 'u' || command[2] == 'u')
        {
            if (al.infUtil)
            {
                ml.infUtil = !ml.infUtil;
                if (ml.infUtil)
                {
                    EnableInfUtil();
                    std::cout << enableInfutilStr;
                }
                else
                {
                    DisableInfUtil();
                    std::cout << disableInfutilStr;
                }
            }
            else
            {
                std::cout << actionUnavailableStr;
              
            }
            return;    
        }
        return;
    }

    if (command == "ka") //keep active
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


    if (command == "set")
    {
        std::string internalCommand;
        //void* plr;
        //plr = getPlayerObjectFromFlashlight(hProcess);
        //std::cout << ">> auto plr: 0x" << plr << "\n";
        std::cout << ">> Varible to change: ";
        std::cin >> internalCommand;
        if (internalCommand == "plr")
        {
            std::cout << ">> Enter base player object address: 0x";
            std::cin >> PlrObjAddr;

            if (PlrObjAddr != NULL && verifyPlrObjAddress())
            {
                std::cout << ">> PlrObjAddr: 0x" << PlrObjAddr << "\n";
                al.playerobj = true;
            }
            else
            {
                std::cout << failVerifyAddressStr;
            }
            return;
        }
        return;
    }

    if (command == "am")
    {
        if (!ml.forceCMenu) {
            ml.forceCMenu = true;
            StartActionThread();
            std::cout << enableCMenuStr;
            
        }
        else {
            std::cout << disableCMenuStr;
            ml.forceCMenu = false;
        }
        return;
    }
  

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

    if (command == "kt")
    {
        bActionThread = false;
        return;
    }


    if (command == "smg")
    {
        if (al.playerobj && verifyPlrObjAddress())
        {
            char* PrimaryItemslotAddr = (char*)PlrObjAddr + 0x268;
            GiveItem(PlrObjAddr, 0,(DWORD*)"\x25\x00\x00\x00",(DWORD*)"\x00\x00\x00\x00",(DWORD*)"\x28\x00\x00\x00");
        }
        else
        {
            std::cout << invalidPlrObjStr;
        }
        return;
    }
    
    if (command == "gre")
    {
        if (al.playerobj && verifyPlrObjAddress())
        {
            char* PrimaryItemslotAddr = (char*)PlrObjAddr + 0x268;
            GiveItem(PlrObjAddr, 3, (DWORD*)"\x29\x00\x00\x00", (DWORD*)"\x00\x00\x00\x00", (DWORD*)"\x01\x00\x00\x00");
        }
        else
        {
            std::cout << invalidPlrObjStr;
        }
        return;
    }

    if (command == "loa")
    {
        giveLoadout();
        return;
    }


    if (cmd == '?' || command == "help")
    {
        printHelpMessage();
        return;
    }
    if (cmd == 'x')
    {
        cleanup();
        Sleep(500);
        exit(0);
    }
    std::cout << "Invalid command! (ignore this if it wasnt)\n";
}

void cleanup()
{
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

void GiveItem(void* PlrObjAddr, DWORD itemSlot, void* itemID, void* DWcharge, void* DWammo)
{
    if (al.playerobj && verifyPlrObjAddress())
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
    NopEX(hProcess, DecFlashlightAddr, 6);
    NopEX(hProcess, DecFlareAddr, 4);
    NopEX(hProcess, DecFlaregunAddr, 4);
    NopEX(hProcess, DecUWWeldingTorchAddr, 6);
    NopEX(hProcess, DecPrimaryWeldingTorchAddr, 6);
    NopEX(hProcess, DecPrimaryFireExtAddr, 6);
    NopEX(hProcess, DecMedKitAddr, 4);
}

void DisableInfUtil()
{ 
    void* utilDecInstruction = (void*)"\xF3\x41\x0F\x11\x7D\x04";
    void* FlareDecInstruction = (void*)"\x41\x89\x46\x08";
    void* FireExtDecInstruction = (void*)"\xF3\x41\x0F\x11\x4D\x04";
    void* FlashlightDecInstruction = (void*)"\xF3\x41\x0F\x11\x4D\x04";
    PatchEX(hProcess, DecFlashlightAddr, FlashlightDecInstruction, 6);
    PatchEX(hProcess, DecFlareAddr, FlareDecInstruction, 4);
    PatchEX(hProcess, DecFlaregunAddr, FlareDecInstruction, 4);
    PatchEX(hProcess, DecMedKitAddr, FlareDecInstruction, 4);
    PatchEX(hProcess, DecUWWeldingTorchAddr, utilDecInstruction, 6);
    PatchEX(hProcess, DecPrimaryWeldingTorchAddr, utilDecInstruction, 6);
    PatchEX(hProcess, DecPrimaryFireExtAddr, FireExtDecInstruction, 6);
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
            if (!verifyPlrObjAddress())
            {
                std::cout << invalidPlrObjStr << ">> ";
                ml.autoLoadout = false;
                continue;
            }

            health = ProtectedFloatRead(hProcess, (char*)PlrObjAddr + 0x3C4, 4);
            if (health == (void*)0xFFFFFFFF)
            {
                std::cout << invalidPlrObjStr + prefix;
                ml.autoLoadout = false;
                continue;
            }

            if (health == 0 && hasDied == false) {
                hasDied = true;
                //std::cout << ">> Detected local player death\n";
            }
            if (hasDied && health == (FLOAT*)0x42C80000)
            {
                //std::cout << ">> Detected local player respawn\n";
                hasDied = false;
                giveLoadout();
            }
        }
        //noclip
        if (ml.forceNoClip) {
            PatchEX(hProcess, NoClipAddr, (BYTE*)"\x01", 1);
            PatchEX(hProcess, MapTpAddr, (BYTE*)"\x01", 1);

        }
        //Force admin menu
        if (ml.forceCMenu) {
            PatchEX(hProcess, VehTpAddr, (BYTE*)"\x01", 1); //VehTp
            PatchEX(hProcess, VehMapAddr, (BYTE*)"\x01", 1); //VehMap
            PatchEX(hProcess, VehCleanAddr, (BYTE*)"\x01", 1);//VehClean
            PatchEX(hProcess, LockSettAddr, (BYTE*)"\x00", 1); //LockSett

        }

        if (!ml.forceNoClip && !ml.forceCMenu && !ml.autoLoadout && !bKeepActive)
        {
            bActionThread = false;
            break;
        }
        Sleep(1000);
    }
    //std::cout << stopActionThreadStr + "\n" + prefix;
}



void giveLoadout()
{
    if (al.playerobj && (DWORD)ProtectedRead(hProcess, PlrObjAddr, 4) == 4)
    {
        char* PrimaryItemslotAddr = (char*)PlrObjAddr + 0x268;
        GiveItem(PlrObjAddr, 0, (DWORD*)"\x25\x00\x00\x00", (DWORD*)"\x00\x00\x00\x00", (DWORD*)"\x28\x00\x00\x00"); //smg
        GiveItem(PlrObjAddr, 1, (DWORD*)"\x29\x00\x00\x00", (DWORD*)"\x00\x00\x00\x00", (DWORD*)"\x01\x00\x00\x00"); //grenade
        GiveItem(PlrObjAddr, 2, (DWORD*)"\x0D\x00\x00\x00", (DWORD*)"\x00\x00\x00\x00", (DWORD*)"\x01\x00\x00\x00"); //flaregun
        GiveItem(PlrObjAddr, 3, (DWORD*)"\x1F\x00\x00\x00", (DWORD*)"\x00\x00\x00\x00", (DWORD*)"\x01\x00\x00\x00"); //c4
        GiveItem(PlrObjAddr, 4, (DWORD*)"\x20\x00\x00\x00", (DWORD*)"\x00\x00\x00\x00", (DWORD*)"\x00\x00\x00\x00"); //c4 det
        GiveItem(PlrObjAddr, 6, (DWORD*)"\x0B\x00\x00\x00", (FLOAT*)"\x00\x00\x00\x00", (DWORD*)"\x04\x00\x00\x00"); //med kit
        GiveItem(PlrObjAddr, 7, (DWORD*)"\x0F\x00\x00\x00", (FLOAT*)"\x00\x00\xC8\x42", (DWORD*)"\x00\x00\x00\x00"); //flashlight
        GiveItem(PlrObjAddr, 8, (DWORD*)"\x06\x00\x00\x00", (DWORD*)"\x00\x00\x00\x00", (DWORD*)"\x00\x00\x00\x00"); //binoculars
        GiveItem(PlrObjAddr, 9, (DWORD*)"\x05\x00\x00\x00", (DWORD*)"\x00\x00\x00\x00", (DWORD*)"\x00\x00\x00\x00"); //artic
    }
    else
    {
        std::cout << " \n" << invalidPlrObjStr;
        al.playerobj = false;
    }
}

bool verifyPlrObjAddress()
{
    al.playerobj = (DWORD)ProtectedRead(hProcess, PlrObjAddr, 4) == 4;
    return al.playerobj;
}











