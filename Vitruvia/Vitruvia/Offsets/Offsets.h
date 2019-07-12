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
#include "../Vitruvia.h"

#define OFFSET_EX(name) extern offset_t name;
#define OFFSET(name) offset_t name;

#define OFFSET_PTR_EX(name) extern offset_t* name;
#define OFFSET_PTR(name) offset_t* name;

namespace Vitruvia {
namespace Offsets {

OFFSET_EX(PlayerResource)
OFFSET_EX(GameRules)
OFFSET_EX(LineGoesThroughSmoke)
OFFSET_EX(D3D_Device)
OFFSET_EX(GetItemName)
OFFSET_EX(RankRevealAll)
OFFSET_PTR_EX(GlowObject)
OFFSET_EX(SmokeCount)
OFFSET_EX(IsReady)
OFFSET_EX(SetClanTag)
OFFSET_EX(IsEntityBreakable)
OFFSET_EX(HandleBulletPenetration)
OFFSET_EX(ClipTraceToPlayers)
OFFSET_EX(CustomMaterials)
OFFSET_EX(CustomMaterialInitialized)
OFFSET_EX(GetSequenceActivity)
OFFSET_EX(CreateAnimationState)
OFFSET_EX(ResetAnimationState)
OFFSET_EX(UpdateAnimationState)
OFFSET_EX(CL_SendMove)
OFFSET_EX(WriteUserCmd)

extern void LOG_OFFSET(const std::string& module_name, const std::string& offset_name, const offset_t offset_value, bool skip_base = false);
extern void ScanOffsets();

}
}
