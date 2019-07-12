/*
	____   ____.__  __                     .__        
	\   \ /   /|__|/  |________ __ _____  _|__|____   
	 \   Y   / |  \   __\_  __ \  |  \  \/ /  \__  \  
	  \     /  |  ||  |  |  | \/  |  /\   /|  |/ __ \_
	   \___/   |__||__|  |__|  |____/  \_/ |__(____  /
												   \/ 
	Premium Internal Multihack for Counter-Strike: Global Offensive
	Made with love by double v - 2017
*/

#include "Hooks\Hooks.h"
#include "Config\Config.h"
#include "Hooks\EventListeners\EventListeners.h"
#include "Globals\VitruviaGlobals.h"
#include "Skins\KitParser\KitParser.h"
#include "Features\Visuals\Visuals.h"
#include "Tools\ThirdPerson.h"
#include "Utils\CrashLogger\CrashLogger.h"
#include "Utils\GameHelper\GameHelper.h"
#include "Features/Misc/Misc.h"

HINSTANCE hDLL;
HANDLE hCheat;
void Cheat();
void Close();

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);

	hDLL = hInstance;
	DisableThreadLibraryCalls(hInstance);
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		hCheat = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Cheat, NULL, NULL, NULL);
		break;
	case DLL_PROCESS_DETACH:
		ToggleConsole(false);
		Vitruvia::g_CrashLog.SetWriting(false);
		Vitruvia::g_CrashLog.RemoveLogs();
		std::remove(Vitruvia::global->ConfigFolder().append(enc("\\loader\\vv.dll")).c_str());
		break;
	}
	return TRUE;
}

