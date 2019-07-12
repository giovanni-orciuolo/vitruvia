// Class built with love by Vitruvia :*
// Build date: Friday 7/4/2017

#pragma once
#include "C_BaseCSGrenadeProjectile.h"

SOURCE_INIT

class C_SmokeGrenadeProjectile : public C_BaseCSGrenadeProjectile
{
public:
	MAKE_NETVAR(bool, m_bDidSmokeEffect, "m_bDidSmokeEffect", "DT_SmokeGrenadeProjectile")
	MAKE_NETVAR(int, m_nSmokeEffectTickBegin, "m_nSmokeEffectTickBegin", "DT_SmokeGrenadeProjectile")
};

SOURCE_END
