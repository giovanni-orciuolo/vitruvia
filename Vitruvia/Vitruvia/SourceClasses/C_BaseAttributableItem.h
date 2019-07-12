// Class built with love by Vitruvia :*
// Build date: Friday 7/4/2017

#pragma once
#include "C_BaseAnimating.h"

SOURCE_INIT

class C_BaseAttributableItem : public C_BaseAnimating
{
public:
	void SetModelIndex(int model_index)
	{
		return CallVFunction<void(__thiscall*)(void*, int)>(this, 75)(this, model_index);
	}

	MAKE_NETVAR(EHANDLE, m_hOuter, "m_hOuter", "DT_BaseAttributableItem")
	MAKE_NETVAR(int, m_ProviderType, "m_ProviderType", "DT_BaseAttributableItem")
	MAKE_NETVAR(int, m_iReapplyProvisionParity, "m_iReapplyProvisionParity", "DT_BaseAttributableItem")
	MAKE_NETVAR_PTR(int, m_iItemDefinitionIndex, "m_iItemDefinitionIndex", "DT_BaseAttributableItem")
	MAKE_NETVAR_PTR(int, m_iEntityLevel, "m_iEntityLevel", "DT_BaseAttributableItem")
	MAKE_NETVAR_PTR(int, m_iItemIDHigh, "m_iItemIDHigh", "DT_BaseAttributableItem")
	MAKE_NETVAR_PTR(int, m_iItemIDLow, "m_iItemIDLow", "DT_BaseAttributableItem")
	MAKE_NETVAR_PTR(int, m_iAccountID, "m_iAccountID", "DT_BaseAttributableItem")
	MAKE_NETVAR_PTR(int, m_iEntityQuality, "m_iEntityQuality", "DT_BaseAttributableItem")
	MAKE_NETVAR(bool, m_bInitialized, "m_bInitialized", "DT_BaseAttributableItem")
	MAKE_NETVAR_PTR(char, m_szCustomName, "m_szCustomName", "DT_BaseAttributableItem") // Size = 161
	MAKE_NETVAR(int, lengthprop32, "lengthprop32", "DT_BaseAttributableItem")
	MAKE_NETVAR_PTR(int, m_OriginalOwnerXuidLow, "m_OriginalOwnerXuidLow", "DT_BaseAttributableItem")
	MAKE_NETVAR_PTR(int, m_OriginalOwnerXuidHigh, "m_OriginalOwnerXuidHigh", "DT_BaseAttributableItem")
	MAKE_NETVAR_PTR(int, m_nFallbackPaintKit, "m_nFallbackPaintKit", "DT_BaseAttributableItem")
	MAKE_NETVAR_PTR(int, m_nFallbackSeed, "m_nFallbackSeed", "DT_BaseAttributableItem")
	MAKE_NETVAR_PTR(float, m_flFallbackWear, "m_flFallbackWear", "DT_BaseAttributableItem")
	MAKE_NETVAR_PTR(int, m_nFallbackStatTrak, "m_nFallbackStatTrak", "DT_BaseAttributableItem")
};

SOURCE_END
