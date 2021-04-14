//thx ApexBot for good prediction
#include "BulletPrediction.h"

//Aim Code
void SmoothAngle(Vector3 src, Vector3& dst, float factor) {
	Vector3 delta = dst - src;
	Math::ClampAngle(delta);
	dst = src + delta / factor;
}

CBasePlayer* FindTarget(int& AimEntIndex, int& BestHitBox)
{
	//set global data
	LPlayer* LP_Ent = LP; 
	CBasePlayer* FOV_Target = nullptr;
	float f_Best = AimBot::FOV, d_Best = AimBot::Distance;
	if (!LP_Ent->Alive()) { AimEntIndex = -1;return nullptr; }
	
	//aim cache
	if (AimEntIndex != -1)
	{
		//check player
		CBasePlayer* CurEnt = Ent(AimEntIndex);
		if (CurEnt->Type(Player))
		{
			//check no knocked & on screen & visible
			auto HBoxPos = CurEnt->HitBoxPos(BestHitBox, false); 
			Vector2 ScreenPos;
			if (!CurEnt->Knocked() && World2Screen(HBoxPos, ScreenPos)) {// && CurEnt->VisiblePos(LP_Ent, HBoxPos)
				if (Math::GetFov(LP->DynamicAngles(), Math::CalcAngle(LP->CameraPos(), HBoxPos)) < f_Best)
					return CurEnt;
			}
		}
	}

	//set invalid cache
	AimEntIndex = -1;

	//new find target
	for (int i = 0; i < 15000; i++)
	{
		//get entity & check is player
		CBasePlayer* CurEnt = Ent(i);
		if (!CurEnt->Alive())continue;
		//hp(&CurEnt);
		if (CurEnt->Type(Player)|| CurEnt->Type(NPC))//
		{
			//skip knocked
			if (AimBot::NoKnocked && CurEnt->Knocked())
				continue;

			//cut distance
			Vector3 BodyBonePos = CurEnt->HitBoxPos(3, false);
			float CurDist = Math::GameDist(BodyBonePos, LP->CameraPos());
			if (CurDist > AimBot::Distance) continue;

			//check head
			Vector3 HeadBonePos = CurEnt->HitBoxPos(0, false); 
			Vector2 ScreenPos;
			if (World2Screen(HeadBonePos, ScreenPos))// && CurEnt->VisiblePos(LP_Ent, HeadBonePos)//瞄准头部
			{
				//calc fov
				float CurFOV = Math::GetFov(LP->DynamicAngles(),
					Math::CalcAngle(LP->CameraPos(), HeadBonePos));
				//set fov target
				if (CurFOV < f_Best) {
					//sp("CurFOV < f_Best");
					FOV_Target = CurEnt;
					f_Best = CurFOV;
					AimEntIndex = i;
					//dp(AimEntIndex);
					BestHitBox = 0;
				}
			}

			////check body
			//if (AimBot::Nearest && World2Screen(BodyBonePos, ScreenPos))//  && CurEnt->VisiblePos(LP_Ent, BodyBonePos)//瞄准身体
			//{
			//	//calc fov
			//	float CurFOV = Math::GetFov(LP->DynamicAngles(),
			//		Math::CalcAngle(LP->CameraPos(), BodyBonePos));

			//	//set fov target
			//	if (CurFOV < f_Best) {
			//		FOV_Target = CurEnt;
			//		f_Best = CurFOV;
			//		AimEntIndex = i;
			//		BestHitBox = 3;
			//	}
			//}
		}
	}
	//set best target
	return FOV_Target;
}
namespace guaji
{
	float yaw = 0;
	bool GUAJI;//挂机
	bool GUAJI1;//挂机
	bool GUAJI2;//挂机
};
__forceinline bool Aim(UserCmd* Cmd){
	static int AimHitBox, AimID = -1;
	/*if (Cmd->buttons & 1)sp("1");
	if (Cmd->buttons & 2)sp("2");
	if (Cmd->buttons & 3)sp("3");
	if (Cmd->buttons & 4)sp("4");
	if (Cmd->buttons & 5)sp("5");
	if (Cmd->buttons & 6)sp("6");
	if (Cmd->buttons & 7)sp("7");
	if (Cmd->buttons & 8)sp("8");
	if (Cmd->buttons & 9)sp("9");
	if (Cmd->buttons & 10)sp("10");*/
	if (AimBot::Active && GetAsyncKeyState(0x02) && (LP->Alive()))//((Cmd->buttons & 1) || (Cmd->buttons & 0x20000))||GetAsyncKeyState(0x2)
	{
		//get weapon speed & gravity
		float BulletSpeed = LP->ActiveWeapon()->BulletSpeed(); //bullet speed (units)
		float BulletGrav = LP->ActiveWeapon()->BulletGravity(); //(gravity scale * 750.f)
		//is weapon check
		if ((BulletSpeed > 1.f) || (BulletGrav > 1.f))
		{
			//find target
			Vector3 AimAngles;
			CBasePlayer* AimTarget;
			if ((AimTarget = FindTarget(AimID, AimHitBox)))
			{
				//get target & muzzle positions
				Vector3 MuzzlePos = LP->CameraPos();//->ActiveWeapon()->MuzzlePos();
				Vector3 TargetPos = AimTarget->HitBoxPos(AimHitBox, true);
				
				//check need prediction
				if (BulletSpeed > 1.f)
				{
					//simple prediction
					if (AimBot::PredictType)
					{
						//compensate bullet drop 
						float VerticalTime = Math::Dist3D(TargetPos, MuzzlePos) / BulletSpeed;
						TargetPos.z += (750.f * LP->ActiveWeapon()->BulletGravity() * 0.5f) * (VerticalTime * VerticalTime);

						//compensate velocity
						float HorizontalTime = Math::Dist3D(TargetPos, MuzzlePos) / BulletSpeed;
						TargetPos += (AimTarget->AbsVelocity(true) * HorizontalTime);

						//calculate angle
						AimAngles = Math::CalcAngle(MuzzlePos, TargetPos);
					}

					else {
						//set pred ctx
						PredictCtx Ctx; 
						Ctx.StartPos = MuzzlePos;
						Ctx.TargetPos = TargetPos; 
						Ctx.BulletSpeed = BulletSpeed;
						Ctx.BulletGravity = (BulletGrav * 750.f);
						Ctx.TargetVel = AimTarget->AbsVelocity();

						//target not hittable?!
						if (!BulletPredict(Ctx)) { AimID = -1; return false; }
						AimAngles = Vector3{ Ctx.AimAngles.x, Ctx.AimAngles.y, 0.f };
					}
				}

				else {
					//not needed bullet prediction
					AimAngles = Math::CalcAngle(MuzzlePos, TargetPos);
				}

				//remove sway & recoil
				if (AimBot::WeaponFix) {
					AimAngles -= LP->PunchAngle();
					//fp(LP->PunchAngle().y);
					AimAngles -= ((LP->DynamicAngles() - LP->PunchAngle()) - Cmd->viewangles);
				}

				//apply angle & set aim type
				Math::ClampAngle(AimAngles);
				SmoothAngle(Cmd->viewangles, AimAngles, AimBot::Smooth);
				Cmd->viewangles = AimAngles; //用CMD写入角度
				return true;
			}
		}
	}
	
	if (GetAsyncKeyState(VK_F12)) guaji::GUAJI = !guaji::GUAJI;
	if (guaji::GUAJI) {
		Cmd->forwardmove = 5.f;

		guaji::yaw += 5;
		if (guaji::yaw == 360)guaji::yaw = 0;
		Cmd->viewangles = { Cmd->viewangles.x, Cmd->viewangles.y, guaji::yaw };
	}
	if (GetAsyncKeyState(VK_F11)) guaji::GUAJI1 = !guaji::GUAJI1;
	if (guaji::GUAJI1) {
		Cmd->forwardmove = 5.f;

		guaji::yaw += 5;
		if (guaji::yaw == 360)guaji::yaw = 0;
		Cmd->viewangles = { Cmd->viewangles.x, guaji::yaw, Cmd->viewangles.z };
	}
	if (GetAsyncKeyState(VK_F10)) guaji::GUAJI2 = !guaji::GUAJI2;
	if (guaji::GUAJI2) {
		Cmd->forwardmove = 5.f;

		guaji::yaw += 5;
		if (guaji::yaw == 360)guaji::yaw = 0;
		Cmd->viewangles = { guaji::yaw, Cmd->viewangles.y, Cmd->viewangles.z };
	}
	if (GetAsyncKeyState(VK_F9))Cmd->viewangles = { Cmd->viewangles.x, Cmd->viewangles.y, 180.f };
	//invalid
	AimID = -1;
	return false;
}