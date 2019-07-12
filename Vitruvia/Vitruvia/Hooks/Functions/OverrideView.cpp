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
#include "../../Globals/VitruviaGlobals.h"
#include "../../Config/Config.h"

SOURCE_INIT
namespace Hooks {

void __fastcall hk_OverrideView(IClientMode* _this, void* edx, CViewSetup* setup)
{
	// Check if setup pointer is valid
	if (setup == nullptr)
		return;

	for (auto f : Vitruvia::Features)
		f->OnOverrideView(setup);

	// Store FOV
	Vitruvia::global->set_ClientFOV(setup->fov);

	// Call the original, after our stuff is done
	ClientModeTable->GetFunction<OverrideView_t>(I_OverrideView)(_this, setup);
}

float _fastcall hk_GetViewModelFOV(IClientMode* _this, void* edx)
{
	// Get the real viewmodel fov by calling the original function
	float realFov = ClientModeTable->GetFunction<GetViewModelFOV_t>(I_GetViewModelFOV)(_this);

	// Modify it
	realFov += Vitruvia::Config::VisualsOther.ViewModelFOV;

	// Return it to the real function call
	return realFov;
}

}
SOURCE_END