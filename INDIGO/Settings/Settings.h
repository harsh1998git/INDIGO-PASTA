#pragma once

#include "../Main/Main.h"

#define FIX_MIN_FOV_HEAD	30
#define FIX_MIN_FOV_CHEST	50
#define FOV_BASE_DISTANCE	200

namespace Settings
{
	void LoadSettings(string szIniFile);
	void SaveSettings(string szIniFile);
	extern float hitmarkerAlpha;

	namespace InventoryChanger 
	
	{
		extern int Coin[];
		extern int Slot;
		extern bool enabled;
		extern bool EnableCoins;
		extern bool enbl;
		extern int Rank;
		extern int Wins;
		extern int Rec1;
		extern int Rec2;
		extern int Rec3;
		extern int Level;
		extern int XP;
	}

	namespace Aimbot
	{
		struct weapon_settings_s
		{
			//[swap_lines]
			bool AimBot_Active;
			bool AimBot_pSilent;
			float AimBot_Smooth;
			float AimBot_Fov;
			int AimBot_FovType;
			bool AimBot_BestHit;
			bool AimBot_Use2fovs;
			int AimBot_FType;
			int AimBot_Spot;
			int AimBot_Delay;
			int AimBot_Rcs;
			float AimBot_SecondSmooth;
			int AimBot_RcsAfterXShot;
			float AimBot_SecondFov;
			bool AimBot_RcsClampType;
			bool AimBot_AutoPistol;
			float AimBot_pSecondSilentFov;
			float AimBot_pSilentFov;
			int AimBot_RcsType;
			//[/swap_lines]
		};
		//[swap_lines]
		extern int BackTrackTicks;
		extern bool AimBot_Deathmatch;
		extern bool AimBot_WallAttack;
		extern bool AimBot_StandalonRCS;
		extern bool AimBot_CheckSmoke;
		extern bool AimBot_JumpCheckME;
		extern bool AimBot_AntiJump;
		extern bool AimBot_DrawFov;
		extern bool AimBot_DrawSpot;
		extern weapon_settings_s weapon_settings[33];
		//[/swap_lines]
	}

	namespace Esp
	{
		//[swap_lines]
		extern int Visuals_Style; // 0 - Box 1 - CoalBox
		extern int Visuals_Size;
		extern bool Visuals_Line;
		extern bool Visuals_Name;
		extern bool Visuals_Rank;
		extern bool Visuals_HitMarker;
		extern int Visuals_HitMarkerSound;
		extern float Visuals_HitMarkerColor[3];
		extern int Visuals_Health;
		extern int Visuals_Armor;
		extern bool Visuals_Weapon;
		extern int Visuals_SoundType;
		extern int Visuals_SoundSize;
		extern int Visuals_SoundDist;
		extern bool Visuals_Ammo;
		extern bool Visuals_Distance;
		extern bool Visuals_Sound;
		extern int RenderTypeBox;
		extern bool Visuals_Skeleton;
		extern bool helper;
		extern bool Visuals_HPBasedChams;
		extern bool Visuals_Team;
		extern bool Visuals_Enemy;
		extern int Visuals_Visible;
		extern bool NightMode;
		extern bool Visuals_ChamsRainbow;
		extern bool Visuals_track;
		extern int Visuals_Chams;
		extern bool Visuals_Bomb;
		extern int Visuals_BombTimer;
		extern bool Visuals_WorldWeapons;
		extern bool Visuals_WorldGrenade;
		extern bool Visuals_GrenadePrediction;
		//[/swap_lines]
	}

	namespace Radar
	{
		//[swap_lines]
		extern bool Radar_Enable;
		extern bool Radar_Active;
		extern bool Radar_Team;
		extern bool Radar_Enemy;
		extern bool Radar_Sound;
		extern bool Radar_InGame;
		extern int Radar_Range;
		extern int Radar_Alpha;
		extern float Radar_Color_CT[3];
		extern float Radar_Color_TT[3];
		extern float Radar_Color_VCT[3];
		extern float Radar_Color_VTT[3];
		//[/swap_lines]
	}

