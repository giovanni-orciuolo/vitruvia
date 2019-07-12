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

#include "Autowall.h"
using namespace Source;

// Will help through autowall functions! Those aren't commented because they are mostly rebuilt + used only inside CAutowall
void FixTraceRay(Vector end, Vector start, trace_t* oldtrace, C_CSPlayer* ent) 
{
	auto mins = ent->GetCollideable()->OBBMins();
	auto maxs = ent->GetCollideable()->OBBMaxs();

	auto center = (maxs + mins) * 0.5f;
	auto origin = ent->GetAbsOrigin();

	auto v15(center + origin);

	auto delta(end - start);
	delta.Normalize();

	auto v20 = (v15 - start);
	auto v22 = delta.Dot(v20);

	float v23;

	if (v22 >= 0.f) {
		if (v22 <= delta.Length()) {
			auto s(v15 - ((delta * v22) + start));
			v23 = s.Length();
		}

		else {
			auto b(v15 - end);
			v23 = -b.Length();
		}
	}

	else
		v23 = -v20.Length();

	if (v23 <= 60.f) {
		Ray_t ray;
		ray.Init(start, end);

		trace_t trace;
		Source::EngineTrace->ClipRayToEntity(ray, 0x4600400B, (IHandleEntity*)ent, &trace);

		if (oldtrace->fraction > trace.fraction)
			*oldtrace = trace;
	}
}
bool DidHitNonWorldEntity(C_BaseEntity* m_pEnt) 
{
	return m_pEnt != NULL && m_pEnt != C_World::GetWorld();
}
bool TraceToExitRebuilt(Vector& end, trace_t& enter_trace, Vector start, Vector dir, trace_t *exit_trace) 
{
	float distance = 0.0f;

	while (distance <= 90.0f) {
		distance += 4.0f;
		end = start + dir * distance;

		int point_contents = Source::EngineTrace->GetPointContents(end, MASK_SHOT_HULL | CONTENTS_HITBOX, NULL);

		if (point_contents & MASK_SHOT_HULL && (!(point_contents & CONTENTS_HITBOX)))
			continue;

		auto new_end = end - (dir * 4.0f);

		Ray_t ray;
		ray.Init(end, new_end);
		Source::EngineTrace->TraceRay(ray, MASK_SHOT, 0, exit_trace);

		if (exit_trace->startsolid && exit_trace->surface.flags & SURF_HITBOX) {
			ray.Init(end, start);
			CTraceFilter filter;
			filter.pSkip = (IHandleEntity*)exit_trace->m_pEnt;
			Source::EngineTrace->TraceRay(ray, 0x600400B, &filter, exit_trace);

			if ((exit_trace->fraction < 1.0f || exit_trace->allsolid) && !exit_trace->startsolid) {
				end = exit_trace->endpos;
				return true;
			}
			continue;
		}

		if (!(exit_trace->fraction < 1.0 || exit_trace->allsolid || exit_trace->startsolid) || exit_trace->startsolid) {
			if (exit_trace->m_pEnt) {
				if (DidHitNonWorldEntity(enter_trace.m_pEnt))
					return true;
			}
			continue;
		}

		if (((exit_trace->surface.flags >> 7) & 1) && !((enter_trace.surface.flags >> 7) & 1))
			continue;

		if (exit_trace->plane.normal.Dot(dir) <= 1.0f) {
			float fraction = exit_trace->fraction * 4.0f;
			end = end - (dir * fraction);
			return true;
		}
	}
	return false;
}
bool HandleBreakablePenetration(trace_t& enter_trace, Vector* vecDir, int enter_material, float flPenetrationModifier, float flPenetration, Vector& result, float* bulletdamage) 
{
	Vector end;
	trace_t trace_exit;
	if (!TraceToExitRebuilt(end, enter_trace, enter_trace.endpos, *vecDir, &trace_exit))
		return true;

	float damage = *bulletdamage;
	surfacedata_t* exit_surface_data = Source::Physics->GetSurfaceData(trace_exit.surface.surfaceProps);
	auto exit_material = exit_surface_data->game.material;

	float exit_surf_penetration_mod = exit_surface_data->game.penetrationmodifier;
	float final_damage_modifier = 0.16f;
	float combined_penetration_modifier = 0.0f;

	if (((enter_trace.contents & CONTENTS_GRATE) != 0) || (enter_material == 89) || (enter_material == 71))//|| (enter_material == 70))
	{
		combined_penetration_modifier = 3.0f;
		final_damage_modifier = 0.05f;
	}
	else {
		combined_penetration_modifier = (flPenetrationModifier + exit_surf_penetration_mod) * 0.5f;
	}

	if (enter_material == exit_material) {
		if (exit_material == 87 || exit_material == 85)
			combined_penetration_modifier = 3.0f;
		else if (exit_material == 76)
			combined_penetration_modifier = 2.0f;
	}

	float v34 = fmaxf(0.f, 1.0f / combined_penetration_modifier);
	float v35 = (damage * final_damage_modifier) + v34 * 3.0f * fmaxf(0.0f, (3.0f / flPenetration) * 1.25f);
	float thickness = (trace_exit.endpos - enter_trace.endpos).Length();

	thickness *= thickness;
	thickness *= v34;
	thickness /= 24.0f;

	float lost_damage = fmaxf(0.0f, v35 + thickness);

	if (lost_damage > damage)
		return true;

	if (lost_damage >= 0.0f)
		*bulletdamage -= lost_damage;

	if (*bulletdamage < 1.0f)
		return true;

	result = trace_exit.endpos;
	return false;
}
bool WallIsTooThick(Vector& start, Vector& dir, float flMaxDistance) {
	float flDistance = 0;
	Vector end;

	while (flDistance <= flMaxDistance) {
		flDistance += 24.0f;

		end = start + (dir * flDistance);

		if ((Source::EngineTrace->GetPointContents(end) & MASK_SOLID) == 0) {
			return false;
		}
	}

	return true;
}
// =========================================
bool IsEntityBreakable(IHandleEntity* pBaseEntity) {
	using IsBreakableEntityFn = bool(__thiscall*)(IHandleEntity*);
	static IsBreakableEntityFn fnIsBreakableEntity = reinterpret_cast<IsBreakableEntityFn>(Vitruvia::Offsets::IsEntityBreakable);

	return fnIsBreakableEntity(pBaseEntity);
}
bool StandardFilterRules(IHandleEntity* pHandleEntity, int fContentsMask, int collisionGroup) {
	C_BaseEntity* pCollide = dynamic_cast<C_BaseEntity*>(pHandleEntity);

	if (!pCollide)
		return true;

	const SolidType_t solid = pCollide->GetSolid();
	const model_t* pModel = pCollide->GetRenderable()->GetModel();
	auto mod = Source::ModelInfo->GetModelType(pModel);

	if ((mod != mod_brush) || (solid != SOLID_BSP && solid != SOLID_VPHYSICS)) {
		if ((fContentsMask & CONTENTS_MONSTER) == 0)
			return false;
	}

	if (!(fContentsMask & CONTENTS_WINDOW) && pCollide->IsTransparent())
		return false;

	if (!(fContentsMask & CONTENTS_MOVEABLE) && (rcast<C_CSPlayer*>(pCollide)->GetMoveType() == MOVETYPE_PUSH))// !(touch->flags & FL_WORLDBRUSH) )
		return false;

	return true;
}
bool PassServerEntityFilter(const IHandleEntity *pTouch, const IHandleEntity *pPass) {
	if (!pPass)
		return true;

	if (pTouch == pPass)
		return false;

	C_BaseEntity *pEntTouch = (C_BaseEntity*)pTouch;
	C_BaseEntity *pEntPass = (C_BaseEntity*)pPass;
	if (!pEntTouch || !pEntPass)
		return true;

	if (pEntTouch->GetOwnerEntity() == (IClientEntity*)pEntPass)
		return false;

	if (pEntPass->GetOwnerEntity() == (IClientEntity*)pEntTouch)
		return false;

	return true;
}
bool GameRulesShouldCollide(int collisionGroup0, int collisionGroup1) {
	uintptr_t shouldcollide = reinterpret_cast<uintptr_t>(Source::GameRules);
	__asm {
		push collisionGroup1
		push collisionGroup0
		mov ecx, shouldcollide
		mov ecx, [ecx]
		mov eax, [ecx]
		mov eax, [eax + 0x70]
		call eax
	}
}
bool BaseShouldHitEntity(IHandleEntity* pSkip, IHandleEntity* pHandleEntity, int m_collisionGroup, int contentsMask) {
	if (!StandardFilterRules(pHandleEntity, contentsMask, m_collisionGroup))
		return false;

	if (pSkip) {
		if (!PassServerEntityFilter(pHandleEntity, pSkip)) {
			return false;
		}
	}

	C_BaseEntity* pEntity = dynamic_cast<C_BaseEntity*>(pHandleEntity);//EntityFromEntityHandle(pHandleEntity);
	if (!pEntity)
		return false;
	if (!pEntity->ShouldCollide(m_collisionGroup, contentsMask))
		return false;
	if (!GameRulesShouldCollide(m_collisionGroup, pEntity->m_CollisionGroup()))
		return false;

	return true;
}
// =========================================

