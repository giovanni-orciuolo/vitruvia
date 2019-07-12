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
#include "VitruviaGlobals.h"

namespace Vitruvia {

inline std::string TypeToString(eDebugTypes t)
{
	switch (t)
	{
	case eDebugTypes::INT: return enc("int");
	case eDebugTypes::FLOAT: return enc("float");
	case eDebugTypes::BOOL: return enc("bool");
	case eDebugTypes::STRING: return enc("string");
	case eDebugTypes::ADDRESS: return enc("address");
	case eDebugTypes::VECTOR: return enc("vector");
	default: return enc("?");
	}
}

std::vector<DebugVar> g_vecDebugVars{};
void DebugVar::Draw(int startx, int starty, Source::Color col)
{
	// Draw every single debug var
	Render::String(Render::Fonts::Tahoma_Outline12, startx, starty, col, Render::eAlign::ALIGN_LEFT, enc("DEBUG VARIABLES:"));

	// For each var in s_vecDebugVars
	// I can't use enhanced for because I need the index to format
	for (unsigned int i = 0; i < g_vecDebugVars.size(); ++i)
	{
		// Grab the var
		auto var = g_vecDebugVars[i];

		// Setup the stringstream to store the thing I need to draw
		std::stringstream ss{};
		switch (var.m_eValueType)
		{
		case eDebugTypes::ADDRESS:
		{
			std::istringstream converter(var.m_strValue);
			uintptr_t val; converter >> val;
			ss << var.m_strName << enc(" = 0x") << std::uppercase << std::hex << val << std::dec << std::nouppercase << enc(" (") << TypeToString(var.m_eValueType) << enc(")");
		} break;
		default:
			ss << var.m_strName << enc(" = ") << var.m_strValue << enc(" (") << TypeToString(var.m_eValueType) << enc(")");
		}

		// Draw
		Render::String(Render::Fonts::Tahoma_Outline12, startx, starty + ((i+1) * 16), col, Render::eAlign::ALIGN_LEFT, ss.str().c_str());
	}
}
void DebugVar::End()
{
	// This is the behaviour to perform when the drawing ends
	// I need to clear up the s_vecDebugVars shit, otherwise it will overflow like there is no tomorrow
	g_vecDebugVars.clear();
}

void V_Globals::Init()
{
	GRAB_SCREEN_SIZE
	this->set_ScreenWidth(screen_width);
	this->set_ScreenHeight(screen_height);
	this->set_CSGO_Window(FindWindow(enc("Valve001"), NULL));
	this->set_ClientFOV(0.f);
	this->set_UsingBulletHandler(false);
	this->set_RecoilScale(Source::CVar->FindVar(enc("weapon_recoil_scale"))->GetFloat());
	this->set_ShouldCastBeam(false);
	this->set_Aimbotting(false);
	this->set_CurrentTickCount(0);
}

}
