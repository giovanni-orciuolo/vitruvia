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

bool DropDown::isDropDownOpened; // Used to track if any dropdown is opened
DropDown* DropDown::pOpenedDropDown; // Pointer to the currently opened dropdown, nullptr if no dropdown is opened

void DropDown::Think()
{
	if (!m_bVisible)
		return;

	// Check if clicking on a dropdown with no dropdowns opened
	if (GUI_Input->Clicked(m_iRealX, m_iRealY - 4, m_iWidth, m_iHeight) 
		&& DropDown::pOpenedDropDown == nullptr)
	{
		m_bIsOpened = !m_bIsOpened;
		DropDown::isDropDownOpened = true;
		DropDown::pOpenedDropDown = (m_bIsOpened) ? this : nullptr;
	}

	// Continue only if the dropdown is opened
	if (m_bIsOpened)
	{
		if (GUI_Input->GetClick())
		{
			POINT mouse_point = GUI_Input->GetMousePos();
			// Check if mouse is within bounds and if we are operating with our own dropdown
			if (
				(mouse_point.x < m_iRealX
					|| mouse_point.x > m_iRealX + m_iWidth
					|| mouse_point.y < m_iRealY - 4
					|| mouse_point.y > m_iRealY - 4 + m_iHeight)
				&& DropDown::pOpenedDropDown == this)
			{
				for (unsigned int i = 0; i < m_Items.size(); i++)
				{
					bool hovering = GUI_Input->IsHovering(m_iRealX, m_iRealY - 4 + m_iHeight * (i + 1), m_iWidth, m_iHeight);
					if (hovering && GUI_Input->GetClick())
					{
						*m_iValue = i;
						m_strCurrentItem = m_Items[i];
						m_bIsOpened = false;
						DropDown::isDropDownOpened = false;
						DropDown::pOpenedDropDown = nullptr;
						break;
					}
				}
			}
		}
	}
}

void DropDown::Paint()
{
	if (!m_bVisible)
		return;

	// Main rectangle
	Render::FilledRect(m_iRealX, m_iRealY - 4, m_iWidth, m_iHeight, CLR_DROPDOWN_MAIN);
	// Outline
	Render::OutlinedRect(m_iRealX, m_iRealY - 4, m_iWidth, m_iHeight, CLR_DROPDOWN_OUTLINE);

	// If the dropdown is opened
	if (m_bIsOpened)
	{
		for (unsigned int i = 0; i < m_Items.size(); i++)
		{
			bool hovering = GUI_Input->IsHovering(m_iRealX, m_iRealY - 4 + m_iHeight * (i + 1), m_iWidth, m_iHeight);
			Render::FilledRect(m_iRealX, m_iRealY - 4 + m_iHeight * (i + 1), m_iWidth, m_iHeight,
				hovering ? CLR_DROPDOWN_HOVERING : CLR_DROPDOWN_MAIN);
			Render::String(Render::Fonts::MAIN_MENU_FONT, m_iRealX + 5, m_iRealY - 4 + 5 + m_iHeight * (i + 1), CLR_DROPDOWN_TEXT, Render::eAlign::ALIGN_LEFT, enc("%s"), m_Items[i].c_str());
		}
	}

	// Draw the current item
	Render::String(Render::Fonts::MAIN_MENU_FONT, m_iRealX + 5, m_iRealY + 2 - 4, CLR_DROPDOWN_TEXT, Render::eAlign::ALIGN_LEFT, enc("%s"), m_strCurrentItem.c_str());
	// Draw the title
	Render::String(Render::Fonts::MAIN_MENU_FONT, m_iTextX, m_iTextY, CLR_DROPDOWN_TEXT, Render::eAlign::ALIGN_LEFT, enc("%s"), m_strTitle.c_str());
}

DropDown& DropDown::operator+=(const std::string& item)
{
	// Add an item if it's not already present in the list
	if (std::find(m_Items.begin(), m_Items.end(), item) != m_Items.end())
	{
		Warn(enc("Inside %s: Couldn't add item %s, item is already present"), m_strPrivateName.c_str(), item.c_str());
		return *this;
	}
	m_Items.push_back(item);
	return *this;
}

void DropDown::SetCurrentItem(const std::string& item)
{
	// Set the current item but also set the index to adapt it
	// So we need the index where this item is located
	auto it = std::find(m_Items.begin(), m_Items.end(), item);
	// Check if item is present, otherwise we can't set it as the current item
	if (it == m_Items.end())
	{
		Warn(enc("Inside %s: Couldn't set current item to be %s, item is not present!"), m_strPrivateName.c_str(), item.c_str());
		return;
	}
	// Retrieve the index from the iterator
	int index = it - m_Items.begin();
	// Set current item
	m_strCurrentItem = item;
	// Modify iValue index accordingly
	*m_iValue = index;
}

}
}

#endif