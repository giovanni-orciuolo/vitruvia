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

#include "MenuInput.h"

namespace Vitruvia {
namespace GUI {

V_MenuInput* GUI_Input = new V_MenuInput;

V_MenuInput::V_MenuInput(const std::string& window_name)
{
	m_hWindow = FindWindow(window_name.c_str(), NULL);
}

POINT V_MenuInput::GetMousePos()
{
	static POINT mouse_point;
	GetCursorPos(&mouse_point);
	ScreenToClient(m_hWindow, &mouse_point);
	int scr_width, scr_height;
	Source::Engine->GetScreenSize(scr_width, scr_height);
	mouse_point.x = clamp(mouse_point.x, 1, scr_width);
	mouse_point.y = clamp(mouse_point.y, 1, scr_height);
	return mouse_point;
}

void V_MenuInput::IsClicking()
{
	if (KEY(VK_LBUTTON) && m_hWindow == GetActiveWindow())
	{
		m_bClicked = false;
		m_bMouseController = true;
	}
	else if (m_bMouseController)
	{
		m_bClicked = true;
		m_bMouseController = false;
	}
	else if (m_bClicked)
	{
		m_bClicked = false;
	}
}

bool V_MenuInput::IsHolding(int x, int y, int width, int height)
{
	if (KEY(VK_LBUTTON) && m_hWindow == GetActiveWindow())
	{
		return IsHovering(x, y, width, height);
	} 
	return false;
}

bool V_MenuInput::IsHovering(int x, int y, int width, int height)
{
	POINT mouse_point = GetMousePos();

	if (mouse_point.x >= x && x + width >= mouse_point.x)
		if (mouse_point.y >= y && y + height >= mouse_point.y)
			return true;

	return false;
}

bool V_MenuInput::Clicked(int x, int y, int width, int height)
{
	if (m_bClicked)
		if (IsHovering(x, y, width, height))
			return true;

	return false;
}

}
}

#endif