#pragma once
#include "string"


const static std::string prefix = ">> ";
const static std::string MODVERSION = "Version: 1.1.10 (game version: v1.11.3)\n";
const static std::string exeNotFound = prefix + "Failed to find stormworks!\n";
const static std::string failedSetCH = prefix + "Failed to set control handler!\n";
const static std::string foundPattern = prefix + "Found pattern: ";
const static std::string failFindPatternArg = prefix + "Failed to find pattern: ";
const static std::string failFindPatternGen = prefix + "Failed to find pattern!\n";
const static std::string failVerifyAddressStr = prefix + "Failed to validate address!\n";
const static std::string invalidPlrObjStr = prefix + "PlrObjAddr not specified or is invalid!\n";
const static std::string startActionThreadStr = prefix + "Action thread started!\n";
const static std::string stopActionThreadStr = "Action thread stopped!\n";
const static std::string actionUnavailableStr = prefix + "That action is unavailable!\n";
const static std::string invalidArgumentStr = prefix + "Invalid argument supplied!\n";
const static std::string enableGodmodeStr = prefix + "Godmode enabled.\n";
const static std::string disableGodmodeStr = prefix + "Godmode disabled.\n";
const static std::string enableInfammoStr = prefix + "Inf ammo enabled.\n";
const static std::string disableInfammoStr = prefix + "Inf ammo disabled.\n";
const static std::string enableGInfammoStr = prefix + "Global Inf ammo enabled.\n";
const static std::string disableGInfammoStr = prefix + "Global Inf ammo disabled.\n";
const static std::string enableInfutilStr = prefix + "Inf util enabled.\n";
const static std::string disableInfutilStr = prefix + "Inf util disabled.\n";
const static std::string enableAutoloadoutStr = prefix + "Auto loadout enabled.\n";
const static std::string disableAutoloadoutStr = prefix + "Auto loadout disabled.\n";
const static std::string enableNoclipStr = prefix + "No clip enabled.\n";
const static std::string disableNoclipStr = prefix + "No clip disabled\n";
const static std::string enableAdminMenuStr = prefix + "Admin menu enabled.\n";
const static std::string disableAdminMenuStr = prefix + "Admin menu disabled.\n";
const static std::string enableForceDisableWeaponsStr = prefix + "Force Weapons disable active.\n";
const static std::string disableForceDisableWeaponsStr = prefix + "Force Weapons disable inactive.\n";
const static std::string enableForceEnableWeaponsStr = prefix + "Force Weapons enable active.\n";
const static std::string disableForceEnableWeaponsStr = prefix + "Force Weapons disable inactive.\n";
const static std::string enableKeepActiveStr = prefix + "Keep active enabled.\n";
const static std::string disableKeepActiveStr = prefix + "Keep active disabled.\n";
const static std::string enableInfFuelStr = prefix + "Inf fuel enabled.\n";
const static std::string disableInfFuelStr = prefix + "Inf fuel disabled.\n";
const static std::string enableInfElecStr = prefix + "Inf electricity enabled.\n";
const static std::string disableInfElecStr = prefix + "Inf electricity disabled.\n";
const static std::string enableVehDmgStr = prefix + "Vehicle damage enabled.\n";
const static std::string disableVehDmgStr = prefix + "Vehicle damage disabled.\n";
const static std::string enableMapPlrsStr = prefix + "Map players enabled.\n";
const static std::string disableMapPlrsStr = prefix + "Map players disabled.\n";
const static std::string enableNoSpreadStr = prefix + "No spread enabled.\n";
const static std::string disableNoSpreadStr = prefix + "No spread disabled.\n";
const static std::string enableRapidFireStr = prefix + "Rapid fire enabled.\n";
const static std::string disableRapidFireStr = prefix + "Rapid fire disabled.\n";
const static std::string enableInfFlashlightStr = prefix + "Inf flashlight enabled.\n";
const static std::string disableInfFlashlightStr = prefix + "Inf flashlight disabled.\n";
const static std::string authWorkbenchStr = prefix + "Authed!\n";
const static std::string deauthWorkbenchStr = prefix + "Deauthed!\n";
const static std::string updateRifleProjidStr = prefix + "Updated rifle projectile id.\n";
const static std::string enableVehSpawningStr = prefix + "Vehicle spawning enabled.\n";
const static std::string disableVehSpawningStr = prefix + "Vehicle spawning disabled.\n";
const static std::string enableSaveMenuStr = prefix + "World save menu shown.\n";
const static std::string disableSaveMenuStr = prefix + "World save menu hidden.\n";



void printHelpMessage();

//TODO: short term
// 
// huminize give command
// -------------------------------------------------------------------------------------------------
//TODO: long term
// fix auto loadout
//external gui
//enhance cmd line with more humanized commands.ex.give rifle primary 0 100
//-------------------------------------------------------------------------------------------------
//FINISHED:
//
//
//
