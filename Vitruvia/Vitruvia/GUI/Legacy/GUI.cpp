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

#include "GUI.h"

#if 0

namespace Vitruvia {
namespace GUI {

GUI_Menu* Menu = new GUI_Menu;

// This holds all the in game hitboxes
static const std::vector<std::string> HITBOXES
{
	enc("Head"),
	enc("Neck"),
	enc("Lower Neck"),
	enc("Pelvis"),
	enc("Body"),
	enc("Thorax"),
	enc("Chest"),
	enc("Upper Chest"),
	enc("Right Tigh"),
	enc("Left Tigh"),
	enc("Right Calf"),
	enc("Left Calf"),
	enc("Right Foot"),
	enc("Left Foot"),
	enc("Right Hand"),
	enc("Left Hand"),
	enc("Right Upper Arm"),
	enc("Right Forearm"),
	enc("Left Upper Arm"),
	enc("Left Forearm")
};
static const std::string DEFAULT_HITBOX = enc("Head");
static const int OFFSET = 5;
static const float SMOOTH_MIN = 0.0f;
static const float SMOOTH_MAX = 15.f;
static const float FOV_MIN = 0.1f;
static const float FOV_MAX = 15.f;
static const float TIME_MIN = 0.1f;
static const float TIME_MAX = 15.f;

GUI_Menu::~GUI_Menu()
{
	// Delete frames and let them do the cleanups
	delete m_pMainFrame;
}

void GUI_Menu::SetupMenu()
{
	using namespace Config;
	using Source::WeaponTypes;
	Log(enc("Starting the menu setup..."));

	m_pMainFrame = new Frame(enc("m_pMainFrame"), MENU_TITLE_STR);
	m_pMainFrame->SetSize(MENU_WIDTH, MENU_HEIGHT);
	m_pMainFrame->Center();

	auto tab_Legitbot = new Tab(enc("m_pMainFrame::tab_Legitbot"), enc("Legit"));
	tab_Legitbot->SetImageRenderer(new Render::ImageRenderer(IDR_LEGIT));

	#pragma region LEGITBOT

	auto gb_Aimbot = new GroupBox(enc("tab_Legitbot::gb_Aimbot"), Aimbot.NAME);
	gb_Aimbot->SetOffsetPosition(GROUPBOX_XSTART, GROUPBOX_YSTART);
	gb_Aimbot->SetWidth(GROUPBOX_WIDTH);
	tab_Legitbot->AddChild(gb_Aimbot);

	gb_Aimbot->AddChilds
	({
		new CheckBox(enc("tab_Legitbot::gb_Aimbot::cb_Active"),
		&Aimbot.Active.m_Value, Aimbot.Active.m_strName),

		new DropDown(enc("tab_Legitbot::gb_Aimbot::dd_Mode"),
		&Aimbot.Mode.m_Value, Aimbot.Mode.m_strName,
		{
			enc("On Fire"),
			enc("On Key"),
			enc("On Sight")
		},
			enc("On Key")),

		new DropDown(enc("tab_Legitbot::gb_Aimbot::dd_SortBy"),
			&Aimbot.SortBy.m_Value, Aimbot.SortBy.m_strName,
		{
			enc("Health"),
			enc("Distance"),
			enc("FOV"),
			enc("Threat")
		},
			enc("FOV")),

		new CheckBox(enc("tab_Legitbot::gb_Aimbot::cb_TeamMates"),
		&Aimbot.TeamMates.m_Value, Aimbot.TeamMates.m_strName),

		new CheckBox(enc("tab_Legitbot::gb_Aimbot::cb_VisibleCheck"),
		&Aimbot.VisibleCheck.m_Value, Aimbot.VisibleCheck.m_strName),

		new DropDown(enc("tab_Legitbot::gb_Aimbot::dd_Hitbox"),
		&Aimbot.Hitbox.m_Value, Aimbot.Hitbox.m_strName,
			HITBOXES, DEFAULT_HITBOX),

		new Slider(enc("tab_Legitbot::gb_Aimbot::sl_FOV"),
		&Aimbot.FOV.m_Value, Aimbot.FOV.m_strName,
			FOV_MIN, FOV_MAX),

		new Slider(enc("tab_Legitbot::gb_Aimbot::sl_Time"),
		&Aimbot.Time.m_Value, Aimbot.Time.m_strName,
			TIME_MIN, TIME_MAX, true),

		new CheckBox(enc("tab_Legitbot::gb_Aimbot::cb_SmokeCheck"),
		&Aimbot.SmokeCheck.m_Value, Aimbot.SmokeCheck.m_strName),

		new CheckBox(enc("tab_Legitbot::gb_Aimbot::cb_AwpBody"),
		&Aimbot.AwpBody.m_Value, Aimbot.AwpBody.m_strName),

		new CheckBox(enc("tab_Legitbot::gb_Aimbot::cb_SmartAim"),
		&Aimbot.SmartAim.m_Value, Aimbot.SmartAim.m_strName),

		new Slider(enc("tab_Legitbot::gb_Aimbot::sl_Smooth"),
			&Aimbot.Smooth.m_Value, Aimbot.Smooth.m_strName,
			SMOOTH_MIN, SMOOTH_MAX),

		new Slider(enc("tab_Legitbot::gb_Aimbot::sl_RCS"),
		&Aimbot.RCSFactor.m_Value, Aimbot.RCSFactor.m_strName, 0.f, 2.f),

		new CheckBox(enc("tab_Legitbot::gb_Aimbot::cb_Silent"),
		&Aimbot.Silent.m_Value, Aimbot.Silent.m_strName),

		new KeyReader(enc("tab_Legitbot::gb_Aimbot::kr_Key"),
			&Aimbot.AimKey.m_Value, Aimbot.AimKey.m_strName,
			Source::ButtonCode_t::MOUSE_5),

		new CheckBox(enc("tab_Legitbot::gb_Aimbot::cb_WeaponGroups"),
			&Aimbot.UseGroups.m_Value, Aimbot.UseGroups.m_strName)
	});

	auto gb_RCS = new GroupBox(enc("tab_Legitbot::gb_RCS"), RCS.NAME);
	gb_RCS->SetOffsetPosition(GROUPBOX_XSTART, GROUPBOX_YSTART);
	gb_RCS->SetWidth(GROUPBOX_WIDTH);
	tab_Legitbot->AddChild(gb_RCS);

	gb_RCS->AddChilds
	({
		new CheckBox(enc("tab_Legitbot::gb_RCS::cb_Active"),
		&RCS.Active.m_Value, RCS.Active.m_strName),

		new Slider(enc("tab_Legitbot::gb_RCS::sl_StartBullet"),
		&RCS.StartBullet.m_Value, RCS.StartBullet.m_strName,
			0.f, 10.f, true),

		new Slider(enc("tab_Legitbot::gb_RCS::sl_StopBullet"),
		&RCS.StopBullet.m_Value, RCS.StopBullet.m_strName,
			0.f, 10.f, true),

		new CheckBox(enc("tab_Legitbot::gb_RCS::cb_Smooth"),
		&RCS.Smooth.m_Value, RCS.Smooth.m_strName),
	});

	auto gb_Triggerbot = new GroupBox(enc("tab_Legitbot::gb_Triggerbot"), Trigger.NAME);
	gb_Triggerbot->SetOffsetPosition(GROUPBOX_XSTART + GROUPBOX_WIDTH + OFFSET, GROUPBOX_YSTART);
	gb_Triggerbot->SetWidth(GROUPBOX_WIDTH);
	gb_Triggerbot->SetNewColumn(true);

	gb_Triggerbot->AddChilds
	({
		new CheckBox(enc("tab_Legitbot::gb_Triggerbot::cb_Active"),
		&Trigger.Active.m_Value, Trigger.Active.m_strName),

		new CheckBox(enc("tab_Legitbot::gb_Triggerbot::cb_UseKey"),
		&Trigger.UseKey.m_Value, Trigger.UseKey.m_strName),

		new DropDown(enc("tab_Legitbot::gb_Triggerbot::dd_Hitgroup"),
		&Trigger.Hitgroup.m_Value, Trigger.Hitgroup.m_strName,
		{
			enc("No Filter"),
			enc("Head"),
			enc("Chest"),
			enc("Stomach"),
			enc("Left Arm"),
			enc("Right Arm"),
			enc("Left Leg"),
			enc("Right Leg")
		},
			enc("No Filter")),

		new Slider(enc("tab_Legitbot::gb_Triggerbot::sl_Hitchance"),
		&Trigger.Hitchance.m_Value, Trigger.Hitchance.m_strName, 0.f, 100.f),

		new KeyReader(enc("tab_Legitbot::gb_Triggerbot::kr_Key"),
		&Trigger.Key.m_Value, Trigger.Key.m_strName, Source::ButtonCode_t::MOUSE_4),

		new CheckBox(enc("tab_Legitbot::gb_Triggerbot::cb_SmokeCheck"),
		&Trigger.SmokeCheck.m_Value, Trigger.SmokeCheck.m_strName),

		new CheckBox(enc("tab_Legitbot::gb_Triggerbot::cb_ThroughWalls"),
		&Trigger.ThroughWalls.m_Value, Trigger.ThroughWalls.m_strName),

		new Slider(enc("tab_Legitbot::gb_Triggerbot::sl_MinWallDamage"),
		&Trigger.MinWallDamage.m_Value, Trigger.MinWallDamage.m_strName, 0.f, 120.f),
	});

	#pragma region WEAPON GROUPS

	// Quick lambda function to add common weapongroups items
	auto AddWeaponGroupChilds = [](GroupBox* gb, WeaponTypes weapon_type) -> void 
	{
		std::string full = gb->GetPrivateName();
		auto cfg = WeaponGroups[weapon_type];
		gb->AddChilds
		({
			new CheckBox(full + enc("::cb_Active"),
			&cfg->Active.m_Value, cfg->Active.m_strName),

			new DropDown(full + enc("::dd_Hitbox"),
			&cfg->Hitbox.m_Value, cfg->Hitbox.m_strName,
				HITBOXES, DEFAULT_HITBOX),

			new Slider(full + enc("::sl_FOV"),
			&cfg->FOV.m_Value, cfg->FOV.m_strName,
				FOV_MIN, FOV_MAX),

			new Slider(full + enc("::sl_Smooth"),
			&cfg->Smooth.m_Value, cfg->Smooth.m_strName,
				SMOOTH_MIN, SMOOTH_MAX),

			new Slider(full + enc("::sl_Time"),
			&cfg->Time.m_Value, cfg->Time.m_strName,
				TIME_MIN, TIME_MAX),
		});
	};

	auto gb_Rifle = new GroupBox(enc("tab_Legitbot::gb_Rifle"), WeaponGroups[WeaponTypes::WEAPONTYPE_RIFLE]->NAME);
	gb_Rifle->SetOffsetPosition(GROUPBOX_XSTART, GROUPBOX_YSTART);
	gb_Rifle->SetWidth(GROUPBOX_WIDTH);
	AddWeaponGroupChilds(gb_Rifle, WeaponTypes::WEAPONTYPE_RIFLE);
	tab_Legitbot->AddChild(gb_Rifle);

	auto gb_Pistol = new GroupBox(enc("tab_Legitbot::gb_Pistol"), WeaponGroups[WeaponTypes::WEAPONTYPE_PISTOL]->NAME);
	gb_Pistol->SetOffsetPosition(GROUPBOX_XSTART, GROUPBOX_YSTART);
	gb_Pistol->SetWidth(GROUPBOX_WIDTH);
	AddWeaponGroupChilds(gb_Pistol, WeaponTypes::WEAPONTYPE_PISTOL);
	tab_Legitbot->AddChild(gb_Pistol);

	tab_Legitbot->AddChild(gb_Triggerbot);

	auto gb_Sniper = new GroupBox(enc("tab_Legitbot::gb_Sniper"), WeaponGroups[WeaponTypes::WEAPONTYPE_SNIPER_RIFLE]->NAME);
	gb_Sniper->SetOffsetPosition(GROUPBOX_XSTART + GROUPBOX_WIDTH + OFFSET, GROUPBOX_YSTART);
	gb_Sniper->SetWidth(GROUPBOX_WIDTH);
	AddWeaponGroupChilds(gb_Sniper, WeaponTypes::WEAPONTYPE_SNIPER_RIFLE);
	tab_Legitbot->AddChild(gb_Sniper);

	auto gb_Shotgun = new GroupBox(enc("tab_Legitbot::gb_Shotgun"), WeaponGroups[WeaponTypes::WEAPONTYPE_SHOTGUN]->NAME);
	gb_Shotgun->SetOffsetPosition(GROUPBOX_XSTART + GROUPBOX_WIDTH + OFFSET, GROUPBOX_YSTART);
	gb_Shotgun->SetWidth(GROUPBOX_WIDTH);
	AddWeaponGroupChilds(gb_Shotgun, WeaponTypes::WEAPONTYPE_SHOTGUN);
	tab_Legitbot->AddChild(gb_Shotgun);

	auto gb_Smg = new GroupBox(enc("tab_Legitbot::gb_Smg"), WeaponGroups[WeaponTypes::WEAPONTYPE_SUBMACHINEGUN]->NAME);
	gb_Smg->SetOffsetPosition(GROUPBOX_XSTART + GROUPBOX_WIDTH + OFFSET, GROUPBOX_YSTART);
	gb_Smg->SetWidth(GROUPBOX_WIDTH);
	AddWeaponGroupChilds(gb_Smg, WeaponTypes::WEAPONTYPE_SUBMACHINEGUN);
	tab_Legitbot->AddChild(gb_Smg);

	auto gb_Heavy = new GroupBox(enc("tab_Legitbot::gb_Heavy"), WeaponGroups[WeaponTypes::WEAPONTYPE_MACHINEGUN]->NAME);
	gb_Heavy->SetOffsetPosition(GROUPBOX_XSTART + GROUPBOX_WIDTH + OFFSET, GROUPBOX_YSTART);
	gb_Heavy->SetWidth(GROUPBOX_WIDTH);
	AddWeaponGroupChilds(gb_Heavy, WeaponTypes::WEAPONTYPE_MACHINEGUN);
	tab_Legitbot->AddChild(gb_Heavy);
	#pragma endregion
	
	#pragma endregion

	m_pMainFrame->AddChild(tab_Legitbot);

	auto tab_Ragebot = new Tab(enc("m_pMainFrame::tab_Ragebot"), enc("Rage"));
	tab_Ragebot->SetImageRenderer(new Render::ImageRenderer(IDR_RAGE));

	#pragma region RAGE



	#pragma endregion

	m_pMainFrame->AddChild(tab_Ragebot);

	auto tab_Visuals = new Tab(enc("m_pMainFrame::tab_Visuals"), enc("Visuals"));
	tab_Visuals->SetImageRenderer(new Render::ImageRenderer(IDR_VISUALS));

	#pragma region VISUALS

	#pragma endregion

	m_pMainFrame->AddChild(tab_Visuals);

	auto tab_Misc = new Tab(enc("m_pMainFrame::tab_Misc"), enc("Misc"));
	tab_Misc->SetImageRenderer(new Render::ImageRenderer(IDR_MISC));

	#pragma region MISC

	#pragma endregion

	m_pMainFrame->AddChild(tab_Misc);

	auto tab_PlayerList = new Tab(enc("m_pMainFrame::tab_PlayerList"), enc("Players"));
	tab_PlayerList->SetImageRenderer(new Render::ImageRenderer(IDR_PLAYERS));

	#pragma region PLAYER LIST

	#pragma endregion

	m_pMainFrame->AddChild(tab_PlayerList);

	auto tab_Skins = new Tab(enc("m_pMainFrame::tab_Skins"), enc("Skins"));
	tab_Skins->SetImageRenderer(new Render::ImageRenderer(IDR_SKINS));

	#pragma region SKINS

	#pragma endregion

	m_pMainFrame->AddChild(tab_Skins);

	auto tab_Config = new Tab(enc("m_pMainFrame::tab_Config"), enc("Config"));
	tab_Config->SetImageRenderer(new Render::ImageRenderer(IDR_CONFIGS));

	#pragma region CONFIG

	#pragma endregion

	m_pMainFrame->AddChild(tab_Config);

	// BUILD THE MENU HERE!

	Log(enc("Done building the menu!"));
}

void GUI_Menu::RenderMouse()
{
	// Customize the mouse here
	static int mouse_size = 25;
	static Source::VGUI::Vertex_t mouse_shape[3];
	// de_stroyer HEHE :)
	POINT mouse_point = GUI_Input->GetMousePos();
	mouse_shape[0].Init(Source::Vector2(mouse_point.x, mouse_point.y));
	mouse_shape[1].Init(Source::Vector2(mouse_point.x + mouse_size, mouse_point.y + mouse_size / 2));
	mouse_shape[2].Init(Source::Vector2(mouse_point.x + mouse_size / 2, mouse_point.y + mouse_size));
	Render::Polygon(3, mouse_shape, Source::Color::White());
}

bool adjusted = false;
void GUI_Menu::Tick()
{
	GUI_Input->IsClicking();

	// Let the frames tick for themselves
	m_pMainFrame->Tick();

	// Adjust groupboxes
	if (!adjusted)
	{
		this->AdjustGroupboxes(m_pMainFrame);
		adjusted = true;
	}

	// Render the "mouse" used to navigate the menu
	if (m_pMainFrame->IsVisible())
		this->RenderMouse();
}

void GUI_Menu::AdjustGroupboxes(Frame* m_pFrame)
{
	// This function will adjust the offset y for the groupboxes currently inside m_pFrame

	// Get frame childs (tabs most of the time)
	auto childs = m_pFrame->GetChilds();
	// For each child
	for (auto c : childs)
	{
		// For each groupbox inside the child tab
		for (unsigned int i = 0; i < c->GetChilds().size(); ++i)
		{
			if (i != 0)
			{
				auto child = c->GetChilds()[i];
				if (child->GetType() == eMenuItem::GROUPBOX
					&& ((GroupBox*)child)->IsDynamic()
					&& !child->StartsNewColumn())
				{
					auto prev_child = c->GetChilds()[i - 1];
					child->SetOffsetY(prev_child->GetOffsetY() + prev_child->GetHeight() + OFFSET);
				}
			}
		}
	}
}

}
}

#endif