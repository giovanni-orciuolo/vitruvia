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
#include "IClientEntity.h"

SOURCE_INIT

struct EntityCacheInfo_t
{
	// Cached off because GetClientNetworkable is called a *lot*
	IClientNetworkable* m_pNetworkable;
	unsigned short m_BaseEntitiesIndex;	// Index into m_BaseEntities (or m_BaseEntities.InvalidIndex() if none).
	unsigned short m_bDormant;	// cached dormant state - this is only a bit
};

abstract_class IClientEntityList
{
public:
	virtual IClientNetworkable*   GetClientNetworkable(int Index) = 0;
	virtual IClientNetworkable*   GetClientNetworkable(CBaseHandle hEnt) = 0;
	virtual IClientUnknown*       GetClientUnknownFromHandle(CBaseHandle hEnt) = 0;
	virtual IClientEntity*        GetClientEntity(int Index) = 0;
	virtual IClientEntity*        GetClientEntityFromHandle(CBaseHandle hEntity) = 0;
	virtual int                   NumberOfEntities(bool bIncludeNonNetworkable) = 0;
	virtual int                   GetHighestEntityIndex(void) = 0;
	virtual void                  SetMaxEntities(int MaxEntities) = 0;
	virtual int                   GetMaxEntities() = 0;
	virtual EntityCacheInfo_t*	  GetClientNetworkableArray() = 0;
};

SOURCE_END