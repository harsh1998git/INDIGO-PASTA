#include "Esp.h"
#include "../../GranadeHelper/CGrenadeAPI.h"
backtrackData headPositions[64][16];
bool done = false;
using namespace Client;

//[enc_string_enable /]
//[junk_enable /]

enum
{
	PITCH = 0, // up / down
	YAW, // left / right
	ROLL // fall over
};

void inline SinCos(float radians, float* sine, float* cosine)
{
	*sine = sin(radians);
	*cosine = cos(radians);
}

void AngleVectors(const QAngle& angles, Vector* forward)
{
	float sp, sy, cp, cy;

	SinCos(DEG2RAD(angles[YAW]), &sy, &cy);
	SinCos(DEG2RAD(angles[PITCH]), &sp, &cp);

	forward->x = cp * cy;
	forward->y = cp * sy;
	forward->z = -sp;
}

int StringToWeapon(std::string weapon) {
	if (!strcmp(weapon.c_str(), "smokegrenade"))
		return 45;
	if (!strcmp(weapon.c_str(), "flashbang"))
		return 43;
	if (!strcmp(weapon.c_str(), "incgrenade"))
		return 46; // TODO: гавнокод 
}

CSoundEsp::CSoundEsp()
{
	SoundColor = Color::White();
}

void CSoundEsp::Update()
{
	for ( size_t i = 0; i < Sound.size(); i++ )
	{
		if ( Sound[i].dwTime + 800 <= GetTickCount64() )
		{
			Sound.erase( Sound.begin() + i );
		}
	}
}

void CSoundEsp::AddSound( Vector vOrigin )
{
	Sound_s Sound_Entry;

	Sound_Entry.dwTime = GetTickCount64();
	Sound_Entry.vOrigin = vOrigin;

	Sound.push_back( Sound_Entry );
}

void Draw3DTriangle(Vector pos, float size, Color color)
{
	// Drawing a Triangle
	Vector up, right, left, up_right, up_left;
	Vector s_up, s_right, s_left, s_up_right, s_up_left;

	up[0] = pos[0];
	up[1] = pos[1];
	up[2] = pos[2] + size / 2;

	WorldToScreen(up, s_up);

	up_left[0] = pos[0] - size / 2;
	up_left[1] = pos[1] + size / 2;
	up_left[2] = pos[2] - size / 2;

	WorldToScreen(up_left, s_up_left);

	up_right[0] = pos[0] + size / 2;
	up_right[1] = pos[1] + size / 2;
	up_right[2] = pos[2] - size / 2;

	WorldToScreen(up_right, s_up_right);

	right[0] = pos[0] + size / 2;
	right[1] = pos[1] - size / 2;
	right[2] = pos[2] - size / 2;

	WorldToScreen(right, s_right);

	left[0] = pos[0] - size / 2;
	left[1] = pos[1] - size / 2;
	left[2] = pos[2] - size / 2;

	WorldToScreen(left, s_left);


	g_pRender->DrawLine(s_right[0], s_right[1], s_left[0], s_left[1], color);

	g_pRender->DrawLine(s_right[0], s_right[1], s_up_right[0], s_up_right[1], color);
	g_pRender->DrawLine(s_left[0], s_left[1], s_up_left[0], s_up_left[1], color);

	g_pRender->DrawLine(s_up_right[0], s_up_right[1], s_up_left[0], s_up_left[1], color);

	g_pRender->DrawLine(s_right[0], s_right[1], s_up[0], s_up[1], color);
	g_pRender->DrawLine(s_left[0], s_left[1], s_up[0], s_up[1], color);
	g_pRender->DrawLine(s_up_left[0], s_up_left[1], s_up[0], s_up[1], color);
	g_pRender->DrawLine(s_up_right[0], s_up_right[1], s_up[0], s_up[1], color);
}

void CSoundEsp::DrawSoundEsp()
{
	for (size_t i = 0; i < Sound.size(); i++)
	{
		Vector vScreen;

		Color SoundVisuals_Color = Color(int(Settings::Colors::SoundVisuals_Color[0] * 255.f),
			int(Settings::Colors::SoundVisuals_Color[1] * 255.f),
			int(Settings::Colors::SoundVisuals_Color[2] * 255.f));

		CBaseEntity* localplayer = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());
		CPlayer* pPlayer;

		if (WorldToScreen(Sound[i].vOrigin, vScreen))
		{
			if (Settings::Esp::Visuals_Sound)
			{
				if(Settings::Esp::Visuals_SoundType == 1)
				{
					CSoundEsp::DrawWave(Sound[i].vOrigin, (int)Settings::Esp::Visuals_SoundSize , SoundVisuals_Color);
				}
				if (Settings::Esp::Visuals_SoundType == 2)
				{
					g_pRender->DrawBox((int)vScreen.x, (int)vScreen.y, (int)Settings::Esp::Visuals_SoundSize, (int)Settings::Esp::Visuals_SoundSize, SoundVisuals_Color);
				}
				if (Settings::Esp::Visuals_SoundType == 3)
				{
					g_pRender->DrawAlphaBox((int)vScreen.x, (int)vScreen.y, (int)Settings::Esp::Visuals_SoundSize, (int)Settings::Esp::Visuals_SoundSize, SoundVisuals_Color);
					g_pRender->DrawBox((int)vScreen.x, (int)vScreen.y, (int)Settings::Esp::Visuals_SoundSize, (int)Settings::Esp::Visuals_SoundSize, SoundVisuals_Color);
				}
				if (Settings::Esp::Visuals_SoundType == 4)
				{
					Draw3DTriangle(Sound[i].vOrigin,(int)Settings::Esp::Visuals_SoundSize, SoundVisuals_Color);
				}
			}
		}
	}
}

void CSoundEsp::DrawWave(Vector loc, float radius, Color color)
{
	static float Step = M_PI * 3.0f / 40;
	Vector prev;
	for (float lat = 0; lat <= M_PI * 3.0f; lat += Step)
	{
		float sin1 = sin(lat);
		float cos1 = cos(lat);
		float sin3 = sin(0.0);
		float cos3 = cos(0.0);

		Vector point1;
		point1 = Vector(sin1 * cos3, cos1, sin1 * sin3) * radius;
		Vector point3 = loc;
		Vector Out;
		point3 += point1;

		if (WorldToScreen(point3, Out))
		{
			if (lat > 0.000)
				g_pRender->DrawLine(prev.x, prev.y, Out.x, Out.y, color);
		}
		prev = Out;
	}
}

CEsp::CEsp()
{
	// Цвета хп бара

	CT_HP_ColorM.SetColor( 255 , 64 , 64 );
	TT_HP_ColorM.SetColor( 255 , 64 , 64 );

	// Цвета армор бара

	CT_AR_ColorM.SetColor( 255 , 64 , 64 );
	TT_AR_ColorM.SetColor( 255 , 64 , 64 );

	visible_flat = nullptr;
	visible_flat2 = nullptr;
	visible_tex = nullptr;
	visible_tex2 = nullptr;
	hidden_flat = nullptr;
	hidden_flat2 = nullptr;
	hidden_tex = nullptr;
	hidden_tex2 = nullptr;

	fExplodeC4Timer = 0.f;
	fC4Timer = 0.f;
}

