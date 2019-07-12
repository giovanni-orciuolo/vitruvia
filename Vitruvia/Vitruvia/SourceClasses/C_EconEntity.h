// Class built with love by Vitruvia :*
// Build date: Friday 7/4/2017

#pragma once
#include "C_BaseFlex.h"

SOURCE_INIT

class C_EconEntity : public C_BaseFlex
{
public:
	MAKE_NETVAR(EHANDLE, m_hOuter, "m_hOuter", "DT_EconEntity")
	MAKE_NETVAR(int, m_ProviderType, "m_ProviderType", "DT_EconEntity")
	MAKE_NETVAR(int, m_iReapplyProvisionParity, "m_iReapplyProvisionParity", "DT_EconEntity")
	MAKE_NETVAR_PTR(int, m_iItemDefinitionIndex, "m_iItemDefinitionIndex", "DT_EconEntity")
	MAKE_NETVAR_PTR(int, m_iEntityLevel, "m_iEntityLevel", "DT_EconEntity")
	MAKE_NETVAR_PTR(int, m_iItemIDHigh, "m_iItemIDHigh", "DT_EconEntity")
	MAKE_NETVAR_PTR(int, m_iItemIDLow, "m_iItemIDLow", "DT_EconEntity")
	MAKE_NETVAR_PTR(int, m_iAccountID, "m_iAccountID", "DT_EconEntity")
	MAKE_NETVAR_PTR(int, m_iEntityQuality, "m_iEntityQuality", "DT_EconEntity")
	MAKE_NETVAR(bool, m_bInitialized, "m_bInitialized", "DT_EconEntity")
	MAKE_NETVAR_PTR(char, m_szCustomName, "m_szCustomName", "DT_EconEntity") // Size = 161
	MAKE_NETVAR_PTR(int, m_OriginalOwnerXuidLow, "m_OriginalOwnerXuidLow", "DT_EconEntity")
	MAKE_NETVAR_PTR(int, m_OriginalOwnerXuidHigh, "m_OriginalOwnerXuidHigh", "DT_EconEntity")
	MAKE_NETVAR_PTR(int, m_nFallbackPaintKit, "m_nFallbackPaintKit", "DT_EconEntity")
	MAKE_NETVAR_PTR(int, m_nFallbackSeed, "m_nFallbackSeed", "DT_EconEntity")
	MAKE_NETVAR_PTR(float, m_flFallbackWear, "m_flFallbackWear", "DT_EconEntity")
	MAKE_NETVAR_PTR(int, m_nFallbackStatTrak, "m_nFallbackStatTrak", "DT_EconEntity")
};

SOURCE_END