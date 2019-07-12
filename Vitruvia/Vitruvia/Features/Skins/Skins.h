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

#pragma once
#include "../IFeature.h"
#include "../../SourceClasses/C_CSPlayer.h"

namespace Vitruvia {

class F_SkinChanger : public V_IFeature
{
private:
	void ForceSkinConfig(Source::C_CSPlayer* local_player, std::vector<Source::C_WeaponCSBase*> weapons);
	void ForceKnifeConfig(Source::C_CSPlayer* local_player, Source::C_WeaponCSBase* active_weapon);
	void ForceStickers(Source::C_WeaponCSBase* weapon);
	void ForceGloves();
	void ForceInventory(Source::C_WeaponCSBase* weapon);
	void ApplySkinConfig(Source::C_WeaponCSBase* weapon, Config::EconomyItem economy_item, int xuid);
public:
	void OnFrameStageNotify(Source::ClientFrameStage_t stage) override;
	void OnFireEvent(Source::IGameEvent* game_event) override;
};

extern F_SkinChanger* SkinChanger;

}