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

int Tab::s_iTabHovering = 0;

inline void Smooth(Source::Vector2& from, Source::Vector2& to, float amount)
{
	to = from - (from - to) / 100.f * amount;
}

void Tab::Think()
{
	// Elements in the tab are visible only if I did select this tab
	// m_pParent will be 100% a pointer to Frame element
	this->m_bVisible = (m_pParent->IsVisible() && m_pParent->GetTab() == this->m_iTab);

	// If the user clicked on this tab, set the frame tab
	if (GUI_Input->Clicked(m_iRealX, m_iRealY, m_iWidth, m_iHeight))
		m_pParent->SetTab(this->m_iTab);

	// If I'm only holding in these coordinates
	if (GUI_Input->IsHovering(m_iRealX, m_iRealY, m_iWidth, m_iHeight))
		Tab::s_iTabHovering = this->m_iTab;
}

void Tab::Paint()
{
	// If I'm visible (which means that all my childs are visible)
	// I can draw the green bar
	if (this->m_bVisible)
		Render::FilledRect(m_iRealX, m_iRealY + TAB_EMPTY_OFFSET, TAB_BAR_WIDTH, TAB_BAR_HEIGHT, CLR_TAB_SELECTED);

	if (m_pParent->IsVisible())
	{
		// Draw the icon of the tab
		if (m_pImageRenderer->IsValid())
			m_pImageRenderer->Draw(m_iRealX + 29, m_iRealY);
		// Draw the title of the tab
		Render::String(Render::Fonts::Arial20, m_iRealX + (m_iWidth / 2.f), m_iRealY + m_iHeight - 1, CLR_TAB_TEXT, Render::eAlign::ALIGN_CENTER, "%s", this->m_strTitle.c_str());
	}
}

}
}

#endif