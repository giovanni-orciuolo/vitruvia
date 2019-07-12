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
#include "../Renderer/D3D/D3D_Renderer.h"
#include "../SourceClasses/C_CSPlayer.h"
#include "../Config/Config.h"

namespace Vitruvia {

abstract_class V_IFeature
{
public:
	virtual ~V_IFeature() {}
	virtual void OnCreateMovePrePrediction(Source::CUserCmd* cmd) {}
	virtual void OnCreateMove(Source::CUserCmd* cmd) {}
	virtual void OnCreateMovePostPrediction(Source::CUserCmd* cmd) {}
	virtual void OnPaintTraverse() {}
	virtual void OnFrameStageNotify(Source::ClientFrameStage_t stage) {}
	virtual void OnFrameStageNotifyPostOriginal(Source::ClientFrameStage_t stage) {}
	virtual void OnOverrideView(Source::CViewSetup* setup) {}
	virtual void OnDrawModelExecute(void* context, void* state, const Source::ModelRenderInfo_t& render_info, matrix3x4_t* custom_bone_to_world) {}
	virtual void OnFireEvent(Source::IGameEvent* game_event) {}
	virtual void OnListenedEvent(Source::IGameEvent* game_event) {}
	virtual void OnPlaySound(const char* file_name) {}
	virtual void OnScreenSizeChanged(int old_width, int old_height) {}
	virtual void OnEndScene(Render::D3D::DrawManager* graphics) {}
	virtual void OnRenderView(const Source::CViewSetup& view, const Source::CViewSetup& hud_view, int clear_flags, int what_to_draw) {}
	virtual void OnKeyEvent(int event_code, Source::ButtonCode_t key, const char* key_binding) {}
	virtual void OnEmitSound(int ent_index, const char* sound_entry, const char* sound_sample, Source::Vector origin,
		Source::CUtlVector<Source::Vector>* origins, float sound_time) {}
	virtual void OnSceneEnd() {}
	virtual void OnUserMessage(int message_type, unsigned int message_length, const void* msg_data) {}
	virtual void OnLBYProxy(Source::C_CSPlayer* updater, float newValue) {}
	virtual void OnLoad() {}
	virtual void OnShutdown() {}
};

extern std::vector<V_IFeature*> Features;
extern void ClearFeatures();

}
