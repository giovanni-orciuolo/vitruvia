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

// Codenz by namazso bae <3

#include "KitParser.h"
#include "../../SourceClasses/C_BaseCombatWeapon.h"
using namespace Source;

#define GLOVE_BLOODHOUND_MDL enc("models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl")
#define GLOVE_SPECIALIST_MDL enc("models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl")
#define GLOVE_WRAP_MDL enc("models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl")
#define GLOVE_SPORTY_MDL enc("models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl")
#define GLOVE_SLICK_MDL enc("models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl")
#define GLOVE_MOTO_MDL enc("models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl")
#define GLOVE_HYDRA_MDL enc("models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound_hydra.mdl")

std::vector<Kit_t> k_skins{};
std::vector<Kit_t> k_gloves{};
std::vector<Kit_t> k_stickers{};

std::vector<Glove_t> k_myGloves = // UPDATE GLOVENZ HERE!
{
	{ "Default", 0, 0, "default_nogloves" },

	// Bloodhound
	{ "Bloodhound | Charred", 10006, GLOVE_STUDDED_BLOODHOUND, GLOVE_BLOODHOUND_MDL },
	{ "Bloodhound | Snakebite", 10007, GLOVE_STUDDED_BLOODHOUND, GLOVE_BLOODHOUND_MDL },
	{ "Bloodhound | Bronzed", 10008, GLOVE_STUDDED_BLOODHOUND, GLOVE_BLOODHOUND_MDL },
	{ "Bloodhound | Guerrilla", 10039, GLOVE_STUDDED_BLOODHOUND, GLOVE_BLOODHOUND_MDL },

	// Wraps
	{ "Wraps | Leather", 10009, GLOVE_LEATHER_WRAP, GLOVE_WRAP_MDL },
	{ "Wraps | Spruce DDPAT", 10010, GLOVE_LEATHER_WRAP, GLOVE_WRAP_MDL },
	{ "Wraps | Slaughter", 10021, GLOVE_LEATHER_WRAP, GLOVE_WRAP_MDL },
	{ "Wraps | Badlands", 10036, GLOVE_LEATHER_WRAP, GLOVE_WRAP_MDL },
	{ "Wraps | Cobalt Skulls", 10053, GLOVE_LEATHER_WRAP, GLOVE_WRAP_MDL },
	{ "Wraps | Overprint", 10054, GLOVE_LEATHER_WRAP, GLOVE_WRAP_MDL },
	{ "Wraps | Duct Tape", 10055, GLOVE_LEATHER_WRAP, GLOVE_WRAP_MDL },
	{ "Wraps | Arboreal", 10056, GLOVE_LEATHER_WRAP, GLOVE_WRAP_MDL },

	// Driver
	{ "Driver | Lunar Weave", 10013, GLOVE_SLICK, GLOVE_SLICK_MDL },
	{ "Driver | Convoy", 10015, GLOVE_SLICK, GLOVE_SLICK_MDL },
	{ "Driver | Crimson Weave", 10016, GLOVE_SLICK, GLOVE_SLICK_MDL },
	{ "Driver | Diamondback", 10013, GLOVE_SLICK, GLOVE_SLICK_MDL },
	{ "Driver | Overtake", 10043, GLOVE_SLICK, GLOVE_SLICK_MDL },
	{ "Driver | Racing Green", 10044, GLOVE_SLICK, GLOVE_SLICK_MDL },
	{ "Driver | King Snake", 10041, GLOVE_SLICK, GLOVE_SLICK_MDL },
	{ "Driver | Imperial Plaid", 10042, GLOVE_SLICK, GLOVE_SLICK_MDL },

	// Sport
	{ "Sport | Hedge Maze", 10038, GLOVE_SPORTY, GLOVE_SPORTY_MDL },
	{ "Sport | Pandoras Box", 10037, GLOVE_SPORTY, GLOVE_SPORTY_MDL },
	{ "Sport | Superconductor", 10018, GLOVE_SPORTY, GLOVE_SPORTY_MDL },
	{ "Sport | Arid", 10019, GLOVE_SPORTY, GLOVE_SPORTY_MDL },
	{ "Sport | Vice", 10048, GLOVE_SPORTY, GLOVE_SPORTY_MDL },
	{ "Sport | Omega", 10047, GLOVE_SPORTY, GLOVE_SPORTY_MDL },
	{ "Sport | Amphibious", 10045, GLOVE_SPORTY, GLOVE_SPORTY_MDL },
	{ "Sport | Bronze Morph", 10046, GLOVE_SPORTY, GLOVE_SPORTY_MDL },

	// Moto
	{ "Moto | Eclipse", 10024, GLOVE_MOTORCYCLE, GLOVE_MOTO_MDL },
	{ "Moto | Spear Mint", 10026, GLOVE_MOTORCYCLE, GLOVE_MOTO_MDL },
	{ "Moto | Boom!", 10027, GLOVE_MOTORCYCLE, GLOVE_MOTO_MDL },
	{ "Moto | Cool Mint", 10028, GLOVE_MOTORCYCLE, GLOVE_MOTO_MDL },
	{ "Moto | POW!", 10049, GLOVE_MOTORCYCLE, GLOVE_MOTO_MDL },
	{ "Moto | Turtle", 10050, GLOVE_MOTORCYCLE, GLOVE_MOTO_MDL },
	{ "Moto | Transport", 10051, GLOVE_MOTORCYCLE, GLOVE_MOTO_MDL },
	{ "Moto | Polygon", 10052, GLOVE_MOTORCYCLE, GLOVE_MOTO_MDL },

	// Specialist
	{ "Specialist | Forest DDPAT", 10030, GLOVE_SPECIALIST, GLOVE_SPECIALIST_MDL },
	{ "Specialist | Crimson Kimono", 10033, GLOVE_SPECIALIST, GLOVE_SPECIALIST_MDL },
	{ "Specialist | Emerald Web", 10034, GLOVE_SPECIALIST, GLOVE_SPECIALIST_MDL },
	{ "Specialist | Foundation", 10035, GLOVE_SPECIALIST, GLOVE_SPECIALIST_MDL },
	{ "Specialist | Crimson Web", 10061, GLOVE_SPECIALIST, GLOVE_SPECIALIST_MDL },
	{ "Specialist | Buckshot", 10062, GLOVE_SPECIALIST, GLOVE_SPECIALIST_MDL },
	{ "Specialist | Fade", 10063, GLOVE_SPECIALIST, GLOVE_SPECIALIST_MDL },
	{ "Specialist | Mogul", 10064, GLOVE_SPECIALIST, GLOVE_SPECIALIST_MDL },

	// Hydra
	{ "Hydra | Emerald", 10057, GLOVE_HYDRA, GLOVE_HYDRA_MDL },
	{ "Hydra | Mangrove", 10058, GLOVE_HYDRA, GLOVE_HYDRA_MDL },
	{ "Hydra | Rattler", 10059, GLOVE_HYDRA, GLOVE_HYDRA_MDL },
	{ "Hydra | Case Hardened", 10060, GLOVE_HYDRA, GLOVE_HYDRA_MDL },
};

