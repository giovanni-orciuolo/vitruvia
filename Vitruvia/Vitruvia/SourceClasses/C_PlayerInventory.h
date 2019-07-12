#pragma once
#include "../../SourceSDK/SourceCommon.h"
#include "C_EconItem.h"
#include <vector>

SOURCE_INIT

class CSharedObjectTypeCache
{
public:
	void AddObject(void* obj);
	void RemoveObject(void* obj);
	std::vector<C_EconItem*> GetEconItems();
};

class C_PlayerInventory
{
public:
	void RemoveItem(uint64_t id);
	void RemoveItem(C_EconItem* item);
	void ClearInventory();
	CSharedObjectTypeCache* GetBaseTypeCache();
	uint32_t GetSteamID();
	bool AddEconItem(C_EconItem* item);
};

SOURCE_END