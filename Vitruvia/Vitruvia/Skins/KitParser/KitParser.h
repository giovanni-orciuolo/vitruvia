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

#pragma once
#include "../../Vitruvia.h"
#include "../../InterfaceList/InterfaceList.h"

struct Kit_t
{
	int index;
	std::string name;
	bool operator < (const Kit_t& other) const { return (name < other.name); }
};
struct Glove_t
{
	Glove_t(const std::string& fancy_name, int paint_kit, int item_def, const std::string& mdl_path)
		: fancy_name(fancy_name),
		paint_kit(paint_kit),
		item_def_index(item_def),
		mdl_path(mdl_path) {}

	std::string fancy_name = ""; // For example: "Bloodhound | Charred"
	int paint_kit = -1; // Should always be > 10000
	int item_def_index = -1; // Should always be around 5000
	std::string mdl_path = ""; // Smth like "models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl"
};

extern std::vector<Kit_t> k_skins;
extern std::vector<Kit_t> k_gloves;
extern std::vector<Kit_t> k_stickers;

extern std::vector<Glove_t> k_myGloves;

class CCStrike15ItemSchema;
class CCStrike15ItemSystem;

template <typename Key, typename Value>
struct Node_t
{
	int previous_id;		//0x0000
	int next_id;			//0x0004
	void* _unknown_ptr;		//0x0008
	int _unknown;			//0x000C
	Key key;				//0x0010
	Value value;			//0x0014
};

template <typename Key, typename Value>
struct Head_t
{
	Node_t<Key, Value>* memory;		//0x0000
	int allocation_count;			//0x0004
	int grow_size;					//0x0008
	int start_element;				//0x000C
	int next_available;				//0x0010
	int _unknown;					//0x0014
	int last_element;				//0x0018
}; //Size=0x001C

// could use CUtlString but this is just easier and CUtlString isn't needed anywhere else
struct String_t
{
	char* buffer;	//0x0000
	int capacity;	//0x0004
	int grow_size;	//0x0008
	int length;		//0x000C
}; //Size=0x0010

struct CPaintKit
{
	int id;						//0x0000

	String_t name;				//0x0004
	String_t description;		//0x0014
	String_t item_name;			//0x0024
	String_t material_name;		//0x0034
	String_t image_inventory;	//0x0044

	char pad_0x0054[0x8C];		//0x0054
}; //Size=0x00E0

struct CStickerKit
{
	int id;

	int item_rarity;

	String_t name;
	String_t description;
	String_t item_name;
	String_t material_name;
	String_t image_inventory;

	int tournament_event_id;
	int tournament_team_id;
	int tournament_player_id;
	bool is_custom_sticker_material;

	float rotate_end;
	float rotate_start;

	float scale_min;
	float scale_max;

	float wear_min;
	float wear_max;

	String_t image_inventory2;
	String_t image_inventory_large;

	uint32_t pad0[4];
};

// This function is really heavy. Call it once to fill all the kit vectors
extern void GrabKits();