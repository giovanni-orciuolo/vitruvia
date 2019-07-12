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
#include "../IFeature.h"
#include "../../Tools/GrenadePrediction.h"

namespace Vitruvia {

class F_Visuals : public V_IFeature
{
public:
	bool ApplyPlayerFilters(Source::C_CSPlayer* entity, Source::C_CSPlayer* local);

	void Watermark();
	void ExecuteESP(Render::D3D::DrawManager* graphics);
	void DrawSounds(Render::D3D::DrawManager* graphics);
	void DrawSkeltal(Render::D3D::DrawManager* graphics, Source::C_CSPlayer* local, Source::C_CSPlayer* player, Source::IClientRenderable* renderable, BoneMatrix boneMatrix, Source::Color color, bool engine_drawing = false);

	void OnPaintTraverse() override;
	void OnEndScene(Render::D3D::DrawManager* graphics) override;
	void OnCreateMovePostPrediction(Source::CUserCmd* cmd) override;
	void OnOverrideView(Source::CViewSetup* setup) override;
	void OnEmitSound(int ent_index, const char* sound_entry, const char* sound_sample, Source::Vector origin,
		Source::CUtlVector<Source::Vector>* origins, float sound_time) override;
};

class F_Glow : public V_IFeature
{
	friend class F_Visuals;
private:
	Source::CGlowObjectManager* m_pGlowObjectManager;
public:
	F_Glow() : m_pGlowObjectManager(nullptr) {}

	void SetGlowManager(Source::CGlowObjectManager* glow_manager) { this->m_pGlowObjectManager = glow_manager; }
	Source::CGlowObjectManager* GetGlowManager() { return m_pGlowObjectManager; }

	void OnRenderView(const Source::CViewSetup& view, const Source::CViewSetup& hud_view, int clear_flags, int what_to_draw) override;
	void OnShutdown() override;
};

class F_Removals : public V_IFeature
{
public:
	friend class F_Chams;

	void NoPostProcessing();
	void NoScopeLines(Render::D3D::DrawManager* graphics);
	void NoVisualRecoil(Source::ClientFrameStage_t stage);
	void RestoreRecoilAngles();
	void NoFlash();
	void NoHands(const std::string& model_name);
	void NoWeapons(const std::string& model_name);
	void NoSmoke(Source::ClientFrameStage_t stage);
	void NoSky();

	void OnDrawModelExecute(void* context, void* state, const Source::ModelRenderInfo_t& render_info, matrix3x4_t* custom_bone_to_world) override;
	void OnFrameStageNotify(Source::ClientFrameStage_t stage) override;
	void OnEndScene(Render::D3D::DrawManager* graphics) override;
	void OnShutdown() override;
	void OnListenedEvent(Source::IGameEvent* game_event) override;
private:
	// Used inside NoVisRecoil
	Source::Vector m_vecAimPunch, m_vecViewPunch;
	Source::Vector* m_pAimPunch = nullptr, *m_pViewPunch = nullptr;
};

struct MaterialInfo_t;

class F_Chams : public V_IFeature
{
public:
	bool CreateMaterial(const std::string& material_name, MaterialInfo_t info); // Helper function to create a material
	void CreateMaterials(); // Call this once to create materials! Do it inside main startup
	void PlayerChams(Source::C_CSPlayer* player);
	void HandChams();
	void WeaponChams();
	void ThirdPersonTransparency();
	Source::IMaterial* GetFakeAnglesMaterial() { return m_pMaterialFakeAngles; }
	void OnDrawModelExecute(void* context, void* state, const Source::ModelRenderInfo_t& render_info, matrix3x4_t* custom_bone_to_world) override;
private:
	Source::IMaterial* m_pMaterialFlat; // Mat to use non-xqz, flat
	Source::IMaterial* m_pMaterialFlatXQZ; // Mat to use xqz, flat
	Source::IMaterial* m_pMaterialTextured; // Mat to use non-xqz, textured
	Source::IMaterial* m_pMaterialTexturedXQZ; // Mat to use xqz, textured
	Source::IMaterial* m_pMaterialFakeAngles; // Mat to use for fake angles chams
	 // Used across chams functions
	void* m_pContext;
	void* m_pState;
	Source::ModelRenderInfo_t* m_RenderInfo;
	matrix3x4_t* m_pCustomBoneToWorld;
};

struct Laser_t
{
	Source::C_CSPlayer* entity = nullptr;
	bool render = false;
	float alpha = 0.f;
	bool created = false;
	Source::Vector start = Source::Vector();
	Source::Vector end = Source::Vector();
	Source::Color color = Source::Color();

