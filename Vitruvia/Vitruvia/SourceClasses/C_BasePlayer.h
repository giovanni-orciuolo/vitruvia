// Class built with love by Vitruvia :*
// Build date: Sunday 19/3/2017

#pragma once
#include "C_BaseCombatCharacter.h"

SOURCE_INIT

class C_BaseViewModel
{
public:
	IClientNetworkable* GetNetworkable() { return MAKE_PTR<IClientNetworkable>(this, 0x8); }

	inline void SetModelIndex(int model_index)
	{
		return CallVFunction<void(__thiscall*)(void*, int)>(this, 75)(this, model_index);
	}
	void SendViewModelMatchingSequence(int sequence)
	{
		return CallVFunction<void(__thiscall*)(void*, int)>(this, 241)(this, sequence);
	}
	/*
	m_nModelIndex[0x254](int)
	m_hWeapon[0x29B8](int)
	m_nSkin[0xA1C](int)
	m_nBody[0xA20](int)
	m_nSequence[0x28AC](int)
	m_nViewModelIndex[0x29B0](int)
	m_flPlaybackRate[0xA18](float)
	m_fEffects[0xEC](int)
	m_nAnimationParity[0x29B4](int)
	m_hOwner[0x29BC](int)
	m_nNewSequenceParity[0xA38](int)
	m_nResetEventsParity[0xA3C](int)
	m_nMuzzleFlashParity[0xA58](int)
	m_bShouldIgnoreOffsetAndAccuracy[0x2980](int)
	*/
	MAKE_NETVAR(int, m_nModelIndex, "m_nModelIndex", "DT_BaseViewModel")
	MAKE_NETVAR(CBaseHandle, m_hOwner, "m_hOwner", "DT_BaseViewModel")
};

class C_BasePlayer : public C_BaseCombatCharacter
{
public:
	FORCEINLINE bool IsAlive() { return m_lifeState() == LIFE_ALIVE; }

	C_BasePlayer* GetObserverTarget()
	{
		static offset_t observer_target = GET_NETVAR(enc("DT_BasePlayer"), enc("m_hObserverTarget"));
		return (C_BasePlayer*)Source::EntityList->GetClientEntityFromHandle(*MAKE_PTR<Source::CHandle<C_BasePlayer>>((void*)this, observer_target));
	}
	CUserCmd** m_pCurrentCommand()
	{
		static offset_t offset = GET_NETVAR(enc("DT_BasePlayer"), enc("m_hConstraintEntity")) - 0x4;
		return (CUserCmd**)(std::uintptr_t(this) + offset);
	}

