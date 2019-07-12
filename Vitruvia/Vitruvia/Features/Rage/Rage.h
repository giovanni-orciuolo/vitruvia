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
#include "../Legit/Legit.h" // To access Target_t class
#include "../HvH/HvH.h" // To access HvH shit
#include "../../Config/Config.h" // To access Configs

namespace Vitruvia {

class F_Rage : public V_IFeature
{
public:
	F_Rage()
		: m_pCmd(nullptr), m_Targets({}), m_BestTarget(Target_t()), m_posTarget(Source::Vector()), m_iTargetTick(0),
		  m_Matrix({}), m_bIsFiring(false), m_iShotsFired(0), m_flLastFireTime(0)
	{
	}

	bool PriorChecks(); // My prior checks
	// Return multiple points on the edge of an hitbox
	static bool GetPointsForHitbox(Source::C_CSPlayer* player, int hitbox_id, BoneMatrix boneMatrix, std::vector<Source::Vector>& out_points);

	void FillTargets(); // Will fill m_Targets with all the players that meet filters, not ordered yet!
	void FindBestTarget(); // Order m_Targets based on SortBy, first one will be the best target
	void ShootBestTarget(); // Apply checks to m_BestTarget and finally shoot at him!
	Source::Vector GetSpot() { return this->m_posTarget; }
	Target_t GetBestTarget() { return m_BestTarget; }
	int GetTargetTick() { return this->m_iTargetTick; }

	void OnCreateMove(Source::CUserCmd* cmd) override;
	void OnEndScene(Render::D3D::DrawManager* graphics) override;
	void OnFrameStageNotify(Source::ClientFrameStage_t stage) override;
	void OnListenedEvent(Source::IGameEvent* game_event) override;
private:
	Source::CUserCmd* m_pCmd; // Cmd to work with, passed from CreateMove
	std::vector<Target_t> m_Targets; // Will hold all good targets ordered by SortBy predicate
	Target_t m_BestTarget; // Will hold the current very best target
	Source::Vector m_posTarget; // Current position of the target vector (used inside AimSpot)
	int m_iTargetTick; // Backtrack target tick
	BoneMatrix m_Matrix; // Backtracked matrix
	bool m_bIsFiring; // True if the local player is firing this tick
	int m_iShotsFired; // Increments while firing
	float m_flLastFireTime; // Last time where the local player did fire
};

extern F_Rage* Rage;

}