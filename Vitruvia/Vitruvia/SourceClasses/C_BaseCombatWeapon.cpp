/*
	____   ____.__  __                     .__        
	\   \ /   /|__|/  |________ __ _____  _|__|____   
	 \   Y   / |  \   __\_  __ \  |  \  \/ /  \__  \  
	  \     /  |  ||  |  |  | \/  |  /\   /|  |/ __ \_
	   \___/   |__||__|  |__|  |____/  \_/ |__(____  /
												   \/ 
	Premium Internal Multihack for Counter-Strike: Global Offensive
	Made with love by double v - 2017
*/

#include "C_BaseCombatWeapon.h"

SOURCE_INIT

const char* C_BaseCombatWeapon::GetName()
{
	return CallVFunction<const char*(__thiscall*)(void*)>(this, 378)(this);
}

int C_BaseCombatWeapon::GetWeaponId()
{
	return CallVFunction<int(__thiscall*)(void*)>(this, 458)(this);
}

bool C_BaseCombatWeapon::CanFire(int tick_base)
{
	float server_time = tick_base * Source::Globals->interval_per_tick;
	if (m_flNextPrimaryAttack() > server_time)
		return false;

	if (m_iClip1() <= 0)
		return false;

	return true;
}

WeaponInfo_t* C_BaseCombatWeapon::GetWeaponInfo()
{
	return CallVFunction<WeaponInfo_t*(__thiscall*)(void*)>(this, 447)(this);
}

float C_BaseCombatWeapon::GetSpread()
{
	return CallVFunction<float(__thiscall*)(void*)>(this, 439)(this);
}

float C_BaseCombatWeapon::GetCone()
{
	return CallVFunction<float(__thiscall*)(void*)>(this, 469)(this);
}

float C_BaseCombatWeapon::GetInaccuracy()
{
	return CallVFunction<float(__thiscall*)(void*)>(this, 470)(this);
}

void C_BaseCombatWeapon::UpdateAccuracyPenalty()
{
	return CallVFunction<void(__thiscall*)(void*)>(this, 471)(this);
}

void C_BaseCombatWeapon::SetModelIndex(int model_index)
{
	return CallVFunction<void(__thiscall*)(void*, int)>(this, 75)(this, model_index);
}

C_BaseAttributableItem* C_BaseCombatWeapon::ToAttributable()
{
	return rcast<C_BaseAttributableItem*>(this);
}

CEconItemDefinition* C_BaseCombatWeapon::GetEconItemView()
{
	// m_Item yields a valid pointer to CEconItemDefinition!
	// (FuckFace32 is a fucking genius)
	static int m_Item = GET_NETVAR(enc("DT_BaseAttributableItem"), enc("m_Item"));
	return MAKE_PTR<CEconItemDefinition>(this, m_Item);
}

std::string C_BaseCombatWeapon::GetDisplayName(bool name_tag)
{
	// This is the function we are going to call from the internal class
	typedef wchar_t*(__thiscall* GetItemName_t)(void*, bool);
	// Now we get it, GetItemName() calls ILocalize wrappers so it does all the hard work for us
	static GetItemName_t get_item_name = rcast<GetItemName_t>(Vitruvia::Offsets::GetItemName);
	// true if you don't want nametags false if you want nametags when they exist
	auto item = this->GetEconItemView();
	if (item)
		return WS2S(std::wstring(get_item_name(item, !name_tag)));

	return enc("");
}

void C_BaseCombatWeapon::ForceUpdate()
{
	auto view = this->GetEconItemView();
	if (view)
	{

	}
}

SOURCE_END