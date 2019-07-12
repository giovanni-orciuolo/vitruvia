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
#include "Fonts.h"
#include "../../InterfaceList/InterfaceList.h"

// Little helper function to setup a single font
bool SetupFont(HFont& font, 
	const std::string& windows_font_name, 
	int tall, 
	int weight = FW_EXTRABOLD, 
	int font_flags = Source::VGUI::FontFlags::FONTFLAG_NONE)
{
	return Source::Surface->SetFontGlyphSet(
		font = Source::Surface->SCreateFont(),
		windows_font_name.c_str(),
		tall,
		weight, 0, 0, font_flags
	);
}

#define SETUP_FONT_BIG(name, win_name, flags, weight) if (!SetupFont(name##25, win_name, 25, weight, flags)) Err("Failed to setup font %s!", win_name);
#define SETUP_FONT_MED(name, win_name, flags, weight) if (!SetupFont(name##20, win_name, 20, weight, flags)) Err("Failed to setup font %s!", win_name);
#define SETUP_FONT_SMALL(name, win_name, flags, weight) if (!SetupFont(name##15, win_name, 15, weight, flags)) Err("Failed to setup font %s!", win_name);
#define SETUP_FONT_MINI(name, win_name, flags, weight) if (!SetupFont(name##12, win_name, 12, weight, flags)) Err("Failed to setup font %s!", win_name);