namespace Vitruvia
{
	namespace Tools
	{
		/*
		* Returns the damage calculated in a specific point
		* 
		* @param start_pos Starting position of the penetration ray
		* @param angles View angles to account for
		* @param player Runner of the autowall (usually local player)
		* @param hbox If this is head, traceray is fixed internally
		* @param OUT hitgroup Hitgroup that got hit by the penetration ray
		* @param OUT didPenetrate True if the autowall did actually penetrate through something
		* @param filterTeamMates True if you want to check for teammates
		* @param wantsOverride True if you want to override values
		* @param od Override data
		* @returns: The damage calculated
		*/
		float CAutowall::GetDamage(const Source::Vector& start_pos,
			const Source::QAngle& angles, 
			Source::C_CSPlayer* player, 
			Source::Hitboxes hbox,
			int& hitgroup, bool& didPenetrate,
			bool filterTeamMates,
			bool wantsOverride, OverrideData od)
		{
			float damage = 0.f;
			C_CSPlayer* returnedPlayer = this->Run(
				start_pos, angles, player, damage, hitgroup, didPenetrate, hbox, filterTeamMates, wantsOverride, od
			);
			if (returnedPlayer == nullptr) 
				return 0.f;
			return damage;
		}
		/*
		* Returns the damage multiplier for a specific hitgroup
		* hitgroup (int) -> Valid hitgroup ID to account for
		* @returns: The multiplier (constant)
		*/
		float CAutowall::GetHitgroupDamageMultiplier(int hitgroup)
		{
			switch (hitgroup) 
			{
			case HITGROUP_HEAD:
				return 4.0f;
			case HITGROUP_STOMACH:
				return 1.25f;
			case HITGROUP_LEFTLEG:
			case HITGROUP_RIGHTLEG:
				return 0.75f;
			case HITGROUP_CHEST:
			case HITGROUP_LEFTARM:
			case HITGROUP_RIGHTARM:
			default: return 1.0f;
			}
		}
		/*
		* Scales the damage on the player (based on armor and hitgroup multiplier)
		* hitgroup (int) -> Valid hitgroup ID to account for (used to get multiplier)
		* armor_value -> armor value
		* has_helmet -> has helmet
		* weapon_armor_ratio -> You get this value from local weapon info
		* current_damage -> This becomes the scaled damage [OUT]
		* @returns: void
		*/
		void CAutowall::ScaleDamage(int hitgroup, float armor_value, bool has_helmet, float weapon_armor_ratio, float& current_damage)
		{
			current_damage *= GetHitgroupDamageMultiplier(hitgroup);
			if (armor_value > 0.0f && hitgroup < HITGROUP_LEFTLEG) // Don't do gear, or legs
			{
				if (hitgroup == HITGROUP_HEAD && !has_helmet)
					return;

				float armorscaled = (weapon_armor_ratio * 0.5f) * current_damage;
				if ((current_damage - armorscaled) * 0.5f > armor_value)
					armorscaled = current_damage - (armor_value * 2.0f);
				current_damage = armorscaled;
			}
		}
		/*
		* Internally runs autowall
		* 
		* @param start_pos Starting position of the penetration ray
		* @param angles View angles to account for
		* @param player Runner of the autowall (usually local player)
		* @param damage OUT Damage calculated
		* @param OUT hitgroup Hitgroup that got hit by the penetration ray
		* @param OUT didPenetrate True if the autowall did actually penetrate through something
		* @param hbox If this is head, traceray is fixed internally
		* @param filterTeamMates True if you want to check for teammates
		* @param wantsOverride True if you want to override values
		* @param od Override data
		* @returns: Pointer to player that autowall found
		*/
		Source::C_CSPlayer* CAutowall::Run(const Source::Vector& start_pos, 
			const Source::QAngle& angles, 
			Source::C_CSPlayer* player, 
			float& damage, 
			int& hitgroup, 
			bool& didPenetrate,
			Source::Hitboxes hbox,
			bool filterTeamMates,
			bool wantsOverride, OverrideData od)
		{
			static auto HandleBulletPenetration = reinterpret_cast<bool(__thiscall*)(C_BaseEntity*, float&, int &, char*, trace_t*, Vector*,
				float, float, float, int, int, float, int*, Vector*, float, float, float*)>(Vitruvia::Offsets::HandleBulletPenetration);

			static auto UTIL_ClipTraceToPlayers = [](const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr) {
				static uintptr_t clptrtp = Vitruvia::Offsets::ClipTraceToPlayers;
				__asm {
					push tr
					push filter
					push mask
					lea edx, vecAbsEnd
					lea ecx, vecAbsStart
					call clptrtp
					add esp, 0xC
				}
			};

			static auto UTIL_TraceLineIgnoreTwoEntities = [](const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, C_BaseEntity* ignore, C_BaseEntity* ignore2, int collisionGroup, trace_t* ptr) {
				Ray_t ray;
				ray.Init(vecAbsStart, vecAbsEnd);
				CTraceFilterSkipTwoEntities traceFilter(ignore, ignore2, collisionGroup);
				Source::EngineTrace->TraceRay(ray, mask, &traceFilter, ptr);
			};

			if (player == nullptr)
				return nullptr;

			auto* weapon = player->GetActiveWeapon();
			if (weapon == nullptr)
				return nullptr;

			auto* wpn_data = weapon->GetWeaponInfo();
			if (wpn_data == nullptr)
				return nullptr;

			const float RAY_EXTENSION = 40.0f;
			didPenetrate = false;
			trace_t enter_tr;
			Vector vecSrc = start_pos;
			Vector result = vecSrc;
			Vector vecDir; Maths::AngleVectors(angles, &vecDir);

			int hitsLeft = 4;
			C_BaseEntity* lastEntityHit = nullptr;
			float wpnPenetration = wpn_data->flPenetration;
			float bulletDmg = static_cast<float>(wpn_data->iDamage);
			float tempDmg = bulletDmg;
			float distance = wpn_data->flRange;
			float traveledDistance = 0.0f;

			while (bulletDmg > 0.0f)
			{
				distance -= traveledDistance;
				Vector endPos = vecSrc + vecDir * distance;

				UTIL_TraceLineIgnoreTwoEntities(result, 
					endPos, 
					MASK_SHOT, 
					player,
					lastEntityHit, 
					COLLISION_GROUP_NONE, 
					&enter_tr);

				//if (player != nullptr && hbox == HITBOX_HEAD) {
					//FixTraceRay(endPos + vecDir * RAY_EXTENSION, vecSrc, &enter_tr, player);
				//} else {
					CTraceFilterSkipTwoEntities filter(player, lastEntityHit, COLLISION_GROUP_NONE);
					UTIL_ClipTraceToPlayers(vecSrc, endPos + vecDir * RAY_EXTENSION, 0x4600400B, &filter, &enter_tr);
				//}

				surfacedata_t* entersurf = Source::Physics->GetSurfaceData(enter_tr.surface.surfaceProps);
				float penetrationModifier = entersurf->game.penetrationmodifier;
				traveledDistance += enter_tr.fraction * distance;
				//bulletDmg *= powf(wpn_data->m_flRangeModifier, traveledDistance / 500);
				bulletDmg = powf(wpn_data->flRangeModifier, traveledDistance / 500) * tempDmg;

				if (!wantsOverride && enter_tr.hitgroup != 0 && enter_tr.m_pEnt != nullptr)
				{
					// An entity has been found, just break if I want to filter for teammates and he's a teammate
					if (filterTeamMates && enter_tr.m_pEnt->m_iTeamNum() == player->m_iTeamNum())
						break;

					// I've found an entity behind the walls
					// Scale the damage based on armor, etc...
					C_CSPlayer* trace_entity = rcast<C_CSPlayer*>(enter_tr.m_pEnt);
					ScaleDamage(enter_tr.hitgroup, trace_entity->m_ArmorValue(), trace_entity->m_bHasHelmet(), wpn_data->flArmorRatio, bulletDmg);
					// Set output values
					damage = bulletDmg;
					hitgroup = enter_tr.hitgroup;
					didPenetrate = !(hitsLeft == 4);
					// Return the pointer to the entity that I've found
					return (C_CSPlayer*)enter_tr.m_pEnt;
				}

				if (wantsOverride && enter_tr.hitgroup != 0 && enter_tr.m_pEnt != nullptr)
				{
					if (enter_tr.endpos.DistTo(od.position) < 5)
					{
						// It is close enough, override
						ScaleDamage(enter_tr.hitgroup, od.entity->m_ArmorValue(), od.entity->m_bHasHelmet(), wpn_data->flArmorRatio, bulletDmg);
						// Set output values
						damage = bulletDmg;
						hitgroup = enter_tr.hitgroup;
						didPenetrate = hitsLeft != 4;
						// Return the pointer to the override entity
						return od.entity;
					}
				}

				lastEntityHit = enter_tr.m_pEnt;
				int material = entersurf->game.material;
				char flags = (enter_tr.contents >> 3) & CONTENTS_SOLID;

				// I will signal TraceRay hook that I'm going to call HBP in some way, so I can avoid effect spam
				global->set_UsingBulletHandler(true);
				// Handle windows and breakable contents
				if (enter_tr.contents & CONTENTS_WINDOW || IsEntityBreakable(rcast<IHandleEntity*>(enter_tr.m_pEnt))) {
					if (HandleBreakablePenetration(enter_tr, 
						&vecDir, 
						material, 
						penetrationModifier, 
						wpnPenetration, 
						result, 
						&bulletDmg)) 
					{
						// HBP returned true, it means I can't penetrate
						break;
					}
				}
				// Handle walls and normal content
				else if (HandleBulletPenetration(player, 
					wpnPenetration, 
					material, 
					&flags,
					&enter_tr, 
					&vecDir, 
					0.f, 
					penetrationModifier,
					entersurf->game.damagemodifier, 
					0, 
					0x1002, 
					wpnPenetration, 
					&hitsLeft, 
					&result, 
					0.f, 
					0.f, 
					&bulletDmg)) 
				{
					// HBP returned true, it means I can't penetrate
					break;
				}
				// I'm done with HBP
				global->set_UsingBulletHandler(false);

				tempDmg = bulletDmg;
			}

			// If I'm here it means that the while loop got interrupted before I could return
			// Set output values and return nullptr
			global->set_UsingBulletHandler(false);
			didPenetrate = hitsLeft != 4;
			hitgroup = 0;
			damage = 0.f;
			return nullptr;
		}
	}
}