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
#include "../../Classes/Maths/Vector.h"
#include "../../Classes/BSPFlags.h"
#include "../Client/IClientEntity.h"
#include <functional>

SOURCE_INIT

class __declspec(align(16)) VectorAligned : public Vector
{
public:
	inline VectorAligned(void) {};
	inline VectorAligned(float X, float Y, float Z)
	{
		Init(X, Y, Z);
	}

public:
	explicit VectorAligned(const Vector &vOther)
	{
		Init(vOther.x, vOther.y, vOther.z);
	}

	VectorAligned& operator=(const Vector &vOther)
	{
		Init(vOther.x, vOther.y, vOther.z);
		return *this;
	}

	VectorAligned& operator=(const VectorAligned &vOther)
	{
		_mm_store_ps(Base(), _mm_load_ps(vOther.Base()));
		return *this;
	}
	float w;
};

struct Ray_t;
class CGameTrace;
using trace_t = CGameTrace;
class ICollideable;
class ITraceListData;
class CPhysCollide;
class C_BaseEntity;

struct cplane_t
{
	Vector	normal;
	float	dist;
	BYTE	type;
	BYTE	signbits;
	BYTE	pad[2];
};
struct virtualmeshlist_t;

enum class TraceType
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,
	TRACE_ENTITIES_ONLY,
	TRACE_EVERYTHING_FILTER_PROPS,
};
enum modtype_t {
	mod_bad = 0,
	mod_brush,
	mod_sprite,
	mod_studio
};

class ITraceFilter
{
public:
	virtual bool ShouldHitEntity(C_BaseEntity* pEntityHandle, int contentsMask)
	{
		return !(pEntityHandle == pSkip);
	}
	virtual TraceType GetTraceType() 
	{
		return TraceType::TRACE_EVERYTHING;
	}
	void* pSkip;
};

//-----------------------------------------------------------------------------
// Classes are expected to inherit these + implement the ShouldHitEntity method
//-----------------------------------------------------------------------------

// This is the one most normal traces will inherit from
class CTraceFilter : public ITraceFilter
{
public:
	bool ShouldHitEntity(C_BaseEntity* pEntityHandle, int /*contentsMask*/)
	{
		return !(pEntityHandle == pSkip);
	}
	virtual TraceType GetTraceType() const
	{
		return TraceType::TRACE_EVERYTHING;
	}
};

class CTraceFilterEntitiesOnly : public ITraceFilter
{
public:
	bool ShouldHitEntity(C_BaseEntity* pEntityHandle, int /*contentsMask*/)
	{
		return !(pEntityHandle == pSkip);
	}
	virtual TraceType GetTraceType() const
	{
		return TraceType::TRACE_ENTITIES_ONLY;
	}
};

//-----------------------------------------------------------------------------
// Classes need not inherit from these
//-----------------------------------------------------------------------------
class CTraceFilterWorldOnly : public ITraceFilter
{
public:
	bool ShouldHitEntity(C_BaseEntity* /*pServerEntity*/, int /*contentsMask*/)
	{
		return false;
	}
	virtual TraceType GetTraceType() const
	{
		return TraceType::TRACE_WORLD_ONLY;
	}
};
class CTraceFilterWorldAndPropsOnly : public ITraceFilter
{
public:
	bool ShouldHitEntity(C_BaseEntity* /*pServerEntity*/, int /*contentsMask*/)
	{
		return false;
	}
	virtual TraceType GetTraceType() const
	{
		return TraceType::TRACE_EVERYTHING;
	}
};
class CTraceFilterHitAll : public CTraceFilter
{
public:
	virtual bool ShouldHitEntity(C_BaseEntity* /*pServerEntity*/, int /*contentsMask*/)
	{
		return true;
	}
};
class CTraceFilterNoPlayer : public CTraceFilter
{
public:
	CTraceFilterNoPlayer() {}
	virtual bool ShouldHitEntity(IHandleEntity *pServerEntity, int contentsMask)
	{
		IClientUnknown *pUnk = (IClientUnknown*)pServerEntity;
		void *pEnt = pUnk->GetBaseEntity();
		typedef bool(__thiscall *IsPlayerFn)(PVOID);
		return ((IsPlayerFn)(*(PDWORD)(*(PDWORD)(pEnt)+0x260)))(pEnt); // Straight outta ghetto
	}
};
class CSimpleTraceFilter : public ITraceFilter
{
public:
	CSimpleTraceFilter(C_BaseEntity* pEnt, TraceType traceType = TraceType::TRACE_EVERYTHING)
	{
		m_pPassEnt = pEnt;
		iTraceType = traceType;
	}

	virtual bool ShouldHitEntity(C_BaseEntity *pHandleEntity, int contentsMask)
	{
		return pHandleEntity != m_pPassEnt;
	}

	virtual TraceType GetTraceType() const
	{
		return iTraceType;
	}

