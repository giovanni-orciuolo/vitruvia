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
#include "../../SourceClasses/C_EconItem.h"

namespace Vitruvia
{
	/*
	 * This class is a collection of useful functions within the game
	 */
	class CGameHelper
	{
	public:
		int GetPing();
		int GetFPS();
		bool IsPointVisible(const Source::Vector& point);
	};
	extern CGameHelper g_GameHelper;
}