	MAKE_NETVAR(int, m_iHideHUD, "m_iHideHUD", "DT_BasePlayer")
	MAKE_NETVAR(float, m_flFOVRate, "m_flFOVRate", "DT_BasePlayer")
	MAKE_NETVAR(bool, m_bDucked, "m_bDucked", "DT_BasePlayer")
	MAKE_NETVAR(bool, m_bDucking, "m_bDucking", "DT_BasePlayer")
	MAKE_NETVAR(float, m_flLastDuckTime, "m_flLastDuckTime", "DT_BasePlayer")
	MAKE_NETVAR(bool, m_bInDuckJump, "m_bInDuckJump", "DT_BasePlayer")
	MAKE_NETVAR(int, m_nDuckTimeMsecs, "m_nDuckTimeMsecs", "DT_BasePlayer")
	MAKE_NETVAR(int, m_nDuckJumpTimeMsecs, "m_nDuckJumpTimeMsecs", "DT_BasePlayer")
	MAKE_NETVAR(int, m_nJumpTimeMsecs, "m_nJumpTimeMsecs", "DT_BasePlayer")
	MAKE_NETVAR(float, m_flFallVelocity, "m_flFallVelocity", "DT_BasePlayer")
	MAKE_NETVAR(Vector, m_viewPunchAngle, "m_viewPunchAngle", "DT_BasePlayer")
	MAKE_NETVAR_PTR(Vector, m_viewPunchAnglePtr, "m_viewPunchAngle", "DT_BasePlayer")
	MAKE_NETVAR(Vector, m_aimPunchAngle, "m_aimPunchAngle", "DT_BasePlayer")
	MAKE_NETVAR_PTR(Vector, m_aimPunchAnglePtr, "m_aimPunchAngle", "DT_BasePlayer")
	MAKE_NETVAR(Vector, m_aimPunchAngleVel, "m_aimPunchAngleVel", "DT_BasePlayer")
	MAKE_NETVAR(bool, m_bDrawViewmodel, "m_bDrawViewmodel", "DT_BasePlayer")
	MAKE_NETVAR(bool, m_bWearingSuit, "m_bWearingSuit", "DT_BasePlayer")
	MAKE_NETVAR(bool, m_bPoisoned, "m_bPoisoned", "DT_BasePlayer")
	MAKE_NETVAR(float, m_flStepSize, "m_flStepSize", "DT_BasePlayer")
	MAKE_NETVAR(bool, m_bAllowAutoMovement, "m_bAllowAutoMovement", "DT_BasePlayer")
	MAKE_NETVAR(int, m_skybox3d_scale, "m_skybox3d.scale", "DT_BasePlayer")
	MAKE_NETVAR(Vector, m_skybox3d_origin, "m_skybox3d.origin", "DT_BasePlayer")
	MAKE_NETVAR(int, m_skybox3d_area, "m_skybox3d.area", "DT_BasePlayer")
	MAKE_NETVAR(int, m_skybox3d_fog_enable, "m_skybox3d.fog.enable", "DT_BasePlayer")
	MAKE_NETVAR(int, m_skybox3d_fog_blend, "m_skybox3d.fog.blend", "DT_BasePlayer")
	MAKE_NETVAR(Vector, m_skybox3d_fog_dirPrimary, "m_skybox3d.fog.dirPrimary", "DT_BasePlayer")
	MAKE_NETVAR(int, m_skybox3d_fog_colorPrimary, "m_skybox3d.fog.colorPrimary", "DT_BasePlayer")
	MAKE_NETVAR(int, m_skybox3d_fog_colorSecondary, "m_skybox3d.fog.colorSecondary", "DT_BasePlayer")
	MAKE_NETVAR(float, m_skybox3d_fog_start, "m_skybox3d.fog.start", "DT_BasePlayer")
	MAKE_NETVAR(float, m_skybox3d_fog_end, "m_skybox3d.fog.end", "DT_BasePlayer")
	MAKE_NETVAR(float, m_skybox3d_fog_maxdensity, "m_skybox3d.fog.maxdensity", "DT_BasePlayer")
	MAKE_NETVAR(float, m_skybox3d_fog_HDRColorScale, "m_skybox3d.fog.HDRColorScale", "DT_BasePlayer")
	MAKE_NETVAR(Vector, m_audio_localSound_0, "m_audio.localSound[0]", "DT_BasePlayer")
	MAKE_NETVAR(Vector, m_audio_localSound_1, "m_audio.localSound[1]", "DT_BasePlayer")
	MAKE_NETVAR(Vector, m_audio_localSound_2, "m_audio.localSound[2]", "DT_BasePlayer")
	MAKE_NETVAR(Vector, m_audio_localSound_3, "m_audio.localSound[3]", "DT_BasePlayer")
	MAKE_NETVAR(Vector, m_audio_localSound_4, "m_audio.localSound[4]", "DT_BasePlayer")
	MAKE_NETVAR(Vector, m_audio_localSound_5, "m_audio.localSound[5]", "DT_BasePlayer")
	MAKE_NETVAR(Vector, m_audio_localSound_6, "m_audio.localSound[6]", "DT_BasePlayer")
	MAKE_NETVAR(Vector, m_audio_localSound_7, "m_audio.localSound[7]", "DT_BasePlayer")
	MAKE_NETVAR(int, m_audio_soundscapeIndex, "m_audio.soundscapeIndex", "DT_BasePlayer")
	MAKE_NETVAR(int, m_audio_localBits, "m_audio.localBits", "DT_BasePlayer")
	MAKE_NETVAR(int, m_audio_entIndex, "m_audio.entIndex", "DT_BasePlayer")
	MAKE_NETVAR(Vector, m_vecViewOffset, "m_vecViewOffset[0]", "DT_BasePlayer")
	MAKE_NETVAR(float, m_vecViewOffset_0, "m_vecViewOffset[0]", "DT_BasePlayer")
	MAKE_NETVAR(float, m_vecViewOffset_1, "m_vecViewOffset[1]", "DT_BasePlayer")
	MAKE_NETVAR(float, m_vecViewOffset_2, "m_vecViewOffset[2]", "DT_BasePlayer")
	MAKE_NETVAR(float, m_flFriction, "m_flFriction", "DT_BasePlayer")
	MAKE_NETVAR(int, m_fOnTarget, "m_fOnTarget", "DT_BasePlayer")
	MAKE_NETVAR(int, m_nTickBase, "m_nTickBase", "DT_BasePlayer")
	MAKE_NETVAR_PTR(int, m_nTickBasePtr, "m_nTickBase", "DT_BasePlayer")
	MAKE_NETVAR(int, m_nNextThinkTick, "m_nNextThinkTick", "DT_BasePlayer")
	MAKE_NETVAR(EHANDLE, m_hLastWeapon, "m_hLastWeapon", "DT_BasePlayer")
	MAKE_NETVAR(Vector, m_vecVelocity, "m_vecVelocity[0]", "DT_BasePlayer")
	MAKE_NETVAR(float, m_vecVelocity_0, "m_vecVelocity[0]", "DT_BasePlayer")
	MAKE_NETVAR(float, m_vecVelocity_1, "m_vecVelocity[1]", "DT_BasePlayer")
	MAKE_NETVAR(float, m_vecVelocity_2, "m_vecVelocity[2]", "DT_BasePlayer")
	MAKE_NETVAR(Vector, m_vecBaseVelocity, "m_vecBaseVelocity", "DT_BasePlayer")
	MAKE_NETVAR(EHANDLE, m_hConstraintEntity, "m_hConstraintEntity", "DT_BasePlayer")
	MAKE_NETVAR(Vector, m_vecConstraintCenter, "m_vecConstraintCenter", "DT_BasePlayer")
	MAKE_NETVAR(float, m_flConstraintRadius, "m_flConstraintRadius", "DT_BasePlayer")
	MAKE_NETVAR(float, m_flConstraintWidth, "m_flConstraintWidth", "DT_BasePlayer")
	MAKE_NETVAR(float, m_flConstraintSpeedFactor, "m_flConstraintSpeedFactor", "DT_BasePlayer")
	MAKE_NETVAR(bool, m_bConstraintPastRadius, "m_bConstraintPastRadius", "DT_BasePlayer")
	MAKE_NETVAR(float, m_flDeathTime, "m_flDeathTime", "DT_BasePlayer")
	MAKE_NETVAR(float, m_flNextDecalTime, "m_flNextDecalTime", "DT_BasePlayer")
	MAKE_NETVAR(float, m_fForceTeam, "m_fForceTeam", "DT_BasePlayer")
	MAKE_NETVAR(int, m_nWaterLevel, "m_nWaterLevel", "DT_BasePlayer")
	MAKE_NETVAR(float, m_flLaggedMovementValue, "m_flLaggedMovementValue", "DT_BasePlayer")
	MAKE_NETVAR(EHANDLE, m_hTonemapController, "m_hTonemapController", "DT_BasePlayer")
	MAKE_NETVAR(int, deadflag, "deadflag", "DT_BasePlayer")
	MAKE_NETVAR(int, m_iFOV, "m_iFOV", "DT_BasePlayer")
	MAKE_NETVAR(int, m_iFOVStart, "m_iFOVStart", "DT_BasePlayer")
	MAKE_NETVAR(float, m_flFOVTime, "m_flFOVTime", "DT_BasePlayer")
	MAKE_NETVAR(int, m_iDefaultFOV, "m_iDefaultFOV", "DT_BasePlayer")
	MAKE_NETVAR(EHANDLE, m_hZoomOwner, "m_hZoomOwner", "DT_BasePlayer")
	MAKE_NETVAR(int, m_afPhysicsFlags, "m_afPhysicsFlags", "DT_BasePlayer")
	MAKE_NETVAR(EHANDLE, m_hVehicle, "m_hVehicle", "DT_BasePlayer")
	MAKE_NETVAR(EHANDLE, m_hUseEntity, "m_hUseEntity", "DT_BasePlayer")
	MAKE_NETVAR(EHANDLE, m_hGroundEntity, "m_hGroundEntity", "DT_BasePlayer")
	MAKE_NETVAR(int, m_iHealth, "m_iHealth", "DT_BasePlayer")
	MAKE_NETVAR(int, m_lifeState, "m_lifeState", "DT_BasePlayer")
	MAKE_NETVAR(int, m_iBonusProgress, "m_iBonusProgress", "DT_BasePlayer")
	MAKE_NETVAR(int, m_iBonusChallenge, "m_iBonusChallenge", "DT_BasePlayer")
	MAKE_NETVAR(float, m_flMaxspeed, "m_flMaxspeed", "DT_BasePlayer")
	MAKE_NETVAR(int, m_fFlags, "m_fFlags", "DT_BasePlayer")
	MAKE_NETVAR_PTR(int, m_fFlagsPtr, "m_fFlags", "DT_BasePlayer")
	MAKE_NETVAR(int, m_iObserverMode, "m_iObserverMode", "DT_BasePlayer")
	MAKE_NETVAR_PTR(int, m_iObserverModePtr, "m_iObserverMode", "DT_BasePlayer")
	MAKE_NETVAR(bool, m_bActiveCameraMan, "m_bActiveCameraMan", "DT_BasePlayer")
	MAKE_NETVAR(bool, m_bCameraManXRay, "m_bCameraManXRay", "DT_BasePlayer")
	MAKE_NETVAR(bool, m_bCameraManOverview, "m_bCameraManOverview", "DT_BasePlayer")
	MAKE_NETVAR(bool, m_bCameraManScoreBoard, "m_bCameraManScoreBoard", "DT_BasePlayer")
	MAKE_NETVAR(int, m_uCameraManGraphs, "m_uCameraManGraphs", "DT_BasePlayer")
	MAKE_NETVAR(int, m_iDeathPostEffect, "m_iDeathPostEffect", "DT_BasePlayer")
	MAKE_NETVAR(EHANDLE, m_hObserverTarget, "m_hObserverTarget", "DT_BasePlayer")
	MAKE_NETVAR(EHANDLE, m_hViewModel, "m_hViewModel[0]", "DT_BasePlayer")
	MAKE_NETVAR(int, m_iCoachingTeam, "m_iCoachingTeam", "DT_BasePlayer")
	MAKE_NETVAR_PTR(char, m_szLastPlaceName, "m_szLastPlaceName", "DT_BasePlayer") // Size = 18
	MAKE_NETVAR(Vector, m_vecLadderNormal, "m_vecLadderNormal", "DT_BasePlayer")
	MAKE_NETVAR(int, m_ladderSurfaceProps, "m_ladderSurfaceProps", "DT_BasePlayer")
	MAKE_NETVAR(int, m_ubEFNoInterpParity, "m_ubEFNoInterpParity", "DT_BasePlayer")
	MAKE_NETVAR(EHANDLE, m_hPostProcessCtrl, "m_hPostProcessCtrl", "DT_BasePlayer")
	MAKE_NETVAR(EHANDLE, m_hColorCorrectionCtrl, "m_hColorCorrectionCtrl", "DT_BasePlayer")
	MAKE_NETVAR(UINT, m_PlayerFog_m_hCtrl, "m_PlayerFog.m_hCtrl", "DT_BasePlayer")
	MAKE_NETVAR(int, m_vphysicsCollisionState, "m_vphysicsCollisionState", "DT_BasePlayer")
	MAKE_NETVAR(EHANDLE, m_hViewEntity, "m_hViewEntity", "DT_BasePlayer")
	MAKE_NETVAR(bool, m_bShouldDrawPlayerWhileUsingViewEntity, "m_bShouldDrawPlayerWhileUsingViewEntity", "DT_BasePlayer")
	MAKE_NETVAR(float, m_flDuckAmount, "m_flDuckAmount", "DT_BasePlayer")
	MAKE_NETVAR(float, m_flDuckSpeed, "m_flDuckSpeed", "DT_BasePlayer")
};

SOURCE_END