#include "../GuiRender/D3D_Experimental/D3D_Render.h"
#include "../GuiRender/Gui.h"

Vector2 MenuPos = { 50.f, 50.f };

__forceinline const wchar_t* LootName(int ScriptID, Render::Color& Color)
{
	switch (ScriptID)
	{
		//energy
		//case 12:  Color = Render::Color(255, 255, 0); return E(L"Havoc");
		case 7:  Color = Render::Color(255, 255, 0); return E(L"L-Star");
		case 127: Color = Render::Color(255, 255, 0); return E(L"ENERGY");

		//gold
		case 1:  Color = Render::Color(255, 236, 66); return E(L"Kraber");
		//case 2:  Color = Render::Color(255, 236, 66); return E(L"Mastiff");
		//case 18:  Color = Render::Color(255, 236, 66); return E(L"Devotion");

		//sniper
		//case 24:  Color = Render::Color(96, 102, 208); return E(L"Triple-Take");
		//case 116: Color = Render::Color(96, 102, 208); return E(L"Sentinel");
		case 66: Color = Render::Color(127, 130, 177); return E(L"LongBow");
		//case 21: Color = Render::Color(96, 102, 208); return E(L"Volt");
		//case 130: Color = Render::Color(96, 102, 208); return E(L"SNIPER");

		//heavy
		//case 29:  Color = Render::Color(0, 191, 255); return E(L"FlatLine");
		//case 34: Color = Render::Color(0, 191, 255); return E(L"Hemlok");
		//case 56: Color = Render::Color(0, 191, 255); return E(L"Prowler");
		//case 76: Color = Render::Color(0, 191, 255); return E(L"SpitFire");
		//case 98: Color = Render::Color(0, 191, 255); return E(L"WingMan");
		//case 129: Color = Render::Color(0, 191, 255); return E(L"HEAVY");

		//light
		//case 39: Color = Render::Color(255, 128, 0); return E(L"G7-Scout");
		//case 45: Color = Render::Color(255, 128, 0); return E(L"Alternator");
		//case 51: Color = Render::Color(255, 128, 0); return E(L"R-99");
		//case 81: Color = Render::Color(255, 128, 0); return E(L"R-301");
		//case 104: Color = Render::Color(255, 128, 0); return E(L"P2020");
		//case 110: Color = Render::Color(255, 128, 0); return E(L"RE-45");
		//case 126: Color = Render::Color(255, 128, 0); return E(L"LIGHT");

		//shotgun
		//case 86: Color = Render::Color(255, 0, 0); return E(L"EVA-8");
		//case 91: Color = Render::Color(255, 0, 0); return E(L"PeaceKeeper");
		//case 30: Color = Render::Color(255, 0, 0); return E(L"Zaman");
		//case 42: Color = Render::Color(255, 0, 0); return E(L"FRACTION");
		
		//misc
		//case 131: Color = Render::Color(0, 191, 255); return E(L"Accelerant");
		//case 132: Color = Render::Color(0.65f, 0.27f, 1.f, 1.f); return E(L"Phoenix");
		//case 133: Color = Render::Color(255, 10, 0); return E(L"MedKit");
		//case 134: Color = Render::Color(255, 10, 0); return E(L"Syringe");
		//case 135: Color = Render::Color(0, 191, 255); return E(L"SH-Battery");
		//case 136: Color = Render::Color(0, 191, 255); return E(L"SH-Cell");

		//helmet
		//case 137: Color = Render::Color(255, 255, 255); return E(L"Helmet-1");
		//case 138: Color = Render::Color(0, 191, 255); return E(L"Helmet-2");
		//case 139: Color = Render::Color(0.65f, 0.27f, 1.f, 1.f); return E(L"Helmet-3");
		//case 140: Color = Render::Color(255, 215, 0); return E(L"Helmet-4");

		//armor
		//case 146: Color = Render::Color(255, 255, 255); return E(L"EVO-1");
		//case 147: Color = Render::Color(0, 191, 255); return E(L"EVO-2");
		//case 148: Color = Render::Color(0.65f, 0.27f, 1.f, 1.f); return E(L"EVO-3");
		//case 149: Color = Render::Color(255, 215, 0); return E(L"EVO-4");

		//case 144: Color = Render::Color(255, 215, 0); return E(L"Armor-4");

		//case 154: Color = Render::Color(255, 255, 255); return E(L"BackPack-1");
		//case 155: Color = Render::Color(0, 191, 255); return E(L"BackPack-2");
		//case 156: Color = Render::Color(0.65f, 0.27f, 1.f, 1.f); return E(L"BackPack-3");
		//case 157: Color = Render::Color(255, 215, 0); return E(L"BackPack-4");

		case 202: Color = Render::Color(255, 215, 0); return E(L"HAMMPERPOINT");
		default: return nullptr;
	}
}

