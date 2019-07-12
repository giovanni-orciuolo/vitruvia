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
#include "CUtlVector.h"

SOURCE_INIT

//typedef void(*ClientEffectCallback)(const CEffectData &data);

class CClientEffectRegistration
{
public:
	CClientEffectRegistration(const char *pEffectName, void* fn);

	const char *m_pEffectName;
	void* m_pFunction;
	CClientEffectRegistration *m_pNext;

	static CClientEffectRegistration* s_pHead;
};

SOURCE_END