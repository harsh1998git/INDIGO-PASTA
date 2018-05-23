#include "Misc.h"
using namespace Client;


//[junk_enable /]
//[enc_string_enable /]
void CMisc::OnRender()
{
	if (Settings::Misc::Other_Punch)
	{
		CBaseEntity* localplayer = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());
		if (Interfaces::Engine()->IsInGame() && localplayer)
		{
			Vector ViewAngles;
			Interfaces::Engine()->GetViewAngles(ViewAngles);
			ViewAngles += (localplayer->GetAimPunchAngle()) * 2.f;

			Vector fowardVec;
			AngleVectors(ViewAngles, fowardVec);
			fowardVec *= 10000;

			Vector start = localplayer->GetEyePosition();
			Vector end = start + fowardVec, endScreen;

			if (WorldToScreen(end, endScreen) && IsLocalAlive())
			{
				g_pRender->DrawFillBox(endScreen.x - 1, endScreen.y - 1, 3, 3, Color::Green());
			}

		}
	}
}

void CMisc::OnCreateMove( CUserCmd* pCmd )
{
	if (Settings::Misc::Other_MemeWalk)
	{
		enum MoveType_t;
		if (enum MoveType_t() & (MOVETYPE_NOCLIP | MOVETYPE_LADDER | FL_ONGROUND))
			return;
		{
			if (pCmd->buttons & IN_FORWARD)
			{
				pCmd->forwardmove = 450;
				pCmd->buttons &= ~IN_FORWARD;
				pCmd->buttons |= IN_BACK;
			}
			else if (pCmd->buttons & IN_BACK)
			{
				pCmd->forwardmove = -450;
				pCmd->buttons &= ~IN_BACK;
				pCmd->buttons |= IN_FORWARD;
			}

			if (pCmd->buttons & IN_MOVELEFT)
			{
				pCmd->sidemove = -450;
				pCmd->buttons &= ~IN_MOVELEFT;
				pCmd->buttons |= IN_MOVERIGHT;
			}
			else if (pCmd->buttons & IN_MOVERIGHT)
			{
				pCmd->sidemove = 450;
				pCmd->buttons &= ~IN_MOVERIGHT;
				pCmd->buttons |= IN_MOVELEFT;
			}
		}
	}

	if (Settings::Misc::misc_Name)
		NameChange();

	if (Settings::Misc::Other_Bhop)
	{
		enum MoveType_t;
		if (enum MoveType_t() & (MOVETYPE_NOCLIP | MOVETYPE_LADDER ))
			return;

		if (pCmd->buttons & IN_JUMP && !(g_pPlayers->GetLocal()->iFlags & FL_ONGROUND))
		{
			pCmd->buttons &= ~IN_JUMP;
		}
	}

	if ( Settings::Misc::Other_AutoStrafe && !( g_pPlayers->GetLocal()->iFlags & FL_ONGROUND ) )
	{
		enum MoveType_t;
		if (enum MoveType_t() & (MOVETYPE_NOCLIP | MOVETYPE_LADDER))
			return;

		if ( pCmd->mousedx < 0 )
		{
			pCmd->sidemove = -450.0f;
		}
		else if ( pCmd->mousedx > 0 )
		{
			pCmd->sidemove = 450.0f;
		}
	}
}

void CMisc::Backtrack(CUserCmd* pCmd)
{
	if (Settings::Misc::Other_Backtrack)
	{

		int bestTargetIndex = -1;
		float bestFov = FLT_MAX;
		CBaseEntity* local = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());
		PlayerInfo info;
		for (int i = 0; i < SDK::Interfaces::Engine()->GetMaxClients(); i++)
		{
			auto entity = (CBaseEntity*)SDK::Interfaces::EntityList()->GetClientEntity(i);

			if (!entity || !local)
				continue;

			if (entity == local)
				continue;

			if (!Interfaces::Engine()->GetPlayerInfo(i, &info))
				continue;

			if (entity->IsDormant())
				continue;

			if (!entity->IsDead())
			{
				float simtime = entity->GetSimTime();
				Vector hitboxPos = entity->GetHitboxPosition(0);

				headPositions[i][pCmd->command_number % 13] = backtrackData{ simtime, hitboxPos };
				Vector ViewDir = AngleVector(pCmd->viewangles + (local->GetAimPunchAngle() * 2.f));
				float FOVDistance = DistancePointToLine(hitboxPos, local->GetEyePosition(), ViewDir);

				if (bestFov > FOVDistance)
				{
					bestFov = FOVDistance;
					bestTargetIndex = i;
				}
			}
		}

		float bestTargetSimTime;
		if (bestTargetIndex != -1)
		{
			float tempFloat = FLT_MAX;

			Vector ViewDir = AngleVector(pCmd->viewangles + (local->GetAimPunchAngle() * 2.f));

			for (int t = 0; t <= 12; ++t)
			{
				float tempFOVDistance = DistancePointToLine(headPositions[bestTargetIndex][t].hitboxPos, local->GetEyePosition(), ViewDir);

				if (tempFloat > tempFOVDistance && headPositions[bestTargetIndex][t].simtime > Interfaces::GlobalVars()->curtime - 1)
				{

					tempFloat = tempFOVDistance;
					bestTargetSimTime = headPositions[bestTargetIndex][t].simtime;
				}
			}

			if (pCmd->buttons & IN_ATTACK)
			{
				pCmd->tick_count = TIME_TO_TICKS(bestTargetSimTime);

			}

		}

	}
}

