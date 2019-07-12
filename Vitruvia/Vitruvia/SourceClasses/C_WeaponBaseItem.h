// Class built with love by Vitruvia :*
// Build date: Friday 7/4/2017

#pragma once
#include "C_WeaponCSBase.h"

SOURCE_INIT

class C_WeaponBaseItem : public C_WeaponCSBase
{
public:
	MAKE_NETVAR(bool, m_bRedraw, "m_bRedraw", "DT_WeaponBaseItem")
};

SOURCE_END
