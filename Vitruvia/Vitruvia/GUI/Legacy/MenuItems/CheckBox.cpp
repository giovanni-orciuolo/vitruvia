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

void CheckBox::Think()
{
	if (!m_bVisible && !DropDown::IsDropDownOpened())
		return;

	if (GUI_Input->Clicked(m_iX - 2, m_iY - 2, m_iWidth, m_iHeight))
	{
		*m_bValue = !*m_bValue;
	}
}

void CheckBox::Paint()
{
	if (!m_bVisible)
		return;

	Render::FilledRect(m_iX, m_iY - 2, m_iWidth, m_iHeight, *m_bValue ? CLR_CHECKBOX_FULL : CLR_CHECKBOX_EMPTY);
	Render::OutlinedRect(m_iX, m_iY - 2, m_iWidth, m_iHeight, CLR_CHECKBOX_OUTLINE);
	Render::String(Render::Fonts::MAIN_MENU_FONT, m_iTextX, m_iTextY, CLR_CHECKBOX_TEXT, Render::eAlign::ALIGN_LEFT, enc("%s"), m_strTitle.c_str());
}

}
}

#endif