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
#include "IFeature.h"
#include "Legit\Legit.h"
#include "Rage\Rage.h"
#include "HvH\HvH.h"
#include "Visuals\Visuals.h"
#include "Misc\Misc.h"
#include "Skins\Skins.h"

namespace Vitruvia {

// A complete list of all the features

std::vector<V_IFeature*> Features
{
	Legit = new F_Legit,
	Resolver = new F_Resolver,
	Backtrack = new F_Backtrack,
	Rage = new F_Rage,
	HvH = new F_HvH,
	Miscs = new F_Misc,
	NotificationSystem = new F_NotificationSystem,
	Visuals = new F_Visuals,
	Glow = new F_Glow,
	Chams = new F_Chams,
	Removals = new F_Removals,
	//Radar = new F_Radar, Needs to be fixed
	VisualsOthers = new F_VisualsOthers,
	SkinChanger = new F_SkinChanger
};

void ClearFeatures()
{
	for (auto f : Features)
		delete f;
	Features.clear();
}

}