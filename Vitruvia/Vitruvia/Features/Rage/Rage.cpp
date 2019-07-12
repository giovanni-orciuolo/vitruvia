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

#include "Rage.h"
#include "../../Tools/Autowall.h"
#include "../../Tools/SpreadManager.h"
#include "../Visuals/Visuals.h"
using namespace Source;

namespace Vitruvia {

F_Rage* Rage;

// ==================================================================
// Sorter functions
// Those are predicates to pass std::sort to order m_Targets

using sorter_function = std::function<bool(const Target_t& a, const Target_t& b)>;
std::array<sorter_function, 4> pred_sorters
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
std::array<shooter_function, 3> pred_shooters
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
	}
};

// ==================================================================

ConVar* weapon_recoil_scale = nullptr;
float pos_alpha = 0.f;

void F_Rage::OnCreateMove(Source::CUserCmd* cmd)
{
	// Set the cmd to work with
	this->m_pCmd = cmd;
	// Reset the best target because I will always need to regrab him
	this->m_BestTarget.Reset();
	this->m_Targets.clear(); // Same goes for targets, clear the vector containing them

	// Reset pos target
	this->m_posTarget.Reset();

	if (weapon_recoil_scale == nullptr)
		weapon_recoil_scale = Source::CVar->FindVar(enc("weapon_recoil_scale"));

	// Set firing
	// If too much time has passed since the last time I've fired a weapon, reset shots fired
	auto local = C_CSPlayer::GetLocal();
	if (local) m_bIsFiring = cmd->buttons & IN_ATTACK && !local->GotInvalidWeapon();
	if (fabs(Source::Globals->cur_time - m_flLastFireTime) > 2.f)
		m_iShotsFired = 0;

	if (Config::Ragebot.Active)
	{
		// Apply some prior checks before going into the rage functions
		if (this->PriorChecks())
		{
			// Fill m_Targets
			this->FillTargets();

			// Order m_Targets and grab m_BestTarget
			this->FindBestTarget();

			// Shoot at m_BestTarget
			this->ShootBestTarget();
		}
	}
}

void F_Rage::OnEndScene(Render::D3D::DrawManager* graphics)
{
	// AimSpot
	if (graphics && Config::ESP.AimSpot && !this->m_posTarget.IsNull())
	{
		Vector pos_2d;
		if (graphics->W2S(this->m_posTarget, pos_2d))
		{
			Color clr = Color::White();
			clr.SetAlpha(pos_alpha);
			graphics->CircleFilled(ImVec2(pos_2d), 2.5f, ImU32(clr.toD3D()), 32);
			pos_alpha -= 0.0001;
		}
	}
}

void F_Rage::OnFrameStageNotify(Source::ClientFrameStage_t stage)
{
	// Perform PVS fix here on FRAME_RENDER_START
	auto local = C_CSPlayer::GetLocal();
	if (local && Source::Engine->IsInGame() && stage == FRAME_RENDER_START)
	{
		FOR_EACH_CLIENT
		{
			auto player = GET_ENTITY(ClientIndex);
			if (player != nullptr 
				&& player != local) // Do not apply pvs-fix on yourself idiot...
			{
				*(int*)(rcast<uintptr_t>(player) + 0xA28) = 0;
				*(int*)(rcast<uintptr_t>(player) + 0xA30) = Source::Globals->frame_count;
			}
		}
	}
}

void F_Rage::OnListenedEvent(Source::IGameEvent* game_event)
{
	auto local = C_CSPlayer::GetLocal();
	if (local && local->IsAlive() && EVENT_IS(enc("weapon_fire")))
	{
		auto* whoFired = USERID2PLAYER(game_event->GetInt(enc("userid")));
		if (whoFired && whoFired == local && m_bIsFiring)
		{
			m_flLastFireTime = Source::Globals->cur_time;
			m_iShotsFired++;
		}
	}
}

