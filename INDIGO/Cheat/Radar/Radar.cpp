#include "Radar.h"

using namespace Client;
//[junk_enable /]
void CRadar::RadarRange( float* x , float* y , float range )
{
	if ( fabs( ( *x ) ) > range || fabs( ( *y ) ) > range )
	{
		if ( ( *y ) > ( *x ) )
		{
			if ( ( *y ) > -( *x ) )
			{
				( *x ) = range*( *x ) / ( *y );
				( *y ) = range;
			}
			else
			{
				( *y ) = -range*( *y ) / ( *x );
				( *x ) = -range;
			}
		}
		else
		{
			if ( ( *y ) > -( *x ) )
			{
				( *y ) = range*( *y ) / ( *x );
				( *x ) = range;
			}
			else
			{
				( *x ) = -range*( *x ) / ( *y );
				( *y ) = -range;
			}
		}
	}
}

void CRadar::CalcRadarPoint( Vector vOrigin , int& screenx , int& screeny )
{
	Vector vAngle;

	Interfaces::Engine()->GetViewAngles( vAngle );

	float dx = vOrigin.x - g_pPlayers->GetLocal()->vEyeOrigin.x;
	float dy = vOrigin.y - g_pPlayers->GetLocal()->vEyeOrigin.y;

	float fYaw = float( vAngle.y * ( M_PI / 180.0 ) );

	float fsin_yaw = sin( fYaw );
	float fminus_cos_yaw = -cos( fYaw );

	float x = dy*fminus_cos_yaw + dx*fsin_yaw;
	float y = dx*fminus_cos_yaw - dy*fsin_yaw;

	float range = (float)Settings::Radar::Radar_Range;

	RadarRange( &x , &y , range );

	ImVec2 DrawPos = ImGui::GetCursorScreenPos();
	ImVec2 DrawSize = ImGui::GetContentRegionAvail();

	int Radar_x = (int)DrawPos.x;
	int Radar_y = (int)DrawPos.y;

	float r_siz_x = DrawSize.x;
	float r_siz_y = DrawSize.y;

	int x_max = (int)r_siz_x + Radar_x - 5;
	int y_max = (int)r_siz_y + Radar_y - 5;

	screenx = Radar_x + ( (int)r_siz_x / 2 + int( x / range * r_siz_x ) );
	screeny = Radar_y + ( (int)r_siz_y / 2 + int( y / range * r_siz_y ) );

	if ( screenx > x_max )
		screenx = x_max;
	
	if ( screenx < Radar_x )
		screenx = Radar_x;
	
	if ( screeny > y_max )
		screeny = y_max;
	
	if ( screeny < Radar_y )
		screeny = Radar_y;
}

static void SquareConstraint(ImGuiSizeConstraintCallbackData *data)
{
	data->DesiredSize = ImVec2(max(data->DesiredSize.x, data->DesiredSize.y), max(data->DesiredSize.x, data->DesiredSize.y));
}

void CRadar::OnRenderPlayer()
{
	float Alpha = (float)Settings::Radar::Radar_Alpha / 255.f;

	for ( BYTE PlayerIndex = 0; PlayerIndex < g_pPlayers->GetSize(); PlayerIndex++ )
	{
		CPlayer* pPlayer = g_pPlayers->GetPlayer( PlayerIndex );

		if ( pPlayer && pPlayer->m_pEntity && pPlayer->bUpdate )
		{
			if ( !Settings::Radar::Radar_Active )
				continue;

			if ( !Settings::Radar::Radar_Team && pPlayer->Team == g_pPlayers->GetLocal()->Team )
				continue;

			if ( !Settings::Radar::Radar_Enemy && pPlayer->Team != g_pPlayers->GetLocal()->Team )
				continue;

			int screenx = 0;
			int screeny = 0;

			float Color_R = 0.f;
			float Color_G = 0.f;
			float Color_B = 0.f;

			if ( pPlayer->Team == TEAM_CT )
			{
				Color_R = Settings::Radar::Radar_Color_CT[0];
				Color_G = Settings::Radar::Radar_Color_CT[1];
				Color_B = Settings::Radar::Radar_Color_CT[2];

				if ( pPlayer->bVisible )
				{
					Color_R = Settings::Radar::Radar_Color_VCT[0];
					Color_G = Settings::Radar::Radar_Color_VCT[1];
					Color_B = Settings::Radar::Radar_Color_VCT[2];
				}
			}
			else if ( pPlayer->Team == TEAM_TT )
			{
				Color_R = Settings::Radar::Radar_Color_TT[0];
				Color_G = Settings::Radar::Radar_Color_TT[1];
				Color_B = Settings::Radar::Radar_Color_TT[2];

				if ( pPlayer->bVisible )
				{
					Color_R = Settings::Radar::Radar_Color_VTT[0];
					Color_G = Settings::Radar::Radar_Color_VTT[1];
					Color_B = Settings::Radar::Radar_Color_VTT[2];
				}
			}

			CalcRadarPoint( pPlayer->vOrigin , screenx , screeny );

			ImDrawList* Draw = ImGui::GetWindowDrawList();

			Draw->AddRectFilled( ImVec2( (float)screenx , (float)screeny ) ,
								 ImVec2( (float)screenx + 5 , (float)screeny + 5 ) ,
								 ImColor( Color_R , Color_G , Color_B , Alpha ) );
		}
	}
}

