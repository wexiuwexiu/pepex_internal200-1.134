//class UserCmd
//{
//public:
//	int command_number;
//	int tick_count;
//	float command_time;
//	Vector3 viewangles;
//	BYTE pad1[0xC];
//	float forwardmove;
//	float sidemove;
//	float upmove;
//	int buttons;
//	BYTE pad2[0x24C];
//};

class UserCmd//CInput::DecodeUserCmdFromBuffer
{
public:
	int           command_number;
	int           tick_count;
	float         command_time;
	Vector3       viewangles;
	BYTE          _pad0[0x14];
	float         forwardmove;
	float         sidemove;
	float         upmove;
	int           buttons;
	BYTE          impulse;
	BYTE          _pad1[492];
};
//update new user_cmd
//class UserCmd
//{
//public:
//	int command_number;//0x0
//	int tick_count;//0x4
//	float command_time;//0x8
//	Vector3 viewangles;//0xc
//	char pad[0xC];//0x18
//	float forwardmove; //clamp to [-1;1]//0x24
//	float sidemove; //clamp to [-1;1]//0x28
//	float upmove; //clamp to [-1;1]//0x2C
//	int buttons;//0x30
//	BYTE impulse;//0x34
//	BYTE _unk;//0x35
//	BYTE _unk2;//0x36
//	short mousedx;//0x37
//	BYTE _unk3;//0x39
//	short mousedy;//0x3A
//	char _pad0x5[5];//0x3C
//	boolean sync_viewangles;//0x41//if set 1 then localviewangles not sync
//	char _pad5[0xAE];//0x42
//	int CheckSum;//0xF0
//
//};
struct kbutton_t {
	int down[2];
	int state;
};

class Ray
{
public:
	BYTE RayRaw[0x68];
	__forceinline Ray(const Vector3& Start, const Vector3& End) {
		SpoofCall(Fn::RayInit, RayRaw, &Start, &End);
	}
};

struct surface_t
{
	const char*		name;				//0x0000
	int				surfaceProp;		//0x0008
	unsigned short	flags;				//0x000C
};

struct plane_t
{
	Vector3	surfaceNormal;	//0x0000
	float	dist;			//0x000C
};

//struct TraceData
//{
//	Vector3 startpos;
//	int unk0;
//	Vector3 endpos;
//	int unk01;
//	plane_t plane;
//	float fraction;
//	int contents;
//	unsigned short dispFlags;
//	bool allsolid;
//	bool startsolid;
//	int unk02;
//	float fractionLeftSolid;
//	int unk03;
//	surface_t surface;
//	int hitgroup;
//	short physicsBone;
//	unsigned short m_nWorldSurfaceIndex;
//	void* m_pEnt;
//	int hitbox;
//	char pad[0x114];
//};
struct TraceData {
	Vector3 start; //0x0000
	char pad_000C[4]; //0x000C
	Vector3 end; //0x0010
	char pad_001C[4]; //0x001C
	plane_t plane; //0x0020
	float fraction; //0x0030
	int32_t contents; //0x0034
	uint16_t disp_flags; //0x0038
	bool all_solid; //0x003A
	bool start_solid; //0x003B
	char pad_003C[4]; //0x003C
	float fraction_left_solid; //0x0040
	char pad_0044[4]; //0x0044
	surface_t surface; //0x0048
	int32_t hitgroup; //0x0050
	int16_t physics_bone; //0x0054
	uint16_t world_surface_index; //0x0056
	void* m_pEnt; //0x0058
	int32_t hitbox; //0x005C
	char pad_0060[288]; //0x0060
};
bool World2Screen(const Vector3& Position, Vector2& ScreenPosition)
{
	DWORD64 ViewRender = *(DWORD64*)Off::ViewRender; 
	if (!ViewRender) return false;

	Matrix4x4* m_Matrix = *(Matrix4x4**)(ViewRender + Off::ViewMatrix); 
	if (!m_Matrix) return false;

	float w = 0.f;
	ScreenPosition.x = (m_Matrix->m[0][0] * Position.x) + (m_Matrix->m[0][1] * Position.y) + (m_Matrix->m[0][2] * Position.z + m_Matrix->m[0][3]);
	ScreenPosition.y = (m_Matrix->m[1][0] * Position.x) + (m_Matrix->m[1][1] * Position.y) + (m_Matrix->m[1][2] * Position.z + m_Matrix->m[1][3]);
	w = (m_Matrix->m[3][0] * Position.x) + (m_Matrix->m[3][1] * Position.y) + (m_Matrix->m[3][2] * Position.z + m_Matrix->m[3][3]);

	if (w < 0.01f)
		return false;

	float invw = 1.f / w;
	ScreenPosition.x *= invw;
	ScreenPosition.y *= invw;

	float x = Global::WndSize.x / 2.f;
	float y = Global::WndSize.y / 2.f;
	x += .5f * ScreenPosition.x * Global::WndSize.x + .5f;
	y -= .5f * ScreenPosition.y * Global::WndSize.y + .5f;
	ScreenPosition.x = x; ScreenPosition.y = y; return true;
}