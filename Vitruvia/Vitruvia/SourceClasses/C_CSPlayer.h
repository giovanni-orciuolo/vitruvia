// Class built with love by Vitruvia :*
// Build date: Sunday 19/3/2017

#pragma once
#include "C_BasePlayer.h"
#include "C_BaseCombatWeapon.h"
#include "C_BaseAttributableItem.h"
#include "C_WeaponCSBase.h"
#include "C_World.h"
#include "../Offsets/Offsets.h"
#include "../InterfaceList/InterfaceList.h"

SOURCE_INIT

class C_CSPlayer : public C_BasePlayer
{
public:
	static FORCEINLINE C_CSPlayer* Get(int index) { return (C_CSPlayer*)(Source::EntityList->GetClientEntity(index)); }
	static FORCEINLINE C_CSPlayer* GetLocal() { return Get(Source::Engine->GetLocalPlayer()); }

	// Utility functions

	// Returns the eye position of the entity, which is origin + m_vecViewOffset
	Vector GetEyePosition();
	// Returns a pointer to the current active weapon the entity is holding
	C_WeaponCSBase* GetActiveWeapon();
	// Returns a pointer to the view model of the entity (LOCAL ONLY)
	C_BaseViewModel* GetViewModel();
	// Returns a fixed vector of pointers to weapons (WeaponList) that the entity has currently got
	std::vector<C_WeaponCSBase*> GetWeapons();
	// Returns a fixed vector of weapon names that the entity has currently got
	std::vector<std::string> GetWeaponNames();
	// Returns true if the entity is not the local player and it's aiming at the local player
	bool IsTargetingMe();
	// Returns the position in the world of an entity bone based on the bone id
	Vector GetBonePosition(int bone);
	// Returns a pointer to bbox of the entity hitbox
	mstudiobbox_t* GetStudioBox(int hitbox);
	// Returns the bone id based on hitbox id
	int GetBoneByHitbox(int hitbox);
	// Get the world position of an entity hitbox
	Vector GetHitboxPosition(int hitbox);
	// Get the world position of an entity hitbox based on a custom matrix
	Vector GetHitboxPositionCustom(int hitbox, BoneMatrix customMatrix);
	// Get hitbox name (doesn't work properly?)
	std::string GetHitboxName(int hitbox);
	// Get hitgroup by hitbox
	int GetHitgroupByHitbox(int hitbox);
	// Returns true if the entity hitbox is behind a smoke
	bool IsBehindSmoke(int hitbox);
	// Returns true if the entity hitbox is visible
	bool IsVisible(int hitbox);
	// Returns true if the active weapon isn't valid for the aimbot to work with
	bool GotInvalidWeapon();
	// Returns un-safe infos about the player. It is just a wrapper for IEngineClient::GetPlayerInfo()
	player_info_t GetPlayerInfo();
	// Returns the name of the player
	std::string GetPlayerName();
	// Returns true if the player is HLTV
	bool IsHLTV();
	// Returns true if the player is fakeplayer
	bool IsFakePlayer();
	// Returns xuid of the player
	int64_t GetXuid();
	// Returns userid of the player
	int GetUserId();
	// Returns the move type of the player
	MoveType_t GetMoveType();
	// Returns the clan tag of the player
	std::string GetClanTag();
	// Returns the probability to hit something accurately
	float GetHitchance(C_CSPlayer* entity_to_hit, QAngle my_angles);
	// Convenient
	bool IsValid();
	
//	MAKE_NETVAR(Vector, m_vecOrigin, "m_vecOrigin", "DT_CSPlayer")
	MAKE_NETVAR(float, m_flStamina, "m_flStamina", "DT_CSPlayer")
	MAKE_NETVAR(int, m_iDirection, "m_iDirection", "DT_CSPlayer")
	MAKE_NETVAR(int, m_iShotsFired, "m_iShotsFired", "DT_CSPlayer")
	MAKE_NETVAR(int, m_nNumFastDucks, "m_nNumFastDucks", "DT_CSPlayer")
	MAKE_NETVAR(bool, m_bDuckOverride, "m_bDuckOverride", "DT_CSPlayer")
	MAKE_NETVAR(float, m_flVelocityModifier, "m_flVelocityModifier", "DT_CSPlayer")
	MAKE_NETVAR(int, m_nQuestProgressReason, "m_nQuestProgressReason", "DT_CSPlayer")
	MAKE_NETVAR(float, m_angEyeAngles_0, "m_angEyeAngles[0]", "DT_CSPlayer")
	MAKE_NETVAR(float, m_angEyeAngles_1, "m_angEyeAngles[1]", "DT_CSPlayer")
	MAKE_NETVAR(int, m_iAddonBits, "m_iAddonBits", "DT_CSPlayer")
	MAKE_NETVAR(int, m_iPrimaryAddon, "m_iPrimaryAddon", "DT_CSPlayer")
	MAKE_NETVAR(int, m_iSecondaryAddon, "m_iSecondaryAddon", "DT_CSPlayer")
	MAKE_NETVAR(int, m_iThrowGrenadeCounter, "m_iThrowGrenadeCounter", "DT_CSPlayer")
	MAKE_NETVAR(bool, m_bWaitForNoAttack, "m_bWaitForNoAttack", "DT_CSPlayer")
	MAKE_NETVAR(bool, m_bIsRespawningForDMBonus, "m_bIsRespawningForDMBonus", "DT_CSPlayer")
	MAKE_NETVAR(int, m_iPlayerState, "m_iPlayerState", "DT_CSPlayer")
	MAKE_NETVAR(int, m_iAccount, "m_iAccount", "DT_CSPlayer")
	MAKE_NETVAR(int, m_iStartAccount, "m_iStartAccount", "DT_CSPlayer")
	MAKE_NETVAR(int, m_totalHitsOnServer, "m_totalHitsOnServer", "DT_CSPlayer")
	MAKE_NETVAR(bool, m_bInBombZone, "m_bInBombZone", "DT_CSPlayer")
	MAKE_NETVAR(bool, m_bInBuyZone, "m_bInBuyZone", "DT_CSPlayer")
	MAKE_NETVAR(bool, m_bInNoDefuseArea, "m_bInNoDefuseArea", "DT_CSPlayer")
	MAKE_NETVAR(bool, m_bKilledByTaser, "m_bKilledByTaser", "DT_CSPlayer")
	MAKE_NETVAR(int, m_iMoveState, "m_iMoveState", "DT_CSPlayer")
	MAKE_NETVAR(int, m_iClass, "m_iClass", "DT_CSPlayer")
	MAKE_NETVAR(int, m_ArmorValue, "m_ArmorValue", "DT_CSPlayer")
	MAKE_NETVAR(Vector, m_angEyeAngles, "m_angEyeAngles", "DT_CSPlayer")
	MAKE_NETVAR_PTR(Vector, m_angEyeAnglesPtr, "m_angEyeAngles", "DT_CSPlayer")
	MAKE_NETVAR(bool, m_bHasDefuser, "m_bHasDefuser", "DT_CSPlayer")
	MAKE_NETVAR(bool, m_bNightVisionOn, "m_bNightVisionOn", "DT_CSPlayer")
	MAKE_NETVAR(bool, m_bHasNightVision, "m_bHasNightVision", "DT_CSPlayer")
	MAKE_NETVAR(bool, m_bInHostageRescueZone, "m_bInHostageRescueZone", "DT_CSPlayer")
	MAKE_NETVAR(bool, m_bIsDefusing, "m_bIsDefusing", "DT_CSPlayer")
	MAKE_NETVAR(bool, m_bIsGrabbingHostage, "m_bIsGrabbingHostage", "DT_CSPlayer")
	MAKE_NETVAR(bool, m_bIsScoped, "m_bIsScoped", "DT_CSPlayer")
	MAKE_NETVAR(bool, m_bIsWalking, "m_bIsWalking", "DT_CSPlayer")
	MAKE_NETVAR(bool, m_bResumeZoom, "m_bResumeZoom", "DT_CSPlayer")
	MAKE_NETVAR(float, m_fImmuneToGunGameDamageTime, "m_fImmuneToGunGameDamageTime", "DT_CSPlayer")
	MAKE_NETVAR(bool, m_bGunGameImmunity, "m_bGunGameImmunity", "DT_CSPlayer")
	MAKE_NETVAR(bool, m_bHasMovedSinceSpawn, "m_bHasMovedSinceSpawn", "DT_CSPlayer")
	MAKE_NETVAR(bool, m_bMadeFinalGunGameProgressiveKill, "m_bMadeFinalGunGameProgressiveKill", "DT_CSPlayer")
	MAKE_NETVAR(int, m_iGunGameProgressiveWeaponIndex, "m_iGunGameProgressiveWeaponIndex", "DT_CSPlayer")
	MAKE_NETVAR(int, m_iNumGunGameTRKillPoints, "m_iNumGunGameTRKillPoints", "DT_CSPlayer")
	MAKE_NETVAR(int, m_iNumGunGameKillsWithCurrentWeapon, "m_iNumGunGameKillsWithCurrentWeapon", "DT_CSPlayer")
	MAKE_NETVAR(int, m_iNumRoundKills, "m_iNumRoundKills", "DT_CSPlayer")
	MAKE_NETVAR(float, m_fMolotovUseTime, "m_fMolotovUseTime", "DT_CSPlayer")
	MAKE_NETVAR(float, m_fMolotovDamageTime, "m_fMolotovDamageTime", "DT_CSPlayer")
	MAKE_NETVAR_PTR(char, m_szArmsModel, "m_szArmsModel", "DT_CSPlayer") // Size = 256
	MAKE_NETVAR(EHANDLE, m_hCarriedHostage, "m_hCarriedHostage", "DT_CSPlayer")
	MAKE_NETVAR(EHANDLE, m_hCarriedHostageProp, "m_hCarriedHostageProp", "DT_CSPlayer")
	MAKE_NETVAR(bool, m_bIsRescuing, "m_bIsRescuing", "DT_CSPlayer")
	MAKE_NETVAR(float, m_flGroundAccelLinearFracLastTime, "m_flGroundAccelLinearFracLastTime", "DT_CSPlayer")
	MAKE_NETVAR(bool, m_bCanMoveDuringFreezePeriod, "m_bCanMoveDuringFreezePeriod", "DT_CSPlayer")
	MAKE_NETVAR(bool, m_isCurrentGunGameLeader, "m_isCurrentGunGameLeader", "DT_CSPlayer")
	MAKE_NETVAR(bool, m_isCurrentGunGameTeamLeader, "m_isCurrentGunGameTeamLeader", "DT_CSPlayer")
	MAKE_NETVAR(float, m_flGuardianTooFarDistFrac, "m_flGuardianTooFarDistFrac", "DT_CSPlayer")
	MAKE_NETVAR(float, m_flDetectedByEnemySensorTime, "m_flDetectedByEnemySensorTime", "DT_CSPlayer")
	MAKE_NETVAR(int, m_unMusicID, "m_unMusicID", "DT_CSPlayer")
	MAKE_NETVAR(bool, m_bHasHelmet, "m_bHasHelmet", "DT_CSPlayer")
	MAKE_NETVAR(bool, m_bHasHeavyArmor, "m_bHasHeavyArmor", "DT_CSPlayer")
	MAKE_NETVAR(int, m_nHeavyAssaultSuitCooldownRemaining, "m_nHeavyAssaultSuitCooldownRemaining", "DT_CSPlayer")
	MAKE_NETVAR(float, m_flFlashDuration, "m_flFlashDuration", "DT_CSPlayer")
	MAKE_NETVAR(float, m_flFlashMaxAlpha, "m_flFlashMaxAlpha", "DT_CSPlayer")
	MAKE_NETVAR(int, m_iProgressBarDuration, "m_iProgressBarDuration", "DT_CSPlayer")
	MAKE_NETVAR(float, m_flProgressBarStartTime, "m_flProgressBarStartTime", "DT_CSPlayer")
	MAKE_NETVAR(EHANDLE, m_hRagdoll, "m_hRagdoll", "DT_CSPlayer")
	MAKE_NETVAR(int, m_cycleLatch, "m_cycleLatch", "DT_CSPlayer")
	MAKE_NETVAR(int, m_unCurrentEquipmentValue, "m_unCurrentEquipmentValue", "DT_CSPlayer")
	MAKE_NETVAR(int, m_unRoundStartEquipmentValue, "m_unRoundStartEquipmentValue", "DT_CSPlayer")
	MAKE_NETVAR(int, m_unFreezetimeEndEquipmentValue, "m_unFreezetimeEndEquipmentValue", "DT_CSPlayer")
	MAKE_NETVAR(bool, m_bIsControllingBot, "m_bIsControllingBot", "DT_CSPlayer")
	MAKE_NETVAR(bool, m_bHasControlledBotThisRound, "m_bHasControlledBotThisRound", "DT_CSPlayer")
	MAKE_NETVAR(bool, m_bCanControlObservedBot, "m_bCanControlObservedBot", "DT_CSPlayer")
	MAKE_NETVAR(int, m_iControlledBotEntIndex, "m_iControlledBotEntIndex", "DT_CSPlayer")
	MAKE_NETVAR(bool, m_bIsAssassinationTarget, "m_bIsAssassinationTarget", "DT_CSPlayer")
	MAKE_NETVAR(bool, m_bHud_MiniScoreHidden, "m_bHud_MiniScoreHidden", "DT_CSPlayer")
	MAKE_NETVAR(bool, m_bHud_RadarHidden, "m_bHud_RadarHidden", "DT_CSPlayer")
	MAKE_NETVAR(int, m_nLastKillerIndex, "m_nLastKillerIndex", "DT_CSPlayer")
	MAKE_NETVAR(int, m_nLastConcurrentKilled, "m_nLastConcurrentKilled", "DT_CSPlayer")
	MAKE_NETVAR(int, m_nDeathCamMusic, "m_nDeathCamMusic", "DT_CSPlayer")
	MAKE_NETVAR(bool, m_bIsHoldingLookAtWeapon, "m_bIsHoldingLookAtWeapon", "DT_CSPlayer")
	MAKE_NETVAR(bool, m_bIsLookingAtWeapon, "m_bIsLookingAtWeapon", "DT_CSPlayer")
	MAKE_NETVAR(int, m_iNumRoundKillsHeadshots, "m_iNumRoundKillsHeadshots", "DT_CSPlayer")
	MAKE_NETVAR(float, m_flLowerBodyYawTarget, "m_flLowerBodyYawTarget", "DT_CSPlayer")
	MAKE_NETVAR_PTR(float, m_flLowerBodyYawTargetPtr, "m_flLowerBodyYawTarget", "DT_CSPlayer")
	MAKE_NETVAR(bool, m_bStrafing, "m_bStrafing", "DT_CSPlayer")
	MAKE_NETVAR(float, m_flThirdpersonRecoil, "m_flThirdpersonRecoil", "DT_CSPlayer")
};

SOURCE_END