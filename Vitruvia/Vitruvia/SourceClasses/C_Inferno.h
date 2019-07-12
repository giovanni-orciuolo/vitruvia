// Class built with love by Vitruvia :*
// Build date: Friday 7/4/2017

#pragma once
#include "C_BaseEntity.h"

SOURCE_INIT

class C_Inferno : public C_BaseEntity
{
public:
	MAKE_NETVAR(int, m_fireCount, "m_fireCount", "DT_Inferno")
};

SOURCE_END
