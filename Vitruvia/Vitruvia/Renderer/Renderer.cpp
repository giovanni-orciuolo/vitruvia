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

#include "Renderer.h"

namespace Vitruvia {
namespace Render {

ImageRenderer::ImageRenderer(std::uint32_t res_id, std::uint32_t width, std::uint32_t height, Source::Color color)
{
	m_iWidth = width;
	m_iHeight = height;
	m_bgColor = color;
	m_bValid = false;

	// TODO: Add error checking here!
	HMODULE this_module = global->ThisModule();
	HRSRC hRes = FindResource(this_module, MAKEINTRESOURCE(res_id), RT_RCDATA);
	HGLOBAL hMemory = LoadResource(this_module, hRes);
	DWORD dwSize = SizeofResource(this_module, hRes);
	BYTE* lpAddress = (BYTE*)LockResource(hMemory);
	m_pRawData = new BYTE[dwSize];
	CopyMemory(m_pRawData, lpAddress, dwSize);

	m_iTexture = Source::Surface->CreateNewTextureID(true);
	if (!m_iTexture) 
		return;

	Source::Surface->DrawSetTextureRGBA(m_iTexture, m_pRawData, width, height);
	m_bValid = true;
}
bool ImageRenderer::Draw(int x, int y, float scale)
{
	if (!Source::Surface->IsTextureIDValid(m_iTexture))
		return false;

	Source::Surface->DrawSetTexture(m_iTexture);
	Source::Surface->DrawSetColor(m_bgColor);
	Source::Surface->DrawTexturedRect(x, y, x + m_iWidth * scale, y + m_iHeight * scale);

	return true;
}

bool WorldToScreen(const Source::Vector& origin, Source::Vector& screen)
{
	using namespace Source;
	// This is just a wrapper for IVDebugOverlay::ScreenPosition, which does exactly what we need
	// Manually calculating shit is way better for externals lmao
	return DebugOverlay->ScreenPosition(origin, screen) != -1;
}
void String(HFont font, int x, int y, Source::Color color, eAlign align, const char* str, ...)
{
	using namespace Source;

	// Store in a va_list the variadic arguments, in a printf fashioned way
	va_list variadic_list;
	char buffer[1024];
	va_start(variadic_list, str);
	_vsnprintf(buffer, sizeof(buffer), str, variadic_list);
	va_end(variadic_list);

	// Convert the buffer in wstring
	std::wstring wstr = S2WS(buffer);

	// Set text font and color
	Surface->DrawSetTextFont(font);
	Surface->DrawSetTextColor(color.r(), color.g(), color.b(), color.a());
	
	switch (align)
	{

	case eAlign::ALIGN_LEFT: {
		Surface->DrawSetTextPos(x, y);
	}
	break;
	case eAlign::ALIGN_CENTER: {
		int txt_width, txt_height;
		Surface->GetTextSize(font, wstr.c_str(), txt_width, txt_height);
		Surface->DrawSetTextPos(x - txt_width / 2, y - txt_height / 2);
	}
	break;
	case eAlign::ALIGN_RIGHT: {
		int txt_width = GetStringWidth(font, buffer);
		Surface->DrawSetTextPos(x - txt_width, y);
	}

	}

	// Finally draw the text
	Surface->DrawPrintText(wstr.c_str(), wcslen(wstr.c_str()));
}
void Polygon(int vertex_count, Source::VGUI::Vertex_t* vertexes, Source::Color color)
{
	using namespace Source;

	// Create a static new texture to draw our polygons
	static int texture = Surface->CreateNewTextureID(true);

	// Set the texture RGBA values (width and height will be 1)
	byte buffer[4] = { 255, 255, 255, 255 };
	Surface->DrawSetTextureRGBA(texture, buffer, 1, 1);

	// Just a check to be careful...
	if (!Surface->IsTextureIDValid(texture))
		return;

	// Set color and texture
	Surface->DrawSetColor(color);
	Surface->DrawSetTexture(texture);

	// Draw a textured polygon
	Surface->DrawTexturedPolygon(vertex_count, vertexes);
}
void FilledRect(int x, int y, int w, int h, Source::Color color)
{
	using namespace Source;
	// This is actually straightforward af, do I even need to comment?
	Surface->DrawSetColor(color);
	Surface->DrawFilledRect(x, y, x + w, y + h);
}
void OutlinedRect(int x, int y, int w, int h, Source::Color color)
{
	using namespace Source;
	// Well, if you need comments for this you better leave now
	Surface->DrawSetColor(color);
	Surface->DrawOutlinedRect(x, y, x + w, y + h);
}
void RainbowRect(int x, int y, int width, int height, float speed, float& rainbow_amt)
{
	using namespace Source;
	// NOTE: This function is fucking heavy, especially if the width is high, it will need to draw 'width'-n rectangles

	// Increase and clamp the rainbow amount
	rainbow_amt += speed;
	if (rainbow_amt > 1.f) rainbow_amt = 0.f;

	// For each pixel until we reach the width
	for (int i = 0; i < width; i++)
	{
		// Calculate the hue
		float hue = (1.f / (float)(width)) * i;
		hue -= rainbow_amt;
		// Clamp hue value
		if (hue < 0.f) hue += 1.f;

		// Extract the rainbow color
		Color col_rainbow = Color::FromHSB(hue, 1.f, 1.f);
		// Draw a rect using the color extracted
		FilledRect(x + i, y, 1, height, col_rainbow);
	}
}
void RectGradientHorizontal(int x, int y, int width, int height, Source::Color color1, Source::Color color2)
{
	using namespace Source;
	// NOTE: This function is fucking heavy, especially if the width is high, it will need to draw 'width'-n rectangles

	// Calculate rgb values delta / width
	float difference_r = (float)(color2.r() - color1.r()) / (float)(width);
	float difference_g = (float)(color2.g() - color1.g()) / (float)(width);
	float difference_b = (float)(color2.b() - color1.b()) / (float)(width);

	// For each pixel until we reach the width
	for (float i = 0.f; i < width; i++)
	{
		// Calculate the gradient
		Color col_gradient = Color(color1.r() + (difference_r * i), color1.g() + (difference_g * i), color1.b() + (difference_b * i), color1.a());
		// Draw a 1 pixel filled rect with the color extracted
		FilledRect(x + i, y, 1, height, col_gradient);
	}
}
void RectGradientVertical(int x, int y, int width, int height, Source::Color color1, Source::Color color2)
{
	using namespace Source;
	// NOTE: This function is fucking heavy, especially if the width is high, it will need to draw 'height'-n rectangles

	// Calculate rgb values delta / height
	float difference_r = (float)(color2.r() - color1.r()) / (float)(height);
	float difference_g = (float)(color2.g() - color1.g()) / (float)(height);
	float difference_b = (float)(color2.b() - color1.b()) / (float)(height);

	// For each pixel until we reach the height
	for (float i = 0.f; i < height; i++)
	{
		// Calculate the gradient
		Color col_gradient = Color(color1.r() + (difference_r * i), color1.g() + (difference_g * i), color1.b() + (difference_b * i), color1.a());
		// Draw a 1 pixel filled rect with the color extracted
		FilledRect(x, y + i, width, 1, col_gradient);
	}
}
void Pixel(int x, int y, Source::Color color)
{
	using namespace Source;
	// We just draw a filled rect of width 1 and height 1 rofl
	FilledRect(x, y, 1, 1, color);
}
void Line(Source::Vector2 v0, Source::Vector2 v1, Source::Color color)
{
	using namespace Source;
	// We just use the vfunc that ISurface provides (good boy ISurface)
	Surface->DrawSetColor(color);
	Surface->DrawLine(v0.x, v0.y, v1.x, v1.y);
}
void Line(int x0, int y0, int x1, int y1, Source::Color color)
{
	using namespace Source;
	// We just use the vfunc that ISurface provides (good boy ISurface)
	Surface->DrawSetColor(color);
	Surface->DrawLine(x0, y0, x1, y1);
}
void OutlinedCircle(int x, int y, int radius, int segments, Source::Color color)
{
	using namespace Source;
	// We would need to calculate shit to draw a basic circle, but we can use ISurface func :) (thx based ISurface)
	Surface->DrawSetColor(color);
	Surface->DrawOutlinedCircle(x, y, radius, segments);
}
void FilledCircle(int x, int y, int radius, int segments, Source::Color color)
{
	using namespace Source;
	// Since ISurface betrayed us, we don't have a func to easily draw a filled circle... :'(
	// We will just draw a lot of outlined circles until we reach the end of the radius
	// Notice that this makes this function quite heavy, depending on how big the radius is
	for (int i = 0; i <= radius; i++)
	{
		OutlinedCircle(x, y, i, segments, color);
	}
}
int GetStringWidth(HFont font, const char* str, ...)
{
	using namespace Source;
	// Since we will be dealing with strings modified by variadic arguments, we need to account for them...
	// In a similar way we are doing in String()

	// Store in a va_list the variadic arguments, in a printf fashioned way
	va_list variadic_list;
	char buffer[1024];
	va_start(variadic_list, str);
	_vsnprintf(buffer, sizeof(buffer), str, variadic_list);
	va_end(variadic_list);

	// Convert the buffer in wstring
	std::wstring wstr = S2WS(buffer);

	// Call the ISurface vfunc
	int width, height;
	Surface->GetTextSize(font, wstr.c_str(), width, height);
	return width;
}
void Rect3D(Source::Vector* vectors, Source::Color col)
{
	using namespace Source;
	// NOTE: This is probably the most heavy render function alive.
	// Use carefully

	// Perform a WorldToScreen for each vertex (so 8 W2S calls)
	Vector box_vectors[8]{};
	for (int i = 0; i < 8; i++)
		if (!WorldToScreen(vectors[i], box_vectors[i]))
			return; // Abort immediately if we can't w2s a single point

	// Start calculating manually everything
	// I didn't write the lines below (just re-adapted), can't remember exactly who did all this shit in the first place

	Vector lines[12][2];
	//bottom of box
	lines[0][0] = { box_vectors[0].x, box_vectors[0].y };
	lines[0][1] = { box_vectors[1].x, box_vectors[1].y };
	lines[1][0] = { box_vectors[1].x, box_vectors[1].y };
	lines[1][1] = { box_vectors[2].x, box_vectors[2].y };
	lines[2][0] = { box_vectors[2].x, box_vectors[2].y };
	lines[2][1] = { box_vectors[3].x, box_vectors[3].y };
	lines[3][0] = { box_vectors[3].x, box_vectors[3].y };
	lines[3][1] = { box_vectors[0].x, box_vectors[0].y };

	lines[4][0] = { box_vectors[0].x, box_vectors[0].y };
	lines[4][1] = { box_vectors[6].x, box_vectors[6].y };

	// top of box
	lines[5][0] = { box_vectors[6].x, box_vectors[6].y };
	lines[5][1] = { box_vectors[5].x, box_vectors[5].y };
	lines[6][0] = { box_vectors[5].x, box_vectors[5].y };
	lines[6][1] = { box_vectors[4].x, box_vectors[4].y };
	lines[7][0] = { box_vectors[4].x, box_vectors[4].y };
	lines[7][1] = { box_vectors[7].x, box_vectors[7].y };
	lines[8][0] = { box_vectors[7].x, box_vectors[7].y };
	lines[8][1] = { box_vectors[6].x, box_vectors[6].y };

	lines[9][0] = { box_vectors[5].x, box_vectors[5].y };
	lines[9][1] = { box_vectors[1].x, box_vectors[1].y };

	lines[10][0] = { box_vectors[4].x, box_vectors[4].y };
	lines[10][1] = { box_vectors[2].x, box_vectors[2].y };

	lines[11][0] = { box_vectors[7].x, box_vectors[7].y };
	lines[11][1] = { box_vectors[3].x, box_vectors[3].y };

	// Draw lines to form the 3D box
	for (int i = 0; i < 12; i++)
	{
		Line(lines[i][0].x, lines[i][0].y, lines[i][1].x, lines[i][1].y, col);
	}
}
void Image(int x, int y, int w, int h, Source::Color color, const byte* raw_data, float scale)
{
	using namespace Source;

	// Create a static new texture to draw our image on
	static int texture = Surface->CreateNewTextureID(true);

	// Set the texture RGBA values based on the raw_data bytes
	Surface->DrawSetTextureRGBA(texture, raw_data, w, h);

	// Just a check to be careful...
	if (Surface->IsTextureIDValid(texture))
		return;

	// Set color and texture
	Surface->DrawSetColor(color);
	Surface->DrawSetTexture(texture);

	// Draw a textured rect
	Surface->DrawTexturedRect(x, y, x + w * scale, y + h * scale);
}

}
}