Color CEsp::GetPlayerColor( CPlayer* pPlayer )
{
	Color CT_Color( int( Settings::Colors::Visuals_Color_CT[0] * 255.f ) , int( Settings::Colors::Visuals_Color_CT[1] * 255.f ) , int( Settings::Colors::Visuals_Color_CT[2] * 255.f ) );
	Color TT_Color( int( Settings::Colors::Visuals_Color_TT[0] * 255.f ) , int( Settings::Colors::Visuals_Color_TT[1] * 255.f ) , int( Settings::Colors::Visuals_Color_TT[2] * 255.f ) );

	if ( pPlayer->Team == TEAM_CT )
	{
		return CT_Color;
	}
	else if ( pPlayer->Team == TEAM_TT )
	{
		return TT_Color;
	}

	return Color::White();
}

Color CEsp::GetPlayerVisibleColor( CPlayer* pPlayer )
{
	Color PlayerColor = GetPlayerColor( pPlayer );

	bool SetColor = false;

	// Подсвечивать зелёным если игрок виден:

	if ( pPlayer->bVisible )
	{
		if ( Settings::Esp::Visuals_Visible == 0 && pPlayer->Team != g_pPlayers->GetLocal()->Team ) // Противников
		{
			SetColor = true;
		}
		else if ( Settings::Esp::Visuals_Visible == 1 && pPlayer->Team == g_pPlayers->GetLocal()->Team ) // Своих
		{
			SetColor = true;
		}
		else if ( Settings::Esp::Visuals_Visible == 2 ) // Всех
		{
			SetColor = true;
		}

		Color VCT_Color( int( Settings::Colors::Visuals_Color_VCT[0] * 255.f ) , int( Settings::Colors::Visuals_Color_VCT[1] * 255.f ) , int( Settings::Colors::Visuals_Color_VCT[2] * 255.f ) );
		Color VTT_Color( int( Settings::Colors::Visuals_Color_VTT[0] * 255.f ) , int( Settings::Colors::Visuals_Color_VTT[1] * 255.f ) , int( Settings::Colors::Visuals_Color_VTT[2] * 255.f ) );

		if ( SetColor )
		{
			if ( pPlayer->Team == TEAM_CT )
			{
				PlayerColor = VCT_Color;
			}
			else if ( pPlayer->Team == TEAM_TT )
			{
				PlayerColor = VTT_Color;
			}
		}
	}

	return PlayerColor;
}

bool CEsp::CheckPlayerTeam( CPlayer* pPlayer )
{
	bool CheckTeam = false;
	bool PlayerVisible = pPlayer->bVisible;

	// Показываем esp только на:
	if ( Settings::Esp::Visuals_Enemy && pPlayer->Team != g_pPlayers->GetLocal()->Team ) // Противников
		CheckTeam = true;

	if ( Settings::Esp::Visuals_Team && pPlayer->Team == g_pPlayers->GetLocal()->Team ) // Своих
		CheckTeam = true;

	if ( Settings::Esp::Visuals_Visible >= 3 && !PlayerVisible )
		CheckTeam = false;

	return CheckTeam;
}

void CEsp::GrenadePrediction()
{
	CBaseEntity* pLocal = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());

	const float TIMEALIVE = 5.f;
	const float GRENADE_COEFFICIENT_OF_RESTITUTION = 0.4f;

	float fStep = 0.1f;
	float fGravity = 800.0f / 8.f;

	Vector vPos, vThrow, vThrow2;
	Vector vStart;

	int iCollisions = 0;

	QAngle vViewAngles;
	Interfaces::Engine()->GetViewAngles(vViewAngles);

	vThrow[0] = vViewAngles[0];
	vThrow[1] = vViewAngles[1];
	vThrow[2] = vViewAngles[2];

	if (vThrow[0] < 0)
		vThrow[0] = -10 + vThrow[0] * ((90 - 10) / 90.0);
	else
		vThrow[0] = -10 + vThrow[0] * ((90 + 10) / 90.0);

	float fVel = (90 - vThrow[0]) * 4;
	if (fVel > 500)
		fVel = 500;

	AngleVectors(vThrow, vThrow2);

	Vector vEye = pLocal->GetEyePosition();
	vStart[0] = vEye[0] + vThrow2[0] * 16;
	vStart[1] = vEye[1] + vThrow2[1] * 16;
	vStart[2] = vEye[2] + vThrow2[2] * 16;

	vThrow2[0] = (vThrow2[0] * fVel) + pLocal->GetVelocity()[0];
	vThrow2[1] = (vThrow2[1] * fVel) + pLocal->GetVelocity()[1];
	vThrow2[2] = (vThrow2[2] * fVel) + pLocal->GetVelocity()[2];

