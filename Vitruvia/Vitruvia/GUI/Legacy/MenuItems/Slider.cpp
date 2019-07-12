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

// THIS IS LEGACY. IT'S JUST HERE TO COMMEMORATE :^(

#if 0

#include "../GUI.h"

namespace Vitruvia {
namespace GUI {

bool Slider::isGrabbed = false;

void Slider::Think()
{
	if (!m_bVisible || DropDown::IsDropDownOpened())
		return;

	// Clamp the value between the bounds if necessary
	*m_flValue = clamp<float, float>(*m_flValue, m_flMin, m_flMax);

	//////////////////////////////
	// FIXME: THIS IS A HUGE BUG RELATIVE TO INPUT FEELING
	// When sliding a slider, if you lower the mouse while still holding mouse1 you can
	// move the slider just behind and I DON'T WANT THIS TO HAPPEN.
	//////////////////////////////

	POINT mouse_point = GUI_Input->GetMousePos();
	if (GUI_Input->IsHolding(m_iRealX, m_iRealY - 6, m_iWidth, m_iHeight + 12))
	{
		// If holding within width and height, set the value accordingly to mouse position
		*m_flValue = clamp((mouse_point.x - m_iRealX) * m_flMax / ((float)(m_iWidth) - 2.f), m_flMin, m_flMax);
	}
}

void Slider::Paint()
{
	if (!m_bVisible)
		return;

	// Main rectangle
	Render::FilledRect(m_iRealX, m_iRealY + 3, m_iWidth, m_iHeight, CLR_SLIDER_MAIN);
	// Filler
	Render::FilledRect(m_iRealX, m_iRealY + 3, *m_flValue * (((float)(m_iWidth) - 2.f) / m_flMax), m_iHeight, CLR_SLIDER_FILL);
	// Outline
	Render::OutlinedRect(m_iRealX, m_iRealY + 3, m_iWidth, m_iHeight, CLR_SLIDER_OUTLINE);
	// Value text
	Render::String(Render::Fonts::MAIN_MENU_FONT, 
		m_iRealX + 2 - Render::GetStringWidth(Render::Fonts::MAIN_MENU_FONT, (!m_bIsInt) ? enc("%.1f") : enc("%.0f"), *m_flValue) - 4,
		m_iRealY, CLR_SLIDER_TEXT, Render::eAlign::ALIGN_LEFT, (!m_bIsInt) ? enc("%.1f") : enc("%.0f"), *m_flValue);
	// Title text
	Render::String(Render::Fonts::MAIN_MENU_FONT, m_iTextX, m_iTextY, CLR_SLIDER_TEXT, Render::eAlign::ALIGN_LEFT, enc("%s"), m_strTitle.c_str());
}

}
}

#endif