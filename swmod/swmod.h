#pragma once
#include <windows.h> 
#include <iostream>

void readyCmdLine();

void ProcessCommand(std::string command);

void EnableInfAmmo();

void DisableInfAmmo();

void EnableInfUtil();

void DisableInfUtil();

void GiveItem(void* PlrObjAddr, DWORD itemSlot, void* itemID, void* DWcharge, void* DWammo);

struct allowList {
    bool god = false;
    bool infAmmo = false;
    bool infUtil = false;
    bool playerobj = false;
};

struct modList {
    bool god = false;
    bool infAmmo = false;
    bool infAmmoG = false;
    bool infUtil = false;
    bool infElec = false;
    bool infFuel = false;
    bool autoLoadout = false;
    bool forceNoClip = false;
    bool forceAdminMenu = false;
    bool vehDamage = false;
    bool mapPlrs = false;
    bool disableWeapons = false;
    bool enableWeapons = false;
};

void cleanup();

BOOL WINAPI HandlerRoutine(
    _In_ DWORD dwCtrlType
);

void giveLoadout();

bool verifyPlrObjAddress();

void StartActionThread();

void ActionThread();