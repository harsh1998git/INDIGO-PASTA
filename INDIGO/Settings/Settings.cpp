#include "Settings.h"

//[enc_string_enable /]
//[junk_enable /]

using namespace Client;

namespace Settings
{
	void LoadSettings( string szIniFile )
	{
		CSX::Cvar::InitPath( szIniFile.c_str() );

		Aimbot::AimBot_Deathmatch = CSX::Cvar::LoadCvar(AIMBOT_TEXT, CVAR_AIMBOT_DEATHMATCH, Aimbot::AimBot_Deathmatch);
		Aimbot::AimBot_WallAttack = CSX::Cvar::LoadCvar(AIMBOT_TEXT, CVAR_AIMBOT_WALLATTACK, Aimbot::AimBot_WallAttack);
		Aimbot::AimBot_CheckSmoke = CSX::Cvar::LoadCvar(AIMBOT_TEXT, CVAR_AIMBOT_CHECKSMOKE, Aimbot::AimBot_CheckSmoke);
		Aimbot::AimBot_AntiJump = CSX::Cvar::LoadCvar(AIMBOT_TEXT, CVAR_AIMBOT_ANTIJUMP, Aimbot::AimBot_AntiJump);
		Aimbot::BackTrackTicks = CSX::Cvar::LoadCvar(AIMBOT_TEXT, CVAR_BackTrackTicks, Aimbot::BackTrackTicks);
		Aimbot::AimBot_JumpCheckME = CSX::Cvar::LoadCvar(AIMBOT_TEXT, CVAR_AIMBOT_JUMPCHEKME, Aimbot::AimBot_JumpCheckME);
		Aimbot::AimBot_DrawFov = CSX::Cvar::LoadCvar(AIMBOT_TEXT, CVAR_AIMBOT_DRAWFOV, Aimbot::AimBot_DrawFov);
		Aimbot::AimBot_DrawSpot = CSX::Cvar::LoadCvar(AIMBOT_TEXT, CVAR_AIMBOT_DRAWSPOT, Aimbot::AimBot_DrawSpot);
		for (DWORD i = 0; i < WEAPON_DATA_SIZE; i++)
		{
			// пистолеты
			if (i <= 9)
			{
				Aimbot::weapon_settings[i].AimBot_Active = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_ACTIVE, 0);
				Aimbot::weapon_settings[i].AimBot_Smooth = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_SMOOTH, 10);
				Aimbot::weapon_settings[i].AimBot_Fov = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_FOV, 6);
				Aimbot::weapon_settings[i].AimBot_pSilentFov = CSX::Cvar::LoadCvar(pWeaponData[i], "AimBot_pSilentFov", 6);
				Aimbot::weapon_settings[i].AimBot_pSecondSilentFov = CSX::Cvar::LoadCvar(pWeaponData[i], "AimBot_pSecondSilentFov", 6);
				Aimbot::weapon_settings[i].AimBot_FovType = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_FOVTYPE, 3);
				Aimbot::weapon_settings[i].AimBot_BestHit = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_BESTHIT, 0);
				Aimbot::weapon_settings[i].AimBot_Spot = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_SPOT, 5);
				Aimbot::weapon_settings[i].AimBot_Delay = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_DELAY, 0);
				Aimbot::weapon_settings[i].AimBot_Use2fovs = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_2FOV, 0);
				Aimbot::weapon_settings[i].AimBot_Rcs = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_RCS, 50);
				Aimbot::weapon_settings[i].AimBot_SecondFov = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_RCSFOV, 20);
				Aimbot::weapon_settings[i].AimBot_RcsAfterXShot = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AimBot_RcsAfterXShot, 1);
				Aimbot::weapon_settings[i].AimBot_AutoPistol = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_AUTOPISTOL, 0);
				Aimbot::weapon_settings[i].AimBot_SecondSmooth = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_RCSSMOOTH, 10);
				Aimbot::weapon_settings[i].AimBot_RcsType = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_RCSTYPE, 1);
				g_SkinChangerCfg[pWeaponItemIndexData[i]].nFallbackPaintKit = CSX::Cvar::LoadCvar(pWeaponData[i], "Skin_SkinID", 1);
				g_SkinChangerCfg[pWeaponItemIndexData[i]].flFallbackWear = CSX::Cvar::LoadCvar(pWeaponData[i], "Skin_Float", 1);
				g_SkinChangerCfg[pWeaponItemIndexData[i]].nFallbackStatTrak = CSX::Cvar::LoadCvar(pWeaponData[i], "Skin_StatTrack", 1);
				g_SkinChangerCfg[pWeaponItemIndexData[i]].LiveStatTrack = CSX::Cvar::LoadCvar(pWeaponData[i], "Skin_LiveStatTrack", 1);
				g_SkinChangerCfg[pWeaponItemIndexData[i]].Rarity = CSX::Cvar::LoadCvar(pWeaponData[i], "Skin_Rarity", 1);
			}

			// автоматы
			if (i >= 10 && i <= 30)
			{
				Aimbot::weapon_settings[i].AimBot_Active = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_ACTIVE, 0);
				Aimbot::weapon_settings[i].AimBot_Smooth = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_SMOOTH, 10);
				Aimbot::weapon_settings[i].AimBot_pSilentFov = CSX::Cvar::LoadCvar(pWeaponData[i], "AimBot_pSilentFov", 6);
				Aimbot::weapon_settings[i].AimBot_pSecondSilentFov = CSX::Cvar::LoadCvar(pWeaponData[i], "AimBot_pSecondSilentFov", 6);
				Aimbot::weapon_settings[i].AimBot_Fov = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_FOV, 6);
				Aimbot::weapon_settings[i].AimBot_FovType = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_FOVTYPE, 1);
				Aimbot::weapon_settings[i].AimBot_BestHit = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_BESTHIT, 0);
				Aimbot::weapon_settings[i].AimBot_Spot = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_SPOT, 5);
				Aimbot::weapon_settings[i].AimBot_Delay = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_DELAY, 0);
				Aimbot::weapon_settings[i].AimBot_Use2fovs = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_2FOV, 0);
				Aimbot::weapon_settings[i].AimBot_Rcs = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_RCS, 50);
				Aimbot::weapon_settings[i].AimBot_SecondFov = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_RCSFOV, 20);
				Aimbot::weapon_settings[i].AimBot_RcsAfterXShot = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AimBot_RcsAfterXShot, 1);
				Aimbot::weapon_settings[i].AimBot_SecondSmooth = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_RCSSMOOTH, 10);
				Aimbot::weapon_settings[i].AimBot_RcsClampType = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_RCSCLAMPTYPE, 0);
				Aimbot::weapon_settings[i].AimBot_RcsType = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_RCSTYPE, 1);
				g_SkinChangerCfg[pWeaponItemIndexData[i]].nFallbackPaintKit = CSX::Cvar::LoadCvar(pWeaponData[i], "Skin_SkinID", 1);
				g_SkinChangerCfg[pWeaponItemIndexData[i]].flFallbackWear = CSX::Cvar::LoadCvar(pWeaponData[i], "Skin_Float", 1);
				g_SkinChangerCfg[pWeaponItemIndexData[i]].nFallbackStatTrak = CSX::Cvar::LoadCvar(pWeaponData[i], "Skin_StatTrack", 1);
				g_SkinChangerCfg[pWeaponItemIndexData[i]].LiveStatTrack = CSX::Cvar::LoadCvar(pWeaponData[i], "Skin_LiveStatTrack", 1);
				g_SkinChangerCfg[pWeaponItemIndexData[i]].Rarity = CSX::Cvar::LoadCvar(pWeaponData[i], "Skin_Rarity", 1);
			}

			// снайперки
			if (i >= 31 && i <= 32)
			{
				Aimbot::weapon_settings[i].AimBot_Active = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_ACTIVE, 0);
				Aimbot::weapon_settings[i].AimBot_Smooth = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_SMOOTH, 10);
				Aimbot::weapon_settings[i].AimBot_pSilentFov = CSX::Cvar::LoadCvar(pWeaponData[i], "AimBot_pSilentFov", 6);
				Aimbot::weapon_settings[i].AimBot_pSecondSilentFov = CSX::Cvar::LoadCvar(pWeaponData[i], "AimBot_pSecondSilentFov", 6);
				Aimbot::weapon_settings[i].AimBot_Fov = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_FOV, 6);
				Aimbot::weapon_settings[i].AimBot_FovType = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_FOVTYPE, 1);
				Aimbot::weapon_settings[i].AimBot_BestHit = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_BESTHIT, 0);
				Aimbot::weapon_settings[i].AimBot_Spot = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_SPOT, 5);
				Aimbot::weapon_settings[i].AimBot_Delay = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_DELAY, 0);
				Aimbot::weapon_settings[i].AimBot_Use2fovs = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_2FOV, 0);
				Aimbot::weapon_settings[i].AimBot_Rcs = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_RCS, 50);
				Aimbot::weapon_settings[i].AimBot_SecondFov = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_RCSFOV, 20);
				Aimbot::weapon_settings[i].AimBot_RcsAfterXShot = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AimBot_RcsAfterXShot, 1);
				Aimbot::weapon_settings[i].AimBot_SecondSmooth = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_RCSSMOOTH, 10);
				Aimbot::weapon_settings[i].AimBot_RcsType = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_RCSTYPE, 1);
				g_SkinChangerCfg[pWeaponItemIndexData[i]].nFallbackPaintKit = CSX::Cvar::LoadCvar(pWeaponData[i], "Skin_SkinID", 1);
				g_SkinChangerCfg[pWeaponItemIndexData[i]].flFallbackWear = CSX::Cvar::LoadCvar(pWeaponData[i], "Skin_Float", 1);
				g_SkinChangerCfg[pWeaponItemIndexData[i]].nFallbackStatTrak = CSX::Cvar::LoadCvar(pWeaponData[i], "Skin_StatTrack", 1);
				g_SkinChangerCfg[pWeaponItemIndexData[i]].LiveStatTrack = CSX::Cvar::LoadCvar(pWeaponData[i], "Skin_LiveStatTrack", 1);
				g_SkinChangerCfg[pWeaponItemIndexData[i]].Rarity = CSX::Cvar::LoadCvar(pWeaponData[i], "Skin_Rarity", 1);
			}
		}

		Esp::Visuals_Style = CSX::Cvar::LoadCvar( VISUAL_TEXT , CVAR_ESP_STYLE , Esp::Visuals_Style );
		Esp::Visuals_Line = CSX::Cvar::LoadCvar( VISUAL_TEXT , CVAR_ESP_LINE , Esp::Visuals_Line ) != false;
		Esp::NightMode = CSX::Cvar::LoadCvar( VISUAL_TEXT , CVAR_ESP_NightMode, Esp::NightMode) != false;
		Esp::Visuals_Name = CSX::Cvar::LoadCvar( VISUAL_TEXT , CVAR_ESP_NAME , Esp::Visuals_Name ) != false;
		Esp::Visuals_Rank = CSX::Cvar::LoadCvar( VISUAL_TEXT , CVAR_ESP_RANK , Esp::Visuals_Rank ) != false;
		Esp::Visuals_Health = CSX::Cvar::LoadCvar( VISUAL_TEXT , CVAR_ESP_HEALTH , Esp::Visuals_Health );
		Esp::Visuals_Armor = CSX::Cvar::LoadCvar( VISUAL_TEXT , CVAR_ESP_ARMOR , Esp::Visuals_Armor );
		Esp::Visuals_Weapon = CSX::Cvar::LoadCvar( VISUAL_TEXT , CVAR_ESP_WEAPON , Esp::Visuals_Weapon ) != false;
		Esp::Visuals_Ammo = CSX::Cvar::LoadCvar( VISUAL_TEXT , CVAR_ESP_AMMO , Esp::Visuals_Ammo ) != false;
		Esp::Visuals_Sound = CSX::Cvar::LoadCvar( VISUAL_TEXT , CVAR_ESP_SOUND , Esp::Visuals_Sound ) != false;
		Esp::Visuals_SoundType = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_Visuals_SoundType, Esp::Visuals_SoundType);
		Esp::Visuals_SoundSize = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_Visuals_SoundSize, Esp::Visuals_SoundSize);
	//	Esp::Visuals_SoundDist = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_Visuals_SoundDist, Esp::Visuals_SoundDist);
		Esp::Visuals_ChamsRainbow = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_Visuals_ChamsRainbow, Esp::Visuals_ChamsRainbow);
		Esp::Visuals_HPBasedChams = CSX::Cvar::LoadCvar(VISUAL_TEXT,"Visuals_HPBasedChams", Esp::Visuals_HPBasedChams);
		Esp::Visuals_Skeleton = CSX::Cvar::LoadCvar( VISUAL_TEXT , CVAR_ESP_SKELETON , Esp::Visuals_Skeleton ) != false;
		Esp::Visuals_Team = CSX::Cvar::LoadCvar( VISUAL_TEXT , CVAR_ESP_TEAM , Esp::Visuals_Team ) != false;
		Esp::Visuals_Enemy = CSX::Cvar::LoadCvar( VISUAL_TEXT , CVAR_ESP_ENEMY , Esp::Visuals_Enemy ) != false;
		Esp::Visuals_Visible = CSX::Cvar::LoadCvar( VISUAL_TEXT , CVAR_ESP_VISIBLE , Esp::Visuals_Visible );
		Esp::Visuals_track = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_TRACK, Esp::Visuals_Skeleton) != false;
		Esp::Visuals_Chams = CSX::Cvar::LoadCvar( VISUAL_TEXT , CVAR_ESP_CHAMS , Esp::Visuals_Chams );
		Esp::Visuals_Bomb = CSX::Cvar::LoadCvar( VISUAL_TEXT , CVAR_ESP_BOMB , Esp::Visuals_Bomb ) != false;
		Esp::Visuals_BombTimer = CSX::Cvar::LoadCvar( VISUAL_TEXT , CVAR_ESP_BOMBTIMER , Esp::Visuals_BombTimer );
		Esp::Visuals_WorldWeapons = CSX::Cvar::LoadCvar( VISUAL_TEXT , CVAR_ESP_WORLDWEAPONS , Esp::Visuals_WorldWeapons ) != false;
		Esp::Visuals_WorldGrenade = CSX::Cvar::LoadCvar( VISUAL_TEXT , CVAR_ESP_WORLDGRENADE , Esp::Visuals_WorldGrenade ) != false;

		string SoundVisuals_Color = CSX::Cvar::LoadCvar("Colors", CVAR_ESP_COLOR_SOUNDESP, "124,255,0");
		string WeaponEnemy_Color = CSX::Cvar::LoadCvar("Colors", CVAR_ESP_COLOR_WEAPONENEMY, "0,0,0");
		string WorldWeapon_Color = CSX::Cvar::LoadCvar("Colors", CVAR_ESP_COLOR_WORLDWEAPON, "0,0,0");
		string Visuals_Line_Color = CSX::Cvar::LoadCvar("Colors", CVAR_ESP_COLOR_LINE, "0,0,0");
		string Name_Color = CSX::Cvar::LoadCvar("Colors", CVAR_ESP_COLOR_NAME, "255,255,0");

		string Fraggrenade_Color = CSX::Cvar::LoadCvar("Colors", CVAR_ESP_COLOR_FRAGGRENADE, "255,64,64");
		string Flashbang_Color = CSX::Cvar::LoadCvar("Colors", CVAR_ESP_COLOR_FLASHBANG, "64,128,255");
		string Smokegrenade_Color = CSX::Cvar::LoadCvar("Colors", CVAR_ESP_COLOR_SMOKEGRENADE, "100,100,100");
		string GrenadeHelper = CSX::Cvar::LoadCvar("Colors", CVAR_ESP_COLOR_GrenadeHelper, "255,0,0");

		string Skelet = CSX::Cvar::LoadCvar("Colors", CVAR_ESP_COLOR_SKELET, "0,128,255");

		string Health_Color = CSX::Cvar::LoadCvar("Colors", CVAR_ESP_COLOR_HEALTH, "0,128,255");
		string Armor_Color = CSX::Cvar::LoadCvar("Colors", CVAR_ESP_COLOR_ARMOR, "0,255,0");

		string Visuals_Color_CT = CSX::Cvar::LoadCvar( "Colors" , CVAR_ESP_COLOR_CT , "0,0,50" );
		string Visuals_Color_TT = CSX::Cvar::LoadCvar( "Colors" , CVAR_ESP_COLOR_TT , "0,0,50");
		string Visuals_Color_VCT = CSX::Cvar::LoadCvar( "Colors" , CVAR_ESP_COLOR_VCT , "0,255,0" );
		string Visuals_Color_VTT = CSX::Cvar::LoadCvar( "Colors" , CVAR_ESP_COLOR_VTT , "0,255,0" );

		string chams_Color_CT = CSX::Cvar::LoadCvar( "Colors" , CVAR_CHAMS_COLOR_CT , "0,0,50");
		string chams_Color_TT = CSX::Cvar::LoadCvar( "Colors" , CVAR_CHAMS_COLOR_TT , "0,0,50");
		string chams_Color_VCT = CSX::Cvar::LoadCvar( "Colors" , CVAR_CHAMS_COLOR_VCT , "0,255,0" );

		string chams_Color_VTT = CSX::Cvar::LoadCvar( "Colors" , CVAR_CHAMS_COLOR_VTT , "0,255,0" );
		ScanColorFromCvar(SoundVisuals_Color.c_str(), Settings::Colors::SoundVisuals_Color);
		ScanColorFromCvar(WeaponEnemy_Color.c_str(), Settings::Colors::WeaponEnemy_Color);
		ScanColorFromCvar(WorldWeapon_Color.c_str(), Settings::Colors::WorldWeapon_Color);
		ScanColorFromCvar(Visuals_Line_Color.c_str(), Settings::Colors::Visuals_Line_Color);

		// =============

		ScanColorFromCvar(Name_Color.c_str(), Settings::Colors::Name_Color);
		ScanColorFromCvar(Fraggrenade_Color.c_str(), Settings::Colors::Fraggrenade_Color);
		ScanColorFromCvar(Flashbang_Color.c_str(), Settings::Colors::Flashbang_Color);
		ScanColorFromCvar(Smokegrenade_Color.c_str(), Settings::Colors::Smokegrenade_Color);
		ScanColorFromCvar(GrenadeHelper.c_str(), Settings::Colors::GrenadeHelper);
		ScanColorFromCvar(Skelet.c_str(), Settings::Colors::Skelet);
		ScanColorFromCvar(Health_Color.c_str(), Settings::Colors::Health_Color);
		ScanColorFromCvar(Armor_Color.c_str(), Settings::Colors::Armor_Color);

		ScanColorFromCvar( Visuals_Color_CT.c_str() , Settings::Colors::Visuals_Color_CT );
		ScanColorFromCvar( Visuals_Color_TT.c_str() , Settings::Colors::Visuals_Color_TT );
		ScanColorFromCvar( Visuals_Color_VCT.c_str() , Settings::Colors::Visuals_Color_VCT );
		ScanColorFromCvar( Visuals_Color_VTT.c_str() , Settings::Colors::Visuals_Color_VTT );

		ScanColorFromCvar( chams_Color_CT.c_str() , Settings::Colors::chams_Color_CT );
		ScanColorFromCvar( chams_Color_TT.c_str() , Settings::Colors::chams_Color_TT );
		ScanColorFromCvar( chams_Color_VCT.c_str() , Settings::Colors::chams_Color_VCT );
		ScanColorFromCvar( chams_Color_VTT.c_str() , Settings::Colors::chams_Color_VTT );

		Radar::Radar_Enable = CSX::Cvar::LoadCvar(RADAR_TEXT, CVAR_RAD_ENABLE, Radar::Radar_Enable) != false;
		Radar::Radar_Active = CSX::Cvar::LoadCvar( RADAR_TEXT , CVAR_RAD_ACTIVE , Radar::Radar_Active ) != false;
		Radar::Radar_Team = CSX::Cvar::LoadCvar( RADAR_TEXT , CVAR_RAD_TEAM , Radar::Radar_Team ) != false;
		Radar::Radar_Enemy = CSX::Cvar::LoadCvar( RADAR_TEXT , CVAR_RAD_ENEMY , Radar::Radar_Enemy ) != false;
		Radar::Radar_Sound = CSX::Cvar::LoadCvar( RADAR_TEXT , CVAR_RAD_SOUND , Radar::Radar_Sound ) != false;
		Radar::Radar_Range = CSX::Cvar::LoadCvar( RADAR_TEXT , CVAR_RAD_RANGE , Radar::Radar_Range );

		string Color_CT = CSX::Cvar::LoadCvar( RADAR_TEXT , CVAR_RAD_COLOR_CT , "0,164,255" );
		string Color_TT = CSX::Cvar::LoadCvar( RADAR_TEXT , CVAR_RAD_COLOR_TT , "255,64,64" );
		string Color_VCT = CSX::Cvar::LoadCvar( RADAR_TEXT , CVAR_RAD_COLOR_VCT , "0,255,0" );
		string Color_VTT = CSX::Cvar::LoadCvar( RADAR_TEXT , CVAR_RAD_COLOR_VTT , "0,255,0" );

		ScanColorFromCvar( Color_CT.c_str() , Radar::Radar_Color_CT );
		ScanColorFromCvar( Color_TT.c_str() , Radar::Radar_Color_TT );
		ScanColorFromCvar( Color_VCT.c_str() , Radar::Radar_Color_VCT );
		ScanColorFromCvar( Color_VTT.c_str() , Radar::Radar_Color_VTT );

		Misc::Other_Backtrack = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MISC_BACKTRACK, Misc::Other_Backtrack) != false;
		Misc::Other_Bhop = CSX::Cvar::LoadCvar( MISC_TEXT , CVAR_MISC_BHOP , Misc::Other_Bhop ) != false;
		Misc::Other_Punch = CSX::Cvar::LoadCvar( MISC_TEXT , CVAR_MISC_PUNCH , Misc::Other_Punch ) != false;
		Misc::Other_AwpAim = CSX::Cvar::LoadCvar( MISC_TEXT , CVAR_MISC_AWPAIM , Misc::Other_AwpAim ) != false;
		Misc::Other_NoFlash = CSX::Cvar::LoadCvar( MISC_TEXT , CVAR_MISC_NOFLASH , Misc::Other_NoFlash ) != false;
		Misc::Other_AutoStrafe = CSX::Cvar::LoadCvar( MISC_TEXT , CVAR_MISC_AUTOSTRAFE , Misc::Other_AutoStrafe ) != false;
		Misc::Other_AutoAccept = CSX::Cvar::LoadCvar( MISC_TEXT , CVAR_MISC_AUTOACCEPT , Misc::Other_AutoAccept ) != false;
		Misc::Other_Spectators = CSX::Cvar::LoadCvar( MISC_TEXT , CVAR_MISC_SPECTATORS , Misc::Other_Spectators ) != false;
		Misc::Other_FovChanger = CSX::Cvar::LoadCvar( MISC_TEXT , CVAR_MISC_FOV_CHANGER , Misc::Other_FovChanger ) != false;
		Misc::Other_FovView = CSX::Cvar::LoadCvar( MISC_TEXT , CVAR_MISC_FOV_VIEW , Misc::Other_FovView );
		Misc::Other_FovModelView = CSX::Cvar::LoadCvar( MISC_TEXT , CVAR_MISC_FOV_MDL_VIEW , Misc::Other_FovModelView );
		Misc::Other_WireHands = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MISC_WIREHANDS, Misc::Other_WireHands) != false;
		Misc::Other_NoHands = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MISC_NOHANDS, Misc::Other_NoHands) != false;
		Misc::EnableSkinchanger = CSX::Cvar::LoadCvar(MISC_TEXT,"EnableSkinchanger", Misc::EnableSkinchanger) != false;
		Menu::Menu_UseCustomColors = CSX::Cvar::LoadCvar("Menu Settings", "Menu_UseCustomColors", Menu::Menu_UseCustomColors) != false;

		string Menu_MainColor = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MENU1, "33,165,234");
		string Menu_BackGround = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MENU2, "33,165,234");
		string Menu_BackGround1 = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MENU3, "33,165,234");
		string Menu_Text = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MENU4, "33,165,234");

		ScanColorFromCvar(Menu_BackGround.c_str(), Settings::Menu::Menu_BackGround);
		ScanColorFromCvar(Menu_BackGround1.c_str(), Settings::Menu::Menu_BackGround1);
		ScanColorFromCvar(Menu_Text.c_str(), Settings::Menu::Menu_Text);
		ScanColorFromCvar(Menu_MainColor.c_str(), Settings::Menu::Menu_MainColor);

		Misc::bombtimery = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MISC_BOMBY, Misc::bombtimery);
		Misc::bombtimerx = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MISC_BOMBX, Misc::bombtimerx);

		string Color_AWPAIM = CSX::Cvar::LoadCvar( MISC_TEXT , CVAR_MISC_AWPAIM_COLOR , "255,0,0" );

		ScanColorFromCvar( Color_AWPAIM.c_str() , Misc::Other_AwpAimColor );

		Knifebot::knf_Active = CSX::Cvar::LoadCvar(KNIFEBOT_TEXT, CVAR_KNIFEBOT_ACTIVE, Knifebot::knf_Active) != false;
		Knifebot::knf_Team = CSX::Cvar::LoadCvar(KNIFEBOT_TEXT, CVAR_KNIFEBOT_TEAM, Knifebot::knf_Team) != false;
		Knifebot::knf_Attack = CSX::Cvar::LoadCvar(KNIFEBOT_TEXT, CVAR_KNIFEBOT_ATTACK, Knifebot::knf_Attack);
		Knifebot::knf_DistAttack = CSX::Cvar::LoadCvar(KNIFEBOT_TEXT, CVAR_KNIFEBOT_DISTATTACK, Knifebot::knf_DistAttack);
		Knifebot::knf_DistAttack2 = CSX::Cvar::LoadCvar(KNIFEBOT_TEXT, CVAR_KNIFEBOT_DISTATTACK2, Knifebot::knf_DistAttack2);

		Skin::knf_ct_model = CSX::Cvar::LoadCvar(SKIN_TEXT, CVAR_SKIN_CT_MODEL, Skin::knf_ct_model);
		Skin::knf_ct_skin = CSX::Cvar::LoadCvar(SKIN_TEXT, CVAR_SKIN_CT_SKIN, Skin::knf_ct_skin);
		Skin::knf_tt_model = CSX::Cvar::LoadCvar(SKIN_TEXT, CVAR_SKIN_TT_MODEL, Skin::knf_tt_model);
		Skin::knf_tt_skin = CSX::Cvar::LoadCvar(SKIN_TEXT, CVAR_SKIN_TT_SKIN, Skin::knf_tt_skin);
		Skin::gloves_skin = CSX::Cvar::LoadCvar(SKIN_TEXT, CVAR_SKIN_GLOVES, Skin::gloves_skin);

		Client::SendClientHello();

		if (Interfaces::Engine()->IsConnected())
			ForceFullUpdate();
	}

	void SaveSettings(string szIniFile)
	{
		CSX::Cvar::InitPath(szIniFile.c_str());

		CSX::Cvar::SaveCvar(AIMBOT_TEXT, CVAR_AIMBOT_DEATHMATCH, Aimbot::AimBot_Deathmatch);
		CSX::Cvar::SaveCvar(AIMBOT_TEXT, CVAR_AIMBOT_WALLATTACK, Aimbot::AimBot_WallAttack);
		CSX::Cvar::SaveCvar(AIMBOT_TEXT, CVAR_AIMBOT_CHECKSMOKE, Aimbot::AimBot_CheckSmoke);
		CSX::Cvar::SaveCvar(AIMBOT_TEXT, CVAR_AIMBOT_ANTIJUMP, Aimbot::AimBot_AntiJump);
		CSX::Cvar::SaveCvar(AIMBOT_TEXT, CVAR_AIMBOT_JUMPCHEKME, Aimbot::AimBot_JumpCheckME);
		CSX::Cvar::SaveCvar(AIMBOT_TEXT, CVAR_BackTrackTicks, Aimbot::BackTrackTicks);
		CSX::Cvar::SaveCvar(AIMBOT_TEXT, CVAR_AIMBOT_DRAWFOV, Aimbot::AimBot_DrawFov);
		CSX::Cvar::SaveCvar(AIMBOT_TEXT, CVAR_AIMBOT_DRAWSPOT, Aimbot::AimBot_DrawSpot);
		for (DWORD i = 0; i < WEAPON_DATA_SIZE; i++)
		{
			// пистолеты
			if (i <= 9)
			{
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_ACTIVE, Aimbot::weapon_settings[i].AimBot_Active);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_SMOOTH, Aimbot::weapon_settings[i].AimBot_Smooth);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_FOV, Aimbot::weapon_settings[i].AimBot_Fov);
				CSX::Cvar::SaveCvar(pWeaponData[i], "AimBot_pSecondSilentFov", Aimbot::weapon_settings[i].AimBot_pSilentFov);
				CSX::Cvar::SaveCvar(pWeaponData[i], "AimBot_pSecondSilentFov", Aimbot::weapon_settings[i].AimBot_pSecondSilentFov);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_FOVTYPE, Aimbot::weapon_settings[i].AimBot_FovType);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_BESTHIT, Aimbot::weapon_settings[i].AimBot_BestHit);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_SPOT, Aimbot::weapon_settings[i].AimBot_Spot);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_DELAY, Aimbot::weapon_settings[i].AimBot_Delay);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_2FOV, Aimbot::weapon_settings[i].AimBot_Use2fovs);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_RCS, Aimbot::weapon_settings[i].AimBot_Rcs);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_RCSFOV, Aimbot::weapon_settings[i].AimBot_SecondFov);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_RCSSMOOTH, Aimbot::weapon_settings[i].AimBot_SecondSmooth);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AimBot_RcsAfterXShot, Aimbot::weapon_settings[i].AimBot_RcsAfterXShot);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_AUTOPISTOL, Aimbot::weapon_settings[i].AimBot_AutoPistol);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_RCSTYPE, Aimbot::weapon_settings[i].AimBot_RcsType);
				CSX::Cvar::SaveCvar(pWeaponData[i], "Skin_SkinID", g_SkinChangerCfg[pWeaponItemIndexData[i]].nFallbackPaintKit);
				CSX::Cvar::SaveCvar(pWeaponData[i], "Skin_Float", g_SkinChangerCfg[pWeaponItemIndexData[i]].flFallbackWear);
				CSX::Cvar::SaveCvar(pWeaponData[i], "Skin_StatTrack", g_SkinChangerCfg[pWeaponItemIndexData[i]].nFallbackStatTrak);
				CSX::Cvar::SaveCvar(pWeaponData[i], "Skin_LiveStatTrack", g_SkinChangerCfg[pWeaponItemIndexData[i]].LiveStatTrack);
				CSX::Cvar::SaveCvar(pWeaponData[i], "Skin_Rarity", g_SkinChangerCfg[pWeaponItemIndexData[i]].Rarity);
			}

			// автоматы
			if (i >= 10 && i <= 30)
			{
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_ACTIVE, Aimbot::weapon_settings[i].AimBot_Active);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_SMOOTH, Aimbot::weapon_settings[i].AimBot_Smooth);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_FOV, Aimbot::weapon_settings[i].AimBot_Fov);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_FOVTYPE, Aimbot::weapon_settings[i].AimBot_FovType);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_BESTHIT, Aimbot::weapon_settings[i].AimBot_BestHit);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_SPOT, Aimbot::weapon_settings[i].AimBot_Spot);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_DELAY, Aimbot::weapon_settings[i].AimBot_Delay);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_2FOV, Aimbot::weapon_settings[i].AimBot_Use2fovs);
				CSX::Cvar::SaveCvar(pWeaponData[i], "AimBot_pSecondSilentFov", Aimbot::weapon_settings[i].AimBot_pSilentFov);
				CSX::Cvar::SaveCvar(pWeaponData[i], "AimBot_pSecondSilentFov", Aimbot::weapon_settings[i].AimBot_pSecondSilentFov);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_RCS, Aimbot::weapon_settings[i].AimBot_Rcs);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_RCSFOV, Aimbot::weapon_settings[i].AimBot_SecondFov);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_RCSSMOOTH, Aimbot::weapon_settings[i].AimBot_SecondSmooth);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AimBot_RcsAfterXShot, Aimbot::weapon_settings[i].AimBot_RcsAfterXShot);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_RCSCLAMPTYPE, Aimbot::weapon_settings[i].AimBot_RcsClampType);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_RCSTYPE, Aimbot::weapon_settings[i].AimBot_RcsType);
				CSX::Cvar::SaveCvar(pWeaponData[i], "Skin_SkinID", g_SkinChangerCfg[pWeaponItemIndexData[i]].nFallbackPaintKit);
				CSX::Cvar::SaveCvar(pWeaponData[i], "Skin_Float", g_SkinChangerCfg[pWeaponItemIndexData[i]].flFallbackWear);
				CSX::Cvar::SaveCvar(pWeaponData[i], "Skin_StatTrack", g_SkinChangerCfg[pWeaponItemIndexData[i]].nFallbackStatTrak);
				CSX::Cvar::SaveCvar(pWeaponData[i], "Skin_LiveStatTrack", g_SkinChangerCfg[pWeaponItemIndexData[i]].LiveStatTrack);
				CSX::Cvar::SaveCvar(pWeaponData[i], "Skin_Rarity", g_SkinChangerCfg[pWeaponItemIndexData[i]].Rarity);
			}

			// снайперки
			if (i >= 31 && i <= 32)
			{
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_ACTIVE, Aimbot::weapon_settings[i].AimBot_Active);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_SMOOTH, Aimbot::weapon_settings[i].AimBot_Smooth);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_FOV, Aimbot::weapon_settings[i].AimBot_Fov);
				CSX::Cvar::SaveCvar(pWeaponData[i], "AimBot_pSecondSilentFov", Aimbot::weapon_settings[i].AimBot_pSilentFov);
				CSX::Cvar::SaveCvar(pWeaponData[i], "AimBot_pSecondSilentFov", Aimbot::weapon_settings[i].AimBot_pSecondSilentFov);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_2FOV, Aimbot::weapon_settings[i].AimBot_Use2fovs);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_FOVTYPE, Aimbot::weapon_settings[i].AimBot_FovType);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_BESTHIT, Aimbot::weapon_settings[i].AimBot_BestHit);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_SPOT, Aimbot::weapon_settings[i].AimBot_Spot);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_DELAY, Aimbot::weapon_settings[i].AimBot_Delay);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_RCS, Aimbot::weapon_settings[i].AimBot_Rcs);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_RCSFOV, Aimbot::weapon_settings[i].AimBot_SecondFov);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_RCSSMOOTH, Aimbot::weapon_settings[i].AimBot_SecondSmooth);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AimBot_RcsAfterXShot, Aimbot::weapon_settings[i].AimBot_RcsAfterXShot);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_RCSTYPE, Aimbot::weapon_settings[i].AimBot_RcsType);
				CSX::Cvar::SaveCvar(pWeaponData[i], "Skin_SkinID", g_SkinChangerCfg[pWeaponItemIndexData[i]].nFallbackPaintKit);
				CSX::Cvar::SaveCvar(pWeaponData[i], "Skin_Float", g_SkinChangerCfg[pWeaponItemIndexData[i]].flFallbackWear);
				CSX::Cvar::SaveCvar(pWeaponData[i], "Skin_StatTrack", g_SkinChangerCfg[pWeaponItemIndexData[i]].nFallbackStatTrak);
				CSX::Cvar::SaveCvar(pWeaponData[i], "Skin_LiveStatTrack", g_SkinChangerCfg[pWeaponItemIndexData[i]].LiveStatTrack);
				CSX::Cvar::SaveCvar(pWeaponData[i], "Skin_Rarity", g_SkinChangerCfg[pWeaponItemIndexData[i]].Rarity);
			}
		}

		CSX::Cvar::SaveCvar( VISUAL_TEXT , CVAR_ESP_STYLE , Settings::Esp::Visuals_Style );
		CSX::Cvar::SaveCvar( VISUAL_TEXT , CVAR_ESP_LINE , Settings::Esp::Visuals_Line );
		CSX::Cvar::SaveCvar( VISUAL_TEXT , CVAR_ESP_NightMode, Settings::Esp::NightMode);
		CSX::Cvar::SaveCvar( VISUAL_TEXT , CVAR_ESP_NAME , Settings::Esp::Visuals_Name );
		CSX::Cvar::SaveCvar( VISUAL_TEXT , CVAR_ESP_RANK , Settings::Esp::Visuals_Rank );
		CSX::Cvar::SaveCvar( VISUAL_TEXT , CVAR_ESP_HEALTH , Settings::Esp::Visuals_Health );
		CSX::Cvar::SaveCvar( VISUAL_TEXT , CVAR_ESP_ARMOR , Settings::Esp::Visuals_Armor );
		CSX::Cvar::SaveCvar( VISUAL_TEXT , CVAR_ESP_WEAPON , Settings::Esp::Visuals_Weapon );
		CSX::Cvar::SaveCvar( VISUAL_TEXT , CVAR_ESP_AMMO , Settings::Esp::Visuals_Ammo );
		CSX::Cvar::SaveCvar( VISUAL_TEXT , CVAR_ESP_SOUND , Settings::Esp::Visuals_Sound );
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_Visuals_SoundType, Settings::Esp::Visuals_SoundType);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_Visuals_SoundSize, Settings::Esp::Visuals_SoundSize);
	//	CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_Visuals_SoundDist, Settings::Esp::Visuals_SoundDist);

		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_Visuals_ChamsRainbow, Settings::Esp::Visuals_ChamsRainbow);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, "Visuals_HPBasedChams", Settings::Esp::Visuals_HPBasedChams);

		CSX::Cvar::SaveCvar( VISUAL_TEXT , CVAR_ESP_SKELETON , Settings::Esp::Visuals_Skeleton );
		CSX::Cvar::SaveCvar( VISUAL_TEXT , CVAR_ESP_TEAM , Settings::Esp::Visuals_Team );
		CSX::Cvar::SaveCvar( VISUAL_TEXT , CVAR_ESP_ENEMY , Settings::Esp::Visuals_Enemy );
		CSX::Cvar::SaveCvar( VISUAL_TEXT , CVAR_ESP_VISIBLE , Settings::Esp::Visuals_Visible );
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_TRACK, Settings::Esp::Visuals_track);
		CSX::Cvar::SaveCvar( VISUAL_TEXT , CVAR_ESP_CHAMS , Settings::Esp::Visuals_Chams );
		CSX::Cvar::SaveCvar( VISUAL_TEXT , CVAR_ESP_BOMB , Settings::Esp::Visuals_Bomb );
		CSX::Cvar::SaveCvar( VISUAL_TEXT , CVAR_ESP_BOMBTIMER , Settings::Esp::Visuals_BombTimer );
		CSX::Cvar::SaveCvar( VISUAL_TEXT , CVAR_ESP_WORLDWEAPONS , Settings::Esp::Visuals_WorldWeapons );
		CSX::Cvar::SaveCvar( VISUAL_TEXT , CVAR_ESP_WORLDGRENADE , Settings::Esp::Visuals_WorldGrenade );

		string Visuals_Color_CT =
			to_string( int( Settings::Colors::Visuals_Color_CT[0] * 255.f ) ) + "," +
			to_string( int( Settings::Colors::Visuals_Color_CT[1] * 255.f ) ) + "," +
			to_string( int( Settings::Colors::Visuals_Color_CT[2] * 255.f ) );

		string Visuals_Color_TT =
			to_string( int( Settings::Colors::Visuals_Color_TT[0] * 255.f ) ) + "," +
			to_string( int( Settings::Colors::Visuals_Color_TT[1] * 255.f ) ) + "," +
			to_string( int( Settings::Colors::Visuals_Color_TT[2] * 255.f ) );

		string Visuals_Color_VCT =
			to_string( int( Settings::Colors::Visuals_Color_VCT[0] * 255.f ) ) + "," +
			to_string( int( Settings::Colors::Visuals_Color_VCT[1] * 255.f ) ) + "," +
			to_string( int( Settings::Colors::Visuals_Color_VCT[2] * 255.f ) );

		string Menu_MainColor =
			to_string(int(Settings::Menu::Menu_MainColor[0] * 255.f)) + "," +
			to_string(int(Settings::Menu::Menu_MainColor[1] * 255.f)) + "," +
			to_string(int(Settings::Menu::Menu_MainColor[2] * 255.f)) + "," +
			to_string(int(Settings::Menu::Menu_MainColor[3] * 255.f));

		string Visuals_Color_VTT =
			to_string( int( Settings::Colors::Visuals_Color_VTT[0] * 255.f ) ) + "," +
			to_string( int( Settings::Colors::Visuals_Color_VTT[1] * 255.f ) ) + "," +
			to_string( int( Settings::Colors::Visuals_Color_VTT[2] * 255.f ) );

		string SoundVisuals_Color =
			to_string(int(Settings::Colors::SoundVisuals_Color[0] * 255.f)) + "," +
			to_string(int(Settings::Colors::SoundVisuals_Color[1] * 255.f)) + "," +
			to_string(int(Settings::Colors::SoundVisuals_Color[2] * 255.f));

		string WeaponEnemy_Color =
			to_string(int(Settings::Colors::WeaponEnemy_Color[0] * 255.f)) + "," +
			to_string(int(Settings::Colors::WeaponEnemy_Color[1] * 255.f)) + "," +
			to_string(int(Settings::Colors::WeaponEnemy_Color[2] * 255.f));

		string WorldWeapon_Color =
			to_string(int(Settings::Colors::WorldWeapon_Color[0] * 255.f)) + "," +
			to_string(int(Settings::Colors::WorldWeapon_Color[1] * 255.f)) + "," +
			to_string(int(Settings::Colors::WorldWeapon_Color[2] * 255.f));

		string Visuals_Line_Color =
			to_string(int(Settings::Colors::Visuals_Line_Color[0] * 255.f)) + "," +
			to_string(int(Settings::Colors::Visuals_Line_Color[1] * 255.f)) + "," +
			to_string(int(Settings::Colors::Visuals_Line_Color[2] * 255.f));

		string Name_Color =
			to_string(int(Settings::Colors::Name_Color[0] * 255.f)) + "," +
			to_string(int(Settings::Colors::Name_Color[1] * 255.f)) + "," +
			to_string(int(Settings::Colors::Name_Color[2] * 255.f));

		string Fraggrenade_Color =
			to_string(int(Settings::Colors::Fraggrenade_Color[0] * 255.f)) + "," +
			to_string(int(Settings::Colors::Fraggrenade_Color[1] * 255.f)) + "," +
			to_string(int(Settings::Colors::Fraggrenade_Color[2] * 255.f));

		string Flashbang_Color =
			to_string(int(Settings::Colors::Flashbang_Color[0] * 255.f)) + "," +
			to_string(int(Settings::Colors::Flashbang_Color[1] * 255.f)) + "," +
			to_string(int(Settings::Colors::Flashbang_Color[2] * 255.f));

		string Smokegrenade_Color =
			to_string(int(Settings::Colors::Smokegrenade_Color[0] * 255.f)) + "," +
			to_string(int(Settings::Colors::Smokegrenade_Color[1] * 255.f)) + "," +
			to_string(int(Settings::Colors::Smokegrenade_Color[2] * 255.f));

		string Skelet =
			to_string(int(Settings::Colors::Skelet[0] * 255.f)) + "," +
			to_string(int(Settings::Colors::Skelet[1] * 255.f)) + "," +
			to_string(int(Settings::Colors::Skelet[2] * 255.f));

		string Health_Color =
			to_string(int(Settings::Colors::Health_Color[0] * 255.f)) + "," +
			to_string(int(Settings::Colors::Health_Color[1] * 255.f)) + "," +
			to_string(int(Settings::Colors::Health_Color[2] * 255.f));

		string Armor_Color =
			to_string(int(Settings::Colors::Armor_Color[0] * 255.f)) + "," +
			to_string(int(Settings::Colors::Armor_Color[1] * 255.f)) + "," +
			to_string(int(Settings::Colors::Armor_Color[2] * 255.f));

		string GrenadeHelper =
			to_string(int(Settings::Colors::GrenadeHelper[0] * 255.f)) + "," +
			to_string(int(Settings::Colors::GrenadeHelper[1] * 255.f)) + "," +
			to_string(int(Settings::Colors::GrenadeHelper[2] * 255.f));

		string Menu_BackGround =
			to_string(int(Settings::Menu::Menu_BackGround[0] * 255.f)) + "," +
			to_string(int(Settings::Menu::Menu_BackGround[1] * 255.f)) + "," +
			to_string(int(Settings::Menu::Menu_BackGround[2] * 255.f)) + "," +
			to_string(int(Settings::Menu::Menu_BackGround[3] * 255.f));
		string Menu_BackGround1 =
			to_string(int(Settings::Menu::Menu_BackGround1[0] * 255.f)) + "," +
			to_string(int(Settings::Menu::Menu_BackGround1[1] * 255.f)) + "," +
			to_string(int(Settings::Menu::Menu_BackGround1[2] * 255.f)) + "," +
			to_string(int(Settings::Menu::Menu_BackGround1[3] * 255.f));
		string Menu_Text =
			to_string(int(Settings::Menu::Menu_Text[0] * 255.f)) + "," +
			to_string(int(Settings::Menu::Menu_Text[1] * 255.f)) + "," +
			to_string(int(Settings::Menu::Menu_Text[2] * 255.f)) + "," +
			to_string(int(Settings::Menu::Menu_Text[3] * 255.f));

		CSX::Cvar::SaveCvar("Colors", CVAR_ESP_COLOR_FRAGGRENADE, Fraggrenade_Color);
		CSX::Cvar::SaveCvar("Colors", CVAR_ESP_COLOR_FLASHBANG, Flashbang_Color);
		CSX::Cvar::SaveCvar("Colors", CVAR_ESP_COLOR_SMOKEGRENADE, Smokegrenade_Color);

		CSX::Cvar::SaveCvar("Colors", CVAR_ESP_COLOR_SKELET, Skelet);
		CSX::Cvar::SaveCvar("Colors", CVAR_ESP_COLOR_HEALTH, Health_Color);
		CSX::Cvar::SaveCvar("Colors", CVAR_ESP_COLOR_ARMOR, Armor_Color);

		CSX::Cvar::SaveCvar("Colors", CVAR_ESP_COLOR_SOUNDESP, SoundVisuals_Color);
		CSX::Cvar::SaveCvar("Colors", CVAR_ESP_COLOR_WEAPONENEMY, WeaponEnemy_Color);
		CSX::Cvar::SaveCvar("Colors", CVAR_ESP_COLOR_WORLDWEAPON, WorldWeapon_Color);
		CSX::Cvar::SaveCvar("Colors", CVAR_ESP_COLOR_LINE, Visuals_Line_Color);
		CSX::Cvar::SaveCvar("Colors", CVAR_ESP_COLOR_NAME, Name_Color);

		CSX::Cvar::SaveCvar( "Colors" , CVAR_ESP_COLOR_CT , Visuals_Color_CT );
		CSX::Cvar::SaveCvar( "Colors" , CVAR_ESP_COLOR_TT , Visuals_Color_TT );
		CSX::Cvar::SaveCvar( "Colors" , CVAR_ESP_COLOR_VCT , Visuals_Color_VCT );
		CSX::Cvar::SaveCvar( "Colors" , CVAR_ESP_COLOR_VTT , Visuals_Color_VTT );
		CSX::Cvar::SaveCvar( "Colors" , CVAR_ESP_COLOR_GrenadeHelper , GrenadeHelper);
		string chams_Color_CT =
			to_string( int( Settings::Colors::chams_Color_CT[0] * 255.f ) ) + "," +
			to_string( int( Settings::Colors::chams_Color_CT[1] * 255.f ) ) + "," +
			to_string( int( Settings::Colors::chams_Color_CT[2] * 255.f ) );

		string chams_Color_TT =
			to_string( int( Settings::Colors::chams_Color_TT[0] * 255.f ) ) + "," +
			to_string( int( Settings::Colors::chams_Color_TT[1] * 255.f ) ) + "," +
			to_string( int( Settings::Colors::chams_Color_TT[2] * 255.f ) );

		string chams_Color_VCT =
			to_string( int( Settings::Colors::chams_Color_VCT[0] * 255.f ) ) + "," +
			to_string( int( Settings::Colors::chams_Color_VCT[1] * 255.f ) ) + "," +
			to_string( int( Settings::Colors::chams_Color_VCT[2] * 255.f ) );

		string chams_Color_VTT =
			to_string( int( Settings::Colors::chams_Color_VTT[0] * 255.f ) ) + "," +
			to_string( int( Settings::Colors::chams_Color_VTT[1] * 255.f ) ) + "," +
			to_string( int( Settings::Colors::chams_Color_VTT[2] * 255.f ) );
		CSX::Cvar::SaveCvar( "Colors" , CVAR_CHAMS_COLOR_CT , chams_Color_CT );
		CSX::Cvar::SaveCvar( "Colors" , CVAR_CHAMS_COLOR_TT , chams_Color_TT );
		CSX::Cvar::SaveCvar( "Colors" , CVAR_CHAMS_COLOR_VCT , chams_Color_VCT );
		CSX::Cvar::SaveCvar( "Colors" , CVAR_CHAMS_COLOR_VTT , chams_Color_VTT );

		CSX::Cvar::SaveCvar(RADAR_TEXT, CVAR_RAD_ENABLE, Radar::Radar_Enable);
		CSX::Cvar::SaveCvar( RADAR_TEXT , CVAR_RAD_ACTIVE , Radar::Radar_Active );
		CSX::Cvar::SaveCvar( RADAR_TEXT , CVAR_RAD_TEAM , Radar::Radar_Team );
		CSX::Cvar::SaveCvar( RADAR_TEXT , CVAR_RAD_ENEMY , Radar::Radar_Enemy );
		CSX::Cvar::SaveCvar( RADAR_TEXT , CVAR_RAD_SOUND , Radar::Radar_Sound );
		CSX::Cvar::SaveCvar( RADAR_TEXT , CVAR_RAD_RANGE , Radar::Radar_Range );

		string Color_CT =
			to_string( int( Radar::Radar_Color_CT[0] * 255.f ) ) + "," +
			to_string( int( Radar::Radar_Color_CT[1] * 255.f ) ) + "," +
			to_string( int( Radar::Radar_Color_CT[2] * 255.f ) );

		string Color_TT =
			to_string( int( Radar::Radar_Color_TT[0] * 255.f ) ) + "," +
			to_string( int( Radar::Radar_Color_TT[1] * 255.f ) ) + "," +
			to_string( int( Radar::Radar_Color_TT[2] * 255.f ) );

		string Color_VCT =
			to_string( int( Radar::Radar_Color_VCT[0] * 255.f ) ) + "," +
			to_string( int( Radar::Radar_Color_VCT[1] * 255.f ) ) + "," +
			to_string( int( Radar::Radar_Color_VCT[2] * 255.f ) );

		string Color_VTT =
			to_string( int( Radar::Radar_Color_VTT[0] * 255.f ) ) + "," +
			to_string( int( Radar::Radar_Color_VTT[1] * 255.f ) ) + "," +
			to_string( int( Radar::Radar_Color_VTT[2] * 255.f ) );


		CSX::Cvar::SaveCvar( RADAR_TEXT , CVAR_RAD_COLOR_CT , Color_CT );
		CSX::Cvar::SaveCvar( RADAR_TEXT , CVAR_RAD_COLOR_TT , Color_TT );
		CSX::Cvar::SaveCvar( RADAR_TEXT , CVAR_RAD_COLOR_VCT , Color_VCT );
		CSX::Cvar::SaveCvar( RADAR_TEXT , CVAR_RAD_COLOR_VTT , Color_VTT );

		CSX::Cvar::SaveCvar(MISC_TEXT, CVAR_MISC_BACKTRACK, Misc::Other_Backtrack);
		CSX::Cvar::SaveCvar( MISC_TEXT , CVAR_MISC_BHOP , Misc::Other_Bhop );
		CSX::Cvar::SaveCvar( MISC_TEXT , CVAR_MISC_PUNCH , Misc::Other_Punch );
		CSX::Cvar::SaveCvar( MISC_TEXT , CVAR_MISC_NOFLASH , Misc::Other_NoFlash );
		CSX::Cvar::SaveCvar( MISC_TEXT , CVAR_MISC_AWPAIM , Misc::Other_AwpAim );
		CSX::Cvar::SaveCvar( MISC_TEXT , CVAR_MISC_AUTOSTRAFE , Misc::Other_AutoStrafe );
		CSX::Cvar::SaveCvar( MISC_TEXT , CVAR_MISC_AUTOACCEPT , Misc::Other_AutoAccept );
		CSX::Cvar::SaveCvar( MISC_TEXT , CVAR_MISC_SPECTATORS , Misc::Other_Spectators );
		CSX::Cvar::SaveCvar( MISC_TEXT , CVAR_MISC_FOV_CHANGER , Misc::Other_FovChanger );
		CSX::Cvar::SaveCvar(MISC_TEXT, CVAR_MISC_FOV_VIEW, Misc::Other_FovView);
		CSX::Cvar::SaveCvar( MISC_TEXT , CVAR_MISC_FOV_MDL_VIEW , Misc::Other_FovModelView );
		CSX::Cvar::SaveCvar(MISC_TEXT, CVAR_MISC_WIREHANDS, Misc::Other_WireHands);
		CSX::Cvar::SaveCvar(MISC_TEXT, CVAR_MISC_NOHANDS, Misc::Other_NoHands);
		CSX::Cvar::SaveCvar(MISC_TEXT, "EnableSkinchanger", Misc::EnableSkinchanger);
		CSX::Cvar::SaveCvar("Menu Settings", "Menu_UseCustomColors", Menu::Menu_UseCustomColors);
		CSX::Cvar::SaveCvar("Menu Settings", "Menu_MainColor", Menu_MainColor);
		CSX::Cvar::SaveCvar("Menu Settings", "Menu_BackGround", Menu_BackGround);
		CSX::Cvar::SaveCvar("Menu Settings", "Menu_BackGround1", Menu_BackGround1);
		CSX::Cvar::SaveCvar("Menu Settings", "Menu_Text", Menu_Text);

		CSX::Cvar::SaveCvar(MISC_TEXT, CVAR_MISC_BOMBY, Misc::bombtimery);
		CSX::Cvar::SaveCvar(MISC_TEXT, CVAR_MISC_BOMBX, Misc::bombtimerx);

		string Color_AWP_AIM =
			to_string( int( Misc::Other_AwpAimColor[0] * 255.f ) ) + "," +
			to_string( int( Misc::Other_AwpAimColor[1] * 255.f ) ) + "," +
			to_string( int( Misc::Other_AwpAimColor[2] * 255.f ) );

		CSX::Cvar::SaveCvar( MISC_TEXT , CVAR_MISC_AWPAIM_COLOR , Color_AWP_AIM );

		CSX::Cvar::SaveCvar(KNIFEBOT_TEXT, CVAR_KNIFEBOT_ACTIVE, Knifebot::knf_Active);
		CSX::Cvar::SaveCvar(KNIFEBOT_TEXT, CVAR_KNIFEBOT_TEAM, Knifebot::knf_Team);
		CSX::Cvar::SaveCvar(KNIFEBOT_TEXT, CVAR_KNIFEBOT_ATTACK, Knifebot::knf_Attack);
		CSX::Cvar::SaveCvar(KNIFEBOT_TEXT, CVAR_KNIFEBOT_DISTATTACK, Knifebot::knf_DistAttack);
		CSX::Cvar::SaveCvar(KNIFEBOT_TEXT, CVAR_KNIFEBOT_DISTATTACK2, Knifebot::knf_DistAttack2);

		CSX::Cvar::SaveCvar(SKIN_TEXT, CVAR_SKIN_CT_MODEL, Skin::knf_ct_model);
		CSX::Cvar::SaveCvar(SKIN_TEXT, CVAR_SKIN_CT_SKIN, Skin::knf_ct_skin);
		CSX::Cvar::SaveCvar(SKIN_TEXT, CVAR_SKIN_TT_MODEL, Skin::knf_tt_model);
		CSX::Cvar::SaveCvar(SKIN_TEXT, CVAR_SKIN_TT_SKIN, Skin::knf_tt_skin);
		CSX::Cvar::SaveCvar(SKIN_TEXT, CVAR_SKIN_GLOVES, Skin::gloves_skin);
	}
	float hitmarkerAlpha;

	namespace InventoryChanger 
	{
		bool enabled = true;
		int Coin[10] = { 0,0,0,0,0,0,0,0,0,0 };
		int Slot = 1;
		bool enbl;
		bool EnableCoins = false;
		int Rank;
		int Wins;
		int Rec1;
		int Rec2;
		int Rec3;
		int Level;
		int XP;
		// айди медалей для примера
	}

	namespace Aimbot
	{
		bool AimBot_Deathmatch = false;
		bool AimBot_WallAttack = false;
		bool AimBot_CheckSmoke = false;
		bool AimBot_AntiJump = false;
		bool AimBot_DrawFov = false;
		bool AimBot_JumpCheckME = false;
		bool AimBot_DrawSpot = false;
		int BackTrackTicks = 12;
		bool AimBot_pSilent = false;

		weapon_settings_s weapon_settings[33] = { 0 };
	}

	namespace Esp
	{
		int Visuals_Style = 0; // 0 - Box 1 - CoalBox
		int Visuals_Size = 1;
		bool Visuals_Line = false;
		int Visuals_SoundType = 1;
		int Visuals_SoundSize = 9;
		int Visuals_SoundDist = 1200;
		int RenderTypeBox = 1;
		bool Visuals_Name = false;
		bool Visuals_Rank = false;
		int Visuals_Health = 0;
		int Visuals_Armor = 0;
		bool Visuals_Weapon = false;
		bool Visuals_HitMarker = false;
		int Visuals_HitMarkerSound = 0;
		float Visuals_HitMarkerColor[3] = { 0.f, 0.f, 0.f };
		bool Visuals_Ammo = false;
		bool Visuals_Distance = false;
		bool Visuals_Sound = false;
		bool Visuals_ChamsRainbow = false;
		bool NightMode = false;
		bool Visuals_Skeleton = 0;
		bool helper = false;
		bool Visuals_Team = 1;
		bool Visuals_Enemy = 1;
		bool Visuals_track = false;
		int Visuals_Visible = 1;
		bool Visuals_GrenadePrediction = false;
		int Visuals_Chams = 0;
		bool Visuals_Bomb = false;
		int Visuals_BombTimer = 40;
		bool Visuals_WorldWeapons = false;
		bool Visuals_HPBasedChams = false;
		bool Visuals_WorldGrenade = false;
	}

	namespace Knifebot
	{
		bool knf_Active = false;
		bool knf_Team = false;
		int knf_Attack = 2;
		int knf_DistAttack = 72;
		int knf_DistAttack2 = 64;
	}

	namespace Radar
	{
		bool Radar_Enable = false;
		bool Radar_Active = false;
		bool Radar_Team = false;
		bool Radar_Enemy = false;
		bool Radar_Sound = false;
		bool Radar_InGame = false;
		int Radar_Range = 3500;
		int Radar_Alpha = 255;
		float Radar_Color_CT[3] = { 0.f,0.0f,0.f };
		float Radar_Color_TT[3] = { 0.f,0.0f,0.f };
		float Radar_Color_VCT[3] = { 0.f,0.f,0.f };
		float Radar_Color_VTT[3] = { 0.f,0.f,0.f };
	}

	namespace Skin
	{
		int knf_ct_model = 0;
		int knf_ct_skin = 0;
		int knf_tt_model = 0;
		int knf_tt_skin = 0;
		int gloves_skin = 0;
		int act_skin = 0;
	}

	namespace Misc
	{
		bool Other_Bhop = false;
		bool Other_Punch = false;
		bool Other_NoFlash = false;
		bool Other_AwpAim = false;
		bool Other_Backtrack = false;
		bool Other_AutoStrafe = false;
		bool Other_AutoAccept = false;
		bool Other_Spectators = false;
		bool Other_FovChanger = false;
		bool Other_Color = false;
		bool misc_Name = false;
		bool EnableSkinchanger = false;
		int Other_ArmMaterialsList = 0;
		int	Other_ArmMaterialsType = 0;
		bool Other_ArmMaterials = false;
		int bombtimerx = 12;
		int bombtimery = 12;
		bool Other_MemeWalk = false;
		bool Other_spamregular = false;
		int Other_FovView = 90;
		int Other_FovModelView = 60;
		float Other_AwpAimColor[3] = { 0.f,0.f,0.f };
		bool Other_WireHands = false;
		bool Other_NoHands = false;
		bool Experemental = false;
	}
	namespace Colors
	{
		float Skelet[3] = { 0.f,0.0f,0.f };
		float Fraggrenade_Color[3] = { 0.f,0.0f,0.f };
		float Flashbang_Color[3] = { 0.f,0.0f,0.f };
		float Smokegrenade_Color[3] = { 0.f,0.0f,0.f };
		float GrenadeHelper[3] = { 0.f,0.f,0.f };
		float SoundVisuals_Color[3] = { 0.f,0.0f,0.f };
		float WeaponEnemy_Color[3] = { 0.f,0.0f,0.f };
		float WorldWeapon_Color[3] = { 0.f,0.0f,0.f };
		float Visuals_Line_Color[3] = { 0.f,0.0f,0.f };
		float Name_Color[3] = { 0.f,0.0f,0.f };
		float Health_Color[3] = { 0.f,0.0f,0.f };
		float Armor_Color[3] = { 0.f,0.0f,0.f };
		float Visuals_Color_CT[3] = { 0.f,0.0f,0.f };
		float Visuals_Color_TT[3] = { 0.f,0.0f,0.f };
		float Visuals_Color_VCT[3] = { 0.f,0.0f,0.f };
		float Visuals_Color_VTT[3] = { 0.f,0.0f,0.f };
		float chams_Color_CT[3] = { 0.f,0.0f,0.f };
		float chams_Color_TT[3] = { 0.f,0.0f,0.f };
		float chams_Color_VCT[3] = { 0.f,0.0f,0.f };
		float chams_Color_VTT[3] = { 0.f,0.0f,0.f };
	}
	namespace Menu
	{
		bool Menu_UseCustomColors = false;
		float Menu_MainColor[3] = { 0.f,0.f,0.f };
		float Menu_BackGround1[3] = { 0.f,0.f,0.f };
		float Menu_BackGround[3] = { 0.f,0.f,0.f };
		float Menu_Text[3] = { 0.f,0.f,0.f };
		bool Menu_Aim = 0;
	}
}