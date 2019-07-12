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
#include "Visuals.h"
#include "../../Tools/ThirdPerson.h"
#include "../../SourceClasses/C_CSPlayer.h"
#include "../../SourceClasses/C_PlantedC4.h"
#include "../../Database.h"
#include "../../Features/HvH/HvH.h"
using namespace Source;

#define CD3D(clr) ImU32(clr.toD3D())

// The blue that I'm using through visuals
static Source::Color light_blue = Source::Color(0, 200, 255);
static const float headpoint_radius = 2.f;

// Used for fadein/out esp
static std::array<float, MAX_CLIENTS> g_VisualsAlpha{};

namespace Vitruvia {

F_Visuals* Visuals;
F_Glow* Glow;
F_Removals* Removals;
F_Chams* Chams;
F_Radar* Radar;
F_VisualsOthers* VisualsOthers;

/////////////////////////////////  VISUALS
#pragma region VISUALS

// Sound info to pass drawing
struct Snd_t
{
	int ent_index;
	std::string sound_entry;
	std::string sound_sample;
	Vector origin;
	float time;
	float alpha;
	bool operator==(const Snd_t& rhs) { return this->origin == rhs.origin; }
};
static std::vector<Snd_t> sounds{};

void ApplyDormantAlpha()
{
	auto local = C_CSPlayer::GetLocal();
	if (local)
	{
		// Opacity should reach 1 in ESP.FadeSpeed second(s).
		float secs = Config::ESP.FadeSpeed.m_Value;
		secs = clamp<float, float>(secs, 0.1f, 2.f);
		float alpha_frequency = 1.f / Config::ESP.FadeSpeed.m_Value;
		FOR_EACH_CLIENT
		{
			auto player = GET_ENTITY(ClientIndex);
			if (player != nullptr && player != local)
			{
				float step = alpha_frequency * Source::Globals->frame_time;
				(player->GetNetworkable()->IsDormant())
					? g_VisualsAlpha[player->GetIndex()] -= step :
					g_VisualsAlpha[player->GetIndex()] += step;
				g_VisualsAlpha[player->GetIndex()] = clamp<float, float>(g_VisualsAlpha[player->GetIndex()], 0.f, 1.f);
			}
		}
	}
}

// Apply various filters to a player, used inside ExecuteESP()
bool F_Visuals::ApplyPlayerFilters(C_CSPlayer* player, C_CSPlayer* local)
{
	// Check if player is valid
	if (player == nullptr)
		return false;

	// Check if player is myself
	if (player == local)
		return false;

	// Check if player is dormant
	//if (player->GetNetworkable()->IsDormant())
	//	return false;

	// Check if player is alive
	if (player->m_lifeState() != LIFE_ALIVE)
		return false;

	// If filters isn't set to "All", it means we need to filter
	if (!Config::VisualsFilters.All)
	{
		// If filter TeamMates isn't enabled we need to skip team mates
		if (!Config::VisualsFilters.TeamMates &&
			local->m_iTeamNum() == player->m_iTeamNum())
			return false;

		// If filter Enemies isn't enabled we need to skip enemies
		if (!Config::VisualsFilters.Enemies &&
			local->m_iTeamNum() != player->m_iTeamNum())
			return false;

		// If filter VisibleOnly is enabled we need to skip non-visible players
		if (Config::VisualsFilters.VisibleOnly)
		{
			// This means first of all players that are behind a smoke, then players non-visible at all
			// Maybe TODO: Add BehindSmoke option in filters

			// If player is behind a smoke, skip
			if (player->IsBehindSmoke(HITBOX_BODY))
				return false;

			// If player isn't visible at all, skip
			if (!player->IsVisible(HITBOX_BODY))
				return false;
		}
	}

	return true;
}

// Simply draws the watermark
void F_Visuals::Watermark()
{
	using namespace Render;
	if (!Config::ESP.Watermark)
		return;

	// Here we can draw the watermark for this pCheat xd
	Color watermark_color = Color(255, 255, 255, scast<int>(Config::ESP.WatermarkAlpha.m_Value));
	String(Fonts::Tahoma_Outline15, global->ScreenWidth(), 0, watermark_color, eAlign::ALIGN_RIGHT, WATERMARK);
}

// I use PaintTraverse to draw the watermark and to perform various tests.
// I also perform Hitbox ESP here since I need access to DebugOverlay to draw proper hitpills!
void F_Visuals::OnPaintTraverse()
{
	Watermark();

	// TODO: Hitbox ESP is flickering kinda a lot. Fix pliz
	if (Config::ESP.Active && Config::ESP.Hitbox != 0)
	{
		auto local = C_CSPlayer::GetLocal();
		if (local == nullptr)
			return;

		// For each client (a player from slot 1 to 64), check if client is valid, apply filters, perform hitbox esp
		FOR_EACH_CLIENT
		{
			auto player = GET_ENTITY(ClientIndex);

			if (!this->ApplyPlayerFilters(player, local))
				continue;

			// Determine color
			Color hitbox_color = player->m_iTeamNum() != local->m_iTeamNum() ?
				Config::Colors.HitboxEnemy.m_Value : Config::Colors.HitboxTeam.m_Value;
			// Get a static matrix
			static matrix3x4_t boneToWorld[MAXSTUDIOBONES];
			auto renderable = player->GetRenderable();
			if (rcast<offset_t>(renderable) == 0xE || rcast<offset_t>(renderable) == 0xF)
				continue;

			if (!renderable->SetupBones(boneToWorld, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, Source::Globals->cur_time))
				continue;

			// If value is 1 draw all the hitboxes, otherwise if value is 2 or 3 etc draw the hitbox which is value - 1
			// For each hitbox
			for (int i = 0; i < HITBOX_MAX; i++)
			{
				// Skip it if value isn't 'All' and this is not the right hitbox
				if (Config::ESP.Hitbox != 1 && i != (Config::ESP.Hitbox - 1))
					continue;

				// Get studio box for this hitbox
				auto hitbox = player->GetStudioBox(i);
				if (hitbox == nullptr)
					continue;

				// Apply some transformations
				Vector mins = Maths::VectorTransform(hitbox->bbmin, boneToWorld[hitbox->bone]);
				Vector maxs = Maths::VectorTransform(hitbox->bbmax, boneToWorld[hitbox->bone]);
				float diameter = hitbox->radius;

				// Account for alpha
				hitbox_color.SetAlpha(g_VisualsAlpha[player->GetIndex()]);

				// Add pill to debug overlay...
				Source::DebugOverlay->AddPill(mins, maxs, diameter, hitbox_color.r(), hitbox_color.g(), hitbox_color.b(), hitbox_color.a(), 0.02f);
			}
		}
	}

	/* 
	// DRAW MULTIPOINTS DEBUG
	std::vector<Vector> points{};
	if (GetPointsForHitbox(GET_ENTITY(2), HITBOX_HEAD, points))
	{
		for (auto& point : points)
		{
			Vector point_screen;
			if (Render::WorldToScreen(point, point_screen))
				Render::FilledCircle(point_screen.x, point_screen.y, 3, 32, Color::White());
		}
	}
	*/
}

// Box definitions

struct Box_t
{
	ImVec2 topLeft = ImVec2(0, 0);
	ImVec2 bottomRight = ImVec2(0, 0);
	Color color = Color::White();
	float rounding = 0.f;
	float thickness = 1.f;
	float height = 0.f;
	float width = 0.f;

	void Render(Render::D3D::DrawManager* graphics)
	{
		graphics->Rect(topLeft, bottomRight, CD3D(color), rounding, -1, thickness);
		if (Config::ESP.BoxOutline.m_Value)
		{
			Vector2 tl_outline = Vector2(topLeft.x, topLeft.y); tl_outline -= 1.f;
			Vector2 br_outline = Vector2(bottomRight.x, bottomRight.y); br_outline += 1.f;
			graphics->Rect(ImVec2(tl_outline.x, tl_outline.y), ImVec2(br_outline.x, br_outline.y), CD3D(Color(0, 0, 0, color.a())), rounding, -1, thickness);
			Vector2 tl_outline2 = Vector2(topLeft.x, topLeft.y); tl_outline2 += 1.f;
			Vector2 br_outline2 = Vector2(bottomRight.x, bottomRight.y); br_outline2 -= 1.f;
			graphics->Rect(ImVec2(tl_outline2.x, tl_outline2.y), ImVec2(br_outline2.x, br_outline2.y), CD3D(Color(0, 0, 0, color.a())), rounding, -1, thickness);
		}
	}
	void RenderFiller(Render::D3D::DrawManager* graphics)
	{
		// Render the filler rectangle here
		if (!Config::ESP.BoxFilled.m_Value) 
			return;
		Color color_filler = color;
		color_filler.SetAlpha(Config::ESP.BoxFillAlpha.m_Value * color.a());
		graphics->RectFilled(topLeft, bottomRight, CD3D(color_filler));
	}
};

struct BoxCorner_t : Box_t
{
	void Render(Render::D3D::DrawManager* graphics)
	{
		/* TODO: I corner sono un po' storti... */
		float w = width / 2.f;
		float h = height;
		float x = topLeft.x + w;
		float y = topLeft.y;

		auto draw_corner = [&](float _x, float _y, float _w, float _h, bool _right_side, bool _down, Color _col, bool _outline) -> void
		{
			const auto corner_x = _right_side ? _x - _w : _x;
			const auto corner_y = _down ? _y - _h : _y;
			const auto corner_w = _down && _right_side ? _w + 1 : _w;

			graphics->Line(ImVec2(corner_x, _y), corner_w, 1, CD3D(_col));
			graphics->Line(ImVec2(_x, corner_y), 1, _h, CD3D(_col));

			if (_outline)
			{
				graphics->Line(ImVec2(corner_x, _down ? _y + 1 : _y - 1), !_down && _right_side ? corner_w + 1 : corner_w, 1, CD3D(Color::Black()));
				graphics->Line(ImVec2(_right_side ? _x + 1 : _x - 1, _down ? corner_y : corner_y - 1), 1, _down ? _h + 2 : _h + 1, CD3D(Color::Black()));
			}
		};

		draw_corner(x - w, y, w / 2, w / 2, false, false, color, Config::ESP.BoxOutline.m_Value);
		draw_corner(x - w, y + h, w / 2, w / 2, false, true, color, Config::ESP.BoxOutline.m_Value);
		draw_corner(x + w, y, w / 2, w / 2, true, false, color, Config::ESP.BoxOutline.m_Value);
		draw_corner(x + w, y + h, w / 2, w / 2, true, true, color, Config::ESP.BoxOutline.m_Value);
	}

	BoxCorner_t& operator=(const Box_t& box)
	{
		this->topLeft = box.topLeft;
		this->bottomRight = box.bottomRight;
		this->color = box.color;
		this->rounding = box.rounding;
		this->thickness = box.thickness;
		this->height = box.height;
		this->width = box.width;
		return *this;
	}
};

struct Box3D_t : Box_t
{
	static const int MAX_CORNERS = 8;
	C_CSPlayer* entity = nullptr;
	Vector corners[MAX_CORNERS] = {};