	namespace Skin
	{
		//[swap_lines]
		extern int knf_ct_model;
		extern int knf_ct_skin;
		extern int knf_tt_model;
		extern int knf_tt_skin;
		extern int gloves_skin;
		//[/swap_lines]
	}

	namespace Misc
	{
		//[swap_lines]
		extern bool Other_Bhop;
		extern bool Other_Backtrack;
		extern bool Other_Punch;
		extern bool Other_MemeWalk;
		extern bool Other_AwpAim;
		extern bool Other_NoFlash;
		extern bool Other_AutoStrafe;
		extern bool Other_AutoAccept;
		extern bool Other_Spectators;
		extern bool Other_FovChanger;
		extern int bombtimerx;
		extern int bombtimery;
		extern int Other_FovView;
		extern int Other_FovModelView;
		extern int Other_ArmMaterialsList;
		extern int Other_ArmMaterialsType;
		extern bool misc_Name;
		extern bool Other_ArmMaterials;
		extern float Other_AwpAimColor[3];
		extern bool Other_WireHands;
		extern bool EnableSkinchanger;
		extern bool Other_NoHands;
		extern bool Experemental;
		//[/swap_lines]
	}

	namespace Knifebot
	{
		//[swap_lines]
		extern bool knf_Active;
		extern bool knf_Team;
		extern int knf_Attack;
		extern int knf_DistAttack;
		extern int knf_DistAttack2;
		//[/swap_lines]
	}
	namespace Colors
	{
		//[swap_lines]
		extern float Fraggrenade_Color[3];
		extern float Flashbang_Color[3];
		extern float Smokegrenade_Color[3];
		extern float SoundVisuals_Color[3];
		extern float WeaponEnemy_Color[3];
		extern float WorldWeapon_Color[3];
		extern float Visuals_Line_Color[3];
		extern float Name_Color[3];
		extern float GrenadeHelper[3];
		extern float Skelet[3];
		extern float Health_Color[3];
		extern float Armor_Color[3];
		extern float Visuals_Color_CT[3];
		extern float Visuals_Color_TT[3];
		extern float Visuals_Color_VCT[3];
		extern float Visuals_Color_VTT[3];
		extern float chams_Color_CT[3];
		extern float chams_Color_TT[3];
		extern float chams_Color_VCT[3];
		extern float chams_Color_VTT[3];
		//[/swap_lines]
	}
	namespace Menu
	{
		//[swap_lines]
		extern float Menu_MainColor[3];
		extern float Menu_BackGround[3];
		extern float Menu_BackGround1[3];
		extern float Menu_Text[3];
		extern bool Menu_UseCustomColors;
		extern bool Menu_Aim;
		//[/swap_lines]
	}
}

//[enc_string_enable /]

#define AIMBOT_TEXT "Aimbot"
#define CONFIG_TEXT "Config"
#define TRIGGER_TEXT "TriggerBot"
#define VISUAL_TEXT "Visual"
#define RADAR_TEXT "Radar"
#define SKIN_TEXT "Skin"
#define KNIFEBOT_TEXT "Knife"
#define MISC_TEXT "Misc"
#define COLOR_TEXT "Colors"

#define CVAR_KNIFEBOT_ACTIVE "knf_Active"
#define CVAR_KNIFEBOT_TEAM "knf_Team"
#define CVAR_KNIFEBOT_ATTACK "knf_Attack"
#define CVAR_KNIFEBOT_DISTATTACK "knf_DistAttack"
#define CVAR_KNIFEBOT_DISTATTACK2 "knf_DistAttack2"

#define CVAR_AIMBOT_DEATHMATCH "AimBot_Deathmatch"
#define CVAR_AIMBOT_WALLATTACK "AimBot_WallAttack"
#define CVAR_AIMBOT_CHECKSMOKE "AimBot_CheckSmoke"
#define CVAR_AIMBOT_JUMPCHEKME "AimBot_JumpCheckME"
#define CVAR_AIMBOT_ANTIJUMP	"AimBot_IgnoreJump"