void CMisc::OnDrawModelExecute()
{
	static bool NoFlashReset = false;

	if (Settings::Misc::Other_NoFlash && !NoFlashReset)
	{
		IMaterial* flash = Interfaces::MaterialSystem()->FindMaterial(
			"effects/flashbang", TEXTURE_GROUP_CLIENT_EFFECTS);

		IMaterial* flashWhite = Interfaces::MaterialSystem()->FindMaterial("effects/flashbang_white",
			TEXTURE_GROUP_CLIENT_EFFECTS);

		if (flash && flashWhite)
		{
			flash->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			flashWhite->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);

			NoFlashReset = true;
		}
	}
	else if (!Settings::Misc::Other_NoFlash && NoFlashReset)
	{
		IMaterial* flash = Interfaces::MaterialSystem()->FindMaterial(
			"effects/flashbang", TEXTURE_GROUP_CLIENT_EFFECTS);

		IMaterial* flashWhite = Interfaces::MaterialSystem()->FindMaterial("effects/flashbang_white",
			TEXTURE_GROUP_CLIENT_EFFECTS);

		if (flash && flashWhite)
		{
			flash->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
			flashWhite->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);

			NoFlashReset = false;
		}
	}
}

void CMisc::OnPlaySound(const char* pszSoundName)
{
	if (Settings::Misc::Other_AutoAccept && !strcmp(pszSoundName, "!UI/competitive_accept_beep.wav"))
	{
		typedef void(*IsReadyCallBackFn)();

		IsReadyCallBackFn IsReadyCallBack = 0;

		if (!IsReadyCallBack)
		{
			IsReadyCallBack = (IsReadyCallBackFn)(
				CSX::Memory::FindPattern(CLIENT_DLL, "55 8B EC 83 E4 F8 83 EC 08 56 8B 35 ? ? ? ? 57 83 BE", 0));

#if ENABLE_DEBUG_FILE == 1
			CSX::Log::Add("::IsReadyCallBack = %X", IsReadyCallBack);
#endif
		}

		if (IsReadyCallBack)
		{
			IsReadyCallBack();
		}
	}
}

void CMisc::OnOverrideView(CViewSetup * pSetup)
{
	if (Settings::Misc::Other_FovChanger && !Interfaces::Engine()->IsTakingScreenshot() && Interfaces::Engine()->IsInGame())
	{
		CBaseEntity* pPlayer = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());
		//CBaseEntity* IsScoped = (CBaseEntity*)Interfaces::EntityList()->GetClientEntityFromHandle(pPlayer->GetIsScoped());
		if (!pPlayer)
			return;
		if (pPlayer->IsDead())
		{
			if (pPlayer->GetObserverMode() == ObserverMode_t::OBS_MODE_IN_EYE && pPlayer->GetObserverTarget())
				pPlayer = (CBaseEntity*)Interfaces::EntityList()->GetClientEntityFromHandle(pPlayer->GetObserverTarget());
			if (!pPlayer)
				return;
		}
		if (pPlayer->GetBaseWeapon())
		{
			if (pPlayer->GetBaseWeapon()->GetZoomLevel() == 1 || pPlayer->GetBaseWeapon()->GetZoomLevel() == 2)
				return;
		}
		pSetup->fov = (float)Settings::Misc::Other_FovView;
	}
}

