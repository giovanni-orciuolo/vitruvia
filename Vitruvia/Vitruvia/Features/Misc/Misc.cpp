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

#include "Misc.h"
#include "../../SourceClasses/C_WeaponC4.h"
using namespace Source;

namespace Vitruvia {

F_Misc* Miscs;
#pragma region MISCS
int flags_backup = 0;

enum States
{
	STATE_BEGIN,
	STATE_DUCKING
};
States current_state;

void F_Misc::OnCreateMovePrePrediction(Source::CUserCmd* cmd)
{
	auto local = C_CSPlayer::GetLocal();
	if (local)
	{
		flags_backup = local->m_fFlags();
		this->KnifeBot(local, cmd);
	}

	this->BunnyHop(cmd);
	this->AutoStrafe(cmd);
	this->ChatSpam();
	//this->NameSpam();
	//this->NameStealer();
	//this->ClanTag(); // < FIX TAG CLAN
	this->AirStuck(cmd);
}

void F_Misc::OnCreateMovePostPrediction(Source::CUserCmd* cmd)
{
}

void F_Misc::OnCreateMove(Source::CUserCmd* cmd)
{
	this->EdgeJump(cmd);
}

void F_Misc::OnPlaySound(const char* file_name)
{
	if (!Source::Engine->IsConnected() && strstr(file_name, enc("accept_beep.wav")))
	{
		this->AutoAccept();
	}
}

void F_Misc::OnListenedEvent(Source::IGameEvent* game_event)
{
	// Try to find GameRules and PlayerResource again
	if (EVENT_IS(enc("player_connect_full")))
	{
		if (GameRules == nullptr)
		{
			GameRules = *(C_CSGameRulesProxy**)(CLIENT_BASE + Vitruvia::Offsets::GameRules);
			GameRules == nullptr ?
				Err(enc("GameRules not found in client.dll")) :
				Log(enc("Found GameRules [0x%X] in client.dll"), rcast<offset_t>(GameRules));
		}
		if (PlayerResource == nullptr)
		{
			PlayerResource = *(C_CSPlayerResource**)(CLIENT_BASE + Vitruvia::Offsets::PlayerResource);
			PlayerResource == nullptr ?
				Err(enc("PlayerResource not found in client.dll")) :
				Log(enc("Found PlayerResource [0x%X] in client.dll"), rcast<offset_t>(PlayerResource));
		}
	}
}

void F_Misc::OnShutdown()
{
	//SetClanTag("", "");
}

void F_Misc::BunnyHop(Source::CUserCmd* cmd)
{
	// BunnyHop logic, SMAC proof
	static bool last_jumped = false;
	static bool should_fake = false;

	auto local = C_CSPlayer::GetLocal();
	if (local && Config::Misc.BunnyHop)
	{
		if (!last_jumped && should_fake)
		{
			should_fake = false;
			cmd->buttons |= IN_JUMP;
		}
		else if (cmd->buttons & IN_JUMP)
		{
			if (local->m_fFlags() & FL_ONGROUND)
			{
				last_jumped = true;
				should_fake = true;
			}
			else
			{
				cmd->buttons &= ~IN_JUMP;
				last_jumped = false;
			}
		}
		else
		{
			last_jumped = false;
			should_fake = false;
		}
	}
}

void F_Misc::EdgeJump(Source::CUserCmd* cmd)
{
	auto local = C_CSPlayer::GetLocal();
	if (local && Config::Misc.EdgeJump)
	{
		auto move_type = local->GetMoveType();
		if (move_type != MOVETYPE_NOCLIP && move_type != MOVETYPE_LADDER)
		{
			switch (current_state)
			{
			case STATE_BEGIN:
				if (flags_backup & FL_ONGROUND && !(local->m_fFlags() & FL_ONGROUND) && !(cmd->buttons & IN_JUMP))
				{
					cmd->buttons |= IN_JUMP;
					current_state = STATE_DUCKING;
				}
				break;
			case STATE_DUCKING:
				if (local->m_fFlags() & FL_ONGROUND)
					current_state = STATE_BEGIN;
				else
					cmd->buttons |= IN_DUCK;
				break;
			}
		}
	}
}

float circle_yaw;
float previous_yaw;
// MAX SPEED AUTOSTRAFE (NOT LEGIT)
void AutoStrafe_Rage(Source::C_CSPlayer* local, Source::CUserCmd* cmd)
{
	auto get_angle_from_speed = [](float speed)
	{
		auto ideal_angle = RAD2DEG(std::atan2(30.f, speed));
		clamp<float, float>(ideal_angle, 0.f, 90.f);
		return ideal_angle;
	};

	auto get_velocity_step = [](Source::C_CSPlayer* local, Vector velocity, float speed, float circle_yaw)
	{
		auto velocity_degree = RAD2DEG(std::atan2(velocity.x, velocity.y));
		auto step = 1.5f;

		Vector start = local->m_vecOrigin(), end = local->m_vecOrigin();

		Ray_t ray;
		CGameTrace trace;
		CTraceFilter filter;

		while (true)
		{
			end.x += (std::cos(DEG2RAD(velocity_degree + circle_yaw)) * speed);
			end.y += (std::sin(DEG2RAD(velocity_degree + circle_yaw)) * speed);
			end *= Source::Globals->frame_time;

			ray.Init(start, end, Vector(-20.f, -20.f, 0.f), Vector(20.f, 20.f, 32.f));
			Source::EngineTrace->TraceRay(ray, CONTENTS_SOLID, &filter, &trace);

			if (trace.fraction < 1.f || trace.allsolid || trace.startsolid)
				break;

			step -= Source::Globals->frame_time;

			if (step == 0.f)
				break;

			start = end;
			velocity_degree += (velocity_degree + circle_yaw);
		}

		return step;
	};

	auto set_button_state = [](CUserCmd* cmd)
	{
		cmd->buttons &= ~(IN_MOVELEFT | IN_MOVERIGHT | IN_FORWARD | IN_BACK);

		if (cmd->sidemove < 0.f)
			cmd->buttons |= IN_MOVELEFT;
		else
			cmd->buttons |= IN_MOVERIGHT;

		if (cmd->forwardmove < 0.f)
			cmd->buttons |= IN_BACK;
		else
			cmd->buttons |= IN_FORWARD;
	};

	if (local && cmd)
	{
		if (!Config::AutoStrafe.GroundStrafe)
			if (local->GetMoveType() != MOVETYPE_WALK || local->m_fFlags() & FL_ONGROUND)
				return; // If this check if enabled, stops you from ground-strafing

		if (Config::AutoStrafe.AirStrafe || Config::AutoStrafe.GroundStrafe)
		{
			auto velocity = local->m_vecVelocity();
			velocity.z = 0;

			static bool flip = false;
			auto turn_direction_modifier = flip ? 1.f : -1.f;
			flip = !flip;

			if (cmd->forwardmove > 0.f)
				cmd->forwardmove = 0.f;

			auto speed = velocity.Length2D();

			// Circle Strafer
			if (Source::InputSystem->IsButtonDown((ButtonCode_t)Config::AutoStrafe.CircleStrafe.m_Value))
			{
				auto ideal_speed_angle = get_angle_from_speed(speed);
				circle_yaw = Maths::NormalizeValue(circle_yaw + ideal_speed_angle);
				auto step = get_velocity_step(local, velocity, speed, ideal_speed_angle);

				if (step != 0.f)
					circle_yaw += (((Source::Globals->frame_time * 128.f) * step) * step);

				cmd->viewangles.y = Maths::NormalizeValue(circle_yaw);
				cmd->sidemove = -450.f;
				set_button_state(cmd);
				return;
			}

			auto ideal_move_angle = RAD2DEG(std::atan2(15.f, speed));
			clamp<float, float>(ideal_move_angle, 0.f, 90.f);

			auto yaw_delta = Maths::NormalizeValue(cmd->viewangles.y - previous_yaw);
			auto abs_yaw_delta = abs(yaw_delta);
			circle_yaw = previous_yaw = cmd->viewangles.y;

			if (yaw_delta > 0.f)
				cmd->sidemove = -450.f;

			else if (yaw_delta < 0.f)
				cmd->sidemove = 450.f;

			if (abs_yaw_delta <= ideal_move_angle || abs_yaw_delta >= 30.f)
			{
				QAngle velocity_angles = Maths::VectorAngles(velocity);

				auto velocity_angle_yaw_delta = Maths::NormalizeValue(cmd->viewangles.y - velocity_angles.y);
				auto velocity_degree = get_angle_from_speed(speed) * Config::AutoStrafe.RetrackSpeed.m_Value;

				if (velocity_angle_yaw_delta <= velocity_degree || speed <= 15.f)
				{
					if (-(velocity_degree) <= velocity_angle_yaw_delta || speed <= 15.f)
					{
						cmd->viewangles.y += (ideal_move_angle * turn_direction_modifier);
						cmd->sidemove = 450.f * turn_direction_modifier;
					}
					else
					{
						cmd->viewangles.y = velocity_angles.y - velocity_degree;
						cmd->sidemove = 450.f;
					}
				}
				else
				{
					cmd->viewangles.y = velocity_angles.y + velocity_degree;
					cmd->sidemove = -450.f;
				}
			}

			set_button_state(cmd);
		}
	}
}
// LEGIT AUTOSTRAFE
void AutoStrafe_Legit(Source::C_CSPlayer* local, Source::CUserCmd* cmd)
{
	if (local && cmd)
	{
		if (Config::AutoStrafe.AirStrafe && !(local->m_fFlags() & FL_ONGROUND))
		{
			if (cmd->mousedx > 1 || cmd->mousedx < -1)
				cmd->sidemove = cmd->mousedx < 0.f ? -450.f : 450.f;
		}

		if (Config::AutoStrafe.GroundStrafe && (local->m_fFlags() & FL_ONGROUND))
		{
			if (cmd->mousedx > 1 || cmd->mousedx < -1)
				cmd->sidemove = cmd->mousedx < 0.f ? -450.f : 450.f;
		}
	}
}

void F_Misc::AutoStrafe(Source::CUserCmd* cmd)
{
	if (Config::Misc.AutoStrafe)
	{
		switch (Config::AutoStrafe.Mode)
		{
		case 0: // Legit
			AutoStrafe_Legit(C_CSPlayer::GetLocal(), cmd);
			break;
		case 1: // Rage
			AutoStrafe_Rage(C_CSPlayer::GetLocal(), cmd);
			break;
		}
	}
}

void F_Misc::AutoAccept()
{
	// Restore the window in case the user tabbed out
	if (GetForegroundWindow() != global->CSGO_Window())
	{
		ShowWindow(global->CSGO_Window(), SW_RESTORE);
		SetActiveWindow(global->CSGO_Window());
	}

	// Accepts
	typedef void(*IsReady_t)();
	static IsReady_t IsReady = (IsReady_t)(Offsets::IsReady);
	IsReady();
}

void F_Misc::ChatSpam()
{
	static const std::vector<std::string> chat_spams
	{
		// Add chat spam lines here
		enc("VITRUVIA.ME || THE ENDS JUSTIFY THE MEANS"),
		enc("VITRUVIA.ME || PRECISION MATTERS"),
		enc("VITRUVIA.ME || NEVER VAC AND YOU KNOW"),
		enc("VITRUVIA.ME || THE SOLUTION TO WIN"),
		enc("VITRUVIA.ME || 777 $$$")
	};

	auto local = C_CSPlayer::GetLocal();
	if (local && Config::Misc.ChatSpam)
	{
		// Valve imposes a limit of chat messages per tick so I need to limit
		static float next_time = 0.f;
		float server_time = local->m_nTickBase() * Source::Globals->interval_per_tick;
		if (next_time < server_time)
		{
			// Setup stringstream to build the chatspam line
			std::stringstream ss;
			ss << enc("say ") << *select_randomly(chat_spams.begin(), chat_spams.end());
			// Say the chat spam line randomly selected
			Source::Engine->ExecuteClientCmd(ss.str().c_str());
		}
	}
}

void F_Misc::NameSpam()
{
	static ConVar* name = Source::CVar->FindVar(enc("name"));
	const char* custom_name = enc("--VITRUVIA.ME");

	if (Source::Engine->IsInGame() && Config::Misc.NameSpam)
	{
		SetName(enc("\n"), name);
		SetName(custom_name, name);
	}
}

void F_Misc::NameStealer()
{
	static ConVar* name = Source::CVar->FindVar(enc("name"));

	if (Source::Engine->IsInGame() && Config::Misc.NameStealer)
	{
		FOR_EACH_CLIENT
		{
			auto player = GET_ENTITY(ClientIndex);
			if (player)
				SetName(player->GetPlayerName().c_str(), name);
		}
	}
}

void F_Misc::NoName()
{
	static bool removed_name = false;
	if (Config::Misc.NoName && !removed_name)
	{
		SetClanTag(enc("\n"), "");
		removed_name = true;
	}
	if (!Config::Misc.NoName && removed_name)
 	{
		SetClanTag("", "");
		removed_name = false;
	}
}

void F_Misc::SpectatorList()
{
	// Little snippeteroni
	auto ObserverModeString = [](ObserverMode_t mode) -> char*
	{
		switch (mode)
		{
		case OBSERVER_NONE:
			return enc("None");
		case OBSERVER_DEATHCAM:
			return enc("DeathCam");
		case OBSERVER_FREEZECAM:
			return enc("FreezeCam");
		case OBSERVER_FIXED:
			return enc("Fixed");
		case OBSERVER_IN_EYE:
			return enc("FirstPerson");
		case OBSERVER_CHASE:
			return enc("ThirdPerson");
		case OBSERVER_OTHER:
			return enc("Other");
		}
		return enc("Unknown");
	};

	// Draws spectator list. This will get called inside RenderMenu() function inside ImGui!
	auto local = C_CSPlayer::GetLocal();
	if (Source::Engine->IsInGame() && local && local->IsAlive())
	{
		FOR_EACH_CLIENT
		{
			auto player = GET_ENTITY(ClientIndex);
			if (player == nullptr)
				continue;
			if (player->IsAlive())
				continue; // Can't spectate and be alive at the same time...
			
			// Get the observer target
			auto obs_target = player->GetObserverTarget();
			if (obs_target == nullptr)
				continue; // This entity isn't spectating anyone!
			if (!obs_target->IsAlive())
				continue; // Can't spectate someone dead
			//if (obs_target->GetNetworkable()->IsDormant())
			//	continue;
			if (obs_target != local)
				continue; // This entity isn't spectating me! (You can disable this check or make the user decide)
			
			ObserverMode_t obs_mode = scast<ObserverMode_t>(player->m_iObserverMode());
			if (obs_mode == OBSERVER_NONE)
				continue; // This entity isn't spectating anyone! (double check cuz I'm paranoid)

			// Normal color is white, yellow if the observer target is the local player (d3d mode)
			auto clr = obs_target == local ? Color::Yellow() : Color::White();

			// Draw this spectator
			ImGui::TextColored(clr.toVec4(), enc("%s > %s [%s]"),
				player->GetPlayerName().c_str(), 
				rcast<C_CSPlayer*>(obs_target)->GetPlayerName().c_str(), 
				ObserverModeString(obs_mode)
			);
		}
	}
}

namespace ClanTags
{
	static const int ANIMATION_SPEED = 650;

