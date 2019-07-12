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

void __fastcall hk_PlaySound(VGUI::ISurface* _this, void* edx, const char* file_name)
{
	// Call the original, otherwise the sound won't be played
	SurfaceTable->GetFunction<PlaySound_t>(I_PlaySound)(_this, file_name);

	for (auto f : Vitruvia::Features)
		f->OnPlaySound(file_name);
}

}
SOURCE_END