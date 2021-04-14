#include "../Funcs/AimBot.h"

//Entity List
int EntityCount = 0;
struct Entity { 
	int EntID;
	bool Visible;
};
Entity EntityCache[1000];
void ProcessEntityCache(UserCmd* Cmd){
	//setup global data
	LPlayer* LP_Ent = LP; 
	//hp(LP_Ent);
	int CurCount = 80;
	if (!LP_Ent->Alive()) {
		//sp("!LP_Ent->Alive()");
		EntityCount = CurCount; return;
	} 
	Vector3 CameraPos = LP_Ent->CameraPos();
	//process entities
	for (int i = 0; i < 80; i++){
		auto CurEnt = Ent(i);
		if (CurEnt->Type(Player)){	
			//sp("Player");
			//clamp distance
			Vector3 Head3D = CurEnt->HitBoxPos(0, false);
			if ((Math::GameDist(CameraPos, Head3D)) > Visuals::DistanceESP)
				goto InvalidEnt;

			//save entity
			EntityCache[i].EntID = i;
			EntityCache[i].Visible = 0;// CurEnt->VisiblePos(LP_Ent, Head3D);
		} 
		else { //skip
			InvalidEnt: 
			EntityCache[i].EntID = -1;
		}
	}

	//wait update tick
	static BYTE Tick = 0;
	if (Visuals::LootESP && !Tick++){
		//sp("LootESP");
		//process loot
		for (int i = 0; i < 15000; i++){
			auto CurEnt = Ent(i);
			if (CurEnt->Type(NPC)) {
				//clamp distance
				Vector3 Head3D = CurEnt->HitBoxPos(0, false);

				//save entity
				EntityCache[CurCount].EntID = i;
				EntityCache[CurCount].Visible = 1;// CurEnt->VisiblePos(LP_Ent, Head3D);
				CurCount++;
			}
			if (CurEnt->Type(LootEnt))
			{
				//clamp distance
				Vector3 RootPos = CurEnt->AbsOrgin();
				if (Math::GameDist(CameraPos, RootPos) > Visuals::DistanceLoot)
					continue;

				//save entity
				if (CurCount == 999) break;
				EntityCache[CurCount].EntID = i; 
				EntityCache[CurCount].Visible = 1;
				CurCount++;
			}
		}
		//dp(CurCount);
		//update full count
		EntityCount = CurCount;
	}
}

//store original function
typedef void(__fastcall* CreateMoveFn)(DWORD64, int, float, bool);
CreateMoveFn CreateMoveOrg = nullptr;

