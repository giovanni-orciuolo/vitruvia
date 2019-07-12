// Class built with love by Vitruvia :*
// Build date: Friday 7/4/2017

#pragma once
#include "C_BaseAnimating.h"

SOURCE_INIT

class C_BaseGrenade : public C_BaseAnimating
{
public:
	MAKE_NETVAR(float, m_flDamage, "m_flDamage", "DT_BaseGrenade")
	MAKE_NETVAR(float, m_DmgRadius, "m_DmgRadius", "DT_BaseGrenade")
	MAKE_NETVAR(bool, m_bIsLive, "m_bIsLive", "DT_BaseGrenade")
	MAKE_NETVAR(EHANDLE, m_hThrower, "m_hThrower", "DT_BaseGrenade")
	MAKE_NETVAR(Vector, m_vecVelocity, "m_vecVelocity", "DT_BaseGrenade")
	MAKE_NETVAR(int, m_fFlags, "m_fFlags", "DT_BaseGrenade")
};

SOURCE_END
