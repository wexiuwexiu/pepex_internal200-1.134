#define Ent(Id) (*(CBasePlayer**)(Off::EntityList + (Id << 5)))
#define LP (*(LPlayer**)Off::LocalPlayer)

enum EntType {
	Player,
	LootEnt,
	NPC
};
struct Fade {
	int a, b;
	float c, d, e, f;
};
struct GColor {
	float r, g, b;
};
struct GlowMode {
	BYTE GeneralGlowMode, BorderGlowMode, BorderSize, TransparentLevel;
};
class CBasePlayer
{
private:
	//get bone by hitbox
	int BoneByHitBox(int HitBox)
	{
		//get model ptr
		DWORD64 Model = *(DWORD64*)(this + Off::StudioHdr);
		if (!Model) return -1;

		//get studio hdr
		DWORD64 StudioHdr = *(DWORD64*)(Model + 8);
		if (!StudioHdr) return -1;

		//get hitbox array
		DWORD64 HitBoxsArray = StudioHdr + *(int*)(StudioHdr + 0xB4);
		if (!HitBoxsArray) return -1;

		//get bone index
		int Bone =  *(int*)(HitBoxsArray + *(int*)(HitBoxsArray + 8) + (HitBox * 0x2C));
		return ((Bone < 0) || (Bone > 255)) ? -1 : Bone;
	}

public:
	//get current health
	int Health() {
		if (!this) return 0;
		return *(int*)(this + Off::Health);
	}

	//get ent state
	DWORD Flags() {
		if (!this) return 0;
		return *(DWORD*)(this + Off::Flags);
	}

	//is knocked out?
	bool Knocked() {
		if (!this) return true;
		return (*(int*)(this + Off::Bleedout) != 0);
	}
	
	//get feet position
	Vector3 AbsOrgin(bool Update = false) {
		if (!this) return Vector3{ 0.f, 0.f, 0.f };
		if (Update) SpoofCall(Fn::UpdateAbsOrigin, this);
		return *(Vector3*)(this + Off::AbsVecOrgin);
	}

	//get player speed
	Vector3 AbsVelocity(bool Update = false) {
		if (!this) return Vector3();
		typedef void(__fastcall* GetAbsFn)(PVOID);
		if (Update) SpoofCall(Fn::UpdateAbsVelocity, this);
		return *(Vector3*)(this + Off::AbsVelocity);
	}

	//get script id
	int ScriptID() {
		if (!this) return -1;
		return *(int*)(this + Off::ScriptInt);
	}

	//get player name
	const char* PlayerName() {
		if (!this) return nullptr;
		return SpoofCall<const char*>(Fn::GetEntName, this);
	}

	const char* SignifierName() {
		DWORD64 PropHash = *(DWORD64*)(this + Off::PropName);
		return *(char**)PropHash;
	}

	//get camera position
	Vector3 CameraPos() {
		if (!this) return Vector3();
		return *(Vector3*)(this + Off::CameraPos);
	}

	//get alive state
	bool Alive() {
		if (!this) return false;
		return bool(*(int*)(this + Off::LifeState) == 0);
	}
	//check entity type
	bool Type(EntType Type)
	{
		//basic check's
		CBasePlayer* LPlayer = *(CBasePlayer**)Off::LocalPlayer;
		if (!this || !LPlayer || (this == LPlayer)) return false;

		//get prop hash
		DWORD64 PropHash = *(DWORD64*)(this + Off::PropName); 
		if (!PropHash) return false; PropHash = *(DWORD64*)PropHash;

		//check is loot
		if (Type == LootEnt)
			return (PropHash == 0x7275735F706F7270);

		else if (Type == NPC) {
			const char* PropName = *(const char**)(this + Off::PropName);
			return FC::StrCmp(E("npc_dummie"), PropName, true);
		}

		//check if player
		else if ((PropHash == 0x726579616C70) && Alive()) {
			return (*(int*)(this + Off::TeamNum) != *(int*)((DWORD64)LPlayer + Off::TeamNum));
		}

		//ent invalid
		return false;
	}

	wchar_t* modulename() {

	}
	void ItemGlow() {
		*(int*)(this + 0x02c0) = 1379961481;
	}

#define GLOW_CONTEXT		0x3C8 //Script_Highlight_SetCurrentContext
#define GLOW_LIFE_TIME		0x3A4 //Script_Highlight_SetLifeTime + 4
#define GLOW_DISTANCE		0x3B4 //Script_Highlight_SetFarFadeDist
#define GLOW_TYPE			0x2C4 //Script_Highlight_GetState + 4
#define GLOW_COLOR			0x1D0 //Script_CopyHighlightState 15th mov
#define GLOW_VISIBLE_TYPE	0x3D0 //Script_Highlight_SetVisibilityType 5th mov
#define GLOW_FADE			0x388 //Script_Highlight_GetCurrentInsideOpacity 

	void GlowCRZ(const Vector3& Color) {
		int await1 = 0;
		await1++;
		if (await1 =30) {
			await1 = 0;
			float currentEntityTime = 5000.f;//(float)target->buffer[0xEE4];
			GlowMode mode = { 101,102,96,90 };
			*(GlowMode*)(this + GLOW_TYPE) = mode;
			*(Vector3*)(this + GLOW_COLOR) = Color;

			*(float*)(this + GLOW_DISTANCE) = 40000.f;
			*(float*)(this + GLOW_LIFE_TIME) = currentEntityTime;
			currentEntityTime -= 1.f;

			*(int*)(this + GLOW_CONTEXT) = 1;
			*(int*)(this + GLOW_VISIBLE_TYPE) = 1;
			*(Fade*)(this + GLOW_FADE) = { 872415232, 872415232, currentEntityTime, currentEntityTime, currentEntityTime, currentEntityTime };
		}		
	}

