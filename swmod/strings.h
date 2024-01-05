#pragma once
#include "string"

static std::string prefix = ">> ";
static std::string MODVERSION = "Version: 1.1.1 (game version: v1.9.21)\n";
static std::string exeNotFound = prefix + "Failed to find stormworks!\n";
static std::string failedSetCH = prefix + "Failed to set control handler!\n";
static std::string foundPattern = prefix + "Found pattern: ";
static std::string failFindPatternArg = prefix + "Failed to find pattern: ";
static std::string failFindPatternGen = prefix + "Failed to find pattern!\n";
static std::string failVerifyAddressStr = prefix + "Failed to validate address!\n"; 
static std::string invalidPlrObjStr = prefix + "PlrObjAddr not specified or is invalid!\n";
static std::string startActionThreadStr = prefix + "Action thread started!\n";
static std::string stopActionThreadStr = "Action thread stopped!\n";
static std::string actionUnavailableStr = prefix + "That action is unavailable!\n";
static std::string invalidArgumentStr = prefix + "Invalid argument supplied!\n";
static std::string enableGodmodeStr = prefix + "Godmode enabled.\n";
static std::string disableGodmodeStr = prefix + "Godmode disabled.\n";
static std::string enableInfammoStr = prefix + "Inf ammo enabled.\n";
static std::string disableInfammoStr = prefix + "Inf ammo disabled.\n";
static std::string enableGInfammoStr = prefix + "Global Inf ammo enabled.\n";
static std::string disableGInfammoStr = prefix + "Global Inf ammo disabled.\n";
static std::string enableInfutilStr = prefix + "Inf util enabled.\n";
static std::string disableInfutilStr = prefix + "Inf util disabled.\n";
static std::string enableAutoloadoutStr = prefix + "Auto loadout enabled.\n";
static std::string disableAutoloadoutStr = prefix + "Auto loadout disabled.\n";
static std::string enableNoclipStr = prefix + "No clip enabled.\n";
static std::string disableNoclipStr = prefix + "No clip disabled\n";
static std::string enableAdminMenuStr = prefix + "Admin menu enabled.\n";
static std::string disableAdminMenuStr = prefix + "Admin menu disabled.\n";
static std::string enableForceDisableWeaponsStr = prefix + "Force Weapons disable active.\n";
static std::string disableForceDisableWeaponsStr = prefix + "Force Weapons disable inactive.\n";
static std::string enableForceEnableWeaponsStr = prefix + "Force Weapons enable active.\n";
static std::string disableForceEnableWeaponsStr = prefix + "Force Weapons disable inactive.\n";
static std::string enableKeepActiveStr = prefix + "Keep active enabled.\n";
static std::string disableKeepActiveStr = prefix + "Keep active disabled.\n";
static std::string enableInfFuelStr = prefix + "Inf fuel enabled.\n";
static std::string disableInfFuelStr = prefix + "Inf fuel disabled.\n";
static std::string enableInfElecStr = prefix + "Inf electricity enabled.\n";
static std::string disableInfElecStr = prefix + "Inf electricity disabled.\n";
static std::string enableVehDmgStr = prefix + "Vehicle damage enabled.\n";
static std::string disableVehDmgStr = prefix + "Vehicle damage disabled.\n";
static std::string enableMapPlrsStr = prefix + "Map players enabled.\n";
static std::string disableMapPlrsStr = prefix + "Map players disabled.\n";
static std::string enableDebugStr = prefix + "Debugging Enabled.\n";
static std::string disableDebugStr = prefix + "Debugging Disabled.\n";


void printHelpMessage();

//TODO: short term
// fix auto loadout
// -------------------------------------------------------------------------------------------------
//TODO: long term
// force workbench
// find chat func