#define CVAR_AIMBOT_RCSTYPE "AimBot_RcsType"
#define CVAR_AIMBOT_DRAWFOV "AimBot_DrawFov"
#define CVAR_AIMBOT_DRAWSPOT "AimBot_DrawSpot"
#define CVAR_BackTrackTicks "BackTrackTicks"
#define CVAR_AIMBOT_ACTIVE "AimBot_Active"
#define CVAR_AIMBOT_SMOOTH "AimBot_Smooth"
#define CVAR_AIMBOT_FOV "AimBot_Fov"
#define CVAR_AIMBOT_FOVTYPE "AimBot_FovType"
#define CVAR_AIMBOT_FTYPE "AimBot_FType"
#define CVAR_AIMBOT_BESTHIT "AimBot_BestHit"
#define CVAR_AIMBOT_SPOT "AimBot_Spot"
#define CVAR_AIMBOT_DELAY "AimBot_Delay"
#define CVAR_AimBot_RcsStopAfterXShot "AimBot_RcsStopAfterXShot"
#define CVAR_AimBot_RcsAfterXShot "AimBot_RcsAfterXShot"
#define CVAR_AIMBOT_RCS "AimBot_Rcs"

#define CVAR_AIMBOT_2FOV "AimBot_Use2fovs"
#define CVAR_AIMBOT_RCSFOV "AimBot_SecondFov"

#define CVAR_AIMBOT_pFOV "aim_pSilentFov"
#define CVAR_AIMBOT_RCSSMOOTH "AimBot_SecondSmooth"
#define CVAR_AIMBOT_RCSCLAMPTYPE "AimBot_RcsClampType"
#define CVAR_AIMBOT_AUTOPISTOL "AimBot_AutoPistol"

#define CVAR_ESP_STYLE "Visuals_Style"
#define CVAR_ESP_SIZE "Visuals_Size"
#define CVAR_ESP_LINE "Visuals_Line"
#define CVAR_ESP_OUTLINE "Visuals_Outline"
#define CVAR_ESP_NAME "Visuals_Name"
#define CVAR_ESP_RANK "Visuals_Rank"

#define CVAR_ESP_HEALTH "Visuals_Health"
#define CVAR_ESP_ARMOR "Visuals_Armor"
#define CVAR_ESP_WEAPON "Visuals_Weapon"
#define CVAR_ESP_AMMO "Visuals_Ammo"
#define CVAR_ESP_DISTANCE "Visuals_Distance"
#define CVAR_ESP_SOUND "Visuals_Sound"

#define CVAR_Visuals_SoundType "Visuals_SoundType"
#define CVAR_Visuals_SoundDist "Visuals_SoundDist"
#define CVAR_Visuals_SoundSize "Visuals_SoundSize"

#define CVAR_ESP_TRACK "Visuals_track"
#define CVAR_ESP_SKELETON "Visuals_Skeleton"
#define CVAR_ESP_BULLETTRACE "Visuals_BulletTrace"
#define CVAR_ESP_TEAM "Visuals_Team"
#define CVAR_ESP_ENEMY "Visuals_Enemy"
#define CVAR_ESP_VISIBLE "Visuals_Visible"
#define CVAR_MenuBackGround "MenuBackGround"
#define CVAR_ESP_COLOR_SKELET "Skelet"
#define CVAR_ESP_COLOR_GrenadeHelper "GrenadeHelper"
#define CVAR_ESP_COLOR_HEALTH "Health_Color"
#define CVAR_ESP_COLOR_ARMOR "Armor_Color"
#define CVAR_ESP_COLOR_FRAGGRENADE "Fraggrenade_Color"
#define CVAR_ESP_COLOR_FLASHBANG "Flashbang_Color"
#define CVAR_ESP_COLOR_SMOKEGRENADE "Smokegrenade_Color"

#define CVAR_ESP_Visuals_XQZ "Visuals_XQZ"
#define CVAR_ESP_Visuals_ChamsRainbow "Visuals_ChamsRainbow"

#define CVAR_ESP_COLOR_SOUNDESP "SoundVisuals_Color"
#define CVAR_ESP_COLOR_WEAPONENEMY "WeaponEnemy_Color"
#define CVAR_ESP_COLOR_WORLDWEAPON "WorldWeapon_Color"
#define CVAR_ESP_COLOR_LINE "Visuals_Line_Color"
#define CVAR_ESP_COLOR_NAME "Name_Color"

