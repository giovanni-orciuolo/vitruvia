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

#include "C_CSPlayer.h"
#include <algorithm>

// Definition of C_CSPlayer utility functions

SOURCE_INIT

Vector C_CSPlayer::GetEyePosition()
{
	return this->m_vecOrigin() + this->m_vecViewOffset();
}

C_WeaponCSBase* C_CSPlayer::GetActiveWeapon()
{
	static offset_t m_hActiveWeapon = GET_NETVAR(enc("DT_BaseCombatCharacter"), enc("m_hActiveWeapon"));
	return (C_WeaponCSBase*)Source::EntityList->GetClientEntityFromHandle(*MAKE_PTR<Source::CHandle<C_WeaponCSBase>>((void*)this, m_hActiveWeapon));
}

std::vector<C_WeaponCSBase*> C_CSPlayer::GetWeapons()
{
	// NOTE: m_hMyWeapons is returning 0x5BD0 which is 0x2DE8 * 2, wtf?
	static offset_t m_hMyWeapons = GET_NETVAR(enc("DT_BaseCombatCharacter"), enc("m_hMyWeapons")) / 2; // <- Hotfix here to adjust the netvar found
	std::vector<C_WeaponCSBase*> weaps{};
	int i = 0;
	for (
		C_WeaponCSBase* weap = // Head of the list
		(C_WeaponCSBase*)Source::EntityList->GetClientEntityFromHandle(*MAKE_PTR<Source::CHandle<C_WeaponCSBase>>((void*)this, m_hMyWeapons));
		weap != nullptr;
		weap = (C_WeaponCSBase*)Source::EntityList->GetClientEntityFromHandle(*MAKE_PTR<Source::CHandle<C_WeaponCSBase>>((void*)this, m_hMyWeapons + (i * 0x4)))
		)
	{
		weaps.push_back(weap);
		i++;
	}
	return weaps;
}

std::vector<std::string> C_CSPlayer::GetWeaponNames()
{
	auto weapons = this->GetWeapons();
	if (weapons.empty()) return {};

	std::vector<std::string> weapon_names{};

	for (auto w : weapons)
	{
		weapon_names.push_back(w->GetDisplayName());
	}
	
	return weapon_names;
}

C_BaseViewModel* C_CSPlayer::GetViewModel()
{
	static offset_t m_hViewModel = GET_NETVAR(enc("DT_BasePlayer"), enc("m_hViewModel[0]"));
	return (C_BaseViewModel*)Source::EntityList->GetClientEntityFromHandle(*MAKE_PTR<Source::CHandle<C_BaseViewModel>>((void*)this, m_hViewModel));
}

bool C_CSPlayer::IsTargetingMe()
{
	// If this entity is the local player, it's useless to calculate shit, return false directly
	if (this == GetLocal())
		return false;

	Vector src, dst, forward;
	trace_t tr;
	Vector viewangle = this->m_angEyeAngles();

	Maths::AngleVectors(viewangle, &forward, nullptr, nullptr);
	forward *= 8192.f;
	src = this->GetEyePosition();
	dst = src + forward;

	Ray_t ray;
	ray.Init(src, dst);
	CTraceFilter filter;
	filter.pSkip = this;
	Source::EngineTrace->TraceRay(ray, MASK_SHOT, &filter, &tr);

	if (tr.m_pEnt == GetLocal())
		return true;

	return false;
}

Vector C_CSPlayer::GetBonePosition(int bone)
{
	matrix3x4_t bone_matrix[MAXSTUDIOBONES];
	if (this->GetRenderable()->SetupBones(bone_matrix, MAXSTUDIOBONES, 0x100, Source::Globals->cur_time))
	{
		return Vector(
			bone_matrix[bone][0][3],
			bone_matrix[bone][1][3],
			bone_matrix[bone][2][3]
		);
	}
	return Vector(0, 0, 0);
}