	void Render(Render::D3D::DrawManager* graphics)
	{
		if (entity == nullptr) return;

		Vector vecMins = entity->GetCollideable()->OBBMins();
		Vector vecMaxs = entity->GetCollideable()->OBBMaxs();

		Vector vecCorners[MAX_CORNERS] =
		{
			Vector(vecMins.x,vecMins.y,vecMins.z),
			Vector(vecMins.x,vecMaxs.y,vecMins.z),
			Vector(vecMaxs.x,vecMaxs.y,vecMins.z),
			Vector(vecMaxs.x,vecMins.y,vecMins.z),
			Vector(vecMins.x,vecMins.y,vecMaxs.z),
			Vector(vecMins.x,vecMaxs.y,vecMaxs.z),
			Vector(vecMaxs.x,vecMaxs.y,vecMaxs.z),
			Vector(vecMaxs.x,vecMins.y,vecMaxs.z)
		};

		float eye_yaw = entity->GetAbsAngles().y;

		for (int i = 0; i < MAX_CORNERS; i++)
			vecCorners[i].Rotate(eye_yaw);

		Vector _vecCorners[MAX_CORNERS];
		for (int i = 0; i < MAX_CORNERS; i++) _vecCorners[i] = Vector();

		for (int i = 0; i < MAX_CORNERS; i++)
			if (!graphics->W2S(entity->GetAbsOrigin() + vecCorners[i], _vecCorners[i]))
				continue;

		for (int i = 1; i <= 4; i++)
		{
			if (_vecCorners[i - 1].x == 0 || _vecCorners[i - 1].y == 0) continue;
			if (_vecCorners[i % 4].x == 0 || _vecCorners[i % 4].y == 0) continue;
			if (_vecCorners[i + 3].x == 0 || _vecCorners[i + 3].y == 0) continue;
			if (_vecCorners[i % 4 + 4].x == 0 || _vecCorners[i % 4 + 4].y == 0) continue;

			// Base [0 - 1 - 2 - 3]
			graphics->Line(ImVec2(_vecCorners[i - 1]), ImVec2(_vecCorners[i % 4]), CD3D(color));
			if (Config::ESP.BoxOutline.m_Value)
			{
				graphics->Line(ImVec2((int)(_vecCorners[i - 1].x - 1), (int)(_vecCorners[i - 1].y - 1)), ImVec2((int)(_vecCorners[i % 4].x - 1), (int)(_vecCorners[i % 4].y - 1)), CD3D(Color(0, 0, 0, color.a())));
				graphics->Line(ImVec2((int)(_vecCorners[i - 1].x + 1), (int)(_vecCorners[i - 1].y + 1)), ImVec2((int)(_vecCorners[i % 4].x + 1), (int)(_vecCorners[i % 4].y + 1)), CD3D(Color(0, 0, 0, color.a())));
			}

			// 4 lines that go to the top
			graphics->Line(ImVec2(_vecCorners[i - 1]), ImVec2(_vecCorners[i + 3]), CD3D(color));
			if (Config::ESP.BoxOutline.m_Value)
			{
				graphics->Line(ImVec2((int)(_vecCorners[i - 1].x - 1), (int)(_vecCorners[i - 1].y - 1)), ImVec2((int)(_vecCorners[i + 3].x - 1), (int)(_vecCorners[i + 3].y - 1)), CD3D(Color(0, 0, 0, color.a())));
				graphics->Line(ImVec2((int)(_vecCorners[i - 1].x + 1), (int)(_vecCorners[i - 1].y + 1)), ImVec2((int)(_vecCorners[i + 3].x + 1), (int)(_vecCorners[i + 3].y + 1)), CD3D(Color(0, 0, 0, color.a())));
			}

			// Top [4 - 5 - 6 - 7]
			graphics->Line(ImVec2(_vecCorners[i + 3]), ImVec2(_vecCorners[i % 4 + 4]), CD3D(color));
			if (Config::ESP.BoxOutline.m_Value)
			{
				graphics->Line(ImVec2((int)(_vecCorners[i + 3].x - 1), (int)(_vecCorners[i + 3].y - 1)), ImVec2((int)(_vecCorners[i % 4 + 4].x - 1), (int)(_vecCorners[i % 4 + 4].y - 1)), CD3D(Color(0, 0, 0, color.a())));
				graphics->Line(ImVec2((int)(_vecCorners[i + 3].x + 1), (int)(_vecCorners[i + 3].y + 1)), ImVec2((int)(_vecCorners[i % 4 + 4].x + 1), (int)(_vecCorners[i % 4 + 4].y + 1)), CD3D(Color(0, 0, 0, color.a())));
			}
		}

		for (int i = 0; i < MAX_CORNERS; i++)
			corners[i] = _vecCorners[i];
	}
	void RenderFiller(Render::D3D::DrawManager* graphics)
	{
		if (Config::ESP.BoxFilled.m_Value)
		{
			Color color_filler = color;
			color_filler.SetColor(color_filler.r(), color_filler.g(), color_filler.b(), Config::ESP.BoxFillAlpha.m_Value);
			for (int i = 0; i < MAX_CORNERS; i++)
				if (corners[i].x == 0 || corners[i].y == 0) return;

			// Base layer [0 - 1 - 2 - 3]
			graphics->QuadFilled(
				ImVec2(corners[0]), ImVec2(corners[1]),
				ImVec2(corners[2]), ImVec2(corners[3]),
				CD3D(color_filler));

			// Left layer [0 - 4 - 7 - 3]
			graphics->QuadFilled(
				ImVec2(corners[0]), ImVec2(corners[4]),
				ImVec2(corners[7]), ImVec2(corners[3]),
				CD3D(color_filler));

			// Back layer [0 - 1 - 5 - 4]
			graphics->QuadFilled(
				ImVec2(corners[0]), ImVec2(corners[1]),
				ImVec2(corners[5]), ImVec2(corners[4]),
				CD3D(color_filler));

			// Right layer [1 - 5 - 6 - 2]
			graphics->QuadFilled(
				ImVec2(corners[1]), ImVec2(corners[5]),
				ImVec2(corners[6]), ImVec2(corners[2]),
				CD3D(color_filler));

			// Front layer [2 - 3 - 7 - 6]
			graphics->QuadFilled(
				ImVec2(corners[2]), ImVec2(corners[3]),
				ImVec2(corners[7]), ImVec2(corners[6]),
				CD3D(color_filler));

			// Top layer [4 - 5 - 6 - 7]
			graphics->QuadFilled(
				ImVec2(corners[4]), ImVec2(corners[5]),
				ImVec2(corners[6]), ImVec2(corners[7]),
				CD3D(color_filler));
		}
	}

	void RenderBars(Render::D3D::DrawManager* graphics)
	{
		// TODO: Fix bar maths
		for (int i = 0; i < MAX_CORNERS; i++)
			if (corners[i].x == 0 || corners[i].y == 0) 
				return;
		if (Config::ESP.Health.m_Value)
		{
			ImVec2 a = { corners[7].x - 5, corners[7].y },
				b = { a.x + 4, a.y },
				c = { corners[3].x - 2, corners[3].y },
				d = { corners[3].x - 5, corners[3].y };

			// Modulate color from green to red based on health
			int value = entity->m_iHealth();
			int r = scast<int>(255.f - (value * 2.55f));
			int g = scast<int>(value * 2.55f);

			int height = a.y - d.y;
			int hp = height - (int)((height * entity->m_iHealth()) / 100);
			graphics->QuadFilled(a, b, { c.x, c.y + hp }, { d.x, d.y + hp }, CD3D(Color(r, g, 0, color.a())));
			if (Config::ESP.BoxOutline)
				graphics->Quad(a, b, c, d, CD3D(Color(0, 0, 0, color.a())));
		}
		if (Config::ESP.Armor.m_Value)
		{
			ImVec2 a = { corners[6].x + 2, corners[6].y },
				b = { corners[6].x + 5, corners[6].y },
				c = { corners[2].x + 5, corners[2].y },
				d = { corners[2].x + 2, corners[2].y };

			int height = a.y - d.y;
			int hp = height - (int)((height * entity->m_iHealth()) / 100);
			Color clr = light_blue;
			clr.SetAlpha(color.a());
			graphics->QuadFilled(a, b, { c.x, c.y + hp }, { d.x, d.y + hp }, CD3D(clr));
			if (Config::ESP.BoxOutline)
				graphics->Quad(a, b, c, d, CD3D(Color(0, 0, 0, color.a())));
		}
	}
};

// Bar definitions

struct Bar_t : Box_t
{
	int value = 0;

	void Render(Render::D3D::DrawManager* graphics)
	{
		int hp = height - scast<int>(height * value / 100);
		auto black = CD3D(Color(0, 0, 0, color.a()));
		// Normal bar
		graphics->Rect(
			{ topLeft.x + 1, topLeft.y - 1 }, 
			1, height - hp, 
			CD3D(color)
		);
		if (Config::ESP.BoxOutline)
		{
			// Bar outline
			graphics->Rect(
				{ topLeft.x - 1, topLeft.y - 1 }, 
				{ bottomRight.x + 1, bottomRight.y + 1 }, black
			);
		}
	}

	void RenderBattery(Render::D3D::DrawManager* graphics)
	{
		// Bar outline + battery bars
		float boxes = std::ceil(value / 10);
		auto black = CD3D(Color(0, 0, 0, color.a()));
		graphics->Rect(
			{ topLeft.x + 1, topLeft.y - 1 },
			1, height / 10 * boxes + 1,
			CD3D(color)
		);
		graphics->Rect(
		{ topLeft.x - 1, topLeft.y - 1 },
		{ bottomRight.x + 1, bottomRight.y + 1 }, black
		);
		for (int i = 0; i < 10; i++)
		{
			float y = topLeft.y + i * (height / 10);
			graphics->Line(
			{ topLeft.x, y },
			{ topLeft.x + width, y }, black
			);
		}
	}
};

// Little helper function to compute mins and maxs into something usable
auto ComputeMinsAndMaxs(Render::D3D::DrawManager* graphics, C_BaseEntity* entity,
	float& left, float& right, float& top, float& bottom) -> bool
{
	// Check if entity passed is valid ptr
	if (entity == nullptr)
		return false;

	// Get coordinate frame of the entity to get transform matrix
	matrix3x4_t* trans_ptr = entity->GetCoordinateFrame();
	if (trans_ptr == nullptr)
		return false;

	const matrix3x4_t& trans = *trans_ptr;

	// Get collisions min and max
	Vector min = entity->GetCollideable()->OBBMins();
	Vector max = entity->GetCollideable()->OBBMaxs();

	// Setup points array
	Vector points[] = { Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z) };

	// Prepare array of points transformed
	Vector points_transformed[8];

	// Transform points through coordinate frame which is the transformer matrix
	for (int i = 0; i < 8; i++)
		points_transformed[i] = Maths::VectorTransform(points[i], trans);

	// Now we need to perform 8 W2S calls to transform each of these transformed points in screen coordinates
	Vector flb; // Front left bottom
	Vector brt; // Bottom right top
	Vector blb; // Back left bottom
	Vector frt; // Front right top
	Vector frb; // Front right back
	Vector brb; // Back right bottom
	Vector blt; // Back left top
	Vector flt; // Front left top

	// EXECUTE 8 WORLD TO SCREENS!
	if (!graphics->W2S(points_transformed[3], flb)
		|| !graphics->W2S(points_transformed[5], brt)
		|| !graphics->W2S(points_transformed[0], blb)
		|| !graphics->W2S(points_transformed[4], frt)
		|| !graphics->W2S(points_transformed[2], frb)
		|| !graphics->W2S(points_transformed[1], brb)
		|| !graphics->W2S(points_transformed[6], blt)
		|| !graphics->W2S(points_transformed[7], flt))
		return false;

