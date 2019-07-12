// Class built with love by Vitruvia :*
// Build date: Friday 7/4/2017

#pragma once
#include "C_EconEntity.h"
#include "C_BaseAttributableItem.h"
#include "../InterfaceList/InterfaceList.h"

SOURCE_INIT

// Reclassed after new update (18/08/17) by @namaszo, @n0xius and @GeTLeFtX. Big +rep to them
struct WeaponInfo_t
{
	virtual ~WeaponInfo_t() {};

	/*
	Parse(KeyValues *, char const*)
	RefreshDynamicParameters(void)
	GetPrimaryClipSize(C_EconItemView const*, int, float)const
	GetSecondaryClipSize(C_EconItemView const*, int, float)const
	GetDefaultPrimaryClipSize(C_EconItemView const*, int, float)const
	GetDefaultSecondaryClipSize(C_EconItemView const*, int, float)const
	GetPrimaryReserveAmmoMax(C_EconItemView const*, int, float)const
	GetSecondaryReserveAmmoMax(C_EconItemView const*, int, float)const
	*/

	char*		consoleName;					// 0x0004
	char		pad_0008[12];					// 0x0008
	int			iMaxClip1;						// 0x0014
	int			iMaxClip2;						// 0x0018
	int			iDefaultClip1;					// 0x001C
	int			iDefaultClip2;					// 0x0020
	char		pad_0024[8];					// 0x0024
	char*		szWorldModel;					// 0x002C
	char*		szViewModel;					// 0x0030
	char*		szDroppedModel;					// 0x0034
	char		pad_0038[4];					// 0x0038
	char*		N0000023E;						// 0x003C
	char		pad_0040[56];					// 0x0040
	char*		szEmptySound;					// 0x0078
	char		pad_007C[4];					// 0x007C
	char*		szBulletType;					// 0x0080
	char		pad_0084[4];					// 0x0084
	char*		szHudName;						// 0x0088
	char*		szWeaponName;					// 0x008C
	char		pad_0090[56];					// 0x0090
	WeaponTypes WeaponType;						// 0x00C8
	int			iWeaponPrice;					// 0x00CC
	int			iKillAward;						// 0x00D0
	char*		szAnimationPrefix;				// 0x00D4
	float		flCycleTime;					// 0x00D8
	float		flCycleTimeAlt;					// 0x00DC
	float		flTimeToIdle;					// 0x00E0
	float		flIdleInterval;					// 0x00E4
	bool		bFullAuto;						// 0x00E8
	char		pad_0x00E5[3];					// 0x00E9
	int			iDamage;						// 0x00EC
	float		flArmorRatio;					// 0x00F0
	int			iBullets;						// 0x00F4
	float		flPenetration;					// 0x00F8
	float		flFlinchVelocityModifierLarge;	// 0x00FC
	float		flFlinchVelocityModifierSmall;	// 0x0100
	float		flRange;						// 0x0104
	float		flRangeModifier;				// 0x0108
	float		flThrowVelocity;				// 0x010C
	char		pad_0x010C[12];					// 0x0110
	bool		bHasSilencer;					// 0x011C
	char		pad_0x0119[3];					// 0x011D
	char*		pSilencerModel;					// 0x0120
	int			iCrosshairMinDistance;			// 0x0124
	int			iCrosshairDeltaDistance;		// 0x0128 - iTeam?
	float		flMaxPlayerSpeed;				// 0x012C
	float		flMaxPlayerSpeedAlt;			// 0x0130
	float		flSpread;						// 0x0134
	float		flSpreadAlt;					// 0x0138
	float		flInaccuracyCrouch;				// 0x013C
	float		flInaccuracyCrouchAlt;			// 0x0140
	float		flInaccuracyStand;				// 0x0144
	float		flInaccuracyStandAlt;			// 0x0148
	float		flInaccuracyJumpInitial;		// 0x014C
	float		flInaccuracyJump;				// 0x0150
	float		flInaccuracyJumpAlt;			// 0x0154
	float		flInaccuracyLand;				// 0x0158
	float		flInaccuracyLandAlt;			// 0x015C
	float		flInaccuracyLadder;				// 0x0160
	float		flInaccuracyLadderAlt;			// 0x0164
	float		flInaccuracyFire;				// 0x0168
	float		flInaccuracyFireAlt;			// 0x016C
	float		flInaccuracyMove;				// 0x0170
	float		flInaccuracyMoveAlt;			// 0x0174
	float		flInaccuracyReload;				// 0x0178
	int			iRecoilSeed;					// 0x017C
	float		flRecoilAngle;					// 0x0180
	float		flRecoilAngleAlt;				// 0x0184
	float		flRecoilAngleVariance;			// 0x0188
	float		flRecoilAngleVarianceAlt;		// 0x018C
	float		flRecoilMagnitude;				// 0x0190
	float		flRecoilMagnitudeAlt;			// 0x0194
	float		flRecoilMagnitudeVariance;		// 0x0198
	float		flRecoilMagnitudeVarianceAlt;	// 0x019C
	float		flRecoveryTimeCrouch;			// 0x01A0
	float		flRecoveryTimeStand;			// 0x01A4
	float		flRecoveryTimeCrouchFinal;		// 0x01A8
	float		flRecoveryTimeStandFinal;		// 0x01AC
	int			iRecoveryTransitionStartBullet;	// 0x01B0 
	int			iRecoveryTransitionEndBullet;	// 0x01B4
	bool		bUnzoomAfterShot;				// 0x01B8
	bool		bHideViewModelZoomed;			// 0x01B9
	char		pad_0x01B5[2];					// 0x01BA
	char		iZoomLevels[3];					// 0x01BC
	int			iZoomFOV[2];					// 0x01C0
	float		fZoomTime[3];					// 0x01C4
	char*		szWeaponClass;					// 0x01D4
	float		flAddonScale;					// 0x01D8
	char		pad_0x01DC[4];					// 0x01DC
	char*		szEjectBrassEffect;				// 0x01E0
	char*		szTracerEffect;					// 0x01E4
	int			iTracerFrequency;				// 0x01E8
	int			iTracerFrequencyAlt;			// 0x01EC
	char*		szMuzzleFlashEffect_1stPerson;	// 0x01F0
	char		pad_0x01F4[4];					// 0x01F4
	char*		szMuzzleFlashEffect_3rdPerson;	// 0x01F8
	char		pad_0x01FC[4];					// 0x01FC
	char*		szMuzzleSmokeEffect;			// 0x0200
	float		flHeatPerShot;					// 0x0204
	char*		szZoomInSound;					// 0x0208
	char*		szZoomOutSound;					// 0x020C
	float		flInaccuracyPitchShift;			// 0x0210
	float		flInaccuracySoundThreshold;		// 0x0214
	float		flBotAudibleRange;				// 0x0218
	char		pad_0x0218[8];					// 0x0220
	char*		pWrongTeamMsg;					// 0x0224
	bool		bHasBurstMode;					// 0x0228
	char		pad_0x0225[3];					// 0x0229
	bool		bIsRevolver;					// 0x022C
	bool		bCannotShootUnderwater;			// 0x0230
};

