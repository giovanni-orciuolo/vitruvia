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

#include "../Hooks.h"

SOURCE_INIT
namespace Hooks {

bool __fastcall hk_FireEvent(IGameEventManager2* _this, void* edx, IGameEvent* game_event, bool no_broadcast)
{
	// Validity check for game event pointer
	if (game_event == nullptr)
		return false;

	for (auto f : Vitruvia::Features)
		f->OnFireEvent(game_event);

	// Call the original after our stuff is done
	return GameEventManagerTable->GetFunction<FireEvent_t>(I_FireEvent)(_this, game_event, no_broadcast);
}

}
SOURCE_END