	C_BaseEntity* m_pPassEnt;
	TraceType iTraceType;
};
class CLambdaTraceFilter : public CTraceFilter
{
private:
	std::function<bool(C_BaseEntity*, int)> filter;
	TraceType trace_type;
public:
	CLambdaTraceFilter(const std::function<bool(C_BaseEntity*, int)>& filter, const TraceType& traceType = TraceType::TRACE_EVERYTHING)
		: filter(filter), trace_type(traceType) {}

	bool ShouldHitEntity(C_BaseEntity* entity, int mask) override
	{
		return this->filter(entity, mask);
	}

	virtual TraceType GetTraceType() const
	{
		return this->trace_type;
	}
};
class CTraceFilterIgnoreGrenades 
{
public:
	virtual bool ShouldHitEntity(IHandleEntity* pEntityHandle, int contentsMask) 
	{
		if (!pEntityHandle) {
			return !(pEntityHandle == pSkip);
		}

		//return BaseShouldHitEntity( pSkip, ( IHandleEntity* ) pEntityHandle, m_icollisionGroup, contentsMask );

		ClientClass *clclass = ((IClientEntity*)pEntityHandle)->GetClientClass();

		if (clclass) 
		{
			if (strstr(clclass->m_pNetworkName, "CBaseCSGrenadeProjectile"))
				return false;
		}

		return !(pEntityHandle == pSkip);
	}

	virtual TraceType GetTraceType() const
	{
		return TraceType::TRACE_EVERYTHING;
	}

	IHandleEntity* pSkip;
	int iCollisionGroup;
};
class CGrenadePredictTraceFilter 
{
public:
	virtual bool ShouldHitEntity(IHandleEntity* pEntityHandle, int contentsMask) {
		if (!pEntityHandle)
			return !(pEntityHandle == pSkip);

		if (pEntityHandle == pSkip)
			return false;

		ClientClass *clclass = ((IClientEntity*)pEntityHandle)->GetClientClass();

		if (clclass) {
			if (strstr(clclass->m_pNetworkName, "CBaseCSGrenadeProjectile"))
				return false;
		}

		return !(pEntityHandle == pSkip);

		//return BaseShouldHitEntity( pSkip, ( IHandleEntity* ) pEntityHandle, m_icollisionGroup, contentsMask );
	}

	virtual TraceType GetTraceType() const
	{
		return TraceType::TRACE_EVERYTHING;
	}

	IHandleEntity* pSkip;
	int iCollisionGroup;
};

enum class DebugTraceCounterBehavior_t
{
	kTRACE_COUNTER_SET = 0,
	kTRACE_COUNTER_INC,
};

//-----------------------------------------------------------------------------
// Enumeration interface for EnumerateLinkEntities
//-----------------------------------------------------------------------------
class IEntityEnumerator
{
public:
	// This gets called with each handle
	virtual bool EnumEntity(IHandleEntity* pHandleEntity) = 0;
};

struct BrushSideInfo_t
{
	Vector4 plane;           // The plane of the brush side
	unsigned short bevel;    // Bevel plane?
	unsigned short thin;     // Thin?
};

class CPhysCollide;

struct vcollide_t
{
	unsigned short solidCount : 15;
	unsigned short isPacked : 1;
	unsigned short descSize;
	// VPhysicsSolids
	CPhysCollide   **solids;
	char           *pKeyValues;
	void           *pUserData;
};

struct cmodel_t
{
	Vector         mins, maxs;
	Vector         origin;        // for sounds or lights
	int            headnode;
	vcollide_t     vcollisionData;
};

struct csurface_t
{
	const char     *name;
	short          surfaceProps;
	unsigned short flags;         // BUGBUG: These are declared per surface, not per material, but this database is per-material now
};

struct Ray_t
{
	VectorAligned  m_Start;  // starting point, centered within the extents
	VectorAligned  m_Delta;  // direction + length of the ray
	VectorAligned  m_StartOffset; // Add this to m_Start to get the actual ray start
	VectorAligned  m_Extents;     // Describes an axis aligned box extruded along a ray
	const matrix3x4_t *m_pWorldAxisTransform;
	bool m_IsRay;  // are the extents zero?
	bool m_IsSwept;     // is delta != 0?

	Ray_t() : m_pWorldAxisTransform(NULL) {}

	void Init(Vector const& start, Vector const& end)
	{
		m_Delta = end - start;

		m_IsSwept = (m_Delta.LengthSqr() != 0);

		m_Extents.Init(0.f, 0.f, 0.f);

		m_pWorldAxisTransform = NULL;
		m_IsRay = true;

		// Offset m_Start to be in the center of the box...
		m_StartOffset.Init(0.f, 0.f, 0.f);
		m_Start = start;
	}

