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

#include "HvH.h"
#include "../Legit/Legit.h"
#include "../Rage/Rage.h"
#include "../Visuals/Visuals.h"
#include "../../Utils/GameHelper/GameHelper.h"
#include "../../SourceClasses/C_BaseCSGrenade.h"
#include "../../Utils/GameHelper/GameHelper.h"
#include "../../Tools/Autowall.h"
using namespace Source;

namespace Vitruvia {

#pragma region ANTIAIMS
F_HvH* HvH;

// Used to fix the movement after performing AA so I don't fuck up you know
void F_HvH::FixMovement(Source::CUserCmd* cmd, Source::QAngle oldAngles, float oldForward, float oldSide)
{
	float deltaView;
	float f1;
	float f2;

	if (oldAngles.y < 0.f)
		f1 = 360.0f + oldAngles.y;
	else
		f1 = oldAngles.y;

	if (cmd->viewangles.y < 0.0f)
		f2 = 360.0f + cmd->viewangles.y;
	else
		f2 = cmd->viewangles.y;

	if (f2 < f1)
		deltaView = abs(f2 - f1);
	else
		deltaView = 360.0f - abs(f1 - f2);

	deltaView = 360.0f - deltaView;

	cmd->forwardmove = cos(DEG2RAD(deltaView)) * oldForward + cos(DEG2RAD(deltaView + 90.f)) * oldSide;
	cmd->sidemove = sin(DEG2RAD(deltaView)) * oldForward + sin(DEG2RAD(deltaView + 90.f)) * oldSide;
}

// Used to perform AA, fakelag, all this stuff
void F_HvH::OnCreateMove(Source::CUserCmd* cmd)
{
	auto local = C_CSPlayer::GetLocal();
	if (local)
	{
		auto weapon = local->GetActiveWeapon();
		if (weapon)
		{
			// Flip m_bFlip
			m_bFlip = !m_bFlip;

			// FakeLag (choke cmd before doing AA)
			this->Choke(cmd, local);

			// AntiAim
			if (Config::AntiAim.Active)
			{
				// Check if I can perform AA
				if ((!(cmd->buttons & IN_ATTACK) // If I'm not attacking
					|| !weapon->CanFire(local->m_nTickBase())) // or I can't fire my weapon
					&& !(cmd->buttons & IN_USE)) // and I'm not using anything
				{
					// I'm not aimbotting, I'm antiaiming
					global->set_Aimbotting(false);
					global->set_AntiAiming(true);
					// Run AA!
					this->RunAA(cmd, local);
				}
				else
				{
					// I'm not antiaiming anymore
					global->set_AntiAiming(false);
				}
			}
			else
			{
				// Not antiaiming for sure, AntiAim ain't even active
				global->set_AntiAiming(false);
			}
		}

		// FakeWalk
		if (Config::AntiAim.FakeWalk)
		{
			this->FakeWalk(cmd, local);
		}
	}
}

// Used to set visual angles in case user wants to see aa angles in thirdperson
void F_HvH::OnFrameStageNotify(Source::ClientFrameStage_t stage)
{
	if (stage == FRAME_RENDER_START)
	{
		auto local = C_CSPlayer::GetLocal();
		if (local)
		{
			// Check if user is in thirdperson
			if (Source::Input->m_fCameraInThirdPerson || Config::VisualsOther.ThirdPerson)
			{
				// Get the offset and set it to my visual angles
				if (Config::FakeLag.Active)
				{
					static offset_t fsnViewAngles = GET_NETVAR(enc("DT_CSPlayer"), enc("deadflag")) + 0x4;
					*(QAngle*)(rcast<uintptr_t>(local) + fsnViewAngles) = global->VisualAngles();
				}
			}
		}
	}
}

// Used to draw angles info
void F_HvH::OnPaintTraverse()
{
	if (Config::AntiAim.Active && Config::AntiAim.AnglesInfo && Source::Engine->IsInGame())
	{
		auto local = C_CSPlayer::GetLocal();
		if (local && local->IsAlive())
		{
			const float DISTANCE = 75.f;
			const Color REAL_CLR = Color::Lime();
			const Color FAKE_CLR = Color::Red();
			const Color LBY_CLR = Color::White();

			float realYaw = this->m_RealAngles[YAW], fakeYaw = this->m_FakeAngles[YAW];
			float myLby = local->m_flLowerBodyYawTarget();

			if (Config::VisualsOther.ThirdPerson || Source::Input->m_fCameraInThirdPerson)
			{
				Vector linesOrigin = local->GetAbsOrigin();

				Vector linesOrigin2D;
				Vector realVector2D, fakeVector2D, lbyVector2D;
				if (Render::WorldToScreen(linesOrigin, linesOrigin2D))
				{
					Vector forward;

					Maths::AngleVectors(QAngle(0, realYaw, 0), &forward);
					Vector realVector = linesOrigin + (forward * DISTANCE);
					if (Render::WorldToScreen(realVector, realVector2D))
					{
						Render::Line(
							Vector2(linesOrigin2D.x, linesOrigin2D.y),
							Vector2(realVector2D.x, realVector2D.y), REAL_CLR);
					}

					Maths::AngleVectors(QAngle(0, fakeYaw, 0), &forward);
					Vector fakeVector = linesOrigin + (forward * DISTANCE);
					if (Render::WorldToScreen(fakeVector, fakeVector2D))
					{
						Render::Line(
							Vector2(linesOrigin2D.x, linesOrigin2D.y),
							Vector2(fakeVector2D.x, fakeVector2D.y), FAKE_CLR);
					}

					Maths::AngleVectors(QAngle(0, myLby, 0), &forward);
					Vector lbyVector = linesOrigin + (forward * DISTANCE);
					if (Render::WorldToScreen(lbyVector, lbyVector2D))
					{
						Render::Line(
							Vector2(linesOrigin2D.x, linesOrigin2D.y),
							Vector2(lbyVector2D.x, lbyVector2D.y), LBY_CLR);
					}
				}
			}

			// I consider my lby broken af if the delta between curtime and lastlbyupdatetime is >= 1.1f
			bool breakingLby = fabs(Source::Globals->cur_time - m_flLastLBYUpdate) >= 1.1f;
			const unsigned int DRAW_START_OFFSET = 400;
			const unsigned int DRAW_SPACING = 20;
			int off = 0;

			String(Render::Fonts::Arial_Outline15, 0, DRAW_START_OFFSET - DRAW_SPACING, breakingLby ? Color::Lime() : Color::Red(), Render::eAlign::ALIGN_LEFT, breakingLby ? enc("LBY IS SAFE") : enc("LBY IS NOT SAFE"));
			String(Render::Fonts::Arial_Outline15, 0, DRAW_START_OFFSET, Color::Orange(), Render::eAlign::ALIGN_LEFT, enc("Last LBY Update: %f"), m_flLastLBYUpdate);
			String(Render::Fonts::Arial_Outline15, 0, DRAW_START_OFFSET + (++off * DRAW_SPACING), REAL_CLR, Render::eAlign::ALIGN_LEFT, enc("Real: %.2f"), realYaw);
			String(Render::Fonts::Arial_Outline15, 0, DRAW_START_OFFSET + (++off * DRAW_SPACING), FAKE_CLR, Render::eAlign::ALIGN_LEFT, enc("Fake: %.2f"), fakeYaw);
			String(Render::Fonts::Arial_Outline15, 0, DRAW_START_OFFSET + (++off * DRAW_SPACING), LBY_CLR, Render::eAlign::ALIGN_LEFT, enc("LBY: %.2f"), myLby);
			String(Render::Fonts::Arial_Outline15, 0, DRAW_START_OFFSET + (++off * DRAW_SPACING), Color::Lime(), Render::eAlign::ALIGN_LEFT, enc("Max Backtrack Tick: %d"), Backtrack->GetMaxBacktrackTick());
		}
	}
}

// Used to draw model for fake angles chams
void F_HvH::OnSceneEnd()
{
	if (Config::AntiAim.FakeAnglesChams)
	{
		if (Config::VisualsOther.ThirdPerson || Source::Input->m_fCameraInThirdPerson)
		{
			auto local = C_CSPlayer::GetLocal();
			if (local)
			{
				IMaterial* fakeAnglesMat = Chams->GetFakeAnglesMaterial();
				if (fakeAnglesMat)
				{
					QAngle origAngle = local->GetAbsAngles();
					QAngle modAngle = QAngle(0, m_FakeAngles.y, 0);

					local->SetAbsAngles(&modAngle);
					Source::ModelRender->ForcedMaterialOverride(fakeAnglesMat);
					local->GetRenderable()->DrawModel(STUDIO_RENDER, 0xFF);
					Source::ModelRender->ForcedMaterialOverride(nullptr);
					local->SetAbsAngles(&origAngle);
				}
			}
		}
	}
}

// Used to detect the last LBY update time for local player
void F_HvH::OnLBYProxy(Source::C_CSPlayer* updater, float newValue)
{
	auto local = C_CSPlayer::GetLocal();
	if (updater == local)
	{
		m_flLastLBYUpdate = local->m_flSimulationTime();
	}
}

// Used to reset stuff
void F_HvH::OnListenedEvent(Source::IGameEvent* game_event)
{
	if (DID_DISCONNECT())
	{
		m_flLastLBYUpdate = 0.f;
		Resolver->ResetAllBacktrackedTicks();
	}
}

// AntiAim for Pitch
float F_HvH::GetPitch(Source::CUserCmd* cmd)
{
	switch (Config::AntiAim.Pitch)
	{
	case 0: // None
		return cmd->viewangles.x;
	case 1: // Emotion
		return m_bFlip ? 89.f : -89.f;
	case 2: // Up
		return -89.f;
	case 3: // Down
		return 89.f;
	case 4: // Overflow Down
		return -1080.f;
	case 5: // Overflow Center
		return -991.f;
	case 6: // Random
		return Maths::RandomFloat(-89.f, 89.f);
	case 7: // Custom
		return Config::AntiAim.CustomPitch;
	default: return cmd->viewangles.x;
	}
}

// AntiAim for Real Yaw
float F_HvH::GetRealYaw(Source::CUserCmd* cmd)
{
	auto local = C_CSPlayer::GetLocal();
	float jitter_range = Config::AntiAim.JitterRange.m_Value;
	float custom_spin = Config::AntiAim.SpinSpeed.m_Value;
	float lby_delta = Config::AntiAim.LowerBodyDelta.m_Value;
	float server_time = local->m_nTickBase() * Source::Globals->interval_per_tick;

	// LBY flipping : flips whenever LBY is updated
	static bool flipLower = false;
	static float oldLBY = 0.f;
	float lby = local->m_flLowerBodyYawTarget();
	if (lby != oldLBY)
	{
		flipLower = !flipLower;
		oldLBY = lby;
	}

	float yaw = cmd->viewangles.y;

	switch (Config::AntiAim.RealYaw)
	{
	case 0: // None
		return yaw;
	case 1: // Backwards
		return yaw + 180.f;
	case 2: // Jitter
		return yaw + (m_bFlip ? -jitter_range : +jitter_range);
	case 3: // Backwards Jitter
		return yaw + (m_bFlip ? -jitter_range : +jitter_range) + 180.f;
	case 4: // Sideways
		return yaw - 90.f;
	case 5: // Fast Spin
		return fmod(Source::Globals->cur_time / 0.07f * 360.f, 360.f);
	case 6: // Slow Spin
		return fmod(Source::Globals->cur_time / 0.8f * 360.f, 360.f);
	case 7: // Custom Spin
		return fmod(Source::Globals->cur_time / custom_spin * 360.f, 360.f);
	case 8: // Real
		return yaw;
	case 9: // Fake Lower Body
		return flipLower ?
			yaw + lby_delta :
			yaw - lby_delta;
	case 10: // Paranoia
		return yaw + (server_time * 240.f);
	case 11: // Mirror
		return yaw; // Works only while faking
	case 12: // SMART
		return GetSmartYaw();
	default: return yaw;
	}
}

// AntiAim for Fake Yaw
float F_HvH::GetFakeYaw(Source::CUserCmd* cmd)
{
	auto local = C_CSPlayer::GetLocal();
	float jitter_range = Config::AntiAim.JitterRange.m_Value;
	float custom_spin = Config::AntiAim.SpinSpeed.m_Value;
	float lby_delta = Config::AntiAim.LowerBodyDelta.m_Value;
	float server_time = local->m_nTickBase() * Source::Globals->interval_per_tick;

	// LBY flipping : flips whenever LBY is updated
	static bool flipLower = false;
	static float oldLBY = 0.f;
	float lby = local->m_flLowerBodyYawTarget();
	if (lby != oldLBY)
	{
		flipLower = !flipLower;
		oldLBY = lby;
	}

	switch (Config::AntiAim.FakeYaw)
	{
	case 0: // None
		return cmd->viewangles.y;
	case 1: // Backwards
		return cmd->viewangles.y + 180.f;
	case 2: // Jitter
		return cmd->viewangles.y + (m_bFlip ? -jitter_range : +jitter_range);
	case 3: // Backwards Jitter
		return cmd->viewangles.y + (m_bFlip ? -jitter_range : +jitter_range) + 180.f;
	case 4: // Sideways
		return cmd->viewangles.y - 90.f;
	case 5: // Fast Spin
		return fmod(server_time / 0.07f * 360.f, 360.f);
	case 6: // Slow Spin
		return fmod(server_time / 0.8f * 360.f, 360.f);
	case 7: // Custom Spin
		return fmod(server_time / custom_spin * 360.f, 360.f);
	case 8: // Real
		return cmd->viewangles.y;
	case 9: // Fake Lower Body
		return flipLower ?
			cmd->viewangles.y + lby_delta :
			cmd->viewangles.y - lby_delta;
	case 10: // Paranoia
		return cmd->viewangles.y + server_time * 240.f;
	case 11: // Mirror
		return Rage->GetBestTarget().IsValid() ? Rage->GetBestTarget().GetEntity()->m_angEyeAngles().y : 0.f;
	case 12: // SMART
		return cmd->viewangles.y; // Smart works only for real yaw!
	default: return cmd->viewangles.y;
	}
}

// Aim at targets
void F_HvH::AtTargets(Source::C_CSPlayer* local)
{
	Vector localEyePos = local->GetEyePosition();

	int bestDistance = INT_MAX;
	Vector targetPos;

	FOR_EACH_CLIENT
	{
		auto* player = GET_ENTITY(ClientIndex);

		if (!player->IsValid())
			continue;

		Vector headPos = player->GetHitboxPosition(HITBOX_HEAD);

		float dist = localEyePos.DistTo(headPos);

		if (dist < bestDistance)
		{
			bestDistance = dist;
			targetPos = headPos;
		}
	}

	if (targetPos.Length() > 0)
	{
		QAngle Target = Maths::GetAngle(localEyePos, targetPos);
		//m_RealAngles = QAngle(m_RealAngles.x, Target.y + 180, 0);
		m_FakeAngles = QAngle(m_FakeAngles.x, Target.y + 180, 0);
	}
}

// Edge AA (the infamous one)
void F_HvH::EdgeAntiAim(Source::CUserCmd* cmd, Source::C_CSPlayer* local)
{
	// Straight outta CoreHook
	float closest_distance = FLT_MAX;
	for (int i = 0; i < 360; i += 6)
	{
		Vector forward, eye_pos, end_pos;
		trace_t trace;
		Ray_t ray;
		CTraceFilter filter; filter.pSkip = local;

		QAngle myAngles = cmd->viewangles;
		myAngles.x = 0;
		myAngles.y = i;

		Maths::AngleVectors(myAngles, &forward);
		Vector src = local->GetAbsOrigin() + Vector(0, 0, 20.f);
		Vector dest = src + forward * 25.f;

		ray.Init(src, dest);
		EngineTrace->TraceRay(ray, MASK_SHOT, &filter, &trace);

		float distance = src.DistTo(trace.endpos);

		if (distance < 24) // Magic number!
		{
			if (distance < closest_distance)
			{
				closest_distance = distance;
				m_RealAngles.y = i;
				m_FakeAngles.y = i + (m_bFlip ? 90.f : -90.f);
			}
		}
	}
}

float F_HvH::GetSmartYaw()
{
	// Credits: C0re
	struct Gap
	{
		int From;
		int To;
	};
	auto lGetCircleIntersect = [](Vector2 circlePos, float radius, Vector2 lineStart, Vector2 lineEnd, Vector2& firstIntersect, Vector2& secondIntersect) 
		-> int
	{
		// This returns the number of intersections found and their positions in the circle
		auto lineDelta = lineEnd - lineStart;
		float t;

		// I'm building an equation y = ax^2 + bx + c
		float a = lineDelta.x * lineDelta.x + lineDelta.y * lineDelta.y;
		float b = 2 * (lineDelta.x * (lineStart.x - circlePos.x) + lineDelta.y * (lineStart.y - circlePos.y));
		float c = (lineStart.x - circlePos.x) * (lineStart.x - circlePos.x) +
				  (lineStart.y - circlePos.y) * (lineStart.y - circlePos.y) - radius * radius;

		float det = b*b - 4*a*c;
		if (a <= 0.0000001 || det < 0)
		{
			firstIntersect = secondIntersect = Vector2();
			return 0; // No intersections found (det < 0)
		}
		else if (det == 0)
		{
			t = -b / (2*a);
			firstIntersect = secondIntersect = Vector2(lineStart.x + t * lineDelta.x, lineStart.y + t * lineDelta.y);
			return 1; // The line is tangent to the circle
		}
		else
		{
			t = (-b + sqrtf(det)) / (2.f*a);
			firstIntersect = Vector2(lineStart.x + t * lineDelta.x, lineStart.y + t * lineDelta.y);
			t = (-b - sqrtf(det)) / (2.f*a);
			secondIntersect = Vector2(lineStart.x + t * lineDelta.x, lineStart.y + t * lineDelta.y);
			return 2; // The line goes through the circle with 2 intersects
		}
	};

	auto local = C_CSPlayer::GetLocal();
	if (!local)
		return 0.f;

	Vector localOrigin = local->m_vecOrigin();
	std::array<bool, 360> blacklistedAngles;
	blacklistedAngles.fill(false);

	FOR_EACH_CLIENT
	{
		auto* player = GET_ENTITY(ClientIndex);
		if (!player || !player->IsValid())
			continue;

		Vector playerOrigin = player->m_vecOrigin();
		Vector forward; Maths::AngleVectors(QAngle(0, m_FakeAngles.y, 0), &forward);

		// This is kinda ghetto gotta find a better way to do that
		float radius = fabs((local->GetEyePosition() - localOrigin).Length()) / 2.f;

		// Get the position of our fake head
		Vector fakeHead = localOrigin + forward * radius;

		// Check if there are intersects
		Vector2 firstIntersect, secondIntersect;
		if (lGetCircleIntersect(
			Vector2(localOrigin.x, localOrigin.y),
			radius,
			Vector2(playerOrigin.x, playerOrigin.y),
			Vector2(fakeHead.x, fakeHead.y),
			firstIntersect, secondIntersect) > 0)
		{
			// Get the bad angles based on intersects
			int badAngleFake = Maths::NormalizeValue(RAD2DEG(atan2(firstIntersect.y - localOrigin.y, firstIntersect.x - localOrigin.x)));
			int badAngleIntersect = Maths::NormalizeValue(RAD2DEG(atan2(secondIntersect.y - localOrigin.y, secondIntersect.x - localOrigin.x)));

			// Remap to value between 0-360 for easier usage in the blacklist array
			badAngleFake = (badAngleFake += 360) % 360;
			badAngleIntersect = (badAngleIntersect += 360) % 360;

			// Blacklist the angle
			blacklistedAngles[badAngleFake] = true;
			blacklistedAngles[badAngleIntersect] = true;
		}
	}

	// Now find the gaps
	std::vector<Gap> gaps;

	// Search for not blacklisted angles
	for (int i = 0; i < 360; i++)
	{
		if (!blacklistedAngles[i])
		{
			if (gaps.empty() || gaps.back().To != 360)
				gaps.push_back(Gap{ i, 360 });
		}
		else if (!gaps.empty())
			gaps.back().To = i;
	}

	// If we found gaps find the best one and use that
	if (!gaps.empty())
	{
		Gap bestGap = Gap{ -1, -1 };
		int highestGapDelta = -1;

		for (Gap gap : gaps)
		{
			int gapDelta = gap.To - gap.From;

			if (gapDelta > highestGapDelta)
			{
				bestGap = gap;
				highestGapDelta = gapDelta;
			}
		}

		return bestGap.From + highestGapDelta / 2;
	}

	// Nothing
	return 0.f;
}

void F_HvH::RunAA(Source::CUserCmd* cmd, Source::C_CSPlayer* local)
{
	// Grab weapon info
	auto* weapon = local->GetActiveWeapon();
	if (!weapon)
		return;

	WeaponInfo_t* weaponInfo = weapon->GetWeaponInfo();
	if (!weaponInfo)
		return;

	// Grab weapon type, buttons and move type
	WeaponTypes weaponType = weaponInfo->WeaponType;
	int moveType = local->GetMoveType();

	// Check for valid weapon types and move types
	if (weaponType == WeaponTypes::WEAPONTYPE_GRENADE && !Config::AntiAim.OnGrenades ||
		weaponType == WeaponTypes::WEAPONTYPE_C4 ||
		weaponType == WeaponTypes::WEAPONTYPE_KNIFE && !Config::AntiAim.OnKnife)
		return;

	if (moveType == MOVETYPE_LADDER && !Config::AntiAim.OnLadder ||
		moveType == MOVETYPE_NOCLIP)
		return;

	if (cmd->buttons & IN_DUCK && moveType == MOVETYPE_LADDER)
	{
		static int ticks = 0;
		if (ticks != 0)
		{
			g_bSendPackets = false;
			ticks--;
		}
		else
		{
			cmd->buttons &= ~IN_DUCK;
			ticks = 4;
			g_bSendPackets = true;
		}
	}

	if (Config::AntiAim.OnGrenades && weaponType == WeaponTypes::WEAPONTYPE_GRENADE)
	{
		bool throwing = rcast<C_BaseCSGrenade*>(weapon)->m_fThrowTime() > 0.f;
		if (throwing)
			return;
	}

	if (g_bSendPackets)
	{
		m_FakeAngles = QAngle(GetPitch(cmd), GetFakeYaw(cmd) + Config::AntiAim.FakeYawMod, 0);
		Maths::NormalizeAngle(m_FakeAngles);
		cmd->viewangles = m_FakeAngles;
	}
	else
	{
		m_RealAngles = QAngle(GetPitch(cmd), GetRealYaw(cmd) + Config::AntiAim.RealYawMod, 0);

		if (Config::AntiAim.AtTargets)
			AtTargets(local);

		if (Config::AntiAim.Edge)
			EdgeAntiAim(cmd, local);

		Maths::NormalizeAngle(m_RealAngles);
		cmd->viewangles = m_RealAngles;
	}
}

void F_HvH::FakeWalk(Source::CUserCmd* cmd, Source::C_CSPlayer* local)
{
	static int choked = 0;
	// Continue only if I'm holding the fakewalk key
	if (Source::InputSystem->IsButtonDown((ButtonCode_t)Config::AntiAim.FakeWalkKey.m_Value))
	{
		choked = choked > 7 ? 0 : choked + 1;
		cmd->forwardmove = choked < 2 || choked > 5 ? 0 : 400;
		cmd->sidemove = choked < 2 || choked > 5 ? 0 : 400;
		g_bSendPackets = choked < 1;
	}
}

void F_HvH::Choke(Source::CUserCmd* cmd, Source::C_CSPlayer* local)
{
	static int nChoked = 0; // if nChoked is 0, I'm not choking any packets. Otherwise I'm choking nChoked packets
	if (Config::FakeLag.Active)
	{
		// If use key is enabled and I'm not pressing the fakelag key, do not fakelag
		if (Config::FakeLag.UseKey)
			if (!Source::InputSystem->IsButtonDown(ButtonCode_t(Config::FakeLag.Key.m_Value)))
				return;

		// If In Air Only is set, check if player is in air
		if (Config::FakeLag.AirOnly)
			if (local->m_fFlags() & FL_ONGROUND)
				return;

		// Init factor to be user-defined factor value
		auto factor = Config::FakeLag.Factor.m_Value;

		// Modify factor based on the mode
		// Static: Nothing changes, factor is still user-defined
		// Random Factor: factor is a random number between 1 and 14
		// Adaptive: factor changes based on the speed
		switch (Config::FakeLag.Mode)
		{
		case 0: // Static
			// Nothing changes, factor is still user-defined
			break;
		case 1: // Random Factor
			// factor is a random number between 1 and 14
			factor = Maths::RandomInt(1, 14);
			break;
		case 2: // Adaptive
			// TODO: Adaptive shit to break lagcomp
			break;
		default:
			break;
		}

		// If number of packets choked is < factor (user-defined), choke
		if (nChoked < factor)
		{
			g_bSendPackets = false; // While choking, bSendPackets is false
			nChoked++; // 1 more packet choked
		}
		else
		{
			g_bSendPackets = true; // bSendPackets is true again
			nChoked = 0;
		}
	}
	else
	{
		// Fakelag is not active, but if antiaims are active, I need to choke 1 packet to fake angles
		if (Config::AntiAim.Active && Config::AntiAim.FakeYaw.m_Value > 0)
			g_bSendPackets = cmd->command_number % 2 == 0;
	}
}

#pragma endregion

#pragma region RESOLVER
F_Resolver* Resolver;
CUserCmd* F_Resolver::UserCmd = nullptr;
const float LBY_TOLLERANCE = 3.f;

static float lastFireTime = -1.f;
int tickHitWall = 0; // Tick where I did hit the world
int tickHitPlayer = 0; // Tick where I did hit a player

void F_Resolver::OnListenedEvent(Source::IGameEvent* game_event)
{
	auto local = C_CSPlayer::GetLocal();
	if (local)
	{
		if (Config::Ragebot.Active && Config::Resolver.Active)
		{
			// Hit/Miss logic goes fully here while Resolver and Ragebot are both active

			// Listen for attacks
			if (EVENT_IS(enc("weapon_fire")))
			{
				auto* player = USERID2PLAYER(game_event->GetInt(enc("userid")));
				if (player // Valid pointer
					&& player == local) // Player who fired is myself
				{
					// Local player did fire a bullet. Register a new bullet and store the time
					m_iFiredBullets++;
					lastFireTime = Source::Globals->cur_time;
				}
			}

			// Listen for bullet impacts
			if (EVENT_IS(enc("bullet_impact")))
			{
				auto* player = USERID2PLAYER(game_event->GetInt(enc("userid")));
				if (player // Valid pointer
					&& player == local) // Player that caused the impact is myself
				{
					int tickCount = Source::Globals->tick_count;
					if (tickCount != tickHitPlayer)
					{
						tickHitWall = tickCount;
						auto target = Rage->GetBestTarget().GetEntity();
						if (target)
						{
							if (local->m_vecVelocity().Length2D() < 35)
							{
								auto* targetRecord = &PlayerList[target->GetIndex()];
								float lby = target->m_flLowerBodyYawTarget();
								// Blacklist the delta lby - hsangle, reset it
								float badDelta = std::abs(lby - targetRecord->HeadshotAngle);
								auto blacklisted = targetRecord->BlacklistedDeltas;
								// If I didn't register this bad delta already
								if (std::find(blacklisted.begin(), blacklisted.end(), badDelta) == blacklisted.end())
								{
									// Register it as a new bad delta
									targetRecord->BlacklistedDeltas.push_back(std::abs(lby - targetRecord->HeadshotAngle));
								}
								// Reset headshot angle
								targetRecord->HeadshotAngle = 0.f;
								// Count this as a new missed shot
								PlayerList[target->GetIndex()].MissedShots++;
							}
						}
					}
				}
			}

			// Listen for hurts
			if (EVENT_IS(enc("player_hurt")))
			{
				auto* target = USERID2PLAYER(game_event->GetInt(enc("userid")));
				auto* attacker = USERID2PLAYER(game_event->GetInt(enc("attacker")));
				if (target && attacker // Valid pointers
					&& attacker == local // Attacker is myself
					&& game_event->GetInt(enc("hitgroup")) == HITGROUP_HEAD) // I did an headshot
				{
					// The hurt is caused by the local player
					int tickCount = Source::Globals->tick_count;
					if (tickCount != tickHitPlayer)
					{
						auto* targetRecord = &PlayerList[target->GetIndex()];
						tickHitPlayer = tickCount;
						targetRecord->HitgroupHit = HITGROUP_HEAD;
						targetRecord->HeadshotAngle = target->m_angEyeAngles().y;
						targetRecord->MissedShots = 0;
					}
				}
			}

		}
	}
	if (DID_DISCONNECT())
	{
		// Once we disconnect from a server, make sure to reset everything
		m_iFiredBullets = 0; // Reset fired bullets
		BacktrackedTicks.fill(TickRecord());
	}
}

void F_Resolver::OnFrameStageNotify(Source::ClientFrameStage_t stage)
{
	if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		auto local = C_CSPlayer::GetLocal();
		if (local == nullptr)
			return;

		FOR_EACH_CLIENT
		{
			// Get the target
			auto* target = GET_ENTITY(ClientIndex);
			if (target && target != local && target->IsAlive() && target->m_iTeamNum() != local->m_iTeamNum())
			{
				// Store backtrack ticks for him
				Backtrack->StorePlayer(target);
				
				if (Config::Resolver.Active)
				{
					// Perform resolver
					float resolvedYaw = RESOLVE_SmartYaw(local, target);
					resolvedYaw = Maths::NormalizeValue(resolvedYaw);
					QAngle* targetAngles = target->m_angEyeAnglesPtr();
					if (targetAngles)
					{
						targetAngles->y = resolvedYaw;
						// Useless stuff. Setting yaw angles is just enough to hit
						//target->SetPoseAngles(targetAngles->x, targetAngles->y);
						//target->SetAbsAngles(&QAngle(0, resolvedYaw, 0));
						//*target->m_angRotationPtr() = QAngle(0, 0, 0);
						//target->m_angAbsRotation() = QAngle(0, 0, 0);
					}
				}

				// This is now the previous tick record
				PlayerList[ClientIndex].m_prevTickRecord = PlayerList[ClientIndex].m_currTickRecord;
			}
		}

		// End of Resolver FSN behaviour
	}
}

