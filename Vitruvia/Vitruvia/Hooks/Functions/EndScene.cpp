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

static ConVar* cl_mouseenable;

long __stdcall hk_EndScene(IDirect3DDevice9* device)
{
	using namespace Vitruvia::Render::D3D;
	static auto addr = _ReturnAddress();

	if (!g_bMenuInitialized)
	{
		Vitruvia::GUI::InitMenu(device);
		cl_mouseenable = Source::CVar->FindVar(enc("cl_mouseenable"));
		g_bMenuInitialized = true;
	}

	if (g_bMenuInitialized)
	{
		// Start my own renderer
		g_pRenderer->BeginRendering();

		for (auto f : Vitruvia::Features)
			f->OnEndScene(g_pRenderer);

		// End my own renderer
		g_pRenderer->EndRendering();

		//if (addr == _ReturnAddress())
		//{
			// Render the menu
			Vitruvia::GUI::InitMenu_Style();
			Vitruvia::GUI::RenderMenu(device);
			ImGui::Render();
		//}
	}

	// Call the original
	if (D3DTable != nullptr)
		return D3DTable->GetFunction<D3D_EndScene_t>(I_D3D_EndScene)(device);

	return S_FALSE;
}

}
SOURCE_END