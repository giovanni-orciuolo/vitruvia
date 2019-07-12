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
#include "../../../Config/Config.h"

SOURCE_INIT
namespace Hooks
{

void hk_bDidSmokeEffect(const CRecvProxyData* pData, void* pStruct, void* pOut)
{
	// Get the original function once
	static auto original_fn = m_bDidSmokeEffect->GetOriginalFunction();
	
	if (Vitruvia::Config::Removals.NoSmoke)
	{
		// Set the value to true so the game thinks that the effect is ended
		*rcast<bool*>(rcast<DWORD>(pOut) + 1) = true;
	}

	// Call original function
	original_fn(pData, pStruct, pOut);
}

}
SOURCE_END