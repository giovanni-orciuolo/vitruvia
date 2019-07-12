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

#include "../../Config/Config.h"
#include "../../Renderer/Renderer.h"
#include "../../Globals/VitruviaGlobals.h"
#include "Input/MenuInput.h"

namespace Vitruvia {
namespace GUI {

static const int MENU_WIDTH = 640;
static const int MENU_HEIGHT = 634;
static const int MIN_GRAB_HEIGHT = 24;
static const int DROPDOWN_WIDTH = 100;
static const int DROPDOWN_HEIGHT = 17;
static const int KEYREADER_WIDTH = 100;
static const int KEYREADER_HEIGHT = 17;
static const int SLIDER_WIDTH = 100;
static const int SLIDER_HEIGHT = 8;
static const int GROUPBOX_WIDTH = 250;
static const int GROUPBOX_XSTART = 117;
static const int GROUPBOX_YSTART = 35;
static const int GROUPBOX_XSEPARATION = 15;
static const int GROUPBOX_YSEPARATION = 8;
static const int GROUPBOX_DEFAULT_HEIGHT = 12;
static const int TAB_EMPTY_OFFSET = 4;
static const int TAB_BAR_HEIGHT = 55;
static const int TAB_BAR_WIDTH = 7;
static const int TAB_WIDTH = 105;

static const Source::Color CLR_FRAME_MAIN = Source::Color(21, 21, 21);
static const Source::Color CLR_FRAME_UPPER_BAR = Source::Color(16, 16, 16);
static const Source::Color CLR_FRAME_UPPER_BAR_MINI = Source::Color(89, 126, 170);
static const Source::Color CLR_FRAME_TABS = Source::Color(18, 18, 18);
static const Source::Color CLR_FRAME_TABS_DIV = Source::Color(41, 41, 41);
static const Source::Color CLR_FRAME_TEXT = Source::Color::White();

static const Source::Color CLR_CHECKBOX_FULL = Source::Color(89, 126, 170);
static const Source::Color CLR_CHECKBOX_EMPTY = Source::Color(68, 68, 68);
static const Source::Color CLR_CHECKBOX_OUTLINE = Source::Color::Black();
static const Source::Color CLR_CHECKBOX_TEXT = Source::Color::White();

static const Source::Color CLR_DROPDOWN_MAIN = Source::Color(24, 24, 24);
static const Source::Color CLR_DROPDOWN_OUTLINE = Source::Color::Black();
static const Source::Color CLR_DROPDOWN_HOVERING = Source::Color(89, 126, 170);
static const Source::Color CLR_DROPDOWN_TEXT = Source::Color::White();

static const Source::Color CLR_KEYREADER_MAIN = Source::Color(24, 24, 24);
static const Source::Color CLR_KEYREADER_OUTLINE = Source::Color::Black();
static const Source::Color CLR_KEYREADER_TEXT = Source::Color::White();

static const Source::Color CLR_SLIDER_MAIN = Source::Color(68, 68, 68);
static const Source::Color CLR_SLIDER_OUTLINE = Source::Color::Black();
static const Source::Color CLR_SLIDER_FILL = Source::Color(89, 126, 170);
static const Source::Color CLR_SLIDER_TEXT = Source::Color::White();

static const Source::Color CLR_GROUPBOX_OUTLINE = Source::Color::Black();
static const Source::Color CLR_GROUPBOX_INNERLINE = Source::Color(46, 46, 46);
static const Source::Color CLR_GROUPBOX_COVER = CLR_FRAME_MAIN;
static const Source::Color CLR_GROUPBOX_TEXT = Source::Color::White();

static const Source::Color CLR_TAB_SELECTED = Source::Color(68, 190, 115);
static const Source::Color CLR_TAB_TEXT = Source::Color::White();

// ---- IMPORTANT
// Remember to update this enum each time you add a single MenuItem definition!!!
// ---- IMPORTANT
enum class eMenuItem
{
	NONE,
	FRAME,
	CHECKBOX,
	DROPDOWN,
	KEYREADER,
	SLIDER,
	GROUPBOX,
	TAB,

