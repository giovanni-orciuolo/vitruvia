// Class built with love by Vitruvia :*
// Build date: Sunday 19/3/2017

#pragma once
#include "../Vitruvia.h"
#include "../InterfaceList/InterfaceList.h"

#define MAKE_FIELD(prop_type, func_name, prop_name)										\
prop_type func_name()																	\
{																						\
    static offset_t field = this->FindDataMapOffset(this->GetPredDescMap(), prop_name);	\
	return *MAKE_PTR<prop_type>(this, field);											\
}

SOURCE_INIT

class C_BaseEntity
{
public:
	// Base functions
	IClientRenderable* GetRenderable() { return MAKE_PTR<IClientRenderable>(this, 0x4); }
	IClientNetworkable* GetNetworkable() { return MAKE_PTR<IClientNetworkable>(this, 0x8); }
	FORCEINLINE int GetIndex() { return *MAKE_PTR<int>(this, 0x64); } // Quick and fast
	ICollideable* GetCollideable()
	{
		return CallVFunction<ICollideable*(__thiscall*)(void*)>(this, 3)(this);
	}
	Vector& GetAbsOrigin()
	{
		return CallVFunction<Vector&(__thiscall*)(void*)>(this, 10)(this);
	}
	Vector& GetAbsAngles()
	{
		return CallVFunction<Vector&(__thiscall*)(void*)>(this, 11)(this);
	}
	void SetAbsOrigin(const Vector* origin)
	{
		using SetAbsOrigin_t = void(__thiscall*)(void*, const Vector*);
		static SetAbsOrigin_t set_abs_origin = rcast<SetAbsOrigin_t>(FindPattern(CLIENT_DLL, enc("55 8B EC 83 E4 F8 51 53 56 57 8B F1")));
		set_abs_origin(this, origin);
	}
	void SetAbsAngles(const QAngle* angles)
	{
		using SetAbsAngles_t = void(__thiscall*)(void*, const QAngle*);
		static SetAbsAngles_t set_abs_angles = rcast<SetAbsAngles_t>(FindPattern(CLIENT_DLL, enc("55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1")));
		set_abs_angles(this, angles);
	}
	datamap_t* GetDataDescMap()
	{
		return CallVFunction<datamap_t*(__thiscall*)(void*)>(this, 15)(this);
	}
	datamap_t* GetPredDescMap()
	{
		return CallVFunction<datamap_t*(__thiscall*)(void*)>(this, 17)(this);
	}

	// Utility functions
	matrix3x4_t* GetCoordinateFrame()
	{
		// Coordinate frame is located at m_CollisionGroup - 0x30
		static offset_t coordinate_frame = GET_NETVAR(enc("DT_BaseEntity"), enc("m_CollisionGroup")) - 0x30;
		return MAKE_PTR<matrix3x4_t>(this, coordinate_frame);
	}
	IClientEntity* GetOwnerEntity()
	{
		static offset_t m_hOwnerEntity = GET_NETVAR(enc("DT_BaseEntity"), enc("m_hOwnerEntity"));
		return Source::EntityList->GetClientEntityFromHandle(*MAKE_PTR<CHandle<IClientEntity>>(this, m_hOwnerEntity));
	}

