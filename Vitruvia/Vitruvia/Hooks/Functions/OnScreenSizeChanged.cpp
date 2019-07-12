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
namespace Hooks {

void __fastcall hk_OnScreenSizeChanged(VGUI::ISurface* _this, void* edx, int old_width, int old_height)
{
	// Call the original so the game does what it needs to do, then we can do our stuff
	SurfaceTable->GetFunction<OnScreenSizeChanged_t>(I_OnScreenSizeChanged)(_this, old_width, old_height);

	for (auto f : Vitruvia::Features)
		f->OnScreenSizeChanged(old_width, old_height);

	GRAB_SCREEN_SIZE;
	Log("Detected screen size change!\n> Old Width: %d\n> Old Height: %d", old_width, old_height);
	Log("Setting up fonts again...");
	Vitruvia::Render::Fonts::SetupFonts();
	Log("Setting new global width/height...");
	Vitruvia::global->set_ScreenWidth(screen_width);
	Vitruvia::global->set_ScreenHeight(screen_height);
}

}
SOURCE_END