	NUM_MENU_ITEMS
};

// Every single menu item inherits from here
class MenuItem
{
public:
	MenuItem();									// Main constructor
	MenuItem(const std::string& priv_name, const std::string& title);
	virtual ~MenuItem();						// Destructor

	void Tick();								// Called each tick, manages shit and calls Think() & Paint()
	virtual void Think() {}						// Handles everything related to calcs, etc, just NO DRAWINGS HERE
	virtual void Paint() {}						// Handles everything related to drawings, so ALL DRAWINGS HERE

	// This method is used to determine if an item as priority for overlapping
	bool HasPriority()
	{
		// Edit this to edit priorities
		return GetType() ==
			eMenuItem::DROPDOWN;
	}
	// Should be called once recursively
	void OrderItems();

	// Getters
	std::string GetTitle() { return m_strTitle; }
	std::string GetPrivateName() { return m_strPrivateName; }
	int GetX() { return m_iX; }
	int GetY() { return m_iY; }
	void GetPosition(int& x, int& y) { x = m_iX; y = m_iY; }
	int GetOffsetX() { return m_iOffsetX; }
	int GetOffsetY() { return m_iOffsetY; }
	void GetOffsetPosition(int& x, int& y) { x = m_iOffsetX; y = m_iOffsetY; }
	int GetRealX() { return m_iRealX; }
	int GetRealY() { return m_iRealY; }
	void GetRealPosition(int& x, int& y) { x = m_iRealX; y = m_iRealY; }
	int GetTextX() { return m_iTextX; }
	int GetTextY() { return m_iTextY; }
	void GetTextPosition(int& x, int& y) { x = m_iTextX; y = m_iTextY; }
	int GetWidth() { return m_iWidth; }
	int GetHeight() { return m_iHeight; }
	int GetSize(int& w, int& h) { w = m_iWidth; h = m_iHeight; }
	int GetTab() { return m_iTab; }
	int GetTick() { return m_iTick; }
	bool IsVisible() { return m_bVisible; }
	MenuItem* GetParent() { return m_pParent; }
	std::vector<MenuItem*> GetChilds() { return m_Childs; }
	std::vector<MenuItem*> GetOrderedChilds() { return m_OrderedChilds; }
	eMenuItem GetType() { return m_eType; }
	bool StartsNewColumn() { return m_bNewColumn; }

	// Setters
	void SetTitle(const std::string& title) { m_strTitle = title; }
	void SetPrivateName(const std::string& name) { m_strPrivateName = name; }
	void SetX(int x) { m_iX = x; }
	void SetY(int y) { m_iY = y; }
	void SetPosition(int x, int y) { m_iX = x; m_iY = y; }
	void SetOffsetX(int x) { m_iOffsetX = x; }
	void SetOffsetY(int y) { m_iOffsetY = y; }
	void SetOffsetPosition(int x, int y) { m_iOffsetX = x; m_iOffsetY = y; }
	void SetRealX(int x) { m_iRealX = x; }
	void SetRealY(int y) { m_iRealY = y; }
	void SetRealPosition(int x, int y) { m_iRealX = x; m_iRealY = y; }
	void SetTextX(int x) { m_iTextX = x; }
	void SetTextY(int y) { m_iTextY = y; }
	void SetTextPosition(int x, int y) { m_iTextX = x; m_iTextY = y; }
	void SetWidth(int w) { m_iWidth = w; }
	void SetHeight(int h) { m_iHeight = h; }
	void SetSize(int w, int h) { m_iWidth = w; m_iHeight = h; }
	void SetTab(int tab) { m_iTab = tab; }
	void SetTick(int tick) { m_iTick = tick; }
	void SetVisible(bool visible) { m_bVisible = visible; }
	void SetParent(MenuItem* parent) { m_pParent = parent; }
	void SetType(eMenuItem type) { m_eType = type; }
	void SetNewColumn(bool new_column) { m_bNewColumn = new_column; }

