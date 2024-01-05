#pragma once
#include "iostream"
#include <Windows.h>

enum Itembool {
	True=true, False=false, Null=-1
};


namespace SWITEM {
	static constexpr UINT bomb_disposalID = 74;
	static constexpr Itembool bBomb_disposalUF = Null;
	static constexpr UINT chest_rigID = 75;
	static constexpr Itembool bChest_rigUF = Null;
	static constexpr UINT black_hawk_vestID = 76;
	static constexpr Itembool bBlack_hawk_vestUF = Null;
	static constexpr UINT plate_vestID = 77;
	static constexpr Itembool bPlate_vestUF = Null;
	static constexpr UINT armor_vestID = 78;
	static constexpr Itembool bArmor_vestUF = Null;
	static constexpr UINT divingID = 1;
	static constexpr Itembool bDivingUF = True;
	static constexpr UINT firefighterID = 2;
	static constexpr Itembool bFirefighterUF = Null;
	static constexpr UINT scubaID = 3;
	static constexpr Itembool bScubaUF = Null;
	static constexpr UINT parachuteID = 4;
	static constexpr Itembool bParachuteUF = False;
	static constexpr UINT arcticID = 5;
	static constexpr Itembool bArcticUF = Null;
	static constexpr UINT hazmatID = 29;
	static constexpr Itembool bHazmatUF = Null;
	static constexpr UINT binocularsID = 6;
	static constexpr Itembool bBinocularsUF = Null;
	static constexpr UINT cableID = 7;
	static constexpr Itembool bCableUF = Null;
	static constexpr UINT compassID = 8;
	static constexpr Itembool bCompassUF = Null;
	static constexpr UINT defibID = 9;
	static constexpr Itembool bDefibUF = Null;
	static constexpr UINT fire_extID = 10;
	static constexpr Itembool bFire_extUF = Null;
	static constexpr UINT medkitID = 11;
	static constexpr Itembool bMedkitUF = False;
	static constexpr UINT flareID = 12;
	static constexpr Itembool bFlareUF = Null;
	static constexpr UINT flaregunID = 13;
	static constexpr Itembool bFlaregunUF = Null;
	static constexpr UINT flaregun_ammoID = 14;
	static constexpr Itembool bFlaregun_ammoUF = Null;
	static constexpr UINT flashlightID = 15;
	static constexpr Itembool bFlashlightUF = True;
	static constexpr UINT hoseID = 16;
	static constexpr Itembool bHoseUF = Null;
	static constexpr UINT NV_binocularsID = 17;
	static constexpr Itembool bNV_binocularsUF = Null;
	static constexpr UINT oxygen_maskID = 18;
	static constexpr Itembool bOxygen_maskUF = True;
	static constexpr UINT radioID = 19;
	static constexpr Itembool bRadioUF = Null;
	static constexpr UINT radio_locatorID = 20;
	static constexpr Itembool bRadio_locatorUF = True;
	static constexpr UINT remote_ctrlID = 21;
	static constexpr Itembool bRemote_ctrlUF = True;
	static constexpr UINT ropeID = 22;
	static constexpr Itembool bRopeUF = Null;
	static constexpr UINT strobeID = 23;
	static constexpr Itembool bStrobeUF = Null;
	static constexpr UINT strobe_irID = 24;
	static constexpr Itembool bStrobe_irUF = Null;
	static constexpr UINT transponderID = 25;
	static constexpr Itembool bTransponderUF = True;
	static constexpr UINT UW_welding_torchID = 26;
	static constexpr Itembool bUW_welding_torchUF = True;
	static constexpr UINT welding_torchID = 27;
	static constexpr Itembool bWelding_torchUF = True;
	static constexpr UINT coalID = 28;
	static constexpr Itembool bCoalUF = False;
	static constexpr UINT rad_detectID = 30;
	static constexpr Itembool bRad_detectUF = True;
	static constexpr UINT C4ID = 31;
	static constexpr Itembool bC4UF = False;
	static constexpr UINT C4_detID = 32;
	static constexpr Itembool bC4_detUF = Null;
	static constexpr UINT speargunID = 33;
	static constexpr Itembool bSpeargunUF = False;
	static constexpr UINT speargun_ammoID = 34;
	static constexpr Itembool bSpeargun_ammoUF = False;
	static constexpr UINT pistolID = 35;
	static constexpr Itembool bPistolUF = False;
	static constexpr UINT pistol_ammoID = 36;
	static constexpr Itembool bPistol_ammoUF = False;
	static constexpr UINT smgID = 37;
	static constexpr Itembool bSmgUF = False;
	static constexpr UINT smg_ammoID = 38;
	static constexpr Itembool bSmg_ammoUF = False;
	static constexpr UINT rifleID = 39;
	static constexpr Itembool bRifleUF = False;
	static constexpr UINT rifle_ammoID = 40;
	static constexpr Itembool bRifle_ammoUF = False;
	static constexpr UINT grenadeID = 41;
	static constexpr Itembool bGrenadeUF = False;
}

