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
namespace Hooks
{

bool __fastcall hk_DispatchUserMessage(IBaseClientDLL* _this, void* edx, int type, unsigned int unknown, unsigned int length, const void* msg_data)
{
	for (auto f : Vitruvia::Features)
		f->OnUserMessage(type, length, msg_data);

	return BaseClientTable->GetFunction<DispatchUserMessage_t>(I_DispatchUserMessage)(_this, type, unknown, length, msg_data);
}

}
SOURCE_END