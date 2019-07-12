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
#include "..\InterfaceList\InterfaceList.h"
#include "..\Config\Config.h"
#include "..\Renderer\Renderer.h"
#include "..\Renderer\D3D\D3D_Renderer.h"
#include "..\Features\IFeature.h"
#include "..\GUI\Menu.h"
#include "Manager\HookManager.h"
#include "..\Globals\VitruviaGlobals.h"

// List of indexes

#define I_CreateMove 21
#define I_CMS_CreateMove 24
#define I_PaintTraverse 41
#define I_FrameStageNotify 36
#define I_OverrideView 18
#define I_DrawModelExecute 21
#define I_FireEvent 8
#define I_PlaySound 82
#define I_OnScreenSizeChanged 116
#define I_D3D_Reset 16
#define I_D3D_EndScene 42
#define I_IN_KeyEvent 20
#define I_RenderView 6
#define I_RenderSmokeOverlay 41
#define I_EmitSound 5
#define I_TraceRay 5
#define I_DispatchUserMessage 37
#define I_GetViewModelFOV 35
#define I_OverrideConfig 21
#define I_SceneEnd 9
#define I_IsBoxVisible 32
#define I_WriteUserCmdDeltaToBuffer 23 

SOURCE_INIT
namespace Hooks {

// List of typedefs

// IBaseClient::CreateMove
typedef
void(__thiscall* CreateMove_t)
(IBaseClientDLL* _this, int sequence_number, float input_sample_frametime, bool active);

// ClientModeShared::CreateMove
typedef
bool(__thiscall* CMS_CreateMove_t)
(IClientMode* _this, float sample_frametime, CUserCmd* cmd);

// VGUI::IPanel::PaintTraverse
typedef
void(__thiscall* PaintTraverse_t)
(VGUI::IPanel* _this, VPANEL panel, bool force_repaint, bool allow_force);

// IBaseClient::FrameStageNotify
typedef
void(__thiscall* FrameStageNotify_t)
(IBaseClientDLL* _this, ClientFrameStage_t stage);

// ClientModeShared::OverrideView
typedef
void(__thiscall* OverrideView_t)
(IClientMode* _this, CViewSetup* view);

// IVModelRender::DrawModelExecute
typedef
void*(__thiscall* DrawModelExecute_t)
(IVModelRender* _this, void* context, void* state, const ModelRenderInfo_t& render_info, matrix3x4_t* custom_bone_to_world);

// IGameEventManager::FireEvent
typedef
bool(__thiscall* FireEvent_t)
(IGameEventManager2* _this, IGameEvent* game_event, bool no_broadcast);

// VGUI::ISurface::PlaySound
typedef
void(__thiscall* PlaySound_t)
(VGUI::ISurface* _this, const char* file_name);

// VGUI::ISurface::OnScreenSizeChanged
typedef
void(__thiscall* OnScreenSizeChanged_t)
(VGUI::ISurface* _this, int old_width, int old_height);

// D3D::Reset
typedef
long(__stdcall* D3D_Reset_t)
(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_params);

// D3D::EndScene
typedef
long(__stdcall* D3D_EndScene_t)
(IDirect3DDevice9* device);

// IBaseClient::IN_KeyEvent
typedef
int(__thiscall* IN_KeyEvent_t)
(IBaseClientDLL* _this, int event_code, ButtonCode_t key, const char* key_binding);

// IViewRender::RenderView
typedef
void(__thiscall* RenderView_t)
(IViewRender* _this, const CViewSetup& view, const CViewSetup& hud_view, int clear_flags, int what_to_draw);

// IViewRender::RenderSmokeOverlay
typedef
void(__thiscall* RenderSmokeOverlay_t)
(IViewRender* _this);

// IEngineSound::EmitSound
typedef
int(__thiscall* EmitSound_t)
(IEngineSound* _this, IRecipientFilter& filter, int ent_index, int channel, const char* sound_entry, unsigned int sound_entry_hash,
	const char* sample, float volume, float attenuation, int seed, int flags, int pitch, const Vector* origin,
	const Vector* direction, CUtlVector<Vector>* vec_origins, bool update_positions, float sound_time, int speaker_entity);

// IEngineTrace::TraceRay
typedef
void(__thiscall* TraceRay_t)
(IEngineTrace* _this, const Ray_t& ray, unsigned int mask, ITraceFilter* filter, CGameTrace* trace);

// IBaseClient::DispatchUserMessage
typedef
bool(__thiscall* DispatchUserMessage_t)
(IBaseClientDLL* _this, int type, unsigned int unknown, unsigned int length, const void* msg_data);

// ClientModeShared::GetViewModelFOV
typedef
float(__thiscall* GetViewModelFOV_t)
(IClientMode* _this);

// CMaterialSystem::OverrideConfig
typedef
bool(__thiscall* OverrideConfig_t)
(IMaterialSystem* _this, MaterialSystem_Config_t* config, bool force_update);

// IVRenderView::SceneEnd
typedef
void(__thiscall* SceneEnd_t)
(IVRenderView* _this);

// IVEngineClient::IsBoxVisible
typedef
bool(__thiscall* IsBoxVisible_t)
(IVEngineClient* _this, const Vector& mins, const Vector& maxs);

// IBaseClient::WriteUserCmdDeltaToBuffer
typedef
bool(__thiscall* WriteUserCmdDeltaToBuffer_t)
(IBaseClientDLL* _this, int slot, void* buf, int from, int to, bool is_new_cmd);

// List of hk_ functions declarations, that need to be defined inside the proper .cpp file

extern void __fastcall hk_CreateMove(IBaseClientDLL* _this, void* edx, int sequence_number, float input_sample_frametime, bool active);
extern bool __fastcall hk_CMS_CreateMove(IClientMode* _this, void* edx, float sample_frametime, CUserCmd* cmd);
extern void __fastcall hk_PaintTraverse(VGUI::IPanel* _this, void* edx, VPANEL panel, bool force_repaint, bool allow_force);
extern void __fastcall hk_FrameStageNotify(IBaseClientDLL* _this, void* edx, ClientFrameStage_t stage);
extern void __fastcall hk_OverrideView(IClientMode* _this, void* edx, CViewSetup* setup);
extern void __fastcall hk_DrawModelExecute(IVModelRender* _this, void* edx, void* context, void* state, const ModelRenderInfo_t& render_info, matrix3x4_t* custom_bone_to_world);
extern bool __fastcall hk_FireEvent(IGameEventManager2* _this, void* edx, IGameEvent* game_event, bool no_broadcast);
extern void __fastcall hk_PlaySound(VGUI::ISurface* _this, void* edx, const char* file_name);
extern void __fastcall hk_OnScreenSizeChanged(VGUI::ISurface* _this, void* edx, int old_width, int old_height);
extern int  __fastcall hk_IN_KeyEvent(IBaseClientDLL* _this, void* edx, int event_code, ButtonCode_t key, const char* key_binding);
extern void __fastcall hk_RenderView(IViewRender* _this, void* edx, const CViewSetup& view, const CViewSetup& hud_view, int clear_flags, int what_to_draw);
extern void __fastcall hk_RenderSmokeOverlay(IViewRender* _this, void* edx);
extern int  __fastcall hk_EmitSound(IEngineSound* _this, void* edx, IRecipientFilter& filter, int ent_index, int channel, 
	const char* sound_entry, unsigned int sound_entry_hash,
	const char* sample, float volume, float attenuation, int seed, int flags, int pitch, const Vector* origin,
	const Vector* direction, CUtlVector<Vector>* vec_origins, bool update_positions, float sound_time, int speaker_entity);
extern void __fastcall hk_TraceRay(IEngineTrace* _this, void* edx, const Ray_t& ray, unsigned int mask, ITraceFilter* filter, CGameTrace* trace);
extern bool __fastcall hk_DispatchUserMessage(IBaseClientDLL* _this, void* edx, int type, unsigned int unknown, unsigned int length, const void* msg_data);
extern float __fastcall hk_GetViewModelFOV(IClientMode* _this, void* edx);
extern bool __fastcall hk_OverrideConfig(IMaterialSystem* _this, void* edx, MaterialSystem_Config_t* config, bool force_update);
extern void __fastcall hk_SceneEnd(IVRenderView* _this, void* edx);
extern bool __fastcall hk_IsBoxVisible(IVEngineClient* _this, void* edx, const Vector& mins, const Vector& maxs);
extern bool __fastcall hk_WriteUserCmdDeltaToBuffer(IBaseClientDLL* _this, void* edx, int slot, void* buf, int from, int to, bool is_new_cmd);

extern long __stdcall hk_Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_params);
extern long __stdcall hk_EndScene(IDirect3DDevice9* device);

