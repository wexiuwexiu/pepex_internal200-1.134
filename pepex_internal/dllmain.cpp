#include "Globals.h"

//game sdk
#include "GameSDK/NetvarMgr.h"
#include "GameSDK/GameSDK.h"

//cheat sdk
#include "CheatSDK/CheatSDK.h"
#include "CheatSDK/Hooks/GameMovement.h"
#include "CheatSDK/Hooks/CreateMove.h"
#include "CheatSDK/Hooks/DirectX.h"


BOOL DllMain(HMODULE ModBase, DWORD CallReason, PVOID Reserv)
{
	//VIRTUALIZER_MUTATE_ONLY_START
	if (CallReason == DLL_PROCESS_ATTACH)
	{
		Beep(1200,500);
		//base init
		FC::SetSpoofStub(FindPattern(E("FF 27")));
		DWORD64 ModBase = (DWORD64)GetModuleBase(nullptr);
		sp("ModBase:"); hp(ModBase);
		//get game funcs
		Fn::UpdateAbsOrigin = (PVOID)RVA(FindPattern(E("E8 ? ? ? ? F3 0F 10 76 ? F3 0F 5C")), 5);
		//Fn::MuzzlePos = (PVOID)FindPattern(E("40 53 48 83 EC 30 48 8B DA 8B 91"));
		Fn::RayInit = (PVOID)FindPattern(E("F3 41 0F 10 08 F3 0F 5C 0A F3 0F 11"));
		Fn::GetEntName = (PVOID)FindPattern(E("40 53 48 83 EC 20 48 8B D9 E8 ? ? ? ? 48 63 43 38"));
		Fn::UpdateAbsVelocity = (PVOID)FindPattern(E("40 57 48 83 EC 50 F7"));

		//get global vars
		Off::EntityList = RVA(FindPattern(E("48 8D 05 ? ? ? ? 44 2B")), 7);
		Off::LocalPlayer = RVA(FindPattern(E("48 8B 05 ? ? ? ? 48 85 C0 74 07 44 88 B8 ? ? ? ?")), 7);	
		sp("EntityList:");
		hp((DWORD64)Off::EntityList);
		sp("LocalPlayer:"); 
		hp((DWORD64)Off::LocalPlayer);
		
		//get interfaces
		I::CHLClient = (PVOID)RVA(FindPattern(E("48 8D 0D ? ? ? ? 48 8B 05 ? ? ? ? 4C 8D 05 ? ? ? ?")), 7);
		I::SwapChain = *(IDXGISwapChain**)RVA(FindPattern(E("4C 8D 0D ? ? ? ? 2B 45")), 7);
		I::EngineTrace = *(PVOID*)(RVA(FindPattern(E("48 8D 0D 62 81 ? ? 48 89 0D ? ? ? ? 48 8D 0D ? ? ? ? 48 89 0D")), 7));
		PVOID CInputClass = (PVOID)RVA(FindPattern(E("4C 8B 05 ? ? ? ? 48 8D 0D ? ? ? ? 49 8B 80 ? ? ? ? 4C 3B C1 0F 85 ? ? ? ?")), 7);
		PVOID CGameMoveClass = (PVOID)RVA(FindPattern(E("48 8B 05 ? ? ? ? 48 8D 0D ? ? ? ? C6 05 ? ? ? ? ? 48 C7 ? ? ?")), 7);
		
		//get offsets  C7 05 01 21 0E 0A ? ? ? ? C7 05 FF 20 0E 0A ? ? ? ? 48 C7 05 24 21 0E
		//Off::PunchAngle = *(DWORD*)(FindPattern(E("C7 05 01 21 0E 0A ? ? ? ? C7 05 FF 20 0E 0A ? ? ? ? 48 C7 05 24 21 0E")) + 5);
		//Off::PunchAngle = (DWORD)0x2390;
		Off::StudioHdr = *(DWORD*)(FindPattern(E("48 83 B9 ? ? ? ? ? 48 8B D9 75 36")) + 3);
		Off::ViewRender = RVA(FindPattern(E("48 8B 0D ? ? ? ? 48 8D 15 ? ? ? ? 48 8B 01 48 3B C2 0F 85 ? ? ? ? 48 8B 81 ? ? ? ?")), 7);
		Off::AbsVelocity = GetOffset(E("DT_LocalPlayerExclusive"), E("m_vecAbsVelocity"));
		Off::ViewMatrix = *(DWORD*)(FindPattern(E("48 89 AB ? ? ? ? 4C 89 9B")) + 3);
		Off::ActiveWeapon = GetOffset(E("DT_Player"), E("m_latestPrimaryWeapons"));
		Off::BoneClass =  GetOffset(E("DT_BaseEntity"), E("m_nForceBone")) + 0x48;
		Off::ScriptInt = GetOffset(E("DT_PropSurvival"), E("m_customScriptInt"));
		Off::PropName = GetOffset(E("DT_BaseEntity"), E("m_iSignifierName"));
		Off::Bleedout = GetOffset(E("DT_Player"), E("m_bleedoutState"));
		Off::LifeState = GetOffset(E("DT_Player"), E("m_lifeState"));
		Off::TeamNum = GetOffset(E("DT_Player"), E("m_iTeamNum"));
		Off::Health = GetOffset(E("DT_Player"), E("m_iHealth"));
		Off::Flags = GetOffset(E("DT_Player"), E("m_fFlags"));
		
				
		//sp("CHLClient Offset:");hp((DWORD64)I::CHLClient - ModBase);
		//sp("EngineTrace Offset:"); hp((DWORD64)I::EngineTrace - ModBase);	
		//sp("CInputClass Offset:"); hp((DWORD64)CInputClass - ModBase);
		//sp("CGameMoveClass Offset:"); hp((DWORD64)CGameMoveClass - ModBase);
		sp("PunchAngle Offset:");
		hp((DWORD64)Off::PunchAngle);
		//sp("StudioHdr Offset:"); 
		//hp((DWORD64)Off::StudioHdr);
		//sp("ViewRender Offset:"); 
		//hp((DWORD64)Off::ViewRender);
		////sp("AbsVelocity Offset:"); 
		//hp(Off::AbsVelocity);
		////sp("ViewMatrix Offset:"); 
		//hp((DWORD64)Off::ViewMatrix);
		////sp("ActiveWeapon Offset:"); 
		//hp(Off::ActiveWeapon);
		//sp("BoneClass Offset:"); 
		//hp(Off::BoneClass);
		////sp("ScriptInt Offset:"); 
		//hp(Off::ScriptInt);
		////sp("PropName Offset:"); 
		//hp(Off::PropName);
		////sp("Bleedout Offset:"); 
		//hp(Off::Bleedout);
		////sp("LifeState Offset:"); 
		//hp(Off::LifeState);
		////sp("TeamNum Offset:"); 
		//hp(Off::TeamNum);
		////sp("Health Offset:"); 
		//hp(Off::Health);
		////sp("Flags Offset:"); 
		//hp(Off::Flags);
		//game movement hook
		VMT_Hook CGameMove(nullptr, CGameMoveClass);
		CGameMove.SetupHook(&SilentEndOrg, SilentEnd, 5);
		CGameMove.SetupHook(&SilentStartOrg, SilentStart, 4);

		//cinput hook
		VMT_Hook CCreateMove(nullptr, CInputClass);
		CCreateMove.SetupHook(&CreateMoveOrg, CreateMoveHk, 4);
		
		//present hook
		BYTE ClassOffset = *(BYTE*)(FindPattern(E("8B ? ? ? ? ? 48 8B 01 48 8D 55"), E("dxgi")) + 2);
		VMT_Hook D3D(E("d3d11"), *(DWORD64*)((DWORD64)I::SwapChain + ClassOffset));
		D3D.SetupHook(&PresentOrg, Present, 13);
	} 
	
	//inject ok
	//VIRTUALIZER_MUTATE_ONLY_END
	return true;
}