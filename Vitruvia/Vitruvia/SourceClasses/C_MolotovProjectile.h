// Class built with love by Vitruvia :*
// Build date: Friday 7/4/2017

#pragma once
#include "C_BaseCSGrenadeProjectile.h"

SOURCE_INIT

class C_MolotovProjectile : public C_BaseCSGrenadeProjectile
{
public:
	MAKE_NETVAR(int, m_bIsIncGrenade, "m_bIsIncGrenade", "DT_MolotovProjectile")
};

SOURCE_END