#define IsGrenade g_pPlayers->GetLocal()->WeaponIndex == WEAPON_SMOKEGRENADE | g_pPlayers->GetLocal()->WeaponIndex == WEAPON_HEGRENADE | g_pPlayers->GetLocal()->WeaponIndex == WEAPON_INCGRENADE | g_pPlayers->GetLocal()->WeaponIndex == WEAPON_MOLOTOV | g_pPlayers->GetLocal()->WeaponIndex == WEAPON_DECOY | g_pPlayers->GetLocal()->WeaponIndex == WEAPON_FLASHBANG

	if (!pLocal->IsDead() && IsGrenade)
	{
		for (float fTime = 0.0f; fTime < TIMEALIVE; fTime += fStep)
		{
			vPos = vStart + vThrow2 * fStep;

			Ray_t ray;
			trace_t tr;
			CTraceFilter loc;
			loc.pSkip = pLocal;

			ray.Init(vStart, vPos);
			Interfaces::EngineTrace()->TraceRay(ray, MASK_SOLID, &loc, &tr);
			Color color = Color(0, 255, 0);
			if (tr.DidHit())
			{
				float anglez = DotProduct(Vector(0, 0, 1), tr.plane.normal);
				float invanglez = DotProduct(Vector(0, 0, -1), tr.plane.normal);
				float angley = DotProduct(Vector(0, 1, 0), tr.plane.normal);
				float invangley = DotProduct(Vector(0, -1, 0), tr.plane.normal);
				float anglex = DotProduct(Vector(1, 0, 0), tr.plane.normal);
				float invanglex = DotProduct(Vector(3 - 1, 0, 0), tr.plane.normal);
				float scale = tr.endpos.DistTo(pLocal->GetOrigin()) / 60;
				Color color = Color(0, 0, 255);
				if (anglez > 0.5)
				{
					tr.endpos.z += 1;
					Vector startPos = tr.endpos + Vector(-scale, 0, 0);
					Vector endPos = tr.endpos + Vector(scale, 0, 0);
					Vector outStart, outEnd;
					if (WorldToScreen(startPos, outStart) && WorldToScreen(endPos, outEnd))
						g_pRender->DrawLine((int)outStart.x, (int)outStart.y, (int)outEnd.x, (int)outEnd.y, Color::Green());

					startPos = tr.endpos + Vector(0, -scale, 0);
					endPos = tr.endpos + Vector(0, scale, 0);
					if (WorldToScreen(startPos, outStart) && WorldToScreen(endPos, outEnd))
						g_pRender->DrawLine((int)outStart.x, (int)outStart.y, (int)outEnd.x, (int)outEnd.y, Color::Green());
				}
				else if (invanglez > 0.5)
				{
					tr.endpos.z += 1;
					Vector startPos = tr.endpos + Vector(-scale, 0, 0);
					Vector endPos = tr.endpos + Vector(scale, 0, 0);
					Vector outStart, outEnd;
					if (WorldToScreen(startPos, outStart) && WorldToScreen(endPos, outEnd))
						g_pRender->DrawLine((int)outStart.x, (int)outStart.y, (int)outEnd.x, (int)outEnd.y, Color::Green());

					startPos = tr.endpos + Vector(0, -scale, 0);
					endPos = tr.endpos + Vector(0, scale, 0);
					if (WorldToScreen(startPos, outStart) && WorldToScreen(endPos, outEnd))
						g_pRender->DrawLine((int)outStart.x, (int)outStart.y, (int)outEnd.x, (int)outEnd.y, Color::Green());
				}
				else if (angley > 0.5)
				{
					tr.endpos.y += 1;
					Vector startPos = tr.endpos + Vector(0, 0, -scale);
					Vector endPos = tr.endpos + Vector(0, 0, scale);
					Vector outStart, outEnd;
					if (WorldToScreen(startPos, outStart) && WorldToScreen(endPos, outEnd))
						g_pRender->DrawLine((int)outStart.x, (int)outStart.y, (int)outEnd.x, (int)outEnd.y, Color::Green());

					startPos = tr.endpos + Vector(-scale, 0, 0);
					endPos = tr.endpos + Vector(scale, 0, 0);
					if (WorldToScreen(startPos, outStart) && WorldToScreen(endPos, outEnd))
						g_pRender->DrawLine((int)outStart.x, (int)outStart.y, (int)outEnd.x, (int)outEnd.y, Color::Green());
				}
				else if (invangley > 0.5)
				{
					tr.endpos.y += 1;
					Vector startPos = tr.endpos + Vector(0, 0, -scale);
					Vector endPos = tr.endpos + Vector(0, 0, scale);
					Vector outStart, outEnd;
					if (WorldToScreen(startPos, outStart) && WorldToScreen(endPos, outEnd))
						g_pRender->DrawLine((int)outStart.x, (int)outStart.y, (int)outEnd.x, (int)outEnd.y, Color::Green());

					startPos = tr.endpos + Vector(-scale, 0, 0);
					endPos = tr.endpos + Vector(scale, 0, 0);
					if (WorldToScreen(startPos, outStart) && WorldToScreen(endPos, outEnd))
						g_pRender->DrawLine((int)outStart.x, (int)outStart.y, (int)outEnd.x, (int)outEnd.y, Color::Green());
				}
				else if (anglex > 0.5)
				{
					tr.endpos.x += 1;
					Vector startPos = tr.endpos + Vector(0, -scale, 0);
					Vector endPos = tr.endpos + Vector(0, scale, 0);
					Vector outStart, outEnd;
					if (WorldToScreen(startPos, outStart) && WorldToScreen(endPos, outEnd))
						g_pRender->DrawLine((int)outStart.x, (int)outStart.y, (int)outEnd.x, (int)outEnd.y, Color::Green());

					startPos = tr.endpos + Vector(0, 0, -scale);
					endPos = tr.endpos + Vector(0, 0, scale);
					if (WorldToScreen(startPos, outStart) && WorldToScreen(endPos, outEnd))
						g_pRender->DrawLine((int)outStart.x, (int)outStart.y, (int)outEnd.x, (int)outEnd.y, Color::Green());
				}
				else if (invanglex > 0.5)
				{
					tr.endpos.x += 1;
					Vector startPos = tr.endpos + Vector(0, -scale, 0);
					Vector endPos = tr.endpos + Vector(0, scale, 0);
					Vector outStart, outEnd;
					if (WorldToScreen(startPos, outStart) && WorldToScreen(endPos, outEnd))
						g_pRender->DrawLine((int)outStart.x, (int)outStart.y, (int)outEnd.x, (int)outEnd.y, Color::Green());

					startPos = tr.endpos + Vector(0, 0, -scale);
					endPos = tr.endpos + Vector(0, 0, scale);
					if (WorldToScreen(startPos, outStart) && WorldToScreen(endPos, outEnd))
						g_pRender->DrawLine((int)outStart.x, (int)outStart.y, (int)outEnd.x, (int)outEnd.y, Color::Green());
				}

				vThrow2 = tr.plane.normal * -2.0f * DotProduct(vThrow2, tr.plane.normal) + vThrow2;
				vThrow2 *= GRENADE_COEFFICIENT_OF_RESTITUTION;

				iCollisions++;
				if (iCollisions > 2)
					break;

				vPos = vStart + vThrow2 * tr.fraction * fStep;
				fTime += (fStep * (1 - tr.fraction));
			}

			Vector vOutStart, vOutEnd;

			if (WorldToScreen(vStart, vOutStart) && WorldToScreen(vPos, vOutEnd))
				g_pRender->DrawLine((int)vOutStart.x, (int)vOutStart.y, (int)vOutEnd.x, (int)vOutEnd.y, Color::Blue());

			vStart = vPos;
			vThrow2.z -= fGravity * tr.fraction * fStep;
		}
	}
}

