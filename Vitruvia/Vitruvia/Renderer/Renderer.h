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
#include "Fonts\Fonts.h"
#include "..\Globals\VitruviaGlobals.h"

namespace Vitruvia {
namespace Render {

enum class eAlign
{
	ALIGN_LEFT,
	ALIGN_CENTER,
	ALIGN_RIGHT
};

// This class is used to render a specific image. Just build it and wrap it in a function
class ImageRenderer
{
public:
	ImageRenderer()
		: m_iWidth(0), m_iHeight(0), m_bgColor(Source::Color::White()), m_bValid(false) {}
	ImageRenderer(std::uint32_t res_id, 
		std::uint32_t width = 50, 
		std::uint32_t height = 50, 
		Source::Color color = Source::Color(200, 200, 200));
	~ImageRenderer() { delete[] m_pRawData; }

	bool IsValid() { return this->m_bValid; }
	std::uint32_t GetTextureID() const { return this->m_iTexture; };
	BYTE* GetRawData() const { return this->m_pRawData; }
	int GetWidth() { return this->m_iWidth; }
	int GetHeight() { return this->m_iHeight; }
	void GetSize(int& w, int& h) { w = this->m_iWidth; h = this->m_iHeight; }
	Source::Color GetColor() { return this->m_bgColor; }

	void SetWidth(int w) { this->m_iWidth = w; }
	void SetHeight(int h) { this->m_iHeight = h; }
	void SetSize(int w, int h) { this->m_iWidth = w; this->m_iHeight = h; }
	void SetColor(const Source::Color& col) { this->m_bgColor = col; }

	bool Draw(int x, int y, float scale = 1.f);

private:
	std::uint32_t m_iTexture; // Texture ID that the image is using in the engine
	BYTE* m_pRawData; // Raw data bytes
	std::uint32_t m_iWidth, m_iHeight; // Width and height of the image
	Source::Color m_bgColor; // Color of the image
	bool m_bValid; // True if the image is valid
};

extern bool WorldToScreen(const Source::Vector& origin, Source::Vector& screen);
extern void String(HFont font, int x, int y, Source::Color color, eAlign align, const char* str, ...);
extern void Polygon(int vertex_count, Source::VGUI::Vertex_t* vertexes, Source::Color color);
extern void FilledRect(int x, int y, int w, int h, Source::Color color);
extern void OutlinedRect(int x, int y, int w, int h, Source::Color color);
extern void RainbowRect(int x, int y, int width, int height, float speed, float& rainbow_amt);
extern void RectGradientHorizontal(int x, int y, int width, int height, Source::Color color1, Source::Color color2);
extern void RectGradientVertical(int x, int y, int width, int height, Source::Color color1, Source::Color color2);
extern void Pixel(int x, int y, Source::Color color);
extern void Line(Source::Vector2 v0, Source::Vector2 v1, Source::Color color);
extern void Line(int x0, int y0, int x1, int y1, Source::Color color);
extern void OutlinedCircle(int x, int y, int radius, int segments, Source::Color color);
extern void FilledCircle(int x, int y, int radius, int segments, Source::Color color);
extern int  GetStringWidth(HFont font, const char* str, ...);
extern void Rect3D(Source::Vector* vectors, Source::Color color);
extern void Image(int x, int y, int w, int h, Source::Color color, const byte* raw_data, float scale = 1.0f);

}
}