	struct Frame
	{
		int time;
		std::wstring text;

		Frame(const std::wstring& text, int time)
		{
			this->text = text;
			this->time = time;
		}
	};

	enum AnimationLoopType
	{
		ANIM_LOOP,
		ANIM_LOOP_BACK
	};

	struct Animation
	{
		std::string name;
		unsigned int currentFrame;
		std::vector<Frame> frames;
		AnimationLoopType loopType;

		Animation(const std::string& name, const std::vector<Frame>& frames, AnimationLoopType loopType)
		{
			currentFrame = 0;
			this->name = name;
			this->frames = frames;
			this->loopType = loopType;
		}

		Frame GetCurrentFrame()
		{
			return frames[currentFrame];
		}

		void NextFrame()
		{
			currentFrame++;

			if (currentFrame >= frames.size())
				currentFrame = 0;
		}
	};

	std::vector<std::wstring> split_words(std::wstring text)
	{
		std::wistringstream stream(text);
		std::wstring word;
		std::vector<std::wstring> words;
		while (stream >> word)
			words.push_back(word);

		return words;
	}

	Animation Marquee(const std::string& name, std::wstring text, int width = 15)
	{
		text.erase(std::remove(text.begin(), text.end(), '\0'), text.end());

		std::wstring cropString = std::wstring(width, ' ') + text + std::wstring(width - 1, ' ');

		std::vector<Frame> frames;
		for (unsigned long i = 0; i < text.length() + width; i++)
			frames.push_back(Frame(cropString.substr(i, width + i), ANIMATION_SPEED));

		return Animation(name, frames, ANIM_LOOP);
	}
	Animation Words(const std::string& name, std::wstring text)
	{
		// Outputs a word by word animation
		std::vector<std::wstring> words = split_words(text);
		std::vector<Frame> frames;
		for (unsigned long i = 0; i < words.size(); i++)
			frames.push_back(Frame(words[i], ANIMATION_SPEED));

		return Animation(name, frames, ANIM_LOOP);
	}
	Animation Letters(const std::string name, std::wstring text)
	{
		// Outputs a letter incrementing animation
		std::vector<Frame> frames;
		for (unsigned long i = 1; i <= text.length(); i++)
			frames.push_back(Frame(text.substr(0, i), ANIMATION_SPEED));

		for (unsigned long i = text.length() - 2; i > 0; i--)
			frames.push_back(Frame(frames[i].text, ANIMATION_SPEED));

		return Animation(name, frames, ANIM_LOOP);
	}
}
void F_Misc::ClanTag()
{
	ClanTags::Animation ct_animation = ClanTags::Letters("VITRUVIA", L"$vitruvia.me");
	ClanTags::Animation ct_animation2 = ClanTags::Words("VITRUVIA", L"vitruvia bling 777 $");
	ClanTags::Animation ct_animation3 = ClanTags::Marquee("VITRUVIA", L"$vitruvia.me");

	if (Config::Misc.ClanTag > 0 && Source::Engine->IsInGame())
	{
		switch (Config::Misc.ClanTag)
		{
		case 1: // vitruvia.me $
		{
			SetClanTag("vitruvia.me $", "VITRUVIA");
		}
		break;
		case 2: // vitruvia777 $
		{
			SetClanTag("vitruvia777 $", "VITRUVIA");
		}
		break;
		case 3: // Time
		{
			SetClanTag(GetLocalTimeString().c_str(), "VITRUVIA");
		}
		break;
		case 4: // ValvE
		{
			SetClanTag("[VALV\xE1\xB4\xB1]", "VITRUVIA");
		}
		break;
		case 5: // Animated Vitruvia
		{
			long current_time = GetEpochTime();
			static long time_stamp = current_time;

			if (current_time - time_stamp > ct_animation.GetCurrentFrame().time)
			{
				time_stamp = current_time;
				ct_animation.NextFrame();
			}

			std::string ct_escaped = "$vitruvia.me";
			StdReplaceStr(ct_escaped, "\\n", "\n");

			SetClanTag(WS2S(ct_animation.GetCurrentFrame().text).c_str(), "VITRUVIA");
		}
		break;
		case 6: // Animated Vitruvia 2
		{
			long current_time = GetEpochTime();
			static long time_stamp = current_time;

			if (current_time - time_stamp > ct_animation2.GetCurrentFrame().time)
			{
				time_stamp = current_time;
				ct_animation2.NextFrame();
			}

			std::string ct_escaped = "vitruvia bling 777 $";
			StdReplaceStr(ct_escaped, "\\n", "\n");

			SetClanTag(WS2S(ct_animation2.GetCurrentFrame().text).c_str(), "VITRUVIA");
		}
		break;
		case 7: // Animated Vitruvia 3
		{
			long current_time = GetEpochTime();
			static long time_stamp = current_time;

			if (current_time - time_stamp > ct_animation3.GetCurrentFrame().time)
			{
				time_stamp = current_time;
				ct_animation3.NextFrame();
			}

			std::string ct_escaped = "$vitruvia.me";
			StdReplaceStr(ct_escaped, "\\n", "\n");

			SetClanTag(WS2S(ct_animation3.GetCurrentFrame().text).c_str(), "VITRUVIA");
		}
		break;
		}
	}
}

void F_Misc::AirStuck(Source::CUserCmd* cmd)
{
	// If airstuck key is down, set cmd->tick_count to INT_MAX to make the command invalid
	if (Source::InputSystem->IsButtonDown(ButtonCode_t(Config::Misc.AirStuckKey.m_Value))
		&& !cmd->buttons & IN_ATTACK	// Player is not shooting
		&& !cmd->buttons & IN_ATTACK2)
	{
		cmd->tick_count = INT_MAX;
	}
}

void TraceKnife(Source::CUserCmd* cmd, Source::C_CSPlayer* local, bool stab, Source::trace_t* tr) 
{
	// Attack range of knife attack
	float range = stab ? 32.0f : 48.0f;

	// Calculte end spot
	Vector myPos = local->GetEyePosition();
	Vector vecAim;
	Maths::AngleVectors(cmd->viewangles, &vecAim);
	Vector end = myPos + vecAim * range;

	// Trace attack
	Ray_t ray;
	ray.Init(myPos, end);
	CTraceFilter filter;
	filter.pSkip = local;
	Source::EngineTrace->TraceRay(ray, 0x46004003, &filter, tr);

	// If that failed, try a hull trace
	if (tr->fraction >= 1.0f) 
	{
		static const Vector mins = Vector(-16.0f, -16.0f, -18.0f);
		static const Vector maxs = Vector(16.0f, 16.0f, 18.0f);

		ray.Init(myPos, end, mins, maxs);

		Source::EngineTrace->TraceRay(ray, 0x200400B, &filter, tr);
	}
}

bool IsBehind(Source::C_CSPlayer* local, Source::C_CSPlayer* pTarget) 
{
	Vector vecLOS = pTarget->GetAbsOrigin() - local->GetAbsOrigin();
	vecLOS.z = 0.0f;
	vecLOS.Normalize();

	Vector vTargetForward;
	QAngle angAbsRotation = pTarget->GetAbsAngles();
	Maths::AngleVectors(pTarget->GetAbsAngles(), &vTargetForward);
	vTargetForward.z = 0.0f; // BUG? vTargetForward.AsVector2D() isn't normalized.
							 // CSS:0.8, CSGO:0.475
	return (vecLOS.Dot(vTargetForward) > 0.475f);
}

void F_Misc::KnifeBot(Source::C_CSPlayer* local, Source::CUserCmd* cmd)
{
	if (Config::Misc.KnifeBot)
	{
		// Execute trace knife
		trace_t tr;
		TraceKnife(cmd, local, false, &tr);

		if (!tr.m_pEnt ||
			!tr.m_pEnt->IsPlayer() ||
			tr.m_pEnt->m_iTeamNum() == local->m_iTeamNum())
			return; // Do not attack!

		C_CSPlayer* pl = rcast<C_CSPlayer*>(tr.m_pEnt);
		C_WeaponCSBase* pWeapon = local->GetActiveWeapon();
		if (pWeapon)
		{
			WeaponInfo_t* pWeaponInfo = pWeapon->GetWeaponInfo();
			if (pWeaponInfo && pWeaponInfo->WeaponType == WeaponTypes::WEAPONTYPE_KNIFE)
			{
				//------------------------------------------------
				// Swing and stab damage

				struct table_t {
					// [first][armor][back]
					unsigned char swing[2][2][2];
					// [armor][back]
					unsigned char stab[2][2];
				};
				static const table_t table = {
					{ { { 25,90 },{ 21,76 } },{ { 40,90 },{ 34,76 } } },
					{ { 65,180 },{ 55,153 } }
				};

				bool armor = (pl->m_ArmorValue() > 0);
				bool first = pWeapon->m_flNextPrimaryAttack() + 0.4f < Source::Globals->cur_time;
				bool back = IsBehind(local, pl);

				int stab_dmg = table.stab[armor][back];
				int slash_dmg = table.swing[first][armor][back]; // First slash, then swings
				int swing_dmg = table.swing[false][armor][back];

				int health = pl->m_iHealth();
				bool stab;

				if (health <= slash_dmg) stab = false;
				else if (health <= stab_dmg) stab = true;
				else if (health > (slash_dmg + swing_dmg + stab_dmg)) stab = true;
				else stab = false;

				cmd->buttons |= stab ? IN_ATTACK2 : IN_ATTACK;
			}
		}
	}
}

#pragma endregion

F_NotificationSystem* NotificationSystem;
#pragma region NOTIFICATIONS
#define ESTRING(str) std::string(game_event->GetString(str))

// Notifications behaviours
/*
{enc("event"), [](Notification* ctx, Source::C_CSPlayer* local, C_CSPlayer* user, IGameEvent* game_event) -> void {
	Color clr = local->m_iTeamNum() != user->m_iTeamNum() ? Color::Orange() : Color::Lime();
	Notification n = Notification(clr, ctx->GetEventName(), ctx->GetDuration());
	std::stringstream ss;
	n.SetText(ss.str());
	NotificationSystem->PushNewNotification(n);
}}
 */
const static std::unordered_map<std::string, notify_behaviour> notify_behaviours 
{
	{enc("item_purchase"), [](Notification* ctx, Source::C_CSPlayer* local, C_CSPlayer* user, IGameEvent* game_event) -> void {
		if (Config::Notifications.ItemPurchase) {
			Color clr = local->m_iTeamNum() != user->m_iTeamNum() ? Color::Orange() : Color::Lime();
			Notification n = Notification(clr, ctx->GetEventName(), ctx->GetDuration());
			std::stringstream ss;
			ss << user->GetPlayerName() << enc(" bought ") << ESTRING(enc("weapon"));
			n.SetText(ss.str());
			NotificationSystem->PushNewNotification(n);
		}
	}},
	{enc("player_death"), [](Notification* ctx, Source::C_CSPlayer* local, C_CSPlayer* user, IGameEvent* game_event) -> void {
		if (Config::Notifications.PlayerDeath) {
			auto attacker = USERID2PLAYER(game_event->GetInt(enc("attacker")));
			if (attacker)
			{
				Color clr = local->m_iTeamNum() != user->m_iTeamNum() ? Color::Lime() : Color::Orange();
				Notification n = Notification(clr, ctx->GetEventName(), ctx->GetDuration());
				std::stringstream ss;
				ss << attacker->GetPlayerName() << enc(" killed ") << user->GetPlayerName();
				auto assister = USERID2PLAYER(game_event->GetInt(enc("assister")));
				if (assister->GetPlayerName() != enc("???"))
					ss << enc(" (assisted by ") << assister->GetPlayerName() << enc(")");
				ss << enc(" using a ") << ESTRING(enc("weapon"));
				if (game_event->GetBool(enc("headshot")))
					ss << enc(" *HEADSHOT*");
				n.SetText(ss.str());
				if (attacker->GetPlayerName() == user->GetPlayerName() 
					|| attacker->GetPlayerName() == enc("???") 
					|| ESTRING(enc("weapon")) == enc("worldspawn"))
					n.SetText(user->GetPlayerName() + enc(" killed himself"));
				NotificationSystem->PushNewNotification(n);
			}
		}
	}},
	{enc("bomb_beginplant"), [](Notification* ctx, Source::C_CSPlayer* local, C_CSPlayer* user, IGameEvent* game_event) -> void {
		if (Config::Notifications.BombBeginPlant) {
			Color clr = local->m_iTeamNum() != user->m_iTeamNum() ? Color::Orange() : Color::Lime();
			Notification n = Notification(clr, ctx->GetEventName(), ctx->GetDuration());
			std::stringstream ss;
			std::string site = game_event->GetInt(enc("site")) == 1 ? enc("A") : enc("B");
			ss << user->GetPlayerName() << enc(" started planting the bomb on site ") << site;
			n.SetText(ss.str());
			NotificationSystem->PushNewNotification(n);
		}
	}},
	{enc("bomb_abortplant"), [](Notification* ctx, Source::C_CSPlayer* local, C_CSPlayer* user, IGameEvent* game_event) -> void {
		if (Config::Notifications.BombAbortPlant) {
			Color clr = local->m_iTeamNum() != user->m_iTeamNum() ? Color::Orange() : Color::Lime();
			Notification n = Notification(clr, ctx->GetEventName(), ctx->GetDuration());
			std::stringstream ss;
			std::string site = game_event->GetInt(enc("site")) == 0 ? enc("A") : enc("B");
			ss << user->GetPlayerName() << enc(" stopped planting the bomb on site ") << site;
			n.SetText(ss.str());
			NotificationSystem->PushNewNotification(n);
		}
	}},
	{enc("bomb_planted"), [](Notification* ctx, Source::C_CSPlayer* local, C_CSPlayer* user, IGameEvent* game_event) -> void {
		if (Config::Notifications.BombPlanted) {
			Color clr = local->m_iTeamNum() != user->m_iTeamNum() ? Color::Orange() : Color::Lime();
			Notification n = Notification(clr, ctx->GetEventName(), ctx->GetDuration());
			std::stringstream ss;
			std::string site = game_event->GetInt(enc("site")) == 0 ? enc("A") : enc("B");
			ss << user->GetPlayerName() << enc(" planted the bomb on site ") << site;
			n.SetText(ss.str());
			NotificationSystem->PushNewNotification(n);
		}
	}},
	{enc("bomb_defused"), [](Notification* ctx, Source::C_CSPlayer* local, C_CSPlayer* user, IGameEvent* game_event) -> void {
		if (Config::Notifications.BombDefused) {
			Color clr = local->m_iTeamNum() != user->m_iTeamNum() ? Color::Orange() : Color::Lime();
			Notification n = Notification(clr, ctx->GetEventName(), ctx->GetDuration());
			std::stringstream ss;
			std::string site = game_event->GetInt(enc("site")) == 0 ? enc("A") : enc("B");
			ss << user->GetPlayerName() << enc(" defused the bomb on site ") << site;
			n.SetText(ss.str());
			NotificationSystem->PushNewNotification(n);
		}
	}},
	{enc("bomb_exploded"), [](Notification* ctx, Source::C_CSPlayer* local, C_CSPlayer* user, IGameEvent* game_event) -> void {
		if (Config::Notifications.BombExploded) {
			Color clr = local->m_iTeamNum() != user->m_iTeamNum() ? Color::Orange() : Color::Lime();
			Notification n = Notification(clr, ctx->GetEventName(), ctx->GetDuration());
			std::stringstream ss;
			std::string site = game_event->GetInt(enc("site")) == 0 ? enc("A") : enc("B");
			ss << enc("The bomb exploded on site ") << site;
			n.SetText(ss.str());
			NotificationSystem->PushNewNotification(n);
		}
	}},
	{enc("bomb_dropped"), [](Notification* ctx, Source::C_CSPlayer* local, C_CSPlayer* user, IGameEvent* game_event) -> void {
		if (Config::Notifications.BombDropped) {
			Color clr = local->m_iTeamNum() != user->m_iTeamNum() ? Color::Orange() : Color::Lime();
			Notification n = Notification(clr, ctx->GetEventName(), ctx->GetDuration());
			std::stringstream ss;
			ss << user->GetPlayerName() << enc(" just dropped the bomb");
			n.SetText(ss.str());
			NotificationSystem->PushNewNotification(n);
		}
	}},
	{enc("bomb_pickup"), [](Notification* ctx, Source::C_CSPlayer* local, C_CSPlayer* user, IGameEvent* game_event) -> void {
		if (Config::Notifications.BombPickup) {
			Color clr = local->m_iTeamNum() != user->m_iTeamNum() ? Color::Orange() : Color::Lime();
			Notification n = Notification(clr, ctx->GetEventName(), ctx->GetDuration());
			std::stringstream ss;
			ss << user->GetPlayerName() << enc(" just picked up the bomb");
			n.SetText(ss.str());
			NotificationSystem->PushNewNotification(n);
		}
	}},
	{enc("bomb_begindefuse"), [](Notification* ctx, Source::C_CSPlayer* local, C_CSPlayer* user, IGameEvent* game_event) -> void {
		if (Config::Notifications.BombBeginDefuse) {
			Color clr = local->m_iTeamNum() != user->m_iTeamNum() ? Color::Orange() : Color::Lime();
			Notification n = Notification(clr, ctx->GetEventName(), ctx->GetDuration());
			std::stringstream ss;
			ss << user->GetPlayerName() << enc(" started defusing the bomb");
			if (game_event->GetBool(enc("haskit")))
				ss << enc(" *HAS KIT*");
			n.SetText(ss.str());
			NotificationSystem->PushNewNotification(n);
		}
	}},
	{enc("bomb_abortdefuse"), [](Notification* ctx, Source::C_CSPlayer* local, C_CSPlayer* user, IGameEvent* game_event) -> void {
		if (Config::Notifications.BombAbortDefuse) {
			Color clr = local->m_iTeamNum() != user->m_iTeamNum() ? Color::Orange() : Color::Lime();
			Notification n = Notification(clr, ctx->GetEventName(), ctx->GetDuration());
			std::stringstream ss;
			ss << user->GetPlayerName() << enc(" aborted bomb defusal");
			n.SetText(ss.str());
			NotificationSystem->PushNewNotification(n);
		}
	}},
	{enc("hostage_hurt"), [](Notification* ctx, Source::C_CSPlayer* local, C_CSPlayer* user, IGameEvent* game_event) -> void {
		if (Config::Notifications.HostageHurt) {
			Color clr = local->m_iTeamNum() != user->m_iTeamNum() ? Color::Orange() : Color::Lime();
			Notification n = Notification(clr, ctx->GetEventName(), ctx->GetDuration());
			std::stringstream ss;
			ss << user->GetPlayerName() << enc(" did damage to an hostage");
			n.SetText(ss.str());
			NotificationSystem->PushNewNotification(n);
		}
	}},
	{enc("hostage_killed"), [](Notification* ctx, Source::C_CSPlayer* local, C_CSPlayer* user, IGameEvent* game_event) -> void {
		if (Config::Notifications.HostageKilled) {
			Color clr = local->m_iTeamNum() != user->m_iTeamNum() ? Color::Orange() : Color::Lime();
			Notification n = Notification(clr, ctx->GetEventName(), ctx->GetDuration());
			std::stringstream ss;
			ss << user->GetPlayerName() << enc(" killed an hostage");
			n.SetText(ss.str());
			NotificationSystem->PushNewNotification(n);
		}
	}},
	{enc("hostage_rescued"), [](Notification* ctx, Source::C_CSPlayer* local, C_CSPlayer* user, IGameEvent* game_event) -> void {
		if (Config::Notifications.HostageRescued) {
			Color clr = local->m_iTeamNum() != user->m_iTeamNum() ? Color::Orange() : Color::Lime();
			Notification n = Notification(clr, ctx->GetEventName(), ctx->GetDuration());
			std::stringstream ss;
			ss << user->GetPlayerName() << enc(" rescued an hostage");
			n.SetText(ss.str());
			NotificationSystem->PushNewNotification(n);
		}
	}},
	{enc("weapon_reload"), [](Notification* ctx, Source::C_CSPlayer* local, C_CSPlayer* user, IGameEvent* game_event) -> void {
		if (Config::Notifications.WeaponReload) {
			Color clr = local->m_iTeamNum() != user->m_iTeamNum() ? Color::Orange() : Color::Lime();
			Notification n = Notification(clr, ctx->GetEventName(), ctx->GetDuration());
			std::stringstream ss;
			ss << user->GetPlayerName() << enc(" is reloading");
			n.SetText(ss.str());
			NotificationSystem->PushNewNotification(n);
		}
	}},
	{enc("item_pickup"), [](Notification* ctx, Source::C_CSPlayer* local, C_CSPlayer* user, IGameEvent* game_event) -> void {
		if (Config::Notifications.ItemPickup) {
			if (game_event->GetBool(enc("silent")))
				return;
			Color clr = local->m_iTeamNum() != user->m_iTeamNum() ? Color::Orange() : Color::Lime();
			Notification n = Notification(clr, ctx->GetEventName(), ctx->GetDuration());
			std::stringstream ss;
			ss << user->GetPlayerName() << enc(" picked up a ") << ESTRING(enc("item"));
			n.SetText(ss.str());
			NotificationSystem->PushNewNotification(n);
		}
	}}
};

void F_NotificationSystem::CreateNotification(Notification notify)
{
	if (g_pEventListener->ListenFor(notify.GetEventName()))
	{
		notify.SetBehaviour(notify_behaviours.at(notify.GetEventName()));
		notifications.emplace_back(notify);
	} 
	else Err(enc("FAILED TO LISTEN FOR NOTIFICATION %s\n"), notify.GetEventName().c_str());
}

void F_NotificationSystem::AddNotifications()
{
	Log(enc("Initializing notification events listeners"));

	CreateNotification(Notification(enc("item_purchase")));
	CreateNotification(Notification(enc("player_death")));
	CreateNotification(Notification(enc("bomb_beginplant")));
	CreateNotification(Notification(enc("bomb_abortplant")));
	CreateNotification(Notification(enc("bomb_planted")));
	CreateNotification(Notification(enc("bomb_defused")));
	CreateNotification(Notification(enc("bomb_exploded")));
	CreateNotification(Notification(enc("bomb_dropped")));
	CreateNotification(Notification(enc("bomb_pickup")));
	CreateNotification(Notification(enc("bomb_begindefuse")));
	CreateNotification(Notification(enc("bomb_abortdefuse")));
	CreateNotification(Notification(enc("hostage_hurt")));
	CreateNotification(Notification(enc("hostage_killed")));
	CreateNotification(Notification(enc("hostage_rescued")));
	CreateNotification(Notification(enc("weapon_reload")));
	CreateNotification(Notification(enc("item_pickup")));

	Log(enc("Done"));
}

void F_NotificationSystem::PushNewNotification(Notification notify)
{
	current_notifications.emplace_back(std::move(notify));
}

void F_NotificationSystem::DrawNotifications()
{
	GRAB_SCREEN_SIZE;
	int32_t y = screen_height;
	auto now = std::chrono::high_resolution_clock::now();

	int i = 0;
	for (auto iter = current_notifications.begin(); iter != current_notifications.end();)
	{
		if (iter->GetEnd() > now)
		{
			Render::String(Render::Fonts::Tahoma_Outline12, screen_width / 2, screen_height - 6 - 12 * i, iter->GetColor(), 
				Render::eAlign::ALIGN_CENTER , iter->GetText().c_str());
			++iter; ++i;
		}
		else
		{
			iter = current_notifications.erase(iter);
			--i;
		}
	}
}

void F_NotificationSystem::OnListenedEvent(Source::IGameEvent* game_event)
{
	for (auto& n : notifications)
		n.Behave(game_event);
}

void F_NotificationSystem::OnPaintTraverse()
{
	this->DrawNotifications();
}

#pragma endregion

}