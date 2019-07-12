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

#pragma once

#if 0

#include "../GUI.h"

namespace Vitruvia {
namespace GUI {

// Handles the input related to the menu
class V_MenuInput
{
public:
	V_MenuInput(const std::string& window_name = enc("Valve001"));
	POINT GetMousePos();										// Returns X and Y coords of the mouse
	void IsClicking();											// Returns true whenever you are clicking and the active window is CS:GO
	bool IsHolding(int x, int y, int width, int height);		// Returns true if you are holding left click inside a defined rectangle
	bool IsHovering(int x, int y, int width, int height);		// Returns true if you are only hovering the mouse inside a defined rectangle
	bool Clicked(int x, int y, int width, int height);			// Returns true if you are clicking inside a defined rectangle
	// Getters / Setters
	HWND GetWindow() { return m_hWindow; }
	bool GetClick() { return m_bClicked; }
	void SetWindow(HWND hwnd) { m_hWindow = hwnd; }
	void SetClick(bool click) { m_bClicked = click; }
private:
	HWND m_hWindow;
	bool m_bClicked;
	bool m_bMouseController;
};
extern V_MenuInput* GUI_Input;

}
}

#endif