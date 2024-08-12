#include "mods.h"
#include "swmod.h"


void GiveItem(HANDLE hProcess, DWORD itemSlot, int itemID, float charge, int ammo)
{
    if (!verifyPlrObjAddress())
    {
        printf("%s", invalidPlrObjStr.c_str());
        return;
    }

    DWORD* plrAddr = (DWORD*)getPlayerObjAddr();
    DWORD* slotAddr = plrAddr + 0x95 + 7 * itemSlot;
    DWORD* itemAddr = slotAddr + 5;
    DWORD* chargeAddr = slotAddr + 6;
    DWORD* ammoAddr = slotAddr + 7;
    PatchEX(hProcess, itemAddr, reinterpret_cast<int*>(&itemID), 4);
    PatchEX(hProcess, chargeAddr, reinterpret_cast<int*>(&charge), 4);
    PatchEX(hProcess, ammoAddr, reinterpret_cast<int*>(&ammo), 4);
    //std::cout << ">> Item given!" << "\n";


}

uint16_t GetItem(HANDLE hProcess, DWORD itemSlot)
{
    if (!verifyPlrObjAddress())
    {
        printf("%s", invalidPlrObjStr.c_str());
        return NULL;
    }

    DWORD* plrAddr = (DWORD*)getPlayerObjAddr();
    DWORD* slotAddr = plrAddr + 0x95 + 7 * itemSlot;
    DWORD* itemAddr = slotAddr + 5;
    return (uint16_t)(uintptr_t)ProtectedRead(hProcess,itemAddr,4);

    //std::cout << ">> Item given!" << "\n";


}


bool verifyPlrObjAddress()
{
    void* plrobj = getPlayerObjAddr();
    if (plrobj == NULL) { return false; }
    bool playerobj = plrobj == tryGetPlrObj() && (DWORD)ProtectedRead(gethProcess(), plrobj, 4) == 4;
    //al.playerobj = (DWORD)ProtectedRead(hProcess, PlrObjAddr, 4) == 4;
    return playerobj;
}

BYTE* tryGetPlrObj() {
    uintptr_t plrPtrChainBase = (uintptr_t)getSWModule().modBaseAddr + 0xC0EA88;
    return (BYTE*)FindDMAAddy(gethProcess(), plrPtrChainBase, { 0x1F0, 0x20, 0x270, 0x50, 0x48, 0x370, 0x0 });
}

BYTE* tryGetWorkbenchLock() {
    uintptr_t plrPtrChainBase = (uintptr_t)getSWModule().modBaseAddr + 0xC0EA88;
    return (BYTE*)FindDMAAddy(gethProcess(), plrPtrChainBase, {0x370, 0x20, 0x258, 0x8, 0xE60, 0x368, 0x0}) + 0xA3;
}

void EnableInfAmmo(void* DecPrimarySmgAmmoAddr, void* DecPrimaryRifleAmmoAddr, void* DecPistolAmmoAddr,void* DecC4Addr,void* DecGrenadeAddr)
{
    HANDLE hProcess = gethProcess();
    NopEX(hProcess, DecPrimarySmgAmmoAddr, 4);
    NopEX(hProcess, DecPrimaryRifleAmmoAddr, 4);
    NopEX(hProcess, DecPistolAmmoAddr, 2);
    NopEX(hProcess, DecC4Addr, 4);
    NopEX(hProcess, DecGrenadeAddr, 4);
}

void EnableRapidFire(void* RifleRapidFireAddr) {
    int i = 0;
    PatchEX(gethProcess(), RifleRapidFireAddr, reinterpret_cast<int*>(&i), 4);
}

void DisableRapidFire(void* RifleRapidFireAddr) {
    int i = 7;
    PatchEX(gethProcess(), RifleRapidFireAddr, reinterpret_cast<int*>(&i), 4);
}

void EnableNoSpread(void* RifleNoSpreadAddr) {
    void* patch = (void*)"\x69\xC1\x00\x00\x00\x00";
    PatchEX(gethProcess(), RifleNoSpreadAddr, patch, 6);
}

void DisableNoSpread(void* RifleNoSpreadAddr) {
    void* patch = (void*)"\x69\xC1\xFD\x43\x03\x00";
    PatchEX(gethProcess(), RifleNoSpreadAddr, patch, 6);
}


