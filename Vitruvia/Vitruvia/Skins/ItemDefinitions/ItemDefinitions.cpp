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

#include "ItemDefinitions.h"

using namespace Source;
const std::unordered_map<size_t, Item_t> k_weapon_info =
{
	{ WEAPON_KNIFE,{ "models/weapons/v_knife_default_ct.mdl", "knife_default_ct" } },
	{ WEAPON_KNIFE_T,{ "models/weapons/v_knife_default_t.mdl", "knife_t" } },
	{ WEAPON_KNIFE_BAYONET,{ "models/weapons/v_knife_bayonet.mdl", "bayonet" } },
	{ WEAPON_KNIFE_FLIP,{ "models/weapons/v_knife_flip.mdl", "knife_flip" } },
	{ WEAPON_KNIFE_GUT,{ "models/weapons/v_knife_gut.mdl", "knife_gut" } },
	{ WEAPON_KNIFE_KARAMBIT,{ "models/weapons/v_knife_karam.mdl", "knife_karambit" } },
	{ WEAPON_KNIFE_M9_BAYONET,{ "models/weapons/v_knife_m9_bay.mdl", "knife_m9_bayonet" } },
	{ WEAPON_KNIFE_TACTICAL,{ "models/weapons/v_knife_tactical.mdl", "knife_tactical" } },
	{ WEAPON_KNIFE_FALCHION,{ "models/weapons/v_knife_falchion_advanced.mdl", "knife_falchion" } },
	{ WEAPON_KNIFE_BOWIE,{ "models/weapons/v_knife_survival_bowie.mdl", "knife_survival_bowie" } },
	{ WEAPON_KNIFE_BUTTERFLY,{ "models/weapons/v_knife_butterfly.mdl", "knife_butterfly" } },
	{ WEAPON_KNIFE_PUSH,{ "models/weapons/v_knife_push.mdl", "knife_push" } },
	{ GLOVE_STUDDED_BLOODHOUND,{ "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl" } },
	{ GLOVE_T_SIDE,{ "models/weapons/v_models/arms/glove_fingerless/v_glove_fingerless.mdl" } },
	{ GLOVE_CT_SIDE,{ "models/weapons/v_models/arms/glove_hardknuckle/v_glove_hardknuckle.mdl" } },
	{ GLOVE_SPORTY,{ "models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl" } },
	{ GLOVE_SLICK,{ "models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl" } },
	{ GLOVE_LEATHER_WRAP,{ "models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl" } },
	{ GLOVE_MOTORCYCLE,{ "models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl" } },
	{ GLOVE_SPECIALIST,{ "models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl" } },
	{ GLOVE_HYDRA,{ "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound_hydra.mdl" } }
};

const std::vector<WeaponName_t> k_knife_names =
{
	{ 0, "Default" },
	{ WEAPON_KNIFE_BAYONET, "Bayonet" },
	{ WEAPON_KNIFE_FLIP, "Flip Knife" },
	{ WEAPON_KNIFE_GUT, "Gut Knife" },
	{ WEAPON_KNIFE_KARAMBIT, "Karambit" },
	{ WEAPON_KNIFE_M9_BAYONET, "M9 Bayonet" },
	{ WEAPON_KNIFE_TACTICAL, "Huntsman Knife" },
	{ WEAPON_KNIFE_FALCHION, "Falchion Knife" },
	{ WEAPON_KNIFE_BOWIE, "Bowie Knife" },
	{ WEAPON_KNIFE_BUTTERFLY, "Butterfly Knife" },
	{ WEAPON_KNIFE_PUSH, "Shadow Daggers" }
};

const std::vector<WeaponName_t> k_glove_names =
{
	{ 0, "Default" },
	{ GLOVE_STUDDED_BLOODHOUND, "Bloodhound" },
	{ GLOVE_T_SIDE, "Default (Terrorists)" },
	{ GLOVE_CT_SIDE, "Default (Counter-Terrorists)" },
	{ GLOVE_SPORTY, "Sporty" },
	{ GLOVE_SLICK, "Slick" },
	{ GLOVE_LEATHER_WRAP, "Handwrap" },
	{ GLOVE_MOTORCYCLE, "Motorcycle" },
	{ GLOVE_SPECIALIST, "Specialist" },
	{ GLOVE_HYDRA, "Hydra" }
};

const std::vector<WeaponName_t> k_weapon_names =
{
	{ WEAPON_AK47, "AK-47" },
	{ WEAPON_AUG, "AUG" },
	{ WEAPON_AWP, "AWP" },
	{ WEAPON_CZ75A, "CZ75 Auto" },
	{ WEAPON_DEAGLE, "Desert Eagle" },
	{ WEAPON_ELITE, "Dual Berettas" },
	{ WEAPON_FAMAS, "FAMAS" },
	{ WEAPON_FIVESEVEN, "Five-SeveN" },
	{ WEAPON_G3SG1, "G3SG1" },
	{ WEAPON_GALILAR, "Galil AR" },
	{ WEAPON_GLOCK, "Glock-18" },
	{ WEAPON_M249, "M249" },
	{ WEAPON_M4A1_SILENCER, "M4A1-S" },
	{ WEAPON_M4A4, "M4A4" },
	{ WEAPON_MAC10, "MAC-10" },
	{ WEAPON_MAG7, "MAG-7" },
	{ WEAPON_MP7, "MP7" },
	{ WEAPON_MP9, "MP9" },
	{ WEAPON_NEGEV, "Negev" },
	{ WEAPON_NOVA, "Nova" },
	{ WEAPON_HKP2000, "P2000" },
	{ WEAPON_P250, "P250" },
	{ WEAPON_P90, "P90" },
	{ WEAPON_BIZON, "PP-Bizon" },
	{ WEAPON_REVOLVER, "R8 Revolver" },
	{ WEAPON_SAWEDOFF, "Sawed-Off" },
	{ WEAPON_SCAR20, "SCAR-20" },
	{ WEAPON_SSG08, "SSG 08" },
	{ WEAPON_SG556, "SG 553" },
	{ WEAPON_TEC9, "Tec-9" },
	{ WEAPON_UMP45, "UMP-45" },
	{ WEAPON_USP_SILENCER, "USP-S" },
	{ WEAPON_XM1014, "XM1014" },
	{ WEAPON_KNIFE_BAYONET, "Bayonet" },
	{ WEAPON_KNIFE_FLIP, "Flip Knife" },
	{ WEAPON_KNIFE_GUT, "Gut Knife" },
	{ WEAPON_KNIFE_KARAMBIT, "Karambit" },
	{ WEAPON_KNIFE_M9_BAYONET, "M9 Bayonet" },
	{ WEAPON_KNIFE_TACTICAL, "Huntsman Knife" },
	{ WEAPON_KNIFE_FALCHION, "Falchion Knife" },
	{ WEAPON_KNIFE_BOWIE, "Bowie Knife" },
	{ WEAPON_KNIFE_BUTTERFLY, "Butterfly Knife" },
	{ WEAPON_KNIFE_PUSH, "Shadow Daggers" }
};