void CEsp::OnRender()
{
	if (Settings::Esp::Visuals_Sound)
		SoundEsp.DrawSoundEsp();

	if (Settings::Esp::Visuals_GrenadePrediction)
		GrenadePrediction();

	if ( g_pAimbot )
		g_pAimbot->OnRender();

	if ( Settings::Misc::Other_AwpAim && IsLocalAlive() && g_pPlayers->GetLocal()->WeaponIndex == WEAPON_AWP )
	{
		Color AwpAimColor = Color( int( Settings::Misc::Other_AwpAimColor[0] * 255.f ) ,
								   int( Settings::Misc::Other_AwpAimColor[1] * 255.f ) ,
								   int( Settings::Misc::Other_AwpAimColor[2] * 255.f ) );

		g_pRender->DrawFillBox( iScreenWidth / 2 - 1 , iScreenHeight / 2 - 1 , 3 , 3 , AwpAimColor );
	}

	if (Settings::Misc::Other_AwpAim && IsLocalAlive() && g_pPlayers->GetLocal()->WeaponIndex == WEAPON_SSG08 )
	{
		Color AwpAimColor = Color(int(Settings::Misc::Other_AwpAimColor[0] * 255.f),
			int(Settings::Misc::Other_AwpAimColor[1] * 255.f),
			int(Settings::Misc::Other_AwpAimColor[2] * 255.f));

		g_pRender->DrawFillBox(iScreenWidth / 2 - 1, iScreenHeight / 2 - 1, 3, 3, AwpAimColor);
	}

	if (Settings::Misc::Other_AwpAim && IsLocalAlive() && g_pPlayers->GetLocal()->WeaponIndex == WEAPON_SCAR20 )
	{
		Color AwpAimColor = Color(int(Settings::Misc::Other_AwpAimColor[0] * 255.f),
			int(Settings::Misc::Other_AwpAimColor[1] * 255.f),
			int(Settings::Misc::Other_AwpAimColor[2] * 255.f));

		g_pRender->DrawFillBox(iScreenWidth / 2 - 1, iScreenHeight / 2 - 1, 3, 3, AwpAimColor);
	}

	if (Settings::Misc::Other_AwpAim && IsLocalAlive() && g_pPlayers->GetLocal()->WeaponIndex == WEAPON_G3SG1 )
	{
		Color AwpAimColor = Color(int(Settings::Misc::Other_AwpAimColor[0] * 255.f),
								  int(Settings::Misc::Other_AwpAimColor[1] * 255.f),
								  int(Settings::Misc::Other_AwpAimColor[2] * 255.f));

		g_pRender->DrawFillBox(iScreenWidth / 2 - 1, iScreenHeight / 2 - 1, 3, 3, AwpAimColor);
	}

	CBaseEntity* local = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());
	for ( BYTE PlayerIndex = 0; PlayerIndex < g_pPlayers->GetSize(); PlayerIndex++ )
	{
		CPlayer* pPlayer = g_pPlayers->GetPlayer( PlayerIndex );

		if ( pPlayer && pPlayer->m_pEntity && pPlayer->bUpdate && CheckPlayerTeam( pPlayer ) )
		{
			DrawPlayerEsp( pPlayer );

			if (Settings::Esp::Visuals_track)
			{
				if (IsLocalAlive())
				{
					for (int t = 0; t < 12; ++t)
					{
						Vector screenbacktrack[64][12];

						if (headPositions[PlayerIndex][t].simtime && headPositions[PlayerIndex][t].simtime + 1 > local->GetSimTime())
						{
							if (WorldToScreen(headPositions[PlayerIndex][t].hitboxPos, screenbacktrack[PlayerIndex][t]))
							{
								g_pRender->DrawFillBox(screenbacktrack[PlayerIndex][t].x, screenbacktrack[PlayerIndex][t].y, 3, 2, Color(0, 195, 255, 255));
							}
						}
					}
				}
				else
				{
					memset(&headPositions[0][0], 0, sizeof(headPositions));
				}
			}

			if ( Settings::Esp::Visuals_Skeleton )
				DrawPlayerSkeleton( pPlayer );
		}
	}

	if (Settings::Esp::Visuals_BombTimer)
	{
		if (bC4Timer && iC4Timer)
		{
			float fTimeStamp = Interfaces::Engine()->GetLastTimeStamp();

			if (!fExplodeC4Timer)
			{
				fExplodeC4Timer = fTimeStamp + (float)iC4Timer;
			}
			else
			{
				fC4Timer = fExplodeC4Timer - fTimeStamp;

				if (fC4Timer < 0.f)
					fC4Timer = 0.f;
			}
		}
		else
		{
			fExplodeC4Timer = 0.f;
			fC4Timer = 0.f;
		}
	}
	
	Color GrenadeHelper = Color(int(Settings::Colors::GrenadeHelper[0] * 255.f),
		int(Settings::Colors::GrenadeHelper[1] * 255.f),
		int(Settings::Colors::GrenadeHelper[2] * 255.f));

	if (Settings::Esp::helper)
	{
		CBaseEntity* local = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());
		
		for (int i = 0; i < cGrenade.GrenadeInfo.size(); i++)
		{
			GrenadeInfo_t info;
			if (!cGrenade.GetInfo(i, &info))
				continue;

			int iGrenadeID = StringToWeapon(info.szWeapon);

			
			if (!local->GetBaseWeapon())
				continue;

			if (!(local->GetBaseWeapon()->GeteAttributableItem()->GetItemDefinitionIndex2() == iGrenadeID || (iGrenadeID == 46 && local->GetBaseWeapon()->GeteAttributableItem()->GetItemDefinitionIndex2() == 48)))
				continue;

			Vector vecOnScreenOrigin, vecOnScreenAngles;
			int iCenterY, iCenterX;
			Interfaces::Engine()->GetScreenSize(iCenterY, iCenterX);
			iCenterX /= 2;
			iCenterY /= 2;

			float dist = sqrt(pow(local->GetRenderOrigin().x - info.vecOrigin.x, 2) + pow(local->GetRenderOrigin().y - info.vecOrigin.y, 2) + pow(local->GetRenderOrigin().z - info.vecOrigin.z, 2)) * 0.0254f;
			
			if (dist < 0.5f) 
			{
				if (WorldToScreen(info.vecOrigin, vecOnScreenOrigin))
					g_pRender->DrawWave1(info.vecOrigin, 4, Color::Red());

				Vector vecAngles;
				AngleVectors(info.vecViewangles, &vecAngles);
				vecAngles *= 100;

				if (WorldToScreen((local->GetEyePosition() + vecAngles), vecAngles))
					g_pRender->DrawFillBox(vecAngles.x, vecAngles.y, 7, 7, Color::Green());

				g_pRender->Text(iCenterX, iCenterY + 30, true, true, Color::White(), (char*)info.szName.c_str());
				g_pRender->Text(iCenterX, iCenterY, true, true, Color::White(), (char*)info.szDescription.c_str());

			}
			else
			{
				if (WorldToScreen(info.vecOrigin, vecOnScreenOrigin));

					g_pRender->DrawWave1(info.vecOrigin, 10, Color (GrenadeHelper));
					g_pRender->DrawWave1(info.vecOrigin, 7, Color (GrenadeHelper) );
			}
		}
	}

	if ( Settings::Esp::Visuals_Bomb || Settings::Esp::Visuals_WorldWeapons || Settings::Esp::Visuals_WorldGrenade )
	{
		for ( int EntIndex = 0; EntIndex < Interfaces::EntityList()->GetHighestEntityIndex(); EntIndex++ )
		{
			CBaseEntity* pEntity = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity( EntIndex );

			if ( !pEntity || pEntity->IsPlayer() )
				continue;

			const model_t* pModel = pEntity->GetModel();

			if ( pModel )
			{
				const char* pModelName = Interfaces::ModelInfo()->GetModelName( pModel );

				if ( pModelName )
				{
					Vector vEntScreen;

					if ( WorldToScreen( pEntity->GetRenderOrigin() , vEntScreen ) )
					{
						if (Settings::Esp::Visuals_Bomb && pEntity->GetClientClass()->m_ClassID == (int)CLIENT_CLASS_ID::CC4)
						{
							g_pRender->Text((int)vEntScreen.x, (int)vEntScreen.y, true, true, Color::Yellow(),
								"[C4]");
						}
						if (Settings::Esp::Visuals_Bomb && pEntity->GetClientClass()->m_ClassID == (int)CLIENT_CLASS_ID::CPlantedC4)
						{
							g_pRender->Text((int)vEntScreen.x, (int)vEntScreen.y, true, true, Color::Orange(),
								"[C4 PLANTED]");

							g_pRender->Text((int)vEntScreen.x, (int)vEntScreen.y - 13, true, true, Color::Orange(),
								C4_TIMER1_STRING, g_pEsp->fC4Timer);
						}
					}

						if ( Settings::Esp::Visuals_WorldWeapons && !strstr( pModelName , "models/weapons/w_eq_" )
							 && !strstr( pModelName , "models/weapons/w_ied" ) ) // бомба не оружие
						{
							if ( strstr( pModelName , "models/weapons/w_" ) && strstr( pModelName , "_dropped.mdl" ) )
							{
								string WeaponName = pModelName + 17;
								

								Color ColorWorldWeapon = Color(int(Settings::Colors::WorldWeapon_Color[0] * 255.f),
									int(Settings::Colors::WorldWeapon_Color[1] * 255.f),
									int(Settings::Colors::WorldWeapon_Color[2] * 255.f));

								WeaponName[WeaponName.size() - 12] = '\0';

								g_pRender->Text( (int)vEntScreen.x , (int)vEntScreen.y , true , true , ColorWorldWeapon,
												 WeaponName.c_str() );
							}
						}

						if ( Settings::Esp::Visuals_WorldGrenade &&
							( strstr( pModelName , "models/weapons/w_eq_" ) ||
							 strstr( pModelName , "models/Weapons/w_eq_" ) ) )
						{
							if ( strstr( pModelName , "_dropped.mdl" ) )
							{
								string WeaponName = pModelName + 20;

								WeaponName[WeaponName.size() - 12] = '\0';

								Color GrenadeColor = Color::White(); // по умолчанию decoy - это ложная

								if ( strstr( pModelName , "fraggrenade" ) || strstr( pModelName , "molotov" ) ) // осколочная или молотов
								{
									Color ColorGrenade = Color(int(Settings::Colors::Fraggrenade_Color[0] * 255.f),
										int(Settings::Colors::Fraggrenade_Color[1] * 255.f),
										int(Settings::Colors::Fraggrenade_Color[2] * 255.f));

									g_pRender->Text((int)vEntScreen.x, (int)vEntScreen.y, true, true, ColorGrenade,
										WeaponName.c_str());
								}
								else if ( strstr( pModelName , "flashbang" ) ) // световая
								{
									Color ColorGrenadeFlashbang = Color(int(Settings::Colors::Flashbang_Color[0] * 255.f),
										int(Settings::Colors::Flashbang_Color[1] * 255.f),
										int(Settings::Colors::Flashbang_Color[2] * 255.f));


									g_pRender->Text((int)vEntScreen.x, (int)vEntScreen.y, true, true, ColorGrenadeFlashbang,
										WeaponName.c_str());
								}
							}
							else if ( strstr( pModelName , "smokegrenade_thrown.mdl" ) ) // смок
							{
								string WeaponName = pModelName + 20;

								WeaponName[WeaponName.size() - 11] = '\0';


								Color ColorSmokeGrenade = Color(int(Settings::Colors::Smokegrenade_Color[0] * 255.f),
									int(Settings::Colors::Smokegrenade_Color[1] * 255.f),
									int(Settings::Colors::Smokegrenade_Color[2] * 255.f));

								g_pRender->Text( (int)vEntScreen.x , (int)vEntScreen.y , true , true , ColorSmokeGrenade,
												 WeaponName.c_str() );
							}
						}
					}
				}
			}
		}
	}