void F_Resolver::OnFrameStageNotifyPostOriginal(Source::ClientFrameStage_t stage)
{
}

void F_Resolver::OnPaintTraverse()
{
	// Dump my own anim layers
	/*auto local = C_CSPlayer::GetLocal();
	FOR_EACH_CLIENT
	{
		auto ent = GET_ENTITY(ClientIndex);
		if (ent && ent != local && ent->m_iTeamNum() != Teams::TEAM_SPEC)
		{
			for (int j = 0; j < C_BaseAnimatingOverlay::MAX_OVERLAYS; j++)
			{
				std::string layerDump = ent->DumpAnimOverlay(j);
				Render::String(Render::Fonts::Tahoma_Outline12, 300, 0 + (j * 12), Color::White(), Render::eAlign::ALIGN_LEFT, layerDump.c_str());
			}
		}
	}*/
}

void F_Resolver::OnCreateMove(CUserCmd* cmd)
{
	UserCmd = cmd;
}

void F_Resolver::OnLBYProxy(Source::C_CSPlayer* updater, float newValue)
{
}

float F_Resolver::RESOLVE_SmartYaw(Source::C_CSPlayer* local, Source::C_CSPlayer* target)
{
	// TODO: SMART YAW
	// SmartYaw is the next gen resolver. 
	// It is even able to resolve and compute the exact amount of ayyware pasters' autism

	float lby = target->m_flLowerBodyYawTarget();
	Vector targetVelocity = target->m_vecVelocity();
	Vector currentAngles = target->m_angEyeAngles();
	float simulationTime = target->m_flSimulationTime();
	float resolvedYaw = currentAngles.y;

	auto* targetRecord = &PlayerList[target->GetIndex()];
	if (targetRecord == nullptr)
		return resolvedYaw;

	auto resolverInfo = targetRecord->ResolveInfo;

	// First of all determine if the target is effectively using fake angles or antiaiming in general
	// Atm I don't have a reliable way to check this (I'm studying a way involving simtime check)
	// This var is kind of important because the body aim imposition depends on it
	//bool IsAntiAiming = Maths::NormalizeAngle(currentAngles).x > 60.f; // GHETTO AS SHIT ç___ç
	//if (!IsAntiAiming && targetRecord->ResolveInfo.ShouldBodyAim)
	//	targetRecord->ResolveInfo.ShouldBodyAim = false;

	// Stage 1: Search for a tick where target was moving and not fakewalking or a tick where delta > 35
	if (targetRecord->m_prevTickRecord.Velocity.Length2D() == 0.f && targetRecord->m_currTickRecord.Velocity.Length2D() != 0.f)
	{
		// Target started moving! Store direction
		targetRecord->m_angMovingDirection = Maths::VectorAngles(targetRecord->m_currTickRecord.Velocity);
	}

	for (auto& r : targetRecord->Records)
	{
		// This tick is not valid
		if (!F_Backtrack::IsRecordValid(r))
			continue;

		// If I can't damage the head on this tick keep searching
		//Tools::CAutowall::OverrideData od;
		//od.entity = target;
		//od.hitgroup = HITGROUP_HEAD;
		//od.position = r.HeadPos;
		//int hitgroup = 0;
		//bool didPenetrate = false;
		//float damage = Tools::Autowall->GetDamage(
		//	local->GetEyePosition(),
		//	Maths::GetAngle(local->GetEyePosition(), r.HeadPos),
		//	local,
		//	HITBOX_HEAD,
		//	hitgroup,
		//	didPenetrate,
		//	!Config::RageTarget.TeamMates,
		//	true, // I want to override with my data
		//	od
		//);
		//if (damage <= 0)
		//	continue; // Can't damage the head!
		if (!g_GameHelper.IsPointVisible(r.HeadPos))
			continue;
		
		if (r.IsMoving && r.Flags & FL_ONGROUND)
		{
			// Was moving on this tick, now check if fakewalking
			float yaw = lby;
			targetRecord->ResolveInfo.ResolveStage = enc("BT LBY Moving");
			//if (r.Velocity.Length2D() > 6.f && r.Velocity.Length2D() < 45.f && !(r.Flags & FL_DUCKING))
			//{
			//	// Fakewalking! TODO: PROPER CHECK THIS IS GHETTO
			//	targetRecord->ResolveInfo.ResolveStage = enc("BT LBY Fakewalking");
			//	yaw = targetRecord->m_angMovingDirection.y + 180.f;
			//}
			// Set target angles to lby (or flipped lby) directly and inform Backtrack that we need to backtrack here
			Backtrack->BacktrackToTick(r.TickCount);
			// Inform Ragebot to use this backtracked tick
			BacktrackedTicks[target->GetIndex()] = r;
			r.Resolved = true;
			resolvedYaw = yaw;
			return resolvedYaw;
		}

		// If I still didn't manage to find the lby delta through animations use normal check
		if (fabsf(r.Angles->y - r.LowerBodyYaw) > 35.f)
		{
			// Found! Backtrack here
			targetRecord->ResolveInfo.ResolveStage = enc("BT LBY Delta");
			Backtrack->BacktrackToTick(r.TickCount);
			// Inform Ragebot to use this backtracked tick
			BacktrackedTicks[target->GetIndex()] = r;
			r.Resolved = true;
			resolvedYaw = r.Angles->y - r.LowerBodyYaw;
			return resolvedYaw;
		}
	}

	// If we are here it means we didn't find a moving valid and damageable tick to backtrack to
	// Stage 2: Animation Layers analysis a.k.a the Sexy AnimLayer Resolver (codename: SAR)
	static C_AnimationLayer previousLayer;
	for (int layer_index = 0; layer_index < C_BaseAnimatingOverlay::MAX_OVERLAYS; layer_index++)
	{
		// Get the current animation layer
		auto currentLayer = target->GetAnimOverlay(layer_index);

		// Determine current activity and previous activity
		const auto currentActivity = target->GetSequenceActivity(currentLayer.m_nSequence);
		const auto previousActivity = target->GetSequenceActivity(previousLayer.m_nSequence);

		// Get current cycle and weight
		float cycle = currentLayer.m_flCycle, weight = currentLayer.m_flWeight;
		float previousCycle = previousLayer.m_flCycle, previousWeight = previousLayer.m_flWeight;

		if (currentLayer.m_nOrder == 3)
		{
			if (currentActivity == Activity::ACT_CSGO_IDLE_TURN_BALANCEADJUST)
			{
				if (previousActivity == Activity::ACT_CSGO_IDLE_TURN_BALANCEADJUST 
					&& cycle != previousCycle || weight == 1.f)
				{
					if (F_Backtrack::IsTimeValid(simulationTime - cycle) && cycle < 0.01f && previousCycle > 0.01f)
					{
						// LBY flicked here: I can inform Backtrack to backtrack here
						targetRecord->ResolveInfo.ResolveStage = enc("BT LBY Flick");
						Backtrack->BacktrackToTick(TIME_TO_TICKS(simulationTime - cycle));
						resolvedYaw = lby;
						previousLayer = currentLayer;
						return resolvedYaw;
					}
					// I can just flip lby at this point NOT SURE IF WORKING!
					targetRecord->ResolveInfo.ResolveStage = enc("Flip LBY");
					resolvedYaw = lby - 180.f;
				}
				else if (weight == 0.f && (previousCycle > 0.92f && cycle > 0.92f))
				{
					// Breaking LBY with a delta < 120. Can fakewalk here as well!
					// We need to "logically resolve" him here or just bruteforce him to attempt every single delta
					// Personally, I'm just bruteforcing the target here based on logged hits and stuff like that!
					// Deltas go: 0, 40, 80, 120
					// TODO: Improve this part
					static const std::array<float, 4> deltas_bruteforce = { 0, 40, 80, 120 };
					std::vector<float> possible_deltas = { 0, 40, 80, 120 };
					if (!targetRecord->BlacklistedDeltas.empty())
					{
						targetRecord->ResolveInfo.ResolveStage = enc("Delta < 120 (LIST)");
						// Next level bruteforcing? Lmao
						targetRecord->BlacklistedDeltas.unique([](float first, float second) {
							return scast<int>(first) == scast<int>(second);
						});
						for (auto i = possible_deltas.begin(); i < possible_deltas.end();)
						{
							auto possible = *i;
							bool erased = false;
							for (auto blacklisted : targetRecord->BlacklistedDeltas)
							{
								auto it = std::lower_bound(possible_deltas.begin(), possible_deltas.end(), possible);
								if (it != possible_deltas.end()
									&& std::abs(possible - blacklisted) <= 5)
								{
									i = possible_deltas.erase(it);
									erased = true;
								}
							}
							if (!erased)
								++i;
						}
					}
					if (possible_deltas.size() > 0)
					{
						float possibleDelta = possible_deltas[m_iFiredBullets % possible_deltas.size()];
						float delta = m_iFiredBullets % 2 == 0 ? -possibleDelta : possibleDelta;
						targetRecord->ResolveInfo.LBYDelta = delta;
						resolvedYaw = lby - delta;
						previousLayer = currentLayer;
						return resolvedYaw;
					}
					else
					{
						// No possible deltas, proceed to normal bruteforce
						targetRecord->ResolveInfo.ResolveStage = enc("Delta < 120 (BRUTEFORCE)");
						float possibleDelta = deltas_bruteforce[m_iFiredBullets % deltas_bruteforce.size()];
						float delta = m_iFiredBullets % 2 == 0 ? -possibleDelta : possibleDelta;
						targetRecord->ResolveInfo.LBYDelta = delta;
						resolvedYaw = lby - delta;
						previousLayer = currentLayer;
						return resolvedYaw;
					}
				}
			}
			previousLayer = currentLayer;
		}

		// THE CHECK FOR SHOOTING HAS BEEN TEMPORARLY DISABLED
		//if (currentActivity == Activity::ACT_CSGO_FIRE_PRIMARY && weight > 0.f)
		//{
		//	if (cycle > 0.f && cycle <= 0.9f)
		//	{
		//		// We can safely assume the target is shooting. This means that he has disabled antiaims!
		//		// So we can just calculate the angle from us and the target and that will be his real angle
		//		// TODO: Most cheats are choking while shooting, so this won't be pretty accurate on them.
		//		// Set target angles to calculated angles and inform Backtrack that we need to backtrack here
		//		if (F_Backtrack::IsTimeValid(simulationTime - cycle))
		//		{
		//			targetRecord->ResolveInfo.ResolveStage = enc("Backtrack Shoot");
		//			Backtrack->BacktrackToTick(TIME_TO_TICKS(simulationTime - cycle));
		//			// The angle I need to calculate goes from target head position to my eye position.
		//			// I only account for yaw
		//			QAngle angle = Maths::GetAngle(target->GetHitboxPosition(HITBOX_HEAD), local->GetEyePosition());
		//			resolvedYaw = angle.y;
		//		}
		//	}
		//}
	}

	// At this point I should have a resolvedYaw different from current yaw
	// If it's not the case, then ... *sigh* ... bruteforce
	if (resolvedYaw == currentAngles.y)
	{
		// This bruteforce part is straight from Pycache!
		/* For now I won't bruteforce since this method is shit
		targetRecord->ResolveInfo.ResolveStage = enc("Bruteforce");
		auto lGetDelta = [](float a, float b)
		{
			int x = scast<int>(fabsf(b - a)) % 360;
			return x > 180 ? 360 - x : x;
		};
		int average = 1, count = 1;
		float prevLby = 0.f;
		for (auto& r : targetRecord->Records)
		{
			if (r.LowerBodyYaw == prevLby)
				continue;
			prevLby = r.LowerBodyYaw;
			average += lGetDelta(currentAngles.y, r.LowerBodyYaw);
			count++;
		}
		average /= count;
		float add = 0;
		if (average > 145) add = 180;
		else if (average > 110) add = 135;
		else if (average > 75) add = 90;
		else if (average > 40) add = 45;
		resolvedYaw = UserCmd->command_number % 2 ? add : -add;
		*/
		// Just body aim in this case. I will inform Ragebot
		targetRecord->ResolveInfo.ResolveStage = enc("Body Aim");
		targetRecord->ResolveInfo.ShouldBodyAim = true;
		return resolvedYaw;
	}

	targetRecord->ResolveInfo.ResolveStage = enc("Not Resolved");
	return resolvedYaw;
}