void CRadar::OnRenderSounds()
{
	float Alpha = (float)Settings::Radar::Radar_Alpha / 255.f;

	for ( size_t i = 0; i < g_pEsp->SoundEsp.Sound.size(); i++ )
	{
		int screenx = 0;
		int screeny = 0;

		CalcRadarPoint( g_pEsp->SoundEsp.Sound[i].vOrigin , screenx , screeny );

		ImDrawList* Draw = ImGui::GetWindowDrawList();

		Draw->AddRectFilled( ImVec2( (float)screenx , (float)screeny ) ,
							 ImVec2( (float)screenx + 5 , (float)screeny + 5 ) ,
							 ImColor( 1.f , 1.f , 1.f , Alpha ) );
	}
}

void CRadar::OnRender()
{
	if (Settings::Radar::Radar_Enable)
	{
		if (Settings::Radar::Radar_Active && IsLocalAlive() || bIsGuiVisible)
		{
			//float prevAlpha = ImGui::GetStyle().Alpha;

			//float Alpha = (float)Settings::Radar::Radar_Alpha / 255.f;

			ImGuiStyle& style = ImGui::GetStyle();
			ImVec2 oldPadding = style.WindowPadding;
			float Alpha = style.Colors[ImGuiCol_WindowBg].w;
			float oldAlpha = style.Colors[ImGuiCol_WindowBg].w;
			style.Colors[ImGuiCol_WindowBg].w = (float)Settings::Radar::Radar_Alpha / 255.0f;

			//ImGui::GetStyle().Alpha = Alpha;

			ImGui::SetNextWindowSize(ImVec2(185, 185), ImGuiSetCond_FirstUseEver);
			ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, FLT_MAX), SquareConstraint);

			if (ImGui::Begin(RADAR_TEXT, &Settings::Radar::Radar_Active, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar))
			{
				ImDrawList* Draw = ImGui::GetWindowDrawList();

				ImVec2 DrawPos = ImGui::GetCursorScreenPos();
				ImVec2 DrawSize = ImGui::GetContentRegionAvail();

				ImVec2 winpos = ImGui::GetWindowPos();
				ImVec2 winsize = ImGui::GetWindowSize();

				Draw->AddLine(ImVec2(winpos.x + winsize.x * 0.5f, winpos.y), ImVec2(winpos.x + winsize.x * 0.5f, winpos.y + winsize.y), ImColor(255, 255, 255, 255), 1.f);
				Draw->AddLine(ImVec2(winpos.x, winpos.y + winsize.y * 0.5f), ImVec2(winpos.x + winsize.x, winpos.y + winsize.y * 0.5f), ImColor(255, 255, 255, 255), 1.f);

				Draw->AddLine(ImVec2(winpos.x + winsize.x * 0.5f, winpos.y + winsize.y * 0.5f), ImVec2(winpos.x, winpos.y), ImColor(255, 255, 255, 255), 1.f);
				Draw->AddLine(ImVec2(winpos.x + winsize.x * 0.5f, winpos.y + winsize.y * 0.5f), ImVec2(winpos.x + winsize.x, winpos.y), ImColor(255, 255, 255, 255), 1.f);

				/* Draw->AddLine(
				ImVec2(DrawPos.x + DrawSize.x / 2.f, DrawPos.y),
				ImVec2(DrawPos.x + DrawSize.x / 2.f, DrawPos.y + DrawSize.y),
				ImColor(1.f, 1.f, 1.f, Alpha));

				Draw->AddLine(
				ImVec2(DrawPos.x, DrawPos.y + DrawSize.y / 2.f),
				ImVec2(DrawPos.x + DrawSize.x, DrawPos.y + DrawSize.y / 2.f),
				ImColor(1.f, 1.f, 1.f, Alpha));		*/



				OnRenderPlayer();

				if (Settings::Radar::Radar_Sound)
					OnRenderSounds();

				ImGui::End();
			}
		}
	}
	else if ( !Settings::Radar::Radar_Active && Settings::Radar::Radar_InGame )
	{
		if ( Interfaces::Engine()->IsInGame() )
		{
			OnRenderPlayer();
		}
	}
}