void MsgFunc_ServerRankRevealAll()
{
	using tServerRankRevealAllFn = bool( __cdecl* )( int* );
	static tServerRankRevealAllFn ServerRankRevealAll = 0;

	if ( !ServerRankRevealAll )
	{
		ServerRankRevealAll = (tServerRankRevealAllFn)(
			CSX::Memory::FindPattern( CLIENT_DLL , "55 8B EC 8B 0D ? ? ? ? 68" , 0 ) );
	}

	if ( ServerRankRevealAll )
	{
		int fArray[3] = { 0,0,0 };
		ServerRankRevealAll( fArray );
	}
}

void CEsp::OnCreateMove( CUserCmd* pCmd )
{
	if ( Settings::Esp::Visuals_Rank && pCmd->buttons & IN_SCORE )
		MsgFunc_ServerRankRevealAll();
	
	g_pEsp->SoundEsp.Update();
}

void CEsp::OnReset()
{
	g_pEsp->SoundEsp.Sound.clear();

	if (Settings::Esp::Visuals_BombTimer)
	{
		if (Settings::Esp::Visuals_BombTimer > 60)
			Settings::Esp::Visuals_BombTimer = 60;

		bC4Timer = false;
		iC4Timer = Settings::Esp::Visuals_BombTimer;
	}
}

void CEsp::OnEvents( IGameEvent* pEvent )
{
	if (g_pEsp && Settings::Esp::Visuals_BombTimer)
	{
		if (!strcmp(pEvent->GetName(), "bomb_defused") || !strcmp(pEvent->GetName(), "bomb_exploded"))
		{
			bC4Timer = false;
		}
		else if (!strcmp(pEvent->GetName(), "bomb_planted"))
		{
			bC4Timer = true;
		}
	}
}

