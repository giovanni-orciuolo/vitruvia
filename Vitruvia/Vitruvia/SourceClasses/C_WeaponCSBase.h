// Class built with love by Vitruvia :*
// Build date: Friday 7/4/2017

#pragma once
#include "C_BaseCombatWeapon.h"

SOURCE_INIT

//std::unordered_map<int, float> DeltaMap
//{
//	{ WEAPON_DEAGLE,	0.21f },
//	{ WEAPON_ELITE,		0.10f },
//	{ WEAPON_FIVESEVEN, 0.13f },
//	{ WEAPON_GLOCK,		0.13f },
//	{ WEAPON_AK47,		0.08f },
//	{ WEAPON_AUG,		0.07f },
//	{ WEAPON_AWP,		1.44f },
//	{ WEAPON_FAMAS,		0.07f },
//	{ WEAPON_G3SG1,		0.24f },
//	{ WEAPON_GALILAR,   0.08f },
//	{ WEAPON_M249,		0.06f },
//	{ WEAPON_M4A4,		0.08f },
//	{ WEAPON_MAC10,		0.07f },
//	{ WEAPON_P90,		0.06f },
//	{ WEAPON_UMP45,		0.08f },
//	{ WEAPON_XM1014,	0.34f },
//	{ WEAPON_BIZON,		0.07f },
//	{ WEAPON_MAG7,		0.84f },
//	{ WEAPON_NEGEV,		0.06f },
//	{ WEAPON_SAWEDOFF,	0.84f },
//	{ WEAPON_TEC9,		0.11f },
//	{ WEAPON_HKP2000,	0.16f },
//	{ WEAPON_MP7,		0.07f },
//	{ WEAPON_MP9,		0.06f },
//	{ WEAPON_NOVA,		0.87f },
//	{ WEAPON_P250,		0.06f },
//	{  },
//	{},
//	{},
//};

class C_WeaponCSBase : public C_BaseCombatWeapon
{
public:
	IClientRenderable* GetRenderable() { return MAKE_PTR<IClientRenderable>(this, 0x4); }
	IClientNetworkable* GetNetworkable() { return MAKE_PTR<IClientNetworkable>(this, 0x8); }

	CUtlVector<IRefCounted*>* GetCustomMaterials()
	{
		return MAKE_PTR<CUtlVector<IRefCounted*>>(this, Vitruvia::Offsets::CustomMaterials);
	}
	bool AreCustomMaterialsInitialized()
	{
		return *MAKE_PTR<bool>(this, Vitruvia::Offsets::CustomMaterialInitialized);
	}

	float GetPostponeFireReadyTime()
	{
		// Had to wrap it in a function since apparently the netvar returns some weird infinite number.
		// (wxx "figured it out" but doesn't explain what the mistake is)
		// Lesson is: don't blindly trust the netvar manager. And vv. Neither of them.
		static offset_t m_flPostponeFireReadyTime = GET_NETVAR(enc("DT_WeaponCSBase"), enc("m_flPostponeFireReadyTime"));
		return *MAKE_PTR<float>(this, m_flPostponeFireReadyTime);
	}

	float GetNextAttackDelta()
	{
		// Based on the data that I log
		// Only do this if this ain't an invalid weapon dude!
		if (!(GetWeaponInfo()->WeaponType != WeaponTypes::WEAPONTYPE_KNIFE) &&
			!(GetWeaponInfo()->WeaponType != WeaponTypes::WEAPONTYPE_GRENADE) &&
			!(GetWeaponInfo()->WeaponType != WeaponTypes::WEAPONTYPE_C4) &&
			!(GetWeaponInfo()->WeaponType != WeaponTypes::WEAPONTYPE_UNKNOWN) &&
			!(*m_iItemDefinitionIndex() == WEAPON_TASER))
		{
			return 0.f;
		}
		return 0.f;
	}

	MAKE_NETVAR(int, m_weaponMode, "m_weaponMode", "DT_WeaponCSBase")
	MAKE_NETVAR(float, m_fAccuracyPenalty, "m_fAccuracyPenalty", "DT_WeaponCSBase")
	MAKE_NETVAR(float, m_fLastShotTime, "m_fLastShotTime", "DT_WeaponCSBase")
	MAKE_NETVAR(int, m_iRecoilIndex, "m_iRecoilIndex", "DT_WeaponCSBase")
	MAKE_NETVAR(float, m_flRecoilIndex, "m_flRecoilIndex", "DT_WeaponCSBase")
	MAKE_NETVAR(EHANDLE, m_hPrevOwner, "m_hPrevOwner", "DT_WeaponCSBase")
	MAKE_NETVAR(bool, m_bBurstMode, "m_bBurstMode", "DT_WeaponCSBase")
	//MAKE_NETVAR(float, m_flPostponeFireReadyTime, "m_flPostponeFireReadyTime", "DT_WeaponCSBase")
	MAKE_NETVAR(bool, m_bReloadVisuallyComplete, "m_bReloadVisuallyComplete", "DT_WeaponCSBase")
	MAKE_NETVAR(bool, m_bSilencerOn, "m_bSilencerOn", "DT_WeaponCSBase")
	MAKE_NETVAR(float, m_flDoneSwitchingSilencer, "m_flDoneSwitchingSilencer", "DT_WeaponCSBase")
	MAKE_NETVAR(int, m_iOriginalTeamNumber, "m_iOriginalTeamNumber", "DT_WeaponCSBase")
	MAKE_NETVAR(int, m_iIronSightMode, "m_iIronSightMode", "DT_WeaponCSBase")
};

SOURCE_END
