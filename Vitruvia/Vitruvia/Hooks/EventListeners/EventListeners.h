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
#include "../../InterfaceList/InterfaceList.h"
#include "../../Features/IFeature.h"

namespace Vitruvia {

class V_EventListener : public Source::IGameEventListener2
{
public:
	V_EventListener(const std::initializer_list<std::string>& listened_events);
	~V_EventListener() { Source::EventManager->RemoveListener(this); }

	bool ListenFor(const std::string& event_name)
	{
		return Source::EventManager->AddListener(this, event_name.c_str(), false);
	}

	void FireGameEvent(Source::IGameEvent* game_event);
	int GetEventDebugID() override { return EVENT_DEBUG_ID_INIT; }
};
extern V_EventListener* g_pEventListener;

}