void CMisc::OnGetViewModelFOV(float& fov)
{
	if (Settings::Misc::Other_FovChanger && !Interfaces::Engine()->IsTakingScreenshot() && Interfaces::Engine()->IsInGame())
	{
		CBaseEntity* pPlayer = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());
		//CBaseEntity* IsScoped = (CBaseEntity*)Interfaces::EntityList()->GetClientEntityFromHandle(pPlayer->GetIsScoped());
		if (!pPlayer)
			return;
		if (pPlayer->IsDead())
		{
			if (pPlayer->GetObserverMode() == ObserverMode_t::OBS_MODE_IN_EYE && pPlayer->GetObserverTarget())
				pPlayer = (CBaseEntity*)Interfaces::EntityList()->GetClientEntityFromHandle(pPlayer->GetObserverTarget());
			if (!pPlayer)
				return;
		}
		if (pPlayer->GetBaseWeapon())
		{
			if (pPlayer->GetBaseWeapon()->GetZoomLevel() == 1 || pPlayer->GetBaseWeapon()->GetZoomLevel() == 2)
				return;
		}
		fov = (float)Settings::Misc::Other_FovModelView;
	}
}


vector<int> CMisc::GetObservervators( int playerId )
{
	vector<int> SpectatorList;

	CBaseEntity* pPlayer = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity( playerId );

	if ( !pPlayer )
		return SpectatorList;

	if ( pPlayer->IsDead() )
	{
		CBaseEntity* pObserverTarget = (CBaseEntity*)Interfaces::EntityList()->GetClientEntityFromHandle( pPlayer->GetObserverTarget() );

		if ( !pObserverTarget )
			return SpectatorList;

		pPlayer = pObserverTarget;
	}

	for ( int PlayerIndex = 0; PlayerIndex < g_pPlayers->GetSize(); PlayerIndex++ )
	{
		CBaseEntity* pCheckPlayer = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity( PlayerIndex );

		if ( !pCheckPlayer )
			continue;

		if ( pCheckPlayer->IsDormant() || !pCheckPlayer->IsDead() )
			continue;

		CBaseEntity* pObserverTarget = (CBaseEntity*)Interfaces::EntityList()->GetClientEntityFromHandle( pCheckPlayer->GetObserverTarget() );

		if ( !pObserverTarget )
			continue;

		if ( pPlayer != pObserverTarget )
			continue;

		SpectatorList.push_back( PlayerIndex );
	}

	return SpectatorList;
}

void CMisc::OnRenderSpectatorList()
{
	if (Settings::Misc::Other_Spectators)
	{
		int specs = 0;
		int modes = 0;
		std::string spect = "";
		std::string mode = "";
		int DrawIndex = 1;
		for (int playerId : GetObservervators(Interfaces::Engine()->GetLocalPlayer()))
		{
			if (playerId == Interfaces::Engine()->GetLocalPlayer())
				continue;
			CBaseEntity* pPlayer = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(playerId);
			if (!pPlayer)
				continue;
			PlayerInfo Pinfo;
			Interfaces::Engine()->GetPlayerInfo(playerId, &Pinfo);
			if (Pinfo.m_bIsFakePlayer)
				continue;
			if (g_pRender)
			{
				spect += Pinfo.m_szPlayerName;
				spect += "\n";
				specs++;
				if (spect != "")
				{
					Color PlayerObsColor;
					//[junk_disable /] 
					switch (pPlayer->GetObserverMode())
					{
					case ObserverMode_t::OBS_MODE_IN_EYE:
						mode += (u8"От 1 лица");
						PlayerObsColor = Color::White();
						break;
					case ObserverMode_t::OBS_MODE_CHASE:
						mode += (u8"От 3 лица");
						PlayerObsColor = Color::White();
						break;
					case ObserverMode_t::OBS_MODE_ROAMING:
						mode += (u8"Свободный полет");
						PlayerObsColor = Color::Red();
						break;
					case ObserverMode_t::OBS_MODE_DEATHCAM:
						mode += (u8"Предсмертная");
						PlayerObsColor = Color::Yellow();
						break;
					case ObserverMode_t::OBS_MODE_FREEZECAM:
						mode += (u8"Замороженый");
						PlayerObsColor = Color::LimeGreen();
						break;
					case ObserverMode_t::OBS_MODE_FIXED:
						mode += (u8"Фиксирован");
						PlayerObsColor = Color::Orange();
						break;
					default:
						break;
					}
					mode += "\n";
					modes++;
				}
			}
		}
		if (ImGui::Begin("Spectator List", &Settings::Misc::Other_Spectators, ImVec2(200, 100), 0.9f, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse))
		{
			if (specs > 0) spect += "\n";
			if (modes > 0) mode += "\n";
			ImVec2 size = ImGui::CalcTextSize(spect.c_str());
			ImGui::Columns(2);
			ImGui::Text("Name");
			ImGui::NextColumn();
			ImGui::Text("Mode");
			ImGui::NextColumn();
			ImGui::Separator();
			ImGui::Text(spect.c_str());
			ImGui::NextColumn();
			ImGui::Text(mode.c_str());
			ImGui::Columns(1);
			DrawIndex++;
		}
		ImGui::End();
	}
}