#define CVAR_ESP_CHAMS "Visuals_Chams"
#define CVAR_ESP_BOMB "Visuals_Bomb"
#define CVAR_ESP_BOMBTIMER "Visuals_BombTimer"
#define CVAR_ESP_WORLDWEAPONS "Visuals_WorldWeapons"
#define CVAR_ESP_WORLDGRENADE "Visuals_WorldGrenade"
#define CVAR_ESP_NightMode "NightMode"
#define CVAR_ESP_COLOR_CT "Visuals_Color_CT"
#define CVAR_ESP_COLOR_TT "Visuals_Color_TT"
#define CVAR_ESP_COLOR_VCT "Visuals_Color_VCT"
#define CVAR_ESP_COLOR_VTT "Visuals_Color_VTT"

#define CVAR_CHAMS_COLOR_CT "chams_Color_CT"
#define CVAR_CHAMS_COLOR_TT "chams_Color_TT"
#define CVAR_CHAMS_COLOR_VCT "chams_Color_VCT"
#define CVAR_CHAMS_COLOR_VTT "chams_Color_VTT"

#define C4_TIMER1_STRING "C4: %0.2f"
#define C4_TIMER_STRING "C4 Timer: %0.2f"

#define CVAR_RAD_ENABLE "Radar_Enable"
#define CVAR_RAD_ACTIVE "Radar_Active"
#define CVAR_RAD_TEAM "Radar_Team"
#define CVAR_RAD_ENEMY "Radar_Enemy"
#define CVAR_RAD_SOUND "Radar_Sound"
#define CVAR_RAD_INGAME "Radar_InGame"
#define CVAR_RAD_RANGE "Radar_Range"
#define CVAR_RAD_ALPHA "Radar_Alpha"

#define CVAR_RAD_COLOR_CT "Radar_Color_CT"
#define CVAR_RAD_COLOR_TT "Radar_Color_TT"
#define CVAR_RAD_COLOR_VCT "Radar_Color_VCT"
#define CVAR_RAD_COLOR_VTT "Radar_Color_VTT"

#define CVAR_SKIN_CT_MODEL "knf_ct_model"
#define CVAR_SKIN_CT_SKIN "knf_ct_skin"
#define CVAR_SKIN_TT_MODEL "knf_tt_model"
#define CVAR_SKIN_TT_SKIN "knf_tt_skin"
#define CVAR_SKIN_GLOVES "gloves_skin"

#define CVAR_MISC_BACKTRACK "Other_Backtrack"
#define CVAR_MISC_BHOP "Other_Bhop"
#define CVAR_MISC_PUNCH "Other_Punch"
#define CVAR_MISC_AWPAIM "Other_AwpAim"
#define CVAR_MISC_RADSTYLE "Other_RadStyle"
#define CVAR_MISC_CROSSHAIR "Other_CrossStyle"
#define CVAR_MISC_NOFLASH "Other_NoFlash"
#define CVAR_MISC_AUTOSTRAFE "Other_AutoStrafe"
#define CVAR_MISC_AUTOACCEPT "Other_AutoAccept"
#define CVAR_MISC_SPECTATORS "Other_Spectators"
#define CVAR_MISC_FOV_CHANGER "Other_FovChanger"
#define CVAR_MISC_FOV_VIEW "Other_FovView"
#define CVAR_MISC_FOV_MDL_VIEW "Other_FovModelView"
#define CVAR_MISC_AWPAIM_COLOR "Other_AwpAimColor"
#define CVAR_MISC_NOHANDS "Other_NoHands"
#define CVAR_MISC_WIREHANDS "Other_WireHands"
#define CVAR_MISC_BOMBX "bombtimerx"

#define CVAR_MENU1 "Menu_MainColor"
#define CVAR_MENU2 "Menu_BackGround1"
#define CVAR_MENU3 "Menu_BackGround"
#define CVAR_MENU4 "Menu_Text"
#define CVAR_MISC_BOMBY "bombtimery"
