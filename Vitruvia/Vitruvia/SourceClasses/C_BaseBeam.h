// Class built with love by Vitruvia :*
// Build date: Saturday 23/9/2017

#pragma once
#include "../Vitruvia.h"

#define MAX_BEAM_WIDTH			102.3f
#define MAX_BEAM_SCROLLSPEED	100.0f
#define MAX_BEAM_NOISEAMPLITUDE		64

#define SF_BEAM_STARTON			0x0001
#define SF_BEAM_TOGGLE			0x0002
#define SF_BEAM_RANDOM			0x0004
#define SF_BEAM_RING			0x0008
#define SF_BEAM_SPARKSTART		0x0010
#define SF_BEAM_SPARKEND		0x0020
#define SF_BEAM_DECALS			0x0040
#define SF_BEAM_SHADEIN			0x0080
#define SF_BEAM_SHADEOUT		0x0100
#define	SF_BEAM_TAPEROUT		0x0200	// Tapers to zero
#define SF_BEAM_TEMPORARY		0x8000

SOURCE_INIT

class C_BaseBeam
{
public:
	MAKE_NETVAR_PTR(int, m_nModelIndex, "m_nModelIndex", "DT_BaseBeam")
	MAKE_NETVAR_PTR(int, m_nHaloIndex, "m_nHaloIndex", "DT_BaseBeam")
	MAKE_NETVAR_PTR(int, m_nStartFrame, "m_nStartFrame", "DT_BaseBeam")
	MAKE_NETVAR_PTR(int, m_nFrameRate, "m_nFrameRate", "DT_BaseBeam")
	MAKE_NETVAR_PTR(float, m_fLife, "m_fLife", "DT_BaseBeam")
	MAKE_NETVAR_PTR(float, m_fWidth, "m_fWidth", "DT_BaseBeam")
	MAKE_NETVAR_PTR(float, m_fEndWidth, "m_fEndWidth", "DT_BaseBeam")
	MAKE_NETVAR_PTR(int, m_nFadeLength, "m_nFadeLength", "DT_BaseBeam")
	MAKE_NETVAR_PTR(float, m_fAmplitude, "m_fAmplitude", "DT_BaseBeam")
	MAKE_NETVAR_PTR(int, m_nSpeed, "m_nSpeed", "DT_BaseBeam")
	MAKE_NETVAR_PTR(int, r, "r", "DT_BaseBeam")
	MAKE_NETVAR_PTR(int, g, "g", "DT_BaseBeam")
	MAKE_NETVAR_PTR(int, b, "b", "DT_BaseBeam")
	MAKE_NETVAR_PTR(int, a, "a", "DT_BaseBeam")
	MAKE_NETVAR_PTR(int, m_nFlags, "m_nFlags", "DT_BaseBeam")
};

// Various types of beams

class C_TEBeamSpline : public C_BaseBeam
{
public:
	MAKE_NETVAR_PTR(int, m_nPoints, "m_nPoints", "DT_TEBeamSpline")
	MAKE_NETVAR_PTR(Vector, m_vecPoints_0, "m_vecPoints[0]", "DT_TEBeamSpline")
	MAKE_NETVAR_PTR(Vector, m_vecPoints, "m_vecPoints", "DT_TEBeamSpline")
};

class C_TEBeamRingPoint : public C_BaseBeam
{
public:
	MAKE_NETVAR_PTR(Vector, m_vecCenter, "m_vecCenter", "DT_TEBeamRingPoint")
	MAKE_NETVAR_PTR(float, m_flStartRadius, "m_flStartRadius", "DT_TEBeamRingPoint")
	MAKE_NETVAR_PTR(float, m_flEndRadius, "m_flEndRadius", "DT_TEBeamRingPoint")
};

class C_TEBeamPoints : public C_BaseBeam
{
public:
	MAKE_NETVAR_PTR(Vector, m_vecStartPoint, "m_vecStartPoint", "DT_TEBeamPoints")
	MAKE_NETVAR_PTR(Vector, m_vecEndPoint, "m_vecEndPoint", "DT_TEBeamPoints")
};

class C_TEBeamLaser : public C_BaseBeam
{
public:
	MAKE_NETVAR_PTR(int, m_nStartEntity, "m_nStartEntity", "DT_TEBeamLaser")
	MAKE_NETVAR_PTR(int, m_nEndEntity, "m_nEndEntity", "DT_TEBeamLaser")
};

class C_TEBeamFollow : public C_BaseBeam
{
public:
	MAKE_NETVAR_PTR(int, m_iEntIndex, "m_iEntIndex", "DT_TEBeamFollow")
};

class C_TEBeamEnts : public C_BaseBeam
{
public:
	MAKE_NETVAR_PTR(int, m_nStartEntity, "m_nStartEntity", "DT_TEBeamEnts")
	MAKE_NETVAR_PTR(int, m_nEndEntity, "m_nEndEntity", "DT_TEBeamEnts")
};

class C_TEBeamEntPoint : public C_BaseBeam
{
public:
	MAKE_NETVAR_PTR(int, m_nStartEntity, "m_nStartEntity", "DT_TEBeamEntPoint")
	MAKE_NETVAR_PTR(int, m_nEndEntity, "m_nEndEntity", "DT_TEBeamEntPoint")
	MAKE_NETVAR_PTR(Vector, m_vecStartPoint, "m_vecStartPoint", "DT_TEBeamEntPoint")
	MAKE_NETVAR_PTR(Vector, m_vecEndPoint, "m_vecEndPoint", "DT_TEBeamEntPoint")
};

SOURCE_END
