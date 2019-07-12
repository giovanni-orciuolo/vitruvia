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

#include "../../Hooks.h"
#include "../../../SourceClasses/C_CSPlayer.h"

SOURCE_INIT
namespace Hooks
{

enum ESequence
{
	SEQUENCE_DEFAULT_DRAW = 0,
	SEQUENCE_DEFAULT_IDLE1 = 1,
	SEQUENCE_DEFAULT_IDLE2 = 2,
	SEQUENCE_DEFAULT_LIGHT_MISS1 = 3,
	SEQUENCE_DEFAULT_LIGHT_MISS2 = 4,
	SEQUENCE_DEFAULT_HEAVY_MISS1 = 9,
	SEQUENCE_DEFAULT_HEAVY_HIT1 = 10,
	SEQUENCE_DEFAULT_HEAVY_BACKSTAB = 11,
	SEQUENCE_DEFAULT_LOOKAT01 = 12,

	SEQUENCE_BUTTERFLY_DRAW = 0,
	SEQUENCE_BUTTERFLY_DRAW2 = 1,
	SEQUENCE_BUTTERFLY_LOOKAT01 = 13,
	SEQUENCE_BUTTERFLY_LOOKAT03 = 15,

	SEQUENCE_FALCHION_IDLE1 = 1,
	SEQUENCE_FALCHION_HEAVY_MISS1 = 8,
	SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP = 9,
	SEQUENCE_FALCHION_LOOKAT01 = 12,
	SEQUENCE_FALCHION_LOOKAT02 = 13,

	SEQUENCE_DAGGERS_IDLE1 = 1,
	SEQUENCE_DAGGERS_LIGHT_MISS1 = 2,
	SEQUENCE_DAGGERS_LIGHT_MISS5 = 6,
	SEQUENCE_DAGGERS_HEAVY_MISS2 = 11,
	SEQUENCE_DAGGERS_HEAVY_MISS1 = 12,

	SEQUENCE_BOWIE_IDLE1 = 1,
};

inline int RandomSequence(int low, int high)
{
	return rand() % (high - low + 1) + low;
}

const static std::unordered_map<std::string, int(*)(int)> animation_fix_map
{
	{"models/weapons/v_knife_butterfly.mdl", [](int sequence) -> int
		{
			switch(sequence)
			{
			case SEQUENCE_DEFAULT_DRAW:
				return RandomSequence(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
			case SEQUENCE_DEFAULT_LOOKAT01:
				return RandomSequence(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
			default:
				return sequence + 1;
			}
		}},
	{"models/weapons/v_knife_falchion_advanced.mdl", [](int sequence) -> int
		{
			switch(sequence)
			{
			case SEQUENCE_DEFAULT_IDLE2:
				return SEQUENCE_FALCHION_IDLE1;
			case SEQUENCE_DEFAULT_HEAVY_MISS1:
				return RandomSequence(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP);
			case SEQUENCE_DEFAULT_LOOKAT01:
				return RandomSequence(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02);
			case SEQUENCE_DEFAULT_DRAW:
			case SEQUENCE_DEFAULT_IDLE1:
				return sequence;
			default:
				return sequence - 1;
			}
		}},
	{"models/weapons/v_knife_push.mdl", [](int sequence) -> int
		{
			switch(sequence)
			{
			case SEQUENCE_DEFAULT_IDLE2:
				return SEQUENCE_DAGGERS_IDLE1;
			case SEQUENCE_DEFAULT_LIGHT_MISS1:
			case SEQUENCE_DEFAULT_LIGHT_MISS2:
				return RandomSequence(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5);
			case SEQUENCE_DEFAULT_HEAVY_MISS1:
				return RandomSequence(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1);
			case SEQUENCE_DEFAULT_HEAVY_HIT1:
			case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
			case SEQUENCE_DEFAULT_LOOKAT01:
				return sequence + 3;
			case SEQUENCE_DEFAULT_DRAW:
			case SEQUENCE_DEFAULT_IDLE1:
				return sequence;
			default:
				return sequence + 2;
			}
		}},
	{"models/weapons/v_knife_survival_bowie.mdl", [](int sequence) -> int
		{
			switch(sequence)
			{
			case SEQUENCE_DEFAULT_DRAW:
			case SEQUENCE_DEFAULT_IDLE1:
				return sequence;
			case SEQUENCE_DEFAULT_IDLE2:
				return SEQUENCE_BOWIE_IDLE1;
			default:
				return sequence - 1;
			}
		}}
};

void hk_nSequence(const CRecvProxyData* pData, void* pStruct, void* pOut)
{
	static auto original_fn = m_nSequence->GetOriginalFunction();

	auto local = C_CSPlayer::GetLocal();

	if (!local || local->m_lifeState() != LIFE_ALIVE)
		return original_fn(pData, pStruct, pOut);

	// Remove the constness from the proxy data allowing us to make changes.
	auto proxy_data = const_cast<CRecvProxyData*>(pData);

	auto view_model = static_cast<C_BaseViewModel*>(pStruct);

	if (view_model && view_model->m_hOwner() != INVALID_EHANDLE_INDEX)
	{
		auto owner = (C_CSPlayer*)(Source::EntityList->GetClientEntityFromHandle(view_model->m_hOwner()));

		if (owner == C_CSPlayer::GetLocal())
		{
			// Get the filename of the current view model.
			auto knife_model = Source::ModelInfo->GetModel(view_model->m_nModelIndex());
			auto model_name = Source::ModelInfo->GetModelName(knife_model);

			if (animation_fix_map.count(model_name))
			{
				proxy_data->m_Value.m_Int = animation_fix_map.at(model_name)(proxy_data->m_Value.m_Int);
				// Sync the animations with the server
				view_model->SendViewModelMatchingSequence(proxy_data->m_Value.m_Int);
			}
		}
	}

	// Call the original function with our edited data.
	original_fn(proxy_data, pStruct, pOut);
}

}
SOURCE_END