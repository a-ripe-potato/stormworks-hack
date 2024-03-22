#pragma once
#include "string.h"
#include "windows.h"
#include "mem.h"
#include"stdio.h"
#include "strings.h"
#include "proctools.h"
#include "swmod.h"
#include "items.h"



void GiveItem(HANDLE hProcess, DWORD itemSlot, int itemID, float charge, int ammo);

bool verifyPlrObjAddress();

BYTE* tryGetPlrObj();

BYTE* tryGetWorkbenchLock();

void EnableInfAmmo(void* DecPrimarySmgAmmoAddr, void* DecPrimaryRifleAmmoAddr, void* DecPistolAmmoAddr, void* DecC4Addr, void* DecGrenadeAddr);

void EnableRapidFire(void* RifleRapidFireAddr);

void DisableRapidFire(void* RifleRapidFireAddr);

void EnableNoSpread(void* RifleNoSpreadAddr);

void DisableNoSpread(void* RifleNoSpreadAddr);

void DisableInfAmmo(void* DecPrimarySmgAmmoAddr, void* DecPrimaryRifleAmmoAddr, void* DecPistolAmmoAddr, void* DecC4Addr, void* DecGrenadeAddr);

void EnableInfUtil(void* DecFlashlightAddr, void* DecFlareAddr, void* DecFlaregunAddr, void* DecPrimaryWeldingTorchAddr, void* DecPrimaryFireExtAddr, void* DecMedKitAddr);

void EnableInfFlashlight(void* DecFlashlightAddr);

void DisableInfFlashlight(void* DecFlashlightAddr);

void DisableInfUtil(void* DecFlashlightAddr, void* DecFlareAddr, void* DecFlaregunAddr, void* DecPrimaryWeldingTorchAddr, void* DecPrimaryFireExtAddr, void* DecMedKitAddr);

void setCharge(UINT itemSlot, float charge);

void setAmmo(UINT itemSlot, int ammo);

void changeProjId(UINT projID, void* RifleProjIDAddr);

uint16_t GetItem(HANDLE hProcess, DWORD itemSlot);

bool giveLoadout();