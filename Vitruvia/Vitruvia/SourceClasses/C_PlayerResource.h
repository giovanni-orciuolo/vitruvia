// Class built with love by Vitruvia :*
// Build date: Monday 24/4/2017

#pragma once
#include "../Vitruvia.h"

SOURCE_INIT

class C_PlayerResource
{
public:
	MAKE_NETVAR_ARRAY(int, m_iPing, "m_iPing", "DT_PlayerResource")
	MAKE_NETVAR_ARRAY(int, m_iKills, "m_iKills", "DT_PlayerResource")
	MAKE_NETVAR_ARRAY(int, m_iAssists, "m_iAssists", "DT_PlayerResource")
	MAKE_NETVAR_ARRAY(int, m_iDeaths, "m_iDeaths", "DT_PlayerResource")
	MAKE_NETVAR_ARRAY(bool, m_bConnected, "m_bConnected", "DT_PlayerResource")
	MAKE_NETVAR_ARRAY(int, m_iTeam, "m_iTeam", "DT_PlayerResource")
	MAKE_NETVAR_ARRAY(int, m_iPendingTeam, "m_iPendingTeam", "DT_PlayerResource")
	MAKE_NETVAR_ARRAY(bool, m_bAlive, "m_bAlive", "DT_PlayerResource")
	MAKE_NETVAR_ARRAY(int, m_iHealth, "m_iHealth", "DT_PlayerResource")
	MAKE_NETVAR_ARRAY(int, m_iCoachingTeam, "m_iCoachingTeam", "DT_PlayerResource")
};

SOURCE_END