mstudiobbox_t* C_CSPlayer::GetStudioBox(int hitbox)
{
	if (hitbox < 0 || hitbox >= HITBOX_MAX)
		return nullptr;

	model_t* model = this->GetRenderable()->GetModel();
	if (model == nullptr) return nullptr;

	studiohdr_t* studiohdr = Source::ModelInfo->GetStudioModel(model);
	if (studiohdr == nullptr) return nullptr;

	mstudiohitboxset_t* hitboxset = studiohdr->pHitboxSet(this->m_nHitboxSet());
	if (hitboxset == nullptr) return nullptr;

	return hitboxset->pHitbox(hitbox);
}

int C_CSPlayer::GetBoneByHitbox(int hitbox)
{
	auto bbox = GetStudioBox(hitbox);
	if (bbox == nullptr) return -1;

	return bbox->bone;
}

Vector C_CSPlayer::GetHitboxPosition(int hitbox)
{
	auto bbox = GetStudioBox(hitbox);
	if (bbox == nullptr) return Vector(0, 0, 0);

	matrix3x4_t bone_matrix[MAXSTUDIOBONES];
	if (!this->GetRenderable()->SetupBones(bone_matrix, MAXSTUDIOBONES, 0x100, Source::Globals->cur_time))
		return Vector(0, 0, 0);

	Vector center = ((bbox->bbmin + bbox->bbmax) * .5f);

	return Maths::VectorTransform(center, bone_matrix[bbox->bone]);
}

Vector C_CSPlayer::GetHitboxPositionCustom(int hitbox, BoneMatrix customMatrix)
{
	auto bbox = GetStudioBox(hitbox);
	if (bbox == nullptr) return Vector(0, 0, 0);

	Vector center = ((bbox->bbmin + bbox->bbmax) * .5f);

	return Maths::VectorTransform(center, customMatrix[bbox->bone]);
}

std::string C_CSPlayer::GetHitboxName(int hitbox)
{
	// This always returns "". Gotta find out why... ?
	auto bbox = GetStudioBox(hitbox);
	if (bbox == nullptr) return "?";

	return std::string(bbox->szHitboxName());
}

int C_CSPlayer::GetHitgroupByHitbox(int hitbox)
{
	auto bbox = GetStudioBox(hitbox);
	if (bbox == nullptr) return -1;

	return bbox->group;
}

bool C_CSPlayer::IsBehindSmoke(int hitbox)
{
	typedef bool(*LineGoesThroughSmokeFn)(Vector start, Vector end, short);

	if (hitbox < 0 || hitbox >= HITBOX_MAX)
		return false;

	return rcast<LineGoesThroughSmokeFn>(Vitruvia::Offsets::LineGoesThroughSmoke)
		(GetLocal()->GetEyePosition(), this->GetBonePosition(this->GetBoneByHitbox(hitbox)), 1);
}

bool C_CSPlayer::IsVisible(int hitbox)
{
	if (hitbox < 0 || hitbox >= HITBOX_MAX)
		return false;

	auto local = GetLocal();
	if (local == this)
		return true;
	
	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;
	filter.pSkip = local;

	ray.Init(local->GetEyePosition(), this->GetHitboxPosition(hitbox));
	Source::EngineTrace->TraceRay(ray, MASK_VISIBLE, &filter, &tr);

	return ((C_CSPlayer*)tr.m_pEnt == this || tr.fraction > 0.99f);
}

bool C_CSPlayer::GotInvalidWeapon()
{
	// Grab the active weapon
	auto active_weapon = this->GetActiveWeapon();
	if (active_weapon == nullptr) 
		return true; // If the weapon is null it means that it's invalid
	
	// Grab info
	auto weap_info = active_weapon->GetWeaponInfo();
	if (weap_info == nullptr)
		return true; // If the info is null it means that it's invalid

	// Grab the type
	WeaponTypes type = weap_info->WeaponType;

	// Perform various checks on the type and on the idi to check for invalid weapons
	return (type == WeaponTypes::WEAPONTYPE_KNIFE ||
		type == WeaponTypes::WEAPONTYPE_C4 ||
		type == WeaponTypes::WEAPONTYPE_GRENADE ||
		type == WeaponTypes::WEAPONTYPE_UNKNOWN ||
		*active_weapon->m_iItemDefinitionIndex() == WEAPON_TASER);
}

player_info_t C_CSPlayer::GetPlayerInfo()
{
	player_info_t info;
	if (!Source::Engine->GetPlayerInfo(this->GetIndex(), &info))
		Err(enc("Inside entity #%d -> GetPlayerInfo() is unreliable for this tick"), 
			this->GetIndex());
	return info;
}

