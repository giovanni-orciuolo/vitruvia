// Class built with love by Vitruvia :*
// Build date: Thursday 14/9/2017

#pragma once
#include "C_BaseEntity.h"

SOURCE_INIT

class C_World : public C_BaseEntity
{
public:
	static FORCEINLINE C_World* GetWorld() { return (C_World*)(Source::EntityList->GetClientEntity(0)); }

	MAKE_NETVAR(float, m_flWaveHeight, "m_flWaveHeight", "DT_World")
	MAKE_NETVAR(Vector, m_WorldMins, "m_WorldMins", "DT_World")
	MAKE_NETVAR(Vector, m_WorldMaxs, "m_WorldMaxs", "DT_World")
	MAKE_NETVAR(bool, m_bStartDark, "m_bStartDark", "DT_World")
	MAKE_NETVAR(float, m_flMaxOccludeeArea, "m_flMaxOccludeeArea", "DT_World")
	MAKE_NETVAR(float, m_flMinOccluderArea, "m_flMinOccluderArea", "DT_World")
	MAKE_NETVAR(float, m_flMaxPropScreenSpaceWidth, "m_flMaxPropScreenSpaceWidth", "DT_World")
	MAKE_NETVAR(float, m_flMinPropScreenSpaceWidth, "m_flMinPropScreenSpaceWidth", "DT_World")
	MAKE_NETVAR_PTR(char, m_iszDetailSpriteMaterial, "m_iszDetailSpriteMaterial", "DT_World") // Size = 256
	MAKE_NETVAR(bool, m_bColdWorld, "m_bColdWorld", "DT_World")
	MAKE_NETVAR(int, m_iTimeOfDay, "m_iTimeOfDay", "DT_World")
};

SOURCE_END
