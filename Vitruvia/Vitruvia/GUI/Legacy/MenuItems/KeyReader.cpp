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

bool KeyReader::isWaitingKey = false;

//const char* GetButtonName() 
//{
//	for (auto i = Source::ButtonCode_t::BUTTON_CODE_NONE; i < Source::ButtonCode_t::BUTTON_CODE_COUNT; i++) 
//	{
//		if (!Source::InputSystem->IsButtonDown(i)) continue;
//		return Source::InputSystem->ButtonCodeToString(i);
//	}
//	return "";
//}

void KeyReader::Think()
{
	if (!m_bVisible || DropDown::IsDropDownOpened())
		return;

	// Turn on waiting if clicked on a keyreader
	if (GUI_Input->Clicked(m_iRealX, m_iRealY - 4, m_iWidth, m_iHeight)
		&& m_iTick == 0 && !KeyReader::isWaitingKey)
	{
		m_bWaiting = true;
		KeyReader::isWaitingKey = true;
		Source::InputSystem->EnableInput(true);
	}

	if (m_bWaiting)
	{
		// Loop through every single key, apply filters
		for (int i = Source::ButtonCode_t::KEY_NONE; i < Source::ButtonCode_t::BUTTON_CODE_COUNT; i++)
		{
			// If the key is being pressed
			if (Source::InputSystem->IsButtonDown((Source::ButtonCode_t)i))
			{
				m_iTick = 120; // Wait 120 ticks before re-enabling waiting
				*m_kValue = i; // Set the key value
				m_bWaiting = false;
				KeyReader::isWaitingKey = false; // Not waiting for a key anymore
				Source::InputSystem->EnableInput(false);
			}
		}
	}
}

void KeyReader::Paint()
{
	if (!m_bVisible)
		return;

	// Main rectangle
	Render::FilledRect(m_iRealX, m_iRealY - 4, m_iWidth, m_iHeight, CLR_KEYREADER_MAIN);
	// Outline
	Render::OutlinedRect(m_iRealX, m_iRealY - 4, m_iWidth, m_iHeight, CLR_KEYREADER_OUTLINE);
	// Key Text
	Render::String(Render::Fonts::MAIN_MENU_FONT, m_iRealX + 5, m_iRealY - 4 + 2, CLR_KEYREADER_TEXT, Render::eAlign::ALIGN_LEFT, enc("%s"),
		m_bWaiting ? enc("Waiting Key...") : /*Source::InputSystem->ButtonCodeToString((Source::ButtonCode_t)*m_kValue)*/enc("Pls fix me!"));
	// Title text
	Render::String(Render::Fonts::MAIN_MENU_FONT, m_iTextX, m_iTextY, CLR_KEYREADER_TEXT, Render::eAlign::ALIGN_LEFT, enc("%s"), m_strTitle.c_str());
}

}
}

#endif