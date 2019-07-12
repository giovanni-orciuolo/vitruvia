// Class built with love by Vitruvia :*
// Build date: Friday 7/4/2017

#pragma once
#include "C_BaseGrenade.h"

SOURCE_INIT

class C_BaseCSGrenadeProjectile : public C_BaseGrenade
{
public:
	MAKE_NETVAR(Vector, m_vInitialVelocity, "m_vInitialVelocity", "DT_BaseCSGrenadeProjectile")
	MAKE_NETVAR(int, m_nBounces, "m_nBounces", "DT_BaseCSGrenadeProjectile")
};

SOURCE_END