#pragma endregion

#pragma region BACKTRACK
F_Backtrack* Backtrack;
std::unordered_map<INDEX, PlayerRecord> PlayerList;

// Static members from F_Backtrack
CUserCmd* F_Backtrack::UserCmd = nullptr;
ConVar* F_Backtrack::sv_maxunlag = nullptr;
ConVar* F_Backtrack::cl_interp_ratio = nullptr;
ConVar* F_Backtrack::cl_updaterate = nullptr;
ConVar* F_Backtrack::sv_client_min_interp_ratio = nullptr;
ConVar* F_Backtrack::sv_client_max_interp_ratio = nullptr;
ConVar* F_Backtrack::sv_minupdaterate = nullptr;
ConVar* F_Backtrack::sv_maxupdaterate = nullptr;
ConVar* F_Backtrack::cl_interp = nullptr;
ConVar* F_Backtrack::cl_interpolate = nullptr;

void F_Backtrack::GrabServerVars()
{
	if (sv_maxunlag == nullptr)
		sv_maxunlag = Source::CVar->FindVar(enc("sv_maxunlag"));
	if (cl_interp_ratio == nullptr)
		cl_interp_ratio = Source::CVar->FindVar(enc("cl_interp_ratio"));
	if (cl_updaterate == nullptr)
		cl_updaterate = Source::CVar->FindVar(enc("cl_updaterate"));
	if (sv_client_min_interp_ratio == nullptr)
		sv_client_min_interp_ratio = Source::CVar->FindVar(enc("sv_client_min_interp_ratio"));
	if (sv_client_max_interp_ratio == nullptr)
		sv_client_max_interp_ratio = Source::CVar->FindVar(enc("sv_client_max_interp_ratio"));
	if (sv_minupdaterate == nullptr)
		sv_minupdaterate = Source::CVar->FindVar(enc("sv_minupdaterate"));
	if (sv_maxupdaterate == nullptr)
		sv_maxupdaterate = Source::CVar->FindVar(enc("sv_maxupdaterate"));
	if (cl_interp == nullptr)
		cl_interp = Source::CVar->FindVar(enc("cl_interp"));
	if (cl_interpolate == nullptr)
		cl_interpolate = Source::CVar->FindVar(enc("cl_interpolate"));
	// Add something else here, if needed
}

