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

bool call_once_gb = true;
void GroupBox::Think()
{
	// Only if we want dynamic shit (basically always)
	if (m_bEnableDynamic)
	{
		// Determine the size of the childs vector
		auto size = m_Childs.size();

		// Adapt the height based on the size of the childs vector
		// 17 is the base value for height, basically the height without any childs
		m_iHeight = 17 * (size + 1);

		// For each child in the vector (I need the index so I don't use enhanced for)
		for (unsigned int i = 0; i < m_Childs.size(); i++)
		{
			// Grab the child
			auto child = m_Childs[i];
			// Adapt the positions accordingly :)
			child->SetTextPosition(m_iRealX + 12 - 3, m_iRealY + 17 * (i + 1) - 3);
			child->SetPosition(m_iRealX + m_iWidth - 6 - child->GetWidth() - 3, m_iRealY + 17 * (i + 1) - 3);
		}

		call_once_gb = false;
	}

	if (!m_bVisible)
		return;
	
	// If a parent frame is defined (MUST BE DEFINED)
	if (m_pParent)
	{
		// Set the position accordingly to the parent frame position,
		m_pParent->GetPosition(this->m_iX, this->m_iY);
	}
}

void GroupBox::Paint()
{
	if (!m_bVisible)
		return;
	// Main outer rectangle
	Render::OutlinedRect(m_iRealX, m_iRealY, m_iWidth, m_iHeight, CLR_GROUPBOX_OUTLINE);
	// Main inner rectangle
	Render::OutlinedRect(m_iRealX + 1, m_iRealY + 1, m_iWidth - 2, m_iHeight - 2, CLR_GROUPBOX_INNERLINE);
	// Cover rectangle
	int title_width = Render::GetStringWidth(Render::Fonts::MAIN_MENU_FONT, enc("%s"), m_strTitle.c_str());
	Render::FilledRect(m_iRealX + 25, m_iRealY, 7 + title_width + 7, 2, CLR_GROUPBOX_COVER);
	// Text
	Render::String(Render::Fonts::MAIN_MENU_FONT, m_iRealX + 25 + 7, m_iRealY - Source::Surface->GetFontTall(Render::Fonts::MAIN_MENU_FONT) / 2, CLR_GROUPBOX_TEXT, Render::eAlign::ALIGN_LEFT, enc("%s"), m_strTitle.c_str());
}

void GroupBox::AddChilds(const std::vector<MenuItem*>& menu_items)
{
	// Add childs using vectors from C++11
	for (auto item : menu_items) // For each child in the list given
	{
		if (item) // If the pointer is valid
			this->AddChild(item); // Call normal AddChild()
	}
}

}
}

#endif