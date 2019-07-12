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
#include "../IFeature.h"
#include "../../SourceClasses/C_CSPlayer.h"
#include "../../Config/Config.h"
#include "../../Hooks/EventListeners/EventListeners.h"
#include "../../Renderer/Renderer.h"

namespace Vitruvia {

class F_NotificationSystem;
class Notification;
const std::chrono::milliseconds DEFAULT_DURATION = std::chrono::milliseconds(5000);
using notify_behaviour = std::function<void(Notification*, Source::C_CSPlayer*, Source::C_CSPlayer*, Source::IGameEvent*)>;

class Notification
{
public:
	Notification()
		: color(Source::Color::White()),
		  event_name(""),
		  duration(std::chrono::milliseconds(0)),
		  behaviour([](Notification*, Source::C_CSPlayer*, Source::C_CSPlayer*, Source::IGameEvent*){}),
		  text("")
	{
		set_time(std::chrono::milliseconds(0));
	}
	Notification(Source::Color color, const std::string& event_name, const std::chrono::milliseconds& time = DEFAULT_DURATION)
		: color(color),
		event_name(event_name),
		duration(time),
		behaviour([](Notification*, Source::C_CSPlayer*, Source::C_CSPlayer*, Source::IGameEvent*) {}),
		text("")
	{
		set_time(time);
	}
	explicit Notification(const std::string& event_name, const std::chrono::milliseconds& time = DEFAULT_DURATION)
		: color(Source::Color::White()),
		  event_name(event_name),
		  duration(time),
		  behaviour([](Notification*, Source::C_CSPlayer*, Source::C_CSPlayer*, Source::IGameEvent*) {}),
		  text("")
	{
		set_time(time);
	}

	void Behave(Source::IGameEvent* game_event)
	{
		auto local = Source::C_CSPlayer::GetLocal();
		if (local)
		{
			if (EVENT_IS(event_name.c_str()))
			{
				auto user = USERID2PLAYER(game_event->GetInt(enc("userid")));
				if (user)
				{
					if (Config::Notifications.IgnoreLocal && user == local) return;
					this->behaviour(this, local, user, game_event);
				}
			}
		}
	}

	Source::Color GetColor() { return this->color; }
	std::string GetEventName() { return this->event_name; }
	std::chrono::milliseconds GetDuration() { return this->duration; }
	std::chrono::high_resolution_clock::time_point GetEnd() { return this->end; }
	std::string GetText() { return this->text; }
	void SetColor(Source::Color col) { this->color = col; }
	void SetDuration(std::chrono::milliseconds duration) { this->duration = duration; set_time(duration); }
	void SetText(const std::string& text) { this->text = text; }
	void SetBehaviour(const notify_behaviour& behaviour) { this->behaviour = behaviour; }

private:
	Source::Color color;
	std::string event_name;
	std::chrono::milliseconds duration;
	std::chrono::high_resolution_clock::time_point end;
	notify_behaviour behaviour;
	std::string text;
	void set_time(const std::chrono::milliseconds time) {
		end = std::chrono::high_resolution_clock::now() + time;
	}
};

class F_NotificationSystem : public V_IFeature
{
public:
	void CreateNotification(Notification notify);
	void AddNotifications();
	void PushNewNotification(Notification notify);
	void DrawNotifications();

	void OnListenedEvent(Source::IGameEvent* game_event) override;
	void OnPaintTraverse() override;
private:
	std::vector<Notification> notifications;
	std::vector<Notification> current_notifications;
};

class F_Misc : public V_IFeature
{
public:
	void BunnyHop(Source::CUserCmd* cmd);
	void EdgeJump(Source::CUserCmd* cmd);
	void AutoStrafe(Source::CUserCmd* cmd);
	void AutoAccept();
	void ChatSpam();
	void NameSpam();
	void NameStealer();
	void NoName();
	void SpectatorList();
	void ClanTag();
	void AirStuck(Source::CUserCmd* cmd);
	void KnifeBot(Source::C_CSPlayer* local, Source::CUserCmd* cmd);

	void OnCreateMove(Source::CUserCmd* cmd) override;
	void OnCreateMovePrePrediction(Source::CUserCmd* cmd) override;
	void OnCreateMovePostPrediction(Source::CUserCmd* cmd) override;
	void OnPlaySound(const char* file_name) override;
	void OnListenedEvent(Source::IGameEvent* game_event) override;
	void OnShutdown() override;
};

extern F_Misc* Miscs;
extern F_NotificationSystem* NotificationSystem;

}