void F_Backtrack::OnListenedEvent(Source::IGameEvent* game_event)
{
	if (DID_DISCONNECT())
	{
		// Reset the server vars so we can grab them again when we will reconnect to some other server
		sv_maxunlag = nullptr;
		cl_interp_ratio = nullptr;
		cl_updaterate = nullptr;
		sv_client_min_interp_ratio = nullptr;
		sv_client_max_interp_ratio = nullptr;
		sv_minupdaterate = nullptr;
		sv_maxupdaterate = nullptr;
		cl_interp = nullptr;
		cl_interpolate = nullptr;
		PlayerList.clear();
	}
}

float F_Backtrack::GetLerpTime()
{
	// This function will return the interpolation time, if interpolation needs to be accounted for.
	if (cl_interpolate->GetFloat() != 0.f)
	{
		float updateRate = cl_updaterate->GetFloat();
		if (sv_maxupdaterate->GetFloat() <= updateRate)
			updateRate = sv_maxupdaterate->GetFloat();

		if (sv_minupdaterate->GetFloat() > updateRate)
			updateRate = sv_minupdaterate->GetFloat();

		float lerpRatio = cl_interp_ratio->GetFloat();
		if (lerpRatio == 0)
			lerpRatio = 1.0f;

		if (sv_client_max_interp_ratio 
			&& sv_client_min_interp_ratio 
			&& sv_client_min_interp_ratio->GetFloat() != -1)
		{
			if (sv_client_min_interp_ratio->GetFloat() > lerpRatio)
				lerpRatio = sv_client_min_interp_ratio->GetFloat();

			if (lerpRatio > sv_client_max_interp_ratio->GetFloat())
				lerpRatio = sv_client_max_interp_ratio->GetFloat();
		}
		else
		{
			if (lerpRatio == 0)
				lerpRatio = 1.0f;
		}

		float lerpAmount = cl_interp->GetFloat();

		if (updateRate != 0)
		{
			float div = lerpRatio / updateRate;
			if (div > lerpAmount)
				lerpAmount = div;
		}

		return lerpAmount;
	}
	return 0.f;
}