	// Virtual functions
	bool IsTransparent() 
	{
		return CallVFunction<bool(__thiscall*)(void*)>(this, 50)(this);
	}
	SolidType_t GetSolid()
	{
		return CallVFunction<SolidType_t(__thiscall*)(void*)>(this, 81)(this);
	}
	bool IsPlayer()
	{
		//index: 152
		//ref: "effects/nightvision"
		//sig: 8B 92 ? ? ? ? FF D2 84 C0 0F 45 F7 85 F6
		return CallVFunction<bool(__thiscall*)(C_BaseEntity*)>(this, 152)(this);
	}
	bool IsWeapon()
	{
		//index: 160
		//ref: "CNewParticleEffect::DrawModel"
		//sig: 8B 80 ? ? ? ? FF D0 84 C0 74 6F 8B 4D A4
		return CallVFunction<bool(__thiscall*)(C_BaseEntity*)>(this, 160)(this);
	}
	bool ShouldCollide(int collisionGroup, int contentsMask) 
	{
		return CallVFunction<bool(__thiscall*)(void*, int, int)>(this, 167)(this, collisionGroup, contentsMask);
	}
	void BuildTransformations(studiohdr_t* studioHdr, Vector* pos, Quaternion* q, const matrix3x4_t& cameraTransform, int boneMask, byte* computed)
	{
		CallVFunction<void(__thiscall*)(void*, studiohdr_t*, Vector*, Quaternion*, const matrix3x4_t&, int, byte*)>(this, 184)(this, studioHdr, pos, q, cameraTransform, boneMask, computed);
	}
	void UpdateIKLocks(float currentTime)
	{
		CallVFunction<void(__thiscall*)(void*, float)>(this, 186)(this, currentTime);
	}
	void CalculateIKLocks(float currentTime)
	{
		CallVFunction<void(__thiscall*)(void*, float)>(this, 187)(this, currentTime);
	}
	void ControlMouth(studiohdr_t* studioHdr)
	{
		CallVFunction<void(__thiscall*)(void*, studiohdr_t*)>(this, 191)(this, studioHdr);
	}
	void StandardBlendingRules(studiohdr_t* studioHdr, Vector* pos, Quaternion* q, float currentTime, int boneMask)
	{
		CallVFunction<void(__thiscall*)(void*, studiohdr_t*, Vector*, Quaternion*, float, int)>(this, 200)(this, studioHdr, pos, q, currentTime, boneMask);
	}
	void UpdateClientSideAnimation()
	{
		// Index: 218 according to d3dc0m
		return CallVFunction<void(__thiscall*)(void*)>(this, 218)(this);
	}
	QAngle& m_angAbsRotation()
	{
		return *rcast<QAngle*>((DWORD)&m_angRotation() - 12);
	}

	// Lookup datamap
	offset_t FindDataMapOffset(datamap_t* map, const char* varName)
	{
		while (map)
		{
			for (int i = 0; i < map->dataNumFields; i++)
			{
				if (map->dataDesc[i].fieldName == NULL)
					continue;

				if (strcmp(varName, map->dataDesc[i].fieldName) == 0)
					return map->dataDesc[i].fieldOffset[TD_OFFSET_NORMAL];

				if (map->dataDesc[i].fieldType == FIELD_EMBEDDED)
				{
					if (map->dataDesc[i].td)
					{
						unsigned int offset;

						if ((offset = FindDataMapOffset(map->dataDesc[i].td, varName)) != 0)
							return offset;
					}
				}
			}
			map = map->baseMap;
		}
		return 0x0;
	}

	// Fields
	MAKE_FIELD(float, m_surfaceFriction, "m_surfaceFriction")