void CEsp::OnDrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld)
{
	if (!g_pPlayers || Interfaces::Engine()->IsTakingScreenshot() || !Interfaces::Engine()->IsConnected() || !pInfo.pModel)
		return;

	static bool InitalizeMaterial = false;

	if (!InitalizeMaterial)
	{
		visible_flat = CreateMaterial(true, false);
		visible_flat2 = CreateMaterial(false, false);
		visible_tex = CreateMaterial(false, false);
		visible_tex2 = CreateMaterial(false, false);
		hidden_flat = CreateMaterial(true, true);
		hidden_flat2 = CreateMaterial(false, true);
		hidden_tex = CreateMaterial(false, true);
		hidden_tex2 = CreateMaterial(false, true);
		InitalizeMaterial = true;

		return;
	}

	string strModelName = Interfaces::ModelInfo()->GetModelName(pInfo.pModel);

	if (strModelName.size() <= 1)
		return;

	if (Settings::Misc::Other_ArmMaterials)
	{
		switch (Settings::Misc::Other_ArmMaterialsType)
		{
		case 0: if (strModelName.find("arms") != std::string::npos)
		{
			IMaterial *material;
			switch (Settings::Misc::Other_ArmMaterialsList)
			{
			case 0: material = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/cologne_prediction/cologne_prediction_glass", TEXTURE_GROUP_OTHER); break; // Glass
			case 1:	material = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/trophy_majors/crystal_clear", TEXTURE_GROUP_OTHER); break; // Crystal
			case 2:	material = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/trophy_majors/gold", TEXTURE_GROUP_OTHER); break; // Gold
			case 3:	material = Interfaces::MaterialSystem()->FindMaterial("models/gibs/glass/glass", TEXTURE_GROUP_OTHER); break; // Dark Chrome
			case 4: material = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/trophy_majors/gloss", TEXTURE_GROUP_OTHER); break; // Plastic Glass
			case 5:	material = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/trophy_majors/velvet", TEXTURE_GROUP_OTHER); break; // Velvet
			case 6: material = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/trophy_majors/crystal_blue", TEXTURE_GROUP_OTHER); break; // Crystal Blue
			case 7: material = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/wildfire_gold/wildfire_gold_detail", TEXTURE_GROUP_OTHER); break; // Detailed Gold
			}
			Color color = Color(255, 255, 255, 255);
			ForceMaterial(color, material);
			Interfaces::ModelRender()->DrawModelExecute(ctx, state, pInfo, pCustomBoneToWorld);
		} break;
		case 1: if (strModelName.find("weapons/v") != std::string::npos)
		{
			IMaterial *material;
			switch (Settings::Misc::Other_ArmMaterialsList)
			{
			case 0: material = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/cologne_prediction/cologne_prediction_glass", TEXTURE_GROUP_OTHER); break; // Glass
			case 1:	material = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/trophy_majors/crystal_clear", TEXTURE_GROUP_OTHER); break; // Crystal
			case 2:	material = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/trophy_majors/gold", TEXTURE_GROUP_OTHER); break; // Gold
			case 3:	material = Interfaces::MaterialSystem()->FindMaterial("models/gibs/glass/glass", TEXTURE_GROUP_OTHER); break; // Dark Chrome
			case 4: material = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/trophy_majors/gloss", TEXTURE_GROUP_OTHER); break; // Plastic Glass
			case 5:	material = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/trophy_majors/velvet", TEXTURE_GROUP_OTHER); break; // Velvet
			case 6: material = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/trophy_majors/crystal_blue", TEXTURE_GROUP_OTHER); break; // Crystal Blue
			case 7: material = Interfaces::MaterialSystem()->FindMaterial("models/inventory_items/wildfire_gold/wildfire_gold_detail", TEXTURE_GROUP_OTHER); break; // Detailed Gold
			}
			Color color = Color(255, 255, 255, 255);
			ForceMaterial(color, material);
			Interfaces::ModelRender()->DrawModelExecute(ctx, state, pInfo, pCustomBoneToWorld);
		} break;
		}
	}

	if (Settings::Misc::Other_NoHands)
	{
		if (strModelName.find("arms") != string::npos && Settings::Misc::Other_NoHands)
		{
			IMaterial* Hands = Interfaces::MaterialSystem()->FindMaterial(strModelName.c_str(), TEXTURE_GROUP_MODEL);
			Hands->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			Interfaces::ModelRender()->ForcedMaterialOverride(Hands);
		}
	}
	else
	{
		if (strModelName.find("arms") != string::npos)
		{
			IMaterial* Hands = Interfaces::MaterialSystem()->FindMaterial(strModelName.c_str(), TEXTURE_GROUP_MODEL);
			Hands->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
		}
	}

	if (Settings::Misc::Other_WireHands) //Wireframe Hands
	{
		if (strModelName.find("arms") != string::npos)
		{
			IMaterial* WireHands = Interfaces::MaterialSystem()->FindMaterial(strModelName.c_str(), TEXTURE_GROUP_MODEL);
			WireHands->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
			Interfaces::ModelRender()->ForcedMaterialOverride(WireHands);
		}
	}
	else
	{
		if (strModelName.find("arms") != string::npos)
		{
			IMaterial* WireHands = Interfaces::MaterialSystem()->FindMaterial(strModelName.c_str(), TEXTURE_GROUP_MODEL);
			WireHands->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
		}
	}

	if (Settings::Esp::Visuals_Chams && Client::g_pPlayers && Client::g_pPlayers->GetLocal() && strModelName.find("models/player") != string::npos)
	{
		IClientEntity* pBaseEntity = Interfaces::EntityList()->GetClientEntity(pInfo.entity_index);

		if (pBaseEntity && pBaseEntity->GetClientClass()->m_ClassID == (int)CLIENT_CLASS_ID::CCSPlayer)
		{
			CPlayer* pPlayer = g_pPlayers->GetPlayer(pInfo.entity_index);

			if (pPlayer && pPlayer->bUpdate)
			{
				bool CheckTeam = false;

				Color TeamHideColor;
				Color TeamVisibleColor;

				if (Settings::Esp::Visuals_Enemy && pPlayer->Team != g_pPlayers->GetLocal()->Team) // Противников
					CheckTeam = true;

				if (Settings::Esp::Visuals_Team && pPlayer->Team == g_pPlayers->GetLocal()->Team) // Своих
					CheckTeam = true;

				if (pPlayer->Team == TEAM_CT)
				{
					TeamHideColor = Color(int(Settings::Colors::chams_Color_CT[0] * 255.f),
						int(Settings::Colors::chams_Color_CT[1] * 255.f),
						int(Settings::Colors::chams_Color_CT[2] * 255.f));
				}
				else if (pPlayer->Team == TEAM_TT)
				{
					TeamHideColor = Color(int(Settings::Colors::chams_Color_TT[0] * 255.f),
						int(Settings::Colors::chams_Color_TT[1] * 255.f),
						int(Settings::Colors::chams_Color_TT[2] * 255.f));
				}

				bool SetColor = false;

				if (pPlayer->Team != g_pPlayers->GetLocal()->Team) // Противников
				{
					SetColor = true;
				}
				else if (pPlayer->Team == g_pPlayers->GetLocal()->Team) // Своих
				{
					SetColor = true;
				}

				if (SetColor)
				{
					if (pPlayer->Team == TEAM_CT)
					{
						TeamVisibleColor = Color(int(Settings::Colors::chams_Color_VCT[0] * 255.f),
							int(Settings::Colors::chams_Color_VCT[1] * 255.f),
							int(Settings::Colors::chams_Color_VCT[2] * 255.f));
					}
					else if (pPlayer->Team == TEAM_TT)
					{
						TeamVisibleColor = Color(int(Settings::Colors::chams_Color_VTT[0] * 255.f),
							int(Settings::Colors::chams_Color_VTT[1] * 255.f),
							int(Settings::Colors::chams_Color_VTT[2] * 255.f));
					}
				}
				else
					TeamVisibleColor = TeamHideColor;

				if (Settings::Esp::Visuals_ChamsRainbow)
				{
					Settings::Esp::Visuals_HPBasedChams = false;
					static float rainbow; rainbow += 0.00005f; if
						(rainbow > 0.999f) rainbow = 0.f;
					TeamHideColor = Color::FromHSB(rainbow, 1.f, 1.f);
					TeamVisibleColor = Color::FromHSB(rainbow, 1.f, 1.f);
				}

				if (Settings::Esp::Visuals_HPBasedChams)
				{
					Settings::Esp::Visuals_ChamsRainbow = false;

					int iHealth = pPlayer->iHealth;

					if (iHealth >= 90 && iHealth <= 100)
					{
						TeamHideColor = Color(51, 204, 51);
						TeamVisibleColor = Color(51, 204, 51);
					}
					if (iHealth >= 80 && iHealth <= 89)
					{
						TeamHideColor = Color(102, 255, 51);
						TeamVisibleColor = Color(102, 255, 51);
					}
					if (iHealth >= 70 && iHealth <= 79)
					{
						TeamHideColor = Color(153, 255, 51);
						TeamVisibleColor = Color(153, 255, 51);
					}
					if (iHealth >= 60 && iHealth <= 69)
					{
						TeamHideColor = Color(204, 255, 51);
						TeamVisibleColor = Color(204, 255, 51);
					}
					if (iHealth >= 50 && iHealth <= 59)
					{
						TeamHideColor = Color(255, 255, 0);
						TeamVisibleColor = Color(255, 255, 0);
					}
					if (iHealth >= 40 && iHealth <= 49)
					{
						TeamHideColor = Color(255, 204, 0);
						TeamVisibleColor = Color(255, 204, 0);
					}
					if (iHealth >= 30 && iHealth <= 39)
					{
						TeamHideColor = Color(255, 153, 51);
						TeamVisibleColor = Color(255, 153, 51);
					}
					if (iHealth >= 20 && iHealth <= 29)
					{
						TeamHideColor = Color(255, 102, 0);
						TeamVisibleColor = Color(255, 102, 0);
					}
					if (iHealth >= 10 && iHealth <= 19)
					{
						TeamHideColor = Color(204, 0, 0);
						TeamVisibleColor = Color(204, 0, 0);
					}
					if (iHealth >= 0 && iHealth <= 9)
					{
						TeamHideColor = Color(128, 0, 0);
						TeamVisibleColor = Color(128, 0, 0);
					}
				}

				if (CheckTeam)
				{
					if (Settings::Esp::Visuals_Visible <= 2)
					{
						if (Settings::Esp::Visuals_Chams == 1)
						{
							ForceMaterial(TeamHideColor, hidden_flat);
							hidden_flat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
						}
						else if (Settings::Esp::Visuals_Chams == 2)
						{
							ForceMaterial(TeamHideColor, hidden_tex);
							hidden_tex->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
						}
					}
					else
					{
						if (Settings::Esp::Visuals_Chams == 1)
						{
							ForceMaterial(TeamHideColor, hidden_flat);
							hidden_flat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
						}
						else if (Settings::Esp::Visuals_Chams == 2)
						{
							ForceMaterial(TeamHideColor, hidden_tex);
							hidden_tex->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
						}
					}

					Interfaces::ModelRender()->DrawModelExecute(ctx, state, pInfo, pCustomBoneToWorld);

					if (Settings::Esp::Visuals_Chams == 1)
					{
						ForceMaterial(TeamVisibleColor, visible_flat);
						visible_flat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
					}
					else if (Settings::Esp::Visuals_Chams == 2)
					{
						ForceMaterial(TeamVisibleColor, visible_tex);
						visible_tex->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
					}
				}
			}
		}
	}
}

