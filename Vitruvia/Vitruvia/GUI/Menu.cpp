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

#include "Menu.h"
#include "../Features/Misc/Misc.h"
#include "../Utils/GameHelper/GameHelper.h"

#define CFG(name) name##.m_strName.c_str(), &name##.m_Value

// Export menu related globals
bool g_bMenuOpened = false;
bool g_bMenuClosing = true;
bool g_bMenuInitialized = false;
bool g_bWallColorModified = false;
bool k_shouldListen = false;
int* k_keyOutput = nullptr;

// Dropdown items (mind the dd_ prefix)
static std::vector<std::string> dd_Hitboxes
{
	enc("Head"),
	enc("Neck"),
	enc("Lower Neck"),
	enc("Pelvis"),
	enc("Body"),
	enc("Thorax"),
	enc("Chest"),
	enc("Upper Chest"),
	enc("Right Thigh"),
	enc("Left Thigh"),
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
static std::vector<std::string> dd_Hitgroups
{
	enc("All"),
	enc("Head"),
	enc("Chest"),
	enc("Stomach"),
	enc("Left Arm"),
	enc("Right Arm"),
	enc("Left Leg"),
	enc("Right Leg")
};
static std::vector<std::string> dd_AimModes
{
	enc("On Fire"),
	enc("On Key"),
	enc("On Sight"),
	enc("Never")
};
static std::vector<std::string> dd_SortBy
{
	enc("Health"),
	enc("Distance"),
	enc("FOV"),
	enc("Threat")
};
static std::vector<std::string> dd_AA_Pitch
{
	enc("None"),
	enc("Emotion"),
	enc("Up"),
	enc("Down"),
	enc("Overflow Down"),
	enc("Overflow Center"),
	enc("Random"),
	enc("Custom")
};
static std::vector<std::string> dd_AA_Yaw
{
	enc("None"),
	enc("Backwards"),
	enc("Jitter"),
	enc("Backwards Jitter"),
	enc("Sideways"),
	enc("Fast Spin"),
	enc("Slow Spin"),
	enc("Custom Spin"),
	enc("Real"),
	enc("Fake Lower Body"),
	enc("Paranoia"),
	enc("Mirror"),
	enc("SMART"),
};
static std::vector<std::string> dd_ResolverTypes
{
	enc("Smart-YAW"),
};
static std::vector<std::string> dd_FakeLagModes
{
	enc("Static"),
	enc("Random Factor"),
	enc("Adaptive")
};
static std::vector<std::string> dd_BoxTypes
{
	enc("Full"),
	enc("Corner"),
	enc("3D")
};
static std::vector<std::string> dd_SnapLines
{
	enc("None"),
	enc("Bottom"),
	enc("Center"),
	enc("Up"),
	enc("Left"),
	enc("Right")
};
static std::vector<std::string> dd_ESP_Hitboxes
{
	enc("None"),
	enc("All"),
	enc("Head"),
	enc("Neck"),
	enc("Lower Neck"),
	enc("Pelvis"),
	enc("Body"),
	enc("Thorax"),
	enc("Chest"),
	enc("Upper Chest"),
	enc("Right Thigh"),
	enc("Left Thigh"),
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
static std::vector<std::string> dd_ChamsTypes
{
	enc("None"),
	enc("Textured"),
	enc("Flat"),
	enc("Wireframe"),
	enc("Full Model"),
	enc("Glass"),
	enc("Platinum"),
	enc("Dark Chrome"),
	enc("Crystal"),
	enc("Blue Crystal"),
	enc("Gold"),
	enc("Glow"),
	enc("Plastic"),
	enc("Strings")
};
static std::vector<std::string> dd_ClanTags
{
	enc("None"),
	enc("vitruvia.me $"),
	enc("vitruvia777 $"),
	enc("Time"),
	enc("ValvE"),
	enc("Animated Vitruvia"),
	enc("Animated Vitruvia 2"),
	enc("Animated Vitruvia 3")
};
static std::vector<std::string> dd_AutoStrafeTypes
{
	enc("Legit"),
	enc("Max Speed (NOT LEGIT)")
};
static std::vector<std::string> dd_Colors
{
	enc("Box Team"),
	enc("Box Enemy"),
	enc("Skeleton Team"),
	enc("Skeleton Enemy"),
	enc("Hitbox Team"),
	enc("Hitbox Enemy"),
	enc("Glow Team"),
	enc("Glow Enemy"),
	enc("Watermark"),
	enc("Chams Team"),
	enc("Chams Enemy"),
	enc("Chams Team XQZ"),
	enc("Chams Enemy XQZ"),
	enc("Hand Chams"),
	enc("Weapon Chams"),
	enc("Crosshair"),
	enc("Radar Boxes"),
	enc("Menu Text"),
	enc("Walls")
};
static std::vector<std::string> dd_OverheadTypes
{
	enc("None"),
	enc("Name"),
	enc("Name and Weapon")
};
static std::vector<std::string> dd_Crosshairs
{
	enc("Static"),
	enc("Static Swastika"),
	enc("Rotating Swastika")
};
static std::vector<std::string> dd_RenderModes
{
	enc("Default"),
	enc("Full Gray"),
	enc("Full Bright"),
	enc("Minecraft"),
	enc("Rainbow MLG")
};
static std::vector<std::string> dd_BeamTypes
{
	enc("Physic Beam"), // "sprites/physbeam.vmt"
	enc("Glow Beam")	// "sprites/glow01.vmt"
};

static std::array<ImVec4, 43> style_colors{};

// Extend ImGui
namespace ImGui
{
	void KeyBindButton(const char* label, int* key, ImVec2 size = ImVec2(150, 0))
	{
		using Source::ButtonCode_t;
		const char* text = Source::InputSystem->ButtonCodeToString(ButtonCode_t(*key));

		if (k_shouldListen && key == k_keyOutput)
			text = enc("-- Press a key --");
		//else
		//	text = ToUpper(std::string(text)).c_str();

		if (ImGui::Button(text, size))
		{
			k_shouldListen = true;
			k_keyOutput = key;
		}
		ImGui::SameLine();
		ImGui::Text(label);
	}

	static auto vector_getter = [](void* vec, int idx, const char** out_text)
	{
		auto& vector = *static_cast<std::vector<std::string>*>(vec);
		if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
		*out_text = vector.at(idx).c_str();
		return true;
	};

	bool Combo(const char* label, int* currIndex, std::vector<std::string>& values, int height_in_items = -1)
	{
		if (values.empty()) { return false; }
		return Combo(label, currIndex, vector_getter,
			static_cast<void*>(&values), values.size(), height_in_items);
	}

	bool ListBox(const char* label, int* currIndex, std::vector<std::string>& values, int height_in_items = -1)
	{
		if (values.empty()) { return false; }
		return ListBox(label, currIndex, vector_getter,
			static_cast<void*>(&values), values.size(), height_in_items);
	}

	bool ColorPicker(Source::Color* my_color, bool alphabar = false)
	{
		Source::FloatColor fl_color = my_color->toFloatColor();
		float col[4];
		col[0] = fl_color.r();
		col[1] = fl_color.g();
		col[2] = fl_color.b();
		col[3] = fl_color.a();
		const int EDGE_SIZE = 200; // = int(ImGui::GetWindowWidth() * 0.75f);
		const ImVec2 SV_PICKER_SIZE = ImVec2(EDGE_SIZE, EDGE_SIZE);
		const float SPACING = ImGui::GetStyle().ItemInnerSpacing.x;
		const float HUE_PICKER_WIDTH = 20.f;
		const float CROSSHAIR_SIZE = 7.0f;

		ImColor color(col[0], col[1], col[2]);
		bool value_changed = false;

		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		ImVec2 picker_pos = ImGui::GetCursorScreenPos();

		float hue, saturation, value;
		ImGui::ColorConvertRGBtoHSV(color.Value.x, color.Value.y, color.Value.z, hue, saturation, value);

		ImColor colors[] = {
			ImColor(255, 0, 0),
			ImColor(255, 255, 0),
			ImColor(0, 255, 0),
			ImColor(0, 255, 255),
			ImColor(0, 0, 255),
			ImColor(255, 0, 255),
			ImColor(255, 0, 0)
		};

		for (int i = 0; i < 6; i++)
		{
			draw_list->AddRectFilledMultiColor(
				ImVec2(picker_pos.x + SV_PICKER_SIZE.x + SPACING, picker_pos.y + i * (SV_PICKER_SIZE.y / 6)),
				ImVec2(picker_pos.x + SV_PICKER_SIZE.x + SPACING + HUE_PICKER_WIDTH,
					picker_pos.y + (i + 1) * (SV_PICKER_SIZE.y / 6)),
				colors[i],
				colors[i],
				colors[i + 1],
				colors[i + 1]
			);
		}

		draw_list->AddLine(
			ImVec2(picker_pos.x + SV_PICKER_SIZE.x + SPACING - 2, picker_pos.y + hue * SV_PICKER_SIZE.y),
			ImVec2(picker_pos.x + SV_PICKER_SIZE.x + SPACING + 2 + HUE_PICKER_WIDTH, picker_pos.y + hue * SV_PICKER_SIZE.y),
			ImColor(255, 255, 255)
		);

		if (alphabar)
		{
			float alpha = col[3];

			draw_list->AddRectFilledMultiColor(
				ImVec2(picker_pos.x + SV_PICKER_SIZE.x + 2 * SPACING + HUE_PICKER_WIDTH, picker_pos.y),
				ImVec2(picker_pos.x + SV_PICKER_SIZE.x + 2 * SPACING + 2 * HUE_PICKER_WIDTH, picker_pos.y + SV_PICKER_SIZE.y),
				ImColor(0, 0, 0), ImColor(0, 0, 0), ImColor(255, 255, 255), ImColor(255, 255, 255)
			);

			draw_list->AddLine(
				ImVec2(picker_pos.x + SV_PICKER_SIZE.x + 2 * (SPACING - 2) + HUE_PICKER_WIDTH, picker_pos.y + alpha * SV_PICKER_SIZE.y),
				ImVec2(picker_pos.x + SV_PICKER_SIZE.x + 2 * (SPACING + 2) + 2 * HUE_PICKER_WIDTH, picker_pos.y + alpha * SV_PICKER_SIZE.y),
				ImColor(255.f - alpha, 255.f, 255.f)
			);
		}

		const ImU32 c_oColorBlack = ImGui::ColorConvertFloat4ToU32(ImVec4(0.f, 0.f, 0.f, 1.f));
		const ImU32 c_oColorBlackTransparent = ImGui::ColorConvertFloat4ToU32(ImVec4(0.f, 0.f, 0.f, 0.f));
		const ImU32 c_oColorWhite = ImGui::ColorConvertFloat4ToU32(ImVec4(1.f, 1.f, 1.f, 1.f));

		ImVec4 cHueValue(1, 1, 1, 1);
		ImGui::ColorConvertHSVtoRGB(hue, 1, 1, cHueValue.x, cHueValue.y, cHueValue.z);
		ImU32 oHueColor = ImGui::ColorConvertFloat4ToU32(cHueValue);

		draw_list->AddRectFilledMultiColor(
			ImVec2(picker_pos.x, picker_pos.y),
			ImVec2(picker_pos.x + SV_PICKER_SIZE.x, picker_pos.y + SV_PICKER_SIZE.y),
			c_oColorWhite,
			oHueColor,
			oHueColor,
			c_oColorWhite
		);

		draw_list->AddRectFilledMultiColor(
			ImVec2(picker_pos.x, picker_pos.y),
			ImVec2(picker_pos.x + SV_PICKER_SIZE.x, picker_pos.y + SV_PICKER_SIZE.y),
			c_oColorBlackTransparent,
			c_oColorBlackTransparent,
			c_oColorBlack,
			c_oColorBlack
		);

		float x = saturation * SV_PICKER_SIZE.x;
		float y = (1 - value) * SV_PICKER_SIZE.y;
		ImVec2 p(picker_pos.x + x, picker_pos.y + y);
		draw_list->AddLine(ImVec2(p.x - CROSSHAIR_SIZE, p.y), ImVec2(p.x - 2, p.y), ImColor(255, 255, 255));
		draw_list->AddLine(ImVec2(p.x + CROSSHAIR_SIZE, p.y), ImVec2(p.x + 2, p.y), ImColor(255, 255, 255));
		draw_list->AddLine(ImVec2(p.x, p.y + CROSSHAIR_SIZE), ImVec2(p.x, p.y + 2), ImColor(255, 255, 255));
		draw_list->AddLine(ImVec2(p.x, p.y - CROSSHAIR_SIZE), ImVec2(p.x, p.y - 2), ImColor(255, 255, 255));

		ImGui::InvisibleButton("saturation_value_selector", SV_PICKER_SIZE);

		if (ImGui::IsItemActive() && ImGui::GetIO().MouseDown[0])
		{
			ImVec2 mouse_pos_in_canvas = ImVec2(ImGui::GetIO().MousePos.x - picker_pos.x, ImGui::GetIO().MousePos.y - picker_pos.y);

			if (mouse_pos_in_canvas.x < 0)
				mouse_pos_in_canvas.x = 0;
			else if (mouse_pos_in_canvas.x >= SV_PICKER_SIZE.x - 1)
				mouse_pos_in_canvas.x = SV_PICKER_SIZE.x - 1;

			if (mouse_pos_in_canvas.y < 0)
				mouse_pos_in_canvas.y = 0;
			else if (mouse_pos_in_canvas.y >= SV_PICKER_SIZE.y - 1)
				mouse_pos_in_canvas.y = SV_PICKER_SIZE.y - 1;

			value = 1 - (mouse_pos_in_canvas.y / (SV_PICKER_SIZE.y - 1));
			saturation = mouse_pos_in_canvas.x / (SV_PICKER_SIZE.x - 1);
			value_changed = true;
		}

		// hue bar logic
		ImGui::SetCursorScreenPos(ImVec2(picker_pos.x + SPACING + SV_PICKER_SIZE.x, picker_pos.y));
		ImGui::InvisibleButton("hue_selector", ImVec2(HUE_PICKER_WIDTH, SV_PICKER_SIZE.y));

		if (ImGui::GetIO().MouseDown[0] && (ImGui::IsItemHovered() || ImGui::IsItemActive()))
		{
			ImVec2 mouse_pos_in_canvas = ImVec2(ImGui::GetIO().MousePos.x - picker_pos.x, ImGui::GetIO().MousePos.y - picker_pos.y);

			if (mouse_pos_in_canvas.y < 0)
				mouse_pos_in_canvas.y = 0;
			else if (mouse_pos_in_canvas.y >= SV_PICKER_SIZE.y - 1)
				mouse_pos_in_canvas.y = SV_PICKER_SIZE.y - 1;

			hue = mouse_pos_in_canvas.y / (SV_PICKER_SIZE.y - 1);
			value_changed = true;
		}

		if (alphabar)
		{
			ImGui::SetCursorScreenPos(ImVec2(picker_pos.x + SPACING * 2 + HUE_PICKER_WIDTH + SV_PICKER_SIZE.x, picker_pos.y));
			ImGui::InvisibleButton("alpha_selector", ImVec2(HUE_PICKER_WIDTH, SV_PICKER_SIZE.y));

			if (ImGui::GetIO().MouseDown[0] && (ImGui::IsItemHovered() || ImGui::IsItemActive()))
			{
				ImVec2 mouse_pos_in_canvas = ImVec2(ImGui::GetIO().MousePos.x - picker_pos.x, ImGui::GetIO().MousePos.y - picker_pos.y);

				if (mouse_pos_in_canvas.y < 0)
					mouse_pos_in_canvas.y = 0;
				else if (mouse_pos_in_canvas.y >= SV_PICKER_SIZE.y - 1)
					mouse_pos_in_canvas.y = SV_PICKER_SIZE.y - 1;

				float alpha = mouse_pos_in_canvas.y / (SV_PICKER_SIZE.y - 1);
				col[3] = alpha;
				value_changed = true;
			}
		}

		color = ImColor::HSV(hue >= 1 ? hue - 10 * 1e-6 : hue, saturation > 0 ? saturation : 10 * 1e-6, value > 0 ? value : 1e-6);
		col[0] = color.Value.x;
		col[1] = color.Value.y;
		col[2] = color.Value.z;

		bool widget_used;
		ImGui::PushItemWidth((alphabar ? SPACING + HUE_PICKER_WIDTH : 0) + SV_PICKER_SIZE.x + SPACING + HUE_PICKER_WIDTH - 2 * ImGui::GetStyle().FramePadding.x);
		widget_used = alphabar ? ImGui::ColorEdit4("", col) : ImGui::ColorEdit3("", col);
		ImGui::PopItemWidth();

		float new_hue, new_sat, new_val;
		ImGui::ColorConvertRGBtoHSV(col[0], col[1], col[2], new_hue, new_sat, new_val);

		if (new_hue <= 0 && hue > 0)
		{
			if (new_val <= 0 && value != new_val)
			{
				color = ImColor::HSV(hue, saturation, new_val <= 0 ? value * 0.5f : new_val);
				col[0] = color.Value.x;
				col[1] = color.Value.y;
				col[2] = color.Value.z;
			}
			else if (new_sat <= 0)
			{
				color = ImColor::HSV(hue, new_sat <= 0 ? saturation * 0.5f : new_sat, new_val);
				col[0] = color.Value.x;
				col[1] = color.Value.y;
				col[2] = color.Value.z;
			}
		}

		fl_color.SetColor(col[0], col[1], col[2], col[3]);
		my_color->SetColor(int(fl_color.r() * 255.f), int(fl_color.g() * 255.f), int(fl_color.b() * 255.f), int(fl_color.a() * 255.f));

		return value_changed | widget_used;
	}

	bool ListBox(const char* label, int* current_item, std::function<const char*(int)> lambda, int items_count, int height_in_items)
	{
		return ImGui::ListBox(label, current_item, [](void* data, int idx, const char** out_text)
		{
			*out_text = (*reinterpret_cast<std::function<const char*(int)>*>(data))(idx);
			return true;
		}, &lambda, items_count, height_in_items);
	}

	bool Combo(const char* label, int* current_item, std::function<bool(void*,int,const char**)> lambda, int items_count, int height_in_items)
	{
		return ImGui::Combo(label, current_item, [](void* data, int idx, const char** out_text)
		{
			*out_text = (*reinterpret_cast<std::function<const char*(int)>*>(data))(idx);
			return true;
		}, &lambda, items_count, height_in_items);
	}
}

// Quick function to get files in folder (used to grab configs). I'll move it to VitruviaUtils, one day
std::vector<std::string> GetFilesInFolder(const std::string& folder)
{
	std::vector<std::string> names;
	std::string search_path = folder + "/*.*";
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				names.push_back(fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return names;
}

namespace Vitruvia {
namespace GUI {

void InitMenu(IDirect3DDevice9* device)
{
	//Log(enc("InitMenu() with device: 0x%X"), rcast<DWORD>(device));
	ImGui_ImplDX9_Init(global->CSGO_Window(), device);
	Vitruvia::Render::D3D::g_pRenderer = new Vitruvia::Render::D3D::DrawManager(device);
	Vitruvia::Render::D3D::g_pRenderer->CreateObjects();
}

void RenderMenu(IDirect3DDevice9* device)
{
	using namespace Vitruvia;
	using namespace Source;
	using namespace Config;

	// If menu is not closing it means it is opened

	// Selectable bool storage
	static bool legit = false;
	static bool trigger = false;
	static bool rage = false;
	static bool hvh = false;
	static bool visuals = false;
	static bool misc = false;
	static bool colors = false;
	static bool skins = false;
	static bool styles = false;
	static bool settings = false;

	// Internal statics
	static char cfg_buffer[20]{};
	static int cfg_number = -1;
	static bool rage_settings = false;
	static bool rage_accuracy = false;
	static bool rage_target = false;
	static bool hvh_antiaim = false;
	static bool hvh_resolver = false;
	static bool hvh_playerlist = false;
	static bool hvh_fakelag = false;
	static bool visuals_esp = false;
	static bool visuals_filters = false;
	static bool visuals_removals = false;
	static bool visuals_chams = false;
	static bool visuals_other = false;
	static bool misc_notifications = false;
	static int color_selected = 0;
	static int childs = 0;
	static int paintkit_weapon = 0;
	static int knife = 0;
	static int glove = 0;
	static char buffer_skinfilter[989]{};
	static char buffer_stickerfilter[989]{};

	// Opacity should reach 1 in MenuFadeSpeed.value second(s).
	float secs = Misc.MenuFadeSpeed.m_Value;
	secs = clamp<float, float>(secs, 0.1f, 2.f);
	float alpha_frequency = 1.f / Misc.MenuFadeSpeed.m_Value;
	static float menu_alpha = 0.f;

	// Get I/O gui handle
	auto* io = &ImGui::GetIO();

	// Draw the cursor only when the menu is opened
	io->MouseDrawCursor = g_bMenuOpened;

	// Begin a new ImGui frame
	ImGui_ImplDX9_NewFrame();

	// Store colors
	Source::FloatColor menu = Config::Colors.MenuText.m_Value.toFloatColor();
	// Modify menu alpha (from 0 to 1 if I'm opening. From 1 to 0 if I'm closing)
	float step = alpha_frequency * Source::Globals->frame_time;
	(!g_bMenuClosing) ? menu_alpha += step : menu_alpha -= step;
	menu_alpha = clamp<float, float>(menu_alpha, 0.f, 1.f);

	// Get style pointer
	ImGuiStyle* style = &ImGui::GetStyle();
	// For each color 
	for (int i = 0; i < 43; i++)
	{
		// Apply menu_alpha
		float max = style_colors[i].w;
		style->Colors[i].w = menu_alpha;
		style->Colors[i].w = clamp<float, float>(style->Colors[i].w, 0.f, max);
	}
	// Apply menu alpha on menu_color too!
	menu.SetAlpha(menu_alpha);
	ImVec4 menu_color(menu.r(), menu.g(), menu.b(), menu.a());

	// If menu alpha is <= 0, menu is not opened anymore
	if (menu_alpha <= 0 && g_bMenuOpened) 
		g_bMenuOpened = false;

	if (menu_alpha > 0)
	{
		// Begin main window code
		ImGui::Begin(enc("Vitruvia"), &g_bMenuOpened, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize );
		{
			ImGui::TextColored(menu_color, enc("%s - CS:GO Edition"), CHEAT_NAME);
			ImGui::PushFont(io->Fonts->Fonts[2]);
			ImGui::Text(enc("The ends justify the means."));
			ImGui::Text(enc("Build Date: %s"), __DATE__);
			ImGui::PopFont();

			ImGui::Separator();

			ImGui::PushFont(io->Fonts->Fonts[1]);
			ImGui::Selectable(enc("Legitbot"), &legit);
			ImGui::Selectable(enc("Ragebot"), &rage);
			ImGui::Selectable(enc("Hack vs Hack"), &hvh);
			ImGui::Selectable(enc("Visuals"), &visuals);
			ImGui::Selectable(enc("Misc"), &misc);
			ImGui::Selectable(enc("Colors"), &colors);
			ImGui::Selectable(enc("Skins"), &skins);
			ImGui::Selectable(enc("Styles"), &styles);
			ImGui::Selectable(enc("Config"), &settings);
			ImGui::PopFont();

			ImGui::PushFont(io->Fonts->Fonts[2]);
			ImGui::Text(enc("Version: %s"), CHEAT_VERSION);
			ImGui::Text(enc("Ping: %d | FPS: %d"), g_GameHelper.GetPing(), g_GameHelper.GetFPS());
			ImGui::PopFont();
		}
		ImGui::End();

		// Begin LEGITBOT window code
		if (legit)
		{
			ImGui::Begin(enc("##LEGITBOT"), &legit, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize );
			{
				ImGui::TextColored(menu_color, enc("Legitbot"));
				ImGui::PushFont(io->Fonts->Fonts[2]);
				ImGui::Text(enc("Step up your game"));
				ImGui::PopFont();

				ImGui::Separator();

				ImGui::PushFont(io->Fonts->Fonts[1]);
				ImGui::Checkbox(CFG(Aimbot.Active));
				ImGui::Combo(CFG(Aimbot.Mode), dd_AimModes);
				ImGui::Combo(CFG(Aimbot.SortBy), dd_SortBy);
				ImGui::KeyBindButton(CFG(Aimbot.AimKey));
				ImGui::Checkbox(CFG(Aimbot.TeamMates));
				ImGui::Checkbox(CFG(Aimbot.VisibleCheck));
				ImGui::Combo(CFG(Aimbot.Hitbox), dd_Hitboxes);
				ImGui::SliderFloat(CFG(Aimbot.FOV), 0.f, 45.f);
				ImGui::SliderFloat(CFG(Aimbot.Smooth), 0.1f, 15.f);
				ImGui::SliderFloat(CFG(Aimbot.Time), 0.1f, 15.f);
				ImGui::Checkbox(CFG(Aimbot.SmokeCheck));
				ImGui::Checkbox(CFG(Aimbot.AwpBody));
				ImGui::Checkbox(CFG(Aimbot.SmartAim));
				ImGui::SliderFloat(CFG(Aimbot.RCSFactor), 0.0f, 2.f);
				ImGui::Checkbox(CFG(Aimbot.Silent));
				ImGui::Checkbox(CFG(Aimbot.UseGroups));
				ImGui::Checkbox(CFG(Aimbot.Backtrack));
				ImGui::Checkbox(CFG(Aimbot.AdaptiveBacktrackFOV));
				ImGui::PopFont();

				ImGui::Separator();

				ImGui::PushFont(io->Fonts->Fonts[1]);
				ImGui::Checkbox(CFG(RCS.Active));
				ImGui::SliderInt(CFG(RCS.StartBullet), 0, 15);
				ImGui::SliderInt(CFG(RCS.StopBullet), 0, 15);
				ImGui::Checkbox(CFG(RCS.Smooth));
				ImGui::PopFont();

				ImGui::Separator();

				ImGui::PushFont(io->Fonts->Fonts[1]);
				ImGui::Selectable(enc("Triggerbot"), &trigger);
				ImGui::PopFont();
			} // End legitbot frame
			ImGui::End();

			// Begin WEAPONGROUPS frame
			if (Aimbot.UseGroups.m_Value)
			{
				ImGui::Begin(enc("##WEAPONGROUPS"), &Aimbot.UseGroups.m_Value, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize );
				{
					ImGui::TextColored(menu_color, enc("Legitbot - WeaponGroups"));
					ImGui::PushFont(io->Fonts->Fonts[2]);
					ImGui::Text(enc("Make it truly yours"));
					ImGui::PopFont();

					ImGui::Separator();

					// Loop through all the configs and create collapsing headers accordingly
					ImGui::PushFont(io->Fonts->Fonts[1]);
					int i = 0;
					for (auto e : Config::WeaponGroups)
					{
						auto cfg = e.second;
						if (ImGui::CollapsingHeader(cfg->NAME.c_str()))
						{
							ImGui::Checkbox(std::string(enc("##ACTIVE")).append(std::to_string(i)).c_str(), &cfg->Active.m_Value);
							ImGui::SameLine(); ImGui::Text(cfg->Active.m_strName.c_str());
							ImGui::Combo(std::string(enc("##HITBOX")).append(std::to_string(i)).c_str(), &cfg->Hitbox.m_Value, dd_Hitboxes);
							ImGui::SameLine(); ImGui::Text(cfg->Hitbox.m_strName.c_str());
							ImGui::SliderFloat(std::string(enc("##FOV")).append(std::to_string(i)).c_str(), &cfg->FOV.m_Value, 0.f, 45.f);
							ImGui::SameLine(); ImGui::Text(cfg->FOV.m_strName.c_str());
							ImGui::SliderFloat(std::string(enc("##SMOOTH")).append(std::to_string(i)).c_str(), &cfg->Smooth.m_Value, 0.f, 15.f);
							ImGui::SameLine(); ImGui::Text(cfg->Smooth.m_strName.c_str());
							ImGui::SliderFloat(std::string(enc("##TIME")).append(std::to_string(i)).c_str(), &cfg->Time.m_Value, 0.f, 15.f);
							ImGui::SameLine(); ImGui::Text(cfg->Time.m_strName.c_str());
						}
						i++;
					}
					ImGui::PopFont();
				}
				ImGui::End();
			} // End weapongroups frame

			// Begin TRIGGERBOT window code
			if (trigger)
			{
				ImGui::Begin(enc("##TRIGGERBOT"), &trigger, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize );
				{
					ImGui::TextColored(menu_color, enc("Triggerbot"));
					ImGui::PushFont(io->Fonts->Fonts[2]);
					ImGui::Text(enc("Boost your reactions"));
					ImGui::PopFont();

					ImGui::Separator();

					ImGui::PushFont(io->Fonts->Fonts[1]);
					ImGui::Checkbox(CFG(Trigger.Active));
					ImGui::SliderInt(CFG(Trigger.Delay), 0, 50);
					ImGui::Checkbox(CFG(Trigger.UseKey));
					ImGui::KeyBindButton(CFG(Trigger.Key));
					ImGui::SliderInt(CFG(Trigger.Hitchance), 0, 100, enc("%.0f%%"));
					ImGui::Combo(CFG(Trigger.Hitgroup), dd_Hitgroups);
					ImGui::Checkbox(CFG(Trigger.SmokeCheck));
					ImGui::Checkbox(CFG(Trigger.ThroughWalls));
					ImGui::SliderFloat(CFG(Trigger.MinWallDamage), 0.f, 110.f);
					ImGui::PopFont();
				}
				ImGui::End();
			} // End triggerbot frame
		} // End legitbot

		// Begin RAGEBOT window code
		if (rage)
		{
			ImGui::Begin(enc("##RAGEBOT"), &rage, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize  | ImGuiWindowFlags_NoResize);
			{
				ImGui::TextColored(menu_color, enc("Ragebot"));
				ImGui::PushFont(io->Fonts->Fonts[2]);
				ImGui::Text(enc("Dominate your foes"));
				ImGui::PopFont();

				ImGui::Separator();

				ImGui::PushFont(io->Fonts->Fonts[1]);
				ImGui::Selectable(enc("Settings"), &rage_settings);
				ImGui::Selectable(enc("Accuracy"), &rage_accuracy);
				ImGui::Selectable(enc("Target"), &rage_target);
				ImGui::PopFont();
			}
			ImGui::End();

			// Begin RAGESETTINGS frame
			if (rage_settings)
			{
				ImGui::Begin(enc("##RAGESETTINGS"), &rage_settings, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize );
				{
					ImGui::TextColored(menu_color, enc("Ragebot - Settings"));
					ImGui::PushFont(io->Fonts->Fonts[2]);
					ImGui::Text(enc("Setup your rage"));
					ImGui::PopFont();

					ImGui::Separator();

					ImGui::PushFont(io->Fonts->Fonts[1]);
					ImGui::Checkbox(CFG(Ragebot.Active));
					ImGui::Combo(CFG(Ragebot.Mode), dd_AimModes);
					ImGui::KeyBindButton(CFG(Ragebot.Key));
					ImGui::SliderFloat(CFG(Ragebot.FOV), 0.f, 180.f);
					ImGui::Combo(CFG(Ragebot.Hitbox), dd_Hitboxes);
					ImGui::Checkbox(CFG(Ragebot.Silent));
					ImGui::Checkbox(CFG(Ragebot.AutoPistol));
					ImGui::Checkbox(CFG(Ragebot.EnableSpeedLimit));
					ImGui::SliderFloat(CFG(Ragebot.SpeedLimit), 20.f, 180.f);
					ImGui::Checkbox(CFG(Ragebot.AwpBody));
					ImGui::Checkbox(CFG(Ragebot.Chickens));
					ImGui::PopFont();
				}
				ImGui::End();
			}

			// Begin RAGEACCURACY frame
			if (rage_accuracy)
			{
				ImGui::Begin(enc("##RAGEACCURACY"), &rage_accuracy, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize );
				{
					ImGui::TextColored(menu_color, enc("Ragebot - Accuracy"));
					ImGui::PushFont(io->Fonts->Fonts[2]);
					ImGui::Text(enc("Customize your precision"));
					ImGui::PopFont();

					ImGui::Separator();

					ImGui::PushFont(io->Fonts->Fonts[1]);
					ImGui::Checkbox(CFG(RageAccuracy.NoSpread));
					ImGui::Checkbox(CFG(RageAccuracy.AutoShoot));
					ImGui::Checkbox(CFG(RageAccuracy.AutoWall));
					ImGui::Checkbox(CFG(RageAccuracy.AutoStop));
					ImGui::Checkbox(CFG(RageAccuracy.AutoCrouch));
					ImGui::Checkbox(CFG(RageAccuracy.AutoScope));
					ImGui::Checkbox(CFG(RageAccuracy.AutoRevolver));
					ImGui::SliderInt(CFG(RageAccuracy.HitChance), 0, 100, enc("%.0f%%"));
					ImGui::PopFont();
				}
				ImGui::End();
			}

			// Begin RAGETARGET frame
			if (rage_target)
			{
				ImGui::Begin(enc("##RAGETARGET"), &rage_accuracy, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize );
				{
					ImGui::TextColored(menu_color, enc("Ragebot - Target"));
					ImGui::PushFont(io->Fonts->Fonts[2]);
					ImGui::Text(enc("Select your targets"));
					ImGui::PopFont();

					ImGui::Separator();

					ImGui::PushFont(io->Fonts->Fonts[1]);
					ImGui::Combo(CFG(RageTarget.SortBy), dd_SortBy);
					ImGui::Checkbox(CFG(RageTarget.TeamMates));
					ImGui::SliderInt(CFG(RageTarget.BodyAimAfterShots), 0, 10);
					ImGui::SliderInt(CFG(RageTarget.BodyAimIfHPLessThan), 0, 100);
					ImGui::Checkbox(CFG(RageTarget.BodyAimIfHPLessThanDamage));
					ImGui::Checkbox(CFG(RageTarget.Hitscan));
					ImGui::SliderInt(CFG(RageTarget.MinDamage), 0, 110, enc("%.0f%%"));
					ImGui::Checkbox(CFG(RageTarget.MultiPoint));
					ImGui::PopFont();
				}
				ImGui::End();
			}
		} // End ragebot

		// Begin HVH window code
		if (hvh)
		{
			ImGui::Begin(enc("##HVH"), &hvh, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize  | ImGuiWindowFlags_NoResize);
			{
				ImGui::TextColored(menu_color, enc("Hack VS Hack"));
				ImGui::PushFont(io->Fonts->Fonts[2]);
				ImGui::Text(enc("Challenge other hackers"));
				ImGui::PopFont();

				ImGui::Separator();

				ImGui::PushFont(io->Fonts->Fonts[1]);
				ImGui::Selectable(enc("Anti Aim"), &hvh_antiaim);
				ImGui::Selectable(enc("Resolver"), &hvh_resolver);
				ImGui::Selectable(enc("Fake Lag"), &hvh_fakelag);
				ImGui::Selectable(enc("Player List"), &hvh_playerlist);
				ImGui::PopFont();
			}
			ImGui::End();

			// Begin HVHAA frame
			if (hvh_antiaim)
			{
				ImGui::Begin(enc("##HVHAA"), &hvh_antiaim, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize );
				{
					ImGui::TextColored(menu_color, enc("HvH - Anti Aim"));
					ImGui::PushFont(io->Fonts->Fonts[2]);
					ImGui::Text(enc("Fool their bots"));
					ImGui::PopFont();

					ImGui::Separator();

					ImGui::Checkbox(CFG(AntiAim.Active));
					ImGui::Combo(CFG(AntiAim.Pitch), dd_AA_Pitch);
					ImGui::SliderFloat(CFG(AntiAim.CustomPitch), -89.f, 89.f);
					ImGui::Combo(CFG(AntiAim.RealYaw), dd_AA_Yaw);
					ImGui::SliderFloat(CFG(AntiAim.RealYawMod), -180.f, 180.f);
					ImGui::Combo(CFG(AntiAim.FakeYaw), dd_AA_Yaw);
					ImGui::SliderFloat(CFG(AntiAim.FakeYawMod), -180.f, 180.f);
					ImGui::Checkbox(CFG(AntiAim.Edge));
					ImGui::Checkbox(CFG(AntiAim.AtTargets));
					ImGui::Checkbox(CFG(AntiAim.OnLadder));
					ImGui::Checkbox(CFG(AntiAim.OnKnife));
					ImGui::Checkbox(CFG(AntiAim.OnGrenades));
					ImGui::Checkbox(CFG(AntiAim.FakeAnglesChams));
					ImGui::Checkbox(CFG(AntiAim.FakeWalk));
					ImGui::KeyBindButton(CFG(AntiAim.FakeWalkKey));
					ImGui::SliderFloat(CFG(AntiAim.JitterRange), -180.f, 180.f);
					ImGui::SliderFloat(CFG(AntiAim.SpinSpeed), 0.05f, 0.8f);
					ImGui::SliderFloat(CFG(AntiAim.LowerBodyDelta), -180.f, 180.f);
					//ImGui::Checkbox(CFG(AntiAim.ShowRealAA)); removed
					ImGui::Checkbox(CFG(AntiAim.AnglesInfo));
				}
				ImGui::End();
			}

			// Begin HVHRESOLVER frame
			if (hvh_resolver)
			{
				ImGui::Begin(enc("##HVHRESOLVER"), &hvh_resolver, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize );
				{
					ImGui::TextColored(menu_color, enc("HvH - Resolver"));
					ImGui::PushFont(io->Fonts->Fonts[2]);
					ImGui::Text(enc("Resolve them or die trying"));
					ImGui::PopFont();

					ImGui::Separator();

					ImGui::Checkbox(CFG(Resolver.Active));
					//ImGui::Checkbox(CFG(Resolver.Backtracking)); removed
					ImGui::Combo(CFG(Resolver.Type), dd_ResolverTypes);
					ImGui::Checkbox(CFG(Resolver.BacktrackSkeleton));
					ImGui::Checkbox(CFG(Resolver.BacktrackPoint));
					ImGui::Checkbox(CFG(Resolver.ResolverESP));
				}
				ImGui::End();
			}

			// Begin HVHFAKELAG frame
			if (hvh_fakelag)
			{
				ImGui::Begin(enc("##HVHFAKELAG"), &hvh_fakelag, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize );
				{
					ImGui::TextColored(menu_color, enc("HvH - Fake Lag"));
					ImGui::PushFont(io->Fonts->Fonts[2]);
					ImGui::Text(enc("Gotta fake 'em all"));
					ImGui::PopFont();

					ImGui::Separator();

					ImGui::Checkbox(CFG(FakeLag.Active));
					ImGui::Checkbox(CFG(FakeLag.UseKey));
					ImGui::KeyBindButton(CFG(FakeLag.Key));
					ImGui::SliderInt(CFG(FakeLag.Factor), 0, 15);
					ImGui::Combo(CFG(FakeLag.Mode), dd_FakeLagModes);
					ImGui::Checkbox(CFG(FakeLag.OnShoot));
					ImGui::Checkbox(CFG(FakeLag.AirOnly));
				}
				ImGui::End();
			}

			// Begin HVHPLAYERS frame
			if (hvh_playerlist)
			{
				ImGui::Begin(enc("##HVHPLAYERS"), &hvh_playerlist, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize );
				{
					ImGui::TextColored(menu_color, enc("HvH - PlayerList"));
					ImGui::PushFont(io->Fonts->Fonts[2]);
					ImGui::Text(enc("W.I.P - Coming soon!"));
					ImGui::PopFont();

					ImGui::Separator();

					ImGui::Checkbox(CFG(PlayerList.Active));
				}
				ImGui::End();
			}
		} // End hvh

		// Begin VISUALS window code
		if (visuals)
		{
			ImGui::Begin(enc("##VISUALS"), &visuals, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize  | ImGuiWindowFlags_NoResize);
			{
				ImGui::TextColored(menu_color, enc("Visuals"));
				ImGui::PushFont(io->Fonts->Fonts[2]);
				ImGui::Text(enc("Enhance your vision"));
				ImGui::PopFont();

				ImGui::Separator();

				ImGui::PushFont(io->Fonts->Fonts[1]);
				ImGui::Selectable(enc("ESP"), &visuals_esp);
				ImGui::Selectable(enc("Filters"), &visuals_filters);
				ImGui::Selectable(enc("Removals"), &visuals_removals);
				ImGui::Selectable(enc("Chams"), &visuals_chams);
				ImGui::Selectable(enc("Other"), &visuals_other);
				ImGui::PopFont();
			}
			ImGui::End();

			// Begin ESP frame
			if (visuals_esp)
			{
				ImGui::Begin(enc("##ESP"), &visuals_esp, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize );
				{
					ImGui::TextColored(menu_color, enc("Visuals - ESP"));
					ImGui::PushFont(io->Fonts->Fonts[2]);
					ImGui::Text(enc("Extra Sensory Perception"));
					ImGui::PopFont();

					ImGui::Separator();

					ImGui::Checkbox(CFG(ESP.Active));
					ImGui::Checkbox(CFG(ESP.BoxEnabled));
					ImGui::Combo(CFG(ESP.BoxType), dd_BoxTypes);
					ImGui::Checkbox(CFG(ESP.BoxOutline));
					ImGui::Checkbox(CFG(ESP.BoxFilled));
					ImGui::SliderInt(CFG(ESP.BoxFillAlpha), 0, 255);
					ImGui::SliderFloat(CFG(ESP.FadeSpeed), 0.1f, 2.f);
					ImGui::Checkbox(CFG(ESP.Health));
					ImGui::Checkbox(CFG(ESP.Armor));
					ImGui::Checkbox(CFG(ESP.BatteryBars));
					ImGui::Checkbox(CFG(ESP.Name));
					ImGui::Checkbox(CFG(ESP.Weapon));
					ImGui::Checkbox(CFG(ESP.Ammo));
					ImGui::Checkbox(CFG(ESP.Money));
					ImGui::Checkbox(CFG(ESP.HasBomb));
					ImGui::Checkbox(CFG(ESP.HasDefuser));
					ImGui::Checkbox(CFG(ESP.IsDefusing));
					ImGui::Checkbox(CFG(ESP.RevealRanks));
					ImGui::Checkbox(CFG(ESP.BombTimer));
					ImGui::Checkbox(CFG(ESP.Barrel));
					ImGui::Combo(CFG(ESP.SnapLines), dd_SnapLines);
					ImGui::Checkbox(CFG(ESP.NadePrediction));
					ImGui::Checkbox(CFG(ESP.NadeDamage));
					ImGui::Checkbox(CFG(ESP.Skeleton));
					ImGui::Combo(CFG(ESP.Hitbox), dd_ESP_Hitboxes);
					ImGui::Checkbox(CFG(ESP.HeadPoint));
					ImGui::Checkbox(CFG(ESP.AimSpot));
					ImGui::Checkbox(CFG(ESP.SoundESP));
					ImGui::Checkbox(CFG(ESP.Glow));
					ImGui::SliderFloat(CFG(ESP.GlowAlpha), 0.f, 1.f);
					ImGui::Checkbox(CFG(ESP.Watermark));
					ImGui::SliderInt(CFG(ESP.WatermarkAlpha), 0, 255);
					ImGui::Combo(CFG(ESP.Overhead), dd_OverheadTypes);
				}
				ImGui::End();
			}

			// Begin FILTERS frame
			if (visuals_filters)
			{
				ImGui::Begin(enc("##FILTERS"), &visuals_filters, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize );
				{
					ImGui::TextColored(menu_color, enc("Visuals - Filters"));
					ImGui::PushFont(io->Fonts->Fonts[2]);
					ImGui::Text(enc("Choose what you see"));
					ImGui::PopFont();

					ImGui::Separator();

					ImGui::Checkbox(CFG(VisualsFilters.All));
					ImGui::Checkbox(CFG(VisualsFilters.TeamMates));
					ImGui::Checkbox(CFG(VisualsFilters.Enemies));
					ImGui::Checkbox(CFG(VisualsFilters.VisibleOnly));
					ImGui::Checkbox(CFG(VisualsFilters.Guns));
					ImGui::Checkbox(CFG(VisualsFilters.Grenades));
					ImGui::Checkbox(CFG(VisualsFilters.Bomb));
					ImGui::Checkbox(CFG(VisualsFilters.Hostages));
				}
				ImGui::End();
			}

			// Begin REMOVALS frame
			if (visuals_removals)
			{
				ImGui::Begin(enc("##REMOVALS"), &visuals_removals, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize );
				{
					ImGui::TextColored(menu_color, enc("Visuals - Removals"));
					ImGui::PushFont(io->Fonts->Fonts[2]);
					ImGui::Text(enc("Remove what you want"));
					ImGui::PopFont();

					ImGui::Separator();

					ImGui::Checkbox(CFG(Removals.NoVisualRecoil));
					ImGui::Checkbox(CFG(Removals.NoScope));
					ImGui::Checkbox(CFG(Removals.NoFlash));
					ImGui::Checkbox(CFG(Removals.NoSmoke));
					ImGui::Checkbox(CFG(Removals.NoSky));
					ImGui::Checkbox(CFG(Removals.NoHands));
					ImGui::Checkbox(CFG(Removals.NoWeapons));
					ImGui::Checkbox(CFG(Removals.NoPostProcessing));
				}
				ImGui::End();
			}

			// Begin CHAMS frame
			if (visuals_chams)
			{
				ImGui::Begin(enc("##CHAMS"), &visuals_chams, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize );
				{
					ImGui::TextColored(menu_color, enc("Visuals - Chams"));
					ImGui::PushFont(io->Fonts->Fonts[2]);
					ImGui::Text(enc("Modify your textures"));
					ImGui::PopFont();

					ImGui::Separator();

					ImGui::Checkbox(CFG(Chams.Active));
					ImGui::Checkbox(CFG(Chams.XQZ));
					ImGui::Checkbox(CFG(Chams.EnemyOnly));
					ImGui::Combo(CFG(Chams.VisibleType), dd_ChamsTypes);
					ImGui::Combo(CFG(Chams.HiddenType), dd_ChamsTypes);
					ImGui::Combo(CFG(Chams.HandChams), dd_ChamsTypes);
					ImGui::Combo(CFG(Chams.WeaponChams), dd_ChamsTypes);
				}
				ImGui::End();
			}

			// Begin OTHER frame
			if (visuals_other)
			{
				ImGui::Begin(enc("##OTHER"), &visuals_other, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize );
				{
					ImGui::TextColored(menu_color, enc("Visuals - Other"));
					ImGui::PushFont(io->Fonts->Fonts[2]);
					ImGui::Text(enc("Other visual settings"));
					ImGui::PopFont();

					ImGui::Separator();

					ImGui::KeyBindButton(CFG(VisualsOther.ToggleKey));
					ImGui::Checkbox(CFG(VisualsOther.Lasers));
					ImGui::Checkbox(CFG(VisualsOther.FootstepRings));
					ImGui::Combo(CFG(VisualsOther.LaserType), dd_BeamTypes);
					ImGui::Combo(CFG(VisualsOther.RenderMode), dd_RenderModes);
					ImGui::Checkbox(CFG(VisualsOther.Crosshair));
					ImGui::Checkbox(CFG(VisualsOther.RecoilCrosshair));
					ImGui::Combo(CFG(VisualsOther.CrosshairType), dd_Crosshairs);
					ImGui::Checkbox(CFG(VisualsOther.SpreadCircle));
					ImGui::Checkbox(CFG(VisualsOther.EngineRadar));
					ImGui::Checkbox(CFG(VisualsOther.Radar));
					ImGui::SliderFloat(CFG(VisualsOther.RadarRange), 0.f, 10000.f);
					ImGui::Checkbox(CFG(VisualsOther.ColorWalls));
					ImGui::Checkbox(CFG(VisualsOther.ThirdPerson));
					//ImGui::Checkbox(CFG(VisualsOther.ThirdPersonSmart)); removed
					ImGui::SliderFloat(CFG(VisualsOther.CustomFOV), 0.f, 40.f);
					ImGui::SliderFloat(CFG(VisualsOther.ViewModelFOV), 0.f, 40.f);
					ImGui::Checkbox(CFG(VisualsOther.Hitmarker));
					//ImGui::Checkbox(CFG(VisualsOther.HurtLogs));
				}
				ImGui::End();
			}
		} // End visuals

		// Begin MISC window code
		if (misc)
		{
			ImGui::Begin(enc("##MISC"), &misc, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize );
			{
				ImGui::TextColored(menu_color, enc("Misc"));
				ImGui::PushFont(io->Fonts->Fonts[2]);
				ImGui::Text(enc("Misc stuff to play with"));
				ImGui::PopFont();

				ImGui::Separator();

				ImGui::KeyBindButton(CFG(Misc.MenuKey));
				ImGui::KeyBindButton(CFG(Misc.PanicKey));
				ImGui::SliderFloat(CFG(Misc.MenuFadeSpeed), 0.1f, 2.f);
				ImGui::Checkbox(CFG(Misc.BunnyHop));
				ImGui::Checkbox(CFG(Misc.EdgeJump));
				ImGui::Checkbox(CFG(Misc.AutoStrafe));
				ImGui::Checkbox(CFG(Misc.KnifeBot));
				ImGui::KeyBindButton(CFG(Misc.AirStuckKey));
				ImGui::Checkbox(CFG(Misc.AutoAccept));
				ImGui::Checkbox(CFG(Misc.MovementRecorder));
				ImGui::Checkbox(CFG(Misc.ChatSpam));
				ImGui::Checkbox(CFG(Misc.NameSpam));
				ImGui::Checkbox(CFG(Misc.NameStealer));
				ImGui::Checkbox(CFG(Misc.NoName));
				ImGui::Checkbox(CFG(Misc.SpectatorList));
				ImGui::Combo(CFG(Misc.ClanTag), dd_ClanTags);
				ImGui::Selectable(enc("Notifications"), &misc_notifications);
			}
			ImGui::End();

			// Begin AUTOSTRAFE frame
			if (Misc.AutoStrafe.m_Value)
			{
				ImGui::Begin(enc("##AUTOSTRAFE"), &Misc.AutoStrafe.m_Value, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize );
				{
					ImGui::TextColored(menu_color, enc("Misc - Auto Strafe"));
					ImGui::PushFont(io->Fonts->Fonts[2]);
					ImGui::Text(enc("Become a speed demon"));
					ImGui::PopFont();

					ImGui::Separator();

					ImGui::Combo(CFG(AutoStrafe.Mode), dd_AutoStrafeTypes);
					ImGui::Checkbox(CFG(AutoStrafe.AirStrafe));
					ImGui::Checkbox(CFG(AutoStrafe.GroundStrafe));
					ImGui::KeyBindButton(CFG(AutoStrafe.CircleStrafe));
					ImGui::SliderFloat(CFG(AutoStrafe.RetrackSpeed), 1.f, 10.f);
				}
				ImGui::End();
			}

			// Begin MOVEMENTRECORDER frame
			if (Misc.MovementRecorder.m_Value)
			{
				ImGui::Begin(enc("##MOVEMENTRECORDER"), &Misc.MovementRecorder.m_Value, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize );
				{
					ImGui::TextColored(menu_color, enc("Misc - Movement Recorder"));
					ImGui::PushFont(io->Fonts->Fonts[2]);
					ImGui::Text(enc("Record your steps"));
					ImGui::PopFont();

					ImGui::Separator();

					// TODO: Movement Recorder menu
				}
				ImGui::End();
			}

			// Begin NOTIFICATIONS frame
			if (misc_notifications)
			{
				ImGui::Begin(enc("##NOTIFICATIONS"), &Misc.MovementRecorder.m_Value, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize);
				{
					ImGui::TextColored(menu_color, enc("Misc - Notifications"));
					ImGui::PushFont(io->Fonts->Fonts[2]);
					ImGui::Text(enc("Gain more knownledge"));
					ImGui::PopFont();

					ImGui::Separator();

					ImGui::Checkbox(CFG(Notifications.EnableNotifications));
					ImGui::Checkbox(CFG(Notifications.IgnoreLocal));
					if (ImGui::Button(enc("Listen All")))
						Notifications.ListenAll();
					ImGui::SameLine(0, 10);
					if (ImGui::Button(enc("Listen None")))
						Notifications.ListenNone();
					ImGui::NewLine();
					ImGui::Checkbox(CFG(Notifications.ItemPurchase));
					ImGui::Checkbox(CFG(Notifications.PlayerDeath));
					ImGui::Checkbox(CFG(Notifications.WeaponReload));
					ImGui::Checkbox(CFG(Notifications.ItemPickup));
					ImGui::Checkbox(CFG(Notifications.BombBeginPlant));
					ImGui::Checkbox(CFG(Notifications.BombAbortPlant));
					ImGui::Checkbox(CFG(Notifications.BombPlanted));
					ImGui::Checkbox(CFG(Notifications.BombDefused));
					ImGui::Checkbox(CFG(Notifications.BombDropped));
					ImGui::Checkbox(CFG(Notifications.BombPickup));
					ImGui::Checkbox(CFG(Notifications.BombBeginDefuse));
					ImGui::Checkbox(CFG(Notifications.BombAbortDefuse));
					ImGui::Checkbox(CFG(Notifications.HostageHurt));
					ImGui::Checkbox(CFG(Notifications.HostageKilled));
				}
				ImGui::End();
			}
		} // End misc

		// Begin COLORS window code
		if (colors)
		{
			ImGui::Begin(enc("##COLORS"), &colors, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize );
			{
				// This will contains a pointer to every single color config
				std::vector<ConfigVar_Color*> color_pointers
				{
					&Colors.BoxTeam,
					&Colors.BoxEnemy,
					&Colors.SkeletonTeam,
					&Colors.SkeletonEnemy,
					&Colors.HitboxTeam,
					&Colors.HitboxEnemy,
					&Colors.GlowTeam,
					&Colors.GlowEnemy,
					&Colors.Watermark,
					&Colors.ChamsTeam,
					&Colors.ChamsEnemy,
					&Colors.ChamsTeam_XQZ,
					&Colors.ChamsEnemy_XQZ,
					&Colors.HandChams,
					&Colors.WeaponChams,
					&Colors.Crosshair,
					&Colors.RadarBoxes,
					&Colors.MenuText,
					&Colors.Walls,
					&Colors.Beams,
				};

				ImGui::TextColored(menu_color, enc("Colors"));
				ImGui::PushFont(io->Fonts->Fonts[2]);
				ImGui::Text(enc("Customize your colors"));
				ImGui::PopFont();

				ImGui::Separator();

				ImGui::Combo(enc("##COLORCOMBO"), &color_selected, dd_Colors);
				if (color_pointers[color_selected] != nullptr)
				{
					bool ret = ImGui::ColorPicker(&color_pointers[color_selected]->m_Value);
					if (color_pointers[color_selected]->m_strName.compare(enc("Walls")) == 0)
					{
						g_bWallColorModified = ret;
					}
				}

				if (ImGui::Button(enc("Reset Colors")))
					Config::Colors.InitColors();

				color_pointers.clear();
			}
			ImGui::End();
		} // End colors

		// Begin SKINS window code
		if (skins)
		{
			ImGui::Begin(enc("##SKINS"), &skins, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize);
			{
				ImGui::TextColored(menu_color, enc("Skins"));
				ImGui::PushFont(io->Fonts->Fonts[2]);
				ImGui::Text(enc("Your dream items, for free!"));
				ImGui::PopFont();

				ImGui::Separator();

				ImGui::Checkbox(CFG(Skins.Active));

				ImGui::TextColored(menu_color, enc("Paint Kits"));

				ImGui::Checkbox(CFG(PaintKits.Active));
				ImGui::SameLine(0.f, 230.f);
				ImGui::InputText(enc("##SKIN_FILTER"), buffer_skinfilter, 64);

				ImGui::Combo(enc("##PK_WEAPON"), &paintkit_weapon, [](void* data, int idx, const char** out_text)
				{
					*out_text = k_weapon_names[idx].name;
					return true;
				}, nullptr, k_weapon_names.size(), 10);
				PaintKits.Weapon.m_Value = k_weapon_names[paintkit_weapon].definition_index;

				EconomyItem& selected_item = Config::SkinConfig[PaintKits.Weapon.m_Value];

				ImGui::SameLine(0.f, 10.f);
				static int idx = 0;

				// If I'm filtering start filter
				if (strcmp(buffer_skinfilter, ""))
				{
					static std::vector<std::string> filtered{};
					bool found_one = false;
					for (unsigned int i = 0; i < k_skins.size(); i++)
					{
						if (strstr(k_skins[i].name.c_str(), buffer_skinfilter))
						{
							filtered.push_back(k_skins[i].name);
							found_one = true;
						}
					}
					if (!found_one)
						filtered.push_back(enc("-"));
					ImGui::Combo(enc("##PK_SKIN"), &idx, filtered);
					std::string skin_name = filtered[idx];
					selected_item.pk_vector_index = std::find_if(k_skins.begin(), k_skins.end(), [skin_name](Kit_t kit)->bool {
						return (kit.name == skin_name);
					}) - k_skins.begin();
					filtered.clear();
				}
				else
				{
					idx = 0;
					ImGui::Combo(enc("##PK_SKIN"), &selected_item.pk_vector_index, [](void* data, int idx, const char** out_text)
					{
						*out_text = k_skins[idx].name.c_str();
						return true;
					}, nullptr, k_skins.size(), 10);
				}

				selected_item.UpdateItem();

				ImGui::TextColored(menu_color, enc("Skin Settings"));

				ImGui::Text(enc("Stat-Trak"));
				ImGui::SameLine(0.f, 260.f);
				ImGui::Text(enc("Seed"));

				ImGui::InputInt(enc("##SC_STATTRAK"), &selected_item.StatTrak);
				ImGui::SameLine(0.f, 10.f);
				ImGui::InputInt(enc("##SC_SEED"), &selected_item.Seed);

				ImGui::Text(enc("Wear"));
				ImGui::SameLine(0.f, 290.f);
				ImGui::Text(enc("Name Tag"));

				ImGui::InputFloat(enc("##SC_WEAR"), &selected_item.Wear);
				ImGui::SameLine(0.f, 10.f);
				ImGui::InputText(enc("##SC_NAMETAG"), selected_item.CustomName, 32);

				ImGui::TextColored(menu_color, enc("Skin & Glove Changer"));

				ImGui::Text(enc("Knife"));
				ImGui::SameLine(0.f, 290.f);
				ImGui::Text(enc("Glove"));

				ImGui::Combo(enc("##SC_KNIFE"), &knife, [](void* data, int idx, const char** out_text)
				{
					*out_text = k_knife_names[idx].name;
					return true;
				}, nullptr, k_knife_names.size(), 10);
				KnifeGlove.Knife.m_Value = k_knife_names[knife].definition_index;

				ImGui::SameLine(0.f, 10.f);

				ImGui::Combo(enc("##SC_GLOVE"), &glove, [](void* data, int idx, const char** out_text)
				{
					*out_text = k_myGloves[idx].fancy_name.c_str();
					return true;
				}, nullptr, k_myGloves.size(), 10);
				KnifeGlove.Glove.m_Value = glove;

				selected_item.UpdateItem();

				// STICKERS

				ImGui::TextColored(menu_color, enc("Stickers"));

				static auto selected_sticker_slot = 0;
				auto& selected_sticker = selected_item.Stickers[selected_sticker_slot];

				ImGui::Text(enc("Sticker Slot"));
				ImGui::SameLine(0.f, 243.f);
				ImGui::InputText(enc("##STICKERNAME_FILTER"), buffer_stickerfilter, 64);

				// I will fill the vector before calling Combo
				std::vector<std::string> sticker_slots{};
				for (int i = 0; i < 5; i++)
				{
					// Grab kit vector index to grab sticker name
					auto sticker_config = Config::SkinConfig[PaintKits.Weapon.m_Value];
					auto kit_vector_index = sticker_config.Stickers[i].kit_vector_index;
					auto sticker_name = k_stickers.at(kit_vector_index).name;

					// Setup stream
					std::stringstream ss;
					ss << "Slot " << i + 1 << " (" << sticker_name << ")";

					// Push back new string built
					sticker_slots.push_back(ss.str());
				}

				ImGui::Combo(enc("##SC_STICKER_SLOT"), &selected_sticker_slot, sticker_slots, 10);

				ImGui::SameLine(0.f, 10.f);
				static int idx_stickers = 0;

				// If I'm filtering start the filter
				if (strcmp(buffer_stickerfilter, enc("")))
				{
					static std::vector<std::string> filtered{};
					bool found_one = false;
					for (unsigned int i = 0; i < k_stickers.size(); i++)
					{
						if (strstr(k_stickers[i].name.c_str(), buffer_stickerfilter))
						{
							filtered.push_back(k_stickers[i].name);
							found_one = true;
						}
					}
					if (!found_one)
						filtered.push_back(enc("-"));
					ImGui::Combo(enc("##SC_STICKER_KIT"), &idx_stickers, filtered);
					std::string sticker_name = filtered[idx_stickers];
					selected_sticker.kit_vector_index = std::find_if(k_stickers.begin(), k_stickers.end(), [sticker_name](Kit_t kit)->bool {
						return (kit.name == sticker_name);
					}) - k_stickers.begin();
					filtered.clear();
				}
				else
				{
					idx_stickers = 0;
					ImGui::Combo(enc("##SC_STICKER_KIT"), &selected_sticker.kit_vector_index, [](void* data, int idx, const char** out_text)
					{
						*out_text = k_stickers[idx].name.c_str();
						return true;
					}, nullptr, k_stickers.size(), 10);
				}

				ImGui::TextColored(menu_color, enc("Sticker Settings"));

				ImGui::SliderFloat(enc("Wear"), &selected_sticker.Wear, 0.0f, 1.f, enc("%.10f"), 5);

				ImGui::SliderFloat(enc("Scale"), &selected_sticker.Scale, 0.1f, 5.f, enc("%.3f"));

				ImGui::SliderFloat(enc("Rotation"), &selected_sticker.Rotation, 0.f, 360.f, enc("%.3f"));

				if (ImGui::Button(enc("Update")))
				{
					// Force update
					Source::Engine->ClientCmd_Unrestricted(enc("cl_fullupdate"));
				}
				ImGui::SameLine();
				if (ImGui::Button(enc("Save")))
				{
					// Save skins
					Config::SaveSkins();
				}
				ImGui::SameLine();
				if (ImGui::Button(enc("Load")))
				{
					// Load skins
					Config::LoadSkins();
				}
			}
			ImGui::End();
		} // End skins

		// Begin STYLES window code
		if (styles)
		{
			ImGui::Begin(enc("##STYLES"), &styles, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize);
			{
				ImGui::TextColored(menu_color, enc("Menu Style Editor"));
				ImGui::PushFont(io->Fonts->Fonts[2]);
				ImGui::Text(enc("Make it your menu!"));
				ImGui::PopFont();

				ImGui::Separator();

				ImGui::Checkbox(CFG(MenuStyle.UseCustomStyle));
				if (ImGui::Button(enc("Reset Style")))
					MenuStyle.ResetStyle();
				ImGui::SliderFloat(CFG(MenuStyle.WindowPadding), 0.f, 15.f);
				ImGui::SliderFloat(CFG(MenuStyle.WindowRounding), 0.f, 10.f);
				ImGui::SliderFloat(CFG(MenuStyle.FramePadding), 0.f, 10.f);
				ImGui::SliderFloat(CFG(MenuStyle.FrameRounding), 0.f, 8.f);
				ImGui::SliderFloat(CFG(MenuStyle.ItemSpacing), 0.f, 16.f);
				ImGui::SliderFloat(CFG(MenuStyle.ItemInnerSpacing), 0.f, 12.f);
				ImGui::SliderFloat(CFG(MenuStyle.IndentSpacing), 0.f, 30.f);
				ImGui::SliderFloat(CFG(MenuStyle.ScrollbarSize), 0.f, 30.f);
				ImGui::SliderFloat(CFG(MenuStyle.ScrollbarRounding), 0.f, 18.f);
				ImGui::SliderFloat(CFG(MenuStyle.GrabRounding), 0.f, 30.f);
			}
			ImGui::End();
		} // End styles

		// Begin SETTINGS window code
		if (settings)
		{
			ImGui::Begin(enc("##SETTINGS"), &settings, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize);
			{
				ImGui::TextColored(menu_color, enc("Settings"));
				ImGui::PushFont(io->Fonts->Fonts[2]);
				ImGui::Text(enc("Save and share your configs"));
				ImGui::PopFont();

				ImGui::Separator();

				ImGui::PushFont(io->Fonts->Fonts[1]);
				ImGui::Text(enc("Config Name"));
				ImGui::InputText(enc("##CFGSAVE"), cfg_buffer, 20);
				if (ImGui::Button(enc("Save")))
				{
					if (!std::string(cfg_buffer).empty())
					{
						Config::Save(std::string(cfg_buffer).append(enc(".json")));
						strcpy(cfg_buffer, "");
					}
				}
				std::vector<std::string> configs{};
				std::vector<std::string> vec_files{};
				// Grab all the configs in the configs directory
				vec_files = GetFilesInFolder(global->ConfigFolder());
				for (auto& file : vec_files)
				{
					// If this file is skins.json, skip it
					if (!strcmp(file.c_str(), enc("skins.json")))
						continue;
					// If the suffix isn't .json, skip it
					if (file.find(enc(".json")) != std::string::npos)
						configs.push_back(file);
				}
				ImGui::Text(enc("Load Config"));
				ImGui::Combo(enc("##CFGLOAD"), &cfg_number, configs);
				if (ImGui::Button(enc("Load")))
				{
					if (cfg_number != -1)
						Config::Load(configs[cfg_number]);
				}
				configs.clear();
				vec_files.clear();
				ImGui::PopFont();
			}
			ImGui::End();
		} // End settings
	}

	// Begin SPECTATORLIST frame
	if (Misc.SpectatorList.m_Value)
	{
		for (int i = 0; i < 43; i++)
		{
			// Reset alpha to be 1 for this specific frame
			style->Colors[i].w = 1.f;
		}
		menu_color.w = 1.f;
		ImGui::Begin(enc("##SPECTATORLIST"), &Misc.SpectatorList.m_Value, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize);
		{
			ImGui::PushFont(io->Fonts->Fonts[2]);

			ImGui::TextColored(menu_color, enc("Spectator List"));
			ImGui::Separator();

			Vitruvia::Miscs->SpectatorList();

			ImGui::PopFont();
		}
		ImGui::End();
		menu_color.w = menu_alpha;
		for (int i = 0; i < 43; i++)
		{
			// Re-apply menu alpha as usual
			float max = style_colors[i].w;
			style->Colors[i].w = menu_alpha;
			style->Colors[i].w = clamp<float, float>(style->Colors[i].w, 0.f, max);
		}
	}
}

void InitMenu_Style()
{
	ImGuiStyle* style = &ImGui::GetStyle();

	if (Config::MenuStyle.UseCustomStyle)
	{
		style->WindowPadding = ImVec2(Config::MenuStyle.WindowPadding.m_Value, Config::MenuStyle.WindowPadding.m_Value);
		style->WindowRounding = Config::MenuStyle.WindowRounding.m_Value;
		style->FramePadding = ImVec2(Config::MenuStyle.FramePadding.m_Value, Config::MenuStyle.FramePadding.m_Value);
		style->FrameRounding = Config::MenuStyle.FrameRounding.m_Value;
		style->ItemSpacing = ImVec2(Config::MenuStyle.ItemSpacing.m_Value, Config::MenuStyle.ItemSpacing.m_Value);
		style->ItemInnerSpacing = ImVec2(Config::MenuStyle.ItemInnerSpacing.m_Value, Config::MenuStyle.ItemInnerSpacing.m_Value);
		style->IndentSpacing = Config::MenuStyle.IndentSpacing.m_Value;
		style->ScrollbarSize = Config::MenuStyle.ScrollbarSize.m_Value;
		style->ScrollbarRounding = Config::MenuStyle.ScrollbarRounding.m_Value;
		style->GrabMinSize = Config::MenuStyle.GrabRounding.m_Value + 2.f;
		style->GrabRounding = Config::MenuStyle.GrabRounding.m_Value;
		//style->AntiAliasedLines = false;
		//style->AntiAliasedShapes = false;
	} 
	else
	{
		style->WindowPadding = ImVec2(10, 10);
		style->WindowRounding = 5.0f;
		style->FramePadding = ImVec2(5, 5);
		style->FrameRounding = 4.0f;
		style->ItemSpacing = ImVec2(8, 8);
		style->ItemInnerSpacing = ImVec2(8, 6);
		style->IndentSpacing = 25.0f;
		style->ScrollbarSize = 15.0f;
		style->ScrollbarRounding = 9.0f;
		style->GrabMinSize = 5.0f;
		style->GrabRounding = 3.0f;
	}

	style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_ComboBg] = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_CloseButton] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
	style->Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
	style->Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);

	for (int i = 0; i < 43; i++)
		style_colors[i] = style->Colors[i];

	//Log(enc("Adding custom fonts from system..."));
	static bool once = false;
	if (!once)
	{
		auto io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF(enc("C:\\Windows\\Fonts\\Ruda-Bold.ttf"), 20); // 0
		io.Fonts->AddFontFromFileTTF(enc("C:\\Windows\\Fonts\\Ruda-Bold.ttf"), 18); // 1
		io.Fonts->AddFontFromFileTTF(enc("C:\\Windows\\Fonts\\Ruda-Bold.ttf"), 14); // 2
		io.Fonts->AddFontFromFileTTF(enc("C:\\Windows\\Fonts\\Ruda-Bold.ttf"), 12); // 3
		io.Fonts->AddFontFromFileTTF(enc("C:\\Windows\\Fonts\\Ruda-Bold.ttf"), 10); // 4
		once = true;
	}
}

}
}
