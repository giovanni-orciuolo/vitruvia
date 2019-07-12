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

#include "Skins.h"

enum class EStickerAttributeType
{
	Index,
	Wear,
	Scale,
	Rotation
};
static uint16_t s_econ_item_interface_wrapper_offset = 0;

namespace Hooks
{
	static void* oGetStickerAttributeBySlotIndexFloat;

	static float __fastcall GetStickerAttributeBySlotIndexFloat(void* thisptr, void*, int slot, EStickerAttributeType attribute, float unknown)
	{
		auto item = rcast<Source::C_WeaponCSBase*>(uintptr_t(thisptr) - s_econ_item_interface_wrapper_offset);

		auto item_definition_index = *item->m_iItemDefinitionIndex();
		auto config = Vitruvia::Config::SkinConfig[item_definition_index];

		switch (attribute)
		{
		case EStickerAttributeType::Wear:
			return config.Stickers[slot].Wear;
		case EStickerAttributeType::Scale:
			return config.Stickers[slot].Scale;
		case EStickerAttributeType::Rotation:
			return config.Stickers[slot].Rotation;
		default:
			break;
		}

		return rcast<decltype(GetStickerAttributeBySlotIndexFloat)*>(oGetStickerAttributeBySlotIndexFloat)(thisptr, nullptr, slot, attribute, unknown);
	}

	static void* oGetStickerAttributeBySlotIndexInt;

	static unsigned int __fastcall GetStickerAttributeBySlotIndexInt(void* thisptr, void*, int slot, EStickerAttributeType attribute, unsigned unknown)
	{
		auto item = rcast<Source::C_WeaponCSBase*>(uintptr_t(thisptr) - s_econ_item_interface_wrapper_offset);

		if (attribute == EStickerAttributeType::Index)
		{
			auto item_definition_index = *item->m_iItemDefinitionIndex();
			auto config = Vitruvia::Config::SkinConfig[item_definition_index];
			return config.Stickers[slot].Index;
		}

		return reinterpret_cast<decltype(GetStickerAttributeBySlotIndexInt)*>(oGetStickerAttributeBySlotIndexInt)(thisptr, nullptr, slot, attribute, unknown);
	}
}