void CEsp::DrawPlayerEsp( CPlayer* pPlayer )
{
	bool bOriginScreen = ( pPlayer->vOriginScreen.x > 0 && pPlayer->vOriginScreen.y > 0 );
	bool bHitBoxScreen = ( pPlayer->vHitboxHeadScreen.x > 0 && pPlayer->vHitboxHeadScreen.y > 0 );

	if ( !bOriginScreen && !bHitBoxScreen )
		return;

	Vector vLineOrigin;

	if ( Settings::Esp::Visuals_Size < 0 )
	{
		Settings::Esp::Visuals_Size = 0;
	}
	else if ( Settings::Esp::Visuals_Size > 10 )
	{
		Settings::Esp::Visuals_Size = 10;
	}

	int Height = (int)pPlayer->vOriginScreen.y - (int)pPlayer->vHitboxHeadScreen.y;

	if ( Height < 18 )
		Height = 18;

	int Width = Height / 2;

	int x = (int)pPlayer->vHitboxHeadScreen.x - Width / 2;
	int y = (int)pPlayer->vHitboxHeadScreen.y;

	vLineOrigin = pPlayer->vHitboxHeadScreen;
	vLineOrigin.y += Height;

	Color EspPlayerColor = GetPlayerColor( pPlayer );
	Color EspVisibleColor = GetPlayerVisibleColor( pPlayer );

	for (BYTE PlayerIndex = 0; PlayerIndex < g_pPlayers->GetSize(); PlayerIndex++)

	if (Settings::Esp::Visuals_Style == 0)
	{
		
	}
	if (Settings::Esp::Visuals_Style == 1)
	{
			g_pRender->DrawBox(x, y, Width, Height, EspVisibleColor);
	}
	if (Settings::Esp::Visuals_Style == 2)
	{
			g_pRender->DrawCoalBox( x , y , Width , Height , EspVisibleColor );
	}
	if (Settings::Esp::Visuals_Style == 3)
	{
			g_pRender->DrawAlphaBox(x, y, Width, Height, EspVisibleColor);
	}
	if (Settings::Esp::Visuals_Style == 4)
	{

			g_pRender->DrawAlphaBox(x, y, Width, Height, EspVisibleColor);
			g_pRender->DrawBox(x, y, Width, Height, EspVisibleColor);
	}
	if (Settings::Esp::Visuals_Style == 5)
	{
			g_pRender->DrawAlphaBox(x, y, Width, Height, EspVisibleColor);
			g_pRender->DrawCoalBox(x, y, Width, Height, EspVisibleColor);
	}

	if ( Settings::Esp::Visuals_Line )
	{
		Color ColorsColorLine = Color(int(Settings::Colors::Visuals_Line_Color[0] * 255.f),
			int(Settings::Colors::Visuals_Line_Color[1] * 255.f),
			int(Settings::Colors::Visuals_Line_Color[2] * 255.f));

		if (Interfaces::Engine()->IsInGame())
		{
			g_pRender->DrawLine((int)vLineOrigin.x, (int)vLineOrigin.y, iScreenWidth / 2, iScreenHeight, ColorsColorLine);
		}
	}

	if ( Settings::Esp::Visuals_Name )
	{
		Color ColorsColorName = Color(int(Settings::Colors::Name_Color[0] * 255.f),
			int(Settings::Colors::Name_Color[1] * 255.f),
			int(Settings::Colors::Name_Color[2] * 255.f));

		g_pRender->Text( (int)vLineOrigin.x , (int)pPlayer->vHitboxHeadScreen.y - 13 , true , true , ColorsColorName, pPlayer->Name.c_str() );
	}

	int iHpAmY = 1;

	if ( Settings::Esp::Visuals_Health >= 1 )
	{
		Color Minus = Color::Red();

		if ( pPlayer->Team == TEAM_CT )
		{
			Minus = CT_HP_ColorM;
		}
		else if ( pPlayer->Team == TEAM_TT )
		{
			Minus = TT_HP_ColorM;
		}

		int iHealth = pPlayer->iHealth;

		if ( iHealth )
		{
			if ( Settings::Esp::Visuals_Health == 1 )
			{
				Color ColorsColorHealth = Color(int(Settings::Colors::Health_Color[0] * 255.f),
					int(Settings::Colors::Health_Color[1] * 255.f),
					int(Settings::Colors::Health_Color[2] * 255.f));

				g_pRender->Text( (int)vLineOrigin.x , (int)vLineOrigin.y + iHpAmY , true , true , ColorsColorHealth, to_string( iHealth ).c_str() );
				iHpAmY += 10;
			}
			else if ( Settings::Esp::Visuals_Health == 2 )
			{
				Color ColorsColorHealth = Color(int(Settings::Colors::Health_Color[0] * 255.f),
					int(Settings::Colors::Health_Color[1] * 255.f),
					int(Settings::Colors::Health_Color[2] * 255.f));


				g_pRender->DrawHorBar( x , (int)vLineOrigin.y + iHpAmY , Width , 5 , iHealth , ColorsColorHealth, Minus );
				iHpAmY += 6;
			}
			else if ( Settings::Esp::Visuals_Health == 3 )
			{
				Color ColorsColorHealth = Color(int(Settings::Colors::Health_Color[0] * 255.f),
					int(Settings::Colors::Health_Color[1] * 255.f),
					int(Settings::Colors::Health_Color[2] * 255.f));

				g_pRender->DrawVerBar( x - 6 , (int)pPlayer->vHitboxHeadScreen.y , 5 , Height , iHealth ,ColorsColorHealth , Minus );
			}
			else if (Settings::Esp::Visuals_Health >= 4)
			{
				float flBoxes = std::ceil(pPlayer->iHealth / 10.f);
				float flX = x - 6;
				float flY = y - 1;
				float flHeight = Height / 10.f;
				float flMultiplier = 12 / 360.f;
				flMultiplier *= flBoxes - 1;
				Color ColHealth = Color::FromHSB(flMultiplier, 1, 1);

				g_pRender->DrawBox(flX, flY, 4, Height + 2, Color(80, 80, 80, 125));
				g_pRender->DrawOutlineBox(flX, flY, 4, Height + 2, Color::Black());
				g_pRender->DrawBox(flX + 1, flY, 2, flHeight * flBoxes + 1, ColHealth);

				for (int i = 0; i < 10; i++)
				{
					g_pRender->DrawLine(flX, flY + i * flHeight, flX + 4, flY + i * flHeight, Color::Black());
				}
			}
		}
	}

	if ( Settings::Esp::Visuals_Armor >= 1 )
	{
		Color Minus = Color::Red();

		if ( pPlayer->Team == TEAM_CT )
		{
			Minus = CT_AR_ColorM;
		}
		else if ( pPlayer->Team == TEAM_TT )
		{
			Minus = TT_AR_ColorM;
		}

		int iArmor = pPlayer->iArmor;

		if ( iArmor )
		{
			if ( Settings::Esp::Visuals_Armor == 1 )
			{
				Color ColorsColorArmor = Color(int(Settings::Colors::Armor_Color[0] * 255.f),
					int(Settings::Colors::Armor_Color[1] * 255.f),
					int(Settings::Colors::Armor_Color[2] * 255.f));

				g_pRender->Text( (int)vLineOrigin.x , (int)vLineOrigin.y + iHpAmY , true , true ,
					ColorsColorArmor,
								 to_string( iArmor ).c_str() );
				iHpAmY += 10;
			}
			if ( Settings::Esp::Visuals_Armor == 2 )
			{
				Color ColorsColorArmor = Color(int(Settings::Colors::Armor_Color[0] * 255.f),
					int(Settings::Colors::Armor_Color[1] * 255.f),
					int(Settings::Colors::Armor_Color[2] * 255.f));

				g_pRender->DrawHorBar( x , (int)vLineOrigin.y + iHpAmY , Width , 5 , iArmor ,
					ColorsColorArmor, Minus );
				iHpAmY += 6;
			}
			else if (Settings::Esp::Visuals_Armor >= 3)
			{
				Color ColorsColorArmor = Color(int(Settings::Colors::Armor_Color[0] * 255.f),
					int(Settings::Colors::Armor_Color[1] * 255.f),
					int(Settings::Colors::Armor_Color[2] * 255.f));

				g_pRender->DrawVerBar(x + Width + 1, (int)pPlayer->vHitboxHeadScreen.y, 5, Height, iArmor,
					ColorsColorArmor, Minus);
			}
		}
	}

	if ( Settings::Esp::Visuals_Weapon && !pPlayer->WeaponName.empty() )
	{
		string WeaponStr = pPlayer->WeaponName;

		if ( Settings::Esp::Visuals_Ammo && pPlayer->iWAmmo )
		{
			WeaponStr += " [";
			WeaponStr += to_string( pPlayer->iWAmmo );
			WeaponStr += "]";
		}

		Color ColorsColorWeaponEnemy = Color(int(Settings::Colors::WeaponEnemy_Color[0] * 255.f),
			int(Settings::Colors::WeaponEnemy_Color[1] * 255.f),
			int(Settings::Colors::WeaponEnemy_Color[2] * 255.f));

		g_pRender->Text((int)vLineOrigin.x, (int)vLineOrigin.y + iHpAmY, true, true, ColorsColorWeaponEnemy, WeaponStr.c_str());
		iHpAmY += 10;
	}
}