	// Add / Remove childs
	void AddChild(MenuItem* child);
	void RemoveChild(MenuItem* child);

protected:
	std::string m_strTitle;					// Title of the item
	std::string m_strPrivateName;			// Internal name of the item, used to log
	int m_iX;								// X of the item
	int m_iY;								// Y of the item
	int m_iOffsetX;							// Offset X of the item
	int m_iOffsetY;							// Offset Y of the item
	int m_iRealX;							// Real X of the item (x + offset x)
	int m_iRealY;							// Real Y of the item (y + offset y)
	int m_iTextX;							// X of the text for the item
	int m_iTextY;							// Y of the text for the item
	int m_iWidth;							// Width of the item
	int m_iHeight;							// Height of the item
	int m_iTab;								// Tab index of the item
	int m_iTick;							// Tick counter of the item
	bool m_bVisible;						// True if visible
	MenuItem* m_pParent;					// Pointer to parent item
	std::vector<MenuItem*> m_Childs;		// Vector containing all the childs of the item
	std::vector<MenuItem*> m_OrderedChilds; // Vector containing all the childs ordered by overlapping
	eMenuItem m_eType;						// Describes the type of the menu item
	bool m_bNewColumn;						// True if item starts a new column
};

// A frame is a standalone window that can be moved around
class Frame : public MenuItem
{
public:
	Frame(const std::string& priv_name, const std::string& title);
	void Think();
	void Paint();
	void Center();
protected:
	Source::ConVar* m_pMouseEnable;
};

// A checkbox can be checked on and off to choose whenever something should be active (true) or not (false)
class CheckBox : public MenuItem
{
public:
	CheckBox(const std::string& priv_name, bool* ptr, const std::string& title)
		: MenuItem(priv_name, title), m_bValue(ptr)
	{
		// All the checkboxes have a fixed size of 12x12 pixels
		this->SetSize(12, 12);
		this->m_eType = eMenuItem::CHECKBOX;
	}
	void Think();
	void Paint();
	void SetValue(bool* ptr) { m_bValue = ptr; }
protected:
	bool* m_bValue;	// A pointer to the bool value being modified. This MUST be a bool!
};

// A dropdown can be clicked to display a fixed list of items to choose from
class DropDown : public MenuItem
{
public:
	DropDown(const std::string& priv_name, int* ptr, const std::string& title, const std::vector<std::string>& items, const std::string& current_item, int width = 100, int height = 17)
		: MenuItem(priv_name, title),
		m_iValue(ptr),
		m_bIsOpened(false) 
	{
		DropDown::isDropDownOpened = false;
		DropDown::pOpenedDropDown = nullptr;
		for (auto& i : items)
			*this += i;
		this->SetCurrentItem(current_item);
		this->SetSize(width, height);
		this->m_eType = eMenuItem::DROPDOWN;
	}
	~DropDown() { MenuItem::~MenuItem(); m_Items.clear(); }
	void Think();
	void Paint();
	void SetValue(int* ptr) { m_iValue = ptr; }

	DropDown& operator+=(const std::string& item);
	void SetCurrentItem(const std::string& item);

	std::vector<std::string> GetItems() { return m_Items; }
	std::string GetCurrentItem() { return m_strCurrentItem; }
	bool IsOpened() { return m_bIsOpened; }

	static bool IsDropDownOpened() { return DropDown::isDropDownOpened; }
	static DropDown* OpenedDropDown() { return DropDown::pOpenedDropDown; }
protected:
	int* m_iValue; // A pointer to the int value being modified. This MUST be a int and it will hold the current selected index
	std::vector<std::string> m_Items; // This vector will hold the fixed list of items
	std::string m_strCurrentItem;  // This string holds the current selected item string
	bool m_bIsOpened; // True if the dropdown is opened
	static bool isDropDownOpened; // Used to track if any dropdown is opened
	static DropDown* pOpenedDropDown; // Pointer to the currently opened dropdown, nullptr if no dropdown is opened
};

// A keyreader can be used to read a key from keyboard to assign to a specific variable
class KeyReader : public MenuItem
{
public:
	KeyReader(const std::string& priv_name, int* ptr, const std::string& title, int key, int width = 100, int height = 17)
		: MenuItem(priv_name, title),
		m_kValue(ptr),
		m_bWaiting(false) 
	{
		this->SetSize(width, height);
		*m_kValue = key;
		this->m_eType = eMenuItem::KEYREADER;
	}
	void Think();
	void Paint();
	void SetValue(int* ptr) { m_kValue = ptr; }
	bool IsWaitingKey() { return isWaitingKey; }
protected:
	int* m_kValue; // A pointer to the key being modified. MUST be an int
	bool m_bWaiting; // True when the reader is waiting for user input
	static bool isWaitingKey; // Used to track down if any keyreader is waiting for input
};

// A slider can be used to manage a precise value (usually a float)
class Slider : public MenuItem
{
public:
	Slider(const std::string& priv_name, float* ptr, const std::string& title, float min, float max, bool is_int = false, int width = 100, int height = 7)
		: MenuItem(priv_name, title), m_flValue(ptr), m_flMin(min), m_flMax(max)
	{
		this->m_bIsInt = is_int;
		this->SetSize(width, height);
		this->m_eType = eMenuItem::SLIDER;
	}
	void Think();
	void Paint();

