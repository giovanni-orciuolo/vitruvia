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
void __fastcall hk_TraceRay(IEngineTrace* _this, void* edx, const Ray_t& ray, unsigned int mask, ITraceFilter* filter, CGameTrace* trace)
{
	if (global->UsingBulletHandler())
	{
		// Fix trace only while executing autowall shit
		EngineTraceTable->GetFunction<TraceRay_t>(I_TraceRay)(_this, ray, mask, filter, trace);
		trace->surface.flags |= SURF_SKY; // Set surface to be sky so there won't be any bullet impacts (annoying)
	}
	else
	{
		// Otherwise, just return the original
		EngineTraceTable->GetFunction<TraceRay_t>(I_TraceRay)(_this, ray, mask, filter, trace);
	}
}
}
SOURCE_END