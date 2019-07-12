/*
	 _____                            ___________ _   __
	/  ___|                          /  ___|  _  \ | / /
	\ `--.  ___  _   _ _ __ ___ ___  \ `--.| | | | |/ /
	 `--. \/ _ \| | | | '__/ __/ _ \  `--. \ | | |    \
	/\__/ / (_) | |_| | | | (_|  __/ /\__/ / |/ /| |\  \
	\____/ \___/ \__,_|_|  \___\___| \____/|___/ \_| \_/

	Designed for Counter-Strike: Global Offensive
	Copyright: ValvE Corporation 2017 (C)
*/

#pragma once
#include "../SourceCommon.h"

SOURCE_INIT

class CHudElement;

class CHud
{
public:
	CHudElement* FindElement(const char* pName, std::uintptr_t func_offset)
	{
		typedef CHudElement*(__thiscall *FindElement_t)(CHud*, const char*);
		FindElement_t FindElementFn = reinterpret_cast<FindElement_t>(func_offset);
		return FindElementFn(this, pName);
	}
};

SOURCE_END