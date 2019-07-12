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

#include "IMaterialSystem.h"

SOURCE_INIT

IMaterial* IMaterialSystem::CreateMaterial(const char* pMaterialName, KeyValues* pVMTKeyValues)
{
	typedef IMaterial*(__thiscall* OriginalFn)(void*, const char *pMaterialName, KeyValues *pVMTKeyValues);
	return CallVFunction<OriginalFn>(this, 83)(this, pMaterialName, pVMTKeyValues);
}
IMaterial* IMaterialSystem::FindMaterial(char const* pMaterialName, const char* pTextureGroupName, bool complain, const char* pComplainPrefix)
{
	typedef IMaterial*(__thiscall* OriginalFn)(void*, char const* pMaterialName, const char *pTextureGroupName, bool complain, const char *pComplainPrefix);
	return CallVFunction<OriginalFn>(this, 84)(this, pMaterialName, pTextureGroupName, complain, pComplainPrefix);
}
MaterialHandle_t IMaterialSystem::FirstMaterial()
{
	return CallVFunction<MaterialHandle_t(__thiscall*)(void*)>(this, 86)(this);
}
MaterialHandle_t IMaterialSystem::NextMaterial(MaterialHandle_t MatHandle)
{
	return CallVFunction<MaterialHandle_t(__thiscall*)(void*, MaterialHandle_t)>(this, 87)(this, MatHandle);
}
MaterialHandle_t IMaterialSystem::InvalidMaterial()
{
	return CallVFunction<MaterialHandle_t(__thiscall*)(void*)>(this, 88)(this);
}
IMaterial* IMaterialSystem::GetMaterial(MaterialHandle_t MatHandle)
{
	return CallVFunction<IMaterial*(__thiscall*)(void*, MaterialHandle_t)>(this, 89)(this, MatHandle);
}

SOURCE_END