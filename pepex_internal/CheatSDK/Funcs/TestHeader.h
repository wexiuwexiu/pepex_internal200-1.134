void GlowESP()
{
	float EnemyFOV = 10.f;
	CBasePlayer* AimEnt = nullptr;

	for (CEntityList* CurEntry = CurEntry->Start(); CurEntry; CurEntry = CurEntry->Next())
	{
		auto Entity = CurEntry->Ent;
		if (Entity->Type(Player))
		{
			//setup glow color
			Vector3 GlowColor = { 0.f, 100.f, 10.f };
			if (Entity->Knocked()) 
				GlowColor = { 130.f, 130.f, 130.f };
			Entity->Glow(GlowColor);
			continue;

			//check dist
			Vector3 CameraPos = LP->CameraPos();
			Vector3 Bone3D = Entity->HitBoxPos(3, false);
			if (Math::GameDist(CameraPos, Bone3D) > 200.f)
				continue;

			//check in fov
			float CurFOV = Math::GetFov(LP->DynamicAngles(), Math::CalcAngle(CameraPos, Bone3D));
			if (CurFOV < EnemyFOV) { EnemyFOV = CurFOV; AimEnt = Entity; }
		}
	}
	return;

	//execute aim
	kbutton_t* inAttack = (kbutton_t*)(GetModuleBase(nullptr) + 0x26df0620);
	if (AimEnt && (inAttack->state == 5))
	{
		//get weapon speed & gravity
		float BulletSpeed = LP->ActiveWeapon()->BulletSpeed(); //bullet speed (units)
		float BulletGrav = LP->ActiveWeapon()->BulletGravity(); //(gravity scale * 750.f)

		if ((BulletSpeed > 1.f) || (BulletGrav > 1.f))
		{
			//get target & muzzle positions
			Vector3 MuzzlePos = LP->CameraPos();//->ActiveWeapon()->MuzzlePos();
			Vector3 TargetPos = AimEnt->HitBoxPos(3, false);

			//check need prediction
			Vector3 AimAngles;
			if (BulletSpeed > 1.f)
			{
				//compensate bullet drop 
				float VerticalTime = Math::Dist3D(TargetPos, MuzzlePos) / BulletSpeed;
				TargetPos.z += (750.f * LP->ActiveWeapon()->BulletGravity() * 0.5f) * (VerticalTime * VerticalTime);

				//compensate velocity
				float HorizontalTime = Math::Dist3D(TargetPos, MuzzlePos) / BulletSpeed;
				TargetPos += (AimEnt->AbsVelocity() * HorizontalTime);

				//calculate angle
				AimAngles = Math::CalcAngle(MuzzlePos, TargetPos);
			}

			else {
				//not needed bullet prediction
				AimAngles = Math::CalcAngle(MuzzlePos, TargetPos);
			}

			//remove sway & recoil
			if (AimBot::WeaponFix) {
				AimAngles -= LP->PunchAngle();
				AimAngles -= ((LP->DynamicAngles() - LP->PunchAngle()) - LP->StaticAngles());
			}

			//apply angle & set aim type
			Math::ClampAngle(AimAngles);
			SmoothAngle(LP->StaticAngles(), AimAngles, 20.f);
			Math::ClampAngle(AimAngles);
			LP->StaticAngles(AimAngles);
		}
	}
}