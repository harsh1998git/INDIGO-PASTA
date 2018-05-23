#pragma once

#include "../../Engine/Engine.h"
#define MAXBACKTRACKTICKS /*14*/ int(Settings::Aimbot::BackTrackTicks)
#define TICK_INTERVAL			(Interfaces::GlobalVars()->interval_per_tick)
#define TIME_TO_TICKS( dt )		( (int)( 1.0f + (float)(dt) / TICK_INTERVAL ) )

class CMisc
{
public:
//[swap_lines]
	void OnRender();
	void OnCreateMove( CUserCmd* pCmd );
	void OnDrawModelExecute();
	void OnPlaySound( const char* pszSoundName );
	void Backtrack(CUserCmd* pCmd);
	void OnOverrideView( CViewSetup* pSetup );
	void OnGetViewModelFOV( float& fov );
	vector<int> GetObservervators( int playerId );
	void OnRenderSpectatorList();
//[/swap_lines]
};
