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
#include "../../Tools/Autowall.h"
using namespace Vitruvia;

SOURCE_INIT
namespace Hooks
{
bool __fastcall hk_IsBoxVisible(IVEngineClient* _this, void* edx, const Vector& mins, const Vector& maxs)
{
	// Check if I'm calling it from CViewRenderBeams::CullBeam
	if (!memcmp(_ReturnAddress(), enc("\x85\xC0\x74\x2D\x83\x7D\x10\x00\x75\x1C"), 10))
		return true; // Always return true so I will always draw the beam

	// Otherwise just proceed with normal call
	return EngineClientTable->GetFunction<IsBoxVisible_t>(I_IsBoxVisible)(_this, mins, maxs);
}
}
SOURCE_END