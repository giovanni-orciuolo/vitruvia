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

// Credits: pycache bae <3 (modified)

#pragma once

#include "GrenadePrediction.h"
using namespace Source;

#define CD3D(clr) ImU32(clr.toD3D())

enum acts 
{
	ACT_NONE = 0,
	ACT_THROW,
	ACT_LOB,
	ACT_DROP,
};

int GrenadeType = 0;
acts Act = ACT_NONE;
std::vector<Source::Vector> Path;
static int LastSimulateTick = 0;

inline float CalculateArmorDamage(float damage, int armor_value) 
{
	static const float armorRatio = 0.5f, armorBonus = 0.5f;
	if (armor_value > 0) 
	{
		float newDamage = damage * armorRatio, armor = (damage - newDamage) * armorBonus;
		if (armor > scast<float>(armor_value)) {
			armor = scast<float>(armor_value) * (1.f / armorBonus);
			newDamage = damage - armor;
		}
		damage = newDamage;
	}
	return damage;
}

namespace Vitruvia
{
	namespace Tools
	{
		// -----------------
		// Should be called on each tick, from CreateMove
		// -----------------
		void CGrenadePrediction::Tick(int buttons)
		{
			bool in_attack = (buttons & IN_ATTACK);
			bool in_attack2 = (buttons & IN_ATTACK2);

			Act =	(in_attack && in_attack2) ? ACT_LOB :
					(in_attack2) ? ACT_DROP :
					(in_attack) ? ACT_THROW :
					ACT_NONE;
		}
		// -----------------
		// Prediction behaviour
		// -----------------
		void CGrenadePrediction::PredictGrenade()
		{
			auto local = C_CSPlayer::GetLocal();
			if (local)
			{
				C_WeaponCSBase* weapon = local->GetActiveWeapon();
				
				if (weapon)
				{
					if (weapon->GetWeaponInfo()->WeaponType == WeaponTypes::WEAPONTYPE_GRENADE && Act != ACT_NONE) {
						GrenadeType = *weapon->m_iItemDefinitionIndex();

						int tick = Source::Globals->tick_count;

						if (LastSimulateTick != tick) {
							Simulate();
							LastSimulateTick = tick;
						}
					}
					else {
						GrenadeType = 0;
					}
				}
			}
		}
		// -----------------
		// Paints the grenade trajectory
		// -----------------
		void CGrenadePrediction::PaintGrenadeTrajectory(Render::D3D::DrawManager* graphics)
		{
			auto local = C_CSPlayer::GetLocal();
			if (local && LastSimulateTick != 0 && LastSimulateTick == Source::Globals->tick_count) {
				int size = Path.size();

				if (GrenadeType && size > 1) 
				{
					Vector screen1, screen2;
					Vector prev = Path[0];

					Vector endPos = Path[size - 1];
					Vector absEndPos = endPos;
					float totalAdded = 0.0f;

					while (totalAdded < 30.0f) 
					{
						if (Source::EngineTrace->GetPointContents(endPos) == CONTENTS_EMPTY)
							break;

						totalAdded += 2.0f;
						endPos.z += 2.0f;
					}
					float bestDamage = 0;
					Color bestColor = Color::White();

					auto* weapon = local->GetActiveWeapon();

					if (weapon && *weapon->m_iItemDefinitionIndex() == WEAPON_HEGRENADE) 
					{
						// Calculate damage for each entity
						if (Config::ESP.NadeDamage)
						{
							FOR_EACH_CLIENT
							{
								auto player = GET_ENTITY(ClientIndex);

								if (!player || !player->IsAlive())
									continue;

								float dist = (player->m_vecOrigin() - endPos).Length();

								if (dist < 350.0f)
								{
									CTraceFilterIgnoreGrenades filter;
									filter.pSkip = rcast<IHandleEntity*>(local);
									filter.iCollisionGroup = 0;
									Ray_t ray;
									Vector nadeScreen;

									if (!graphics->W2S(endPos, nadeScreen))
										continue;

									Vector hitbox = player->GetBonePosition(HITBOX_PELVIS);
									ray.Init(endPos, hitbox);
									trace_t tr;
									Source::EngineTrace->TraceRay(ray, MASK_SHOT, rcast<ITraceFilter*>(&filter), &tr);

									if (tr.m_pEnt == player)
									{
										Vector hitboxScreen;

										if (!graphics->W2S(hitbox, hitboxScreen))
											continue;

										// Nade damage calculations
										static const float A = 105.0f, B = 25.0, C = 140.0f;
										float d = ((player->m_vecOrigin() - absEndPos).Length() - B) / C;
										float damage = A*exp(-d * d);
										damage = max(scast<int>(ceilf(CalculateArmorDamage(damage, player->m_ArmorValue()))), 0);

										char dmgBuffer[128];
										sprintf_s(dmgBuffer, "-%d", scast<int>(damage));

										// Determine color
										Color clr = Color::Lime();
										if (damage >= player->m_iHealth())
											clr = Color::Red();

										graphics->Line({ nadeScreen.x, nadeScreen.y }, { hitboxScreen.x, hitboxScreen.y }, CD3D(clr));

										Vector midPoint = (hitbox + absEndPos) * 0.5f, midPointScreen;
										if (!graphics->W2S(midPoint, midPointScreen))
											continue;

										graphics->Text({ midPointScreen.x, midPointScreen.y }, CD3D(clr),
											Render::D3D::text_flags::centered_x, dmgBuffer);

										if (damage > bestDamage)
										{
											bestColor = clr;
											bestDamage = damage;
										}
									}
								}
							}
						}
					}

					for (auto& cur : Path) 
					{
						if (graphics->W2S(prev, screen1) && graphics->W2S(cur, screen2))
							graphics->Line({ screen1.x, screen1.y }, { screen2.x, screen2.y }, CD3D(bestColor));
						prev = cur;
					}

					if (graphics->W2S(prev, screen2)) 
					{
						graphics->Circle({ screen2.x, screen2.y }, 7.f, CD3D(bestColor), 32);
					}
				}
			}
		}
		// -----------------
		// Sets up the whole class
		// -----------------
		void CGrenadePrediction::Setup(Source::C_CSPlayer* pl, Source::Vector& vecSrc, Source::Vector& vecThrow, const Source::QAngle& angEyeAngles)
		{
			auto local = C_CSPlayer::GetLocal();
			if (local)
			{
				QAngle angThrow = angEyeAngles;
				float pitch = angThrow.x;

				if (angThrow.x <= 90.0f) {
					if (angThrow.x < -90.0f) {
						angThrow.x += 360.0f;
					}
				}
				else {
					angThrow.x -= 360.0f;
				}
				angThrow.x = angThrow.x - (90.0f - fabs(angThrow.x)) * 10.0f / 90.0f;

				// Gets ThrowVelocity from weapon files
				// Clamped to [15,750]
				float flVel = 750.0f * 0.9f;

				// Do magic on member of grenade object [esi+9E4h]
				// m1=1  m1+m2=0.5  m2=0
				static const float power[] = { 1.0f, 1.0f, 0.5f, 0.0f };
				float b = power[Act];
				// Clamped to [0,1]
				b = b * 0.7f;
				b = b + 0.3f;
				flVel *= b;

				Vector vForward, vRight, vUp;
				Maths::AngleVectors(angThrow, &vForward, &vRight, &vUp);

				vecSrc = pl->GetEyePosition();
				float off = (power[Act] * 12.0f) - 12.0f;
				vecSrc.z += off;

				// Game calls UTIL_TraceHull here with hull and assigns vecSrc tr.endpos
				trace_t tr;
				Vector vecDest = vecSrc;

				vecDest.MultAdd(vForward, 22.0f);

				CGrenadePredictTraceFilter filter;
				filter.pSkip = (IHandleEntity*)local;
				filter.iCollisionGroup = 0;
				HullTrace(vecSrc, vecDest, (ITraceFilter*)&filter, tr);

				// After the hull trace it moves 6 units back along vForward
				// vecSrc = tr.endpos - vForward * 6
				Vector vecBack = vForward; vecBack *= 6.0f;
				vecSrc = tr.endpos;
				vecSrc -= vecBack;

				// Finally calculate velocity
				vecThrow = pl->m_vecVelocity(); vecThrow *= 1.25f;
				vecThrow.MultAdd(vForward, flVel);
			}
		}
		// -----------------
		// Simulate grenade behaviour
		// -----------------
		void CGrenadePrediction::Simulate()
		{
			auto local = C_CSPlayer::GetLocal();
			if (local)
			{
				Vector vecSrc, vecThrow;
				QAngle eyeangs;
				Source::Engine->GetViewAngles(eyeangs);
				Setup(local, vecSrc, vecThrow, eyeangs);

				float interval = Source::Globals->interval_per_tick;

				// Log positions 20 times per sec
				int logstep = static_cast<int>(0.05f / interval);
				int logtimer = 0;

				Path.clear();
				for (unsigned int i = 0; i < /*path.max_size( ) - 1*/1024; ++i) {
					if (!logtimer)
						Path.push_back(vecSrc);

					int s = Step(vecSrc, vecThrow, i, interval);
					if ((s & 1)) break;

					// Reset the log timer every logstep OR we bounced
					if ((s & 2) || logtimer >= logstep) logtimer = 0;
					else ++logtimer;

					if (vecThrow == Vector())
						break;
				}
				Path.push_back(vecSrc);
			}
		}
		// -----------------
		// Executed on each step
		// -----------------
		int CGrenadePrediction::Step(Source::Vector& vecSrc, Source::Vector& vecThrow, int tick, float interval)
		{
			// Apply gravity
			Vector move;
			AddGravityMove(move, vecThrow, interval, false);

			// Push entity
			trace_t tr;
			PushEntity(vecSrc, move, tr);

			int result = 0;
			// Check ending conditions
			if (CheckDetonate(vecThrow, tr, tick, interval) || tr.fraction == 0.0f) {
				result |= 1;
			}

			// Resolve collisions
			if (tr.fraction != 1.0f) {
				result |= 2; // Collision!
				ResolveFlyCollisionCustom(tr, vecThrow, interval);
			}

			// Set new position
			vecSrc = tr.endpos;

			return result;
		}
		// -----------------
		// Check if nade is gonna detonate
		// -----------------
		bool CGrenadePrediction::CheckDetonate(const Source::Vector& vecThrow, const Source::CGameTrace& tr, int tick, float interval)
		{
			switch (GrenadeType) {
			case WEAPON_HEGRENADE:
			case WEAPON_FLASHBANG:
				// Pure timer based, detonate at 1.5s, checked every 0.2s
				return static_cast< float >(tick)*interval > 1.5f && !(tick%static_cast< int >(0.2f / interval));

			case WEAPON_DECOY:
			case WEAPON_SMOKEGRENADE:
				// Velocity must be <0.1, this is only checked every 0.2s
				if (const_cast<Vector&>(vecThrow).Length() < 0.1f) {
					int det_tick_mod = static_cast< int >(0.2f / interval);
					return !(tick%det_tick_mod);
				}
				return false;

			case WEAPON_INCGRENADE:
			case WEAPON_MOLOTOV:
				// Detonate when hitting the floor
				if (tr.fraction != 1.0f && tr.plane.normal.z > 0.7f)
					return true;

				return static_cast< float >(tick)*interval > 2.35f && !(tick%static_cast< int >(0.05f / interval));

				// OR we've been flying for too long

			default:
				return false;
			}
		}
		// -----------------
		// Add gravity
		// -----------------
		void CGrenadePrediction::AddGravityMove(Source::Vector& move, Source::Vector& vel, float frametime, bool onground)
		{
			static ConVar* sv_gravity = Source::CVar->FindVar(enc("sv_gravity"));

			Vector basevel(0.0f, 0.0f, 0.0f);

			move.x = (vel.x + basevel.x) * frametime;
			move.y = (vel.y + basevel.y) * frametime;

			if (onground) {
				move.z = (vel.z + basevel.z) * frametime;
			}
			else {
				// Game calls GetActualGravity( this );
				float gravity = (sv_gravity ? sv_gravity->GetFloat() : 800.0f) * 0.4f;

				float newZ = vel.z - (gravity * frametime);
				move.z = ((vel.z + newZ) / 2.0f + basevel.z) * frametime;

				vel.z = newZ;
			}
		}
		// -----------------
		// Perform hull trace
		// -----------------
		void CGrenadePrediction::HullTrace(Source::Vector src, Source::Vector end, Source::ITraceFilter* filter, Source::CGameTrace& tr)
		{
			Ray_t ray;

			static const Vector mins(-2.0f, -2.0f, -2.0f);
			static const Vector maxs(2.0f, 2.0f, 2.0f);

			ray.Init(src, end, mins, maxs);

			Source::EngineTrace->TraceRay(ray, 0x200400B, filter, &tr);
		}
		// -----------------
		// Push a new entity
		// -----------------
		void CGrenadePrediction::PushEntity(Source::Vector & src, const Source::Vector & move, Source::CGameTrace & tr)
		{
			auto local = C_CSPlayer::GetLocal();
			if (local)
			{
				Vector vecAbsEnd = src + move;

				// Trace through world
				CGrenadePredictTraceFilter filter;
				filter.pSkip = (IHandleEntity*)local;
				filter.iCollisionGroup = 0;

				HullTrace(src, vecAbsEnd, (ITraceFilter*)&filter, tr);
			}
		}
		// -----------------
		// Resolves collisions
		// -----------------
		void CGrenadePrediction::ResolveFlyCollisionCustom(Source::CGameTrace& tr, Source::Vector& vecVelocity, float interval)
		{
			// Calculate elasticity
			float flSurfaceElasticity;
			C_CSPlayer* trent = (C_CSPlayer*)tr.m_pEnt;

			//Don't bounce off of players with perfect elasticity
			if (tr.m_pEnt && trent->IsPlayer()) {
				flSurfaceElasticity = 0.3f;
			}
			else {
				flSurfaceElasticity = 1.0f;
			}

			if (tr.contents & CONTENTS_WINDOW) {
				vecVelocity *= 0.65f;
				Vector dummy;
				trace_t exit;
				Vector newprev;
				if (Path.size() > 1) {
					Vector cur = Path[Path.size() - 1];
					for (unsigned int i = Path.size() - 2; i >= 0; i--) {
						newprev = Path[i];
						if (newprev != cur && (newprev - cur).Length() > 0.01f) {
							break;
						}
					}
					QAngle angle = Maths::GetAngle(newprev, cur);
					if (angle != QAngle(0.0f, 0.0f, 0.0f)) {
						Vector vDir;
						Maths::AngleVectors(angle, &vDir);
						vDir = Maths::NormalizeAngle(vDir);
						float flDistance = 0;
						Vector end;

						while (flDistance <= 90.0f) {
							flDistance += 4.0f;

							end = tr.endpos + (vDir * flDistance);

							if ((Source::EngineTrace->GetPointContents(end) & MASK_SOLID) == 0) {
								tr.endpos = end;
								break;
							}
						}
					}
				}
				return;
			}

			// ReadFloat((uintptr_t)&entersurf->physics.elasticity); //1.0;  // Assume all surfaces have the same elasticity
			float flGrenadeElasticity = 0.45f; // GetGrenadeElasticity()
			float flTotalElasticity = flGrenadeElasticity * flSurfaceElasticity;
			if (flTotalElasticity > 0.9f) flTotalElasticity = 0.9f;
			if (flTotalElasticity < 0.0f) flTotalElasticity = 0.0f;

			// Calculate bounce
			Vector vecAbsVelocity;
			PhysicsClipVelocity(vecVelocity, tr.plane.normal, vecAbsVelocity, 2.0f);
			vecAbsVelocity *= flTotalElasticity;

			float flSpeedSqr = Maths::DotProduct(vecVelocity, vecVelocity);

			if (tr.plane.normal.z > 0.7f) // Floor
			{
				// Verify that we have an entity.
				vecVelocity = vecAbsVelocity;
				if (flSpeedSqr < (20 * 20)) {
					vecAbsVelocity.Reset();
					vecVelocity = vecAbsVelocity;
				}
				else {
					vecAbsVelocity *= ((1.0f - tr.fraction) * interval);
					PushEntity(tr.endpos, vecAbsVelocity, tr);
				}
			}
			else {
				if (flSpeedSqr < (20 * 20)) {
					vecAbsVelocity.Reset();
					vecVelocity = vecAbsVelocity;
				}
				else {
					vecVelocity = vecAbsVelocity;
				}
			}
		}
		// -----------------
		// Physics according to velocity
		// -----------------
		unsigned char CGrenadePrediction::PhysicsClipVelocity(const Source::Vector& in, const Source::Vector& normal, Source::Vector& out, float overbounce)
		{
			// pMaths boi
			static const float STOP_EPSILON = 0.1f;

			float    backoff;
			float    change;
			float    angle;
			int        i;
			unsigned char blocked;

			blocked = 0;

			angle = normal[2];

			if (angle > 0) {
				blocked |= 1;        // floor
			}
			if (!angle) {
				blocked |= 2;        // step
			}

			backoff = in.Dot(normal) * overbounce;

			for (i = 0; i < 3; i++) {
				change = normal[i] * backoff;
				out[i] = in[i] - change;
				if (out[i] > -STOP_EPSILON && out[i] < STOP_EPSILON) {
					out[i] = 0;
				}
			}

			return blocked;
		}
	}
}