float F_Backtrack::GetLatency()
{
	auto* netChannel = Source::Engine->GetNetChannelInfo();
	if (netChannel == nullptr)
		return -1.f;

	float avgLatency = netChannel->GetAvgLatency(FLOW_OUTGOING);
	avgLatency = max(0, avgLatency);

	return avgLatency;
}

bool F_Backtrack::IsTimeValid(float time)
{
	// Given a tick, determine if the time is VALID -> NOT OUTDATED! (out of the backtracking window)
	// Credits: big boi polak https://www.unknowncheats.me/forum/1887138-post14.html
	// NOTICE THAT HE'S DOING += FLOW_INCOMING TOO. WTF? WHY?
	auto* netChannel = Source::Engine->GetNetChannelInfo();
	if (netChannel == nullptr)
		return false;

	float correct = 0.f;
	correct += netChannel->GetLatency(FLOW_OUTGOING);	// Account for my outgoing latency
	correct += netChannel->GetLatency(FLOW_INCOMING);	// Account for my incoming latency
	correct += GetLerpTime(); 							// Account for interpolation time if needed

	// Clamp the corretion value between 0 and server defined maximum (usually 1)
	correct = clamp<float, float>(correct, 0.f, sv_maxunlag->GetFloat());

	// Get the time between correction and tick
	float deltaTime = correct - time;
	if (fabsf(deltaTime) > 0.2f)
	{
		// This record is not valid!
		return false;
	}

	// This record is valid :)
	return true;
}