	void Init(Vector const& start, Vector const& end, Vector const& mins, Vector const& maxs)
	{
		m_Delta = end - start;

		m_pWorldAxisTransform = NULL;
		m_IsSwept = (m_Delta.LengthSqr() != 0);

		m_Extents = maxs - mins;
		m_Extents *= 0.5f;
		m_IsRay = (m_Extents.LengthSqr() < 1e-6);

		// Offset m_Start to be in the center of the box...
		m_StartOffset = maxs + mins;
		m_StartOffset *= 0.5f;
		m_Start = start + m_StartOffset;
		m_StartOffset *= -1.0f;
	}
	Vector InvDelta() const
	{
		Vector vecInvDelta;
		for (int iAxis = 0; iAxis < 3; ++iAxis) {
			if (m_Delta[iAxis] != 0.0f) {
				vecInvDelta[iAxis] = 1.0f / m_Delta[iAxis];
			}
			else {
				vecInvDelta[iAxis] = FLT_MAX;
			}
		}
		return vecInvDelta;
	}
};

class CBaseTrace
{
public:
	bool IsDispSurface(void) { return ((dispFlags & DISPSURF_FLAG_SURFACE) != 0); }
	bool IsDispSurfaceWalkable(void) { return ((dispFlags & DISPSURF_FLAG_WALKABLE) != 0); }
	bool IsDispSurfaceBuildable(void) { return ((dispFlags & DISPSURF_FLAG_BUILDABLE) != 0); }
	bool IsDispSurfaceProp1(void) { return ((dispFlags & DISPSURF_FLAG_SURFPROP1) != 0); }
	bool IsDispSurfaceProp2(void) { return ((dispFlags & DISPSURF_FLAG_SURFPROP2) != 0); }

public:

	// these members are aligned!!
	Vector         startpos;                // start position
	Vector         endpos;                       // final position
	cplane_t       plane;                        // surface normal at impact

	float          fraction;                // time completed, 1.0 = didn't hit anything

	int            contents;                // contents on other side of surface hit
	unsigned short dispFlags;                    // displacement flags for marking surfaces with data

	bool           allsolid;                // if true, plane is not valid
	bool           startsolid;                   // if true, the initial point was in a solid area

	CBaseTrace() {}
};

class CGameTrace : public CBaseTrace
{
public:
	bool DidHitWorld() const;
	bool DidHitNonWorldEntity() const;
	int GetEntityIndex() const;
	bool DidHit() const;
	bool IsVisible() const;
	float               fractionleftsolid;  // time we left a solid, only valid if we started in solid
	csurface_t          surface;            // surface hit (impact surface)
	int                 hitgroup;           // 0 == generic, non-zero is specific body part
	short               physicsbone;        // physics bone hit by trace in studio
	unsigned short      worldSurfaceIndex;  // Index of the msurface2_t, if applicable
	C_BaseEntity*		m_pEnt;
	int                 hitbox;                       // box hit by trace in studio

	CGameTrace() {}


	CGameTrace& CGameTrace::operator=(const CGameTrace& other)
	{
		startpos = other.startpos;
		endpos = other.endpos;
		plane = other.plane;
		fraction = other.fraction;
		contents = other.contents;
		dispFlags = other.dispFlags;
		allsolid = other.allsolid;
		startsolid = other.startsolid;
		fractionleftsolid = other.fractionleftsolid;
		surface = other.surface;
		hitgroup = other.hitgroup;
		physicsbone = other.physicsbone;
		worldSurfaceIndex = other.worldSurfaceIndex;
		m_pEnt = other.m_pEnt;
		hitbox = other.hitbox;
		return *this;
	}

private:
	// No copy constructors allowed
	CGameTrace(const CGameTrace& other) :
		fractionleftsolid(other.fractionleftsolid),
		surface(other.surface),
		hitgroup(other.hitgroup),
		physicsbone(other.physicsbone),
		worldSurfaceIndex(other.worldSurfaceIndex),
		m_pEnt(other.m_pEnt),
		hitbox(other.hitbox)
	{
		startpos = other.startpos;
		endpos = other.endpos;
		plane = other.plane;
		fraction = other.fraction;
		contents = other.contents;
		dispFlags = other.dispFlags;
		allsolid = other.allsolid;
		startsolid = other.startsolid;
	}
};

inline bool CGameTrace::DidHit() const
{
	return fraction < 1 || allsolid || startsolid;
}

inline bool CGameTrace::IsVisible() const
{
	return fraction > 0.97f;
}

abstract_class IEngineTrace
{
public:
	virtual int         GetPointContents(const Vector &vecAbsPosition, int contentsMask = MASK_ALL, IHandleEntity** ppEntity = nullptr) = 0;
	virtual int         GetPointContents_WorldOnly(const Vector &vecAbsPosition, int contentsMask = MASK_ALL) = 0;
	virtual int         GetPointContents_Collideable(ICollideable *pCollide, const Vector &vecAbsPosition) = 0;
	virtual void        ClipRayToEntity(const Ray_t &ray, unsigned int fMask, IHandleEntity *pEnt, CGameTrace *pTrace) = 0;
	virtual void        ClipRayToCollideable(const Ray_t &ray, unsigned int fMask, ICollideable *pCollide, CGameTrace *pTrace) = 0;
	virtual void        TraceRay(const Ray_t &ray, unsigned int fMask, ITraceFilter *pTraceFilter, CGameTrace *pTrace) = 0;
};

SOURCE_END
