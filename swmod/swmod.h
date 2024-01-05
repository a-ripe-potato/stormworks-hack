#pragma once
#include <windows.h> 
#include <iostream>
#include "vector"
#include <sstream>
#include <iterator>
#include <math.h>


void readyCmdLine();

void ProcessCommand(std::string cmd);

void EnableInfAmmo();

void DisableInfAmmo();

void EnableInfUtil();

void DisableInfUtil();

void GiveItem(DWORD itemSlot, int itemID, float charge, int ammo);

void setCharge(UINT itemSlot, float charge);

void setAmmo(UINT itemSlot, int charge);

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
};

 static const struct ItemData {
    static const bool bC4IsFloat = false;
    static const int C4id = 32;
};


void cleanup();

BOOL WINAPI HandlerRoutine(
    _In_ DWORD dwCtrlType
);

void giveLoadout();

bool verifyPlrObjAddress();

void StartActionThread();

void ActionThread();

void getPlayerObjWhenAvailable();

void waitForPlrObj();

void tryGetPlrObj();

std::vector<std::string> splitStringBySpace(std::string str);

template <size_t N>
void splitString(std::string(&arr)[N], std::string str);