void DisableInfAmmo(void* DecPrimarySmgAmmoAddr, void* DecPrimaryRifleAmmoAddr, void* DecPistolAmmoAddr, void* DecC4Addr, void* DecGrenadeAddr)
{
    HANDLE hProcess = gethProcess();
    void* ammoDecInstruction = (void*)"\x41\xFF\x4F\x08";
    void* PisGrenAmmoDecInstruction = (void*)"\x41\x89\x46\x08";
    void* C4DecInstruction = (void*)"\x41\xFF\x4E\x08";
    void* pistoldec = (void*)"\xFF\xC8";
    PatchEX(hProcess, DecPrimarySmgAmmoAddr, ammoDecInstruction, 4);
    PatchEX(hProcess, DecPrimaryRifleAmmoAddr, ammoDecInstruction, 4);
    PatchEX(hProcess, DecPistolAmmoAddr, pistoldec, 2);
    PatchEX(hProcess, DecC4Addr, C4DecInstruction, 4);
    PatchEX(hProcess, DecGrenadeAddr, PisGrenAmmoDecInstruction, 4);
}

void EnableInfUtil(void* DecFlashlightAddr, void* DecFlareAddr, void* DecFlaregunAddr, void* DecPrimaryWeldingTorchAddr, void* DecPrimaryFireExtAddr, void* DecMedKitAddr)
{
    void* addIns = (void*)"\xF3\x44\x0F\x58\xC7";
    void* addIns2 = (void*)"\xF3\x0F\x58\xC7";
    HANDLE hProcess = gethProcess();
    PatchEX(hProcess, DecFlashlightAddr, addIns2, 4);
    NopEX(hProcess, DecFlareAddr, 4);
    NopEX(hProcess, DecFlaregunAddr, 2);
    PatchEX(hProcess, DecPrimaryWeldingTorchAddr, addIns, 5);
    PatchEX(hProcess, DecPrimaryFireExtAddr, addIns2, 4);
    NopEX(hProcess, DecMedKitAddr, 4);
}

void EnableInfFlashlight(void* DecFlashlightAddr)
{
    HANDLE hProcess = gethProcess();
    void* flashAdd = (void*)"\xF3\x0F\x58\xC7";
    PatchEX(hProcess, DecFlashlightAddr, flashAdd, 4);
}

void DisableInfFlashlight(void* DecFlashlightAddr)
{
    HANDLE hProcess = gethProcess();
    void* flashSub = (void*)"\xF3\x0F\x5C\xC7";
    PatchEX(hProcess, DecFlashlightAddr, flashSub, 4);
}

void DisableInfUtil(void* DecFlashlightAddr, void* DecFlareAddr, void* DecFlaregunAddr, void* DecPrimaryWeldingTorchAddr, void* DecPrimaryFireExtAddr, void* DecMedKitAddr)
{
    HANDLE hProcess = gethProcess();
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

void setCharge(UINT itemSlot, float charge)
{

    if (!verifyPlrObjAddress())
    {
        printf("%s\n", prefix.c_str());
        return;
    }
    DWORD* plrAddr = (DWORD*)getPlayerObjAddr();
    DWORD* slotAddr = plrAddr + 0x95 + 7 * itemSlot;
    DWORD* ammoAddr = slotAddr + 6;
    PatchEX(gethProcess(), ammoAddr, reinterpret_cast<int*>(&charge), 4);
}

void setAmmo(UINT itemSlot, int ammo)
{

    if (!verifyPlrObjAddress())
    {
        printf("%s\n", prefix.c_str());
        return;
    }
    DWORD* plrAddr = (DWORD*)getPlayerObjAddr();
    DWORD* slotAddr = plrAddr + 0x95 + 7 * itemSlot;
    DWORD* ammoAddr = slotAddr + 7;
    PatchEX(gethProcess(), ammoAddr, reinterpret_cast<int*>(&ammo), 4);
}

void changeProjId(UINT projID, void* RifleProjIDAddr) {
    PatchEX(gethProcess(), RifleProjIDAddr, reinterpret_cast<int*>(&projID), 4);
}


bool giveLoadout()
{
    if (verifyPlrObjAddress())
    {
        HANDLE hProcess = gethProcess();
        char* PrimaryItemslotAddr = (char*)getPlayerObjAddr() + 0x268;
        GiveItem(hProcess, 0, SWITEM::smgID, 0, 40);
        GiveItem(hProcess, 1, SWITEM::flashlightID, 100, 0);
        GiveItem(hProcess, 2, SWITEM::medkitID, 0, 4);
        GiveItem(hProcess, 3, SWITEM::C4ID, 0, 1);
        GiveItem(hProcess, 4, SWITEM::C4_detID, 0, 0);
        GiveItem(hProcess, 7, SWITEM::grenadeID, 0, 1);
        GiveItem(hProcess, 8, SWITEM::binocularsID, 0, 0);
        GiveItem(hProcess, 9, SWITEM::arcticID, 0, 0);
        return true;
    }
    else
    {
        std::cout << " \n" << invalidPlrObjStr;
        return false;
    }
}





