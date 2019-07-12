// Class built with love by Vitruvia :*
// Build date: Sunday 19/3/2017

#pragma once
#include "C_BaseFlex.h"

SOURCE_INIT

class C_BaseCombatCharacter : public C_BaseFlex
{
public:
	MAKE_NETVAR(float, m_flNextAttack, "m_flNextAttack", "DT_BaseCombatCharacter")
	MAKE_NETVAR(int, m_LastHitGroup, "m_LastHitGroup", "DT_BaseCombatCharacter")
	MAKE_NETVAR(EHANDLE, m_hActiveWeapon, "m_hActiveWeapon", "DT_BaseCombatCharacter")
	MAKE_NETVAR(float, m_flTimeOfLastInjury, "m_flTimeOfLastInjury", "DT_BaseCombatCharacter")
	MAKE_NETVAR(int, m_nRelativeDirectionOfLastInjury, "m_nRelativeDirectionOfLastInjury", "DT_BaseCombatCharacter")
	MAKE_NETVAR_PTR(EHANDLE, m_hMyWearables, "m_hMyWearables", "DT_BaseCombatCharacter")
};

SOURCE_END