	Laser_t()
		: entity(nullptr), render(false), alpha(0.f), created(false), start(Source::Vector()), end(Source::Vector()), color(Source::Color())
	{}

	Laser_t(Source::C_CSPlayer* entity, bool render, float alpha, Source::Vector start, Source::Vector end, Source::Color color)
		: entity(entity), render(render), alpha(alpha), created(false), start(start), end(end), color(color)
	{}
};
struct BeamRing_t
{
	Source::C_CSPlayer* source = nullptr;
	bool render = false;
	float startRadius = 10.f;
	float endRadius = 285.f;
	float life = 0.6f;
	float width = 5.f;
	float speed = 10.f;
	Source::Color color = Source::Color::Blue();
	float alpha = 1.f;
};

class RadarPoint
{
public:
	RadarPoint()
		: entity(nullptr),
		color(Source::Color::White()),
		position(Source::Vector2())
	{}
	RadarPoint(Source::C_BaseEntity* entity, const Source::Color& color, const Source::Vector2& pos)
		: entity(entity),
		color(color),
		position(pos)
	{}
	Source::C_BaseEntity* GetEntity() const { return entity; }
	Source::Color GetColor() const { return color; }
	Source::Vector2 GetPos() const { return position; }
	void SetEntity(Source::C_BaseEntity* ent) { entity = ent; }
	void SetColor(const Source::Color& col) { color = col; }
	void SetPos(const Source::Vector2& pos) { position = pos; }
private:
	Source::C_BaseEntity* entity;
	Source::Color color;
	Source::Vector2 position;
};

class F_Radar : public V_IFeature
{
public:
	F_Radar()
		: radar_pos(Source::Vector2(20, 20)),
		mouse_down(false)
	{}
	void CollectPoints();
	void HandleMove();
	void Draw(Render::D3D::DrawManager* graphics);
	void MouseDown()
	{
		if (IsHovered()) 
		{
			auto mousePos = ImGui::GetMousePos();
			mouse_down = true;
			last_coords = Source::Vector2(mousePos.x, mousePos.y);
		}
	}
	void MouseUp() { mouse_down = false; }

	void OnEndScene(Render::D3D::DrawManager* graphics) override;
private:
	Source::Vector EntityToRadar(Source::C_BaseEntity* entity);
	bool IsHovered() const;
	Source::Vector2 last_coords;
	Source::Vector2 radar_pos;
	bool mouse_down;
	std::vector<RadarPoint> radar_points;
};

class F_VisualsOthers : public V_IFeature
{
public:
	void RenderCrosshair(Render::D3D::DrawManager* graphics);
	void ForceEngineRadar();
	void ColorWalls();
	void ThirdPerson(Source::CViewSetup* setup);
	void DrawHitmarker();
	void AddLaser(Laser_t laser);
	void AddBeamRing(BeamRing_t ring);
	void RenderSpreadCircle(Render::D3D::DrawManager* graphics);
	void PrintHurtLog(Source::C_CSPlayer* local, Source::C_CSPlayer* got_hurt, Source::IGameEvent* hurt_event);

	void OnRenderView(const Source::CViewSetup& view, const Source::CViewSetup& hud_view, int clear_flags, int what_to_draw) override;
	void OnOverrideView(Source::CViewSetup* setup) override;
	void OnDrawModelExecute(void* context, void* state, const Source::ModelRenderInfo_t& render_info, matrix3x4_t* custom_bone_to_world) override;
	void OnCreateMove(Source::CUserCmd* cmd) override;
	void OnKeyEvent(int event_code, Source::ButtonCode_t key, const char* key_binding) override;
	void OnEndScene(Render::D3D::DrawManager* graphics) override;
	void OnListenedEvent(Source::IGameEvent* game_event) override;
	void OnPaintTraverse() override;
	void OnShutdown() override;
};

extern F_Visuals* Visuals;
extern F_Glow* Glow;
extern F_Removals* Removals;
extern F_Chams* Chams;
extern F_Radar* Radar;
extern F_VisualsOthers* VisualsOthers;

}
