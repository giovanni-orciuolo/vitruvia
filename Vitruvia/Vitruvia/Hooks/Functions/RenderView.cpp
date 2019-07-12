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
#include "../../Features/Visuals/Visuals.h"

SOURCE_INIT
namespace Hooks {

void __fastcall hk_RenderView(IViewRender* _this, void* edx, const CViewSetup& view, const CViewSetup& hud_view, int clear_flags, int what_to_draw)
{
	for (auto f : Vitruvia::Features)
		f->OnRenderView(view, hud_view, clear_flags, what_to_draw);

	ViewRenderTable->GetFunction<RenderView_t>(I_RenderView)(_this, view, hud_view, clear_flags, what_to_draw);
}

void __fastcall hk_RenderSmokeOverlay(IViewRender* _this, void* edx)
{
	if (!Vitruvia::Config::Removals.NoSmoke)
		ViewRenderTable->GetFunction<RenderSmokeOverlay_t>(I_RenderSmokeOverlay)(_this);
}

void __fastcall hk_SceneEnd(IVRenderView* _this, void* edx)
{
	RenderViewTable->GetFunction<SceneEnd_t>(I_SceneEnd)(_this);

	for (auto f : Vitruvia::Features)
		f->OnSceneEnd();
}

}
SOURCE_END