std::string C_CSPlayer::GetPlayerName()
{
	player_info_t info;
	if (!Source::Engine->GetPlayerInfo(this->GetIndex(), &info))
		return "???";
	return std::string(info.name);
}

bool C_CSPlayer::IsHLTV()
{
	player_info_t info;
	if (!Source::Engine->GetPlayerInfo(this->GetIndex(), &info))
		return true;
	return info.ishltv;
}

bool C_CSPlayer::IsFakePlayer()
{
	player_info_t info;
	if (!Source::Engine->GetPlayerInfo(this->GetIndex(), &info))
		return true;
	return info.fakeplayer;
}

int64_t C_CSPlayer::GetXuid()
{
	player_info_t info;
	if (!Source::Engine->GetPlayerInfo(this->GetIndex(), &info))
		return -1;
	return info.xuid;
}

int C_CSPlayer::GetUserId()
{
	player_info_t info;
	if (!Source::Engine->GetPlayerInfo(this->GetIndex(), &info))
		return -1;
	return info.m_nUserID;
}

MoveType_t C_CSPlayer::GetMoveType()
{
	static offset_t m_MoveType = GET_NETVAR(enc("DT_BaseEntity"), enc("m_nRenderMode")) + 1;
	return *MAKE_PTR<MoveType_t>(this, m_MoveType);
}

std::string C_CSPlayer::GetClanTag()
{
	static offset_t m_szClan = GET_NETVAR(enc("DT_CSPlayerResource"), enc("m_szClan"));
	char* shite = rcast<char*>(Source::PlayerResource + m_szClan + this->GetIndex() * 0x4);
	return std::string(shite);
}

float C_CSPlayer::GetHitchance(C_CSPlayer* entity_to_hit, QAngle my_angles)
{
	auto* Weapon = this->GetActiveWeapon();
	if (Weapon)
	{
		Vector Forward, Right, Up, Dir;
		Maths::AngleVectors(my_angles, &Forward, &Right, &Up);

		int SeedsHit = 0;
		Weapon->UpdateAccuracyPenalty();
		float wSpread = Weapon->GetSpread();
		float wCone = Weapon->GetCone();
		Vector myEyePos = this->GetEyePosition();

		for (int i = 0; i < 256; i++) {
			Maths::RandomSeed(i + 1);

			float A = Maths::RandomFloat(0.0f, 1.0f);
			float B = Maths::RandomFloat(0.f, 2.0f * M_PI);
			float C = Maths::RandomFloat(0.0f, 1.0f);
			float D = Maths::RandomFloat(0.f, 2.0f * M_PI);

			float Spread = A * wSpread;
			float Cone = C * wCone;

			Vector vSpread((cos(B) * Spread) + (cos(D) * Cone), (sin(B) * Spread) + (sin(D) * Cone), 0), vDir;

			vDir.x = Forward.x + (Right.x * vSpread.x) + (Up.x * vSpread.y);
			vDir.y = Forward.y + (Right.y * vSpread.x) + (Up.y * vSpread.y);
			vDir.z = Forward.z + (Right.z * vSpread.x) + (Up.z * vSpread.y);

			Vector TraceEnd = myEyePos + (vDir * Weapon->GetWeaponInfo()->flRange);

			CGameTrace tr;
			Ray_t ray;
			ray.Init(myEyePos, TraceEnd);
			CTraceFilterEntitiesOnly filter;
			filter.pSkip = this;

			Source::EngineTrace->ClipRayToEntity(ray, MASK_SHOT, rcast<IHandleEntity*>(entity_to_hit), &tr);

			if (tr.m_pEnt == entity_to_hit)
				SeedsHit++;
		}

		return ((SeedsHit / 256.f) * 100.f);
	}
	else return 0.f;
}

bool C_CSPlayer::IsValid()
{
	return this &&
		this->IsAlive() &&
		this != GetLocal() &&
		this->m_iTeamNum() != GetLocal()->m_iTeamNum() &&
		!this->GetNetworkable()->IsDormant();
}

SOURCE_END
