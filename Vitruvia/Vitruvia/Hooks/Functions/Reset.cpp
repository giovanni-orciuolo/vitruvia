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

long __stdcall hk_Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_params)
{
	using namespace Vitruvia;

	// Correctly handling Reset calls is very important if you have a DirectX hook.
	// IDirect3DDevice9::Reset is called when you minimize the game, Alt-Tab or change resolutions.
	// When it is called, the IDirect3DDevice9 is placed on "lost" state and many related resources are released
	// This means that we need to recreate our own resources when that happens. If we dont, we crash.

	if (!g_bMenuInitialized) 
		return D3DTable->GetFunction<D3D_Reset_t>(I_D3D_Reset)(device, presentation_params);

	// Device is currently on LOST state
	
	// Invalidate GUI resources
	ImGui_ImplDX9_InvalidateDeviceObjects();
	Render::D3D::g_pRenderer->InvalidateObjects();

	// Call the original Reset and store the result of the call
	auto result = D3DTable->GetFunction<D3D_Reset_t>(I_D3D_Reset)(device, presentation_params);

	// Re-create the device objects
	Render::D3D::g_pRenderer->CreateObjects();
	ImGui_ImplDX9_CreateDeviceObjects();
	return result;
}

}
SOURCE_END