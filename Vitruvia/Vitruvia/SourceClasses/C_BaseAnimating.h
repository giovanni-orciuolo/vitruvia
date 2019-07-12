// Class built with love by Vitruvia :*
// Build date: Sunday 19/3/2017

#pragma once
#include "C_BaseEntity.h"

SOURCE_INIT

enum
{
	NUM_POSEPAREMETERS = 24,
	NUM_BONECTRLS = 4
};

class C_BaseAnimating;

class CBoneAccessor
{
public:
	// Only used in the client DLL for debug verification.
	const C_BaseAnimating* m_pAnimating;

	matrix3x4_t* m_pBones;

	int m_ReadableBones;		// Which bones can be read.
	int m_WritableBones;		// Which bones can be written.
};

class C_BaseAnimating : public C_BaseEntity
{
public:
	enum
	{
		NUM_POSEPAREMETERS = 24,
		NUM_BONECTRLS = 4
	};

	CBoneAccessor& GetBoneAccessor()
	{
		// Bone accessor is located at m_nForceBone + 0x1C. Inside it, there is the BoneMatrix that externals uses!
		// See here: https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/game/client/c_baseanimating.h#L509
		// m_BoneAccessor is 12 bytes away in the repo, 28 bytes in the actual game from m_nForceBone which is networked
		static offset_t m_BoneAccessor = GET_NETVAR(enc("DT_BaseAnimating"), enc("m_nForceBone")) + 0x1C;
		return *MAKE_PTR<CBoneAccessor>(this, m_BoneAccessor);
	}

	MAKE_NETVAR(int, m_nSequence, enc("m_nSequence"), enc("DT_BaseAnimating"))
	MAKE_NETVAR(int, m_nForceBone, enc("m_nForceBone"), enc("DT_BaseAnimating"))
	MAKE_NETVAR(Vector, m_vecForce, enc("m_vecForce"), enc("DT_BaseAnimating"))
	MAKE_NETVAR(int, m_nSkin, enc("m_nSkin"), enc("DT_BaseAnimating"))
	MAKE_NETVAR(int, m_nBody, enc("m_nBody"), enc("DT_BaseAnimating"))
	MAKE_NETVAR(int, m_nHitboxSet, enc("m_nHitboxSet"), enc("DT_BaseAnimating"))
	MAKE_NETVAR(float, m_flModelScale, enc("m_flModelScale"), enc("DT_BaseAnimating"))
	MAKE_NETVAR_PTR(float, m_flPoseParameter, enc("m_flPoseParameter"), enc("DT_BaseAnimating"))
	MAKE_NETVAR(float, m_flPlaybackRate, enc("m_flPlaybackRate"), enc("DT_BaseAnimating"))
	MAKE_NETVAR(bool, m_bClientSideAnimation, enc("m_bClientSideAnimation"), enc("DT_BaseAnimating"))
	MAKE_NETVAR(bool, m_bClientSideFrameReset, enc("m_bClientSideFrameReset"), enc("DT_BaseAnimating"))
	MAKE_NETVAR(bool, m_bClientSideRagdoll, enc("m_bClientSideRagdoll"), enc("DT_BaseAnimating"))
	MAKE_NETVAR(int, m_nNewSequenceParity, enc("m_nNewSequenceParity"), enc("DT_BaseAnimating"))
	MAKE_NETVAR(int, m_nResetEventsParity, enc("m_nResetEventsParity"), enc("DT_BaseAnimating"))
	MAKE_NETVAR(int, m_nMuzzleFlashParity, enc("m_nMuzzleFlashParity"), enc("DT_BaseAnimating"))
	MAKE_NETVAR(EHANDLE, m_hLightingOrigin, enc("m_hLightingOrigin"), enc("DT_BaseAnimating"))
	MAKE_NETVAR(float, m_flCycle, enc("m_flCycle"), enc("DT_BaseAnimating"))
	MAKE_NETVAR(float, m_flFrozen, enc("m_flFrozen"), enc("DT_BaseAnimating"))
	MAKE_NETVAR(int, m_ScaleType, enc("m_ScaleType"), enc("DT_BaseAnimating"))
	MAKE_NETVAR(bool, m_bSuppressAnimSounds, enc("m_bSuppressAnimSounds"), enc("DT_BaseAnimating"))

	void SetPoseAngles(float pitch, float yaw)
	{
		float* pose = m_flPoseParameter();
		pose[11] = (pitch + 90) / 180;
		pose[2] = (yaw + 180) / 360;
	}
};

SOURCE_END