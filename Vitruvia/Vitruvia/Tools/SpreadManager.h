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

// Credits: polak + FuckFace32 for various calculations

#pragma once

#include "../Vitruvia.h"
#include "../SourceClasses/C_CSPlayer.h"

#define SpreadManager CSpreadManager::GetInstance()

namespace Vitruvia
{
	namespace Tools
	{
		class CSpreadManager
		{
			IMPLEMENT_SINGLETON(CSpreadManager)
		public:
			// Fill spread_x and spread_y with spread values
			void GetSpreads(Source::CUserCmd* cmd, Source::C_WeaponCSBase* weapon, float& spread_x, float& spread_y);
			// Apply spread values to view angles to adjust
			void ApplySpreadFactor(Source::CUserCmd* cmd, float spread_x, float spread_y);
		};
	}
}