namespace Vitruvia {
namespace Render {

namespace Fonts {

HFont Render::Fonts::MAIN_MENU_FONT;

FONT_BIG(Arial)
FONT_BIG(Tahoma)
FONT_BIG(Consolas)
FONT_BIG(Courier)
FONT_BIG(Gothic)
FONT_BIG(Arial_Outline)
FONT_BIG(Tahoma_Outline)
FONT_BIG(Consolas_Outline)
FONT_BIG(Courier_Outline)
FONT_BIG(Gothic_Outline)

FONT_MED(Arial)
FONT_MED(Tahoma)
FONT_MED(Consolas)
FONT_MED(Courier)
FONT_MED(Gothic)
FONT_MED(Arial_Outline)
FONT_MED(Tahoma_Outline)
FONT_MED(Consolas_Outline)
FONT_MED(Courier_Outline)
FONT_MED(Gothic_Outline)

FONT_SMALL(Arial)
FONT_SMALL(Tahoma)
FONT_SMALL(Consolas)
FONT_SMALL(Courier)
FONT_SMALL(Gothic)
FONT_SMALL(Arial_Outline)
FONT_SMALL(Tahoma_Outline)
FONT_SMALL(Consolas_Outline)
FONT_SMALL(Courier_Outline)
FONT_SMALL(Gothic_Outline)

FONT_MINI(Arial)
FONT_MINI(Tahoma)
FONT_MINI(Consolas)
FONT_MINI(Courier)
FONT_MINI(Gothic)
FONT_MINI(Arial_Outline)
FONT_MINI(Tahoma_Outline)
FONT_MINI(Consolas_Outline)
FONT_MINI(Courier_Outline)
FONT_MINI(Gothic_Outline)

void SetupFonts()
{
	using namespace Source::VGUI;

	SETUP_FONT_BIG(Arial, "Arial", FONTFLAG_NONE, FW_DONTCARE)
	SETUP_FONT_BIG(Tahoma, "Tahoma", FONTFLAG_NONE, FW_EXTRABOLD)
	SETUP_FONT_BIG(Consolas, "Consolas", FONTFLAG_NONE, FW_EXTRABOLD)
	SETUP_FONT_BIG(Courier, "Courier New", FONTFLAG_NONE, FW_EXTRABOLD)
	SETUP_FONT_BIG(Gothic, "Franklin Gothic Medium", FONTFLAG_NONE, FW_EXTRABOLD)
	SETUP_FONT_BIG(Arial_Outline, "Arial", FONTFLAG_OUTLINE, FW_EXTRABOLD)
	SETUP_FONT_BIG(Tahoma_Outline, "Tahoma", FONTFLAG_OUTLINE, FW_EXTRABOLD)
	SETUP_FONT_BIG(Consolas_Outline, "Consolas", FONTFLAG_OUTLINE, FW_EXTRABOLD)
	SETUP_FONT_BIG(Courier_Outline, "Courier New", FONTFLAG_OUTLINE, FW_EXTRABOLD)
	SETUP_FONT_BIG(Gothic_Outline, "Franklin Gothic Medium", FONTFLAG_OUTLINE, FW_EXTRABOLD)

	SETUP_FONT_MED(Arial, "Arial", FONTFLAG_NONE, FW_DONTCARE)
	SETUP_FONT_MED(Tahoma, "Tahoma", FONTFLAG_NONE, FW_EXTRABOLD)
	SETUP_FONT_MED(Consolas, "Consolas", FONTFLAG_NONE, FW_EXTRABOLD)
	SETUP_FONT_MED(Courier, "Courier New", FONTFLAG_NONE, FW_EXTRABOLD)
	SETUP_FONT_MED(Gothic, "Franklin Gothic Medium", FONTFLAG_NONE, FW_EXTRABOLD)
	SETUP_FONT_MED(Arial_Outline, "Arial", FONTFLAG_OUTLINE, FW_EXTRABOLD)
	SETUP_FONT_MED(Tahoma_Outline, "Tahoma", FONTFLAG_OUTLINE, FW_EXTRABOLD)
	SETUP_FONT_MED(Consolas_Outline, "Consolas", FONTFLAG_OUTLINE, FW_EXTRABOLD)
	SETUP_FONT_MED(Courier_Outline, "Courier New", FONTFLAG_OUTLINE, FW_EXTRABOLD)
	SETUP_FONT_MED(Gothic_Outline, "Franklin Gothic Medium", FONTFLAG_OUTLINE, FW_EXTRABOLD)

	SETUP_FONT_SMALL(Arial, "Arial", FONTFLAG_NONE, FW_DONTCARE)
	SETUP_FONT_SMALL(Tahoma, "Tahoma", FONTFLAG_NONE, FW_EXTRABOLD)
	SETUP_FONT_SMALL(Consolas, "Consolas", FONTFLAG_NONE, FW_EXTRABOLD)
	SETUP_FONT_SMALL(Courier, "Courier New", FONTFLAG_NONE, FW_EXTRABOLD)
	SETUP_FONT_SMALL(Gothic, "Franklin Gothic Medium", FONTFLAG_NONE, FW_EXTRABOLD)
	SETUP_FONT_SMALL(Arial_Outline, "Arial", FONTFLAG_OUTLINE, FW_EXTRABOLD)
	SETUP_FONT_SMALL(Tahoma_Outline, "Tahoma", FONTFLAG_OUTLINE, FW_EXTRABOLD)
	SETUP_FONT_SMALL(Consolas_Outline, "Consolas", FONTFLAG_OUTLINE, FW_EXTRABOLD)
	SETUP_FONT_SMALL(Courier_Outline, "Courier New", FONTFLAG_OUTLINE, FW_EXTRABOLD)
	SETUP_FONT_SMALL(Gothic_Outline, "Franklin Gothic Medium", FONTFLAG_OUTLINE, FW_EXTRABOLD)

	SETUP_FONT_MINI(Arial, "Arial", FONTFLAG_NONE, FW_DONTCARE)
	SETUP_FONT_MINI(Tahoma, "Tahoma", FONTFLAG_NONE, FW_EXTRABOLD)
	SETUP_FONT_MINI(Consolas, "Consolas", FONTFLAG_NONE, FW_EXTRABOLD)
	SETUP_FONT_MINI(Courier, "Courier New", FONTFLAG_NONE, FW_EXTRABOLD)
	SETUP_FONT_MINI(Gothic, "Franklin Gothic Medium", FONTFLAG_NONE, FW_EXTRABOLD)
	SETUP_FONT_MINI(Arial_Outline, "Arial", FONTFLAG_OUTLINE, FW_EXTRABOLD)
	SETUP_FONT_MINI(Tahoma_Outline, "Tahoma", FONTFLAG_OUTLINE, FW_EXTRABOLD)
	SETUP_FONT_MINI(Consolas_Outline, "Consolas", FONTFLAG_OUTLINE, FW_EXTRABOLD)
	SETUP_FONT_MINI(Courier_Outline, "Courier New", FONTFLAG_OUTLINE, FW_EXTRABOLD)
	SETUP_FONT_MINI(Gothic_Outline, "Franklin Gothic Medium", FONTFLAG_OUTLINE, FW_EXTRABOLD)

	// Modify here the main font used by the menu
	MAIN_MENU_FONT = Tahoma12;
}

}

}
}
