#include "Client.h"
#include "../image.h"
#include "../GranadeHelper/CGrenadeAPI.h"
#pragma comment(lib, "Urlmon.lib")
#pragma comment(lib, "Shell32.lib")
#include <windows.h>
#include <locale.h>
#include <fstream>
#include <urlmon.h>
#include <io.h>
#include <shellapi.h>
#pragma comment(lib, "Urlmon.lib")
#pragma comment(lib, "Shell32.lib")
#include <windows.h>
#include <locale.h>
//[enc_string_enable /]
//[junk_enable /]
IDirect3DTexture9 *tImage = nullptr;

bool IsProcessRun(const char * const processName)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);
	Process32First(hSnapshot, &pe);

	while (1) {
		if (strcmp(pe.szExeFile, processName) == 0) return true;
		if (!Process32Next(hSnapshot, &pe)) return false;
	}
}


namespace Client
{
	//[swap_lines]
	int	iScreenWidth = 0;
	int	iScreenHeight = 0;

	string BaseDir = "";
	string LogFile = "";
	string GuiFile = "";
	string IniFile = "";

	vector<string> ConfigList;

	Vector2D	g_vCenterScreen = Vector2D(0.f, 0.f);

	CPlayers*	g_pPlayers = nullptr;
	CRender*	g_pRender = nullptr;
	CGui*		g_pGui = nullptr;

	CAimbot*	g_pAimbot = nullptr;
	CEsp*		g_pEsp = nullptr;
	CKnifebot*	g_pKnifebot = nullptr;
	CRadar*		g_pRadar = nullptr;
	CSkin*		g_pSkin = nullptr;
	CMisc*		g_pMisc = nullptr;
	CInventoryChanger* g_pInventoryChanger = nullptr;

	bool		bC4Timer = false;
	int			iC4Timer = 40;

	static char clantag[128] = { 0 };
	static char spamchat[128] = { 0 };

	float SpaceLineOne = 140.f;
	float SpaceLine23 = 120.f;
	float SpaceLineTwo = 255.f;
	float SpaceLineThr = 390.f;
	float SpaceLineTwo2 = 225.f;

	float SpaceLine1 = 170.f;
	float SpaceLine228 = 150.f;
	float SpaceLine2 = 360.f;
	float SpaceLine5 = 120.f;

	int			iWeaponID = 0;
	int			iWeaponSelectIndex = WEAPON_DEAGLE;
	int			iWeaponSelectSkinIndex = -1;
	//[/swap_lines]

	void ReadConfigs(LPCTSTR lpszFileName)
	{
		if (!strstr(lpszFileName, "gui.ini"))
		{
			ConfigList.push_back(lpszFileName);
		}
	}

	void RefreshConfigs()
	{
		ConfigList.clear();
		string ConfigDir = BaseDir + "\\*.ini";
		SearchFiles(ConfigDir.c_str(), ReadConfigs, FALSE);
	}

	bool SendClientHello()
	{
		CMsgClientHello Message;
		Message.set_client_session_need(1);
		Message.clear_socache_have_versions();
		void* ptr = malloc(Message.ByteSize() + 8);
		if (!ptr)
			return false;
		((uint32_t*)ptr)[0] = k_EMsgGCClientHello | ((DWORD)1 << 31);
		((uint32_t*)ptr)[1] = 0;
		Message.SerializeToArray((void*)((DWORD)ptr + 8), Message.ByteSize());
		bool result = Interfaces::SteamGameCoordinator()->SendMessage(k_EMsgGCClientHello | ((DWORD)1 << 31), ptr, Message.ByteSize() + 8) == k_EGCResultOK;
		free(ptr);
		return result;
	}

	bool SendMMHello()
	{
		CMsgGCCStrike15_v2_MatchmakingClient2GCHello Message;
		void* ptr = malloc(Message.ByteSize() + 8);
		if (!ptr)
			return false;

		auto unMsgType = k_EMsgGCCStrike15_v2_MatchmakingClient2GCHello | ((DWORD)1 << 31);
		((uint32_t*)ptr)[0] = unMsgType;
		((uint32_t*)ptr)[1] = 0;

		Message.SerializeToArray((void*)((DWORD)ptr + 8), Message.ByteSize());

		bool result = Interfaces::SteamGameCoordinator()->SendMessage(unMsgType, ptr, Message.ByteSize() + 8) == k_EGCResultOK;

		free(ptr);
		return result;
	}

	bool Initialize(IDirect3DDevice9* pDevice)
	{
		g_pPlayers = new CPlayers();
		g_pRender = new CRender(pDevice);
		g_pGui = new CGui();

		g_pAimbot = new CAimbot();
		g_pEsp = new CEsp();
		g_pRadar = new CRadar();
		g_pSkin = new CSkin();
		g_pMisc = new CMisc();
		g_pKnifebot = new CKnifebot();
		g_pInventoryChanger = new CInventoryChanger();

		GuiFile = BaseDir + "\\" + "gui.ini";
		IniFile = BaseDir + "\\" + "settings.ini";

	//	g_pSkin->InitalizeSkins();

		iWeaponSelectSkinIndex = GetWeaponSkinIndexFromPaintKit(g_SkinChangerCfg[iWeaponSelectIndex].nFallbackPaintKit);

		if (tImage == nullptr)D3DXCreateTextureFromFileInMemoryEx(pDevice, &NameArry, sizeof(NameArry),
			180, 220, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &tImage);

		g_pGui->GUI_Init(pDevice);

		RefreshConfigs();
		SendClientHello();

		return true;
	}

	void Shutdown()
	{
		DELETE_MOD(g_pPlayers);
		DELETE_MOD(g_pRender);
		DELETE_MOD(g_pGui);

		DELETE_MOD(g_pAimbot);
		DELETE_MOD(g_pEsp);
		DELETE_MOD(g_pRadar);
		DELETE_MOD(g_pKnifebot);
		DELETE_MOD(g_pSkin);
		DELETE_MOD(g_pMisc);
	}

	void OnRender()
	{
		if (g_pRender && !Interfaces::Engine()->IsTakingScreenshot() && Interfaces::Engine()->IsActiveApp())
		{
			// Открытие меню без net_graph 1
			g_pRender->BeginRender();

			if (g_pGui)
				g_pGui->GUI_Draw_Elements();

			Interfaces::Engine()->GetScreenSize(iScreenWidth, iScreenHeight);

			g_vCenterScreen.x = iScreenWidth / 2.f;
			g_vCenterScreen.y = iScreenHeight / 2.f;

			// Ватермарк
			if (!Interfaces::Engine()->IsConnected())
			{
				static float rainbow1; rainbow1 += 0.0005f; if
					(rainbow1 > 0.999f) rainbow1 = 0.f;
				g_pRender->Text(15, 15, false, true, Color::FromHSB(rainbow1, 1.f, 1.f), "FOX-PROJECT");
			}

			bool bOpenTimer = (bIsGuiVisible || (bC4Timer && iC4Timer));

			if (g_pEsp && Settings::Esp::Visuals_BombTimer && bOpenTimer)
			{
				g_pRender->Text(int(Settings::Misc::bombtimerx), int(Settings::Misc::bombtimery), false, true, Color::White(), C4_TIMER_STRING, g_pEsp->fC4Timer);
			}

			if (Client::g_pPlayers && Client::g_pPlayers->GetLocal() && Interfaces::Engine()->IsInGame())
			{
				if (g_pEsp)
					g_pEsp->OnRender();

				if (g_pMisc)
				{
					g_pMisc->OnRender();
					g_pMisc->OnRenderSpectatorList();
				}
			}

			g_pRender->EndRender();
		}
	}