extern LRESULT __stdcall hk_WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
extern WNDPROC old_WndProc; // Original wndproc ptr

// List of netvar proxy managers, with relative hooked proxy

extern Vitruvia::V_RecvPropHook
* m_nSequence,
* m_bDidSmokeEffect,
* m_flLowerBodyYawTarget;
extern void hk_nSequence(const CRecvProxyData* pData, void* pStruct, void* pOut);
extern void hk_bDidSmokeEffect(const CRecvProxyData* pData, void* pStruct, void* pOut);
extern void hk_flLowerBodyYawTarget(const CRecvProxyData* pData, void* pStruct, void* pOut);

// List of HookManagers for each table that contains functions to hook

extern Vitruvia::V_HookManager
* BaseClientTable,
* PanelTable,
* ClientModeTable,
* ModelRenderTable,
* GameEventManagerTable,
* SurfaceTable,
* D3DTable,
* ViewRenderTable,
* EngineSoundTable,
* EngineTraceTable,
* MaterialSystemTable,
* RenderViewTable,
* EngineClientTable;

// This is the function that basically does all the hooks
extern void HookFunctions();
// Unhook everything
extern void UnHookFunctions();
// This is the function that doess all the netvar proxy hooks
extern void HookProxies();
// Unhook proxies
extern void UnHookProxies();

}
SOURCE_END