//CreateMove Hook (indx 4)
void __fastcall CreateMoveHk(DWORD64 thisPtr, int SeqNum, float SampleTime, bool Active){
	//call original
	SpoofCall(CreateMoveOrg, thisPtr, SeqNum, SampleTime, Active);
	//get usercmd & process entities
	UserCmd* CurCmd = (UserCmd*)(*(DWORD64*)(thisPtr + 0xF8) + 0x218 * (SeqNum % 750));
	ProcessEntityCache(CurCmd);
	//run aim
	Aim(CurCmd);
	//dp(CurCmd->buttons & 1);
	//bhop (w only)
	if (Misc::Bhop && (CurCmd->buttons & 2)){
		//forward + duck
		CurCmd->buttons |= 4u;
		CurCmd->forwardmove = 1.f;
		//auto strafer
		if (Global::MouseDelta.x > 0.f)
			CurCmd->sidemove = 1.f;
		else if (Global::MouseDelta.x < 0.f)
			CurCmd->sidemove = -1.f;
		//-jump
		if (!(LP->Flags() & 1)) {
			CurCmd->buttons &= ~2u;
		}
	}

	//no spread
	/*if (AimBot::Active && AimBot::NoSpread) {
		DWORD64 Weapon = (DWORD64)LP->ActiveWeapon();
		if (Weapon) {
			*(float*)(Weapon + Off::NoSpreadX) = 0;
			*(float*)(Weapon + Off::NoSpreadY) = 0;
			*(float*)(Weapon + Off::NoSpreadZ) = 0;
		}
	}*/
	//pSlilent (test)
	//static bool ChokedTicks = false;
	//if (AimBot::Active && AimBot::Silent && (CurCmd->buttons & 1))
	//{
	//	//switch tick
	//	ChokedTicks = !ChokedTicks;

	//	if (ChokedTicks) {
	//		//shoot with break packet
	//		*reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(_AddressOfReturnAddress()) + 0x38) = 0;
	//	}

	//	else { //dont shoot w\o break packet
	//		CurCmd->buttons &= ~1;
	//	}
	//}

	//else //reset choke tick
	//	ChokedTicks = false;
	
	

	//static uint64_t uFindPos;
	//if (!uFindPos) uFindPos = (uint64_t)FindPattern("0F 2E C8 7A 02 74 26");
	//auto g_GameSpeedHack = (uFindPos + 7) + *(uint32_t*)(uFindPos + 11) + 8;	
	//static bool flag = false;
	//if (GetAsyncKeyState(VK_RBUTTON)) {//CurCmd->buttons & 1
	//	if (!flag) {
	//		*reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(_AddressOfReturnAddress()) + 0x38) = 0;
	//		*(float*)g_GameSpeedHack = 4.f; flag = true;
	//	}
	//	else
	//		*(float*)g_GameSpeedHack = 1.f;
	//}
	//else {
	//	flag = false;
	//}
	
	

	//bhop (w only)
	//constexpr int Val = 20;
	//static int tick_count = 0;
	//if (Misc::Bhop && (CurCmd->buttons & 2))
	//{
	//	CurCmd->forwardmove = 1.f;

	//	if (!(LP->Flags() & 1)) {
	//		CurCmd->buttons &= ~2u;
	//		tick_count = 0;
	//	}

	//	else if (tick_count < Val) {
	//		CurCmd->buttons |= 4u;
	//		CurCmd->buttons &= ~2u;
	//		tick_count++;
	//	}

	//	else if (tick_count >= Val) {
	//		tick_count = 0;
	//	}

	//	//if (tick_count == 0) {
	//	//	CurCmd->buttons &= ~2u; //-jump
	//	//	CurCmd->buttons |= 4u; //+duck
	//	//}

	//	//else if (tick_count == 5) {
	//	//	CurCmd->buttons |= 2u;
	//	//	CurCmd->buttons &= ~4u;
	//	//}

	//	//else if (tick_count > 5) {
	//	//	if (!(LP->Flags() & 1))
	//	//		CurCmd->buttons &= ~2u;

	//	//	else {
	//	//		tick_count = 0;
	//	//		goto EndTicks;
	//	//	}
	//	//}
	//	//
	//	//tick_count++;
	//	//EndTicks:;

	//	////forward + duck
	//	//CurCmd->buttons |= 4u;
	//	//CurCmd->forwardmove = 1.f;

	//	////auto strafer
	//	//if (Global::MouseDelta.x > 0.f)
	//	//	CurCmd->sidemove = 1.f;
	//	//else if (Global::MouseDelta.x < 0.f)
	//	//	CurCmd->sidemove = -1.f;

	//	////-jump
	//	//if (!(LP->Flags() & 1)) {
	//	//	CurCmd->buttons &= ~2u;
	//	//}
	//}

	//else
	//	tick_count = 0;

	//SlowMo (fixed)
	/*static int ChokedTicks = 0;
	if (Misc::SlowMo && FC(user32, GetAsyncKeyState, VK_MENU)) {
		if (ChokedTicks++ < 16) {
			*reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(_AddressOfReturnAddress()) + 0x38) = 0;
			if (ChokedTicks == 6) { CurCmd->command_number++; } return;
		}
	}

	ChokedTicks = 0;*/
}