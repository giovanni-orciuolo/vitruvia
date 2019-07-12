#include "C_PlayerInventory.h"
#include "C_BasePlayer.h"

SOURCE_INIT

void CSharedObjectTypeCache::AddObject(void* obj)
{
	typedef void(__thiscall* tOriginal)(void*, void*);
	CallVFunction<tOriginal>(this, 1)(this, obj);
}

void CSharedObjectTypeCache::RemoveObject(void* obj)
{
	typedef void(__thiscall* tOriginal)(void*, void*);
	CallVFunction<tOriginal>(this, 3)(this, obj);
}

std::vector<C_EconItem*> CSharedObjectTypeCache::GetEconItems()
{
	std::vector<C_EconItem*> ret;

	auto size = *reinterpret_cast<size_t*>(this + 0x18);

	auto data = *reinterpret_cast<uintptr_t**>(this + 0x4);

	for (size_t i = 0; i < size; i++)
		ret.push_back(reinterpret_cast<C_EconItem*>(data[i]));

	return ret;
}

void C_PlayerInventory::RemoveItem(uint64_t ID)
{
	static auto fnRemoveItem
		= reinterpret_cast<int(__thiscall*)(void*, int64_t)>(
			FindPattern(CLIENT_DLL, enc("55 8B EC 83 E4 F8 56 57 FF 75 0C 8B F1"))
			);

	fnRemoveItem(this, ID);
}

void C_PlayerInventory::RemoveItem(C_EconItem* item)
{
	RemoveItem(*item->GetItemID());
	GetBaseTypeCache()->RemoveObject(item);
}

void C_PlayerInventory::ClearInventory()
{
	auto BaseTypeCache = this->GetBaseTypeCache();
	auto items = BaseTypeCache->GetEconItems();
	for (auto item : items)
	{
		RemoveItem(*item->GetItemID());
		BaseTypeCache->RemoveObject(item);
	}
}

CSharedObjectTypeCache* C_PlayerInventory::GetBaseTypeCache()
{
	static auto fnGCSDK_CGCClient_FindSOCache
		= reinterpret_cast<uintptr_t(__thiscall*)(uintptr_t, uint64_t, uint64_t, bool)>(
			FindPattern(CLIENT_DLL, enc("55 8B EC 83 E4 F8 83 EC 1C 0F 10 45 08"))
			);

	static auto g_GCClientSystem = **reinterpret_cast<uintptr_t**>(FindPattern(CLIENT_DLL, enc("8B 0D ? ? ? ? 6A 00 83 EC 10")) + 0x2);
	auto SOCahce = fnGCSDK_CGCClient_FindSOCache(g_GCClientSystem + 0x60, *reinterpret_cast<uint64_t*>(this + 0x8), *reinterpret_cast<uint64_t*>(this + 0x10), 0);

	auto unk1 = *(DWORD*)(SOCahce + 0x1C);
	auto unk2 = *(DWORD*)(SOCahce + 0x10);
	auto type = 1;

	static auto fnGCSDK_CSharedObjectCache_FindBaseTypeCache = FindPattern(CLIENT_DLL, enc("55 8B EC 56 57 8B F2 8B F9"));
	auto unk3 = unk2 + 4 * unk1;
	auto t = 1;
	CSharedObjectTypeCache** _ret;
	__asm	push this;
	__asm	lea eax, [t];
	__asm	push eax;
	__asm	mov ecx, unk2;
	__asm	mov edx, unk3;
	__asm	call fnGCSDK_CSharedObjectCache_FindBaseTypeCache;
	__asm	mov _ret, eax;
	__asm	add esp, 8;
	return *_ret;
}

uint32_t C_PlayerInventory::GetSteamID()
{
	return *reinterpret_cast<uint32_t*>(this + 0x8);
}

bool C_PlayerInventory::AddEconItem(C_EconItem* item)
{
	static auto fnAddEconItem
		= reinterpret_cast<void*(__thiscall*)(void*, C_EconItem*, int, int, char)>(
			FindPattern("client.dll", "55 8B EC 83 E4 F8 A1 ? ? ? ? 83 EC 14 53 56 57 8B F9 8B 08")
			);

	GetBaseTypeCache()->AddObject(item);

	return fnAddEconItem(this, item, 1, 0, 1);
}

SOURCE_END