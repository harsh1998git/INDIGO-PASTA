#pragma once

#include "../../Engine/Engine.h"

class CTimer;

namespace Engine
{
	class CMe;
	class CPlayer;
}

using namespace Engine;

enum AIM_ATTACK_TYPE
{
	AA_SHOTGUN,
	AA_SNIPER_PISTOL
};

class CAimbot
{
public:

	CAimbot();
	//[swap_lines]
	bool	IsEnable();

	int		GetPlayerFov(CPlayer* pPlayer);

	int		GetBestTarget();
	int		GetBestHitBox();


	void	OnRender();

	void	Aimbot(CUserCmd * pCmd, CMe * pLocal, bool &bSendPacket);
	void	AimbotSet(CUserCmd * pCmd, CMe * pLocal, bool &bSendPacket);
	void	normalize_angle(float& flAngle);

	// ����� �������
	void fake_up(float& angle);
	void fake_down(float& angle);
	void down(float& angle);
	void up(float& angle);

	void auto_revolver(CUserCmd* m_pcmd);
	// ����� �������

	void	AutoPistol();
	void	OnCreateMove(CUserCmd* pCmd, CMe* pLocal);
	//[/swap_lines]
private:

	CMe*		m_pLocal;
	CUserCmd*	m_pCmd;

	int		m_iBestPreTarget;

	int		m_iBestTarget;
	int		m_iBestHitbox;

	bool	m_bClamp;
	bool	m_bAttack;
	bool	m_bAimShot;
	bool	m_bTargetFov;
	bool	m_bTargetChange;
	bool	m_bAutoPistolEn;

	CTimer*	m_pShotDelay;

	Vector	m_vAimBestHitbox;
	Vector  m_vAimBestHitboxScreen;
};