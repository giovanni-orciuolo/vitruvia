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

#include "../Hooks.h"
#include "../../GUI/ImGui/DX9/imgui_impl_dx9.h"

bool g_vecPressedKeys[256]{};
extern LRESULT ImGui_ImplDX9_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

SOURCE_INIT
namespace Hooks {

LRESULT __stdcall hk_WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	using namespace Vitruvia;

	// Capture key states
	switch (msg)
	{
	// Handle mouse buttons down/up states
	case WM_LBUTTONDOWN:
		g_vecPressedKeys[VK_LBUTTON] = true;
		break;
	case WM_LBUTTONUP:
		g_vecPressedKeys[VK_LBUTTON] = false;
		break;
	case WM_RBUTTONDOWN:
		g_vecPressedKeys[VK_RBUTTON] = true;
		break;
	case WM_RBUTTONUP:
		g_vecPressedKeys[VK_RBUTTON] = false;
		break;
	// Handle keyboard state
	case WM_KEYDOWN:
		g_vecPressedKeys[wparam] = true;
		break;
	case WM_KEYUP:
		g_vecPressedKeys[wparam] = false;
		break;
	default: break;
	}

	// ìInsert' toggles the menu
	static bool isDown = false;
	static bool isClicked = false;
	if (g_vecPressedKeys[VK_INSERT])
	{
		isClicked = false;
		isDown = true;
	}
	else if (!g_vecPressedKeys[VK_INSERT] && isDown)
	{
		isClicked = true;
		isDown = false;
	}
	else
	{
		isClicked = false;
		isDown = false;
	}

	if (isClicked)
	{
		g_bMenuClosing = !g_bMenuClosing;
		g_bMenuOpened = !g_bMenuOpened;
		static ConVar* cl_mouseenable = Source::CVar->FindVar(enc("cl_mouseenable"));
		cl_mouseenable->SetValue(!g_bMenuOpened);
	}

	if (g_bMenuInitialized && g_bMenuOpened && !k_shouldListen && ImGui_ImplDX9_WndProcHandler(hwnd, msg, wparam, lparam))
		return true;

	return CallWindowProc(old_WndProc, hwnd, msg, wparam, lparam);
}

int __fastcall hk_IN_KeyEvent(IBaseClientDLL* _this, void* edx, int event_code, ButtonCode_t key, const char* key_binding)
{
	// If the key is valid
	if (event_code && key != ButtonCode_t::BUTTON_CODE_INVALID)
	{
		//Log(enc("KeyEvent: %s [bind: %s]"), Source::InputSystem->ButtonCodeToString(key), key_binding);
		// If keyreader is ready to catch the key
		if (k_shouldListen)
		{
			if (key != ButtonCode_t::KEY_BACKSPACE && key != ButtonCode_t::KEY_ESCAPE)
			{
				// Catch it!
				*k_keyOutput = key;
			}
			else
			{
				// Reset
				*k_keyOutput = 0;
			}
			k_shouldListen = false;
			// Reset input state on the key doesn't get registered in time
			Source::InputSystem->ResetInputState();
			return 0;
		}

		// Call OnKeyEvent for each feature
		for (auto f : Vitruvia::Features)
			f->OnKeyEvent(event_code, key, key_binding);
	}

	return BaseClientTable->GetFunction<IN_KeyEvent_t>(I_IN_KeyEvent)(_this, event_code, key, key_binding);
}

}
SOURCE_END