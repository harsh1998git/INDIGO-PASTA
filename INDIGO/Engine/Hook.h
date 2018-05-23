#pragma once

#include "Engine.h"

namespace Engine
{
	class CPlayers;
	namespace Hook
	{
		bool Initialize();
		void Shutdown();
	}
}

struct backtrackData {
	float simtime;
	Vector hitboxPos;
};

extern backtrackData headPositions[64][16];