//store original function
PVOID PresentOrg = nullptr;
//Present Hook
void __fastcall Present(PVOID a1, PVOID a2){
	//VIRTUALIZER_MUTATE_ONLY_START
	if (Render::NewFrame(I::SwapChain, Global::WndSize)){
		Render::String({ 20, 20 }, E(L"Orion v2 Recode"));
		//misc visuals
		if (AimBot::Active && Visuals::DrawFOV)
		Render::Circle(Global::WndSize / 2.f, Render::Color(192, 192, 192), AimBot::FOV, 2.f);
		if (Visuals::DrawCrossHair)
			Render::FillCircle(Global::WndSize / 2.f, Render::Color(255, 255, 0), 2.f);
		//dp(EntityCount);
		for (int i = 0; i < EntityCount; i++)
		{
			if (EntityCache[i].EntID == -1) continue;
			CBasePlayer* CurEnt = Ent(EntityCache[i].EntID);

			if (Visuals::Developer) {
				Vector2 POS2D; Render::Color Color;
				Color = {255,20,20};
				if (World2Screen(CurEnt->AbsOrgin(), POS2D)) {
					/*const char* sigName = CurEnt->SignifierName();
					if (!sigName) continue; wchar_t Str[64];
					for (int i = 0;; i++) { if (!(Str[i] = sigName[i])) break; }
					Render::String(POS2D, Str, true, Color);*/
				}		
				continue;
			}
			//loot esp
			if (Visuals::LootESP && CurEnt->Type(LootEnt)) 
			{
				//sp("LootESP");
				Vector2 LootPos2D; Render::Color Color; 
				if (World2Screen(CurEnt->AbsOrgin(), LootPos2D)) {
			
					//wchar_t m_reportFileName[1024];
					//swprintf(m_reportFileName, L"%s %d", LootName(CurEnt->ScriptID(), Color), CurEnt->ScriptID());
					//Color = { 255, 255, 255 };
					//const wchar_t* LootStr = m_reportFileName;//LootName(CurEnt->ScriptID(), Color);

					const wchar_t* LootStr = LootName(CurEnt->ScriptID(), Color);
					Render::String(LootPos2D, LootStr, true, Color);

					CurEnt->ItemGlow();
				}
			}			
			//player esp
			else if ((CurEnt->Type(Player)|| CurEnt->Type(NPC))){				
				//get screen position
				Vector2 Head2D, Feet2D; 
				Vector3 Head3D = CurEnt->HitBoxPos(0, false);
				if (World2Screen(Head3D, Head2D) && World2Screen(CurEnt->AbsOrgin(), Feet2D))
				{
					if (Visuals::ESP) {
						//get box size
						float Height = (Head2D.y - Feet2D.y), Width = Height / 2.f;

						//set box color
						Render::Color BoxColor = Render::Color(255, 0, 0);
						if (CurEnt->Knocked()) BoxColor = Render::Color(192, 192, 192);
						else if (EntityCache[i].Visible) BoxColor = Render::Color(0, 255, 0);

						//draw box
						Render::Rectangle({ (Feet2D.x - (Width / 2.f)), Feet2D.y }, { Width, Height }, Render::Color(0, 0, 0), 3.f);
						Render::Rectangle({ (Feet2D.x - (Width / 2.f)), Feet2D.y }, { Width, Height }, BoxColor);

						//draw hp
						float HealthSize = (Height * CurEnt->Health()) / 100.f;
						Render::Line({ (Feet2D.x - (Width / 2.f) + 4.f), Feet2D.y }, { (Feet2D.x - (Width / 2.f) + 4.f), Head2D.y }, Render::Color(0, 0, 0), 5.f);
						Render::Line({ (Feet2D.x - (Width / 2.f) + 4.f), Feet2D.y + HealthSize }, { (Feet2D.x - (Width / 2.f) + 4.f), Head2D.y }, Render::Color(255, 0, 0), 2.f);
						Render::Line({ (Feet2D.x - (Width / 2.f) + 4.f), Feet2D.y }, { (Feet2D.x - (Width / 2.f) + 4.f), Feet2D.y + HealthSize }, Render::Color(0, 255, 0), 2.f);

					}
					//glow esp
					if (Visuals::GlowESP) {
						Vector3 Color = { 100.f, 0.f, 10.f };
						if (CurEnt->Knocked()) Color = { 130.f, 130.f, 130.f };
						else if (EntityCache[i].Visible) Color = { 0.f, 100.f, 10.f };
						//CurEnt->Glow(Color);	

						Vector3 Color1 = { 6.f, 6.f, 0.f };
						if (CurEnt->Knocked()) Color1 = { 0.f, 3.f, 3.f };
						else if (EntityCache[i].Visible) Color1 = { 8.f, 0.f, 0.f };
						CurEnt->GlowCRZ(Color1);
					}

					if (Visuals::DrawName) {
						//draw name
						const char* Name = CurEnt->PlayerName();
						if (!Name) continue; wchar_t Str[64];
						for (int i = 0;; i++) { if (!(Str[i] = Name[i])) break; }
						Render::String({ Feet2D.x, Head2D.y - 10.f }, Str, true);
					}				
				}
			}
		}
		
		
		if (GUI::Visible) 
		{
			//VIRTUALIZER_MUTATE_ONLY_START
			GUI::Begin(E(L"ORION"), MenuPos, { 300.f, 520.f }, { 0.f, 0.f, 0.1f, .75f });

			GUI::Spacing(E(L"Visuals"));
			GUI::CheckBox(E(L"Player ESP  F2"), Visuals::ESP);
			GUI::CheckBox(E(L"Glow ESP    F3"), Visuals::GlowESP);
			GUI::CheckBox(E(L"Loot ESP    F4"), Visuals::LootESP);
			GUI::CheckBox(E(L"Draw FOV    F5"), Visuals::DrawFOV);
			GUI::CheckBox(E(L"Draw Name   F6"), Visuals::DrawName);
			GUI::CheckBox(E(L"Developer   F8"), Visuals::Developer);
			GUI::CheckBox(E(L"Draw CrossHair"), Visuals::DrawCrossHair);
			GUI::SliderFloat(E(L"ESP Distance"), &Visuals::DistanceESP, 100.f, 800.f);
			GUI::SliderFloat(E(L"Loot Distance"), &Visuals::DistanceLoot, 50.f, 300.f);

			GUI::Spacing(E(L"Aimbot"));
			GUI::CheckBox(E(L"Active"), AimBot::Active);
			//GUI::CheckBox(E(L"Draw FOV"), Visuals::DrawFOV);
			GUI::CheckBox(E(L"Nearest bone"), AimBot::Nearest);
			GUI::CheckBox(E(L"Ignore Knocked"), AimBot::NoKnocked);
			GUI::CheckBox(E(L"Remove Sway & Recoil"), AimBot::WeaponFix);
			//GUI::CheckBox(E(L"Prediction Type (Green - Old, Red - New)"), AimBot::PredictType);

			GUI::SliderFloat(E(L"FOV"), &AimBot::FOV, 10.f, 50.f);
			GUI::SliderFloat(E(L"Smooth"), &AimBot::Smooth, 1.f, 10.f);
			GUI::SliderFloat(E(L"Distance"), &AimBot::Distance, 20.f, 300.f);

			GUI::Spacing(E(L"Misc"));
			GUI::CheckBox(E(L"Bhop"), Misc::Bhop);

			GUI::Spacing(E(L"INSECURE FEATURES"));
			GUI::CheckBox(E(L"Silent Aim"), AimBot::Silent);

			//GUI::CheckBox(E(L"SlowMo (ALT Key)"), Misc::SlowMo);
			GUI::End();
			//VIRTUALIZER_MUTATE_ONLY_END
		}

		Render::EndFrame(I::SwapChain);
	}

	static bool init;
	if (!init) {
		//HWND Wnd = FC(user32, GetActiveWindow); if(!Wnd) goto ret;
		//auto ThreadID = FC(user32, GetWindowThreadProcessId, Wnd, nullptr);
		auto ThID = FC(kernel32, GetCurrentThreadId);
		FC(user32, SetWindowsHookExA, WH_GETMESSAGE, GUI::WndHook, (HINSTANCE)GetModuleBase(nullptr), ThID);
		//GUI::NextWndProc = (WNDPROC)FC(user32, SetWindowLongPtrA, Wnd, GWLP_WNDPROC, (LONG_PTR)GUI::WndProc);
		init = true;
	}

	ret:
	SpoofCall(PresentOrg, a1, a2);
}

//HRESULT __fastcall Resize(IDXGISwapChain* Swap, UINT a2, UINT a3, UINT a4, DXGI_FORMAT a5, UINT a6) {
//
//	auto state = oResize(Swap, a2, a3, a4, a5, a6);
//	GUI::Render.Release(); 
//	return state;
//}

//void HookD3D()
//{
//	/*DX11PresentFn* pOBS_Present = (DX11PresentFn*)RVA(FindPattern(E("FF 15 ? ? ? ? 48 8B 16 48 8B CE FF 52 10 48"), E(L"graphics-hook64.dll")), 6);
//	oPresent = *pOBS_Present; *pOBS_Present = Present;*/
//
//	DX11PresentFn* pOBS_Present = (DX11PresentFn*)(GetModuleBase(E(L"IGO64")) + 0x163CE8); 
//	oPresent = *pOBS_Present; *pOBS_Present = Present;
//}