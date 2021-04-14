//vs project settings
#pragma warning(disable : 4530)
#pragma comment(linker,"/MERGE:.rdata=.text")
#pragma comment(linker,"/MERGE:.pdata=.text")
extern "C" int _fltused = 0;

//main includes
#include <windows.h>
#include <winternl.h>
#include <intrin.h>
#include <stdio.h>
#include <d3d11.h>
#include <cfloat>

//#include "CheatSDK/Utils/CV_SDK/VirtualizerSDK64.h"

//face sdk
#include "CheatSDK/Utils/EncStr.h"
#include "CheatSDK/Utils/Utils.h"
#include "CheatSDK/Utils/Math.h"

//global vars
namespace AimBot
{
	bool Active = true;
	bool Silent = false; //
	bool Nearest = true;
	bool WeaponFix = true;
	//bool NoSpread = true;
	bool NoKnocked = false;
	bool PredictType = false;

	float FOV = 15.f;
	float Smooth = 1.f;
	float Distance = 200.f;
}

namespace Visuals
{
	bool ESP = false;
	bool GlowESP = true;
	bool LootESP = true;
	bool DrawCrossHair = true;
	bool DrawFOV = false;
	bool DrawName = false;
	float DistanceESP = 300.f; 
	float DistanceLoot = 100.f;
	bool NicksOutLine = false;
	bool LootOutLine = false;
	bool Developer = false;
}

namespace Misc
{
	bool Bhop = false;
	bool SlowMo = true;
}

namespace Global {
	Vector2 WndSize;
	Vector3 LP_HeadPos;
	Vector2 MouseDelta;
}

//offsets
namespace Off
{
	//global vars
	DWORD64 ViewMatrix;
	DWORD64 ViewRender;
	DWORD64 EntityList;
	DWORD64 LocalPlayer;

	//dinamic
	DWORD Flags;
	DWORD Health;
	DWORD TeamNum;
	DWORD PropName;
	DWORD m_ModelName;
	DWORD Bleedout;
	DWORD LifeState;
	DWORD BoneClass;
	DWORD StudioHdr;
	DWORD ScriptInt;
	DWORD PunchAngle = 0x2390;
	DWORD AbsVelocity;
	DWORD ShieldHealth;
	DWORD ActiveWeapon;

	//"CameraPosition"
	DWORD CameraPos = 0x1E30;

	//"GetAbsOrigin"
	DWORD AbsVecOrgin = 0x14C;

	//"Speed(%f) is greater than sv_maxvelocity(%f)"
	DWORD BulletSpeed = 0x1e50;
	DWORD BulletGravity = BulletSpeed + 0x8;

	//"PlayerRunCommand_Prediction"
	DWORD CameraAng = 0x2478;
	DWORD DynamicAng = CameraAng - 0x10;

	//"GetSpread"
	DWORD NoSpreadX = 0x15C0;
	DWORD NoSpreadY = NoSpreadX - 0x10;
	DWORD NoSpreadZ = NoSpreadY + 0x4;
}

namespace Fn {
	PVOID RayInit;
	//PVOID MuzzlePos;
	PVOID GetEntName;
	//PVOID World2Screen;
	PVOID UpdateAbsOrigin;
	PVOID UpdateAbsVelocity;
}

namespace I {
	PVOID CHLClient;
	PVOID EngineTrace; 
	IDXGISwapChain* SwapChain;
}

typedef struct LDR_DATA_TABLE_ENTRY_FIX {
	PVOID Reserved1[2];
	LIST_ENTRY InMemoryOrderLinks;
	PVOID Reserved2[2];
	PVOID DllBase;
	PVOID Reserved3[1];
	ULONG64 SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
	PVOID Reserved5[2];
	#pragma warning(push)
	#pragma warning(disable: 4201)
	union {
		ULONG CheckSum;
		PVOID Reserved6;
	} DUMMYUNIONNAME;
	#pragma warning(pop)
	ULONG TimeDateStamp;
};