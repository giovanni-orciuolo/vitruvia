// Class built with love by Vitruvia :*
// Build date: Wednesday 23/8/2017

#pragma once
#include "C_BaseAnimating.h"

SOURCE_INIT

class C_PlantedC4 : public C_BaseAnimating
{
public:
	MAKE_NETVAR(bool, m_bBombTicking, "m_bBombTicking", "DT_PlantedC4")
	MAKE_NETVAR(float, m_flC4Blow, "m_flC4Blow", "DT_PlantedC4")
	MAKE_NETVAR(float, m_flTimerLength, "m_flTimerLength", "DT_PlantedC4")
	MAKE_NETVAR(float, m_flDefuseLength, "m_flDefuseLength", "DT_PlantedC4")
	MAKE_NETVAR(float, m_flDefuseCountDown, "m_flDefuseCountDown", "DT_PlantedC4")
	MAKE_NETVAR(bool, m_bBombDefused, "m_bBombDefused", "DT_PlantedC4")
	MAKE_NETVAR(EHANDLE, m_hBombDefuser, "m_hBombDefuser", "DT_PlantedC4")
};

SOURCE_END
