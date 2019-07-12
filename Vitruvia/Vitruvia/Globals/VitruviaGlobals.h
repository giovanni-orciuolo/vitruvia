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
#include "../InterfaceList/InterfaceList.h"
#include "../Renderer/Renderer.h"

// "Globals" is a singleton class that manages every single global in the whole cheat.

#define DEF_GLOBAL(name, intern, type)	\
void set_##name(type v) { intern = v; }	\
type name() { return intern; }			\

#define DEBUG_VAR(type, name, value) Vitruvia::g_vecDebugVars.emplace_back(Vitruvia::DebugVar(name, std::to_string(value), type))
#define DEBUG_VAR_STR(type, name, value) Vitruvia::g_vecDebugVars.emplace_back(Vitruvia::DebugVar(name, value, type))
#define DRAW_DEBUG(startx, starty, color) Vitruvia::DebugVar::Draw(startx, starty, color); Vitruvia::DebugVar::End();

namespace Vitruvia {

enum class eDebugTypes
{
	INT, BOOL, FLOAT, STRING, ADDRESS, VECTOR
};

class DebugVar
{
public:
	static void Draw(int startx, int starty, Source::Color col);
	static void End();

	DebugVar(const std::string& name, const std::string& val, eDebugTypes type)
		: m_strName(name),
		m_strValue(val),
		m_eValueType(type) {}

private:
	std::string m_strName;		// Name of the var, example: "pLocal->m_bIsLookingAtWeapon()"
	std::string m_strValue;		// Value in form of string, grab it using to_string or smth like it
	eDebugTypes m_eValueType;	// Value type of the var
};
extern std::vector<DebugVar> g_vecDebugVars;

class V_Globals
{
public:

	void Init();

	DEF_GLOBAL(ScreenWidth, m_iScreenWidth, int)
	DEF_GLOBAL(ScreenHeight, m_iScreenHeight, int)
	DEF_GLOBAL(ConfigFolder, m_strConfigFolder, std::string)
	DEF_GLOBAL(ThisModule, m_hThisModule, HMODULE)
	DEF_GLOBAL(CSGO_Window, m_hWindow, HWND)
	DEF_GLOBAL(ClientFOV, m_flClientFOV, float)
	DEF_GLOBAL(UsingBulletHandler, m_bUsingBulletHandler, bool)
	DEF_GLOBAL(RecoilScale, m_flRecoilScale, float)
	DEF_GLOBAL(ShouldCastBeam, m_bCastBeam, bool)
	DEF_GLOBAL(Aimbotting, m_bAimbotting, bool)
	DEF_GLOBAL(VisualAngles, m_vecVisualAngles, Source::Vector)
	DEF_GLOBAL(AntiAiming, m_bAntiAiming, bool)
	DEF_GLOBAL(CurrentTickCount, m_iCurrentTickCount, int)

	IMPLEMENT_SINGLETON(V_Globals)
protected:
	int m_iScreenWidth;
	int m_iScreenHeight;
	std::string m_strConfigFolder;
	HMODULE m_hThisModule;
	HWND m_hWindow;
	float m_flClientFOV;
	bool m_bUsingBulletHandler;
	float m_flRecoilScale;
	bool m_bCastBeam;
	bool m_bAimbotting;
	Source::Vector m_vecVisualAngles;
	bool m_bAntiAiming;
	int m_iCurrentTickCount;
};

#define global V_Globals::GetInstance()

}