bool F_Rage::PriorChecks()
{
	auto local = C_CSPlayer::GetLocal();
	if (local)
	{
		auto weapon = local->GetActiveWeapon();
		if (weapon)
		{
			// If AutoShoot is not enabled, use pred_shooters to determine when to perform ragebot
			if (!Config::RageAccuracy.AutoShoot)
			{
				if (!pred_shooters[Config::Ragebot.Mode](m_pCmd, Config::Ragebot.Key))
					return false;
			}

			// SpeedLimit check
			if (Config::Ragebot.EnableSpeedLimit)
			{
				if (local->m_vecVelocity().Length() > Config::Ragebot.SpeedLimit)
					return false;
			}

			if (local->GotInvalidWeapon())
				return false;

			if (!weapon->CanFire(local->m_nTickBase()))
				return false;

			if (this->m_pCmd->weaponselect != 0)
				return false;

			return true;
		}
	}
	return false;
}

int target_index = 0;

void F_Rage::FillTargets()
{
	auto local = C_CSPlayer::GetLocal();
	if (local)
	{
		target_index = 0;

		FOR_EACH_CLIENT
		{
			auto player = GET_ENTITY(ClientIndex);
			if (player // validity of player
				&& player != local // player is not me
				&& !player->GetNetworkable()->IsDormant() // player is networked
				&& player->IsAlive() // player is alive
				&& !player->m_bGunGameImmunity()) // player is not immune
			{
				// If config needs to skip team mates
				if (!Config::RageTarget.TeamMates)
				{
					// If the entity is a team mate, skip
					if (player->m_iTeamNum() == local->m_iTeamNum())
						continue;
				}

				Vector hitbox_position = player->GetHitboxPosition(Config::Ragebot.Hitbox);
				if (hitbox_position != Vector(0, 0, 0))
				{
					// FOV check
					if (Config::Ragebot.FOV > 0.f &&
						Maths::GetFov(this->m_pCmd->viewangles, local->GetEyePosition(), hitbox_position) > Config::Ragebot.FOV)
						continue;

					// Get angle from me to target
					QAngle aim_angle = Maths::GetAngle(local->GetEyePosition(), hitbox_position);
					aim_angle = Maths::NormalizeAngle(aim_angle);

					// Apply recoil compensation
					aim_angle -= local->m_aimPunchAngle() * weapon_recoil_scale->GetFloat();
					aim_angle = Maths::NormalizeAngle(aim_angle);

					// Add it to the list
					Target_t t = Target_t(local, player, aim_angle, hitbox_position);
					t.SetHitgroup(player->GetHitgroupByHitbox(Config::Ragebot.Hitbox));

					m_Targets.push_back(t);
					target_index++;
				}
			}
		}
	}
}

void F_Rage::FindBestTarget()
{
	// If there are actually targets
	if (target_index > 0)
	{
		// Sort them by sort by predicate
		std::sort(m_Targets.begin(), m_Targets.end(), pred_sorters[Config::Ragebot.Mode]);
		// My best target is the first one
		this->m_BestTarget = *m_Targets.begin();
	}
}

