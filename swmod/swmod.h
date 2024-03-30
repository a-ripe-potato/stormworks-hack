#pragma once
#include <windows.h> 
#include <iostream>
#include "vector"
#include <sstream>
#include <iterator>
#include <math.h>
#include <intrin.h>
#include <psapi.h>
#include "mem.h"
#include "proctools.h"
#include "patternscan.h"
#include <thread>
#include "strings.h"
#include "items.h"
#include "injector.h"
#include "WinBase.h"
#include <filesystem>
#include "buildtime.h"
#include "mods.h"
#include "antidbg.h"
#include "ntp.h"


void readyCmdLine();

void ProcessCommand(std::string cmd);

struct addresses {
	BYTE* NoClipAddr;
	BYTE* VehTpAddr;
	BYTE* VehMapAddr;
	BYTE* VehCleanAddr;
	BYTE* LockSettAddr;
	BYTE* ConfigLockAddr;
	BYTE* MapTpAddr;
	BYTE* VehLockAddr;
	//BYTE* VehDamageAddr;
	BYTE* MapPlayersAddr;
	BYTE* InfElecAddr;
	BYTE* InfFuelAddr;
	BYTE* DisableWeaponsAddr;
	BYTE* InfAmmoAddr;
	BYTE* WorkbenchLockAddr;
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
};

struct allowList {
	bool god = false;
	bool infAmmo = false;
	bool infUtil = false;
	bool playerobj = false;
	bool noSpread = false;
	bool rapidFire = false;
	bool projID = false;
	bool workbench = false;
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
	bool forceVehicleSpawning = false;
	//bool vehDamage = false;
	bool mapPlrs = false;
	bool noSpread = false;
	bool rapidFire = false;
	bool projidchanged = false;
	bool forceworkbench = false;
};

struct cmd_flags {
	bool bypass = false;
	std::string method = "default";
};


void cleanup();

BOOL WINAPI HandlerRoutine(
    _In_ DWORD dwCtrlType
);


bool verifyPlrObjAddress();

void StartActionThread();

void ActionThread();

void getPlayerObjWhenAvailable();

void waitForPlrObj();

void ProcessCommand(std::string cmd);

BYTE* tryGetPlrObj();


template <size_t N>
void splitString(std::string(&arr)[N], std::string str);


bool isExpired();

void IntegrityThread();

BYTE* tryGetWorkbenchLock();

MODULEENTRY32 getSWModule();

HANDLE gethProcess();

void* getPlayerObjAddr();

bool isPlrScannerRunning();

struct addresses getAddresses();