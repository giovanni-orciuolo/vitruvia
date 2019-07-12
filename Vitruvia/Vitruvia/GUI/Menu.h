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
#include "../Globals/VitruviaGlobals.h"
#include "../Config/Config.h"
#include "../Renderer/D3D/D3D_Renderer.h"
#include "../Skins/KitParser/KitParser.h"
#include "../Skins/ItemDefinitions/ItemDefinitions.h"

#include "ImGui/imgui.h"
#include "ImGui/DX9/imgui_impl_dx9.h"

namespace Vitruvia {
namespace GUI {

// Call this function to initialize the menu
extern void InitMenu(IDirect3DDevice9* device);

// Call this function to init menu style
extern void InitMenu_Style();

// Call this function inside EndScene to render the menu
extern void RenderMenu(IDirect3DDevice9* device);

}
}
