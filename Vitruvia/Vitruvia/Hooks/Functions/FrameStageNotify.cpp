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
#include "../../Features/Visuals/Visuals.h"
#include "../../SourceClasses/C_BaseBeam.h"
#include "../../Globals/VitruviaGlobals.h"

SOURCE_INIT
namespace Hooks {

void __fastcall hk_FrameStageNotify(IBaseClientDLL* _this, void* edx, ClientFrameStage_t stage)
{
	for (auto f : Vitruvia::Features)
		f->OnFrameStageNotify(stage);

	/*if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_END)
	{
		C_EconItem* Item = C_EconItem::CreateEconItem();
		*Item->GetAccountID() = PlayerInventory->GetSteamID();
		*Item->GetDefIndex() = 7;
		*Item->GetItemID() = Maths::RandomInt(1, 50000);
		*Item->GetInventory() = 1;
		*Item->GetFlags() = 0;
		*Item->GetOriginalID() = 0;
		Item->AddSticker(0, 4, 0, 1, 1);
		Item->SetStatTrak(150);
		Item->SetPaintKit(37);
		Item->SetPaintSeed(0);
		Item->SetPaintWear(0);
		Item->SetOrigin(24);
		Item->SetRarity(ITEM_RARITY_MYTHICAL);
		Item->SetLevel(1);
		Item->SetInUse(true);
		PlayerInventory->AddEconItem(Item);
	}*/

	// Call the original, after our stuff is done
	BaseClientTable->GetFunction<FrameStageNotify_t>(I_FrameStageNotify)(_this, stage);

	for (auto f : Vitruvia::Features)
		f->OnFrameStageNotifyPostOriginal(stage);

	// Reset punch angles for no visual recoil
	Vitruvia::Removals->RestoreRecoilAngles();
}

}
SOURCE_END