/*
	 _____                            ___________ _   __
	/  ___|                          /  ___|  _  \ | / /
	\ `--.  ___  _   _ _ __ ___ ___  \ `--.| | | | |/ /
	 `--. \/ _ \| | | | '__/ __/ _ \  `--. \ | | |    \
	/\__/ / (_) | |_| | | | (_|  __/ /\__/ / |/ /| |\  \
	\____/ \___/ \__,_|_|  \___\___| \____/|___/ \_| \_/

	Designed for Counter-Strike: Global Offensive
	Copyright: ValvE Corporation 2017 (C)
*/

#pragma once
#include "../../SourceCommon.h"
#include "IVModelInfo.h"

#define STUDIO_NONE						0x00000000
#define STUDIO_RENDER					0x00000001
#define STUDIO_VIEWXFORMATTACHMENTS		0x00000002
#define STUDIO_DRAWTRANSLUCENTSUBMODELS 0x00000004
#define STUDIO_TWOPASS					0x00000008
#define STUDIO_STATIC_LIGHTING			0x00000010
#define STUDIO_WIREFRAME				0x00000020
#define STUDIO_ITEM_BLINK				0x00000040
#define STUDIO_NOSHADOWS				0x00000080
#define STUDIO_WIREFRAME_VCOLLIDE		0x00000100
#define STUDIO_NOLIGHTING_OR_CUBEMAP	0x00000200
#define STUDIO_SKIP_FLEXES				0x00000400
#define STUDIO_DONOTMODIFYSTENCILSTATE	0x00000800	// TERROR

// Not a studio flag, but used to flag model as a non-sorting brush model
#define STUDIO_TRANSPARENCY				0x80000000

// Not a studio flag, but used to flag model as using shadow depth material override
#define STUDIO_SHADOWDEPTHTEXTURE		0x40000000

// Not a studio flag, but used to flag model as doing custom rendering into shadow texture
#define STUDIO_SHADOWTEXTURE			0x20000000

#define STUDIO_SKIP_DECALS				0x10000000

SOURCE_INIT

struct studiohwdata_t;

DECLARE_POINTER_HANDLE(StudioDecalHandle_t);
#define STUDIORENDER_DECAL_INVALID  ( (StudioDecalHandle_t)0 )

//-----------------------------------------------------------------------------
// Model rendering state
//-----------------------------------------------------------------------------
struct DrawModelState_t
{
	studiohdr_t*			m_pStudioHdr;
	studiohwdata_t*			m_pStudioHWData;
	IClientRenderable*		m_pRenderable;
	const matrix3x4_t		*m_pModelToWorld;
	StudioDecalHandle_t		m_decals;
	int						m_drawFlags;
	int						m_lod;
};


//-----------------------------------------------------------------------------
// Model Rendering + instance data
//-----------------------------------------------------------------------------

typedef unsigned short ModelInstanceHandle_t;
enum
{
	MODEL_INSTANCE_INVALID = (ModelInstanceHandle_t)~0
};

struct ModelRenderInfo_t
{
	Vector origin;
	QAngle angles;
	char pad[0x4]; // Unknown new data update 03/29/18
	IClientRenderable *pRenderable;
	const model_t *pModel;
	const matrix3x4_t *pModelToWorld;
	const matrix3x4_t *pLightingOffset;
	const Vector *pLightingOrigin;
	int flags;
	int entity_index;
	int skin;
	int body;
	int hitboxset;
	ModelInstanceHandle_t instance;

	ModelRenderInfo_t()
	{
		pModelToWorld = NULL;
		pLightingOffset = NULL;
		pLightingOrigin = NULL;
	}
};

struct StaticPropRenderInfo_t
{
	const matrix3x4_t		*pModelToWorld;
	const model_t			*pModel;
	IClientRenderable		*pRenderable;
	Vector					*pLightingOrigin;
	ModelInstanceHandle_t	instance;
	uint8					skin;
	uint8					alpha;
};

struct LightingQuery_t
{
	Vector m_LightingOrigin;
	ModelInstanceHandle_t m_InstanceHandle;
	bool m_bAmbientBoost;
};

struct StaticLightingQuery_t : public LightingQuery_t
{
	IClientRenderable *m_pRenderable;
};

abstract_class IVModelRender
{
public:
	int DrawModel(int flags,
		void *pRenderable,
		ModelInstanceHandle_t instance,
		int entity_index,
		const model_t *model,
		Vector const& origin,
		QAngle const& angles,
		int skin,
		int body,
		int hitboxset,
		const matrix3x4_t *modelToWorld = NULL,
		const matrix3x4_t *pLightingOffset = NULL)
	{
		typedef int(__thiscall *Fn)(void*, int, void*, ModelInstanceHandle_t, int, const model_t*, 
			const Vector&, const QAngle&, int, int, int, const matrix3x4_t*, const matrix3x4_t*);
		return CallVFunction<Fn>(this, 0)(this, flags, pRenderable, instance, entity_index, model, origin, angles, skin, body, hitboxset, modelToWorld, pLightingOffset);
	}
	void ForcedMaterialOverride(IMaterial* Mat, int OverrideType = NULL, int Unknown = NULL)
	{
		typedef void(__thiscall *Fn)(void*, IMaterial*, int, int);
		return CallVFunction<Fn>(this, 1)(this, Mat, OverrideType, Unknown);
	}
	ModelInstanceHandle_t CreateInstance(void *pRenderable, LightCacheHandle_t *pCache = NULL)
	{
		typedef ModelInstanceHandle_t(__thiscall* Fn)(void*, void*, LightCacheHandle_t*);
		return CallVFunction<Fn>(this, 4)(this, pRenderable, pCache);
	}
	void DestroyInstance(ModelInstanceHandle_t handle)
	{
		typedef void(__thiscall* Fn)(void*, ModelInstanceHandle_t);
		return CallVFunction<Fn>(this, 5)(this, handle);
	}
	void DrawModelExecute(void* ctx, void* state, const ModelRenderInfo_t& info, matrix3x4_t *pCustomBoneToWorld)
	{
		typedef void(__thiscall* Fn)(void*, void*, void*, const ModelRenderInfo_t&, matrix3x4_t*);
		return CallVFunction<Fn>(this, 21)(this, ctx, state, info, pCustomBoneToWorld);
	}
};

SOURCE_END