	float GetMin() { return m_flMin; }
	float GetMax() { return m_flMax; }
	void GetBounds(float& min, float& max) { min = m_flMin, max = m_flMax; }
	bool IsGrabbed() { return Slider::isGrabbed; }

	void SetValue(float* ptr) { m_flValue = ptr; }
	void SetMin(float min) { m_flMin = min; }
	void SetMax(float max) { m_flMax = max; }
	void SetBounds(float min, float max) { m_flMin = min; m_flMax = max; }
protected:
	float m_flMin, m_flMax; // Bounds
	float* m_flValue; // Pointer to the float value being modified by the slider. MUST be a float
	bool m_bIsInt; // True if the value managed is actually used as an int through the feature
	static bool isGrabbed; // Used to track down if any slider is being grabbed xd
};

// A groupbox is a container that contains menu items in a fixed order with proper style
// It should be possible to dynamically add elements to the groupbox without hardcoding coords, using the child system
class GroupBox : public MenuItem
{
public:
	GroupBox(const std::string& priv_name, const std::string& title, bool enable_dynamic = true)
		: MenuItem(priv_name, title) 
	{
		this->m_iHeight = GROUPBOX_DEFAULT_HEIGHT; // Default height
		this->m_bEnableDynamic = enable_dynamic;
		this->m_eType = eMenuItem::GROUPBOX;
		this->m_bDidSet = false;
	}
	void Think();
	void Paint();
	void AddChilds(const std::vector<MenuItem*>& menu_items);
	bool IsDynamic() { return m_bEnableDynamic; }
private:
	bool m_bEnableDynamic; // True if you want dynamic height, false if you don't (default: true ofc)
	bool m_bDidSet; // True if we are done setting dynamic stuff for this control
};

// A tab is a specific container that is used to divide different sections of the menu.
class Tab : public MenuItem
{
public:
	Tab(const std::string& priv_name, const std::string& title, int width = TAB_WIDTH, 
		int height = TAB_BAR_HEIGHT + TAB_EMPTY_OFFSET * 2)
		: MenuItem(priv_name, title)
	{
		this->SetSize(width, height);
		this->m_eType = eMenuItem::TAB;
	}
	~Tab() { delete m_pImageRenderer; }
	void Think();
	void Paint();
	Render::ImageRenderer* GetImageRenderer() { return this->m_pImageRenderer; }
	void SetImageRenderer(Render::ImageRenderer* image_renderer) { this->m_pImageRenderer = image_renderer; }
	int GetTabHovering() { return Tab::s_iTabHovering; }
	void SetTabHovering(int tab_index) { Tab::s_iTabHovering = tab_index; }
private:
	Render::ImageRenderer* m_pImageRenderer; // Used to render the icon for the tab
	static int s_iTabHovering; // Used to track which tab I'm currently hovering
};

// The final menu
class GUI_Menu
{
public:
	~GUI_Menu();
	void SetupMenu();
	void Tick();
	void AdjustGroupboxes(Frame* m_pFrame);
	Frame* GetMainFrame() { return m_pMainFrame; }
protected:
	void RenderMouse();
	Frame* m_pMainFrame; // The frame where everything starts from, the main one
};
extern GUI_Menu* Menu;

}
}

#endif