void Cheat()
{
	// Main entry point of the cheat
	using namespace Vitruvia;
	using namespace Source;
	g_CrashLog.SetWriting(true);
	ToggleConsole(true);

	// First thing I do ever is setting up the config folder
	global->set_ConfigFolder(GetAppDataFolder());
	Log(enc("Config folder is: %s"), global->ConfigFolder().c_str());
	if (!DirectoryExists(global->ConfigFolder()))
	{
		CreateDirectory(global->ConfigFolder().c_str(), NULL);
		Log(enc("Config folder created."));
	}

	g_CrashLog.Add(enc("SETTING UP VITRUVIA..."));

	// Wait at most 10s for the main game modules to be loaded.
	if (WaitForModules(10000, { S2WS(CLIENT_DLL) , S2WS(ENGINE_DLL), S2WS(enc("shaderapidx9.dll")), S2WS(enc("serverbrowser.dll")) }) 
		== WAIT_TIMEOUT) {
		// One or more modules were not loaded in time
		Err(enc("FATAL ERROR: LOADING FOR MODULES TIMED OUT."));
		return;
	}

	srand((unsigned)GetEpochTime());

	Log(enc("Cheat's thread started. Loading..."));

	// Scan offsets
	Offsets::ScanOffsets();

	// Setup interfaces
	Source::SetupInterfaces();
	Log(enc("Done."));

	// Setup netvar manager
	g_pNetvars = new V_NetvarManager(Source::BaseClient->GetAllClasses());
	Log(enc("Netvar manager initialized correctly."));

	// Setup glow object manager for glow feature
	CGlowObjectManager* manager = rcast<CGlowObjectManager*>(Offsets::GlowObject);
	Vitruvia::Glow->SetGlowManager(manager);

	// Create materials for chams in case they ain't already created
	Vitruvia::Chams->CreateMaterials();

	// Setup globals
	global->Init();
	// Special global accessor to the this module base
	global->set_ThisModule(hDLL);

	//Log(enc("Dumping netvars..."));
	//g_pNetvars->DumpNetvars(Source::BaseClient->GetAllClasses());
	//Build tables here if you want to!
	//Log(enc("Done."));

	// Setup config default values
	Config::VisualsOther.ToggleKey.m_Value = ButtonCode_t::KEY_V;
	Config::Misc.MenuKey.m_Value = ButtonCode_t::KEY_INSERT;
	Config::Misc.PanicKey.m_Value = ButtonCode_t::KEY_END;
	Config::AntiAim.JitterRange.m_Value = 25.f;
	Config::AntiAim.SpinSpeed.m_Value = 0.4f;
	Config::AntiAim.LowerBodyDelta = -100.f;
	Config::Misc.MenuFadeSpeed = 1.f;
	Config::ESP.FadeSpeed = 1.f;
	Config::MenuStyle.ResetStyle();

	// Setup fonts
	Render::Fonts::SetupFonts();

	// Setup color configs
	Config::Colors.InitColors();

	// Setup ThirdPerson manager
	Tools::CThirdpersonManager::GetInstance()->Start();

	// Setup event listener
	g_pEventListener = new V_EventListener({
		enc("cs_game_disconnected"),
		enc("player_connect_full"),
		enc("player_death"),
		enc("player_hurt"),
		enc("switch_team"),
		enc("weapon_fire"),
		enc("player_footstep"),
		enc("bullet_impact")
	});
	NotificationSystem->AddNotifications();

	// Setup exports
	Log(enc("Grabbing exports..."));
	Maths::RandomSeed = (RandomSeedFn)GetProcAddress(GetModuleHandleA(enc("vstdlib.dll")), enc("RandomSeed"));
	Maths::RandomFloat = (RandomFloatFn)GetProcAddress(GetModuleHandleA(enc("vstdlib.dll")), enc("RandomFloat"));
	Maths::RandomFloatExp = (RandomFloatExpFn)GetProcAddress(GetModuleHandleA(enc("vstdlib.dll")), enc("RandomFloatExp"));
	Maths::RandomInt = (RandomIntFn)GetProcAddress(GetModuleHandleA(enc("vstdlib.dll")), enc("RandomInt"));
	Maths::RandomGaussianFloat = (RandomFloatFn)GetProcAddress(GetModuleHandleA(enc("vstdlib.dll")), enc("RandomGaussianFloat"));
	Log(enc("Done"));

	// Fill kits
	GrabKits();

	// TESTING ZONE
	// You can print whatever you want here to test shit
	if (Source::Engine->IsInGame())
	{
		auto local = C_CSPlayer::GetLocal();
		Log(enc("Local: 0x%X"), rcast<offset_t>(local));
		Log(enc("Level name: %s"), Source::ClientState->m_szLevelNameShort);
		Log(enc("Valve DS: %d"), Source::GameRules->m_bIsValveDS());
		Log(enc("Map has bomb target: %d"), Source::GameRules->m_bMapHasBombTarget());
		if (local)
		{
			Log(enc("My ping: %d"), Source::PlayerResource->m_iPing(local->GetIndex()));
			Log(enc("My clan tag: %s"), local->GetClanTag().c_str());
			Log(enc("Local ActiveWeapon: 0x%X"), rcast<offset_t>(local->GetActiveWeapon()));
		}
	}

	// Hook everything
	Hooks::HookFunctions();
	Hooks::HookProxies();

	// Log shit
	Warn(enc("Vitruvia has been initialized successfully."));
	Source::CVar->ConsoleColorPrintf(Source::Color::Cyan(), enc("Vitruvia attached successfully.\n"));

	// Execute OnLoad for each feature
	for (auto f : Vitruvia::Features)
		f->OnLoad();

	// Start private thread
	while (!Source::InputSystem->IsButtonDown((ButtonCode_t)Config::Misc.PanicKey.m_Value))
	{
		STL_SLEEP(10);
	}

	Close();
}
void Close()
{
	// Exit point of the cheat
	using namespace Vitruvia;
	using namespace Source;

	Log(enc("Closing Vitruvia..."));
	Config::ESP.Active.m_Value = false;
	STL_SLEEP(500);

	// For each feature call OnShutdown
	for (auto f : Vitruvia::Features)
		f->OnShutdown();

	Hooks::UnHookFunctions();					// Will handle all the unhooking and cleanups
	Hooks::UnHookProxies();						// Restore original proxies
	ImGui_ImplDX9_InvalidateDeviceObjects();	// Invalidate ImGui device objects
	ImGui::Shutdown();							// Clear ImGui
	delete Render::D3D::g_pRenderer;			// Invalidate D3D renderer device objects
	delete g_pNetvars;							// Even if I don't need special cleanups for netvars
	delete g_pEventListener;					// Remove event listener
	Config::ClearWeaponGroups();				// Clear WeaponGroups
	ClearFeatures();							// Clear Features

	// Remove chams
	std::string game_path = GetGamePath();
	if (std::remove(game_path.append(enc("csgo\\materials\\vv_mat_textured.vmt")).c_str()) == 1) {
		Err(enc("Error removing %s: %s"), game_path.append(enc("csgo\\materials\\vv_mat_textured.vmt")).c_str(), strerror(errno));
	}
	if (std::remove(game_path.append(enc("csgo\\materials\\vv_mat_textured_xqz.vmt")).c_str()) == 1) {
		Err(enc("Error removing %s: %s"), game_path.append(enc("csgo\\materials\\vv_mat_textured_xqz.vmt")).c_str(), strerror(errno));
	}
	if (std::remove(game_path.append(enc("csgo\\materials\\vv_mat_flat.vmt")).c_str()) == 1) {
		Err(enc("Error removing %s: %s"), game_path.append(enc("csgo\\materials\\vv_mat_flat.vmt")).c_str(), strerror(errno));
	}
	if (std::remove(game_path.append(enc("csgo\\materials\\vv_mat_flat_xqz.vmt")).c_str()) == 1) {
		Err(enc("Error removing %s: %s"), game_path.append(enc("csgo\\materials\\vv_mat_flat_xqz.vmt")).c_str(), strerror(errno));
	}
	if (std::remove(game_path.append(enc("csgo\\materials\\vv_mat_angs.vmt")).c_str()) == 1) {
		Err(enc("Error removing %s: %s"), game_path.append(enc("csgo\\materials\\vv_mat_angs.vmt")).c_str(), strerror(errno));
	}

	ToggleConsole(false);
	g_CrashLog.Add(enc("VITRUVIA DETACHED."));

	Source::CVar->FindVar(enc("cl_mouseenable"))->SetValue(true);
	Source::Engine->ExecuteClientCmd(enc("crosshair 1"));
	Source::InputSystem->EnableInput(true);
	Source::CVar->ConsoleColorPrintf(Source::Color::Cyan(), enc("Vitruvia detached successfully.\n"));
	
	g_CrashLog.SetWriting(false);
	FreeLibraryAndExitThread(hDLL, 0);
}