	void OnLostDevice()
	{
		if (g_pRender)
			g_pRender->OnLostDevice();

		if (g_pGui)
			ImGui_ImplDX9_InvalidateDeviceObjects();
	}

	void OnResetDevice()
	{
		if (g_pRender)
			g_pRender->OnResetDevice();

		if (g_pGui)
			ImGui_ImplDX9_CreateDeviceObjects();
	}

	void OnRetrieveMessage(void* ecx, void* edx, uint32_t *punMsgType, void *pubDest, uint32_t cubDest, uint32_t *pcubMsgSize)
	{
		g_pInventoryChanger->PostRetrieveMessage(punMsgType, pubDest, cubDest, pcubMsgSize);
	}
	void OnSendMessage(void* ecx, void* edx, uint32_t unMsgType, const void* pubData, uint32_t cubData)
	{
		void* pubDataMutable = const_cast<void*>(pubData);
		g_pInventoryChanger->PreSendMessage(unMsgType, pubDataMutable, cubData);
	}

	void OnCreateMove(CUserCmd* pCmd)
	{
		if (g_pPlayers && Interfaces::Engine()->IsInGame())
		{

			g_pPlayers->Update();

			if (g_pEsp)
				g_pEsp->OnCreateMove(pCmd);

			if (IsLocalAlive())
			{
				if (!bIsGuiVisible)
				{
					int iWeaponSettingsSelectID = GetWeaponSettingsSelectID();

					if (iWeaponSettingsSelectID >= 0)
						iWeaponID = iWeaponSettingsSelectID;
				}

				if (g_pAimbot)
					g_pAimbot->OnCreateMove(pCmd, g_pPlayers->GetLocal());

				if (g_pKnifebot)
					g_pKnifebot->OnCreateMove(pCmd);

				if (g_pMisc)
					g_pMisc->OnCreateMove(pCmd);
			}
		}
	}

	void OnFireEventClientSideThink(IGameEvent* pEvent)
	{
		if (!strcmp(pEvent->GetName(), "player_connect_full") ||
			!strcmp(pEvent->GetName(), "round_start") ||
			!strcmp(pEvent->GetName(), "cs_game_disconnected"))
		{
			if (g_pPlayers)
				g_pPlayers->Clear();

			if (g_pEsp)
				g_pEsp->OnReset();
		}

		if (Interfaces::Engine()->IsConnected())
		{
			if (g_pEsp)
				g_pEsp->OnEvents(pEvent);

			if (g_pSkin)
				g_pSkin->OnEvents(pEvent);
		}
	}

	void OnFrameStageNotify(ClientFrameStage_t Stage)
	{
		if (Interfaces::Engine()->IsInGame())
		{
			Skin_OnFrameStageNotify(Stage);
			Gloves_OnFrameStageNotify(Stage);
		}
	}