void CEsp::DrawPlayerSkeleton( CPlayer* pPlayer )
{
	Color SkeletonColor = GetPlayerColor( pPlayer );

	for ( BYTE IndexArray = 0; IndexArray < 18; IndexArray++ )
	{
		Color Skelet = Color(int(Settings::Colors::Skelet[0] * 255.f),
			int(Settings::Colors::Skelet[1] * 255.f),
			int(Settings::Colors::Skelet[2] * 255.f));
		DrawHitBoxLine( pPlayer->vHitboxSkeletonArray[IndexArray] , Skelet );
	}
}

void CEsp::DrawHitBoxLine( Vector* vHitBoxArray , Color color )
{
	Vector vHitBoxOneScreen;
	Vector vHitBoxTwoScreen;

	if ( vHitBoxArray[0].IsZero() || !vHitBoxArray[0].IsValid() || vHitBoxArray[1].IsZero() || !vHitBoxArray[1].IsValid() )
		return;

	if ( WorldToScreen( vHitBoxArray[0] , vHitBoxOneScreen ) && WorldToScreen( vHitBoxArray[1] , vHitBoxTwoScreen ) )
	{
		g_pRender->DrawLine( (int)vHitBoxOneScreen.x , (int)vHitBoxOneScreen.y ,
			(int)vHitBoxTwoScreen.x , (int)vHitBoxTwoScreen.y , color );
	}
}