// player -> Pointer to player to get hitbox from
// hitbox_id -> Hitbox ID to get points from
// boneMatrix -> The matrix to use for calculations
// OUT points -> Output vector where I'll store point coords
bool F_Rage::GetPointsForHitbox(C_CSPlayer* player, int hitbox_id, BoneMatrix boneMatrix, std::vector<Vector>& out_points)
{
	if (hitbox_id >= 0 && hitbox_id < HITBOX_MAX)
	{
		auto local = C_CSPlayer::GetLocal();
		if (local)
		{
			// Initialize points vector
			std::vector<Vector> points{};

			// Get hitbox
			auto hitbox = player->GetStudioBox(hitbox_id);
			if (hitbox)
			{
				// Get mins and maxs
				Vector mins = Maths::VectorTransform(hitbox->bbmin, boneMatrix[hitbox->bone]);
				Vector maxs = Maths::VectorTransform(hitbox->bbmax, boneMatrix[hitbox->bone]);
				float r = hitbox->radius;

				// Now it's time to fill my points
				points.push_back(Vector(maxs.x - r, maxs.y, maxs.z)); // 1
				points.push_back(Vector(maxs.x, maxs.y, maxs.z + r)); // 2
				points.push_back(Vector(maxs.x + r, maxs.y, maxs.z)); // 3
				points.push_back(Vector(mins.x - r, mins.y, mins.z)); // 4
				points.push_back(Vector(mins.x, mins.y, mins.z - r)); // 5
				points.push_back(Vector(mins.x + r, mins.y, mins.z)); // 6
				points.push_back(Vector(maxs.x, maxs.y + r, maxs.z)); // 7
				points.push_back(Vector(maxs.x, maxs.y - r, maxs.z)); // 8
				points.push_back(Vector(mins.x, mins.y + r, mins.z)); // 9
				points.push_back(Vector(mins.x, mins.y - r, mins.z)); // 10

				out_points = points;
				return true;
			}
		}
	}
	return false;
}

