#pragma once
#include "string"

static std::string prefix = ">> ";
static std::string MODVERSION = "Version: 1.0.1\n";
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
static std::string invalidArgumentStr = prefix + "Invalid argument.\n";
static std::string enableGodmodeStr = prefix + "Godmode enabled.\n";
static std::string disableGodmodeStr = prefix + "Godmode disabled.\n";
static std::string enableInfammoStr = prefix + "Inf ammo enabled.\n";
static std::string disableInfammoStr = prefix + "Inf ammo disabled.\n";
static std::string enableInfutilStr = prefix + "Inf util enabled.\n";
static std::string disableInfutilStr = prefix + "Inf util disabled.\n";
static std::string enableAutoloadoutStr = prefix + "Auto loadout enabled.\n";
static std::string disableAutoloadoutStr = prefix + "Auto loadout disabled.\n";
static std::string enableNoclipStr = prefix + "No clip enabled.\n";
static std::string disableNoclipStr = prefix + "No clip disabled\n";
static std::string enableCMenuStr = prefix + "Admin menu enabled.\n";
static std::string disableCMenuStr = prefix + "Admin menu disabled.\n";
static std::string enableWeaponsStr = prefix + "Weapons enabled.\n";
static std::string disableWeaponsStr = prefix + "Weapons disabled.\n";
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


void printHelpMessage();

//TODO: short term
// testing and bug fixing
// -------------------------------------------------------------------------------------------------
//TODO: long term
// heal command
// show players
// force workbench
// better command parsing and revise command structure
// find more shit to fuck with
// perform full stability test 
// find chat func
// find a way to get plrobj easy
