#include "C_EconItem.h"
#include "C_BasePlayer.h"

SOURCE_INIT

uint32_t* C_EconItem::GetAccountID()
{
	return reinterpret_cast<uint32_t*>(this + 0x1C);
}
uint64_t* C_EconItem::GetItemID()
{
	return reinterpret_cast<uint64_t*>(this + 0x8);
}

uint64_t* C_EconItem::GetOriginalID()
{
	return reinterpret_cast<uint64_t*>(this + 0x10);
}

uint16_t* C_EconItem::GetDefIndex()
{
	return reinterpret_cast<uint16_t*>(this + 0x24);
}

uint32_t* C_EconItem::GetInventory()
{
	return reinterpret_cast<uint32_t*>(this + 0x20);
}

unsigned char* C_EconItem::GetFlags()
{
	return reinterpret_cast<unsigned char*>(this + 0x30);
}

unsigned short* C_EconItem::GetEconItemData()
{
	return reinterpret_cast<unsigned short*>(this + 0x26);
}

void C_EconItem::UpdateEquippedState(unsigned int state)
{
	static auto fnUpdateEquippedState
		= reinterpret_cast<int(__thiscall*)(C_EconItem*, unsigned int)>(
			FindPattern(CLIENT_DLL, enc("55 8B EC 8B 45 08 8B D0 C1 EA 10"))
			);

	fnUpdateEquippedState(this, state);
}

void C_EconItem::EquipTT()
{
	UpdateEquippedState(393218);
}

void C_EconItem::EquipCT()
{
	UpdateEquippedState(393219);
}

void C_EconItem::EquipAll()
{
	EquipTT();
	EquipCT();
}

void C_EconItem::AddSticker(int index, int kit, float wear, float scale, float rotation)
{
	SetAttributeValue<int>(113 + 4 * index, kit);
	SetAttributeValue<float>(114 + 4 * index, wear);
	SetAttributeValue<float>(115 + 4 * index, scale);
	SetAttributeValue<float>(116 + 4 * index, rotation);
}

void C_EconItem::SetStatTrak(int val)
{
	SetAttributeValue<int>(80, 150);
	SetAttributeValue<int>(81, 0);
	SetQuality(ITEM_QUALITY_STRANGE);
}

void C_EconItem::SetPaintKit(float kit)
{
	SetAttributeValue<float>(6, kit);
}

void C_EconItem::SetPaintSeed(float seed)
{
	SetAttributeValue<float>(7, seed);
}

void C_EconItem::SetPaintWear(float wear)
{
	SetAttributeValue<float>(8, wear);
}

void C_EconItem::SetQuality(ItemQuality quality)
{
	auto data = *GetEconItemData();
	*GetEconItemData() = data ^ (data ^ 32 * quality) & 0x1E0;
}

void C_EconItem::SetRarity(ItemRarity rarity)
{
	auto data = *GetEconItemData();
	*GetEconItemData() = (data ^ (rarity << 11)) & 0x7800 ^ data;
}

void C_EconItem::SetOrigin(int origin)
{
	auto data = *GetEconItemData();
	*GetEconItemData() = data ^ ((unsigned __int8)data ^ (unsigned __int8)origin) & 0x1F;
}

void C_EconItem::SetLevel(int level)
{
	auto data = *GetEconItemData();
	*GetEconItemData() = data ^ (data ^ (level << 9)) & 0x600;
}

void C_EconItem::SetInUse(bool in_use)
{
	auto data = *GetEconItemData();
	*GetEconItemData() = data & 0x7FFF | (in_use << 15);
}

void C_EconItem::SetCustomName(const char* name)
{
	static auto Address = FindPattern(CLIENT_DLL, enc("E8 ? ? ? ? 8B 46 78 C1 E8 0A A8 01 74 13 8B 46 34"));
	static auto fnSetCustomName
		= reinterpret_cast<C_EconItem*(__thiscall*)(void*, const char*)>(
			*reinterpret_cast<uintptr_t*>(Address + 1) + Address + 5
			);

	fnSetCustomName(this, name);
}

void C_EconItem::SetCustomDesc(const char* name)
{
	static auto Address = FindPattern(CLIENT_DLL, enc("E8 ? ? ? ? 33 DB 39 5E 3C 7E 5E"));
	static auto fnSetCustomDesc
		= reinterpret_cast<C_EconItem*(__thiscall*)(void*, const char*)>(
			*reinterpret_cast<uintptr_t*>(Address + 1) + Address + 5
			);

	fnSetCustomDesc(this, name);
}

SOURCE_END