void F_Rage::ShootBestTarget()
{
	auto local = C_CSPlayer::GetLocal();
	if (local)
	{
		auto localWeapon = local->GetActiveWeapon();
		if (local && localWeapon)
		{
			// If there are actually targets
			if (target_index > 0)
			{
				Hitboxes hitbox = (Hitboxes)Config::Ragebot.Hitbox.m_Value;
				C_CSPlayer* best_target = this->m_BestTarget.GetEntity();
				if (best_target == nullptr)
					return;

				// Before everything else, if we are using a revolver and autorevolver is enabled
				// I need to start firing without effectively firing
				static bool wantsPostPoneFire = false;
				float postPoneFire = -1.f;
				bool gotRevolver = *localWeapon->m_iItemDefinitionIndex() == Weapons::WEAPON_REVOLVER;
				if (Config::RageAccuracy.AutoRevolver)
				{
					// If my weapon is a revolver
					if (gotRevolver)
					{
						if (wantsPostPoneFire)
							postPoneFire = localWeapon->GetPostponeFireReadyTime();
						// Prematurely start attacking
						if (postPoneFire != -1 && std::isfinite(postPoneFire))
							this->m_pCmd->buttons |= IN_ATTACK;
						wantsPostPoneFire = true;
					}
				}

				// Proceed with normal ragebot logic
				Vector best_point;
				m_Matrix = {}; // Reset matrix
				bool foundGoodTick = false;
				m_iTargetTick = 0;
				bool invalidTarget = false; // True when I need to return

				// Backtrack logic
				// The best backtrack record is the resolved tick
				auto bestRecord = Resolver->GetBacktrackedTick(best_target->GetIndex());
				if (bestRecord.SimulationTime > 0.f)
				{
					// Save this tick
					m_Matrix = bestRecord.BoneMatrix;
					m_iTargetTick = bestRecord.TickCount;
					foundGoodTick = true;
				}
				Resolver->ResetBacktrackedTick(best_target->GetIndex());

				// Resolver tells us if we should bodyaim him no matter what
				bool should_baim = PlayerList[best_target->GetIndex()].ResolveInfo.ShouldBodyAim;
				PlayerList[best_target->GetIndex()].ResolveInfo.ShouldBodyAim = false; // Reset it no matter what

				// Hitscan logic
				if (Config::RageTarget.Hitscan)
				{
					// Calculate the best hitbox
					int best_hitbox = -1;
					float best_damage = 0.f;
					for (int h = HITBOX_HEAD; h < HITBOX_MAX; h++)
					{
						Vector hpos;
						(foundGoodTick) ?
							hpos = best_target->GetHitboxPositionCustom(h, m_Matrix) :
							hpos = best_target->GetHitboxPosition(h);
						QAngle ang = Maths::GetAngle(local->GetEyePosition(), hpos);

						// Prepare to override if needed
						Tools::CAutowall::OverrideData od;
						if (foundGoodTick)
						{
							od.entity = best_target;
							od.hitgroup = best_target->GetHitgroupByHitbox(h);
							od.position = hpos;
						}
						int hitgroup = 0;
						bool didPenetrate = false;
						float damage = Tools::Autowall->GetDamage(
							local->GetEyePosition(),
							ang,
							local,
							Hitboxes(h),
							hitgroup,
							didPenetrate,
							!Config::RageTarget.TeamMates,
							foundGoodTick,
							od
						);

						if (damage > best_damage && damage >= Config::RageTarget.MinDamage.m_Value)
						{
							best_hitbox = h;
							best_damage = damage;
						}
					}
					// Store the best hitbox calculated, if found
					if (best_hitbox > -1)
						hitbox = (Hitboxes)best_hitbox;
				}
				else
				{
					// If hitscan is not enabled then I just need to calculated damage to hitbox pos to check
					// If autowall is enabled
					if (Config::RageAccuracy.AutoWall)
					{
						Vector hpos;
						(foundGoodTick) ?
							hpos = best_target->GetHitboxPositionCustom(hitbox, m_Matrix) :
							hpos = best_target->GetHitboxPosition(hitbox);
						QAngle ang = Maths::GetAngle(local->GetEyePosition(), hpos);

						// Prepare to override if needed
						Tools::CAutowall::OverrideData od;
						if (foundGoodTick)
						{
							od.entity = best_target;
							od.hitgroup = best_target->GetHitgroupByHitbox(hitbox);
							od.position = hpos;
						}
						int hitgroup = 0;
						bool didPenetrate = false;
						float damage = Tools::Autowall->GetDamage(
							local->GetEyePosition(),
							ang,
							local,
							hitbox,
							hitgroup,
							didPenetrate,
							!Config::RageTarget.TeamMates,
							foundGoodTick,
							od
						);

						// Return if damage is not enough
						if (damage < Config::RageTarget.MinDamage.m_Value)
							invalidTarget = true;
					}
				}

				// This section modifies the hitbox position accordingly
				if (hitbox != HITBOX_BODY)
				{
					Vector bodyPos;
					(foundGoodTick) ?
						bodyPos = best_target->GetHitboxPositionCustom(hitbox, m_Matrix) :
						bodyPos = best_target->GetHitboxPosition(hitbox);
					Tools::CAutowall::OverrideData od;
					if (foundGoodTick)
					{
						od.entity = best_target;
						od.hitgroup = best_target->GetHitgroupByHitbox(hitbox);
						od.position = bodyPos;
					}
					QAngle ang = Maths::GetAngle(local->GetEyePosition(), bodyPos);
					int hitgroup = 0;
					bool didPenetrate = false;
					float damage = Vitruvia::Tools::Autowall->GetDamage(
						local->GetEyePosition(),
						ang,
						local,
						Hitboxes(hitbox),
						hitgroup,
						didPenetrate,
						!Config::RageTarget.TeamMates,
						foundGoodTick,
						od
					);
					bool canDamageBody = damage >= Config::RageTarget.MinDamage.m_Value;

					// Modify the hitbox position if AwpBody needs to be accounted for
					if (Config::Ragebot.AwpBody)
					{
						// If I've got an AWP and the body is visible, modify the hitbox
						// Otherwise, do not modify anything
						if (*localWeapon->m_iItemDefinitionIndex() == WEAPON_AWP
							&& canDamageBody)
							hitbox = HITBOX_BODY;
					}

					// Bodyaim after X shots
					if (Config::RageTarget.BodyAimAfterShots.m_Value > 0)
					{
						// If the number of shots fired goes above the value and body is visible, baim
						if (m_iShotsFired > Config::RageTarget.BodyAimAfterShots.m_Value
							&& canDamageBody)
							hitbox = HITBOX_BODY;
					}

					// Bodyaim if hp less than
					if (!Config::RageTarget.BodyAimIfHPLessThanDamage // If the other one is not enabled
						&& this->m_BestTarget.GetHealth() < Config::RageTarget.BodyAimIfHPLessThan.m_Value
						&& canDamageBody)
					{
						hitbox = HITBOX_BODY;
					}

					// Bodyaim if hp less than damage at dick
					if (Config::RageTarget.BodyAimIfHPLessThanDamage)
					{
						if (canDamageBody)
						{
							auto info = localWeapon->GetWeaponInfo();
							if (info)
							{
								if (info->iDamage >= this->m_BestTarget.GetHealth())
									hitbox = HITBOX_BODY;
							}
						}
					}

					// Bodyaim if resolver told us so
					if (should_baim && canDamageBody)
					{
						hitbox = HITBOX_BODY;
					}
				}

				// Now I can determine the new hitgroup by modified hitbox
				this->m_BestTarget.SetHitgroup(best_target->GetHitgroupByHitbox(hitbox));

				// Determine which matrix to use
				BoneMatrix boneMatrix{};
				if (foundGoodTick)
					boneMatrix = m_Matrix; // Use the matrix I got from backtracking
				else
				{
					// Calculate it through setup bones
					auto renderable = best_target->GetRenderable();
					if (renderable)
					{
						if (!renderable->SetupBones(boneMatrix.data(), MAXSTUDIOBONES, BONE_USED_BY_HITBOX, Source::Globals->cur_time))
							invalidTarget = true;
					}
				}

				// MultiPoint logic
				if (Config::RageTarget.MultiPoint)
				{
					std::vector<Vector> multi_points{};
					if (GetPointsForHitbox(best_target, hitbox, boneMatrix, multi_points)) // If GetPointsForHitbox_ operation is successful
					{
						// For each point that I found, apply Autowall->GetDamage
						float best_damage = 0.f;
						for (Vector point : multi_points)
						{
							QAngle ang = Maths::GetAngle(local->GetEyePosition(), point);

							// Prepare to override if needed
							Tools::CAutowall::OverrideData od;
							if (foundGoodTick)
							{
								od.entity = best_target;
								od.hitgroup = best_target->GetHitgroupByHitbox(hitbox);
								od.position = point;
							}
							int hitgroup = 0;
							bool didPenetrate = false;
							float damage = Tools::Autowall->GetDamage(
								local->GetEyePosition(),
								ang,
								local,
								hitbox,
								hitgroup,
								didPenetrate,
								!Config::RageTarget.TeamMates,
								foundGoodTick,
								od
							);

							if (damage > best_damage && damage >= Config::RageTarget.MinDamage.m_Value)
							{
								best_point = point;
								best_damage = damage;
							}
						}
					}
				}
				else if (!foundGoodTick)
				{
					// If multipoint is not enabled and we didn't find a good tick, then just set best_point to hitboxpos(hitbox)
					best_point = best_target->GetHitboxPosition(hitbox);
				}
				else if (foundGoodTick)
				{
					// If multipoint is not enabled and a good tick has been found, set best_point to hitboxpos(hitbox) using backtracked matrix
					best_point = best_target->GetHitboxPositionCustom(hitbox, m_Matrix);
				}

				if (best_point == Vector())
					invalidTarget = true;

				// Calculate the best angle based on the best point I've found
				QAngle best_angle = Maths::GetAngle(local->GetEyePosition(), best_point);
				best_angle -= local->m_aimPunchAngle() * weapon_recoil_scale->GetFloat();

				// HitChance logic (I account for it only if I'm autoshooting)
				// If hitchance I calculate based by best_angle is not enough, return
				if (Config::RageAccuracy.AutoShoot &&
					Config::RageAccuracy.HitChance.m_Value != 0 &&
					local->GetHitchance(best_target, best_angle) < Config::RageAccuracy.HitChance.m_Value)
					invalidTarget = true;

				// Ok now if invalidTarget is true and I'm using autorevolver I need to check if I'm near the curtime and reset attack
				if (invalidTarget)
				{
					if (Config::RageAccuracy.AutoRevolver && gotRevolver)
					{
						if (postPoneFire - Source::Globals->cur_time < 0)
						{
							// Reset attack
							this->m_pCmd->buttons &= ~IN_ATTACK;
						}
					}
					return;
				}

				// Now if I'm here it means that I need to shoot exactly at best_angle,
				// with best target being best_target and best point to shoot being best_point.
				Backtrack->SetBestHitbox(hitbox);

				// Before doing that, I need to apply the accuracy modificators!
				if (Config::RageAccuracy.AutoShoot && !gotRevolver)
				{
					if (localWeapon->CanFire(local->m_nTickBase()))
						this->m_pCmd->buttons |= IN_ATTACK;
					else 
						this->m_pCmd->buttons &= ~IN_ATTACK;
				}
				if (Config::RageAccuracy.AutoStop)
				{
					// Fast autostop
					if (local->m_vecVelocity().Length2D() > 15)
					{
						this->m_pCmd->forwardmove = -this->m_pCmd->forwardmove;
						this->m_pCmd->sidemove = -this->m_pCmd->sidemove;
					}
					else
					{
						this->m_pCmd->forwardmove = 0;
						this->m_pCmd->sidemove = 0;
					}
				}
				if (Config::RageAccuracy.AutoCrouch)
				{
					this->m_pCmd->buttons |= IN_DUCK;
				}
				if (Config::RageAccuracy.AutoScope)
				{
					// If my weapon has got zoom levels
					if (localWeapon->GetWeaponInfo()->WeaponType == WeaponTypes::WEAPONTYPE_SNIPER_RIFLE ||
						*localWeapon->m_iItemDefinitionIndex() == Weapons::WEAPON_SG556
						&& !(this->m_pCmd->buttons & IN_ATTACK2))
					{
						// Scope in
						this->m_pCmd->buttons |= IN_ATTACK2;
					}
				}

				// Normalize the angle
				best_angle = Maths::NormalizeAngle(best_angle);

				// Set the target pos to be best_point
				this->m_posTarget = best_point;
				pos_alpha = 1.f;

				// Continue only if I'm firing
				if (this->m_pCmd->buttons & IN_ATTACK)
				{
					if (Config::VisualsOther.Lasers && localWeapon->CanFire(local->m_nTickBase()))
					{
						// Issue the creation of a new laser going from my head - 5 to the best_point
						auto start = local->GetEyePosition(); start.z -= 5.f;
						VisualsOthers->AddLaser(Laser_t(
							best_target, true, 1.f, start, best_point, Config::Colors.Beams.m_Value
						));
					}

					// I'm effectively aimbotting in this moment
					global->set_Aimbotting(true);
					// Apply the angles
					if (Config::Ragebot.Silent)
					{
						m_pCmd->viewangles = best_angle;
					}
					else
					{
						m_pCmd->viewangles = best_angle;
						Source::Engine->SetViewAngles(best_angle);
					}
				}

				// TODO: Working NoSpread, so I can play on nospread hvh servers without weapon_accuracy_nospread enabled!
				if (Config::RageAccuracy.NoSpread)
				{
					float spreadX = 0.f, spreadY = 0.f;
					Tools::SpreadManager->GetSpreads(this->m_pCmd, localWeapon, spreadX, spreadY);
					Tools::SpreadManager->ApplySpreadFactor(this->m_pCmd, spreadX, spreadY);
				}

				// That's all, folks! cit.
			}
		}
	}
}

}