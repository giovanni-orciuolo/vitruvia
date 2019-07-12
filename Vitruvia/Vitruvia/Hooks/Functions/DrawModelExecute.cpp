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

void __fastcall hk_DrawModelExecute(IVModelRender* _this, void* edx, void* context, void* state, const ModelRenderInfo_t& render_info, matrix3x4_t* custom_bone_to_world)
{
	// Unhook the function before tampering with models
	ModelRenderTable->UnHook();

	// Go on only if the rendered model pointer is valid
	if (render_info.pModel != nullptr)
	{
		for (auto f : Vitruvia::Features)
			f->OnDrawModelExecute(context, state, render_info, custom_bone_to_world);
	}

	// Manually call DME from vtable
	Source::ModelRender->DrawModelExecute(context, state, render_info, custom_bone_to_world);
	Source::ModelRender->ForcedMaterialOverride(nullptr);

	// Hook again after our stuff is done
	ModelRenderTable->Hook();
	//ModelRenderTable->GetFunction<DrawModelExecute_t>(I_DrawModelExecute)(_this, context, state, render_info, custom_bone_to_world);
}

}
SOURCE_END