void GrabKits()
{
	Log(enc("Parsing kits from game..."));

	// Initialize a static converter to convert from wchar_t (everything I grab is in wchar_t)
	static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	// Search the relative calls

	// call    ItemSystem
	// push    dword ptr [esi+0Ch]
	// lea     ecx, [eax+4]
	// call    CEconItemSchema::GetPaintKitDefinition

	auto sig_address = FindPattern(enc("client.dll"), enc("E8 ? ? ? ? FF 76 0C 8D 48 04 E8"));

	// Skip the opcode, read rel32 address
	auto item_system_offset = *reinterpret_cast<int32_t*>(sig_address + 1);

	// Add the offset to the end of the instruction
	auto item_system_fn = reinterpret_cast<CCStrike15ItemSystem* (*)()>(sig_address + 5 + item_system_offset);

	// Skip VTable, first member variable of ItemSystem is ItemSchema
	auto item_schema = reinterpret_cast<CCStrike15ItemSchema*>(uintptr_t(item_system_fn()) + sizeof(void*));

	// Dump paint kits
	{
		Log(enc("Starting paint kits dump"));
		// Skip the instructions between, skip the opcode, read rel32 address
		auto get_paint_kit_definition_offset = *reinterpret_cast<int32_t*>(sig_address + 11 + 1);

		// Add the offset to the end of the instruction
		auto get_paint_kit_definition_fn = reinterpret_cast<CPaintKit* (__thiscall *)(CCStrike15ItemSchema*, int)>(sig_address + 11 + 5 + get_paint_kit_definition_offset);

		// The last offset is head_element, we need that

		// push    ebp
		// mov     ebp, esp
		// sub     esp, 0Ch
		// mov     eax, [ecx+298h]

		// Skip instructions, skip opcode, read offset
		auto start_element_offset = *reinterpret_cast<intptr_t*>(uintptr_t(get_paint_kit_definition_fn) + 8 + 2);

		// Calculate head base from start_element's offset
		auto head_offset = start_element_offset - 12;

		auto map_head = reinterpret_cast<Head_t<int, CPaintKit*>*>(uintptr_t(item_schema) + head_offset);

		for (int i = 0; i <= map_head->last_element; ++i)
		{
			auto paint_kit = map_head->memory[i].value;

			if (paint_kit->id == 9001)
				continue;

			const wchar_t* wide_name = Source::Localize->Find(paint_kit->item_name.buffer + 1);
			auto name = converter.to_bytes(wide_name);

			if (paint_kit->id < 10000)
				k_skins.push_back({ paint_kit->id, name });
			else
				k_gloves.push_back({ paint_kit->id, name });
		}

		std::sort(k_skins.begin(), k_skins.end());
		Log(enc("Paint kits dump ended. %d elements parsed"), map_head->last_element);
	}

	// Dump sticker kits
	{
		Log(enc("Starting sticker dump"));
		auto sticker_sig = FindPattern(enc("client.dll"), enc("53 8D 48 04 E8 ? ? ? ? 8B 4D 10")) + 4;

		// Skip the opcode, read rel32 address
		auto get_sticker_kit_definition_offset = *reinterpret_cast<intptr_t*>(sticker_sig + 1);

		// Add the offset to the end of the instruction
		auto get_sticker_kit_definition_fn = reinterpret_cast<CPaintKit* (__thiscall *)(CCStrike15ItemSchema*, int)>(sticker_sig + 5 + get_sticker_kit_definition_offset);

		// The last offset is head_element, we need that

		//	push    ebp
		//	mov     ebp, esp
		//	push    ebx
		//	push    esi
		//	push    edi
		//	mov     edi, ecx
		//	mov     eax, [edi + 2BCh]

		// Skip instructions, skip opcode, read offset
		auto start_element_offset = *reinterpret_cast<intptr_t*>(uintptr_t(get_sticker_kit_definition_fn) + 8 + 2);

		// Calculate head base from start_element's offset
		auto head_offset = start_element_offset - 12;

		auto map_head = reinterpret_cast<Head_t<int, CStickerKit*>*>(uintptr_t(item_schema) + head_offset);

		for (int i = 0; i <= map_head->last_element; ++i)
		{
			auto sticker_kit = map_head->memory[i].value;

			char translation_error_buffer[64];

			auto sticker_name_ptr = sticker_kit->item_name.buffer + 1;

			if (strstr(sticker_name_ptr, enc("StickerKit_dhw2014_dignitas")))
			{
				strcpy_s(translation_error_buffer, enc("StickerKit_dhw2014_teamdignitas"));
				strcat_s(translation_error_buffer, sticker_name_ptr + 27);
				sticker_name_ptr = translation_error_buffer;
			}

			const wchar_t* wide_name = Source::Localize->Find(sticker_name_ptr);
			auto name = converter.to_bytes(wide_name);

			k_stickers.push_back({ sticker_kit->id, name });
		}

		std::sort(k_stickers.begin(), k_stickers.end());

		k_stickers.insert(k_stickers.begin(), { 0, "None" });
		Log(enc("Sticker dump ended. %d elements parsed"), map_head->last_element);
	}

	Log(enc("Done."));
}