bool F_Backtrack::IsRecordValid(const TickRecord& record)
{
	return IsTimeValid(Source::Globals->cur_time - record.SimulationTime);
}

void F_Backtrack::StorePlayer(Source::C_CSPlayer* player)
{
	if (player && player->IsValid())
	{
		int index = player->GetIndex();
		// If player didn't change simulation time since last time I recorded or didn't change origin, skip this
		if (PlayerList[index].Records.size() > 0)
		{
			auto newest = PlayerList[index].Records.back();
			if (newest.SimulationTime == player->m_flSimulationTime() && newest.Origin == player->m_vecOrigin())
				return;
		}

		bool onGround = (player->m_fFlags() & FL_ONGROUND);
		bool isMoving = (player->m_vecVelocity().Length2D() > 1.f);
		float oldLby = -1.f;
		if (!PlayerList[index].Records.empty())
		{
			// Set the oldest lby
			oldLby = PlayerList[index].Records.back().LowerBodyYaw;
		}

		// Construct a new tick record and update player list
		TickRecord tickRecord;

		tickRecord.Origin = player->m_vecOrigin();
		tickRecord.Angles = player->m_angEyeAnglesPtr();
		tickRecord.SimulationTime = player->m_flSimulationTime();
		tickRecord.LowerBodyYaw = player->m_flLowerBodyYawTarget();
		for (int i = 0; i < C_BaseAnimating::NUM_POSEPAREMETERS; i++)
			tickRecord.PoseParameters[i] = player->m_flPoseParameter()[i];
		for (int j = 0; j < C_BaseAnimatingOverlay::MAX_OVERLAYS; j++)
			tickRecord.AnimationLayers[j] = player->GetAnimOverlay(j);
		tickRecord.Flags = player->m_fFlags();
		tickRecord.TickCount = TIME_TO_TICKS(tickRecord.SimulationTime);
		player->GetRenderable()->SetupBones(tickRecord.BoneMatrix.data(), MAXSTUDIOBONES, BONE_USED_BY_HITBOX, Source::Globals->cur_time);
		tickRecord.HeadPos = player->GetHitboxPositionCustom(HITBOX_HEAD, tickRecord.BoneMatrix);
		tickRecord.LBYUpdated = onGround && isMoving || (oldLby == -1.f ? false : oldLby != tickRecord.LowerBodyYaw);
		tickRecord.IsMoving = isMoving;
		tickRecord.Velocity = player->m_vecVelocity();

		// Update PlayerList
		PlayerList[index].Player = player;
		PlayerList[index].Records.push_back(tickRecord);

		// Pop the old records
		while (PlayerList[index].Records.size() > m_iMaxBacktrackTick)
			PlayerList[index].Records.pop_front();

		PlayerList[index].m_currTickRecord = tickRecord;
	}
}

