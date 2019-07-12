#include "../Hooks.h"

SOURCE_INIT
namespace Hooks
{

bool __fastcall hk_OverrideConfig(IMaterialSystem* _this, void* edx, MaterialSystem_Config_t* config, bool force_update)
{
	int renderMode = Vitruvia::Config::VisualsOther.RenderMode.m_Value;
	switch (renderMode)
	{
	case 0: // Default
		break;
	case 1: // Full Gray
		config->m_bDrawGray = true;
		break;
	case 2: // Full Bright
		config->m_nFullbright = 1;
		break;
	case 3: // Minecraft
		config->m_bShowLowResImage = true;
		break;
	case 4: // Rainbow MLG
		config->m_nShowMipLevels = 1;
		break;
	}

	return MaterialSystemTable->GetFunction<OverrideConfig_t>(I_OverrideConfig)(_this, config, force_update);
}

}
SOURCE_END