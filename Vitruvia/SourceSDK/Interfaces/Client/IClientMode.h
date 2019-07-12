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
#include "../../SourceCommon.h"

SOURCE_INIT

abstract_class IChatElement
{
public:
	void ChatPrintf(int iPlayerIndex, int iFilter, const char* fmt, ...)
	{
		return CallVFunction<void(__cdecl*)(void*, int, int, const char*, ...)>(this, 26)(this, iPlayerIndex, iFilter, fmt);
	}
	void ChatPrintfW(int iPlayerIndex, int iFilter, const wchar_t* fmt)
	{
		return CallVFunction<void(__thiscall*)(void*, int, int, const wchar_t*)>(this, 27)(this, iPlayerIndex, iFilter, fmt);
	}
};

abstract_class IClientMode
{
public:
	// TODO: Maybe add some shit?
	byte _pad[0x1C];
	IChatElement* m_pChatElement;
};

SOURCE_END