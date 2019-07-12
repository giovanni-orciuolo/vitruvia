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

#include "Hooks.h"

SOURCE_INIT
namespace Hooks {

Vitruvia::V_HookManager
* BaseClientTable = nullptr,
* PanelTable = nullptr,
* ClientModeTable = nullptr,
* ModelRenderTable = nullptr,
* GameEventManagerTable = nullptr,
* SurfaceTable = nullptr,
* D3DTable = nullptr,
* ViewRenderTable = nullptr,
* EngineSoundTable = nullptr,
* EngineTraceTable = nullptr,
* MaterialSystemTable = nullptr,
* RenderViewTable = nullptr,
* EngineClientTable = nullptr;

WNDPROC old_WndProc = nullptr;

Vitruvia::V_RecvPropHook 
* m_nSequence = nullptr,
* m_bDidSmokeEffect = nullptr,
* m_flLowerBodyYawTarget = nullptr;

void HookFunctions()
{
	Log(enc("Initializing hook tables..."));

	BaseClientTable = new Vitruvia::V_HookManager(enc("IBaseClientDLL"), (uintptr_t**)Source::BaseClient);
	PanelTable = new Vitruvia::V_HookManager(enc("VGUI::IPanel"), (uintptr_t**)Source::Panel);
	ClientModeTable = new Vitruvia::V_HookManager(enc("ClientModeShared"), (uintptr_t**)Source::ClientMode);
	ModelRenderTable = new Vitruvia::V_HookManager(enc("IVModelRender"), (uintptr_t**)Source::ModelRender);
	GameEventManagerTable = new Vitruvia::V_HookManager(enc("IGameEventManager2"), (uintptr_t**)Source::EventManager);
	SurfaceTable = new Vitruvia::V_HookManager(enc("VGUI::ISurface"), (uintptr_t**)Source::Surface);
	D3DTable = new Vitruvia::V_HookManager(enc("D3DDevice"), (uintptr_t**)Vitruvia::Offsets::D3D_Device);
	ViewRenderTable = new Vitruvia::V_HookManager(enc("IViewRender"), (uintptr_t**)Source::ViewRender, true);
	EngineSoundTable = new Vitruvia::V_HookManager(enc("IEngineSound"), (uintptr_t**)Source::EngineSound);
	EngineTraceTable = new Vitruvia::V_HookManager(enc("IEngineTrace"), (uintptr_t**)Source::EngineTrace);
	MaterialSystemTable = new Vitruvia::V_HookManager(enc("IMaterialSystem"), (uintptr_t**)Source::MaterialSystem);
	RenderViewTable = new Vitruvia::V_HookManager(enc("IVRenderView"), (uintptr_t**)Source::RenderView);
	EngineClientTable = new Vitruvia::V_HookManager(enc("IVEngineClient"), (uintptr_t**)Source::Engine);
	Log(enc("Initializing hooks..."));

	//BaseClientTable->HookFunction(enc("CreateMove"), I_CreateMove, hk_CreateMove);
	BaseClientTable->HookFunction(enc("FrameStageNotify"), I_FrameStageNotify, hk_FrameStageNotify);
	BaseClientTable->HookFunction(enc("IN_KeyEvent"), I_IN_KeyEvent, hk_IN_KeyEvent);
	BaseClientTable->HookFunction(enc("DispatchUserMessage"), I_DispatchUserMessage, hk_DispatchUserMessage);
	//BaseClientTable->HookFunction(enc("WriteUserCmdDeltaToBuffer"), I_WriteUserCmdDeltaToBuffer, hk_WriteUserCmdDeltaToBuffer);
	BaseClientTable->Hook();

	PanelTable->HookFunction(enc("PaintTraverse"), I_PaintTraverse, hk_PaintTraverse);
	PanelTable->Hook();

	ClientModeTable->HookFunction(enc("CreateMove"), I_CMS_CreateMove, hk_CMS_CreateMove);
	ClientModeTable->HookFunction(enc("OverrideView"), I_OverrideView, hk_OverrideView);
	ClientModeTable->HookFunction(enc("GetViewModelFOV"), I_GetViewModelFOV, hk_GetViewModelFOV);
	ClientModeTable->Hook();

	ModelRenderTable->HookFunction(enc("DrawModelExecute"), I_DrawModelExecute, hk_DrawModelExecute);
	ModelRenderTable->Hook();

	GameEventManagerTable->HookFunction(enc("FireEvent"), I_FireEvent, hk_FireEvent);
	GameEventManagerTable->Hook();

	SurfaceTable->HookFunction(enc("PlaySound"), I_PlaySound, hk_PlaySound);
	SurfaceTable->HookFunction(enc("OnScreenSizeChanged"), I_OnScreenSizeChanged, hk_OnScreenSizeChanged);
	SurfaceTable->Hook();

	D3DTable->HookFunction(enc("Reset"), I_D3D_Reset, hk_Reset);
	D3DTable->HookFunction(enc("EndScene"), I_D3D_EndScene, hk_EndScene);
	D3DTable->Hook();

	// + 1 Because ViewRenderTable got RTTI at first position
	ViewRenderTable->HookFunction(enc("RenderView"), I_RenderView + 1, hk_RenderView);
	ViewRenderTable->HookFunction(enc("RenderSmokeOverlay"), I_RenderSmokeOverlay + 1, hk_RenderSmokeOverlay);
	ViewRenderTable->Hook();

	EngineSoundTable->HookFunction(enc("EmitSound"), I_EmitSound, hk_EmitSound);
	EngineSoundTable->Hook();

	EngineTraceTable->HookFunction(enc("TraceRay"), I_TraceRay, hk_TraceRay);
	EngineTraceTable->Hook();

	MaterialSystemTable->HookFunction(enc("OverrideConfig"), I_OverrideConfig, hk_OverrideConfig);
	MaterialSystemTable->Hook();

	RenderViewTable->HookFunction(enc("SceneEnd"), I_SceneEnd, hk_SceneEnd);
	RenderViewTable->Hook();

	EngineClientTable->HookFunction(enc("IsBoxVisible"), I_IsBoxVisible, hk_IsBoxVisible);
	EngineClientTable->Hook();

	old_WndProc = (WNDPROC)SetWindowLongPtr(Vitruvia::global->CSGO_Window(), GWLP_WNDPROC, (LONG_PTR)(hk_WndProc));
}

void HookProxies()
{
	Log(enc("Hooking netvar proxies..."));

	m_nSequence = new Vitruvia::V_RecvPropHook(enc("DT_BaseViewModel"), enc("m_nSequence"), hk_nSequence);
	m_bDidSmokeEffect = new Vitruvia::V_RecvPropHook(enc("DT_SmokeGrenadeProjectile"), enc("m_bDidSmokeEffect"), hk_bDidSmokeEffect);
	m_flLowerBodyYawTarget = new Vitruvia::V_RecvPropHook(enc("DT_CSPlayer"), enc("m_flLowerBodyYawTarget"), hk_flLowerBodyYawTarget);
	// Add here proxies
}

void UnHookFunctions()
{
	Log(enc("Destroying hook tables..."));
	SAFE_DELETE(BaseClientTable)
	SAFE_DELETE(PanelTable)
	SAFE_DELETE(ClientModeTable)
	SAFE_DELETE(ModelRenderTable)
	SAFE_DELETE(GameEventManagerTable)
	SAFE_DELETE(SurfaceTable)
	SAFE_DELETE(D3DTable)
	SAFE_DELETE(ViewRenderTable)
	SAFE_DELETE(EngineSoundTable)
	SAFE_DELETE(EngineTraceTable)
	SAFE_DELETE(MaterialSystemTable)
	SAFE_DELETE(RenderViewTable)
	SAFE_DELETE(EngineClientTable)
	SetWindowLongPtr(Vitruvia::global->CSGO_Window(), GWLP_WNDPROC, (LONG_PTR)(old_WndProc));
	// Delete here new hook tables
	Vitruvia::Render::D3D::g_pRenderer->InvalidateObjects();
}

void UnHookProxies()
{
	SAFE_DELETE(m_nSequence)
	SAFE_DELETE(m_bDidSmokeEffect)
	SAFE_DELETE(m_flLowerBodyYawTarget)
	// Delete here new netvar proxies
}

}
SOURCE_END