namespace Vitruvia
{
F_SkinChanger* SkinChanger;
void F_SkinChanger::ApplySkinConfig(Source::C_WeaponCSBase* weapon, Config::EconomyItem economy_item, int xuid)
{
	// Modify the weapon according to the skin config
	// Also modify the inventory if it needs to
	bool modifyInventory = *weapon->m_nFallbackPaintKit() != economy_item.PaintKit;

	// Force fallback values
	*weapon->m_iItemIDHigh() = -1;

	// Set the account ID to be local player XUID to fix Stat-Trak
	*weapon->m_iAccountID() = xuid;

	// Set everything
	*weapon->m_nFallbackPaintKit() = economy_item.PaintKit;
	*weapon->m_nFallbackStatTrak() = economy_item.StatTrak;
	*weapon->m_flFallbackWear() = economy_item.Wear;
	*weapon->m_nFallbackSeed() = economy_item.Seed;
	if (strcmp(economy_item.CustomName, ""))
		strcpy(weapon->m_szCustomName(), economy_item.CustomName);

	// Force stickers
	ForceStickers(weapon);

	// Force inventory (working on it)
	//if (modifyInventory) ForceInventory(weapon);
}
void F_SkinChanger::ForceInventory(Source::C_WeaponCSBase* weapon)
{
	int defIndex = *weapon->m_iItemDefinitionIndex();
	// Check if this item is already present in the inventory
	auto objectCache = Source::PlayerInventory->GetBaseTypeCache();
	if (objectCache)
	{
		auto econItems = objectCache->GetEconItems();
		for (auto& item : econItems)
		{
			// Remove it if present, so I can replace it
			if (*item->GetDefIndex() == defIndex)
				Source::PlayerInventory->RemoveItem(item);
		}
	}
	// Add the new item
	Source::C_EconItem* item = Source::C_EconItem::CreateEconItem();
	*item->GetAccountID() = Source::PlayerInventory->GetSteamID();
	*item->GetDefIndex() = defIndex;
	*item->GetItemID() = Source::Maths::RandomInt(1, 50000);
	*item->GetInventory() = 1;
	*item->GetFlags() = 0;
	*item->GetOriginalID() = 0;
	item->SetStatTrak(*weapon->m_nFallbackStatTrak());
	item->SetPaintKit(*weapon->m_nFallbackPaintKit());
	item->SetPaintSeed(*weapon->m_nFallbackSeed());
	item->SetPaintWear(*weapon->m_flFallbackWear());
	if (strcmp(weapon->m_szCustomName(), ""))
		item->SetCustomName(weapon->m_szCustomName());
	auto stickers = Config::SkinConfig[*weapon->m_iItemDefinitionIndex()].Stickers;
	int index = 0;
	for (auto& sticker : stickers)
	{
		if (sticker.Index != 0) item->AddSticker(index, sticker.Index, sticker.Wear, sticker.Scale, sticker.Rotation);
		index++;
	}
	item->SetOrigin(24);
	item->SetRarity(Source::ITEM_RARITY_MYTHICAL);
	item->SetLevel(1);
	item->SetInUse(true);
	Source::PlayerInventory->AddEconItem(item);
}
void F_SkinChanger::ForceSkinConfig(Source::C_CSPlayer* local_player, std::vector<Source::C_WeaponCSBase*> weapons)
{
	using namespace Source;

	// For each weapon
	for (auto weapon : weapons)
	{
		// Get config for the weapon
		auto skin_config = Config::SkinConfig[*weapon->m_iItemDefinitionIndex()];
		this->ApplySkinConfig(weapon, skin_config, local_player->GetXuid());
	}
}
void F_SkinChanger::ForceKnifeConfig(Source::C_CSPlayer* local_player, Source::C_WeaponCSBase* active_weapon)
{
	using namespace Source;

	WeaponTypes weapon_type = active_weapon->GetWeaponInfo()->WeaponType;
	bool is_knife = weapon_type == WeaponTypes::WEAPONTYPE_KNIFE;

	// If this is a knife, we need to change it
	if (is_knife && Config::KnifeGlove.Knife.m_Value != 0)
	{
		// Grab info about the knife we want to change
		auto knife_info = k_weapon_info.at(Config::KnifeGlove.Knife.m_Value);
		int model_index = Source::ModelInfo->GetModelIndex(knife_info.model);
		// Set the model
		active_weapon->SetModelIndex(model_index);
		//active_weapon->GetNetworkable()->PreDataUpdate(DATA_UPDATE_CREATED);
		// Set the model for viewmodel
		auto view_model = local_player->GetViewModel();
		if (view_model)
		{
			view_model->SetModelIndex(Source::ModelInfo->GetModelIndex(knife_info.model));
			//view_model->GetNetworkable()->PreDataUpdate(DATA_UPDATE_CREATED);
		}
	}

	// We need to reset the knife...
	// Basically is the same thing as above but done with the default value based on our team
	if (is_knife && Config::KnifeGlove.Knife.m_Value == 0)
	{
		auto knife_info = k_weapon_info.at(local_player->m_iTeamNum() == TEAM_T ? WEAPON_KNIFE_T : WEAPON_KNIFE);
		int model_index = Source::ModelInfo->GetModelIndex(knife_info.model);
		// Set the model
		active_weapon->SetModelIndex(model_index);
		//active_weapon->GetNetworkable()->PreDataUpdate(DATA_UPDATE_CREATED);
		// Set the model for viewmodel
		auto view_model = local_player->GetViewModel();
		if (view_model)
		{
			view_model->SetModelIndex(model_index);
			//view_model->GetNetworkable()->PreDataUpdate(DATA_UPDATE_CREATED);
		}
	}
	
	// For each weapon
	for (auto weapon : local_player->GetWeapons())
	{
		// If knife, set item definition index
		if (weapon->GetWeaponInfo()->WeaponType == WeaponTypes::WEAPONTYPE_KNIFE)
		{
			(Config::KnifeGlove.Knife.m_Value != 0) ?
				*weapon->m_iItemDefinitionIndex() = Config::KnifeGlove.Knife.m_Value :
				*weapon->m_iItemDefinitionIndex() = local_player->m_iTeamNum() == TEAM_T ? WEAPON_KNIFE_T : WEAPON_KNIFE;
			auto skin_config = Config::SkinConfig[*weapon->m_iItemDefinitionIndex()];
			this->ApplySkinConfig(weapon, skin_config, local_player->GetXuid());
		}
	}
}
void F_SkinChanger::OnFrameStageNotify(Source::ClientFrameStage_t stage)
{
	using namespace Source;

	// If this isn't the right stage, return
	if (stage != FRAME_NET_UPDATE_POSTDATAUPDATE_START)
		return;
	// If skinchanger isn't active, return
	if (!Config::Skins.Active.m_Value)
		return;

	// Get local player
	auto local_player = C_CSPlayer::GetLocal();
	if (local_player == nullptr)
		return;

	// Get local player weapons
	auto weapons = local_player->GetWeapons();
	if (!weapons.empty())
	{
		auto active_weapon = local_player->GetActiveWeapon();
		if (active_weapon)
		{
			// Force the knife if it's the active weapon
			ForceKnifeConfig(local_player, active_weapon);

			// Force the weapons if paintkits are enabled
			if (Config::PaintKits.Active.m_Value)
				ForceSkinConfig(local_player, weapons);
		}
	}

	// Force the creation of new gloves where I will apply glove config
	if (Config::KnifeGlove.Glove > 0)
		ForceGloves();
}
void F_SkinChanger::OnFireEvent(Source::IGameEvent* game_event)
{
	using namespace Source;
	// Catch the event player_death, check if we are the killers, check if we used a knife, edit the event to edit killicon EZ
	// *skrt* *skrt* 777

	// Catch player_death only
	if (!strcmp(game_event->GetName(), enc("player_death")))
	{
		// Check if we are the killers
		if (Source::Engine->GetPlayerForUserID(game_event->GetInt(enc("attacker"))) == Source::Engine->GetLocalPlayer())
		{
			// If I used a knife
			// Check if contains "knife" or contains "bayonet"
			const char* weapon = game_event->GetString(enc("weapon"));
			if (strstr(weapon, enc("knife")) || !strcmp(weapon, enc("bayonet")))
			{
				// If knife index is 0, I don't need to adjust
				if (Config::KnifeGlove.Knife.m_Value != 0)
				{
					// I'm surely using a knife, replace
					// Grab info about the knife we want to change the icon
					auto knife_info = k_weapon_info.at(Config::KnifeGlove.Knife.m_Value);
					// Override
					game_event->SetString(enc("weapon"), knife_info.icon);
				}
			}
		}
	}
}
void F_SkinChanger::ForceStickers(Source::C_WeaponCSBase* weapon)
{
	if (!s_econ_item_interface_wrapper_offset)
		s_econ_item_interface_wrapper_offset = GET_NETVAR(enc("DT_EconEntity"), enc("m_Item")) + 0xC;

	void**& vmt = *rcast<void***>(uintptr_t(weapon) + s_econ_item_interface_wrapper_offset);

	static void** hooked_vmt = nullptr;

	if (!hooked_vmt)
	{
		size_t size = 0;

		while (IsCodePtr(vmt[size]))
			++size;

		hooked_vmt = new void*[size];
		memcpy(hooked_vmt, vmt, size * sizeof(void*));

		Hooks::oGetStickerAttributeBySlotIndexFloat = hooked_vmt[4];
		hooked_vmt[4] = rcast<void*>(&Hooks::GetStickerAttributeBySlotIndexFloat);

		Hooks::oGetStickerAttributeBySlotIndexInt = hooked_vmt[5];
		hooked_vmt[5] = rcast<void*>(&Hooks::GetStickerAttributeBySlotIndexInt);
	}

	vmt = hooked_vmt;
}
void F_SkinChanger::ForceGloves()
{
	using namespace Source;

	auto local = C_CSPlayer::GetLocal();

	// If the local player isn't alive, I need to remove the gloves otherwise I will get the weird effect
	if (!local->IsAlive())
	{
		// Get the gloves
		C_BaseAttributableItem* gloves = (C_BaseAttributableItem*)EntityList->GetClientEntity(local->m_hMyWearables()[0] & 0xFFF);
		if (gloves != nullptr) // If we actually have wearables
		{
			// Destroy the gloves
			gloves->GetNetworkable()->SetDestroyedOnRecreateEntities();
			gloves->GetNetworkable()->Release();
		}
		return; // Return prematurely
	}

	// Get my gloves
	C_BaseAttributableItem* gloves = (C_BaseAttributableItem*)EntityList->GetClientEntity(local->m_hMyWearables()[0] & 0xFFF);
	// If I get nullptr, it means that I need to create them
	if (gloves == nullptr)
	{
		// Create a new CEconWearable entity to add to the entity list
		int index = Source::EntityList->GetHighestEntityIndex() + 1, serial = rand() % 0x1000;
		auto new_gloves = ClientClass::NewClass(Source::BaseClient->GetAllClasses(),
			enc("CEconWearable"), index, serial);

		// Set my wearables
		local->m_hMyWearables()[0] = (index | serial << 16);

		// Now that the gloves are added to wearables, get them in a attributable form so I can set the vars
		// Warning: CASTING new_gloves to BaseAttributable is forbidden since that's still IClientNetworkable vtable
		C_BaseAttributableItem* my_gloves = (C_BaseAttributableItem*)EntityList->GetClientEntity(local->m_hMyWearables()[0] & 0xFFF);
		if (my_gloves != nullptr)
		{
			// Ghetto trick by namaszo (here just for the sake of who knows)
			//static const Vector out_of_world = Vector(10000.f, 10000.f, 10000.f);
			//rcast<C_BaseAttributableItem*>(new_gloves)->SetAbsOrigin(&out_of_world);
			*rcast<int*>(uintptr_t(my_gloves) + 0x64) = -1; // Force index to be -1 (ok, Beakers?)

			auto& glove_cfg = k_myGloves.at(Config::KnifeGlove.Glove.m_Value);

			// Assign variables to the new glove on the same frame!
			// This part is important because if I don't assign those vars in this frame the glove will get changed
			// only when I respawn, and it will disappear when I force a full update, which is not cool.
			*my_gloves->m_iItemIDHigh() = -1;
			*my_gloves->m_iAccountID() = local->GetXuid();
			*my_gloves->m_nFallbackPaintKit() = glove_cfg.paint_kit;
			*my_gloves->m_iItemDefinitionIndex() = glove_cfg.item_def_index;
			my_gloves->SetModelIndex(Source::ModelInfo->GetModelIndex(glove_cfg.mdl_path.c_str()));
			my_gloves->GetNetworkable()->PreDataUpdate(DATA_UPDATE_CREATED);
		}
	}
	else
	{
		*rcast<int*>(uintptr_t(gloves) + 0x64) = -1; // Force index to be -1 (ok, Beakers?)

		// If I don't get nullptr on this call it means that we already created our cool glove entity
		// I can start applying shit
		auto& glove_cfg = k_myGloves.at(Config::KnifeGlove.Glove.m_Value);

		// Assign variables to the new glove
		*gloves->m_iItemIDHigh() = -1;
		*gloves->m_iAccountID() = local->GetXuid();
		*gloves->m_nFallbackPaintKit() = glove_cfg.paint_kit;
		*gloves->m_iItemDefinitionIndex() = glove_cfg.item_def_index;
		gloves->SetModelIndex(Source::ModelInfo->GetModelIndex(glove_cfg.mdl_path.c_str()));
		gloves->GetNetworkable()->PreDataUpdate(DATA_UPDATE_CREATED);
	}
}

}