void F_Backtrack::OnFrameStageNotify(Source::ClientFrameStage_t stage)
{
	if (Source::Engine->IsInGame() && Source::Globals->interval_per_tick != 0)
	{
		// Grab server vars if they are null
		GrabServerVars();
		// Determine the max backtracking window
		m_iMaxBacktrackTick = 0.2f / Source::Globals->interval_per_tick - 1;
	}
	if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		// Store the player list here for backtracking purposes!
		// this->StorePlayerList();
		// I don't do this here anymore, I do it in the same loop as the resolver!
		// Check around line 749 to see the call to StorePlayer()
	}
}

void F_Backtrack::OnPaintTraverse()
{
	// Backtrack Skeleton and point
	if (Source::Engine->IsInGame() && PlayerList.size() > 0)
	{
		auto local = C_CSPlayer::GetLocal();
		if (local == nullptr)
			return;

		// Execute backtrack esp on the best entity
		C_CSPlayer* player = nullptr;
		if (Config::Aimbot.Active)
			player = Legit->GetBestTarget().GetEntity();
		else if (Config::Ragebot.Active)
			player = Rage->GetBestTarget().GetEntity();

		if (!player || player == local)
			return;

		// Get the records
		auto& playerRecord = PlayerList[player->GetIndex()];

		// Get model for this player
		auto renderable = player->GetRenderable();
		if (rcast<uintptr_t>(renderable) != 0xE && rcast<uintptr_t>(renderable) != 0xF)
		{
			// For each boneMatrix
			for (auto& record : playerRecord.Records)
			{
				bool condition = false;
				if (Config::Aimbot.Active)
					condition = record.IsMoving;
				else if (Config::Ragebot.Active)
					condition = record.LBYUpdated;

				if (!condition)
					continue;

				Color myColor = Color::Red();
				auto boneMatrix = record.BoneMatrix;
				if (condition && IsRecordValid(record))
					myColor = Color::Lime();

				// Draw the skeleton for this record
				if (Config::Resolver.BacktrackSkeleton)
					Visuals->DrawSkeltal(nullptr, local, player, renderable, boneMatrix, myColor, true);
				// Draw the point for this record
				if (Config::Resolver.BacktrackPoint)
				{
					auto point = player->GetHitboxPositionCustom(m_iBestHitbox, boneMatrix);
					auto point2d = Vector();
					if (Render::WorldToScreen(point, point2d))
						Render::FilledCircle(point2d.x, point2d.y, 10, 64, myColor);
				}
			}
		}
	}
}

void F_Backtrack::OnCreateMove(Source::CUserCmd* cmd)
{
	UserCmd = cmd;
	// Set tick count to be the current tick count (during prediction)
	m_iTickToBacktrack = cmd->tick_count;
}

#pragma endregion

}