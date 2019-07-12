// Class built with love by Vitruvia :*
// Build date: Friday 7/4/2017

#pragma once
#include "C_WeaponCSBase.h"

SOURCE_INIT

class C_BaseCSGrenade : public C_WeaponCSBase
{
public:
	MAKE_NETVAR(bool, m_bRedraw, "m_bRedraw", "DT_BaseCSGrenade")
	MAKE_NETVAR(bool, m_bIsHeldByPlayer, "m_bIsHeldByPlayer", "DT_BaseCSGrenade")
	MAKE_NETVAR(bool, m_bPinPulled, "m_bPinPulled", "DT_BaseCSGrenade")
	MAKE_NETVAR(float, m_fThrowTime, "m_fThrowTime", "DT_BaseCSGrenade")
	MAKE_NETVAR(bool, m_bLoopingSoundPlaying, "m_bLoopingSoundPlaying", "DT_BaseCSGrenade")
	MAKE_NETVAR(float, m_flThrowStrength, "m_flThrowStrength", "DT_BaseCSGrenade")
};

SOURCE_END
