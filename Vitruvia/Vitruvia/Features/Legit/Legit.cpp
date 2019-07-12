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

#include "Legit.h"
#include "../../Tools/Autowall.h"
#include "../Rage/Rage.h"
#include "../../Utils/GameHelper/GameHelper.h"

namespace Vitruvia {

F_Legit* Legit;

using sorter_function = std::function<bool(const Target_t& a, const Target_t& b)>;
std::array<sorter_function, 4> sorters
{
	// Health
	[](const Target_t& a, const Target_t& b) -> bool	
	{
		return a.GetHealth() < b.GetHealth();
	},
	// Distance
	[](const Target_t& a, const Target_t& b) -> bool
	{
		return a.GetDistance() < b.GetDistance();
	},
	// FOV
	[](const Target_t& a, const Target_t& b) -> bool
	{
		return a.GetCrosshairDistance() < b.GetCrosshairDistance();
	},
	// Threat
	[](const Target_t& a, const Target_t& b) -> bool
	{
		return a.GetThreat() < b.GetThreat();
	}
};

using shooter_function = std::function<bool(const Source::CUserCmd*, int)>;
std::array<shooter_function, 4> shooters
{
	// On Fire
	[](const Source::CUserCmd* cmd, int) -> bool
	{
		return cmd->buttons & IN_ATTACK;
	},
	// On Key
	[](const Source::CUserCmd*, int button) -> bool
	{
		return Source::InputSystem->IsButtonDown((Source::ButtonCode_t)button);
	},
	// On Sight
	[](const Source::CUserCmd*, int) -> bool
	{
		return true;
	},
	// Never
	[](const Source::CUserCmd*, int) -> bool
	{
		return false;
	}
};

Source::Hitboxes F_Legit::GetNearestHitbox(Source::C_CSPlayer* player)
{
	using namespace Source;
	// Used to store the current best fov calculated (the minimum)
	float best_fov = FLT_MAX;
	// Used to store the current best hitbox calculated (lowest fov amount)
	int best_hitbox = -1;

	// For each hitbox
	for (int hitbox = 0; hitbox < HITBOX_MAX; hitbox++)
	{
		// If hitbox is too low skip it
		if (IsHitboxLowerBody(hitbox))
			continue;
		// Calculate position, if invalid skip the hitbox
		
		Vector position = player->GetHitboxPosition(hitbox);
		if (position == Vector(0, 0, 0)) continue;
		// Calculate fov amount for the hitbox
		float fov = Maths::GetFov(m_pCmd->viewangles, C_CSPlayer::GetLocal()->GetEyePosition(), position);
		// If fov is lower than the lowest fov
		if (fov < best_fov)
		{
			// This is the new lowest
			best_fov = fov;
			best_hitbox = hitbox;
		}
	}

	return (Source::Hitboxes)best_hitbox;
}

bool F_Legit::FindTarget()
{
	using namespace Source;

	// The objective of this function is to fill m_TargetList, sort it, and grab the best target to aim at.
	auto local = C_CSPlayer::GetLocal();
	if (local == nullptr)
		return false;

	// If the local player has got an invalid weapon in hands, return prematurely
	if (local->GotInvalidWeapon())
		return false;

	// More paranoia! Grab the current active weapon and the info about it and check for valid
	auto active_weapon = local->GetActiveWeapon();
	if (active_weapon == nullptr)
		return false;

	auto weapon_info = active_weapon->GetWeaponInfo();
	if (weapon_info == nullptr)
		return false;

	// Used to track down entities I'm pushing in the targets array
	int target_index = 0;

	// Retrieve the config table to use based on the local active weapon if the user wants weapon groups
	Config::cLegit::cAimbot cfg = Config::Aimbot;

	// If the user doesn't want to use weapon groups, the cfg will stay the same
	if (Config::Aimbot.UseGroups.m_Value)
	{
		// If he wants, I edit the cfg using the weapon cfg
		auto cfg_weap = Config::WeaponGroups[weapon_info->WeaponType];
		cfg.Active = cfg_weap->Active;
		cfg.Hitbox = cfg_weap->Hitbox;
		cfg.FOV = cfg_weap->FOV;
		cfg.Smooth = cfg_weap->Smooth;
		cfg.Time = cfg_weap->Time;
	}

	if (!cfg.Active.m_Value)
		return false;
		
	// Loop through the clients
	FOR_EACH_CLIENT
	{
		// Grab the entity and perform several validity checks on it
		auto entity = GET_ENTITY(ClientIndex);

		// If the entity is not found skip
		if (entity == nullptr)
			continue;
		// If the entity is the local player, skip
		if (entity == local)
			continue;
		// If the entity is dormant, skip
		if (entity->GetNetworkable()->IsDormant())
			continue;
		// If the entity is dead, skip
		if (!entity->IsAlive())
			continue;
		// If the entity has got spawn immunity, skip
		if (entity->m_bGunGameImmunity())
			continue;

		// If config needs to skip team mates
		if (!cfg.TeamMates.m_Value)
		{
			// If the entity is a team mate, skip
			if (entity->m_iTeamNum() == local->m_iTeamNum())
				continue;
		}

		// If config needs to skip targets behind a smoke
		if (cfg.SmokeCheck.m_Value)
		{
			// If the entity is behind a smoke, skip
			if (entity->IsBehindSmoke(cfg.Hitbox.m_Value))
				continue;
		}

		// If user wants smart aim, perform calcs
		if (cfg.SmartAim.m_Value)
		{
			// If the value returned by the function is valid (not -1)
			int best_hitbox = GetNearestHitbox(entity);
			if (best_hitbox != -1)
				cfg.Hitbox.m_Value = best_hitbox; // Modify the config
			m_iSmartHitbox = cfg.Hitbox.m_Value;
		}
		else m_iSmartHitbox = -1;

		// If config needs to skip targets that aren't visible
		if (cfg.VisibleCheck.m_Value)
		{
			// If entity isn't visible, skip
			if (!entity->IsVisible(cfg.Hitbox.m_Value))
				continue;
		}

		// Modify the hitbox position if AwpBody needs to be accounted for
		if (cfg.AwpBody.m_Value)
		{
			// If I've got an AWP
			if (*active_weapon->m_iItemDefinitionIndex() == WEAPON_AWP)
			{
				// If body is visible, modify the hitbox, otherwise just keep the old setting
				// If smart aim is on, it will keep using nearest hitbox
				if (entity->IsVisible(HITBOX_BODY))
					cfg.Hitbox.m_Value = HITBOX_BODY;
			}
		}

		Vector hitbox_position;
		int target_tick = -1; // Used to track where to backtrack

		// Check for FOV
		if (!Config::Aimbot.Backtrack)
		{
			// Get the hitbox position
			hitbox_position = entity->GetHitboxPosition(cfg.Hitbox.m_Value);
			if (Maths::GetFov(m_pCmd->viewangles, local->GetEyePosition(), hitbox_position) > cfg.FOV.m_Value)
			{
				// Backtrack is not enabled, just check for the actual fov to hitbox position
				continue;
			}
		}
		else
		{
			// If the best target is in the player list
			if (PlayerList.find(ClientIndex) != PlayerList.end())
			{
				float bestFov = FLT_MAX;
				int bestTick = -1;
				BoneMatrix bestMatrix{};

				if (!PlayerList[ClientIndex].Records.empty())
				{
					for (int i = 0; i < Backtrack->GetMaxBacktrackTick(); i++)
					{
						// I'm looping from older tick to newest tick
						if (i < PlayerList[ClientIndex].Records.size())
						{
							auto& r = PlayerList[ClientIndex].Records[i];
							auto recordPoint = entity->GetHitboxPositionCustom(cfg.Hitbox.m_Value, r.BoneMatrix);
							// Check if in this record the player was moving and the simulation time is valid
							if (r.IsMoving 
								&& F_Backtrack::IsRecordValid(r)
								&& g_GameHelper.IsPointVisible(recordPoint))
							{
								// Calculate the fov between me and the position of the hitbox in this record
								auto recordFov = Maths::GetFov(m_pCmd->viewangles, local->GetEyePosition(), recordPoint);
								if (recordFov < bestFov)
								{
									bestFov = recordFov;
									bestTick = r.TickCount;
									bestMatrix = r.BoneMatrix;
								}
							}
						}
					}

					if (bestTick != -1)
					{
						float treshold = 1.1f;
						auto info = active_weapon->GetWeaponInfo();
						if (info && info->WeaponType == WeaponTypes::WEAPONTYPE_SNIPER_RIFLE)
							treshold = 2.f;
						target_tick = bestTick;
						if (!Config::Aimbot.AdaptiveBacktrackFOV)
						{
							if (bestFov > cfg.FOV.m_Value)
								continue;
						}
						else
						{
							if (cfg.FOV.m_Value < treshold)
							{
								if (bestFov > 3.f)
									continue;
							}
							else
							{
								if (bestFov > cfg.FOV.m_Value)
									continue;
							}
						}
						hitbox_position = entity->GetHitboxPositionCustom(cfg.Hitbox.m_Value, bestMatrix);
					}
				}
			}
		}

		if (hitbox_position == Vector(0, 0, 0))
			continue;

		// Calculate the angle I need to aim at if this is the best entity and normalize it
		Vector aim_angle = Maths::GetAngle(local->GetEyePosition(), hitbox_position);
		aim_angle = Maths::NormalizeAngle(aim_angle);

		// Correct the recoil based on RCS amount
		aim_angle -= local->m_aimPunchAngle() * cfg.RCSFactor.m_Value;
		aim_angle = Maths::NormalizeAngle(aim_angle);

		// Add a new target to the list
		m_TargetList.push_back(Target_t(local, entity, aim_angle, hitbox_position, target_tick));
		target_index++;
	}

	// If I've actually found entities good enough to aim at, I can start the sorting
	if (target_index > 0)
	{
		// Sort in C++11 style based on the SortBy cfg value
		std::sort(m_TargetList.begin(), m_TargetList.end(), sorters[cfg.SortBy.m_Value]);
		// Now the value at the beginning of the list contains the actual best entity
		m_BestTarget = *m_TargetList.begin();

		//if (m_OldTarget.GetEntity() != nullptr)
		//	m_bIsConfused = m_OldTarget.GetEntity() != m_BestTarget.GetEntity();

		return true;
	}

	return false;
}

void F_Legit::AimTarget()
{
	using namespace Source;

	// If I'm calling this function it means that FindTarget() returned true, which means that m_pBestTarget is not null
	// Now all I need to do is to aim at him, always based on the config
	auto local = C_CSPlayer::GetLocal();
	if (local == nullptr)
		return;

	auto active_weapon = local->GetActiveWeapon();
	if (active_weapon == nullptr)
		return;

	if (local->GotInvalidWeapon())
		return;

	auto weapon_info = active_weapon->GetWeaponInfo();
	if (weapon_info == nullptr)
		return;

	// Retrieve the config table to use based on the local active weapon if the user wants weapon groups
	Config::cLegit::cAimbot cfg = Config::Aimbot;

	// If the user doesn't want to use weapon groups, the cfg will stay the same
	if (Config::Aimbot.UseGroups.m_Value)
	{
		// If he wants, I edit the cfg using the weapon cfg
		auto cfg_weap = Config::WeaponGroups[weapon_info->WeaponType];
		cfg.Active = cfg_weap->Active;
		(m_iSmartHitbox != -1) ?
			cfg.Hitbox = m_iSmartHitbox :
			cfg.Hitbox = cfg_weap->Hitbox;
		cfg.FOV = cfg_weap->FOV;
		cfg.Smooth = cfg_weap->Smooth;
		cfg.Time = cfg_weap->Time;
	}

	QAngle best_angle = m_BestTarget.GetAimbotAngle();

	// Now best_angle should be what we actually need to aim at

	// If this target tick count is != -1, I need to backtrack
	if (m_BestTarget.GetTickToRestore() != -1)
	{
		Backtrack->BacktrackToTick(m_BestTarget.GetTickToRestore());
		Backtrack->SetBestHitbox(cfg.Hitbox.m_Value);
	}

	// Different behaviour based on the value of cfg.Mode
	if (shooters[cfg.Mode.m_Value](m_pCmd, cfg.AimKey.m_Value))
	{
		// If the user wants to apply smooth
		if (cfg.Smooth.m_Value > 0.f)
		{
			// I smooth
			Maths::SmoothAngles(m_pCmd->viewangles, best_angle, cfg.Smooth.m_Value);
		}

		// Normalize the angle
		best_angle = Maths::NormalizeAngle(best_angle);
		
		// Apply the angles
		if (cfg.Silent)
		{
			m_pCmd->viewangles = best_angle;
		}
		else
		{
			m_pCmd->viewangles = best_angle;
			Source::Engine->SetViewAngles(m_pCmd->viewangles);
		}
	}
}

void F_Legit::ExecuteTriggerbot()
{
	using namespace Source;
	if (Config::Trigger.Active)
	{
		// This function executes the triggerbot logic
		// Init vars
		auto local = C_CSPlayer::GetLocal();
		auto weapon = local->GetActiveWeapon();
		if (local && weapon)
		{
			// If the weapon used is invalid, do not perform triggerbot
			if (!local->GotInvalidWeapon())
			{
				// If trigger key is enabled avoid doing all of this if I'm not pressing the triggerkey
				if (Config::Trigger.UseKey)
				{
					if (!Source::InputSystem->IsButtonDown(ButtonCode_t(Config::Trigger.Key.m_Value)))
						return;
				}

				Vector source,			// Where the ray will start from (usually the local player's eyes)
					destination,		// Where the ray will end
					forward;			// Source (angles) transformed into an actual vector

				trace_t trace;			// Trace that the triggerbot will use
				Ray_t ray;				// Ray that the triggerbot will shoot
				CTraceFilterSkipTwoEntities filter(local, nullptr, COLLISION_GROUP_NONE);

				// Get view angles and add recoil punch to it
				QAngle view_angles = this->m_pCmd->viewangles;
				view_angles += local->m_aimPunchAngle() * global->RecoilScale();

				// From view angle to vector, forward will contain the vector
				Maths::AngleVectors(view_angles, &forward);
				forward *= weapon->GetWeaponInfo()->flRange;

				// Source is local eye position
				source = local->GetEyePosition();

				// Destination is source that goes forward
				destination = source + forward;

				if (Config::Trigger.ThroughWalls)
				{
					// Modify the trace type so it ignores props. This will allow me to autowall through doors and shit
					filter.m_TraceType = TraceType::TRACE_EVERYTHING_FILTER_PROPS;
					// Modify the second filter so it ignores world, so I can autowall through walls and shit
					filter.pSkip2 = C_World::GetWorld();
				}

				// Init the ray and perform TraceRay
				ray.Init(source, destination);
				Source::EngineTrace->TraceRay(ray, MASK_SHOT, &filter, &trace);

				// Check if we did hit something
				if (trace.m_pEnt != nullptr)
				{
					// Convert the entity to C_CSPlayer pointer
					C_CSPlayer* trace_entity = rcast<C_CSPlayer*>(trace.m_pEnt);

					// Check if m_pEnt is a player
					if (trace_entity->IsPlayer())
					{
						// Hit will be true if the ray did actually hit the hitgroup the user wants to filter for
						bool hit = false;

						// Check for filter
						if (Config::Trigger.Hitgroup == 0) {
							hit = true;
						} else {
							hit = (trace.hitgroup == Config::Trigger.Hitgroup);
						}

						// If through walls is enabled, skip this entity if minimum wall damage isn't met
						if (Config::Trigger.ThroughWalls)
						{
							QAngle ang = Maths::GetAngle(local->GetEyePosition(), trace.endpos);
							int hitgroup = 0;
							bool didPenetrate = false;
							Tools::CAutowall::OverrideData od;
							float damage = Tools::Autowall->GetDamage(local->GetEyePosition(), ang, local, HITBOX_BODY, hitgroup, didPenetrate, !Config::RageTarget.TeamMates, false, od);
							if (didPenetrate && hitgroup != Config::Trigger.Hitgroup && damage < Config::Trigger.MinWallDamage)
								return;
						}

						// If smoke check is enabled, skip this entity if it's behind a smoke
						if (Config::Trigger.SmokeCheck)
						{
							if (trace_entity->IsBehindSmoke(HITGROUP_CHEST))
								return;
						}

						// If hitchance is enabled, skip this entity if hitchance isn't met
						if (Config::Trigger.Hitchance < 100)
						{
							float chance = local->GetHitchance(trace_entity, this->m_pCmd->viewangles);
							if (chance < Config::Trigger.Hitchance)
								return;
						}

						// Check if trace entity is valid
						if (trace_entity->m_iTeamNum() != local->m_iTeamNum()
							&& trace_entity->IsAlive()
							&& !trace_entity->m_bGunGameImmunity())
						{
							if (hit) {
								// Attac
								if (weapon->CanFire(local->m_nTickBase()))
									this->m_pCmd->buttons |= IN_ATTACK;
								else
									this->m_pCmd->buttons &= ~IN_ATTACK;
							}
						}
					}
				}
			}
		}
	}
}

void F_Legit::ExecuteStandaloneRCS()
{
	// TODO: Implement RCS Smoothing
	using namespace Source;

	if (!Config::RCS.Active.m_Value)
		return;

	// Used to store old punch
	static QAngle old_punch = QAngle(0, 0, 0);

	// Get local player
	C_CSPlayer* local = C_CSPlayer::GetLocal();
	if (!local) return;

	// Start bullet check
	if (Config::RCS.StartBullet.m_Value != 0 && local->m_iShotsFired() < Config::RCS.StartBullet.m_Value)
		return;
	// End bullet check
	if (Config::RCS.StopBullet.m_Value != 0 && local->m_iShotsFired() > Config::RCS.StopBullet.m_Value)
		return;

	// Only compensate when firing
	if (this->m_pCmd->buttons & IN_ATTACK)
	{
		// Get aim punch angles and multiply by recoil scale
		QAngle aim_punch = local->m_aimPunchAngle() * Config::Aimbot.RCSFactor.m_Value;

		// Set compensated angles
		this->m_pCmd->viewangles += (old_punch - aim_punch);
		Maths::NormalizeAngle(this->m_pCmd->viewangles);
		Source::Engine->SetViewAngles(this->m_pCmd->viewangles);

		// Save old punch for the next firing tick
		old_punch = aim_punch;
	}
	else
	{
		// Reset when not firing
		old_punch.Reset();
	}
}

void F_Legit::OnCreateMove(Source::CUserCmd* pCmd)
{
	// Perform legitbot logic only when the user is in game
	if (!Source::Engine->IsInGame())
		return;

	// Set the cmd pointer to work on
	this->m_pCmd = pCmd;
	// Reset best target
	this->m_OldTarget = this->m_BestTarget;
	this->m_BestTarget.Reset();
	// Reset target list
	this->m_TargetList.clear();

	// Perform legitbot
	// Let's find the target
	if (this->FindTarget())
	{
		// If target is found, aim at it
		this->AimTarget();
	}
	else
	{
		// If no target is found, it means I can safely perform standalone RCS
		this->ExecuteStandaloneRCS();
	}

	// Perform triggerbot
	this->ExecuteTriggerbot();
}

}