class CEconItemDefinition 
{
public:
	/*virtual const char * GetDefinitionIndex() = 0;
	virtual const char * GetPrefabName() = 0;
	virtual const char * GetItemBaseName() = 0;
	virtual const char * GetItemTypeName() = 0;
	virtual const char * GetItemDesc() = 0;
	virtual const char * GetInventoryImage() = 0;*/
};

class C_BaseCombatWeapon : public C_EconEntity
{
public:
	// Utility functions

	// Returns a null-terminated string containing the RAW name of the weapon (nothing fancy)
	const char* GetName();
	// Returns an old-style id for the weapon. ItemDefinitionIndex from C_BaseAttributableItem is way better
	int GetWeaponId();
	// Returns true if the weapon can fire
	bool CanFire(int tick_base);
	// Returns a pointer to weapon info. Usually this pointer is never nullptr, so I wouldn't be much paranoid
	WeaponInfo_t* GetWeaponInfo();
	// Returns the spread value of the weapon
	float GetSpread();
	// Returns the cone value of the weapon
	float GetCone();
	// Returns the current inaccuracy of the weapon
	float GetInaccuracy();
	// Forces an update of the accuracy penalty internally
	void UpdateAccuracyPenalty();
	// Sets the model index manually
	void SetModelIndex(int model_index);
	// Converts the weapon into a pointer to C_BaseAttributableItem*
	C_BaseAttributableItem* ToAttributable();
	// Get CEconItemDefinition pointer from m_Item netvar
	CEconItemDefinition* GetEconItemView();
	// Returns cool display name for this weapon (like weapon_glock -> Glock-18) and it's cool af lmao
	// name_tag = true when you want to return the name tag when it exists.
	std::string GetDisplayName(bool name_tag = false);
	// Update this item materials
	void ForceUpdate();

