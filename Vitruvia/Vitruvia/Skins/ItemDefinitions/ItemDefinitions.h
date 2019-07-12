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

struct Item_t
{
	constexpr Item_t(const char* model, const char* icon = nullptr) :
		model(model),
		icon(icon)
	{}

	const char* model;
	const char* icon;
};

struct WeaponName_t
{
	constexpr WeaponName_t(int definition_index, const char* name) :
		definition_index(definition_index),
		name(name)
	{}

	int definition_index = 0;
	const char* name = nullptr;
};

extern const std::unordered_map<size_t, Item_t> k_weapon_info;
extern const std::vector<WeaponName_t> k_knife_names;
extern const std::vector<WeaponName_t> k_glove_names;
extern const std::vector<WeaponName_t> k_weapon_names;