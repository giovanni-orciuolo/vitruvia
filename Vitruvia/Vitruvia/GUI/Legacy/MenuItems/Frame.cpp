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

Frame::Frame(const std::string& priv_name, const std::string& title)
	: MenuItem(priv_name, title)
{
	m_pMouseEnable = Source::CVar->FindVar(enc("cl_mouseenable"));
	m_eType = eMenuItem::FRAME;
}

bool call_once_frame = false;
void Frame::Think()
{
	if (m_iTick == 0 
		&& KEY(VK_INSERT)) // Modify here the menu key
	{
		m_iTick = 60;
		m_bVisible = !m_bVisible;
		m_pMouseEnable->SetValue(!m_bVisible);
		g_bMenuOpened = m_bVisible;
		Source::InputSystem->EnableInput(!m_bVisible);
		if (m_bVisible)
			Source::InputSystem->ResetInputState();
	}

	if (!m_bVisible)
		return;

	static int bWasntHolding = false;
	static int bGrabbing = false;
	static int iOffsetX = 0, iOffsetY = 0;

	POINT ptMouse = GUI_Input->GetMousePos();
	if (GUI_Input->IsHolding(m_iX, m_iY, m_iWidth, MIN_GRAB_HEIGHT) && !bGrabbing && bWasntHolding)
	{
		bGrabbing = true;
		iOffsetX = m_iX - ptMouse.x;
		iOffsetY = m_iY - ptMouse.y;
	}
	else if (bGrabbing && !KEY(VK_LBUTTON))
	{
		bGrabbing = false;
		iOffsetX = 0;
		iOffsetY = 0;
	}

	if (bGrabbing)
	{
		m_iX = ptMouse.x + iOffsetX;
		m_iY = ptMouse.y + iOffsetY;
	}

	int iScrW, iScrH;
	Source::Engine->GetScreenSize(iScrW, iScrH);

	m_iX = clamp(m_iX, 1, iScrW - m_iWidth - 1);
	m_iY = clamp(m_iY, 1, iScrH - m_iHeight - 1);

	bWasntHolding = GUI_Input->IsHovering(m_iX, m_iY, m_iWidth, MIN_GRAB_HEIGHT) && !KEY(VK_LBUTTON);

	// m_Childs WILL contain only pointer to tabs, especially if this is the main frame
	for (unsigned int i = 0; i < m_Childs.size(); ++i)
	{
		// Get the child
		auto c = m_Childs[i];
		// If the child is a tab (hopefully)
		if (c->GetType() == eMenuItem::TAB)
		{
			// Set the real position dynamically
			c->SetPosition(this->m_iX, this->m_iY);
			c->SetOffsetPosition(0, 32 + (i * (c->GetHeight() + 24)));
			// Set the tab index accordingly
			c->SetTab(i);
		}
	}
}

void Frame::Paint()
{
	if (!m_bVisible) 
		return;
	
	// Main frame rectangle
	Render::FilledRect(m_iX, m_iY, m_iWidth, m_iHeight, CLR_FRAME_MAIN);
	// Upper bar rectangle
	Render::FilledRect(m_iX, m_iY, m_iWidth, MIN_GRAB_HEIGHT - 2, CLR_FRAME_UPPER_BAR);
	// Mini upper bar straight down the upper bar
	Render::FilledRect(m_iX, m_iY + MIN_GRAB_HEIGHT - 2, m_iWidth, 2, CLR_FRAME_UPPER_BAR_MINI);
	// Tabs rectangle
	Render::FilledRect(m_iX, m_iY + MIN_GRAB_HEIGHT, 106, m_iHeight - MIN_GRAB_HEIGHT, CLR_FRAME_TABS);
	// Tabs divider
	Render::FilledRect(m_iX + 105, m_iY + MIN_GRAB_HEIGHT, 1, m_iHeight - MIN_GRAB_HEIGHT, CLR_FRAME_TABS_DIV);
	// Small text inside the upper bar
	Render::String(Render::Fonts::MAIN_MENU_FONT, m_iX + 5, m_iY + 4, CLR_FRAME_TEXT, Render::eAlign::ALIGN_LEFT, m_strTitle.c_str());
}

void Frame::Center()
{
	int screen_width = 0, screen_height = 0;
	Source::Engine->GetScreenSize(screen_width, screen_height);
	m_iX = screen_width / 2 - m_iWidth / 2;
	m_iY = screen_height / 2 - m_iHeight / 2;
}

}
}

#endif