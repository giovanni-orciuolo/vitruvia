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

#pragma once

#include "../Vitruvia.h"
#include "../SourceClasses/C_CSPlayer.h"
#include "../Globals/VitruviaGlobals.h"

#define Autowall CAutowall::GetInstance()

// =========================================
inline bool IsSolid(Source::SolidType_t solidType, int nSolidFlags) {
	return (solidType != Source::SOLID_NONE) && ((nSolidFlags & Source::FSOLID_NOT_SOLID) == 0);
}
bool IsEntityBreakable(Source::IHandleEntity* ent);
bool StandardFilterRules(Source::IHandleEntity* pHandleEntity, int fContentsMask, int collisionGroup);
bool PassServerEntityFilter(const Source::IHandleEntity *pTouch, const Source::IHandleEntity *pPass);
bool BaseShouldHitEntity(Source::IHandleEntity* pSkip, Source::IHandleEntity *pHandleEntity, int m_collisionGroup, int contentsMask);
bool GameRulesShouldCollide(int collisionGroup0, int collisionGroup1);
// =========================================

// =========================================
namespace Source
{
	class CTraceFilterSkipTwoEntities : public CTraceFilter
	{
	public:
		CTraceFilterSkipTwoEntities(C_BaseEntity *pPassEnt1, C_BaseEntity *pPassEnt2, int collisionGroup, TraceType traceType = TraceType::TRACE_EVERYTHING) {
			pSkip = pPassEnt1;
			pSkip2 = pPassEnt2;
			m_iCollisionGroup = collisionGroup;
			traceType = m_TraceType;
		};
		virtual bool ShouldHitEntity(C_BaseEntity *pEntityHandle, int contentsMask) {
			return !(pEntityHandle == pSkip || pEntityHandle == pSkip2);
			if (!PassServerEntityFilter((IHandleEntity*)pEntityHandle, (IHandleEntity*)pSkip2))
				return false;
			return BaseShouldHitEntity((IHandleEntity*)pEntityHandle, (IHandleEntity*)pSkip, m_iCollisionGroup, contentsMask);
		}
		virtual TraceType GetTraceType() const
		{
			return m_TraceType;
		}
		TraceType m_TraceType;
		int m_iCollisionGroup;
		const Source::C_BaseEntity *pSkip2;
	};
}
// =========================================

namespace Vitruvia
{
	namespace Tools
	{
		class CAutowall
		{
			IMPLEMENT_SINGLETON(CAutowall);
		public:
			struct OverrideData
			{
				OverrideData(int hitgroup, const Source::Vector& position, Source::C_CSPlayer* override_ent)
					: hitgroup(hitgroup),
					  position(position),
					  entity(override_ent)
				{
				}
				OverrideData()
					: hitgroup(0), entity(nullptr)
				{}

				int hitgroup;
				Source::Vector position;
				Source::C_CSPlayer* entity;
			};

			float GetDamage(const Source::Vector& start_pos, 
				const Source::QAngle& angles, 
				Source::C_CSPlayer* player, 
				Source::Hitboxes hbox,
				int& hitgroup, bool& didPenetrate,
				bool filterTeamMates,
				bool wantsOverride, OverrideData od);
		private:
			float GetHitgroupDamageMultiplier(int hitgroup);
			void ScaleDamage(int hitgroup, float armor_value, bool has_helmet, float weapon_armor_ratio, float& current_damage);
			Source::C_CSPlayer* Run(const Source::Vector& start_pos, 
				const Source::QAngle& angles, 
				Source::C_CSPlayer* player, 
				float& damage, 
				int& hitgroup, 
				bool& didPenetrate, 
				Source::Hitboxes hbox,
				bool filterTeamMates,
				bool wantsOverride, OverrideData od);
		};
	}
}