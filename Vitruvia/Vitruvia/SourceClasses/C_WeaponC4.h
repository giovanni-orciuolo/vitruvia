// Class built with love by Vitruvia :*
// Build date: Friday 7/4/2017

#pragma once
#include "C_WeaponCSBase.h"

SOURCE_INIT

class C_WeaponC4 : public C_WeaponCSBase
{
public:
	MAKE_NETVAR(bool, m_bStartedArming, "m_bStartedArming", "DT_WeaponC4")
	MAKE_NETVAR(bool, m_bBombPlacedAnimation, "m_bBombPlacedAnimation", "DT_WeaponC4")
	MAKE_NETVAR(float, m_fArmedTime, "m_fArmedTime", "DT_WeaponC4")
	MAKE_NETVAR(bool, m_bShowC4LED, "m_bShowC4LED", "DT_WeaponC4")
	MAKE_NETVAR(bool, m_bIsPlantingViaUse, "m_bIsPlantingViaUse", "DT_WeaponC4")
};

SOURCE_END
