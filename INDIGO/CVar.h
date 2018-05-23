#pragma once

#include "SDK/SDK.h"

#define FORCEINLINE_CVAR

#pragma region CVAR Flags
#define FCVAR_NONE                      0
#define FCVAR_UNREGISTERED              (1<<0)
#define FCVAR_DEVELOPMENTONLY           (1<<1)
#define FCVAR_GAMEDLL                   (1<<2)
#define FCVAR_CLIENTDLL                 (1<<3)
#define FCVAR_HIDDEN                    (1<<4)

// ConVar only
#define FCVAR_PROTECTED                 (1<<5)
#define FCVAR_SPONLY                    (1<<6)
#define   FCVAR_ARCHIVE                 (1<<7)
#define   FCVAR_NOTIFY                  (1<<8)
#define   FCVAR_USERINFO                (1<<9)
#define FCVAR_CHEAT                     (1<<14)

#define FCVAR_PRINTABLEONLY             (1<<10)
#define FCVAR_UNLOGGED                  (1<<11)
#define FCVAR_NEVER_AS_STRING           (1<<12)


#define FCVAR_REPLICATED                (1<<13)
#define FCVAR_DEMO                      (1<<16)
#define FCVAR_DONTRECORD                (1<<17)

#define FCVAR_NOT_CONNECTED             (1<<22)

#define FCVAR_ARCHIVE_XBOX              (1<<24)

#define FCVAR_SERVER_CAN_EXECUTE        (1<<28)
#define FCVAR_SERVER_CANNOT_QUERY       (1<<29)
#define FCVAR_CLIENTCMD_CAN_EXECUTE     (1<<30)
#pragma endregion

namespace SDK
{
	class ConVar
	{
	public:

		char pad_0x0000[0x4]; //0x0000
		ConVar* pNext; //0x0004
		__int32 bRegistered; //0x0008
		char* pszName; //0x000C
		char* pszHelpString; //0x0010
		__int32 nFlags; //0x0014
		char pad_0x0018[0x4]; //0x0018
		ConVar* pParent; //0x001C
		char* pszDefaultValue; //0x0020
		char* strString; //0x0024
		__int32 StringLength; //0x0028
		float fValue; //0x002C
		__int32 nValue; //0x0030
		__int32 bHasMin; //0x0034
		float fMinVal; //0x0038
		__int32 bHasMax; //0x003C
		float fMaxVal; //0x0040
		void* fnChangeCallback; //0x0044


		void ConVar::SetValue(const char* value)
		{
			typedef void(__thiscall* OriginalFn)(void*, const char*);
			return GetMethod<OriginalFn>(this, 14)(this, value);
		}

		void ConVar::SetValue(float value)
		{
			typedef void(__thiscall* OriginalFn)(void*, float);
			return GetMethod<OriginalFn>(this, 15)(this, value);
		}

		void ConVar::SetValue(int value)
		{
			typedef void(__thiscall* OriginalFn)(void*, int);
			return GetMethod<OriginalFn>(this, 16)(this, value);
		}

		void ConVar::SetValue(Color value)
		{
			typedef void(__thiscall* OriginalFn)(void*, Color);
			return GetMethod<OriginalFn>(this, 17)(this, value);
		}

		char* ConVar::GetName()
		{
			typedef char*(__thiscall* OriginalFn)(void*);
			return GetMethod<OriginalFn>(this, 5)(this);
		}

		char* ConVar::GetDefault()
		{
			return pszDefaultValue;
		}

		void RegisterConCommand(ConVar* pCommandBase)
		{
			typedef ConVar*(__thiscall* FindVarFn)(void*, ConVar*);
			GetMethod< FindVarFn >(this, 12)(this, pCommandBase);
		}

		void UnRegisterConCommand(ConVar* pCommandBase)
		{
			typedef ConVar*(__thiscall* FindVarFn)(void*, ConVar*);
			GetMethod< FindVarFn >(this, 13)(this, pCommandBase);
		}

		ConVar* FindVar(const char *var_name)
		{
			typedef ConVar*(__thiscall* FindVarFn)(void*, const char*);
			return GetMethod< FindVarFn >(this, 15)(this, var_name);
		}
	};
}