	void OnDrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t &state,
		const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld)
	{
		if (Interfaces::Engine()->IsInGame() && ctx && pCustomBoneToWorld)
		{
			if (g_pEsp)
				g_pEsp->OnDrawModelExecute(ctx, state, pInfo, pCustomBoneToWorld);

			if (g_pMisc)
				g_pMisc->OnDrawModelExecute();
		}
	}

	void OnPlaySound(const Vector* pOrigin, const char* pszSoundName)
	{
		if (!pszSoundName || !Interfaces::Engine()->IsInGame())
			return;

		if (!strstr(pszSoundName, "bulletLtoR") &&
			!strstr(pszSoundName, "rics/ric") &&
			!strstr(pszSoundName, "impact_bullet"))
		{
			if (g_pEsp && IsLocalAlive() && Settings::Esp::Visuals_Sound && pOrigin)
			{
				if (!GetVisibleOrigin(*pOrigin))
					g_pEsp->SoundEsp.AddSound(*pOrigin);
			}
		}
	}

	void OnPlaySound(const char* pszSoundName)
	{
		if (g_pMisc)
			g_pMisc->OnPlaySound(pszSoundName);
	}

	void OnOverrideView(CViewSetup* pSetup)
	{
		if (g_pMisc)
			g_pMisc->OnOverrideView(pSetup);
	}

	void OnGetViewModelFOV(float& fov)
	{
		if (g_pMisc)
			g_pMisc->OnGetViewModelFOV(fov);
	}

	void OnRenderGUI()
	{
		/*if (IsProcessRun("devenv.exe"))
		{
			MessageBox(0, "Dont cryak debil blyad!1!", "Anti-Crack System", MB_OK);
			Engine::Shutdown();
		}

		if (IsProcessRun("idaq64.exe"))
		{
			MessageBox(0, "Dont cryak debil blyad!12!", "Anti-Crack System", MB_OK);
			Engine::Shutdown();
		}

		if (IsProcessRun("idaq32.exe"))
		{
			MessageBox(0, "Dont cryak debil blyad!13!", "Anti-Crack System", MB_OK);
			Engine::Shutdown();
		}

		if (IsProcessRun("ServiceHub.Host.CLR.x86.exe"))
		{
			MessageBox(0, "Dont cryak debil blyad!14!", "Anti-Crack System", MB_OK);
			Engine::Shutdown();
		}

		if (IsProcessRun("ServiceHub.SettingsHost.exe"))
		{
			MessageBox(0, "Dont cryak debil blyad!15!", "Anti-Crack System", MB_OK);
			Engine::Shutdown();
		}

		if (IsProcessRun("ServiceHub.IdentityHost.exe"))
		{
			MessageBox(0, "Dont cryak debil blyad!16!", "Anti-Crack System", MB_OK);
			Engine::Shutdown();
		}

		if (IsProcessRun("ServiceHub.VSDetouredHost.exe"))
		{
			MessageBox(0, "Dont cryak debil blyad!17!", "Anti-Crack System", MB_OK);
			Engine::Shutdown();
		}

		if (IsProcessRun("ServiceHub.Host.Node.x86.exe"))
		{
			MessageBox(0, "Dont cryak debil blyad!18!", "Anti-Crack System", MB_OK);
			Engine::Shutdown();
		}

		if (IsProcessRun("PerfWatson2.exe"))
		{
			MessageBox(0, "Dont cryak debil blyad!19!", "Anti-Crack System", MB_OK);
			Engine::Shutdown();
		}

		if (IsProcessRun("ollydbg.exe"))
		{
			MessageBox(0, "Dont cryak debil blyad!10!", "Anti-Crack System", MB_OK);
			Engine::Shutdown();
		}*/


		ImVec2 mainWindowPos;

		CLicense lic;
		//static bool licactive = lic.CheckLicense();
		//static string ser = lic.GetSerial();
		//static string days = lic.GetUserDayCount();

		//if (licactive == false)
		//{
		//	Engine::Shutdown();
		//}
		if(1==1)		{
			ImGui::SetNextWindowSize(ImVec2(815, 473));
			if (ImGui::Begin("ELITE CHEATS", &bIsGuiVisible, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar))
			{
				if (Settings::Aimbot::weapon_settings[iWeaponID].AimBot_FovType > 4)
					Settings::Aimbot::weapon_settings[iWeaponID].AimBot_FovType = 4;

				if (Settings::Aimbot::weapon_settings[iWeaponID].AimBot_BestHit > 1)
					Settings::Aimbot::weapon_settings[iWeaponID].AimBot_BestHit = 1;

				if (Settings::Aimbot::weapon_settings[iWeaponID].AimBot_Spot > 14)
					Settings::Aimbot::weapon_settings[iWeaponID].AimBot_Spot = 14;

				mainWindowPos = ImGui::GetWindowPos();

				static int tabSelected = 0;

				const char* tabNames[] = {
						u8"LegitBot" , u8"Visuals" , u8"Misc" ,
						u8"Colors" , u8"Settings" , u8"Changers" };

				static int tabOrder[] = { 0 , 1 , 2 , 3 , 4 , 5 };
				const bool tabChanged = ImGui::TabLabels(tabNames, sizeof(tabNames) / sizeof(tabNames[0]), tabSelected, tabOrder);

				ImGui::Spacing();
				ImGui::Spacing();

				ImGuiIO& io = ImGui::GetIO();
				ImGuiStyle& style = ImGui::GetStyle();

				if (tabSelected == 0)
				{
					ImGui::BeginGroup();
					ImGui::BeginChild("secoqand child", ImVec2(220, 61), true);
					{
						ImGui::Checkbox(u8"TeamAttack", &Settings::Aimbot::AimBot_Deathmatch);
						ImGui::Checkbox(u8"AutoWall", &Settings::Aimbot::AimBot_WallAttack);
					}
					ImGui::EndChild();
					ImGui::BeginChild("secowadawqand child", ImVec2(220, 61), true);
					{
						ImGui::Checkbox(u8"Show FOV", &Settings::Aimbot::AimBot_DrawFov);
						ImGui::Checkbox(u8"Show Aim Spot", &Settings::Aimbot::AimBot_DrawSpot);
					}
					ImGui::EndChild();
					ImGui::BeginChild("secoqaawdawdnd child", ImVec2(220, 84), true);
					{
						ImGui::Checkbox(u8"Flash/Smoke Check", &Settings::Aimbot::AimBot_CheckSmoke);
						ImGui::Checkbox(u8"Jump Check", &Settings::Aimbot::AimBot_AntiJump);
						ImGui::Checkbox(u8"Local Jump Check", &Settings::Aimbot::AimBot_JumpCheckME);
					}
					ImGui::EndChild();
					ImGui::BeginChild("secoqand wdawdachild", ImVec2(220, 84), true);
					{
						ImGui::Checkbox(u8"Backtrack ", &Settings::Misc::Other_Backtrack);
						ImGui::PushItemWidth(140.f);
						ImGui::SliderInt(u8"Ticks", &Settings::Aimbot::BackTrackTicks, 1, 12);
						ImGui::PopItemWidth();
						ImGui::Checkbox(u8"Draw BackTrack", &Settings::Esp::Visuals_track);
					}
					ImGui::EndChild();
					ImGui::BeginChild("secoqanawdawdd child", ImVec2(220, 114), true);
					{
						if (iWeaponID <= 9)
						{
							ImGui::Checkbox(u8"Auto Pistol", &Settings::Aimbot::weapon_settings[iWeaponID].AimBot_AutoPistol);
						}
					}
					ImGui::EndChild();
					ImGui::EndGroup();
					ImGui::SameLine();
					ImGui::BeginGroup();
					ImGui::BeginChild("second awdawdchild", ImVec2(570, 35), true);
					{
						ImGui::PushItemWidth(110.f);
						ImGui::Text(u8"Current weapon: ");
						ImGui::SameLine();
						ImGui::Combo("##AimWeapon", &iWeaponID, pWeaponData, IM_ARRAYSIZE(pWeaponData));
					}
					ImGui::EndChild();
					ImGui::BeginChild("second chwdaild", ImVec2(570, 381), true);
					{
						ImGui::PushItemWidth(300.f);
						ImGui::Checkbox(u8"Enable", &Settings::Aimbot::weapon_settings[iWeaponID].AimBot_Active);
						ImGui::SameLine();
						ImGui::Checkbox(u8"pSilent", &Settings::Aimbot::weapon_settings[iWeaponID].AimBot_pSilent);
						ImGui::Checkbox(u8"Use FOV and Smooth for 1 bullet", &Settings::Aimbot::weapon_settings[iWeaponID].AimBot_Use2fovs);

						if (!Settings::Aimbot::weapon_settings[iWeaponID].AimBot_pSilent)
						{
							ImGui::SliderFloat(u8"Smooth", &Settings::Aimbot::weapon_settings[iWeaponID].AimBot_Smooth, 0.1f, 30.0f, "%5.2f");
							ImGui::SliderFloat(u8"FOV", &Settings::Aimbot::weapon_settings[iWeaponID].AimBot_Fov, 0.1f, 40.0f, "%5.2f");
							if (Settings::Aimbot::weapon_settings[iWeaponID].AimBot_Use2fovs)
							{
								ImGui::SliderFloat(u8"Smooth  [1 bullet]", &Settings::Aimbot::weapon_settings[iWeaponID].AimBot_SecondSmooth, 0.1f, 30.0f, "%5.2f");
								ImGui::SliderFloat(u8"FOV [1 bullet]", &Settings::Aimbot::weapon_settings[iWeaponID].AimBot_SecondFov, 0.1f, 40.0f, "%5.2f");
							}
						}
						else
						{
							ImGui::SliderFloat(u8"FOV", &Settings::Aimbot::weapon_settings[iWeaponID].AimBot_pSilentFov, 0.1f, 40.0f, "%5.2f");
							if (Settings::Aimbot::weapon_settings[iWeaponID].AimBot_Use2fovs)
							{
								ImGui::SliderFloat(u8"FOV [1 bullet]", &Settings::Aimbot::weapon_settings[iWeaponID].AimBot_pSecondSilentFov, 0.1f, 40.0f, "%5.2f");
							}
						}
						ImGui::PopItemWidth();
						const char* AimFovType[] = { "Dynamic [Round] ", " Static [Round] ", " From the opponent [Square] ", " Static [Square]" };
						ImGui::PushItemWidth(300.f);
						ImGui::Combo("Type of FOVа", &Settings::Aimbot::weapon_settings[iWeaponID].AimBot_FovType, AimFovType, IM_ARRAYSIZE(AimFovType));
						ImGui::PopItemWidth();
						ImGui::Separator();
						ImGui::Checkbox("Smart Bone", &Settings::Aimbot::weapon_settings[iWeaponID].AimBot_BestHit);
						if (!Settings::Aimbot::weapon_settings[iWeaponID].AimBot_BestHit)
						{
							ImGui::SameLine();
							const char* Aimspot[] = { "Head" , "Neck" , "Body" , "Thorax" , "Chest" , "Right Thigh" , "Left Thigh" , "Right Hand" , "Left Hand" , "Right Upperarm" , "Right Forearm" ,
								"Left Upperarm" , "Left Forearm" };
							ImGui::PushItemWidth(200.f);
							ImGui::Combo("AimSpot", &Settings::Aimbot::weapon_settings[iWeaponID].AimBot_Spot, Aimspot, IM_ARRAYSIZE(Aimspot));
							ImGui::PopItemWidth();
						}
						else
						{

						}
						ImGui::Separator();
						ImGui::PopItemWidth();
						ImGui::PushItemWidth(300.f);
						ImGui::PopItemWidth();
						ImGui::PushItemWidth(300.f);
						ImGui::SliderInt(u8"Delay before the shot", &Settings::Aimbot::weapon_settings[iWeaponID].AimBot_Delay, 0, 200);
						const char* RCSType[] = { u8"1 RCS" , u8"2 RCS [BETA]" , u8"3 RCS [BETA]" };
						ImGui::PushItemWidth(300.f);
						ImGui::Combo(u8"Type RCS", &Settings::Aimbot::weapon_settings[iWeaponID].AimBot_RcsType, RCSType, IM_ARRAYSIZE(RCSType));

						ImGui::SliderInt(u8"RCS after X shots", &Settings::Aimbot::weapon_settings[iWeaponID].AimBot_RcsAfterXShot, 0, 30);
						ImGui::SliderInt(u8"Percent RCS", &Settings::Aimbot::weapon_settings[iWeaponID].AimBot_Rcs, 0, 100);
						ImGui::PopItemWidth();
					}
					ImGui::EndChild();
					ImGui::EndGroup();
				}
				else if (tabSelected == 1)
				{
					ImGui::BeginGroup();
					ImGui::BeginChild("##1secoqand child", ImVec2(200, 316), true);
					{
						ImGui::Text(u8"Filters / Information");
						ImGui::Checkbox(u8"Show on Team", &Settings::Esp::Visuals_Team);
						ImGui::Checkbox(u8"Show on Enemy", &Settings::Esp::Visuals_Enemy);
						ImGui::Checkbox(u8"Bomb", &Settings::Esp::Visuals_Bomb);
						ImGui::Checkbox(u8"Name", &Settings::Esp::Visuals_Name);
						ImGui::Checkbox(u8"Weapon", &Settings::Esp::Visuals_Weapon);
						ImGui::Checkbox(u8"Ammo", &Settings::Esp::Visuals_Ammo);
						ImGui::Checkbox(u8"Lines", &Settings::Esp::Visuals_Line);
						ImGui::Checkbox(u8"Skeleton", &Settings::Esp::Visuals_Skeleton);
						ImGui::Checkbox(u8"Rank", &Settings::Esp::Visuals_Rank);
					}
					ImGui::EndChild();
					ImGui::BeginChild("##123wdcasecond child", ImVec2(200, 36), true);
					{
						//	ImGui::Checkbox(u8"Ночной режим", &Settings::Esp::NightMode);
					}
					ImGui::EndChild();
					ImGui::BeginChild("##s2ecoqand child", ImVec2(200, 60), true);
					{
						ImGui::Checkbox(u8"Dropped Weapons", &Settings::Esp::Visuals_WorldWeapons);
						ImGui::Checkbox(u8"Dropped Grenades", &Settings::Esp::Visuals_WorldGrenade);
					}
					ImGui::EndChild();
					ImGui::EndGroup();
					ImGui::SameLine();
					ImGui::BeginGroup();
					ImGui::BeginChild("second child", ImVec2(362, 110), true);
					{
						ImGui::PushItemWidth(225.f);

						string style_1 = u8" Disabled";
						string style_2 = u8" Boxes";
						string style_3 = u8" Angles";
						string style_4 = u8" Poured";
						string style_5 = u8" Boxed Boxes";
						string style_6 = u8" Folded Corner";

						const char* items1[] = { style_1.c_str() , style_2.c_str() ,  style_3.c_str()  ,  style_4.c_str() ,  style_5.c_str()  ,  style_6.c_str() };

						ImGui::Combo(u8"Box type", &Settings::Esp::Visuals_Style, items1, IM_ARRAYSIZE(items1));

						string visible_1 = u8"	Opponents";
						string visible_2 = u8" Team";
						string visible_3 = u8" All";
						string visible_4 = u8" Only visible";

						const char* items2[] = { visible_1.c_str() , visible_2.c_str() , visible_3.c_str() , visible_4.c_str() };

						ImGui::Combo(u8"ESP display", &Settings::Esp::Visuals_Visible, items2, IM_ARRAYSIZE(items2));

						string hpbar_1 = u8" No";
						string hpbar_2 = u8" Numbers";
						string hpbar_3 = u8" Bar below";
						string hpbar_4 = u8" Bar on the right";
						string hpbar_5 = u8" Segmented";
						const char* items3[] = { hpbar_1.c_str() , hpbar_2.c_str() , hpbar_3.c_str() , hpbar_4.c_str() , hpbar_5.c_str() };
						ImGui::Combo(u8"Health show", &Settings::Esp::Visuals_Health, items3, IM_ARRAYSIZE(items3));

						string arbar_1 = u8" No";
						string arbar_2 = u8" Numbers";
						string arbar_3 = u8" Bar below";
						string arbar_4 = u8" Bar on the left";

						const char* items4[] = { arbar_1.c_str() , arbar_2.c_str() , arbar_3.c_str() , arbar_4.c_str() };
						ImGui::Combo(u8"Armor display", &Settings::Esp::Visuals_Armor, items4, IM_ARRAYSIZE(items4));

					}
					ImGui::EndChild();
					//================================
					ImGui::BeginChild("##342123wdcasecond child", ImVec2(362, 306), true);
					{
						ImDrawList* Draw = ImGui::GetWindowDrawList();

						ImVec2 DrawPos = ImGui::GetCursorScreenPos();
						ImVec2 DrawSize = ImGui::GetContentRegionAvail();

						ImVec2 winpos = ImGui::GetWindowPos();
						ImVec2 winsize = ImGui::GetWindowSize();

						ImColor colBox;
						ImColor NameCol = ImColor(Settings::Colors::Name_Color[0], Settings::Colors::Name_Color[1], Settings::Colors::Name_Color[2]);

						ImColor helcol = ImColor(0, 200, 0);

						ImColor WeaponCol = ImColor(Settings::Colors::WeaponEnemy_Color[0], Settings::Colors::WeaponEnemy_Color[1], Settings::Colors::WeaponEnemy_Color[2]);

						ImColor SkeletonCol = ImColor(Settings::Colors::Skelet[0], Settings::Colors::Skelet[1], Settings::Colors::Skelet[2]);


						static int forCol = 0;
						static int healthbarSize = 100;
						static int healthbarSize2 = 192;

						const char *healthText = "100";
						const char *ArmorText = "100";
						const char *WeaponText;

						static int rand = 0;

						if (rand <= 1333) {
							forCol = 0;
							healthbarSize = 100;
							healthbarSize2 = 100;
							healthText = "100";
							ArmorText = "100";

						}
						else if (rand > 1333 && rand <= 2666)
						{
							forCol = 1;
							healthbarSize = 55;
							healthbarSize2 = 55;
							healthText = "55";
							ArmorText = "68";
						}
						else if (rand > 2666 && rand < 4000)
						{
							forCol = 2;
							healthbarSize = 2;
							healthbarSize2 = 2;
							healthText = "15";
							ArmorText = "38";
						}

						if (rand == 4000) rand = 0;

						rand++;

						if (forCol == 0) helcol = ImColor(0, 255, 0);
						else if (forCol == 1) helcol = ImColor(255, 195, 0);
						else if (forCol == 2) helcol = ImColor(180, 0, 0);
						else  helcol = ImColor(255, 0, 0);

						const char* boxcol[2] =
						{
							u8"CT Color", u8"TT Color"
						};

						ImGui::Combo(u8"Preview", &Settings::Esp::RenderTypeBox, boxcol, IM_ARRAYSIZE(boxcol));
						ImGui::NewLine();
						ImGui::Text("                        ");
						ImGui::SameLine();
						ImGui::Image(tImage, ImVec2(180, 220));


						if (Settings::Esp::Visuals_Skeleton)
						{
							Draw->AddSkeleton(ImVec2(winsize.x / 2 + winpos.x - 5, (winsize.y / 2 + winpos.y) - 80), SkeletonCol, 3);
						}

						if (Settings::Esp::RenderTypeBox == 0)
							colBox = ImColor(Settings::Colors::Visuals_Color_CT[0], Settings::Colors::Visuals_Color_CT[1], Settings::Colors::Visuals_Color_CT[2]);

						else if (Settings::Esp::RenderTypeBox == 1)
							colBox = ImColor(Settings::Colors::Visuals_Color_TT[0], Settings::Colors::Visuals_Color_TT[1], Settings::Colors::Visuals_Color_TT[2]);


						if (Settings::Esp::Visuals_Style == 1)
							Draw->AddBox(ImVec2(winsize.x / 2 + winpos.x, winsize.y / 2 + winpos.y), colBox, 3, 200, 385);
						else if (Settings::Esp::Visuals_Style == 2)
							Draw->AddCornerBox(ImVec2(winsize.x / 2 + winpos.x, winsize.y / 2 + winpos.y), colBox, 3, 200, 385);

						if (Settings::Esp::Visuals_Name)
							Draw->AddText(ImVec2((winsize.x / 2 + winpos.x) - 20, (winsize.y / 2 + winpos.y) - (winsize.y / 4) - 42), NameCol, "Name");

						if (Settings::Esp::Visuals_Health == 1)
							Draw->AddText(ImVec2((winsize.x / 2 + winpos.x) - 7, (winsize.y / 2 + winpos.y) - (winsize.y / 4) + 175), ImColor(255, 255, 255), healthText);
						else if (Settings::Esp::Visuals_Health == 2)
							Draw->AddHealthBar(ImVec2((winsize.x / 2 + winpos.x), (winsize.y / 2 + winpos.y) + (winsize.x / 2) + 40), helcol, 4, healthbarSize);
						else if (Settings::Esp::Visuals_Health == 3)
							Draw->AddHealthBar2(ImVec2((winsize.x / 2 + winpos.x) + (winsize.x / 4) + 30, (winsize.y / 2 + winpos.y)), helcol, 4, healthbarSize2);
						//	else if (Settings::Esp::Visuals_Health == 4)
							//	Draw->AddHealthBar3(ImVec2((winsize.x / 2 + winpos.x) - (winsize.x / 4) - 30, (winsize.y / 2 + winpos.y)), ImColor(0, 255, 0), 6, 355);

						if (Settings::Esp::Visuals_Weapon)
						{
							WeaponText = "AK-47";
							if (Settings::Esp::Visuals_Ammo)
								WeaponText = "AK-47 [30]";
						}

						if (Settings::Esp::Visuals_Weapon)
							Draw->AddText(ImVec2((winsize.x / 2 + winpos.x) - 20, (winsize.y / 2 + winpos.y) - (winsize.y / 4) + 175), WeaponCol, WeaponText);

						if (Settings::Esp::Visuals_Armor == 1)
							Draw->AddText(ImVec2((winsize.x / 2 + winpos.x) - 125, (winsize.y / 2 + winpos.y) - (winsize.y / 4) + 300), ImColor(255, 255, 255), ArmorText);
						else if (Settings::Esp::Visuals_Armor == 2)
							Draw->AddHealthBar(ImVec2((winsize.x / 2 + winpos.x), (winsize.y / 2 + winpos.y) + (winsize.x / 2) + 40), ImColor(255, 255, 255), 4, 100);
						else if (Settings::Esp::Visuals_Armor == 3)
							Draw->AddHealthBar2(ImVec2((winsize.x / 2 + winpos.x) - (winsize.x / 4) - 30, (winsize.y / 2 + winpos.y)), ImColor(255, 255, 255), 4, 100);
						ImGui::Text(u8"                                           * Preview in development");
					}
					ImGui::EndChild();
					//================================
					ImGui::EndGroup();
					ImGui::SameLine();
					ImGui::BeginGroup();
					ImGui::BeginChild("##se23cond c523hil2d", ImVec2(220, 90), true);
					{
						string chams_1 = u8" No";
						string chams_2 = u8" 2D";
						string chams_3 = u8" 3D";


						const char* items5[] = { chams_1.c_str() , chams_2.c_str() , chams_3.c_str() };
						ImGui::Combo(u8"Chams", &Settings::Esp::Visuals_Chams, items5, IM_ARRAYSIZE(items5));
						ImGui::Checkbox(u8"Rainbow", &Settings::Esp::Visuals_ChamsRainbow);
						ImGui::Checkbox(u8"Health Based", &Settings::Esp::Visuals_HPBasedChams);
					}
					ImGui::EndChild();
					ImGui::BeginChild("##se23cond231 chil2d", ImVec2(220, 120), true);
					{
						const char* material_items[] =
						{
							u8" Glass",
							u8" Crystal",
							u8" Gold",
							u8" Black Chrome",
							u8" Glass 2",
							u8" Violet",
							u8" Crystal blue",
							u8" Baby gold"
						};

						ImGui::Checkbox(u8"Hands material", &Settings::Misc::Other_ArmMaterials);
						ImGui::PushItemWidth(150.f);
						ImGui::Combo("##ARMMATERIALS", &Settings::Misc::Other_ArmMaterialsList, material_items, ARRAYSIZE(material_items));

						ImGui::Checkbox("WireHands", &Settings::Misc::Other_WireHands);
						ImGui::Checkbox("NoHands", &Settings::Misc::Other_NoHands);
					}
					ImGui::EndChild();
					ImGui::BeginChild("##sec3ond c231hil2d", ImVec2(220, 120), true);
					{
						ImGui::Checkbox(u8"Sound ESP", &Settings::Esp::Visuals_Sound);
						ImGui::PushItemWidth(120.f);
						ImGui::SliderInt(u8"Size", &Settings::Esp::Visuals_SoundSize, 1, 20);
						ImGui::SliderInt(u8"Type", &Settings::Esp::Visuals_SoundType, 1, 4);
						//ImGui::SliderInt(u8"Дистанция", &Settings::Esp::Visuals_SoundDist, 200, 1200);
						ImGui::PopItemWidth();
					}
					ImGui::EndChild();
					ImGui::BeginChild("##s23ec3ond32 chil2d", ImVec2(220, 77), true);
					{
						ImGui::Checkbox(u8"Grenade Helper", &Settings::Esp::helper); ImGui::SameLine(SpaceLine5);
						ImGui::SameLine();
						if (ImGui::Button(u8"Refresh Map"))
							cGrenade.bUpdateGrenadeInfo(Interfaces::Engine()->GetLevelNameShort());

						ImGui::Checkbox(u8"Grenade Trajectory", &Settings::Esp::Visuals_GrenadePrediction);
					}
					ImGui::EndChild();
					ImGui::EndGroup();
				}
				else if (tabSelected == 2)
				{
					ImGui::BeginGroup();
					ImGui::BeginChild("first child", ImVec2(200, 84), true);
					{
						ImGui::Checkbox(u8"Bhop", &Settings::Misc::Other_Bhop);
						ImGui::Checkbox(u8"Auto Strafe", &Settings::Misc::Other_AutoStrafe);
						ImGui::Checkbox(u8"Meme Walk", &Settings::Misc::Other_MemeWalk);
					}
					ImGui::EndChild();
					ImGui::BeginChild("firaawdfadawadaddawst child", ImVec2(200, 84), true);
					{
						ImGui::Checkbox(u8"No Flash", &Settings::Misc::Other_NoFlash);
						ImGui::Checkbox(u8"Sniper Crosshair", &Settings::Misc::Other_AwpAim);
						ImGui::Checkbox(u8"Punch Crosshair", &Settings::Misc::Other_Punch);
					}
					ImGui::EndChild();
					ImGui::BeginChild("firaawdfadawdawdast child", ImVec2(200, 60), true);
					{
						ImGui::Checkbox(u8"Auto Accept", &Settings::Misc::Other_AutoAccept);
						ImGui::Checkbox(u8"Spectators", &Settings::Misc::Other_Spectators);
					}
					ImGui::EndChild();
					ImGui::BeginChild("firaawdfadawdaadadwst child", ImVec2(200, 180), true);
					{
						ImGui::Checkbox(u8"Skin Changer", &Settings::Misc::EnableSkinchanger);
					}
					ImGui::EndChild();
					ImGui::EndGroup();
					ImGui::SameLine();
					ImGui::BeginGroup();
					ImGui::BeginChild("sewadawdcond child", ImVec2(200, 36), true);
					{
						ImGui::Checkbox(u8"Name Spam", &Settings::Misc::misc_Name);
					}
					ImGui::EndChild();
					ImGui::BeginChild("seadacond child", ImVec2(200, 80), true);
					{
						ImGui::PushItemWidth(178.f);
						ImGui::Text(u8"Clan Tag");
						ImGui::InputText(u8"##clantagggs", clantag, 128);
						if (ImGui::Button(u8"Apply")) Engine::ClanTag228(clantag);
						ImGui::PopItemWidth();
					}
					ImGui::EndChild();
					ImGui::BeginChild("seawdawdcond child", ImVec2(200, 60), true);
					{
						ImGui::PushItemWidth(120.f);
						ImGui::SliderInt("Bomb-X", &Settings::Misc::bombtimerx, 0, 2500);
						ImGui::SliderInt("Bomb-Y", &Settings::Misc::bombtimery, 0, 1500);
						ImGui::PopItemWidth();
					}
					ImGui::EndChild();
					ImGui::BeginChild("seawdawdconуd child", ImVec2(200, 232), true);
					{
					}
					ImGui::EndChild();
					ImGui::EndGroup();
					ImGui::SameLine();
					ImGui::BeginGroup();
					ImGui::BeginChild("third child", ImVec2(382, 110), true);
					{
						ImGui::Text(u8"Radar");

						ImGui::Checkbox(u8"Active", &Settings::Radar::Radar_Active);

						ImGui::Checkbox(u8"Show Team", &Settings::Radar::Radar_Team);
						ImGui::SameLine();
						ImGui::Checkbox(u8"Show Enemy", &Settings::Radar::Radar_Enemy);
						ImGui::SameLine();
						ImGui::Checkbox(u8"Sound", &Settings::Radar::Radar_Sound);

						ImGui::PushItemWidth(239.f);
						ImGui::SliderInt(u8"Range", &Settings::Radar::Radar_Range, 1, 5000);

					}
					ImGui::EndChild();
					ImGui::BeginChild("third c232hild", ImVec2(382, 95), true);
					{
						ImGui::Checkbox(u8"FOV Changer", &Settings::Misc::Other_FovChanger);

						ImGui::PushItemWidth(239.f);
						ImGui::SliderInt(u8"FOV View", &Settings::Misc::Other_FovView, 35, 150);
						ImGui::SliderInt(u8"FOV Model View", &Settings::Misc::Other_FovModelView, 10, 150);

					}
					ImGui::EndChild();
					ImGui::BeginChild("third 232123child", ImVec2(382, 145), true);
					{
						ImGui::Text(u8"Knife Bot");
						ImGui::Checkbox(u8"Active", &Settings::Knifebot::knf_Active);
						ImGui::SameLine();
						ImGui::Checkbox(u8"Team Check", &Settings::Knifebot::knf_Team);

						ImGui::PushItemWidth(239.f);

						string attack_1 = u8"Left";
						string attack_2 = u8"Right";
						string attack_3 = u8"Left + Right";

						const char* items[] = { attack_1.c_str() , attack_2.c_str() , attack_3.c_str() };
						ImGui::Combo(u8"Attack type", &Settings::Knifebot::knf_Attack, items, IM_ARRAYSIZE(items));

						ImGui::SliderInt(u8"Distance Attack", &Settings::Knifebot::knf_DistAttack, 1, 100);
						ImGui::SliderInt(u8"Distance Attack", &Settings::Knifebot::knf_DistAttack2, 1, 100);
					}
					ImGui::EndChild();
					ImGui::BeginChild("t12hird 232123child", ImVec2(382, 58), true);
					{

					}
					ImGui::EndChild();
					ImGui::EndGroup();
				}
				else if (tabSelected == 3)
				{
					ImGui::BeginGroup();

					ImGui::PushItemWidth(220.f);
					ImGui::BeginChild("first child", ImVec2(385, 420), true);
					{
						ImGui::Text(u8"Visuals");
						ImGui::ColorEdit3(u8"ESP CT", Settings::Colors::Visuals_Color_CT, ImGuiColorEditFlags_NoInputs);
						ImGui::ColorEdit3(u8"ESP ТТ", Settings::Colors::Visuals_Color_TT, ImGuiColorEditFlags_NoInputs);
						ImGui::ColorEdit3(u8"Visible ESP CT", Settings::Colors::Visuals_Color_VCT, ImGuiColorEditFlags_NoInputs);
						ImGui::ColorEdit3(u8"Visible ESP ТТ", Settings::Colors::Visuals_Color_VTT, ImGuiColorEditFlags_NoInputs);
						ImGui::ColorEdit3(u8"Chams CT", Settings::Colors::chams_Color_CT, ImGuiColorEditFlags_NoInputs);
						ImGui::ColorEdit3(u8"Chams ТТ", Settings::Colors::chams_Color_TT, ImGuiColorEditFlags_NoInputs);
						ImGui::ColorEdit3(u8"Chams CТ", Settings::Colors::chams_Color_VCT, ImGuiColorEditFlags_NoInputs);
						ImGui::ColorEdit3(u8"Chams ТТ", Settings::Colors::chams_Color_VTT, ImGuiColorEditFlags_NoInputs);
						ImGui::ColorEdit3(u8"Health Color", Settings::Colors::Health_Color, ImGuiColorEditFlags_NoInputs);
						ImGui::ColorEdit3(u8"Armor Color", Settings::Colors::Armor_Color, ImGuiColorEditFlags_NoInputs);
						ImGui::ColorEdit3(u8"Skelet", Settings::Colors::Skelet, ImGuiColorEditFlags_NoInputs);
						ImGui::ColorEdit3(u8"Sound Visuals", Settings::Colors::SoundVisuals_Color, ImGuiColorEditFlags_NoInputs);
						ImGui::ColorEdit3(u8"Weapon Enemy", Settings::Colors::WeaponEnemy_Color, ImGuiColorEditFlags_NoInputs);
						ImGui::ColorEdit3(u8"World Weapon", Settings::Colors::WorldWeapon_Color, ImGuiColorEditFlags_NoInputs);
						ImGui::ColorEdit3(u8"Visuals Line Color", Settings::Colors::Visuals_Line_Color, ImGuiColorEditFlags_NoInputs);
						ImGui::ColorEdit3(u8"Name Color", Settings::Colors::Name_Color, ImGuiColorEditFlags_NoInputs);
					}
					ImGui::EndChild();
					ImGui::SameLine();
					ImGui::BeginChild("second child", ImVec2(405, 420), true);
					{
						ImGui::Text(u8"Radar");
						ImGui::ColorEdit3(u8"Color CТ", Settings::Radar::Radar_Color_CT, ImGuiColorEditFlags_NoInputs);
						ImGui::ColorEdit3(u8"Color ТТ", Settings::Radar::Radar_Color_TT, ImGuiColorEditFlags_NoInputs);
						ImGui::ColorEdit3(u8"Color Color CТ", Settings::Radar::Radar_Color_VCT, ImGuiColorEditFlags_NoInputs);
						ImGui::ColorEdit3(u8"Color Color TТ", Settings::Radar::Radar_Color_VTT, ImGuiColorEditFlags_NoInputs);
						ImGui::Text(u8"Crosshair Colors");
						ImGui::ColorEdit3(u8"AWP Crosshair", Settings::Misc::Other_AwpAimColor, ImGuiColorEditFlags_NoInputs);
						ImGui::Spacing();
						ImGui::Spacing();
						ImGui::ColorEdit3(u8"Molotov/HE", Settings::Colors::Fraggrenade_Color, ImGuiColorEditFlags_NoInputs);
						ImGui::ColorEdit3(u8"FlashBang", Settings::Colors::Flashbang_Color, ImGuiColorEditFlags_NoInputs);
						ImGui::ColorEdit3(u8"Smoke", Settings::Colors::Smokegrenade_Color, ImGuiColorEditFlags_NoInputs);
						ImGui::Spacing();
						ImGui::Spacing();
						ImGui::ColorEdit3(u8"Grenade Helper", Settings::Colors::GrenadeHelper, ImGuiColorEditFlags_NoInputs);
					}
					ImGui::EndChild();
					ImGui::PopItemWidth();
					ImGui::EndGroup();

				}
				else if (tabSelected == 4)
				{
					ImGui::PushItemWidth(220.f);
					ImGui::BeginChild("first child", ImVec2(385, 420), true);
					{
						static int iConfigSelect = 0;
						static int iMenuSheme = 1;
						static char ConfigName[64] = { 0 };

						ImGui::ComboBoxArray(u8"Config", &iConfigSelect, ConfigList);

						if (ImGui::Button(u8"Load"))
						{
							Settings::Esp::helper = false;
							Settings::LoadSettings(BaseDir + "\\" + ConfigList[iConfigSelect]);
							SendClientHello();
							SendMMHello();
						}
						ImGui::SameLine();
						if (ImGui::Button(u8"Save"))
						{
							Settings::Esp::helper = false;
							Settings::SaveSettings(BaseDir + "\\" + ConfigList[iConfigSelect]);
							SendClientHello();
							SendMMHello();
						}
						ImGui::SameLine();
						if (ImGui::Button(u8"Update CFG"))
						{
							RefreshConfigs();
							SendClientHello();
							SendMMHello();
						}

						if (ImGui::Button(u8"Create and Save CFG"))
						{
							string ConfigFileName = ConfigName;

							if (ConfigFileName.size() < 1)
							{
								ConfigFileName = "settings";
							}

							Settings::SaveSettings(BaseDir + "\\" + ConfigFileName + ".ini");
							RefreshConfigs();
							SendClientHello();
							SendMMHello();
						}
					}
					ImGui::EndChild();
					ImGui::SameLine();
					ImGui::BeginGroup();
					ImGui::BeginChild("second child", ImVec2(405, 364), true);
					{
						ImGui::PushItemWidth(250.f);

						ImGui::Checkbox(u8"Use custom colors", &Settings::Menu::Menu_UseCustomColors);

						if (Settings::Menu::Menu_UseCustomColors)
						{
							ImGui::ColorEdit3(u8"Main color", Settings::Menu::Menu_MainColor, ImGuiColorEditFlags_NoInputs);
							ImGui::ColorEdit3(u8"Background", Settings::Menu::Menu_BackGround, ImGuiColorEditFlags_NoInputs);
							ImGui::ColorEdit3(u8"Background of subsections", Settings::Menu::Menu_BackGround1, ImGuiColorEditFlags_NoInputs);
							ImGui::ColorEdit3(u8"Text", Settings::Menu::Menu_Text, ImGuiColorEditFlags_NoInputs);

							if (ImGui::Button(u8"Accept Scheme"))
								g_pGui->Red2Sheme();

							if (ImGui::Button(u8"Standard scheme"))
								g_pGui->RedSheme();
						}

						ImGui::Checkbox(u8"Radar drawing", &Settings::Radar::Radar_Enable);
						ImGui::SameLine();
					}
					ImGui::EndChild();
					ImGui::BeginChild("second chwdawdild", ImVec2(405, 52), true);
					{
						ImGui::Text(u8"User Information:");

						ImGui::Text(u8"Days left:"); ImGui::SameLine();
//						ImGui::TextColored(ImColor(40, 255, 40, 255), days.c_str());
					}
					ImGui::EndChild();
					ImGui::EndGroup();
					ImGui::PopItemWidth();
				}
				else if (tabSelected == 5)
				{
					const char* knife_models_items[] =
					{
						u8"Standard", u8"Bayonet knife", u8"Jackknife", u8"Knife with hook", u8"Karambit" , u8"M9 Bayonet",
						u8"Hunting knife", u8"Falshion", u8"The Bowie Knife", u8"Butterfly knife", u8"Pointing knives"
					};
					const char* gloves_listbox_items[49] =
					{
						"default",
						"Sport Gloves | Superconductor",
						"Sport Gloves | Pandora's Box",
						"Sport Gloves | Hedge Maze",
						"Sport Gloves | Arid",
						"Sport Gloves | Vice",
						"Sport Gloves | Omega",
						"Sport Gloves | Bronze Morph",
						"Sport Gloves | Amphibious",
						"Moto Gloves | Eclipse",
						"Moto Gloves | Spearmint",
						"Moto Gloves | Boom!",
						"Moto Gloves | Cool Mint",
						"Moto Gloves | Polygon",
						"Moto Gloves | Transport",
						"Moto Gloves | Turtle",
						"Moto Gloves | Pow",
						"Specialist Gloves | Crimson Kimono",
						"Specialist Gloves | Emerald Web",
						"Specialist Gloves | Foundation",
						"Specialist Gloves | Forest DDPAT",
						"Specialist Gloves | Mogul",
						"Specialist Gloves | Fade",
						"Specialist Gloves | Buckshot",
						"Specialist Gloves | Crimson Web",
						"Driver Gloves | Lunar Weave",
						"Driver Gloves | Convoy",
						"Driver Gloves | Crimson Weave",
						"Driver Gloves | Diamondback",
						"Driver Gloves | Racing Green",
						"Driver Gloves | Overtake",
						"Driver Gloves | Imperial Plad",
						"Driver Gloves | King Snake",
						"Hand Wraps | Leather",
						"Hand Wraps | Spruce DDPAT",
						"Hand Wraps | Badlands",
						"Hand Wraps | Slaughter",
						"Hand Wraps | Aboreal",
						"Hand Wraps | Duct Tape",
						"Hand Wraps | Overprint",
						"Hand Wraps | Cobalt Skulls",
						"Bloodhound Gloves | Charred",
						"Bloodhound Gloves | Snakebite",
						"Bloodhound Gloves | Bronzed",
						"Bloodhound Gloves | Guerrilla",
						"Hydra Gloves | Case Hardened",
						"Hydra Gloves | Rattler",
						"Hydra Gloves | Mangrove",
						"Hydra Gloves | Emerald"
					};

					static int iSelectKnifeCTSkinIndex = -1;
					static int iSelectKnifeTTSkinIndex = -1;

					int iKnifeCTModelIndex = Settings::Skin::knf_ct_model;
					int iKnifeTTModelIndex = Settings::Skin::knf_tt_model;

					static int iOldKnifeCTModelIndex = -1;
					static int iOldKnifeTTModelIndex = -1;

					if (iOldKnifeCTModelIndex != iKnifeCTModelIndex && Settings::Skin::knf_ct_model)
						iSelectKnifeCTSkinIndex = GetKnifeSkinIndexFromPaintKit(Settings::Skin::knf_ct_skin, false);

					if (iOldKnifeTTModelIndex != iKnifeTTModelIndex && Settings::Skin::knf_tt_model)
						iSelectKnifeTTSkinIndex = GetKnifeSkinIndexFromPaintKit(Settings::Skin::knf_ct_skin, true);

					iOldKnifeCTModelIndex = iKnifeCTModelIndex;
					iOldKnifeTTModelIndex = iKnifeTTModelIndex;

					string KnifeCTModel = knife_models_items[Settings::Skin::knf_ct_model];
					string KnifeTTModel = knife_models_items[Settings::Skin::knf_tt_model];

					ImGui::BeginGroup();
					ImGui::BeginChild("second child", ImVec2(385, 375), true);
					{
						ImGui::PushItemWidth(240.f);
						KnifeTTModel += " ##KTT";
						ImGui::Combo(u8"Knife CT", &Settings::Skin::knf_ct_model, knife_models_items, IM_ARRAYSIZE(knife_models_items));
						ImGui::InputInt(u8"Skin ID##2", &Settings::Skin::knf_ct_skin, 1, 10);
						ImGui::SliderFloat(u8"Quality knife CT", &g_SkinChangerCfg[WEAPON_KNIFE].flFallbackWear, 0.001f, 1.f, "%.3f");

						ImGui::Spacing();
						ImGui::Separator();
						ImGui::Spacing();

						KnifeTTModel += " ##KTT";
						ImGui::Combo(u8"Knife TT", &Settings::Skin::knf_tt_model, knife_models_items, IM_ARRAYSIZE(knife_models_items));
						ImGui::InputInt(u8"Skin ID##1", &Settings::Skin::knf_tt_skin, 1, 10);
						ImGui::SliderFloat(u8"Quality knife TT", &g_SkinChangerCfg[WEAPON_KNIFE_T].flFallbackWear, 0.001f, 1.f, "%.3f");

						ImGui::Spacing();
						ImGui::Separator();
						ImGui::Spacing();

						ImGui::Combo(u8"Skins Gloves", &Settings::Skin::gloves_skin, gloves_listbox_items, IM_ARRAYSIZE(gloves_listbox_items));
						ImGui::PopItemWidth();

						ImGui::Spacing();
						ImGui::Separator();
						ImGui::Spacing();


						ImGui::PushItemWidth(240.f);
						ImGui::Combo("##AimWeapon", &iWeaponID, pWeaponData, IM_ARRAYSIZE(pWeaponData));
						ImGui::InputInt(u8"Skin ID", &g_SkinChangerCfg[pWeaponItemIndexData[iWeaponID]].nFallbackPaintKit, 1, 10);
						const char* Raritys[] =
						{
							"Default","Consumer grade","Industrial grade","Mil-spec","Restricted" ,"Classified",
							"Covert","Contraband"
						};

						ImGui::Combo(u8"Value", &g_SkinChangerCfg[pWeaponItemIndexData[iWeaponID]].Rarity, Raritys, IM_ARRAYSIZE(Raritys));
						ImGui::SliderFloat(u8"Quality of weapons", &g_SkinChangerCfg[pWeaponItemIndexData[iWeaponID]].flFallbackWear, 0.001f, 1.f, "%.3f");
						ImGui::InputInt("StatTrack", &g_SkinChangerCfg[pWeaponItemIndexData[iWeaponID]].nFallbackStatTrak, 1, 100, ImGuiInputTextFlags_CharsDecimal);
						ImGui::Checkbox(u8"Read kills [Beta]", &g_SkinChangerCfg[pWeaponItemIndexData[iWeaponID]].LiveStatTrack);
						ImGui::PopItemWidth();
					}
					ImGui::EndChild();
					ImGui::SameLine();
					ImGui::BeginChild("sechild", ImVec2(406, 375), true);
					{
						ImGui::Checkbox(u8"Enable Profile Changer", &Settings::InventoryChanger::EnableCoins);


						if (Settings::InventoryChanger::EnableCoins)
						{
							ImGui::Text("Medal Changer");

							const char* slots[] =
							{
								"None","1 slot", u8"2 slot", u8"3 slot", u8"4 slot",u8"5 slot", u8"6 slot", u8"7 slot", u8"8 slot", u8"9 slot", u8"10 slot"
							};
							ImGui::Combo(u8"Slot", &Settings::InventoryChanger::Slot, slots, IM_ARRAYSIZE(slots));

							int Slot = Settings::InventoryChanger::Slot;

							if (Slot == 1)ImGui::InputInt("Medal ID", &Settings::InventoryChanger::Coin[1], 100, 100);
							else if (Slot == 2) ImGui::InputInt("Medal ID", &Settings::InventoryChanger::Coin[2], 100, 100);
							else if (Slot == 3) ImGui::InputInt("Medal ID", &Settings::InventoryChanger::Coin[3], 100, 100);
							else if (Slot == 4) ImGui::InputInt("Medal ID", &Settings::InventoryChanger::Coin[4], 100, 100);
							else if (Slot == 5) ImGui::InputInt("Medal ID", &Settings::InventoryChanger::Coin[5], 100, 100);
							else if (Slot == 6) ImGui::InputInt("Medal ID", &Settings::InventoryChanger::Coin[6], 100, 100);
							else if (Slot == 7) ImGui::InputInt("Medal ID", &Settings::InventoryChanger::Coin[7], 100, 100);
							else if (Slot == 8) ImGui::InputInt("Medal ID", &Settings::InventoryChanger::Coin[8], 100, 100);
							else if (Slot == 9) ImGui::InputInt("Medal ID", &Settings::InventoryChanger::Coin[9], 100, 100);
							else if (Slot == 10) ImGui::InputInt("Medal ID", &Settings::InventoryChanger::Coin[10], 100, 100);


							if (ImGui::Button("MEDAL IDS", ImVec2(300, 24)))
							{
								ShellExecute(NULL, "open", "https://tf2b.com/itemlist.php?gid=730", NULL, NULL, SW_SHOW);
							}

							const char* Ranks[] =
							{
								"Unranked",
								"Silver I",
								"Silver II",
								"Silver III",
								"Silver IV",
								"Silver Elite",
								"Silver Elite Master",
								"Gold Nova I",
								"Gold Nova II",
								"Gold Nova III",
								"Gold Nova Master",
								"Master Guardian I",
								"Master Guardian II",
								"Master Guardian Elite",
								"Distinguished Master Guardian",
								"Legendary Eagle",
								"Legendary Eagle Master",
								"Supreme Master First Class",
								"The Global Elite",
							};

							ImGui::PushItemWidth(200.f);
							ImGui::Checkbox("Profile Changer", &Settings::InventoryChanger::enbl);

							if (Settings::InventoryChanger::enbl)
							{
								ImGui::Spacing();
								ImGui::Combo(u8"Rank", &Settings::InventoryChanger::Rank, Ranks, IM_ARRAYSIZE(Ranks));
								ImGui::InputInt(u8"Victory", &Settings::InventoryChanger::Wins, 1, 10);
								ImGui::Spacing();
								ImGui::InputInt(u8"Friendly Commends", &Settings::InventoryChanger::Rec1, 1, 10);
								ImGui::InputInt(u8"Teacher Commends", &Settings::InventoryChanger::Rec2, 1, 10);
								ImGui::InputInt(u8"Great leader Commends", &Settings::InventoryChanger::Rec3, 1, 10);
								ImGui::Spacing();
								ImGui::InputInt(u8"Private Rank Level", &Settings::InventoryChanger::Level, 1, 10);
								ImGui::InputInt(u8"Private Rank XP", &Settings::InventoryChanger::XP, 1, 10);
								ImGui::PopItemWidth();
								ImGui::Spacing();
								ImGui::Separator();
								ImGui::Spacing();
							}
						}
					}
					ImGui::EndChild();
					ImGui::EndGroup();
					ImGui::BeginChild("sec342hild", ImVec2(796, 41), true);
					{
						if (ImGui::Button(u8"Apply ##Skin", ImVec2(780, 24)))
						{
							Settings::Esp::helper = false;
							ForceFullUpdate();
							SendClientHello();
							SendMMHello();
						}
					}
					ImGui::EndChild();
				}
				ImGui::End();
			}
		}
	}
}