	MAKE_NETVAR(int, m_iPrimaryAmmoType, "m_iPrimaryAmmoType", "DT_BaseCombatWeapon")
	MAKE_NETVAR(int, m_iSecondaryAmmoType, "m_iSecondaryAmmoType", "DT_BaseCombatWeapon")
	MAKE_NETVAR(int, m_nViewModelIndex, "m_nViewModelIndex", "DT_BaseCombatWeapon")
	MAKE_NETVAR(bool, m_bFlipViewModel, "m_bFlipViewModel", "DT_BaseCombatWeapon")
	MAKE_NETVAR(int, m_iWeaponOrigin, "m_iWeaponOrigin", "DT_BaseCombatWeapon")
	MAKE_NETVAR(int, m_iWeaponModule, "m_iWeaponModule", "DT_BaseCombatWeapon")
	MAKE_NETVAR(float, m_flNextPrimaryAttack, "m_flNextPrimaryAttack", "DT_BaseCombatWeapon")
	MAKE_NETVAR(float, m_flNextSecondaryAttack, "m_flNextSecondaryAttack", "DT_BaseCombatWeapon")
	MAKE_NETVAR(int, m_nNextThinkTick, "m_nNextThinkTick", "DT_BaseCombatWeapon")
	MAKE_NETVAR(float, m_flTimeWeaponIdle, "m_flTimeWeaponIdle", "DT_BaseCombatWeapon")
	MAKE_NETVAR(int, m_iViewModelIndex, "m_iViewModelIndex", "DT_BaseCombatWeapon")
	MAKE_NETVAR(int, m_iWorldModelIndex, "m_iWorldModelIndex", "DT_BaseCombatWeapon")
	MAKE_NETVAR(int, m_iWorldDroppedModelIndex, "m_iWorldDroppedModelIndex", "DT_BaseCombatWeapon")
	MAKE_NETVAR(int, m_iState, "m_iState", "DT_BaseCombatWeapon")
	MAKE_NETVAR(EHANDLE, m_hOwner, "m_hOwner", "DT_BaseCombatWeapon")
	MAKE_NETVAR(int, m_iClip1, "m_iClip1", "DT_BaseCombatWeapon")
	MAKE_NETVAR(int, m_iClip2, "m_iClip2", "DT_BaseCombatWeapon")
	MAKE_NETVAR(int, m_iPrimaryReserveAmmoCount, "m_iPrimaryReserveAmmoCount", "DT_BaseCombatWeapon")
	MAKE_NETVAR(int, m_iSecondaryReserveAmmoCount, "m_iSecondaryReserveAmmoCount", "DT_BaseCombatWeapon")
	MAKE_NETVAR(EHANDLE, m_hWeaponWorldModel, "m_hWeaponWorldModel", "DT_BaseCombatWeapon")
	MAKE_NETVAR(int, m_iNumEmptyAttacks, "m_iNumEmptyAttacks", "DT_BaseCombatWeapon")
};

SOURCE_END