	//glow esp
	void Glow(const Vector3& Color)
	{
		//Highlight_SetCurrentContext
		*(int*)(this + 0x3C8) = 1;

		//Highlight_ShowOutline
		*(bool*)(this + 0x3E0) = true;

		//Highlight_GetFarFadeDist
		*(float*)(this + 0x3B4) = FLT_MAX;

		//set color
		*(Vector3*)(this + 0x1D0) = Color;

		//Highlight_ShowInside
		*(float*)(this + 0x398) = FLT_MAX;
		*(float*)(this + 0x390) = FLT_MAX;
		*(float*)(this + 0x388) = FLT_MAX;

		//Highlight_ShowOutline
		*(float*)(this + 0x39c) = FLT_MAX;
		*(float*)(this + 0x394) = FLT_MAX;
		*(float*)(this + 0x38c) = FLT_MAX;

		//Highlight_SetLifeTime
		*(float*)(this + 0x3A4) = FLT_MAX;
	}

	//get bone position
	Vector3 HitBoxPos(int HitBox, bool UpdateBones)
	{
		//get bones base data
		DWORD64 Bones = *(DWORD64*)(this + Off::BoneClass); 
		if (!Bones) return Vector3(); 
		Vector3 BoneOff;
		int Bone = BoneByHitBox(HitBox);
		//sp("Bone"); dp(Bone);
		if (Bone == -1) return Vector3();

		//use cached bones
		if (!UpdateBones) {
			Matrix3x4* BoneMatrix = (Matrix3x4*)(Bones + (Bone * sizeof(Matrix3x4)));
			BoneOff = { BoneMatrix->_14, BoneMatrix->_24, BoneMatrix->_34 };
		}

		else 
		{ 
			//get updated bone matrix
			static Matrix3x4 MatrixArray[256];
			if (!VCall<bool>((PVOID)(this + 0x10), 16, MatrixArray, 256, 0xFFFF00, 0.f)) 
				return Vector3();

			//get bone by hitbox
			BoneOff = { MatrixArray[Bone]._14, MatrixArray[Bone]._24, MatrixArray[Bone]._34 };
		}
		
		//build bone pos
		return AbsOrgin(UpdateBones) + BoneOff;
	}

	//check visible bone
	bool VisiblePos(CBasePlayer* LPlayer, Vector3& Pos) {
		if (!this || !LPlayer) return false;
		Ray RayData(LPlayer->CameraPos(), Pos); 
		TraceData TraceOut;
		VCall(I::EngineTrace, 3, RayData.RayRaw, 0x640400B, nullptr, &TraceOut);
		return (TraceOut.m_pEnt == this || TraceOut.fraction > 0.97f);
	}
};

class Weapon 
{
public:
	float BulletSpeed() {
		if (!this) return 0.f;
		return *(float*)(this + Off::BulletSpeed);
	}

	float BulletGravity() {
		if (!this) return 0.f;
		return *(float*)(this + Off::BulletGravity);
	}

	/*Vector3 MuzzlePos() {
		Vector3 Pos; if (!this) return Pos;
		SpoofCall(Fn::MuzzlePos, this, &Pos);
		return Pos;
	}*/
};

class LPlayer : public CBasePlayer
{
public:
	Weapon* ActiveWeapon() {
		if (!this) return nullptr;
		DWORD64 ActWeaponId = *(DWORD64*)(this + Off::ActiveWeapon) & 0xFFFF;
		return (ActWeaponId ? (Weapon*)Ent(ActWeaponId) : nullptr);
	}

	Vector3 PunchAngle() {
		if (!this) return Vector3{};
		return *(Vector3*)(this + Off::PunchAngle);
	}

	__forceinline Vector3 StaticAngles() {
		if (!this) return Vector3{};
		return *(Vector3*)(this + Off::CameraAng);
	}

	__forceinline void StaticAngles(const Vector3& Ang) {
		if (!this) return;
		*(Vector3*)(this + Off::CameraAng) = Ang;
	}

	Vector3 DynamicAngles() {
		if (!this) return Vector3{};
		return *(Vector3*)(this + Off::DynamicAng);
	}
};

class CEntityList
{
public:
	CBasePlayer* Ent;
private:
	int SerialNumber;
	CEntityList* PrevEnt;
	CEntityList* NextEnt;

public:
	__forceinline CEntityList* Start() {
		CEntityList* EntInfo = *(CEntityList**)(Off::EntityList + 0x200000);
		if (EntInfo) { if (EntInfo->Ent) return EntInfo; else EntInfo->Next(); }
		return nullptr;
	}

	__forceinline CEntityList* Next()
	{
		CEntityList* NextEntry = NextEnt;

		GetValidLoop: 
		if (NextEntry) {
			if (NextEntry->Ent) 
				return NextEntry;

			else {
				NextEntry = NextEntry->NextEnt;
				goto GetValidLoop;
			}
		} 
		
		return nullptr;
	}
};