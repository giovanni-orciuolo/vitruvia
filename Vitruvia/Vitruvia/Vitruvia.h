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

#pragma once
#include "../SourceSDK/SourceSDK.h"

#include <TlHelp32.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <chrono>
#include <thread>
#include <Psapi.h>
#include <ShlObj.h>
#include <codecvt>
#include <unordered_map>
#include <typeinfo>
#include <array>
#include <cstdarg>
#include <sys\stat.h>
#include <sys\types.h>
#include <atomic>
#include <filesystem>
#include <intrin.h>
#include <random>
#include <iterator>
#include <deque>
#include <Iphlpapi.h>

#include <d3d9.h>
#include <d3dx9.h>

#if defined(WIN32) || defined(WIN64)
// Copied from linux libc sys/stat.h:
#define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
#define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#endif

#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "comsuppw.lib")
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#pragma warning(disable : 4800)
#pragma warning(disable : 4244)
#pragma warning(disable : 4806)
#pragma warning(disable : 4003)
#pragma warning(disable : 4018)

#ifndef ENABLE_CONSOLE
#define ENABLE_CONSOLE
#endif
#ifndef ENABLE_XOR
#define ENABLE_XOR
#endif
#ifndef ENABLE_HIDE
#define ENABLE_HIDE
#endif
#ifndef ENABLE_PTDEBUG
#define ENABLE_PTDEBUG
#endif

#ifndef _DEBUG
#undef ENABLE_CONSOLE // Disable developer console
#undef ENABLE_PTDEBUG // Disable PaintTraverse debug mode
//#undef ENABLE_XOR // Disable encryption of strings
//#undef ENABLE_HIDE // Disable hiding of the module
#endif

#ifdef _DEBUG
//#undef ENABLE_CONSOLE // Disable developer console
//#undef ENABLE_PTDEBUG // Disable PaintTraverse debug mode
#undef ENABLE_XOR	// Disable encryption of strings
#undef ENABLE_HIDE	// Disable hiding of the module
#endif

#define MODULE_BASE(module) (DWORD)(GetModuleHandle(module))
#define CLIENT_DLL	enc("client.dll")
#define ENGINE_DLL	enc("engine.dll")
#define CLIENT_BASE MODULE_BASE(CLIENT_DLL)
#define ENGINE_BASE MODULE_BASE(ENGINE_DLL)
#define SAFE_DELETE(ptr) if (ptr) { delete ptr; ptr = nullptr; }
#define AllocateMemory(ptr, size) ptr = malloc(size)
#define FreeMemory(ptr) free(ptr)

#define CHEAT_NAME		enc("Vitruvia")
#define CHEAT_VERSION	enc("beta 0.7")
#define MENU_TITLE_STR	enc("Vitruvia - Private Beta v0.7")
#define WATERMARK		enc("VITRUVIA - Made with love by double v")

#define rcast			reinterpret_cast
#define scast			static_cast
#define ccast			const_cast
#define STL_SLEEP(ms)	std::this_thread::sleep_for(std::chrono::milliseconds(ms))
#define offset_t		std::uintptr_t

#define FOR_EACH_CLIENT	for (int ClientIndex = 1; ClientIndex < Source::Engine->GetMaxClients() + 1; ClientIndex++)
#define FOR_EACH_ENTITY for (int EntityIndex = 0; EntityIndex < Source::EntityList->GetHighestEntityIndex(); EntityIndex++)
#define FOR_EACH_WORLD_ENTITY for (int EntityIndex = 65; EntityIndex < Source::EntityList->GetHighestEntityIndex(); EntityIndex++)
#define FOR_EACH_KEY	for (int bc = 0; Source::IsKeyCode(Source::ButtonCode_t(bc)) || Source::IsMouseCode(Source::ButtonCode_t(bc)); bc++)

#define GET_ENTITY(index) (Source::C_CSPlayer*)Source::EntityList->GetClientEntity(index)
#define GET_LOCAL() (Source::C_CSPlayer*)Source::EntityList->GetClientEntity(Source::Engine->GetLocalPlayer())
#define INDEX int

#define EVENT_IS(name) !strcmp(game_event->GetName(), name)
#define DID_DISCONNECT() EVENT_IS(enc("cs_game_disconnected"))
#define USERID2PLAYER(userid) (Source::C_CSPlayer*)Source::EntityList->GetClientEntity(Source::Engine->GetPlayerForUserID(userid))

extern bool g_vecPressedKeys[256];
extern bool g_bMenuInitialized;
extern bool g_bMenuOpened;
extern bool g_bMenuClosing;
extern bool k_shouldListen;
extern int* k_keyOutput;
extern bool g_bWallColorModified;

extern bool g_bSendPackets;
extern int g_iTickBaseShift;

#define DRAW_PANEL enc("FocusOverlayPanel")
#define PRESSED 0x8000
#define KEY(x) (g_vecPressedKeys[x])

#define TICK_INTERVAL			Source::Globals->interval_per_tick
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
#define TICKS_TO_TIME( t )		( TICK_INTERVAL *( t ) )
#define ROUND_TO_TICKS( t )		( TICK_INTERVAL * TIME_TO_TICKS( t ) )
#define TICK_NEVER_THINK		(-1)

#define GRAB_SCREEN_SIZE									\
int screen_width, screen_height;							\
Source::Engine->GetScreenSize(screen_width, screen_height);	\

template<typename T>
FORCEINLINE T* MAKE_PTR(void* base, std::uintptr_t offset)
{
	return reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(base) + offset);
}

#define DO_ONCE											\
	for (static std::atomic<int> _run_already_(false);	\
		!_run_already_.fetch_or(true);)					\

#define IMPLEMENT_SINGLETON(classname)\
public:\
	static std::shared_ptr<classname> GetInstance() {\
		static std::shared_ptr<classname> instance(new classname);\
		return instance;\
	}\
private:\
	classname() {}\
	classname(classname const&) = delete;\
	void operator=(classname const&) = delete;\

#define IMPLEMENT_SINGLETON_INITIALIZED(classname, initfunc_name)\
public:\
	static std::shared_ptr<classname> GetInstance() {\
		static std::shared_ptr<classname> instance(new classname);\
		return instance;\
	}\
private:\
	classname() {initfunc_name();}\
	classname(classname const&) = delete;\
	void operator=(classname const&) = delete;\

struct Point {
	float x;
	float y;
};

struct Rect {
	float x;
	float y;
	float w;
	float h;
};

using BoneMatrix = std::array<matrix3x4_t, MAXSTUDIOBONES>;

#define getStackBase() (reinterpret_cast<char*>(_AddressOfReturnAddress()) - sizeof(void*))

#include "Encryption\XorCompileTime.hpp"
#include "Utils\VitruviaUtils.h"
#include "Netvars\NetvarManager.h"
#include "Offsets\Offsets.h"

using namespace XorCompileTime;