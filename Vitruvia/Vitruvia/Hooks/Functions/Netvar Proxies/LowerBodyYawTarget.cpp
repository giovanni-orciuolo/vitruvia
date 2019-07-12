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

#include "../../Hooks.h"
#include "../../../SourceClasses/C_CSPlayer.h"
#include "../../../Config/Config.h"

SOURCE_INIT
namespace Hooks
{

void hk_flLowerBodyYawTarget(const CRecvProxyData* pData, void* pStruct, void* pOut)
{
	// Get the original function once
	static auto original_fn = m_flLowerBodyYawTarget->GetOriginalFunction();

	C_CSPlayer* player = rcast<C_CSPlayer*>(pStruct);
	if (player)
	{
		/*
		Warn(enc("LBY Update from %s at time %f! ProxyData: %f"), 
			player->GetPlayerName().c_str(), 
			Source::Globals->cur_time, 
			pData->m_Value.m_Float);
		*/
		for (auto f : Vitruvia::Features)
			f->OnLBYProxy(player, pData->m_Value.m_Float);
	}

	// Call the original
	original_fn(pData, pStruct, pOut);
}

}
SOURCE_END