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
#include "C_BaseAnimating.h"

SOURCE_INIT

class C_AnimationLayer
{
private:
	char pad[20];
public:
	uint32_t m_nOrder;					//0x0014
	uint32_t m_nSequence;				//0x0018
	float m_flPrevCycle;				//0x001C
	float m_flWeight;					//0x0020
	float m_flWeightDeltaRate;			//0x0024
	float m_flPlaybackRate;				//0x0028
	float m_flCycle;					//0x002C
	C_BaseAnimating* m_pOwner;			//0x0030
private:
	char  pad_0038[4]; //0x0034
}; //Size: 0x0038

class C_BaseAnimatingOverlay : public C_BaseAnimating
{
public:
	enum { MAX_OVERLAYS = 13 };
	// Retrieve the animation layer index-based (max is 13)
	C_AnimationLayer& GetAnimOverlay(int i);
	// Based on a sequence, get the activity
	Activity GetSequenceActivity(int sequence);
	// Get animation layer, dump a string based on it
	std::string DumpAnimOverlay(int i);
};

SOURCE_END