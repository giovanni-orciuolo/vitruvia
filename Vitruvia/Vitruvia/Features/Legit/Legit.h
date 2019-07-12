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
#include "../../SourceClasses/C_CSPlayer.h"

namespace Vitruvia {

// Class to describe a target
class Target_t
{
public:
	
	// Default constructor, everything initialized to 0
	Target_t()
		: entity(nullptr), entity_index(0),
		distance(0.f), crosshair_distance(0.f),
		health(-1), threat(0), aimbot_angle(Source::QAngle(0, 0, 0)), hitbox(Source::Vector(0, 0, 0)), hitgroup(-1),
		tick_to_restore(-1)
	{
	}

	// Constructor that requires only a pointer to local player and a pointer to the target
	// Everything is calculated inside the constructor
	Target_t(Source::C_CSPlayer* me, Source::C_CSPlayer* target, const Source::QAngle& aim_angle, const Source::Vector& hitbox, int tick_to_restore = -1)
	{
		// Set the pointer to the entity
		this->entity = target;
		auto* networkable = target->GetNetworkable();
		if (networkable && rcast<offset_t>(networkable) != 0xF)
			this->entity_index = networkable->EntIndex();
		else
			this->entity_index = 0;

		// Calculate the distance
		this->distance = me->GetAbsOrigin().DistTo(target->GetAbsOrigin());

		// Calculate the crosshair distance (fov)
		Source::QAngle view_angles;
		Source::Engine->GetViewAngles(view_angles);
		this->crosshair_distance = Source::Maths::GetFov(view_angles, me->GetAbsOrigin(), target->GetAbsOrigin());

		// Get the health
		this->health = target->m_iHealth();

		// Calculate threat
		this->threat = this->health / this->distance;
		this->threat += target->IsTargetingMe() ? 100 : 0;

		// Store aimbot angle and hitbox position
		this->aimbot_angle = aim_angle;
		this->hitbox = hitbox;

		// It will be set later, hopefully
		this->hitgroup = -1;

		this->tick_to_restore = tick_to_restore;
	}

	// Used to reset a target
	void Reset()
	{
		this->entity = nullptr;
		this->entity_index = 0;
		this->distance = 0.0f;
		this->crosshair_distance = 0.0f;
		this->health = 0;
		this->threat = 0;
		this->aimbot_angle = Source::QAngle(0, 0, 0);
		this->hitbox = Source::Vector(0, 0, 0);
		this->hitgroup = -1;
		this->tick_to_restore = -1;
	}

	bool IsValid() { return this->entity != nullptr; }

	Source::C_CSPlayer* GetEntity() const { return this->entity; }
	float GetDistance() const { return this->distance; }
	float GetCrosshairDistance() const { return this->crosshair_distance; }
	int GetHealth() const { return this->health; }
	int GetThreat() const { return this->threat; }
	Source::QAngle GetAimbotAngle() const { return this->aimbot_angle; }
	Source::Vector GetHitboxPos() const { return this->hitbox; }
	int GetIndex() const { return this->entity_index; }
	int GetHitgroup() const { return this->hitgroup; }
	int GetTickToRestore() const { return this->tick_to_restore; }

	void SetHitgroup(int hitgroup) { this->hitgroup = hitgroup; }
private:
	Source::C_CSPlayer* entity;			// Pointer to the target entity
	int entity_index;					// Entity index
	float distance;						// How far I am from this target
	float crosshair_distance;			// Fov calculated from my crosshair to this target
	int health;							// Health of the target
	int threat;							// Threat value of the target
	Source::QAngle aimbot_angle;		// The angle that I need to aim to hit the target (PITCH, YAW)
	Source::Vector hitbox;				// Hitbox position (X,Y,Z)
	int hitgroup;						// Hitgroup based on hitbox
	int tick_to_restore;				// TickCount where to restore
};

class F_Legit : public V_IFeature
{
public:

	F_Legit()
		: m_TargetList({}), m_pCmd(nullptr), m_iSmartHitbox(-1)
	{
		this->m_BestTarget.Reset();
		this->m_OldTarget.Reset();
	}

	// Finds the best target. Returns false if no target is found
	bool FindTarget();
	// Aims at the best target
	void AimTarget();
	// Triggerbot logic here
	void ExecuteTriggerbot();
	// Standalone RCS logic here
	void ExecuteStandaloneRCS();
	// CreateMove behaviour
	void OnCreateMove(Source::CUserCmd* pCmd) override;

	Target_t GetBestTarget() { return this->m_BestTarget; }
	Target_t GetOldTarget() { return this->m_OldTarget; }

	// Get the nearest to the crosshair hitbox to aim at for this player
	// This skips hitboxes like legs or body (low)
	Source::Hitboxes GetNearestHitbox(Source::C_CSPlayer* player);

private:
	std::vector<Target_t> m_TargetList; // Array containing all the valid targets, to be sorted
	Target_t m_BestTarget; // Current best target
	Target_t m_OldTarget; // Best target on the previous tick
	Source::CUserCmd* m_pCmd; // Cmd to work with
	int m_iSmartHitbox; // Value that gets calculated by smart aim. -1 if smart aim is disabled
};

extern F_Legit* Legit;

}