	Vector points_on_screen[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	// Now we can fill params with Y coordinates
	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;

	// Clamp shit
	for (int i = 1; i < 8; i++)
	{
		if (left > points_on_screen[i].x)
			left = points_on_screen[i].x;
		if (bottom < points_on_screen[i].y)
			bottom = points_on_screen[i].y;
		if (right < points_on_screen[i].x)
			right = points_on_screen[i].x;
		if (top > points_on_screen[i].y)
			top = points_on_screen[i].y;
	}

	return true;
}

// Little helper function to get the Box_t of an entity
auto GetBoxForEntity(Render::D3D::DrawManager* graphics, C_BaseEntity* entity, Box_t& box) -> bool
{
	// Compute mins and maxs into something more usable for this entity
	float left, right, top, bottom;

	if (!ComputeMinsAndMaxs(graphics, entity, left, right, top, bottom))
		return false;

	// Now we can actually build our beloved box

	// Build the box coordinates
	box.topLeft.x = left;
	box.topLeft.y = top;
	box.bottomRight.x = right;
	box.bottomRight.y = bottom;

	box.height = (bottom - top);
	box.width = (right - left);

	return true;
}

// This is the huge function that performs all the ESP logic. Most of it is here
void F_Visuals::ExecuteESP(Render::D3D::DrawManager* graphics)
{
	using namespace Render::D3D;
	
	// Grab local player
	auto local = C_CSPlayer::GetLocal();
	if (local == nullptr)
		return;

	ApplyDormantAlpha();

	//////////////////////////////////////////////////////////////////////////////////////////
	// PLAYER ESP HERE
	//////////////////////////////////////////////////////////////////////////////////////////
	
	// For each client (a player from slot 1 to 64), check if client is valid, apply filters, perform ESP
	FOR_EACH_CLIENT
	{
		auto player = GET_ENTITY(ClientIndex);
		
		if (!this->ApplyPlayerFilters(player, local))
			continue;

		// We are done with filters, we have a valid player ptr that respects all the filters and validity conditions
		// Let's perform ESP on him!

		// First of all we get the box for this player
		Box_t player_box;

		// Set the color
		player_box.color = player->m_iTeamNum() != local->m_iTeamNum() ? 
			Config::Colors.BoxEnemy : Config::Colors.BoxTeam;

		// Account for visuals alpha
		player_box.color.SetAlpha(g_VisualsAlpha[player->GetIndex()]);

		// Get box for this player
		if (!GetBoxForEntity(graphics, rcast<C_BaseEntity*>(player), player_box))
			continue;

		// Render the box based on BoxType
		if (Config::ESP.BoxEnabled)
		{
			switch (Config::ESP.BoxType)
			{
			case 0:
			{	// Full
				player_box.Render(graphics);
				player_box.RenderFiller(graphics);
				break;
			}
			case 1:
			{	// Corner
				BoxCorner_t corner_box;
				corner_box = player_box;
				corner_box.Render(graphics);
				corner_box.RenderFiller(graphics);
				break;
			}
			case 2:
			{	// 3D
				Box3D_t box_3d;
				box_3d.color = player_box.color;
				box_3d.entity = player;
				box_3d.Render(graphics);
				box_3d.RenderFiller(graphics);
				box_3d.RenderBars(graphics);
				break;
			}
			}
		}

		// Create health-bar and armor-bar (only if we aren't using 3D mode)
		// If we are using 3D mode, bars are already rendered inside the switch case
		Bar_t barHealth, barArmor;
		if (Config::ESP.BoxType != 2)
		{
			if (Config::ESP.Health)
			{
				barHealth.value = player->m_iHealth();
				// Modulate color from green to red based on health
				float multiplier = 0.03; // 12 / 360
				multiplier *= std::ceil(barHealth.value / 10) - 1;
				// Set bar coords and render
				barHealth.topLeft = { player_box.topLeft.x - 5, player_box.topLeft.y };
				barHealth.bottomRight = { player_box.bottomRight.x - player_box.width - 2, player_box.bottomRight.y };
				barHealth.width = 3;
				barHealth.height = player_box.height;
				Color col = Color::FromHSB(multiplier, 1, 1);
				barHealth.color = Color(col.r(), col.g(), col.b(), g_VisualsAlpha[player->GetIndex()] * 255);
				Config::ESP.BatteryBars ? 
					barHealth.RenderBattery(graphics) :
					barHealth.Render(graphics);
			}
			if (Config::ESP.Armor)
			{
				barArmor.value = player->m_ArmorValue();
				// Set bar coords and render
				barArmor.topLeft = { player_box.topLeft.x + player_box.width + 2, player_box.topLeft.y };
				barArmor.bottomRight = { player_box.bottomRight.x + 5, player_box.bottomRight.y };
				barArmor.color = Source::Color(0, 200, 255, g_VisualsAlpha[player->GetIndex()] * 255);
				barArmor.width = 3;
				barArmor.height = player_box.height;
				Config::ESP.BatteryBars ?
					barArmor.RenderBattery(graphics) :
					barArmor.Render(graphics);
			}
		}

		// Name of the player above the box
		if (Config::ESP.Name)
		{
			// Get player's name
			auto player_name = player->GetPlayerInfo().name;
			
			// Render it!
			graphics->Text({ player_box.topLeft.x + player_box.width / 2, player_box.topLeft.y - graphics->GetFontSize() }, CD3D(Color(255, 255, 255, g_VisualsAlpha[player->GetIndex()] * 255)), centered_x, enc("%s"), player_name);
		}

		// Weapon name text under the box
		if (Config::ESP.Weapon)
		{
			// Get player's weapon
			auto weapon = player->GetActiveWeapon();
			if (weapon == nullptr)
				continue;

			// Get display name for the weapon
			auto weapon_name = weapon->GetDisplayName();

			// Render it!
			graphics->Text({ player_box.bottomRight.x - player_box.width / 2, player_box.bottomRight.y }, CD3D(Color(255, 255, 255, g_VisualsAlpha[player->GetIndex()] * 255)), centered_x,
				!Config::ESP.Ammo.m_Value ?
				enc("%s"), weapon_name.c_str() :
				enc("%s [%d/%d]"), weapon_name.c_str(), weapon->m_iClip1(), weapon->m_iPrimaryReserveAmmoCount()
				);
		}

		// Money text under weapon name if it's enabled, under box if not
		if (Config::ESP.Money)
		{
			// Get money money money
			int money = player->m_iAccount();

			// Render it!
			Color clr = Color::Lime();
			clr.SetAlpha(g_VisualsAlpha[player->GetIndex()]);
			graphics->Text({ player_box.bottomRight.x - player_box.width / 2, player_box.bottomRight.y + ((Config::ESP.Weapon.m_Value) ? graphics->GetFontSize() : 0) }, CD3D(clr), centered_x, enc("%d$"), money);
		}

		// Has Bomb and Has Defuser (+ Is Defusing)
		// This depends on the team of the player. If it's T, I can just check for Has Bomb
		// Otherwise, if CT, I can check for Has Defuser and Is Defusing! Sounds reasonable

		switch (player->m_iTeamNum())
		{
			case TEAM_T:
			{
				if (Config::ESP.HasBomb.m_Value)
				{
					// Check if player has got bomb
					bool has_bomb = false;

					auto weapon_names = player->GetWeaponNames();
					for (auto name : weapon_names)
					{
						if (strstr(name.c_str(), enc("C4")))
							has_bomb = true;
					}

					if (has_bomb)
					{
						// Render warning!
						Color clr = Color::Red();
						clr.SetAlpha(g_VisualsAlpha[player->GetIndex()]);
						graphics->Text({ player_box.topLeft.x + player_box.width / 2, player_box.topLeft.y - graphics->GetFontSize() - ((Config::ESP.Name.m_Value) ? graphics->GetFontSize() : 0) }, CD3D(clr), centered_x, enc("HAS BOMB"));
					}
				}
				break;
			}
			case TEAM_CT:
			{
				// Check if player has got defuser and it's defusing
				bool has_defuser = player->m_bHasDefuser();
				bool is_defusing = player->m_bIsDefusing();

				if (has_defuser && Config::ESP.HasDefuser.m_Value)
				{
					// Render warning!
					Color clr = light_blue;
					clr.SetAlpha(g_VisualsAlpha[player->GetIndex()]);
					graphics->Text({ player_box.topLeft.x + player_box.width / 2, player_box.topLeft.y - graphics->GetFontSize() - ((Config::ESP.Name.m_Value) ? graphics->GetFontSize() : 0) }, CD3D(clr), centered_x, 
						is_defusing && Config::ESP.IsDefusing.m_Value ?
						enc("HAS DEFUSER | IS DEFUSING") :
						enc("HAS DEFUSER")
					);
				}
				if (is_defusing && !has_defuser && Config::ESP.IsDefusing.m_Value)
				{
					// Render warning!
					Color clr = light_blue;
					clr.SetAlpha(g_VisualsAlpha[player->GetIndex()]);
					graphics->Text({ player_box.topLeft.x + player_box.width / 2, player_box.topLeft.y - graphics->GetFontSize() - ((Config::ESP.Name.m_Value) ? graphics->GetFontSize() : 0) }, CD3D(clr), centered_x,
						enc("IS DEFUSING")
					);
				}
				break;
			}
		}

		// Perform Barrel ESP, which is a line that goes from enemy head forwards
		if (Config::ESP.Barrel)
		{
			// Setup TraceRay call
			Vector forward;
			trace_t trace;
			Ray_t ray;
			CTraceFilter filter;
			Maths::AngleVectors(player->GetAbsAngles(), &forward);
			filter.pSkip = player;
			Vector head = player->GetHitboxPosition(HITBOX_HEAD);
			Vector distance = head + (forward * 100);
			ray.Init(head, distance);

			// Call TraceRay to fill trace
			Source::EngineTrace->TraceRay(ray, MASK_SHOT, &filter, &trace);

			// Convert head pos to screen pos, endpos to screen pos
			Vector head_screen, end_screen;
			if (graphics->W2S(head, head_screen) && graphics->W2S(trace.endpos, end_screen))
			{
				// Use trace infos to draw barrel line
				Color clr = Color::Red();
				clr.SetAlpha(g_VisualsAlpha[player->GetIndex()]);
				graphics->Line(ImVec2(head_screen), ImVec2(end_screen), CD3D(clr));
			}
		}

		// Perform SnapLines, which is a line that goes from bottom, center, up, right or left of the screen to the player's head
		if (Config::ESP.SnapLines > 0)
		{
			Vector player_head = player->GetHitboxPosition(HITBOX_HEAD), player_head_screen;
			using Coords = std::pair<Vector, Vector>;
			if (graphics->W2S(player_head, player_head_screen))
			{
				GRAB_SCREEN_SIZE
				// Starting coords depends on the option the user has selected
				const std::array<std::pair<Vector, Vector>, 6> snaplines_coords
				{
					// None
					Coords(Vector(), Vector()),
					// Bottom
					Coords(Vector(screen_width / 2, screen_height), Vector(player_box.bottomRight.x - player_box.width / 2, player_box.bottomRight.y, 0)),
					// Center
					Coords(Vector(screen_width / 2, screen_height / 2), player_head_screen),
					// Up
					Coords(Vector(screen_width, 0), Vector(player_box.topLeft.x + player_box.width / 2, player_box.topLeft.y)),
					// Left
					Coords(Vector(0, screen_height / 2), player_head_screen),
					// Right
					Coords(Vector(screen_width, screen_height / 2), player_head_screen)
				};

				// Draw the line + outline
				Vector starting_coords = snaplines_coords[Config::ESP.SnapLines].first;
				Vector end_coords = snaplines_coords[Config::ESP.SnapLines].second;
				graphics->Line(ImVec2(starting_coords), ImVec2(end_coords), CD3D(player_box.color));
				if (Config::ESP.BoxOutline)
				{
					graphics->Line(ImVec2(starting_coords), ImVec2(end_coords), CD3D(Color(0, 0, 0, g_VisualsAlpha[player->GetIndex()] * 255)));
					graphics->Line(ImVec2(starting_coords), ImVec2(end_coords), CD3D(Color(0, 0, 0, g_VisualsAlpha[player->GetIndex()] * 255)));
				}
			}
		}

		// Draw skeleton for this player
		if (Config::ESP.Skeleton)
		{
			BoneMatrix boneToWorld{};
			auto renderable = player->GetRenderable();
			if (renderable->SetupBones(boneToWorld.data(), MAXSTUDIOBONES, BONE_USED_BY_HITBOX, Source::Globals->cur_time))
			{
				// Determine color
				Color skeleton_color = player->m_iTeamNum() != local->m_iTeamNum() ?
					Config::Colors.SkeletonEnemy.m_Value : Config::Colors.SkeletonTeam.m_Value;
				skeleton_color.SetAlpha(g_VisualsAlpha[player->GetIndex()]);
				this->DrawSkeltal(graphics, local, player, renderable, boneToWorld, skeleton_color);
			}
		}

		// Hitbox ESP is done inside PaintTraverse

		// Head point for this player
		if (Config::ESP.HeadPoint)
		{
			// Get the head and screen head pos
			Vector head_position = player->GetHitboxPosition(HITBOX_HEAD), head_pos_screen;
			if (graphics->W2S(head_position, head_pos_screen)) // W2S it and draw it, ez af
				graphics->CircleFilled(ImVec2(head_pos_screen), headpoint_radius, CD3D(player_box.color), 32);
		}

		// Resolver ESP
		if (Config::Resolver.ResolverESP)
		{
			// Get the resolver mode for this player
			if (Config::Resolver.Active)
			{
				Vector pos = Vector(barArmor.topLeft.x, barArmor.topLeft.y, 0) + barArmor.width + 2;
				Color clr = Color::White();
				clr.SetAlpha(g_VisualsAlpha[ClientIndex]);
				graphics->Text({ player_box.bottomRight.x - player_box.width / 2, player_box.bottomRight.y + (Config::ESP.Money.m_Value ? (graphics->GetFontSize() * 2) : graphics->GetFontSize()) }, CD3D(clr), centered_x, enc("%s (%d)"), 
					PlayerList[ClientIndex].ResolveInfo.ResolveStage.c_str(), PlayerList[ClientIndex].MissedShots);
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	// WORLD ESP HERE
	//////////////////////////////////////////////////////////////////////////////////////////

	// For each world entity (any entity from slot 65 to slot n), check if entity is valid, apply filters, peform ESP
	FOR_EACH_WORLD_ENTITY
	{
		C_BaseEntity* entity = rcast<C_BaseEntity*>(Source::EntityList->GetClientEntity(EntityIndex));
		
		// Check if entity is valid
		if (entity == nullptr)
			continue;

		// Check if entity is dormant
		if (entity->GetNetworkable()->IsDormant())
			continue;

		auto class_name = entity->GetNetworkable()->GetClientClass()->m_pNetworkName;
		
		// If this is the planted c4 and the c4 exploded continue
		if (!strcmp(class_name, enc("CPlantedC4")))
		{
			// Planted C4 bounding boxes ain't accurate. Fix pliz
			C_PlantedC4* c4 = rcast<C_PlantedC4*>(entity);
			if (!c4->m_bBombTicking())
				continue;
		}

		// If filters isn't set to "All", it means we need to filter
		if (!Config::VisualsFilters.All.m_Value)
		{

			// If filter Guns isn't enabled we need to skip guns
			// Everything that contains 'Weapon' in class name is potentially a weapon, skip it
			if (!Config::VisualsFilters.Guns.m_Value)
			{
				if ((strstr(class_name, enc("Weapon")) || strstr(class_name, enc("AK47"))) &&
					!strstr(class_name, enc("C4")))
					continue;
			}

			// If filter Grenades isn't enabled we need to skip grenades
			if (!Config::VisualsFilters.Grenades.m_Value)
			{
				// Everything that contains 'Grenade' or 'Flashbang' in class name is a grenade, skip it
				if (strstr(class_name, enc("Grenade")) || strstr(class_name, enc("Flashbang")) || strstr(class_name, enc("Projectile")))
					continue;
			}

			// If filter Bomb isn't enabled we need to skip the bomb
			if (!Config::VisualsFilters.Bomb.m_Value)
			{
				// We need to skip both dropped bomb and planted bomb which becomes CPlantedC4
				if (strstr(class_name, enc("PlantedC4")) || strstr(class_name, enc("C4")))
					continue;
			}

			// If filter Hostages isn't enabled we need to skip hostages
			if (!Config::VisualsFilters.Hostages.m_Value)
			{
				if (strstr(class_name, enc("Hostage")))
					continue;
			}
		}

		// We are done with filters, we have an entity that respects all of them
		// Problem is that there may be some other props or entities that I can still ESP by mistake so I double check
		if (strstr(class_name, enc("World")) ||
			strstr(class_name, enc("BaseWeaponWorldModel")) ||
			strstr(class_name, enc("Cubemap")) ||
			strstr(class_name, enc("Cycler")) ||
			strstr(class_name, enc("Team")))
			//strstr(class_name, enc("TEClientProjectile")))
			continue;

		if (!strstr(class_name, enc("Weapon")) &&
			!strstr(class_name, enc("Flashbang")) &&
			!strstr(class_name, enc("Grenade")) &&
			!strstr(class_name, enc("Projectile")) &&
			strcmp(class_name, enc("CC4")) &&
			strcmp(class_name, enc("CPlantedC4")) &&
			!strstr(class_name, enc("Hostage")) &&
			!strstr(class_name, enc("AK47")))
			continue;

		// Only continue if there is no owner entity

		if (entity->GetOwnerEntity() != nullptr &&
			!strstr(class_name, enc("Projectile")) &&
			strcmp(class_name, enc("CPlantedC4")))
			continue;

		Box_t entity_box;
		if (!GetBoxForEntity(graphics, entity, entity_box))
			continue;
		entity_box.color = Color::White();

		// Render the box based on BoxType
		if (Config::ESP.BoxEnabled)
		{
			switch (Config::ESP.BoxType.m_Value)
			{
			case 0:
			{	// Full
				entity_box.Render(graphics);
				entity_box.RenderFiller(graphics);
				break;
			}
			case 1:
			{	// Corner
				BoxCorner_t corner_box;
				corner_box = entity_box;
				corner_box.Render(graphics);
				corner_box.RenderFiller(graphics);
				break;
			}
			case 2:
			{	// 3D
				Box3D_t box_3d;
				box_3d.color = entity_box.color;
				box_3d.entity = rcast<C_CSPlayer*>(entity);
				box_3d.Render(graphics);
				box_3d.RenderFiller(graphics);
				break;
			}
			}
		}

		// If it's gun or grenade render display name!
		if (!strstr(class_name, enc("Projectile")))
		{
			if (strstr(class_name, enc("Weapon")) ||
				strstr(class_name, enc("Flashbang")) ||
				strstr(class_name, enc("Grenade")) ||
				!strcmp(class_name, enc("CC4")) ||
				strstr(class_name, enc("AK47")))
			{
				C_WeaponCSBase* weapon = rcast<C_WeaponCSBase*>(entity);
				graphics->Text({ entity_box.bottomRight.x - entity_box.width / 2, entity_box.bottomRight.y }, CD3D(Color::White()), centered_x, enc("%s"), weapon->GetDisplayName().c_str());
			}
		}
		else
		{
			// This is a projectile! But every projectile has got the same class name so I can't distinguish between them
			// I need to recover model name and use it to distinguish, seems reasonable, THANKS VALVE BASED AF
			auto grenade_model = entity->GetRenderable()->GetModel(); // Get the model
			auto model_name = Source::ModelInfo->GetModelName(grenade_model); // Get model name
			const char* fancy_name = ""; // Name I can use inside ESP
			// Model names I can check for:
			if (strstr(model_name, enc("flashbang"))) fancy_name = enc("Flashbang");
			if (strstr(model_name, enc("fraggrenade"))) fancy_name = enc("HE Grenade");
			if (strstr(model_name, enc("smokegrenade"))) fancy_name = enc("Smoke Grenade");
			if (strstr(model_name, enc("decoy"))) fancy_name = enc("Decoy Grenade");
			if (strstr(model_name, enc("molotov"))) fancy_name = enc("Molotov");
			if (strstr(model_name, enc("incendiary"))) fancy_name = enc("Incendiary Grenade");
			// Draw the fancy name
			graphics->Text({ entity_box.bottomRight.x - entity_box.width / 2, entity_box.bottomRight.y }, CD3D(Color::White()), centered_x, enc("%s"), fancy_name);
		}

		// If it's planted render bomb timer if bomb timer is enabled
		if (!strcmp(class_name, enc("CPlantedC4")))
		{
			C_PlantedC4* c4 = rcast<C_PlantedC4*>(entity);
			float bomb_time = c4->m_flC4Blow() - Source::Globals->cur_time;
			graphics->Text({ entity_box.bottomRight.x - entity_box.width / 2, entity_box.bottomRight.y }, CD3D(Color::Red()), centered_x,
				!Config::ESP.BombTimer.m_Value ?
				enc("Planted C4") : enc("Planted C4 - Time: %.2f"), bomb_time);
			if (Config::ESP.BombTimer)
			{
				graphics->Text({ 20, 20 }, CD3D(Color::White()), none,
					enc("Time Left: %.2f"), bomb_time);
			}
		}
	}
}

// Draw sounds vector
void F_Visuals::DrawSounds(Render::D3D::DrawManager* graphics)
{
	// If SoundESP is enabled, I'm in game and sounds is not empty, it's time to d-d-d-draw
	if (Config::ESP.SoundESP && Source::Engine->IsInGame() && !sounds.empty())
	{
		for (auto snd_it = sounds.begin(); snd_it != sounds.end(); ++snd_it)
		{
			if (snd_it != sounds.end())
			{
				auto* snd = &*snd_it;
				std::string snd_entry = snd->sound_entry;
				if (snd_entry.empty())
				{
					// If snd_entry is empty, I switch to sound_sample and parse from there
					if (snd->sound_sample.find(enc("footsteps")) != std::string::npos)
					{
						snd_entry = enc("*STEP*");
					}
				}
				else
				{
					// snd_entry is not empty, parse snd->sound_entry so the sound is more readable
					if (snd_entry.find(enc("Single")) != std::string::npos)
					{
						snd_entry = enc("*GUNSHOT*");
					}
				}

				Vector origin_2d;
				if (graphics->W2S(snd->origin, origin_2d) && snd->alpha > 0)
				{
					//Render::String(Render::Fonts::Tahoma_Outline12, origin_2d.x, origin_2d.y, Color(255, 255, 255, snd->alpha * 255), Render::eAlign::ALIGN_CENTER, enc("%s"), snd->sound_entry);
					graphics->Text(ImVec2(origin_2d), CD3D(Color(255, 255, 255, snd->alpha * 255)), Render::D3D::text_flags::centered_x, enc("%s"), snd_entry.c_str());
				}
				snd->alpha -= 0.0025;
			}
		}
		// Remove the last element if the last element alpha is <= 0
		if ((sounds.end() - 1)->alpha <= 0)
		{
			sounds.pop_back();
		}
	}
}

// Draw a cool skeleton
void F_Visuals::DrawSkeltal(Render::D3D::DrawManager* graphics, Source::C_CSPlayer* local, Source::C_CSPlayer* player, Source::IClientRenderable* renderable, BoneMatrix boneMatrix, Source::Color color, bool engine_drawing)
{
	if (local && player && rcast<offset_t>(renderable) != 0xF)
	{
		// Get model for this player
		auto player_model = renderable->GetModel();
		if (player_model != nullptr)
		{
			// Get studio model for this player
			auto studio_model = Source::ModelInfo->GetStudioModel(player_model);
			if (studio_model != nullptr)
			{
				// For each bone draw lines according to the parent bone
				for (int i = 0; i < studio_model->numbones; i++)
				{
					// Get the bone
					auto bone = studio_model->pBone(i);
					if (bone == nullptr) // If bone is null skip
						continue;
					if (!(bone->flags & BONE_USED_BY_HITBOX)) // If bone isn't used by hitbox skip
						continue;
					if (bone->parent == -1 || bone->parent < 0) // If bone is orphan skip
						continue;
					if (bone->parent > MAXSTUDIOBONES) // If parent bone is out of bounds skip
						continue;

					// Get bone pos on screen
					Vector bone_pos;
					// Get parent bone pos on screen
					Vector parent_bone_pos;

					if (!engine_drawing)
					{
						// Use D3D W2S
						if (graphics && !graphics->W2S(Vector(boneMatrix[i][0][3], boneMatrix[i][1][3], boneMatrix[i][2][3]), bone_pos))
							continue;
						if (graphics && !graphics->W2S(Vector(boneMatrix[bone->parent][0][3], boneMatrix[bone->parent][1][3], boneMatrix[bone->parent][2][3]), parent_bone_pos))
							continue;
					}
					else
					{
						// Use Engine W2S
						if (!Render::WorldToScreen(Vector(boneMatrix[i][0][3], boneMatrix[i][1][3], boneMatrix[i][2][3]), bone_pos))
							continue;
						if (!Render::WorldToScreen(Vector(boneMatrix[bone->parent][0][3], boneMatrix[bone->parent][1][3], boneMatrix[bone->parent][2][3]), parent_bone_pos))
							continue;
					}

					// Draw line from bone pos to parent bone pos
					if (!engine_drawing)
					{
						// This function is being called from EndScene, use graphics manager to draw!
						if (graphics)
						{
							graphics->Line(ImVec2(bone_pos), ImVec2(parent_bone_pos), CD3D(color));
							// Draw outline if required
							if (Config::ESP.BoxOutline)
							{
								graphics->Line(ImVec2(bone_pos.x - 1, bone_pos.y), ImVec2(parent_bone_pos.x - 1, parent_bone_pos.y), CD3D(Color(0, 0, 0, color.a())));
								graphics->Line(ImVec2(bone_pos.x + 1, bone_pos.y), ImVec2(parent_bone_pos.x + 1, parent_bone_pos.y), CD3D(Color(0, 0, 0, color.a())));
							}
						}
					}
					else
					{
						// This function is being called from PaintTraverse, use engine drawing!
						Render::Line(Vector2(bone_pos.x, bone_pos.y), Vector2(parent_bone_pos.x, parent_bone_pos.y), color);
						// Draw outline if required
						if (Config::ESP.BoxOutline)
						{
							Render::Line(Vector2(bone_pos.x - 1, bone_pos.y), Vector2(parent_bone_pos.x - 1, parent_bone_pos.y), Color(0, 0, 0, color.a()));
							Render::Line(Vector2(bone_pos.x + 1, bone_pos.y), Vector2(parent_bone_pos.x + 1, parent_bone_pos.y), Color(0, 0, 0, color.a()));
						}
					}
				}
			}
		}
	}
}

// I use EndScene to perform NEARLY everything related to ESP drawing!
void F_Visuals::OnEndScene(Render::D3D::DrawManager* graphics)
{
	using namespace Render::D3D;
	// graphics is the pointer to the D3D draw manager, it will handle all the drawings
	if (graphics == nullptr)
		return;

	// Continue only if visuals are active
	if (!Config::ESP.Active)
		return;

	if (Config::ESP.NadePrediction)
	{
		Tools::CGrenadePrediction::GetInstance()->PaintGrenadeTrajectory(graphics);
	}

	// Execute ESP behaviour using graphics ptr to draw
	this->ExecuteESP(graphics);
	//this->DrawSounds(graphics);
}

// I use CreateMove to perform RevealRanks and to tick nade prediction
void F_Visuals::OnCreateMovePostPrediction(Source::CUserCmd* cmd)
{
	if (Config::ESP.RevealRanks.m_Value)
	{
		// Check if scoreboard is opened
		if (cmd->buttons & IN_SCORE)
		{
			using RankRevealAll_t = bool(__cdecl*)(int*);
			static RankRevealAll_t RankRevealAll = rcast<RankRevealAll_t>(Offsets::RankRevealAll);
			// One day someone will discover what the fuck is this parameter supposed to mean. But this is not the day
			int empty[3] = { 0, 0, 0 }; // <- empty vector that you must pass to RankRevealAll to reveal all ranks like gucci
			RankRevealAll(empty);
		}
	}
	if (Config::ESP.Active && Config::ESP.NadePrediction)
	{
		Tools::CGrenadePrediction::GetInstance()->Tick(cmd->buttons);
	}
}

// Used to predict nades
void F_Visuals::OnOverrideView(Source::CViewSetup* setup)
{
	if (Config::ESP.Active && Config::ESP.NadePrediction)
	{
		Tools::CGrenadePrediction::GetInstance()->PredictGrenade();
	}
}

// Used for SoundESP
void F_Visuals::OnEmitSound(int ent_index, const char* sound_entry, const char* sound_sample, Source::Vector origin,
	Source::CUtlVector<Source::Vector>* origins, float sound_time)
{
	// TODO: FIX SOUND ESP PLIZ
	/*
	// I will dump only sounds coming from actual players
	// Maybe I will add an option to display sounds coming from props
	if (Config::ESP.SoundESP && Source::Engine->IsInGame()) 
	{
		if (ent_index > 0 && ent_index <= MAX_CLIENTS 
			&& ent_index != Source::Engine->GetLocalPlayer()
			&& origin != Vector(0, 0, 0))
		{
			// Push back a new sound to be drawn if I cannot find the same origin inside the vector
			bool already_there = false;
			for (auto snd : sounds)
			{
				if (snd.origin == origin)
				{
					already_there = true;
					break;
				}
			}
			if (!already_there)
				sounds.push_back({ ent_index, std::string(sound_entry), std::string(sound_sample), origin, sound_time, 1.f});
		}
	}
	*/
}

#pragma endregion

/////////////////////////////////  GLOW
#pragma region GLOW

// I use RenderView to perform glow since it's the best place to perform it, literally
void F_Glow::OnRenderView(const CViewSetup& view, const CViewSetup& hud_view, int clear_flags, int what_to_draw)
{
	if (!Config::ESP.Glow)
		return;

	// Get local player
	auto local = C_CSPlayer::GetLocal();
	if (local == nullptr)
		return;

	// For each glow object
	for (auto i = 0; i < m_pGlowObjectManager->m_GlowObjectDefinitions.Count(); i++)
	{
		// Get the glow object reference
		auto& glow_object = m_pGlowObjectManager->m_GlowObjectDefinitions[i];
		// Get entity related to this object
		auto entity = rcast<C_CSPlayer*>(glow_object.m_pEntity);

		// Skip if glow object or entity is invalid
		if (glow_object.IsUnused())
			continue;
		if (entity == nullptr || entity->GetNetworkable()->IsDormant())
			continue;
		if (!entity->IsPlayer())
			continue;

		if (!Visuals->ApplyPlayerFilters(entity, local))
			continue;

		// Set the color
		auto color = (local->m_iTeamNum() != entity->m_iTeamNum()) ? 
			Config::Colors.GlowEnemy.m_Value.toFloatColor() : Config::Colors.GlowTeam.m_Value.toFloatColor();
		float al = g_VisualsAlpha[entity->GetIndex()];
		al = clamp<float, float>(al, 0.f, Config::ESP.GlowAlpha);
		color.SetAlpha(al);

		// Set the glow object vars to modify it (remember that glow_object is a reference!)
		glow_object.m_flRed = color.r();
		glow_object.m_flGreen = color.g();
		glow_object.m_flBlue = color.b();
		glow_object.m_flAlpha = color.a();
		glow_object.m_bRenderWhenOccluded = true;
		glow_object.m_bRenderWhenUnoccluded = false;
	}
}

// Remove glow effect from every single entity
void F_Glow::OnShutdown()
{
	for (int i = 0; i < m_pGlowObjectManager->m_GlowObjectDefinitions.Count(); i++) {
		auto& glow_object = m_pGlowObjectManager->m_GlowObjectDefinitions[i];
		auto entity = rcast<C_CSPlayer*>(glow_object.m_pEntity);

		if (glow_object.IsUnused())
			continue;
		if (entity == nullptr || entity->GetNetworkable()->IsDormant())
			continue;
		if (!strstr(entity->GetNetworkable()->GetClientClass()->m_pNetworkName, enc("CSPlayer")))
			continue;

		glow_object.m_flAlpha = 0.0f;
	}
}

#pragma endregion

// Little helper function to force a material
auto ForceMaterial(Source::IMaterial* material, Source::FloatColor color = Source::FloatColor(1.f, 1.f, 1.f, 1.f), bool wireframe = false, bool remove = false) -> void
{
	if (material)
	{
		material->AlphaModulate(color.a());
		material->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, wireframe);
		material->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, remove);
		material->ColorModulate(color.r(), color.g(), color.b());
		Source::ModelRender->ForcedMaterialOverride(material);
	}
}

/////////////////////////////////  REMOVALS
#pragma region REMOVALS

void F_Removals::NoPostProcessing()
{
	// If I'm not ingame or connected, it's useless to disable postprocessing
	if (!Source::Engine->IsInGame() || !Source::Engine->IsConnected())
		return;

	// Find the var and remove flags so I can modify it freely
	static ConVar* mat_postprocess_enable = Source::CVar->FindVar(enc("mat_postprocess_enable"));
	*rcast<int*>(rcast<DWORD>(&mat_postprocess_enable->m_fnChangeCallback) + 0xC) = NULL;

	if (!Config::Removals.NoPostProcessing)
	{
		mat_postprocess_enable->SetValue(1);
		return;
	}
	mat_postprocess_enable->SetValue(0);
}

void F_Removals::NoScopeLines(Render::D3D::DrawManager* graphics)
{
	// Draw our fantastic lines when no scope is enabled and player is scoping
	auto local = C_CSPlayer::GetLocal();
	if (local && Config::Removals.NoScope && local->m_bIsScoped())
	{
		GRAB_SCREEN_SIZE
		graphics->Line(ImVec2(screen_width / 2, 0), ImVec2(screen_width / 2, screen_height), CD3D(Color::Black()));
		graphics->Line(ImVec2(0, screen_height / 2), ImVec2(screen_width, screen_height / 2), CD3D(Color::Black()));
	}
}

void F_Removals::NoVisualRecoil(Source::ClientFrameStage_t stage)
{
	auto local = C_CSPlayer::GetLocal();
	if (Config::Removals.NoVisualRecoil && 
		Source::Engine->IsInGame() && 
		stage == FRAME_RENDER_START && 
		local && local->IsAlive())
	{
		// Get pointers to local player punch angles
		this->m_pAimPunch = local->m_aimPunchAnglePtr();
		this->m_pViewPunch = local->m_viewPunchAnglePtr();

		// Store their current values
		if (this->m_pAimPunch && this->m_pViewPunch)
		{
			this->m_vecAimPunch = *this->m_pAimPunch;
			this->m_vecViewPunch = *this->m_pViewPunch;

			// Replace them with zero
			*this->m_pAimPunch = Vector(0, 0, 0);
			*this->m_pViewPunch = Vector(0, 0, 0);
		}
	}
}

void F_Removals::RestoreRecoilAngles()
{
	// Restore them angles after call to original FSN
	if (Config::Removals.NoVisualRecoil)
	{
		if (this->m_pAimPunch && this->m_pViewPunch)
		{
			*this->m_pAimPunch = this->m_vecAimPunch;
			*this->m_pViewPunch = this->m_vecViewPunch;
		}
	}
}

void F_Removals::NoFlash()
{
	if (Config::Removals.NoFlash)
	{
		// Get materials for the flashbang
		IMaterial* mat_flash = Source::MaterialSystem->FindMaterial(enc("effects/flashbang"), TEXTURE_GROUP_CLIENT_EFFECTS);
		IMaterial* mat_flash_white = Source::MaterialSystem->FindMaterial(enc("effects/flashbang_white"), TEXTURE_GROUP_CLIENT_EFFECTS);

		if (mat_flash && mat_flash_white)
		{
			// Toggle one the material var to remove drawing to remove flash effect
			mat_flash->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			mat_flash_white->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
		}

		// Force material override
		Source::ModelRender->ForcedMaterialOverride(mat_flash);
		Source::ModelRender->ForcedMaterialOverride(mat_flash_white);
	}
}

void F_Removals::NoHands(const std::string& model_name)
{
	if (Config::Removals.NoHands)
	{
		// Get materials for the hands
		IMaterial* mat_hands = Source::MaterialSystem->FindMaterial(model_name.c_str());
		if (mat_hands)
		{
			// Force no drawing
			mat_hands->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
		}
		// Force material override
		Source::ModelRender->ForcedMaterialOverride(mat_hands);
	}
}

void F_Removals::NoWeapons(const std::string& model_name)
{
	if (Config::Removals.NoWeapons)
	{
		// Get materials for the weapon
		IMaterial* mat_weapon = Source::MaterialSystem->FindMaterial(model_name.c_str());
		if (mat_weapon)
		{
			// Force no drawing
			mat_weapon->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
		}
		// Force material override
		Source::ModelRender->ForcedMaterialOverride(mat_weapon);
	}
}

void F_Removals::NoSmoke(Source::ClientFrameStage_t stage)
{
	if (Config::Removals.NoSmoke && stage == FRAME_RENDER_START)
	{
		// Set SmokeCount to 0 to effectively remove smokes.
		// The rest is done inside m_bDidSmokeEffect netvar proxy
		*(int*)(Offsets::SmokeCount) = 0;
	}
}

// Becomes true when skybox materials gathering is done, also becomes false when I switch maps
bool skybox_storage_performed = false;
// Becomes true when skybox materials are made black by NoSky
bool skybox_removal_performed = false;
std::unordered_map<IMaterial*, FloatColor> skybox_textures;

void F_Removals::NoSky()
{
	if (Config::Removals.NoSky)
	{
		// I need to store pointers to skybox materials once
		if (!skybox_storage_performed)
		{
			// For each material
			for (MaterialHandle_t i = Source::MaterialSystem->FirstMaterial();
				i != Source::MaterialSystem->InvalidMaterial();
				i = Source::MaterialSystem->NextMaterial(i))
			{
				// Grab current material
				IMaterial* mat = Source::MaterialSystem->GetMaterial(i);
				// If it's valid and the texture group is skybox textures
				if (mat && !strcmp(mat->GetTextureGroupName(), TEXTURE_GROUP_SKYBOX))
				{
					// Store this material
					float r, g, b, a;
					mat->GetColorModulation(&r, &g, &b);
					a = mat->GetAlphaModulation();
					skybox_textures[mat] = FloatColor(r, g, b, a);
				}
			}
			skybox_storage_performed = true;
		}

		if (!skybox_removal_performed && !skybox_textures.empty())
		{
			// For each material inside skybox_textures
			for (auto m : skybox_textures)
			{
				IMaterial* mat = m.first;
				mat->ColorModulate(0.f, 0.f, 0.f);
			}
			skybox_removal_performed = true;
		}
	}
	else
	{
		// If no sky isn't enabled and the skybox_removal_performed is true, it means I need to reset sky to original colors
		if (skybox_removal_performed)
		{
			// Reset skybox materials
			// For each material
			for (MaterialHandle_t i = Source::MaterialSystem->FirstMaterial();
				i != Source::MaterialSystem->InvalidMaterial();
				i = Source::MaterialSystem->NextMaterial(i))
			{
				// Grab current material
				IMaterial* mat = Source::MaterialSystem->GetMaterial(i);
				// If it's valid and the texture group is skybox textures
				if (mat && !strcmp(mat->GetTextureGroupName(), TEXTURE_GROUP_SKYBOX))
				{
					// Reset material colors
					FloatColor original_color = skybox_textures[mat];
					mat->ColorModulate(original_color.r(), original_color.g(), original_color.b());
					mat->AlphaModulate(original_color.a());
				}
			}
			skybox_removal_performed = false;
		}
	}
}

void F_Removals::OnDrawModelExecute(void* context, void* state, const Source::ModelRenderInfo_t& render_info, matrix3x4_t* custom_bone_to_world)
{
	// Get model name
	const std::string model_name = Source::ModelInfo->GetModelName(render_info.pModel);

	// Do different things based on the model name

	// No Flash
	if (model_name.find(enc("flash")) != std::string::npos)
	{
		this->NoFlash();
	}

	// No Hands
	if (model_name.find(enc("arms")) != std::string::npos)
	{
		this->NoHands(model_name);
	}

	// No Weapons
	else if (model_name.find(enc("weapon")) != std::string::npos)
	{
		this->NoWeapons(model_name);
	}

	this->NoSky();
}

void F_Removals::OnFrameStageNotify(Source::ClientFrameStage_t stage)
{
	this->NoVisualRecoil(stage);
	this->NoSmoke(stage);
}

void F_Removals::OnEndScene(Render::D3D::DrawManager* graphics)
{
	if (graphics)
	{
		this->NoScopeLines(graphics);
	}
}

void F_Removals::OnShutdown()
{
	// Reset skybox materials only if removal has been performed
	if (skybox_removal_performed)
	{
		// For each material
		for (MaterialHandle_t i = Source::MaterialSystem->FirstMaterial();
			i != Source::MaterialSystem->InvalidMaterial();
			i = Source::MaterialSystem->NextMaterial(i))
		{
			// Grab current material
			IMaterial* mat = Source::MaterialSystem->GetMaterial(i);
			// If it's valid and the texture group is skybox textures
			if (mat && !strcmp(mat->GetTextureGroupName(), TEXTURE_GROUP_SKYBOX))
			{
				// Reset material colors
				FloatColor original_color = skybox_textures[mat];
				mat->ColorModulate(original_color.r(), original_color.g(), original_color.b());
				mat->AlphaModulate(original_color.a());
			}
		}
	}
}

void F_Removals::OnListenedEvent(Source::IGameEvent* game_event)
{
	if (EVENT_IS(enc("player_connect_full")))
	{
		skybox_storage_performed = false;
		skybox_removal_performed = false;
	}
}

#pragma endregion

/////////////////////////////////  CHAMS
#pragma region CHAMS

static std::vector<std::string> Materials
{
	// None
	enc("INVALID"),
	// Textured
	enc("vv_mat_textured"), // <- Insert here path to textured material created internally
	// Flat
	enc("vv_mat_flat"), // <- Insert here path to flat material created internally
	// Wireframe
	enc("WIREFRAMETHIS"), // <- There is no material path for wireframe, since it's achieved by setting wireframe material var
	// Full Model
	enc("FULLMDL"), // <- There is no material for full model, since I modify directly model materials
	// Glass
	enc("models/inventory_items/cologne_prediction/cologne_prediction_glass"),
	// Platinum
	enc("models/player/ct_fbi/ct_fbi_glass"),
	// Dark Chrome
	enc("models/gibs/glass/glass"),
	// Crystal
	enc("models/inventory_items/trophy_majors/crystal_clear"),
	// Blue Crystal
	enc("models/inventory_items/trophy_majors/crystal_blue"),
	// Gold
	enc("models/inventory_items/trophy_majors/gold"),
	// Glow
	enc("vgui/achievements/glow"),
	// Plastic
	enc("models/inventory_items/trophy_majors/gloss"),
	// Strings
	enc("models/inventory_items/music_kit/darude_01/mp3_detail")
};
static const std::string MATERIAL_INVALID = enc("INVALID");
static const std::string MATERIAL_WIREFRAME = enc("WIREFRAMETHIS");
static const std::string MATERIAL_FULLMODEL = enc("FULLMDL");

// Material creation infos
struct MaterialInfo_t
{
	std::string materialType = enc("VertexLitGeneric");
	std::string baseTexture = enc("VGUI/white_additive");
	std::string surfaceProp = enc("Metal");
	std::string envMap = enc("env_cubemap");
	bool phongEnabled = true;
	std::array<float, 3> phongFresnelRanges = { 0.5f, 0.75f, 0.1f };
	int phongExponent = 25;
	bool ignorez = false;
	bool noFog = true;
	bool model = true;
	bool noCull = false;
	bool halfLambert = true;
	bool rimLight = true;
	int rimLightExponent = 1;
	int rimLightBoost = 1;
	bool selfIllum = true;
	float selfIllum_EnvMapMask_Alpha = 1.f;
	FloatColor selfIllumTint = FloatColor(0.61f, 0.61f, 0.61f);
	FloatColor color = FloatColor(1.f, 1.f, 1.f);
};

// lil functions
inline std::string getStringFromArray(std::array<float, 3> arry)
{
	char buffer[64];
	sprintf(buffer, "[%.2f %.2f %.2f]", arry[0], arry[1], arry[2]);
	return std::string(buffer);
}
inline std::string getStringFromColor(FloatColor colly)
{
	char buffer[64];
	sprintf(buffer, "[%.2f %.2f %.2f]", colly.r(), colly.g(), colly.b());
	return std::string(buffer);
}

bool F_Chams::CreateMaterial(const std::string& material_name, MaterialInfo_t info)
{
	// This will attempt to create a new material
	std::string material_path = GetGamePath().append(enc("csgo\\materials\\")).append(material_name).append(enc(".vmt"));
	if (FileExists(material_path))
	{
		Warn(enc("Tried to create material %s, but it's already there!"), material_name.c_str());
		return false;
	}
	std::ofstream output(material_path);
	if (output.good())
	{
		std::stringstream ss;
		std::string start = enc("{"), end = enc("}"), tab = enc("\t"), space = enc(" ");
		/*
		info.materialType (default: "VertexLitGeneric")
		{
			"$basetexture" "<info.baseTexture>" (default: "VGUI/white_additive")
			"$surfaceprop" "<info.surfaceProp>" (default: "Metal")
			"$envmap" "<info.envMap>" (default: "env_cubemap")
			"$phong" "<info.phongEnabled>" (default: 1)
			"$phongfresnelranges" "[<info.phongFresnelRanges>]" (default: "[0.5 0.75 1]")
			"$phongexponent" "<info.phongExponent>" (default: 25)
			"$ignorez" "<info.ignorez>" (default: 0)
			"$nofog" "<info.noFog>" (default: 1)
			"$model" "<info.model>" (default: 1)
			"$nocull" "<info.noCull>" (default: 0)
			"$halflambert" "<info.halfLambert>" (default: 1)
			"$rimlight" "<info.rimLight>" (default: 1)
			"$rimlightexponent" "<info.rimLightExponent>" (default: 1)
			"$rimlightboost" "<info.rimLightBoost>" (default: 1)
			"$selfillum" "<info.selfIllum>" (default: 1)
			"$selfillum_envmapmask_alpha" "<info.selfIllum_EnvMapMask_Alpha>" (default: 1)
			"$selfillumtint" "[<info.selfIllumTint>]" (default: "[0.61 0.61 0.61]")
			"$color" "[<info.color>]" (default: "[1 1 1]")
		}
		*/
		ss << info.materialType << std::endl
		<< start << std::endl
		<< tab << enc("$basetexture") << space << enc("\"") << info.baseTexture << enc("\"") << std::endl
		<< tab << enc("$surfaceprop") << space << enc("\"") << info.surfaceProp << enc("\"") << std::endl
		<< tab << enc("$envmap") << space << enc("\"") << info.envMap << enc("\"") << std::endl
		<< tab << enc("$phong") << space << enc("\"") << info.phongEnabled << enc("\"") << std::endl
		<< tab << enc("$phongfresnelranges") << space << enc("\"") << getStringFromArray(info.phongFresnelRanges) << enc("\"") << std::endl
		<< tab << enc("$phongexponent") << space << enc("\"") << info.phongExponent << enc("\"") << std::endl
		<< tab << enc("$ignorez") << space << enc("\"") << info.ignorez << enc("\"") << std::endl
		<< tab << enc("$nofog") << space << enc("\"") << info.noFog << enc("\"") << std::endl
		<< tab << enc("$model") << space << enc("\"") << info.model << enc("\"") << std::endl
		<< tab << enc("$nocull") << space << enc("\"") << info.noCull << enc("\"") << std::endl
		<< tab << enc("$halflambert") << space << enc("\"") << info.halfLambert << enc("\"") << std::endl
		<< tab << enc("$rimlight") << space << enc("\"") << info.rimLight << enc("\"") << std::endl
		<< tab << enc("$rimlightexponent") << space << enc("\"") << info.rimLightExponent << enc("\"") << std::endl
		<< tab << enc("$rimlightboost") << space << enc("\"") << info.rimLightBoost << enc("\"") << std::endl
		<< tab << enc("$selfillum") << space << enc("\"") << info.selfIllum << enc("\"") << std::endl
		<< tab << enc("$selfillum_envmapmask_alpha") << space << enc("\"") << std::setprecision(2) << info.selfIllum_EnvMapMask_Alpha << enc("\"") << std::endl
		<< tab << enc("$selfillumtint") << space << enc("\"") << getStringFromColor(info.selfIllumTint) << enc("\"") << std::endl
		<< tab << enc("$color") << space << enc("\"") << getStringFromColor(info.color) << enc("\"") << std::endl
		<< end;
		output << ss.str();
		output.close();
	}
	else
	{
		Warn(enc("Tried to create material %s, but an error occured!"), material_name.c_str());
		return false;
	}
	Warn(enc("Created material at %s"), material_path.c_str());
	return true;
}

void F_Chams::CreateMaterials()
{
	// I need to create several materials
	// Textured normal - Textured ignorez (XQZ)
	// Flat normal - Flat ignorez (XQZ)
	MaterialInfo_t base_material;

	// Textured
	this->CreateMaterial(enc("vv_mat_textured"), base_material);
	this->m_pMaterialTextured = Source::MaterialSystem->FindMaterial(enc("vv_mat_textured"));
	this->m_pMaterialTextured->IncrementReferenceCount();

	// Textured ignorez (XQZ)
	base_material.ignorez = true;
	this->CreateMaterial(enc("vv_mat_textured_xqz"), base_material);
	this->m_pMaterialTexturedXQZ = Source::MaterialSystem->FindMaterial(enc("vv_mat_textured_xqz"));
	this->m_pMaterialTexturedXQZ->IncrementReferenceCount();
	base_material.ignorez = false;

	// Flat
	base_material.materialType = enc("UnlitGeneric");
	this->CreateMaterial(enc("vv_mat_flat"), base_material);
	this->m_pMaterialFlat = Source::MaterialSystem->FindMaterial(enc("vv_mat_flat"));
	this->m_pMaterialFlat->IncrementReferenceCount();

	// Flat ignorez (XQZ)
	base_material.ignorez = true;
	this->CreateMaterial(enc("vv_mat_flat_xqz"), base_material);
	this->m_pMaterialFlatXQZ = Source::MaterialSystem->FindMaterial(enc("vv_mat_flat_xqz"));
	this->m_pMaterialFlatXQZ->IncrementReferenceCount();
	base_material.ignorez = false;

	// Fake Angles
	base_material.phongEnabled = false;
	base_material.rimLight = false;
	base_material.selfIllum = false;
	this->CreateMaterial(enc("vv_mat_angs"), base_material);
	this->m_pMaterialFakeAngles = Source::MaterialSystem->FindMaterial(enc("vv_mat_angs"));
	this->m_pMaterialFakeAngles->IncrementReferenceCount();
	base_material.phongEnabled = true;
	base_material.rimLight = true;
	base_material.selfIllum = true;
}

void F_Chams::PlayerChams(C_CSPlayer* player)
{
	auto lResetIgnorez = [](const std::string& mat_path, IMaterial* textured, IMaterial* flat) -> void
	{
		// Reset ignorez flag
		IMaterial* my_material_xqz = nullptr;
		if (strstr(mat_path.c_str(), enc("vv_mat")))
		{
			my_material_xqz = (strstr(mat_path.c_str(), enc("textured"))) ? textured : flat;
			if (my_material_xqz) my_material_xqz->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
		}
		else
		{
			my_material_xqz = Source::MaterialSystem->FindMaterial(mat_path.c_str());
			if (my_material_xqz) my_material_xqz->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
		}
	};
	auto lIsWireframe = [](const std::string& mat_path) { return mat_path == MATERIAL_WIREFRAME; };
	auto lIsFullModel = [](const std::string& mat_path) { return mat_path == MATERIAL_FULLMODEL; };
	auto lResetFullModel = [](studiohdr_t* hdr, const model_t* model) -> bool
	{
		// Reset full model requested, I need to grab every single material from the player model and reset ignorez
		if (hdr) // studioHdr->numtextures could be potentially replaced by Source::ModelInfo->GetModelMaterialCount but I need to test
		{
			IMaterial* modelMats[MAXSTUDIOSKINS];
			Source::ModelInfo->GetModelMaterials(model, hdr->numtextures, modelMats);
			for (int i = 0; i < hdr->numtextures; i++)
			{
				auto material = modelMats[i];
				if (material)
				{
					material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
					material->AlphaModulate(1.f);
				}
			}
		}
		return false;
	};

	auto local = C_CSPlayer::GetLocal();
	if (local == nullptr)
		return;

	if (player == nullptr)
		return;
	//if (player == local)
	//	return;
	if (player->m_lifeState() != LIFE_ALIVE)
		return;
	if (Config::Chams.EnemyOnly)
		if (player->m_iTeamNum() == local->m_iTeamNum())
			return;
	if (player->GetNetworkable()->IsDormant())
		return;
	if (player->m_bGunGameImmunity())
		return;

	FloatColor color_visible = (local->m_iTeamNum() != player->m_iTeamNum()) ? Config::Colors.ChamsEnemy.m_Value.toFloatColor() :
		Config::Colors.ChamsTeam.m_Value.toFloatColor();
	FloatColor color_hidden = (local->m_iTeamNum() != player->m_iTeamNum()) ? Config::Colors.ChamsEnemy_XQZ.m_Value.toFloatColor() :
		Config::Colors.ChamsTeam_XQZ.m_Value.toFloatColor();

	const std::string mat_path_visible = Materials[Config::Chams.VisibleType];
	const std::string mat_path_hidden = Materials[Config::Chams.HiddenType];
	auto model = this->m_RenderInfo->pModel;
	auto studioHdr = Source::ModelInfo->GetStudioModel(model);
	static bool using_fullmdl = false;

	// If mat path contains vv_mat it's my custom material!
	if (Config::Chams.Active && Config::Chams.XQZ && mat_path_hidden != MATERIAL_INVALID)
	{
		IMaterial* my_material_xqz = nullptr;
		// Determine which material I should use for xqz chams
		if (strstr(mat_path_hidden.c_str(), enc("vv_mat"))) 
		{
			if (using_fullmdl) using_fullmdl = lResetFullModel(studioHdr, model);
			my_material_xqz = strstr(mat_path_hidden.c_str(), enc("textured")) ? this->m_pMaterialTexturedXQZ : this->m_pMaterialFlatXQZ;
			if (my_material_xqz) my_material_xqz->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
		}
		else if (!lIsFullModel(mat_path_hidden))
		{
			if (using_fullmdl) using_fullmdl = lResetFullModel(studioHdr, model);
			my_material_xqz = Source::MaterialSystem->FindMaterial(mat_path_hidden.c_str());
			if (my_material_xqz) my_material_xqz->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
		}
		else if (player != local)
		{
			// Full Model detected, I need to grab every single material from the player model and apply ignorez
			if (studioHdr)
			{
				IMaterial* modelMats[MAXSTUDIOSKINS];
				Source::ModelInfo->GetModelMaterials(model, studioHdr->numtextures, modelMats);
				for (int i = 0; i < studioHdr->numtextures; i++)
				{
					auto material = modelMats[i];
					if (material) 
					{
						material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
						material->AlphaModulate(1.f);
					}
				}
			}
			using_fullmdl = true;
		}

		if (my_material_xqz != nullptr) ForceMaterial(my_material_xqz, color_hidden, lIsWireframe(mat_path_hidden));
		Source::ModelRender->DrawModelExecute(this->m_pContext, this->m_pState, *this->m_RenderInfo, this->m_pCustomBoneToWorld);
	}
	else
	{
		lResetIgnorez(mat_path_hidden, this->m_pMaterialTexturedXQZ, this->m_pMaterialFlatXQZ);
		if (using_fullmdl) using_fullmdl = lResetFullModel(studioHdr, model);
	}

	if (Config::Chams.Active && mat_path_visible != MATERIAL_INVALID)
	{
		IMaterial* my_material = nullptr;
		// Determine which material I should use for visible chams
		if (strstr(mat_path_visible.c_str(), enc("vv_mat")))
			my_material = strstr(mat_path_visible.c_str(), enc("textured")) ? this->m_pMaterialTextured : this->m_pMaterialFlat;
		else
			my_material = Source::MaterialSystem->FindMaterial(mat_path_visible.c_str());

		if (my_material != nullptr) ForceMaterial(my_material, color_visible, lIsWireframe(mat_path_visible));
		Source::ModelRender->DrawModelExecute(this->m_pContext, this->m_pState, *this->m_RenderInfo, this->m_pCustomBoneToWorld);
	}
	else lResetIgnorez(mat_path_hidden, this->m_pMaterialTextured, this->m_pMaterialFlat);
}

void F_Chams::HandChams()
{
	FloatColor color_visible = Config::Colors.HandChams.m_Value.toFloatColor();

	const std::string mat_path = Materials[Config::Chams.HandChams];

	if (mat_path != MATERIAL_INVALID)
	{
		IMaterial* my_material = nullptr;
		// Determine which material I should use for hand chams
		if (strstr(mat_path.c_str(), enc("vv_mat")))
			my_material = (strstr(mat_path.c_str(), enc("textured"))) ? this->m_pMaterialTextured : this->m_pMaterialFlat;
		else
			my_material = Source::MaterialSystem->FindMaterial(mat_path.c_str());

		if (my_material != nullptr)
		{
			ForceMaterial(my_material, color_visible, !strcmp(mat_path.c_str(), enc("WIREFRAMETHIS")), Config::Removals.NoHands);
			Source::ModelRender->DrawModelExecute(this->m_pContext, this->m_pState, *this->m_RenderInfo, this->m_pCustomBoneToWorld);
		}
	}
}

void F_Chams::WeaponChams()
{
	FloatColor color_visible = Config::Colors.WeaponChams.m_Value.toFloatColor();

	const std::string mat_path = Materials[Config::Chams.WeaponChams];

	if (mat_path != MATERIAL_INVALID)
	{
		IMaterial* my_material = nullptr;
		// Determine which material I should use for weapon chams
		if (strstr(mat_path.c_str(), enc("vv_mat")))
			my_material = (strstr(mat_path.c_str(), enc("textured"))) ? this->m_pMaterialTextured : this->m_pMaterialFlat;
		else
			my_material = Source::MaterialSystem->FindMaterial(mat_path.c_str());

		if (my_material != nullptr)
		{
			ForceMaterial(my_material, color_visible, !strcmp(mat_path.c_str(), enc("WIREFRAMETHIS")), Config::Removals.NoWeapons);
			Source::ModelRender->DrawModelExecute(this->m_pContext, this->m_pState, *this->m_RenderInfo, this->m_pCustomBoneToWorld);
		}
	}
}

void F_Chams::ThirdPersonTransparency()
{
	// For each material inside player's model set alpha to 0.5 and redraw
	auto studioHdr = Source::ModelInfo->GetStudioModel(this->m_RenderInfo->pModel);
	if (studioHdr)
	{
		IMaterial* modelMats[MAXSTUDIOSKINS];
		Source::ModelInfo->GetModelMaterials(this->m_RenderInfo->pModel, studioHdr->numtextures, modelMats);
		for (int i = 0; i < studioHdr->numtextures; i++)
		{
			IMaterial* material = modelMats[i];
			if (material)
			{
				material->AlphaModulate(0.5f);
			}
		}
		Source::ModelRender->DrawModelExecute(this->m_pContext, this->m_pState, *this->m_RenderInfo, this->m_pCustomBoneToWorld);
	}
}

void F_Chams::OnDrawModelExecute(void* context, void* state, const Source::ModelRenderInfo_t& render_info, matrix3x4_t* custom_bone_to_world)
{
	this->m_pContext = context;
	this->m_pState = state;
	this->m_RenderInfo = &const_cast<Source::ModelRenderInfo_t&>(render_info);
	this->m_pCustomBoneToWorld = custom_bone_to_world;

	if (!Config::Chams.Active)
		return;

	// Get model name
	const std::string model_name = Source::ModelInfo->GetModelName(render_info.pModel);

	// Do different things based on the model name

	// Player Chams
	if (model_name.find(enc("models/player")) != std::string::npos)
	{
		this->PlayerChams(GET_ENTITY(render_info.entity_index));
		//if (this->m_RenderInfo->entity_index == Source::Engine->GetLocalPlayer() 
		//	&& Config::VisualsOther.ThirdPerson || Source::Input->m_fCameraInThirdPerson
		//	&& rcast<C_CSPlayer*>(GET_ENTITY(render_info.entity_index))->m_bIsScoped())
		//{
		//	// While in third person and scoped in, set local player alpha to 0.5 and redraw
		//	this->ThirdPersonTransparency();
		//}
	}

	// Hand Chams
	if (model_name.find(enc("arms")) != std::string::npos)
	{
		this->HandChams();
	}

	// Weapon Chams
	else if (model_name.find(enc("weapon")) != std::string::npos)
	{
		this->WeaponChams();
	}
}

#pragma endregion

/////////////////////////////////  RADAR
#pragma region RADAR

const int RADAR_SIZE = 250;

void F_Radar::CollectPoints()
{
	radar_points.clear();
	auto local = C_CSPlayer::GetLocal();
	if (local)
	{
		FOR_EACH_CLIENT
		{
			auto* player = GET_ENTITY(ClientIndex);
			if (player && player->IsAlive())
			{
				RadarPoint point;
				point.SetPos(EntityToRadar(player));
				if (player == local)
					point.SetColor(Color::Blue());
				else
					point.SetColor(player->m_iTeamNum() == local->m_iTeamNum() ? Color::Green() : Color::Red());
				radar_points.push_back(point);
			}
		}
	}
}

void F_Radar::HandleMove()
{
	if (mouse_down) 
	{
		auto mousePos = ImGui::GetMousePos();
		radar_pos.x += mousePos.x - last_coords.x;
		radar_pos.y += mousePos.y - last_coords.y;
		last_coords = Source::Vector2(mousePos.x, mousePos.y);
	}
}

void F_Radar::Draw(Render::D3D::DrawManager* graphics)
{
	const ImVec2 point_size = ImVec2(0.25 * RADAR_SIZE, 0.25 * RADAR_SIZE);
	const ImVec2 size = ImVec2(RADAR_SIZE, RADAR_SIZE);

	graphics->RectFilled(ImVec2(radar_pos.x, radar_pos.y), size, CD3D(Color::LightGray()));
	graphics->Rect(ImVec2(radar_pos.x, radar_pos.y), size, CD3D(Color::Black()));
	for (RadarPoint point : radar_points)
		graphics->RectFilled(ImVec2(point.GetPos().x, point.GetPos().y), point_size, CD3D(point.GetColor()));
}

void F_Radar::OnEndScene(Render::D3D::DrawManager* graphics)
{
	if (Config::VisualsOther.Radar)
	{
		CollectPoints();
		mouse_down = ImGui::IsMouseDown(0);
		if (IsHovered())
			HandleMove();
		Draw(graphics);
	}
}

Source::Vector F_Radar::EntityToRadar(Source::C_BaseEntity* entity)
{
	auto local = C_CSPlayer::GetLocal();
	if (!local)
		return Source::Vector();

	if (entity == local)
		return Source::Vector(RADAR_SIZE / 2, RADAR_SIZE / 2, 0);

	const int point_size = 0.25 * RADAR_SIZE;

	const Vector entity_pos = entity->m_vecOrigin();
	const Vector local_pos = local->m_vecOrigin();

	const Vector direction = Vector(entity_pos.x - local_pos.x, entity_pos.y - local_pos.y, 0);

	Vector angles;
	Source::Engine->GetViewAngles(angles);

	const float radian = DEG2RAD(angles.y - 180.f);

	Vector dot_pos;

	dot_pos.x = (direction.y * scast<float>(cos(radian)) - direction.x * scast<float>(sin(radian))) / 20.0f;
	dot_pos.y = (direction.y * scast<float>(sin(radian)) + direction.x * scast<float>(cos(radian))) / 20.0f;

	dot_pos.x += radar_pos.x + RADAR_SIZE / 2;
	dot_pos.y += radar_pos.y + RADAR_SIZE / 2;

	dot_pos.x = clamp<float, float>(dot_pos.x, radar_pos.x, radar_pos.x + RADAR_SIZE - point_size);
	dot_pos.y = clamp<float, float>(dot_pos.y, radar_pos.y, radar_pos.y + RADAR_SIZE - point_size);

	return dot_pos;
}

bool F_Radar::IsHovered() const
{
	const auto p = ImGui::GetMousePos();
	return (p.x > radar_pos.x && p.x < (radar_pos.x + RADAR_SIZE)) && (p.y > radar_pos.y && p.y < (radar_pos.y + RADAR_SIZE));
}

#pragma endregion

/////////////////////////////////  OTHERS
#pragma region OTHERS

// Becomes true when custom crosshair is enabled
bool crosshair_enabled = false;

// Render custom crosshair and recoil crosshair
void F_VisualsOthers::RenderCrosshair(Render::D3D::DrawManager* graphics)
{
	// This will also make the crosshair a recoil crosshair in case the setting is enabled
	if (Config::VisualsOther.Crosshair && Source::Engine->IsInGame())
	{
		// Render the custom crosshair
		GRAB_SCREEN_SIZE
		float drx, dry;
		float ScreenCenterX = screen_width / 2, ScreenCenterY = screen_height / 2;

		auto local = C_CSPlayer::GetLocal();
		if (local)
		{
			if (Config::VisualsOther.RecoilCrosshair)
			{
				drx = ScreenCenterX - (ScreenCenterX / global->ClientFOV() * (local->m_aimPunchAngle().y * (Config::Removals.NoVisualRecoil ? global->RecoilScale() : 1)));
				dry = ScreenCenterY + (ScreenCenterX / global->ClientFOV() * (local->m_aimPunchAngle().x * (Config::Removals.NoVisualRecoil ? global->RecoilScale() : 1)));
			}
			else
			{
				drx = screen_width / 2;
				dry = screen_height / 2;
			}

			switch (Config::VisualsOther.CrosshairType)
			{
			case 0: // Static
				graphics->Line({ drx - 10, dry }, { drx + 10 , dry }, CD3D(Config::Colors.Crosshair.m_Value));
				graphics->Line({ drx, dry - 10 }, { drx, dry + 10 }, CD3D(Config::Colors.Crosshair.m_Value));
				break;
			case 1: // Static Swastika
				graphics->Line({ drx, dry }, { drx - 10, dry }, CD3D(Config::Colors.Crosshair.m_Value));
				graphics->Line({ drx - 10, dry }, { drx - 10, dry - 10 }, CD3D(Config::Colors.Crosshair.m_Value));

				graphics->Line({ drx, dry }, { drx, dry - 10 }, CD3D(Config::Colors.Crosshair.m_Value));
				graphics->Line({ drx, dry - 10 }, { drx + 10, dry - 10 }, CD3D(Config::Colors.Crosshair.m_Value));

				graphics->Line({ drx, dry }, { drx + 10, dry }, CD3D(Config::Colors.Crosshair.m_Value));
				graphics->Line({ drx + 10, dry }, { drx + 10, dry + 10 }, CD3D(Config::Colors.Crosshair.m_Value));

				graphics->Line({ drx, dry }, { drx, dry + 10 }, CD3D(Config::Colors.Crosshair.m_Value));
				graphics->Line({ drx, dry + 10 }, { drx - 10, dry + 10 }, CD3D(Config::Colors.Crosshair.m_Value));
				break;
			case 2: // Rotating Swastika
			{
				// This is special, requires some calculations!
				static float rotation_degree = 0.f;
				if (rotation_degree > 89.f)
					rotation_degree = 0.f;
				rotation_degree += 0.1f;
				int length = (int)(screen_width / 2 / 60);
				float gamma = atan(length / length);
				int i = 0;
				while (i < 4)
				{
					std::vector <int> p
					{
						int(length * sin(DEG2RAD(rotation_degree + (i * 90)))),
						int(length * cos(DEG2RAD(rotation_degree + (i * 90)))),
						int((length / cos(gamma)) * sin(DEG2RAD(rotation_degree + (i * 90) + RAD2DEG(gamma)))),
						int((length / cos(gamma)) * cos(DEG2RAD(rotation_degree + (i * 90) + RAD2DEG(gamma))))
					};
					graphics->Line({ drx, dry }, { drx + p[0], dry - p[1] }, CD3D(Config::Colors.Crosshair.m_Value));
					graphics->Line({ drx + p[0], dry - p[1] }, { drx + p[2], dry - p[3] }, CD3D(Config::Colors.Crosshair.m_Value));
					i++;
				}
			}
			break;
			}
		}

		// I set crosshair_enabled to true and stop original crosshair from drawing
		if (!crosshair_enabled)
		{
			Source::Engine->ExecuteClientCmd(enc("crosshair 0"));
			crosshair_enabled = true;
		}
	}
	else
	{
		// If the feature isn't enabled anymore I need to reset back crosshair to 1
		if (crosshair_enabled)
		{
			Source::Engine->ExecuteClientCmd(enc("crosshair 1"));
			crosshair_enabled = false;
		}
	}
}

// Force internal radar to draw
void F_VisualsOthers::ForceEngineRadar()
{
	if (Config::VisualsOther.EngineRadar)
	{
		// Take every single client and force m_bSpotted to true ez af
		FOR_EACH_CLIENT
		{
			auto player = GET_ENTITY(ClientIndex);
			
			if (player == nullptr)
				continue;
			if (player->m_lifeState() != LIFE_ALIVE)
				continue;
			if (player->GetNetworkable()->IsDormant())
				continue;

			*player->m_bSpottedPtr() = true;
		}
	}
}

// Becomes true when world materials gathering is done, also becomes false when I switch maps
bool world_storage_performed = false;
// Becomes true when world materials are changed
bool world_change_performed = false;
std::unordered_map<IMaterial*, FloatColor> world_textures;

// Color modulate the walls of the entire map with a defined color
void F_VisualsOthers::ColorWalls()
{
	FloatColor walls_color = Config::Colors.Walls.m_Value.toFloatColor();
	if (g_bWallColorModified && world_change_performed)
	{
		world_change_performed = false;
	}

	if (Config::VisualsOther.ColorWalls)
	{
		// I need to store pointers to skybox materials once
		if (!world_storage_performed)
		{
			// For each material
			for (MaterialHandle_t i = Source::MaterialSystem->FirstMaterial();
				i != Source::MaterialSystem->InvalidMaterial();
				i = Source::MaterialSystem->NextMaterial(i))
			{
				// Grab current material
				IMaterial* mat = Source::MaterialSystem->GetMaterial(i);
				// If it's valid and the texture group is skybox textures
				if (mat && !strcmp(mat->GetTextureGroupName(), TEXTURE_GROUP_WORLD))
				{
					// Store this material
					float r, g, b, a;
					mat->GetColorModulation(&r, &g, &b);
					a = mat->GetAlphaModulation();
					world_textures[mat] = FloatColor(r, g, b, a);
				}
			}
			world_storage_performed = true;
		}

		if (!world_change_performed && !world_textures.empty())
		{
			// For each material inside world_textures
			for (auto m : world_textures)
			{
				IMaterial* mat = m.first;
				mat->ColorModulate(walls_color.r(), walls_color.g(), walls_color.b());
			}
			world_change_performed = true;
		}
	}
	else
	{
		// If color walls isn't enabled and change has been performed, reset
		if (world_change_performed)
		{
			// For each material
			for (MaterialHandle_t i = Source::MaterialSystem->FirstMaterial();
				i != Source::MaterialSystem->InvalidMaterial();
				i = Source::MaterialSystem->NextMaterial(i))
			{
				// Grab current material
				IMaterial* mat = Source::MaterialSystem->GetMaterial(i);
				// If it's valid and the texture group is world textures
				if (mat && !strcmp(mat->GetTextureGroupName(), TEXTURE_GROUP_WORLD))
				{
					// Reset material colors
					FloatColor original_color = world_textures[mat];
					mat->ColorModulate(original_color.r(), original_color.g(), original_color.b());
					mat->AlphaModulate(original_color.a());
				}
			}
		}
		world_change_performed = false;
	}
}

// Perform thirdperson behaviour
void F_VisualsOthers::ThirdPerson(CViewSetup* setup)
{
	if (Source::Engine->IsInGame())
	{
		Tools::CThirdpersonManager::GetInstance()->UpdateThirdPerson(setup);
	}
}

bool hit_head = false;
float hit_alpha = 0.f;
const int line_size = 12;
const int line_off = 2;

// Draw the hitmarker if hit has been detected
void F_VisualsOthers::DrawHitmarker()
{
	// If the hit_alpha is > 0 it means that I need to draw an hitmarker
	if (hit_alpha > 0.f)
	{
		Source::Color hitmarker_color = Source::Color(200, 200, 200, hit_alpha * 255);
		GRAB_SCREEN_SIZE
		int center_x = screen_width / 2;
		int center_y = screen_height / 2;
		if (hit_head) hitmarker_color = Source::Color(255, 0, 0, hit_alpha * 255);
		Render::Line(center_x - line_size, center_y - line_size, center_x - (line_size / line_off), center_y - (line_size / line_off), hitmarker_color);
		Render::Line(center_x - line_size, center_y + line_size, center_x - (line_size / line_off), center_y + (line_size / line_off), hitmarker_color);
		Render::Line(center_x + line_size, center_y + line_size, center_x + (line_size / line_off), center_y + (line_size / line_off), hitmarker_color);
		Render::Line(center_x + line_size, center_y - line_size, center_x + (line_size / line_off), center_y - (line_size / line_off), hitmarker_color);
		hit_alpha -= 0.0025f;
	}
}

// Used for thirdperson
void F_VisualsOthers::OnOverrideView(CViewSetup* setup)
{
	this->ThirdPerson(setup);
	// Set custom fov (if I'm not scoped in ofc)
	auto local = C_CSPlayer::GetLocal();
	if (local && Source::Engine->IsInGame())
	{
		if (Config::VisualsOther.CustomFOV > 0.f &&
			setup->fov == 90.f && !local->m_bIsScoped())
		{
			setup->fov += Config::VisualsOther.CustomFOV;
		}
	}
}

// Used for ColorWalls feature
void F_VisualsOthers::OnDrawModelExecute(void* context, void* state, const Source::ModelRenderInfo_t& render_info, matrix3x4_t* custom_bone_to_world)
{
	this->ColorWalls();
}

// Used to force engine radar, it's a good loop I think
void F_VisualsOthers::OnCreateMove(Source::CUserCmd* cmd)
{
	this->ForceEngineRadar();
}

// Used to catch the visual toggle key and toggle visuals on/off
void F_VisualsOthers::OnKeyEvent(int event_code, Source::ButtonCode_t key, const char * key_binding)
{
	if (key == Config::VisualsOther.ToggleKey)
	{
		Config::ESP.Active.m_Value = !Config::ESP.Active.m_Value;
	}
}

// I use EndScene to render custom crosshairs
void F_VisualsOthers::OnEndScene(Render::D3D::DrawManager* graphics)
{
	if (graphics)
	{
		this->RenderCrosshair(graphics);
		this->RenderSpreadCircle(graphics);
	}
}

std::array<Laser_t, MAX_CLIENTS> Lasers{};
std::array<BeamRing_t, MAX_CLIENTS> BeamRings{};
void F_VisualsOthers::AddLaser(Laser_t laser)
{
	Lasers[laser.entity->GetIndex()] = laser;
}
void F_VisualsOthers::AddBeamRing(BeamRing_t ring)
{
	BeamRings[ring.source->GetIndex()] = ring;
}

void F_VisualsOthers::RenderSpreadCircle(Render::D3D::DrawManager* graphics)
{
	auto local = C_CSPlayer::GetLocal();
	if (Config::VisualsOther.SpreadCircle && Source::Engine->IsInGame() && local)
	{
		auto weapon = local->GetActiveWeapon();
		if (weapon)
		{
			graphics->CircleFilled(ImVec2(global->ScreenWidth() / 2, global->ScreenHeight() / 2),
				500 * (weapon->GetInaccuracy() + weapon->GetSpread()), CD3D(Color(0, 0, 0, 100)), 64);
		}
	}
}

void F_VisualsOthers::PrintHurtLog(Source::C_CSPlayer* local, Source::C_CSPlayer* got_hurt,
	Source::IGameEvent* hurt_event)
{
	//char16_t* testString = u"Did you know that all niggers in Africa have AIDS?";
	//Source::CenterPrint->Print(testString);
}

void F_VisualsOthers::OnListenedEvent(Source::IGameEvent* game_event)
{
	if (EVENT_IS(enc("player_connect_full")))
	{
		// Detect player full connection, meaning that I should regrab all the sky and world materials
		world_storage_performed = false;
		world_change_performed = false;
		skybox_removal_performed = false;
		global->set_RecoilScale(Source::CVar->FindVar(enc("weapon_recoil_scale"))->GetFloat());
	}
	if (EVENT_IS(enc("player_hurt")))
	{
		if (Config::VisualsOther.Hitmarker)
		{
			// Detected player hurt, if I'm the one that gave the damage signal new hitmarker
			if (Source::Engine->GetPlayerForUserID(game_event->GetInt(enc("attacker"))) 
				== Source::Engine->GetLocalPlayer())
			{
				// Play the sound and signal drawing
				PlaySound(db_Hitsound, NULL, SND_MEMORY | SND_ASYNC);
				hit_head = game_event->GetInt(enc("hitgroup")) == 1; // Signal HS
				hit_alpha = 1.f;
			}
		}
		if (Config::VisualsOther.HurtLogs)
		{
			// Detected player hurt, if I'm the one that gave the damage signal new hurt log
			auto local = C_CSPlayer::GetLocal();
			if (local)
			{
				auto hurt = USERID2PLAYER(game_event->GetInt(enc("userid")));
				auto attacker = USERID2PLAYER(game_event->GetInt(enc("attacker")));
				if (hurt && attacker && attacker == local)
				{
					this->PrintHurtLog(local, hurt, game_event);
				}
			}
		}
	}
	if (EVENT_IS(enc("weapon_fire")))
	{
		if (Config::VisualsOther.Lasers && !Config::Ragebot.Active)
		{
			// If I'm not the one who fired
			auto local = C_CSPlayer::GetLocal();
			if (local)
			{
				auto shooter = USERID2PLAYER(game_event->GetInt(enc("userid")));
				if (shooter != nullptr && local == shooter)
				{
					// Add a new laser
					QAngle va; Source::Engine->GetViewAngles(va);
					Vector forward; Maths::AngleVectors(va, &forward);
					Vector head = local->GetEyePosition(); head.z -= 5.f;
					auto* weap = local->GetActiveWeapon();
					if (weap)
					{
						auto* info = weap->GetWeaponInfo();
						if (info && info->WeaponType != WeaponTypes::WEAPONTYPE_KNIFE)
							this->AddLaser(Laser_t(shooter, true, 1.f, forward, head + forward * info->flRange, 
								Config::Colors.Beams.m_Value)
							);
						// New laser has been issued
					}
				}
			}
		}
	}
	if (EVENT_IS(enc("player_footstep")))
	{
		if (Config::VisualsOther.FootstepRings)
		{
			// If I'm not the one who stepped
			auto local = C_CSPlayer::GetLocal();
			if (local)
			{
				auto walker = USERID2PLAYER(game_event->GetInt(enc("userid")));
				if (walker != nullptr && local != walker)
				{
					// Add a new ring
					BeamRing_t br;
					br.source = walker;
					br.render = true;
					this->AddBeamRing(br);
					// New ring has been issued
				}
			}
		}
	}
	if (DID_DISCONNECT())
	{
		world_textures.clear();
		skybox_textures.clear();
	}
}

void F_VisualsOthers::OnRenderView(const Source::CViewSetup& view, const Source::CViewSetup& hud_view, int clear_flags, int what_to_draw)
{
	auto local = C_CSPlayer::GetLocal();
	if (local)
	{
		if (Config::VisualsOther.Lasers)
		{
			for (auto& laser : Lasers)
			{
				if (laser.render)
				{
					if (!laser.created && !laser.entity->GotInvalidWeapon())
					{
						std::string laserType;
						switch (Config::VisualsOther.LaserType)
						{
						case 0: // Physic Beam
							laserType = enc("sprites/physbeam.vmt");
							break;
						case 1: // Glow Beam
							laserType = enc("sprites/glow01.vmt");
							break;
						default:
							laserType = enc("sprites/physbeam.vmt");
							break;
						}
						BeamInfo_t beamInfo;
						beamInfo.m_nType = TE_BEAMPOINTS;
						beamInfo.m_vecStart = laser.start;
						beamInfo.m_vecEnd = laser.end;
						beamInfo.m_pszModelName = laserType.c_str();
						beamInfo.m_nModelIndex = -1;
						beamInfo.m_flHaloScale = 0.0f;
						beamInfo.m_flLife = 1.f;
						beamInfo.m_flWidth = 2.0f;
						beamInfo.m_flEndWidth = 2.0f;
						beamInfo.m_flFadeLength = 0.0f;
						beamInfo.m_flAmplitude = 2.0f;
						beamInfo.m_flSpeed = -0.2f;
						beamInfo.m_nStartFrame = 0;
						beamInfo.m_flFrameRate = 0.f;
						beamInfo.m_flRed = laser.color.r();
						beamInfo.m_flGreen = laser.color.g();
						beamInfo.m_flBlue = laser.color.b();
						beamInfo.m_flBrightness = laser.alpha * 255.f;
						beamInfo.m_nSegments = 2;
						beamInfo.m_bRenderable = true;
						beamInfo.m_nFlags = 0;
						Beam_t* myBeam = Source::ViewRenderBeams->CreateBeamPoints(beamInfo);
						if (myBeam)
							Source::ViewRenderBeams->DrawBeam(myBeam);
						laser.created = true;
					}
					laser.alpha -= 0.1f;
					if (laser.alpha <= 0.f)
					{
						laser.alpha = 0.f;
						laser.render = false;
						laser.created = false;
					}
				}
			}
		}
		if (Config::VisualsOther.FootstepRings)
		{
			for (auto& ring : BeamRings)
			{
				if (ring.render)
				{
					BeamInfo_t beamRing;
					beamRing.m_nType = TE_BEAMRINGPOINT;
					beamRing.m_nSegments = 2;
					beamRing.m_vecCenter = ring.source->m_vecOrigin();
					beamRing.m_flStartRadius = ring.startRadius;
					beamRing.m_flEndRadius = ring.endRadius;
					beamRing.m_pszModelName = enc("sprites/physbeam.vmt");
					beamRing.m_nModelIndex = -1;
					beamRing.m_nStartFrame = 0;
					beamRing.m_flFrameRate = 10;
					beamRing.m_flLife = ring.life;
					beamRing.m_flWidth = ring.width;
					beamRing.m_flAmplitude = 0;
					beamRing.m_flSpeed = ring.speed;
					beamRing.m_nFlags = 0;
					beamRing.m_flFadeLength = 0;
					beamRing.m_bRenderable = true;
					beamRing.m_flRed = ring.color.r();
					beamRing.m_flGreen = ring.color.g();
					beamRing.m_flBlue = ring.color.b();
					beamRing.m_flBrightness = ring.alpha;
					Beam_t* myBeam = Source::ViewRenderBeams->CreateBeamRingPoint(beamRing);
					if (myBeam)
						Source::ViewRenderBeams->DrawBeam(myBeam);
				}
				ring.alpha -= 0.1f;
				if (ring.alpha <= 0.f)
				{
					ring.alpha = 0.f;
					ring.render = false;
				}
			}
		}
	}
}

void F_VisualsOthers::OnPaintTraverse()
{
	if (Config::VisualsOther.Hitmarker && Source::Engine->IsInGame())
	{
		this->DrawHitmarker();
	}
}

void F_VisualsOthers::OnShutdown()
{
	// Reset world materials only if change has been performed
	if (world_change_performed)
	{
		// For each material
		for (MaterialHandle_t i = Source::MaterialSystem->FirstMaterial();
			i != Source::MaterialSystem->InvalidMaterial();
			i = Source::MaterialSystem->NextMaterial(i))
		{
			// Grab current material
			IMaterial* mat = Source::MaterialSystem->GetMaterial(i);
			// If it's valid and the texture group is skybox textures
			if (mat && !strcmp(mat->GetTextureGroupName(), TEXTURE_GROUP_WORLD))
			{
				// Reset material colors
				FloatColor original_color = world_textures[mat];
				mat->ColorModulate(original_color.r(), original_color.g(), original_color.b());
				mat->AlphaModulate(original_color.a());
			}
		}
	}
}

#pragma endregion

}