	// Netvars
	MAKE_NETVAR(float, m_flAnimTime, "m_flAnimTime", "DT_BaseEntity")
	MAKE_NETVAR(float, m_flSimulationTime, "m_flSimulationTime", "DT_BaseEntity")
	MAKE_NETVAR(int, m_cellbits, "m_cellbits", "DT_BaseEntity")
	MAKE_NETVAR(int, m_cellX, "m_cellX", "DT_BaseEntity")
	MAKE_NETVAR(int, m_cellY, "m_cellY", "DT_BaseEntity")
	MAKE_NETVAR(int, m_cellZ, "m_cellZ", "DT_BaseEntity")
	MAKE_NETVAR(Vector, m_vecOrigin, "m_vecOrigin", "DT_BaseEntity")
	MAKE_NETVAR(QAngle, m_angRotation, "m_angRotation", "DT_BaseEntity")
	MAKE_NETVAR_PTR(Vector, m_angRotationPtr, "m_angRotation", "DT_BaseEntity")
	MAKE_NETVAR(int, m_nModelIndex, "m_nModelIndex", "DT_BaseEntity")
	MAKE_NETVAR_PTR(int, m_nModelIndexPtr, "m_nModelIndex", "DT_BaseEntity")
	MAKE_NETVAR(int, m_fEffects, "m_fEffects", "DT_BaseEntity")
	MAKE_NETVAR(int, m_nRenderMode, "m_nRenderMode", "DT_BaseEntity")
	MAKE_NETVAR(int, m_nRenderFX, "m_nRenderFX", "DT_BaseEntity")
	MAKE_NETVAR(int, m_clrRender, "m_clrRender", "DT_BaseEntity")
	MAKE_NETVAR(int, m_iTeamNum, "m_iTeamNum", "DT_BaseEntity")
	MAKE_NETVAR(int, m_iPendingTeamNum, "m_iPendingTeamNum", "DT_BaseEntity")
	MAKE_NETVAR(int, m_CollisionGroup, "m_CollisionGroup", "DT_BaseEntity")
	MAKE_NETVAR(float, m_flElasticity, "m_flElasticity", "DT_BaseEntity")
	MAKE_NETVAR(float, m_flShadowCastDistance, "m_flShadowCastDistance", "DT_BaseEntity")
	MAKE_NETVAR(EHANDLE, m_hOwnerEntity, "m_hOwnerEntity", "DT_BaseEntity")
	MAKE_NETVAR(EHANDLE, m_hEffectEntity, "m_hEffectEntity", "DT_BaseEntity")
	MAKE_NETVAR(int, moveparent, "moveparent", "DT_BaseEntity")
	MAKE_NETVAR(int, m_iParentAttachment, "m_iParentAttachment", "DT_BaseEntity")
	MAKE_NETVAR_PTR(char, m_iName, "m_iName", "DT_BaseEntity") // Size = 260
	MAKE_NETVAR(int, movetype, "movetype", "DT_BaseEntity")
	MAKE_NETVAR(int, movecollide, "movecollide", "DT_BaseEntity")
	MAKE_NETVAR(Vector, m_vecMins, "m_vecMins", "DT_BaseEntity")
	MAKE_NETVAR(Vector, m_vecMaxs, "m_vecMaxs", "DT_BaseEntity")
	MAKE_NETVAR(int, m_nSolidType, "m_nSolidType", "DT_BaseEntity")
	MAKE_NETVAR(int, m_usSolidFlags, "m_usSolidFlags", "DT_BaseEntity")
	MAKE_NETVAR(int, m_nSurroundType, "m_nSurroundType", "DT_BaseEntity")
	MAKE_NETVAR(int, m_triggerBloat, "m_triggerBloat", "DT_BaseEntity")
	MAKE_NETVAR(Vector, m_vecSpecifiedSurroundingMins, "m_vecSpecifiedSurroundingMins", "DT_BaseEntity")
	MAKE_NETVAR(Vector, m_vecSpecifiedSurroundingMaxs, "m_vecSpecifiedSurroundingMaxs", "DT_BaseEntity")
	MAKE_NETVAR(int, m_iTextureFrameIndex, "m_iTextureFrameIndex", "DT_BaseEntity")
	MAKE_NETVAR(bool, m_bSimulatedEveryTick, "m_bSimulatedEveryTick", "DT_BaseEntity")
	MAKE_NETVAR(bool, m_bAnimatedEveryTick, "m_bAnimatedEveryTick", "DT_BaseEntity")
	MAKE_NETVAR(bool, m_bAlternateSorting, "m_bAlternateSorting", "DT_BaseEntity")
	MAKE_NETVAR(bool, m_bSpotted, "m_bSpotted", "DT_BaseEntity")
	MAKE_NETVAR_PTR(bool, m_bSpottedPtr, "m_bSpotted", "DT_BaseEntity")
	MAKE_NETVAR(bool, m_bIsAutoaimTarget, "m_bIsAutoaimTarget", "DT_BaseEntity")
	MAKE_NETVAR(float, m_fadeMinDist, "m_fadeMinDist", "DT_BaseEntity")
	MAKE_NETVAR(float, m_fadeMaxDist, "m_fadeMaxDist", "DT_BaseEntity")
	MAKE_NETVAR(float, m_flFadeScale, "m_flFadeScale", "DT_BaseEntity")
	MAKE_NETVAR(int, m_nMinCPULevel, "m_nMinCPULevel", "DT_BaseEntity")
	MAKE_NETVAR(int, m_nMaxCPULevel, "m_nMaxCPULevel", "DT_BaseEntity")
	MAKE_NETVAR(int, m_nMinGPULevel, "m_nMinGPULevel", "DT_BaseEntity")
	MAKE_NETVAR(int, m_nMaxGPULevel, "m_nMaxGPULevel", "DT_BaseEntity")
	MAKE_NETVAR(float, m_flUseLookAtAngle, "m_flUseLookAtAngle", "DT_BaseEntity")
	MAKE_NETVAR(float, m_flLastMadeNoiseTime, "m_flLastMadeNoiseTime", "DT_BaseEntity")
};

SOURCE_END
