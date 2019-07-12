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

MenuItem::MenuItem()
{
	m_strTitle = "";
	m_strPrivateName = "";
	m_iX = m_iY = 0;
	m_iOffsetX = m_iOffsetY = 0;
	m_iRealX = m_iRealY = 0;
	m_iTextX = m_iTextY = 0;
	m_iWidth = m_iHeight = 0;
	m_iTab = 0;
	m_iTick = 0;
	m_bVisible = false;
	m_pParent = nullptr;
	m_Childs.clear();
	m_Childs.shrink_to_fit();
	m_eType = eMenuItem::NONE;
	m_bNewColumn = false;
}

MenuItem::MenuItem(const std::string& priv_name, const std::string& title)
{
	// I KNOW THIS IS REDUNDANT!
	Log(enc("Initializing menu item %s..."), priv_name.c_str());
	m_strTitle = title;
	m_strPrivateName = priv_name;
	m_iX = m_iY = 0;
	m_iOffsetX = m_iOffsetY = 0;
	m_iRealX = m_iRealY = 0;
	m_iTextX = m_iTextY = 0;
	m_iWidth = m_iHeight = 0;
	m_iTab = 0;
	m_iTick = 0;
	m_bVisible = false;
	m_pParent = nullptr;
	m_Childs.clear();
	m_Childs.shrink_to_fit();
	m_eType = eMenuItem::NONE;
	m_bNewColumn = false;
}

MenuItem::~MenuItem()
{
	Log(enc("Destroying menu item %s..."), m_strPrivateName.c_str());
	for (MenuItem* e : m_Childs)
		delete e;
	m_Childs.clear();
}

// Helper function to order items
void MenuItem::OrderItems()
{
	// Set m_OrderedChilds to be the exact same as m_Childs
	m_OrderedChilds = m_Childs;
	// Reverse it!
	std::reverse(m_OrderedChilds.begin(), m_OrderedChilds.end());
	// Call it recursively for each valid child
	for (auto child : m_Childs)
	{
		if (child)
			child->OrderItems();
	}
}

bool call_once = true;
void MenuItem::Tick()
{
	if (m_iTick > 0) m_iTick--;
	bool has_childs = !m_Childs.empty();

	// Adjust m_iRealX based on m_iX and m_iOffsetX
	m_iRealX = m_iX + m_iOffsetX;
	m_iRealY = m_iY + m_iOffsetY;

	// If there is a parent item
	if (m_pParent != nullptr)
	{
		// This item is visible only if the parent is visible
		m_bVisible = m_pParent->IsVisible();
	}

	Think(); // Manage user gui input and shit like that
	Paint(); // Render everything

	// If this item has got childs
	if (has_childs)
	{
		// Order the childs so I can resolve the overlap shit problem
		if (call_once)
		{
			// The ordered childs vector will just contain the original m_Childs vector
			// BUT REVERSED! *zan zan zaaaaan*
			// So basically overlap resolves itself. You also have to call it recursively so it will
			// apply to every single child
			this->OrderItems();
			call_once = false;
		}
		// For each ordered child
		for (auto ochild : m_OrderedChilds)
		{
			// If it's a valid child
			if (ochild != nullptr)	
				ochild->Tick(); // Call this function for the child
		}
	}
}

void MenuItem::AddChild(MenuItem* child)
{
	// Add a child only if it's not already present in the vector of childs
	if (std::find(m_Childs.begin(), m_Childs.end(), child) != m_Childs.end())
		return;
	//m_Childs.push_back(child); <- moveax advice: Replaced push_back with emplace_back
	// Emplace back a child in the m_Childs vector
	m_Childs.emplace_back(child);
	// Set the child's m_pParent pointer to be this
	child->m_pParent = this;
	Log(enc("Pushed a child named %s inside %s->m_Childs [child = 0x%X]"), child->m_strPrivateName.c_str(), m_strPrivateName.c_str(), (DWORD)child);
}

void MenuItem::RemoveChild(MenuItem* child)
{
	// Remove a child only if it's present in the vector of childs
	auto it = std::find(m_Childs.begin(), m_Childs.end(), child); // Store where child is located
	if (it == m_Childs.end()) // If child is not present avoid deleting it
		return;
	m_Childs.erase(it); // Otherwise, just erase it from the vector
	Log(enc("Erased a child named %s from %s->m_Childs [child = 0x%X]"), child->m_strPrivateName.c_str(), m_strPrivateName.c_str(), (DWORD)child);
}

}
}

#endif