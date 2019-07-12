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

#include "EventListeners.h"
#include "../../SourceClasses/C_CSPlayer.h"
#include "../../SourceClasses/C_BaseBeam.h"
using namespace Source;

namespace Vitruvia {

V_EventListener* g_pEventListener;

V_EventListener::V_EventListener(const std::initializer_list<std::string>& listened_events)
{
	Log(enc("Initializing event listeners for events:"));
	// For each event that I need to listen for, setup a listener
	for (auto e : listened_events)
	{
		// Log shit if the console is enabled
#ifdef ENABLE_CONSOLE
		w_printf(enc("> %s\n"), e.c_str());
#endif
		// Register a new listener associated to this pointer
		if (!this->ListenFor(e))
			Err(enc("FAILED TO LISTEN FOR %S!\n"), e.c_str());
	}
}

void V_EventListener::FireGameEvent(Source::IGameEvent* game_event)
{
	if (game_event == nullptr)
		return;

	//Log(enc("Catched listened event: %s [Time: %f]"), game_event->GetName(), Source::Globals->cur_time);

	// Perform behaviour on listened event
	for (auto f : Features)
		f->OnListenedEvent(game_event);
}

}
