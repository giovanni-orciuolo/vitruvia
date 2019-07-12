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
#include "../../Vitruvia.h"

#define FONT_EX_BIG(name) extern HFont name##25;
#define FONT_EX_MED(name) extern HFont name##20;
#define FONT_EX_SMALL(name) extern HFont name##15;
#define FONT_EX_MINI(name) extern HFont name##12;

#define FONT_BIG(name) HFont name##25;
#define FONT_MED(name) HFont name##20;
#define FONT_SMALL(name) HFont name##15;
#define FONT_MINI(name) HFont name##12;

#define FONT_CSGO 0xA1

namespace Vitruvia {
namespace Render {

namespace Fonts {

FONT_EX_BIG(Arial)
FONT_EX_BIG(Tahoma)
FONT_EX_BIG(Consolas)
FONT_EX_BIG(Courier)
FONT_EX_BIG(Gothic)
FONT_EX_BIG(Arial_Outline)
FONT_EX_BIG(Tahoma_Outline)
FONT_EX_BIG(Consolas_Outline)
FONT_EX_BIG(Courier_Outline)
FONT_EX_BIG(Gothic_Outline)

FONT_EX_MED(Arial)
FONT_EX_MED(Tahoma)
FONT_EX_MED(Consolas)
FONT_EX_MED(Courier)
FONT_EX_MED(Gothic)
FONT_EX_MED(Arial_Outline)
FONT_EX_MED(Tahoma_Outline)
FONT_EX_MED(Consolas_Outline)
FONT_EX_MED(Courier_Outline)
FONT_EX_MED(Gothic_Outline)

FONT_EX_SMALL(Arial)
FONT_EX_SMALL(Tahoma)
FONT_EX_SMALL(Consolas)
FONT_EX_SMALL(Courier)
FONT_EX_SMALL(Gothic)
FONT_EX_SMALL(Arial_Outline)
FONT_EX_SMALL(Tahoma_Outline)
FONT_EX_SMALL(Consolas_Outline)
FONT_EX_SMALL(Courier_Outline)
FONT_EX_SMALL(Gothic_Outline)

FONT_EX_MINI(Arial)
FONT_EX_MINI(Tahoma)
FONT_EX_MINI(Consolas)
FONT_EX_MINI(Courier)
FONT_EX_MINI(Gothic)
FONT_EX_MINI(Arial_Outline)
FONT_EX_MINI(Tahoma_Outline)
FONT_EX_MINI(Consolas_Outline)
FONT_EX_MINI(Courier_Outline)
FONT_EX_MINI(Gothic_Outline)

// Function to initialize all the fonts
extern void SetupFonts();
// Will store the main font used
extern HFont MAIN_MENU_FONT;

}

}
}
