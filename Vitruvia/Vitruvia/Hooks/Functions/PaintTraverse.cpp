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
#include "../../SourceClasses/C_CSPlayer.h"
#include "../../Features/Visuals/Visuals.h"

#undef ENABLE_PTDEBUG

SOURCE_INIT
namespace Hooks {

static int draw_panel = -1; // This will hold the panel index we will be drawing on

void __fastcall hk_PaintTraverse(VGUI::IPanel* _this, void* edx, VPANEL panel, bool force_repaint, bool allow_force)
{
	using namespace Vitruvia;
	// Do not continue if the user is taking a screenshot at all (!!!)
	if (Source::Engine->IsTakingScreenshot())
		return;

	// Remove postprocessing if it's the case
	Vitruvia::Removals->NoPostProcessing();

	// Remove scope by skipping the "HudZoom" panel
	// But only if I am alive!
	auto local = C_CSPlayer::GetLocal();
	if (local && local->IsAlive() 
		&& !strcmp(Source::Panel->GetName(panel), enc("HudZoom")) && Vitruvia::Config::Removals.NoScope)
	{
		// Skip this shit panel baby
		return;
	}

	// Call the original PaintTraverse, then we can do our own drawings
	PanelTable->GetFunction<PaintTraverse_t>(I_PaintTraverse)(_this, panel, force_repaint, allow_force);

	// If we didn't determine the panel to draw on yet
	if (draw_panel == -1)
	{
		// Check if this is the right panel we want to draw on
		if (strcmp(Source::Panel->GetName(panel), DRAW_PANEL) == 0)
		{
			// Set the draw panel
			draw_panel = panel;
			// Setup fonts ONCE! This is a heavy task for the game
			Vitruvia::Render::Fonts::SetupFonts();
		}
	}

	// Draw only when we are using the right panel
	if (panel != draw_panel)
		return;

	// Draw shit here

	// Perform PaintTraverse behaviour
	for (auto f : Vitruvia::Features)
		f->OnPaintTraverse();

	// Draw debug vars.
	// Also remember that clogging the debugvar vector can cause lags
	// If it is disabled go on top of this cpp file and comment the #undef clause!
#ifdef ENABLE_PTDEBUG
	if (Source::Engine->IsInGame())
	{
		auto local = C_CSPlayer::GetLocal();
		auto weapon = local->GetActiveWeapon();
		QAngle va; Source::Engine->GetViewAngles(va);
		//DEBUG_VAR_STR(eDebugTypes::STRING, enc("weapon->GetDisplayName()"), local->GetActiveWeapon()->GetDisplayName(false));
		//DEBUG_VAR(eDebugTypes::BOOL, enc("weapon->CanFire()"), local->GetActiveWeapon()->CanFire(local->m_nTickBase()));

		//DEBUG_VAR(eDebugTypes::FLOAT, enc("weapon->GetCone()"), weapon->GetCone());
		//DEBUG_VAR(eDebugTypes::FLOAT, enc("weapon->GetInaccuracy()"), weapon->GetInaccuracy());
		//DEBUG_VAR(eDebugTypes::FLOAT, enc("weapon->GetSpread()"), weapon->GetSpread());
		//DEBUG_VAR(eDebugTypes::FLOAT, enc("local->GetHitchance()"), local->GetHitchance(GET_ENTITY(2), va));

		//DEBUG_VAR(eDebugTypes::FLOAT, enc("Source::Globals->cur_time"), Source::Globals->cur_time);
		//DEBUG_VAR(eDebugTypes::FLOAT, enc("weapon->GetPostponeFireReadyTime"), weapon->GetPostponeFireReadyTime());
		//DEBUG_VAR(eDebugTypes::FLOAT, enc("delta"), weapon->GetPostponeFireReadyTime() - Source::Globals->cur_time);

		DRAW_DEBUG(0, 320, Source::Color::White());
	}
#endif
}

}
SOURCE_END