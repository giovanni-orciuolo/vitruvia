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
#include "..\..\Vitruvia.h"
#include "..\..\InterfaceList\InterfaceList.h"
#include "..\..\GUI\ImGui\imgui.h"
#include "..\..\GUI\ImGui\imgui_internal.h"
#include "..\..\GUI\ImGui\DX9\imgui_impl_dx9.h"

namespace Vitruvia {
namespace Render {

// The namespace contains everything related to D3D rendering.
// Not to be confused with engine rendering!

namespace D3D
{
#define RGBA(r, g, b, a) ((a << 24) | (b << 16) | (g << 8) | r)

	enum text_flags
	{
		none = 0,
		centered_x = 1,
		centered_y = 2,
		outline = 4,
		drop_shadow = 8
	};

	DEFINE_ENUM_FLAG_OPERATORS(text_flags)

	class DrawManager
	{
	public:
		DrawManager(IDirect3DDevice9* device);
		~DrawManager();

		void CreateObjects();
		void InvalidateObjects();
		void BeginRendering();
		void EndRendering();

		void Text(ImVec2 point, ImU32 color, text_flags flags, const char* format, ...);
		void Rect(const ImVec2& a, float w, float h, ImU32 col, float rounding = 0.0f, int rounding_corners_flags = ~0, float thickness = 1.0f);

		void Line(const ImVec2& a, const ImVec2& b, ImU32 col, float thickness = 1.0f);
		void Line(const ImVec2& a, float w, float h, ImU32 col, float thickness = 1.0f);
		void Rect(const ImVec2& a, const ImVec2& b, ImU32 col, float rounding = 0.0f, int rounding_corners_flags = ~0, float thickness = 1.0f);
		void RectFilled(const ImVec2& a, const ImVec2& b, ImU32 col, float rounding = 0.0f, int rounding_corners_flags = ~0);
		void RectFilled(const ImVec2& a, float w, float h, ImU32 col, float rounding = 0.0f, int rounding_corners_flags = ~0);
		void RectFilledMultiColor(const ImVec2& a, const ImVec2& b, ImU32 col_upr_left, ImU32 col_upr_right, ImU32 col_bot_right, ImU32 col_bot_left);
		void Quad(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, ImU32 col, float thickness = 1.0f);
		void QuadFilled(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, ImU32 col);
		void Triangle(const ImVec2& a, const ImVec2& b, const ImVec2& c, ImU32 col, float thickness = 1.0f);
		void TriangleFilled(const ImVec2& a, const ImVec2& b, const ImVec2& c, ImU32 col);
		void Circle(const ImVec2& centre, float radius, ImU32 col, int num_segments = 12, float thickness = 1.0f);
		void CircleFilled(const ImVec2& centre, float radius, ImU32 col, int num_segments = 12);
		void Polyline(const ImVec2* points, const int num_points, ImU32 col, bool closed, float thickness, bool anti_aliased);
		void ConvexPolyFilled(const ImVec2* points, const int num_points, ImU32 col, bool anti_aliased);
		void BezierCurve(const ImVec2& pos0, const ImVec2& cp0, const ImVec2& cp1, const ImVec2& pos1, ImU32 col, float thickness, int num_segments = 0);

		bool W2S(const Source::Vector& origin, Source::Vector& screen);

		std::string GetFontName() { return FontName; }
		float GetFontSize() { return FontSize; }
	private:
		ImDrawData* GetDrawData();

		IDirect3DDevice9*   _device;
		IDirect3DTexture9*  _texture;
		ImDrawList*         _drawList;
		ImDrawData          _drawData;
		ImFontAtlas         _fonts;

		const std::string   FontName = enc("Ruda");
		const float         FontSize = 18.f;
	};
	extern DrawManager* g_pRenderer;

}

}
}
