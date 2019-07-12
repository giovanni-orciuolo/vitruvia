// Class built with love by Vitruvia :*
// Build date: Sunday 19/3/2017

#pragma once
#include "C_BaseAnimatingOverlay.h"

SOURCE_INIT

class C_BaseFlex : public C_BaseAnimatingOverlay
{
public:
	MAKE_NETVAR(bool, m_blinktoggle, "m_blinktoggle", "DT_BaseFlex")
	MAKE_NETVAR(Vector, m_viewtarget, "m_viewtarget", "DT_BaseFlex")
};

SOURCE_END