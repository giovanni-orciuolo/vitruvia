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
#include "../../Classes/Maths/Quaternion.h"
#include "../Client/IClientEntity.h"
#include "../MaterialSystem/IMaterial.h"
#include "../../Classes/ThreadTools.h"

SOURCE_INIT

// Use this when there are translucent parts to the model but we're not going to sort it 
#define STUDIOHDR_FLAGS_FORCE_OPAQUE						0x00000004

// Use this when we want to render the opaque parts during the opaque pass
// and the translucent parts during the translucent pass
#define STUDIOHDR_FLAGS_TRANSLUCENT_TWOPASS					0x00000008

// This is set any time the .qc files has $staticprop in it
// Means there's no bones and no transforms
#define STUDIOHDR_FLAGS_STATIC_PROP							0x00000010

// NOTE:  This flag is set at loadtime, not mdl build time so that we don't have to rebuild
// models when we change materials.
#define STUDIOHDR_FLAGS_USES_FB_TEXTURE						0x00000020

// This flag is set by studiomdl.exe if a separate "$shadowlod" entry was present
//  for the .mdl (the shadow lod is the last entry in the lod list if present)
#define STUDIOHDR_FLAGS_HASSHADOWLOD						0x00000040

// NOTE:  This flag is set at loadtime, not mdl build time so that we don't have to rebuild
// models when we change materials.
#define STUDIOHDR_FLAGS_USES_BUMPMAPPING					0x00000080

// NOTE:  This flag is set when we should use the actual materials on the shadow LOD
// instead of overriding them with the default one (necessary for translucent shadows)
#define STUDIOHDR_FLAGS_USE_SHADOWLOD_MATERIALS				0x00000100

// NOTE:  This flag is set when we should use the actual materials on the shadow LOD
// instead of overriding them with the default one (necessary for translucent shadows)
#define STUDIOHDR_FLAGS_OBSOLETE							0x00000200

#define STUDIOHDR_FLAGS_UNUSED								0x00000400

// NOTE:  This flag is set at mdl build time
#define STUDIOHDR_FLAGS_NO_FORCED_FADE						0x00000800

// NOTE:  The npc will lengthen the viseme check to always include two phonemes
#define STUDIOHDR_FLAGS_FORCE_PHONEME_CROSSFADE				0x00001000

// This flag is set when the .qc has $constantdirectionallight in it
// If set, we use constantdirectionallightdot to calculate light intensity
// rather than the normal directional dot product
// only valid if STUDIOHDR_FLAGS_STATIC_PROP is also set
#define STUDIOHDR_FLAGS_CONSTANT_DIRECTIONAL_LIGHT_DOT		0x00002000

// Flag to mark delta flexes as already converted from disk format to memory format
#define STUDIOHDR_FLAGS_FLEXES_CONVERTED					0x00004000

// Indicates the studiomdl was built in preview mode
#define STUDIOHDR_FLAGS_BUILT_IN_PREVIEW_MODE				0x00008000

// Ambient boost (runtime flag)
#define STUDIOHDR_FLAGS_AMBIENT_BOOST						0x00010000

// Don't cast shadows from this model (useful on first-person models)
#define STUDIOHDR_FLAGS_DO_NOT_CAST_SHADOWS					0x00020000

// alpha textures should cast shadows in vrad on this model (ONLY prop_static!)
#define STUDIOHDR_FLAGS_CAST_TEXTURE_SHADOWS				0x00040000

// flagged on load to indicate no animation events on this model
#define STUDIOHDR_FLAGS_VERT_ANIM_FIXED_POINT_SCALE			0x00200000

class CPhysCollide;
struct cplane_t;
class CUtlBuffer;
struct mstudioanimdesc_t;
struct mstudiobodyparts_t;
struct mstudiotexture_t;
struct vcollide_t;
struct virtualmodel_t;
struct studiohdr_t;

class virtualgroup_t
{
public:
	virtualgroup_t(void) { cache = NULL; };
	// tool dependant.  In engine this is a model_t, in tool it's a direct pointer
	void *cache;
	// converts cache entry into a usable studiohdr_t *
	const studiohdr_t *GetStudioHdr(void) const;

	CUtlVector< int > boneMap;				// maps global bone to local bone
	CUtlVector< int > masterBone;			// maps local bone to global bone
	CUtlVector< int > masterSeq;			// maps local sequence to master sequence
	CUtlVector< int > masterAnim;			// maps local animation to master animation
	CUtlVector< int > masterAttachment;		// maps local attachment to global
	CUtlVector< int > masterPose;			// maps local pose parameter to global
	CUtlVector< int > masterNode;			// maps local transition nodes to global
};

struct virtualsequence_t
{
	int	flags;
	int activity;
	int group;
	int index;
};

struct virtualgeneric_t
{
	int group;
	int index;
};

struct virtualmodel_t
{
	void AppendSequences(int group, const studiohdr_t *pStudioHdr);
	void AppendAnimations(int group, const studiohdr_t *pStudioHdr);
	void AppendAttachments(int ground, const studiohdr_t *pStudioHdr);
	void AppendPoseParameters(int group, const studiohdr_t *pStudioHdr);
	void AppendBonemap(int group, const studiohdr_t *pStudioHdr);
	void AppendNodes(int group, const studiohdr_t *pStudioHdr);
	void AppendTransitions(int group, const studiohdr_t *pStudioHdr);
	void AppendIKLocks(int group, const studiohdr_t *pStudioHdr);
	void AppendModels(int group, const studiohdr_t *pStudioHdr);
	void UpdateAutoplaySequences(const studiohdr_t *pStudioHdr);

	virtualgroup_t *pAnimGroup(int animation) { return &m_group[m_anim[animation].group]; } // Note: user must manage mutex for this
	virtualgroup_t *pSeqGroup(int sequence)
	{
		// Check for out of range access that is causing crashes on some servers.
		// Perhaps caused by sourcemod bugs. Typical sequence in these cases is ~292
		// when the count is 234. Using unsigned math allows for free range
		// checking against zero.
		if ((unsigned)sequence >= (unsigned)m_seq.Count())
		{
			Assert(0);
			return 0;
		}
		return &m_group[m_seq[sequence].group];
	} // Note: user must manage mutex for this

	CThreadFastMutex m_Lock;

	CUtlVector< virtualsequence_t > m_seq;
	CUtlVector< virtualgeneric_t > m_anim;
	CUtlVector< virtualgeneric_t > m_attachment;
	CUtlVector< virtualgeneric_t > m_pose;
	CUtlVector< virtualgroup_t > m_group;
	CUtlVector< virtualgeneric_t > m_node;
	CUtlVector< virtualgeneric_t > m_iklock;
	CUtlVector< unsigned short > m_autoplaySequences;
};

struct mstudioactivitymodifier_t
{
	int					sznameindex;
	inline char			*pszName() { return (sznameindex) ? (char *)(((byte *)this) + sznameindex) : NULL; }
};

struct mstudioiklock_t
{
	int			chain;
	float		flPosWeight;
	float		flLocalQWeight;
	int			flags;
	int			unused[4];
};

struct mstudioautolayer_t
{
	//private:
	short				iSequence;
	short				iPose;
	//public:
	int					flags;
	float				start;	// beginning of influence
	float				peak;	// start of full influence
	float				tail;	// end of full influence
	float				end;	// end of all influence
};

struct mstudioevent_t
{
	float				cycle;
	int					_event;
	int					type;
	inline const char * pszOptions(void) const { return options; }
	char				options[64];

	int					szeventindex;
	inline char * const pszEventName(void) const { return ((char *)this) + szeventindex; }
};

struct mstudioseqdesc_t
{
	int					baseptr;
	inline studiohdr_t	*pStudiohdr(void) const { return (studiohdr_t *)(((byte *)this) + baseptr); }

	int					szlabelindex;
	inline char * const pszLabel(void) const { return ((char *)this) + szlabelindex; }

	int					szactivitynameindex;
	inline char * const pszActivityName(void) const { return ((char *)this) + szactivitynameindex; }

	int					flags;		// looping/non-looping flags

	int					activity;	// initialized at loadtime to game DLL values
	int					actweight;

	int					numevents;
	int					eventindex;
	inline mstudioevent_t *pEvent(int i) const { Assert(i >= 0 && i < numevents); return (mstudioevent_t *)(((byte *)this) + eventindex) + i; };

	Vector				bbmin;		// per sequence bounding box
	Vector				bbmax;

	int					numblends;

	// Index into array of shorts which is groupsize[0] x groupsize[1] in length
	int					animindexindex;

	inline int			anim(int x, int y) const
	{
		if (x >= groupsize[0])
		{
			x = groupsize[0] - 1;
		}

		if (y >= groupsize[1])
		{
			y = groupsize[1] - 1;
		}

		int offset = y * groupsize[0] + x;
		short *blends = (short *)(((byte *)this) + animindexindex);
		int value = (int)blends[offset];
		return value;
	}

	int					movementindex;	// [blend] float array for blended movement
	int					groupsize[2];
	int					paramindex[2];	// X, Y, Z, XR, YR, ZR
	float				paramstart[2];	// local (0..1) starting value
	float				paramend[2];	// local (0..1) ending value
	int					paramparent;

	float				fadeintime;		// ideal cross fate in time (0.2 default)
	float				fadeouttime;	// ideal cross fade out time (0.2 default)

	int					localentrynode;		// transition node at entry
	int					localexitnode;		// transition node at exit
	int					nodeflags;		// transition rules

	float				entryphase;		// used to match entry gait
	float				exitphase;		// used to match exit gait

	float				lastframe;		// frame that should generation EndOfSequence

	int					nextseq;		// auto advancing sequences
	int					pose;			// index of delta animation between end and nextseq

	int					numikrules;

	int					numautolayers;	//
	int					autolayerindex;
	inline mstudioautolayer_t *pAutolayer(int i) const { Assert(i >= 0 && i < numautolayers); return (mstudioautolayer_t *)(((byte *)this) + autolayerindex) + i; };

	int					weightlistindex;
	inline float		*pBoneweight(int i) const { return ((float *)(((byte *)this) + weightlistindex) + i); };
	inline float		weight(int i) const { return *(pBoneweight(i)); };

	// FIXME: make this 2D instead of 2x1D arrays
	int					posekeyindex;
	float				*pPoseKey(int iParam, int iAnim) const { return (float *)(((byte *)this) + posekeyindex) + iParam * groupsize[0] + iAnim; }
	float				poseKey(int iParam, int iAnim) const { return *(pPoseKey(iParam, iAnim)); }

	int					numiklocks;
	int					iklockindex;
	inline mstudioiklock_t *pIKLock(int i) const { Assert(i >= 0 && i < numiklocks); return (mstudioiklock_t *)(((byte *)this) + iklockindex) + i; };

	// Key values
	int					keyvalueindex;
	int					keyvaluesize;
	inline const char * KeyValueText(void) const { return keyvaluesize != 0 ? ((char *)this) + keyvalueindex : NULL; }

	int					cycleposeindex;		// index of pose parameter to use as cycle index

	int					activitymodifierindex;
	int					numactivitymodifiers;
	inline mstudioactivitymodifier_t *pActivityModifier(int i) const { Assert(i >= 0 && i < numactivitymodifiers); return activitymodifierindex != 0 ? (mstudioactivitymodifier_t *)(((byte *)this) + activitymodifierindex) + i : NULL; };

	int					unused[5];		// remove/add as appropriate (grow back to 8 ints on version change!)

	mstudioseqdesc_t() {}
private:
	// No copy constructors allowed
	mstudioseqdesc_t(const mstudioseqdesc_t& vOther);
};

struct mstudiobbox_t
{
	int		bone;
	int		group; // intersection group
	Vector	bbmin; // bounding box 
	Vector	bbmax;
	int		hitboxnameindex; // offset to the name of the hitbox.
	int		pad[3];
	float	radius;
	int		pad2[4];
	char* szHitboxName()
	{
		if (hitboxnameindex == 0)
			return "";
		return ((char*)this) + hitboxnameindex;
	}
	float& diameter()
	{
		return *(float*)((DWORD)this + 0x30);
	}
};

struct mstudiohitboxset_t
{
	int sznameindex;
	inline char* szName(void) const { return ((char*)this) + sznameindex; }
	int numhitboxes;
	int hitboxindex;
	inline mstudiobbox_t* pHitbox(int i) const { return (mstudiobbox_t*)(((byte*)this) + hitboxindex) + i; }
};

struct mstudiobone_t
{
	int					sznameindex;
	inline char * const pszName(void) const { return ((char *)this) + sznameindex; }
	int		 			parent;		// parent bone
	int					bonecontroller[6];	// bone controller index, -1 == none
	Vector				pos;
	Quaternion			quat;
	RadianEuler			rot;
	// compression scale
	Vector				posscale;
	Vector				rotscale;
	matrix3x4_t			poseToBone;
	Quaternion			qAlignment;
	int					flags;
	int					proctype;
	int					procindex;		// procedural rule
	mutable int			physicsbone;	// index into physically simulated bone
	inline void *pProcedure() const { if (procindex == 0) return NULL; else return  (void *)(((byte *)this) + procindex); };
	int					surfacepropidx;	// index into string tablefor property name
	inline char *pszSurfaceProp(void) const { return ((char *)this) + surfacepropidx; }
	inline int			GetSurfaceProp(void) const { return surfacepropLookup; }
	int					contents;		// See BSPFlags.h for the contents flags
	int					surfacepropLookup;	// this index must be cached by the loader, not saved in the file
	int					unused[7];		// remove as appropriate
};

struct mstudiobonecontroller_t
{
	int					bone;	// -1 == 0
	int					type;	// X, Y, Z, XR, YR, ZR, M
	float				start;
	float				end;
	int					rest;	// byte index value at rest
	int					inputfield;	// 0-3 user set controller, 4 mouth
	int					unused[8];
};

#define	ATTACHMENT_FLAG_WORLD_ALIGN 0x10000

// attachment
struct mstudioattachment_t
{
	int					sznameindex;
	inline char * const pszName(void) const { return ((char *)this) + sznameindex; }
	unsigned int		flags;
	int					localbone;
	matrix3x4_t			local; // attachment point
	int					unused[8];
};

#define IK_SELF 1
#define IK_WORLD 2
#define IK_GROUND 3
#define IK_RELEASE 4
#define IK_ATTACHMENT 5
#define IK_UNLATCH 6

struct mstudioflexdesc_t
{
	int					szFACSindex;
	inline char * const pszFACS(void) const { return ((char *)this) + szFACSindex; }
};

struct mstudioflexcontroller_t
{
	int					sztypeindex;
	inline char * const pszType(void) const { return ((char *)this) + sztypeindex; }
	int					sznameindex;
	inline char * const pszName(void) const { return ((char *)this) + sznameindex; }
	mutable int			localToGlobal;	// remapped at load time to master list
	float				min;
	float				max;
};

enum FlexControllerRemapType_t
{
	FLEXCONTROLLER_REMAP_PASSTHRU = 0,
	FLEXCONTROLLER_REMAP_2WAY,	// Control 0 -> ramps from 1-0 from 0->0.5. Control 1 -> ramps from 0-1 from 0.5->1
	FLEXCONTROLLER_REMAP_NWAY,	// StepSize = 1 / (control count-1) Control n -> ramps from 0-1-0 from (n-1)*StepSize to n*StepSize to (n+1)*StepSize. A second control is needed to specify amount to use 
	FLEXCONTROLLER_REMAP_EYELID
};

typedef int LocalFlexController_t;

struct mstudioflexop_t
{
	int	op;
	union
	{
		int		index;
		float	value;
	} d;
};

struct mstudioflexrule_t
{
	int					flex;
	int					numops;
	int					opindex;
	inline mstudioflexop_t *iFlexOp(int i) const { return  (mstudioflexop_t *)(((byte *)this) + opindex) + i; };
};

// ikinfo
struct mstudioiklink_t
{
	int		bone;
	Vector	kneeDir;	// ideal bending direction (per link, if applicable)
	Vector	unused0;	// unused
	mstudioiklink_t() {}
private:
	// No copy constructors allowed
	mstudioiklink_t(const mstudioiklink_t& vOther);
};

struct mstudioikchain_t
{
	int				sznameindex;
	inline char * const pszName(void) const { return ((char *)this) + sznameindex; }
	int				linktype;
	int				numlinks;
	int				linkindex;
	inline mstudioiklink_t *pLink(int i) const { return (mstudioiklink_t *)(((byte *)this) + linkindex) + i; };
	// FIXME: add unused entries
};

struct mstudiomouth_t
{
	int					bone;
	Vector				forward;
	int					flexdesc;
	mstudiomouth_t() {}
private:
	// No copy constructors allowed
	mstudiomouth_t(const mstudiomouth_t& vOther);
};

struct mstudioposeparamdesc_t
{
	int					sznameindex;
	inline char * const pszName(void) const { return ((char *)this) + sznameindex; }
	int					flags;	// ????
	float				start;	// starting value
	float				end;	// ending value
	float				loop;	// looping range, 0 for no looping, 360 for rotations, etc.
};

// demand loaded sequence groups
struct mstudiomodelgroup_t
{
	int					szlabelindex;	// textual name
	inline char * const pszLabel(void) const { return ((char *)this) + szlabelindex; }
	int					sznameindex;	// file name
	inline char * const pszName(void) const { return ((char *)this) + sznameindex; }
};

// used for piecewise loading of animation data
struct mstudioanimblock_t
{
	int					datastart;
	int					dataend;
};

class CStudioHdr;
struct mstudioflexcontrollerui_t
{
	int					sznameindex;
	inline char * const pszName(void) const { return ((char *)this) + sznameindex; }

	// These are used like a union to save space
	// Here are the possible configurations for a UI controller
	//
	// SIMPLE NON-STEREO:	0: control	1: unused	2: unused
	// STEREO:				0: left		1: right	2: unused
	// NWAY NON-STEREO:		0: control	1: unused	2: value
	// NWAY STEREO:			0: left		1: right	2: value

	int					szindex0;
	int					szindex1;
	int					szindex2;

	inline const mstudioflexcontroller_t *pController(void) const
	{
		return !stereo ? (mstudioflexcontroller_t *)((char *)this + szindex0) : NULL;
	}
	inline char * const	pszControllerName(void) const { return !stereo ? pController()->pszName() : NULL; }
	inline int			controllerIndex(const CStudioHdr &cStudioHdr) const;

	inline const mstudioflexcontroller_t *pLeftController(void) const
	{
		return stereo ? (mstudioflexcontroller_t *)((char *)this + szindex0) : NULL;
	}
	inline char * const	pszLeftName(void) const { return stereo ? pLeftController()->pszName() : NULL; }
	inline int			leftIndex(const CStudioHdr &cStudioHdr) const;

	inline const mstudioflexcontroller_t *pRightController(void) const
	{
		return stereo ? (mstudioflexcontroller_t *)((char *)this + szindex1) : NULL;
	}
	inline char * const	pszRightName(void) const { return stereo ? pRightController()->pszName() : NULL; }
	inline int			rightIndex(const CStudioHdr &cStudioHdr) const;

	inline const mstudioflexcontroller_t *pNWayValueController(void) const
	{
		return remaptype == FLEXCONTROLLER_REMAP_NWAY ? (mstudioflexcontroller_t *)((char *)this + szindex2) : NULL;
	}
	inline char * const	pszNWayValueName(void) const { return remaptype == FLEXCONTROLLER_REMAP_NWAY ? pNWayValueController()->pszName() : NULL; }
	inline int			nWayValueIndex(const CStudioHdr &cStudioHdr) const;

	// Number of controllers this ui description contains, 1, 2 or 3
	inline int			Count() const { return (stereo ? 2 : 1) + (remaptype == FLEXCONTROLLER_REMAP_NWAY ? 1 : 0); }
	inline const mstudioflexcontroller_t *pController(int index) const;

	unsigned char		remaptype;	// See the FlexControllerRemapType_t enum
	bool				stereo;		// Is this a stereo control?
	byte				unused[2];
};

struct mstudiolinearbone_t
{
	int numbones;

	int flagsindex;
	inline int flags(int i) const { Assert(i >= 0 && i < numbones); return *((int *)(((byte *)this) + flagsindex) + i); };
	inline int *pflags(int i) { Assert(i >= 0 && i < numbones); return ((int *)(((byte *)this) + flagsindex) + i); };

	int	parentindex;
	inline int parent(int i) const { Assert(i >= 0 && i < numbones); return *((int *)(((byte *)this) + parentindex) + i); };

	int	posindex;
	inline Vector pos(int i) const { Assert(i >= 0 && i < numbones); return *((Vector *)(((byte *)this) + posindex) + i); };

	int quatindex;
	inline Quaternion quat(int i) const { Assert(i >= 0 && i < numbones); return *((Quaternion *)(((byte *)this) + quatindex) + i); };

	int rotindex;
	inline RadianEuler rot(int i) const { Assert(i >= 0 && i < numbones); return *((RadianEuler *)(((byte *)this) + rotindex) + i); };

	int posetoboneindex;
	//inline matrix3x4_t poseToBone(int i) const { Assert(i >= 0 && i < numbones); return *((matrix3x4_t *)(((byte *)this) + posetoboneindex) + i); };

	int	posscaleindex;
	inline Vector posscale(int i) const { Assert(i >= 0 && i < numbones); return *((Vector *)(((byte *)this) + posscaleindex) + i); };

	int	rotscaleindex;
	inline Vector rotscale(int i) const { Assert(i >= 0 && i < numbones); return *((Vector *)(((byte *)this) + rotscaleindex) + i); };

	int	qalignmentindex;
	inline Quaternion qalignment(int i) const { Assert(i >= 0 && i < numbones); return *((Quaternion *)(((byte *)this) + qalignmentindex) + i); };

	int unused[6];

	mstudiolinearbone_t() {}
private:
	// No copy constructors allowed
	mstudiolinearbone_t(const mstudiolinearbone_t& vOther);
};

//-----------------------------------------------------------------------------
// Component is one of Translate X, Y or Z [0,2] (StudioBoneFlexComponent_t)
//-----------------------------------------------------------------------------
struct mstudioboneflexdrivercontrol_t
{
	int m_nBoneComponent;		// Bone component that drives flex, StudioBoneFlexComponent_t
	int m_nFlexControllerIndex;	// Flex controller to drive
	float m_flMin;				// Min value of bone component mapped to 0 on flex controller
	float m_flMax;				// Max value of bone component mapped to 1 on flex controller

	mstudioboneflexdrivercontrol_t() {}
private:
	// No copy constructors allowed
	mstudioboneflexdrivercontrol_t(const mstudioboneflexdrivercontrol_t &vOther);
};

struct mstudioboneflexdriver_t
{
	int m_nBoneIndex;			// Bone to drive flex controller
	int m_nControlCount;		// Number of flex controllers being driven
	int m_nControlIndex;		// Index into data where controllers are (relative to this)

	inline mstudioboneflexdrivercontrol_t *pBoneFlexDriverControl(int i) const
	{
		Assert(i >= 0 && i < m_nControlCount);
		return (mstudioboneflexdrivercontrol_t *)(((byte *)this) + m_nControlIndex) + i;
	}

	int unused[3];

	mstudioboneflexdriver_t() {}
private:
	// No copy constructors allowed
	mstudioboneflexdriver_t(const mstudioboneflexdriver_t &vOther);
};

//-----------------------------------------------------------------------------
// Src bone transforms are transformations that will convert .dmx or .smd-based animations into .mdl-based animations
// NOTE: The operation you should apply is: pretransform * bone transform * posttransform
//-----------------------------------------------------------------------------
struct mstudiosrcbonetransform_t
{
	int	sznameindex;
	inline const char *pszName(void) const { return ((char *)this) + sznameindex; }
	matrix3x4_t	pretransform;
	matrix3x4_t	posttransform;
};

struct studiohdr2_t
{
	// NOTE: For forward compat, make sure any methods in this struct
	// are also available in studiohdr_t so no leaf code ever directly references
	// a studiohdr2_t structure
	int numsrcbonetransform;
	int srcbonetransformindex;

	int	illumpositionattachmentindex;
	inline int			IllumPositionAttachmentIndex() const { return illumpositionattachmentindex; }

	float flMaxEyeDeflection;
	inline float		MaxEyeDeflection() const { return flMaxEyeDeflection != 0.0f ? flMaxEyeDeflection : 0.866f; } // default to cos(30) if not set

	int linearboneindex;
	inline mstudiolinearbone_t *pLinearBones() const { return (linearboneindex) ? (mstudiolinearbone_t *)(((byte *)this) + linearboneindex) : NULL; }

	int sznameindex;
	inline char *pszName() { return (sznameindex) ? (char *)(((byte *)this) + sznameindex) : NULL; }

	int m_nBoneFlexDriverCount;
	int m_nBoneFlexDriverIndex;
	inline mstudioboneflexdriver_t *pBoneFlexDriver(int i) const { Assert(i >= 0 && i < m_nBoneFlexDriverCount); return (mstudioboneflexdriver_t *)(((byte *)this) + m_nBoneFlexDriverIndex) + i; }

	int reserved[56];
};

struct studiohdr_t
{
	int					id;
	int					version;
	long				checksum;		// this has to be the same in the phy and vtx files to load!
	inline const char *	pszName(void) const { return name; }
	char				name[64];
	int					length;
	Vector				eyeposition;	// ideal eye position
	Vector				illumposition;	// illumination center
	Vector				hull_min;		// ideal movement hull size
	Vector				hull_max;
	Vector				view_bbmin;		// clipping bounding box
	Vector				view_bbmax;
	int					flags;
	int					numbones;			// bones
	int					boneindex;
	inline mstudiobone_t *pBone(int i) const
	{
		Assert(i >= 0 && i < numbones);
		return (mstudiobone_t *)(((byte *)this) + boneindex) + i;
	};
	int					RemapSeqBone(int iSequence, int iLocalBone) const;	// maps local sequence bone to global bone
	int					RemapAnimBone(int iAnim, int iLocalBone) const;		// maps local animations bone to global bone
	int					numbonecontrollers;		// bone controllers
	int					bonecontrollerindex;
	inline mstudiobonecontroller_t *pBonecontroller(int i) const { Assert(i >= 0 && i < numbonecontrollers); return (mstudiobonecontroller_t *)(((byte *)this) + bonecontrollerindex) + i; };
	int					numhitboxsets;
	int					hitboxsetindex;
	// Look up hitbox set by index
	mstudiohitboxset_t	*pHitboxSet(int i) const
	{
		Assert(i >= 0 && i < numhitboxsets);
		return (mstudiohitboxset_t *)(((byte *)this) + hitboxsetindex) + i;
	};
	// Calls through to hitbox to determine size of specified set
	inline mstudiobbox_t *pHitbox(int i, int set) const
	{
		mstudiohitboxset_t const *s = pHitboxSet(set);
		if (!s)
			return nullptr;

		return s->pHitbox(i);
	};
	// Calls through to set to get hitbox count for set
	inline int iHitboxCount(int set) const
	{
		mstudiohitboxset_t const *s = pHitboxSet(set);
		if (!s)
			return 0;

		return s->numhitboxes;
	};
	// file local animations? and sequences
	//private:
	int					numlocalanim;			// animations/poses
	int					localanimindex;		// animation descriptions
	inline mstudioanimdesc_t *pLocalAnimdesc(int i) const { return NULL; };

	int					numlocalseq;				// sequences
	int					localseqindex;
	inline mstudioseqdesc_t *pLocalSeqdesc(int i) const { if (i < 0 || i >= numlocalseq) i = 0; return (mstudioseqdesc_t *)(((byte *)this) + localseqindex) + i; };
	//public:
	bool				SequencesAvailable() const
	{
		return true;
	}
	int					GetNumSeq() const;
	mstudioanimdesc_t	&pAnimdesc(int i) const;
	mstudioseqdesc_t	&pSeqdesc(int i) const;
	int					iRelativeAnim(int baseseq, int relanim) const;	// maps seq local anim reference to global anim index
	int					iRelativeSeq(int baseseq, int relseq) const;		// maps seq local seq reference to global seq index
																			//private:
	mutable int			activitylistversion;	// initialization flag - have the sequences been indexed?
	mutable int			eventsindexed;
	//public:
	int					GetSequenceActivity(int iSequence);
	void				SetSequenceActivity(int iSequence, int iActivity);
	int					GetActivityListVersion(void);
	void				SetActivityListVersion(int version) const;
	int					GetEventListVersion(void);
	void				SetEventListVersion(int version);
	// raw textures
	int					numtextures;
	int					textureindex;
	inline mstudiotexture_t *pTexture(int i) const { return NULL; };
	// raw textures search paths
	int					numcdtextures;
	int					cdtextureindex;
	inline char			*pCdtexture(int i) const { return (((char *)this) + *((int *)(((byte *)this) + cdtextureindex) + i)); };
	// replaceable textures tables
	int					numskinref;
	int					numskinfamilies;
	int					skinindex;
	inline short		*pSkinref(int i) const { return (short *)(((byte *)this) + skinindex) + i; };
	int					numbodyparts;
	int					bodypartindex;
	inline mstudiobodyparts_t	*pBodypart(int i) const { return NULL; };
	// queryable attachable points
	//private:
	int					numlocalattachments;
	int					localattachmentindex;
	inline mstudioattachment_t	*pLocalAttachment(int i) const { Assert(i >= 0 && i < numlocalattachments); return (mstudioattachment_t *)(((byte *)this) + localattachmentindex) + i; };
	//public:
	int					GetNumAttachments(void) const;
	const mstudioattachment_t &pAttachment(int i) const;
	int					GetAttachmentBone(int i);
	// used on my tools in hlmv, not persistant
	void				SetAttachmentBone(int iAttachment, int iBone);

	// animation node to animation node transition graph
	//private:
	int					numlocalnodes;
	int					localnodeindex;
	int					localnodenameindex;
	inline char			*pszLocalNodeName(int iNode) const { Assert(iNode >= 0 && iNode < numlocalnodes); return (((char *)this) + *((int *)(((byte *)this) + localnodenameindex) + iNode)); }
	inline byte			*pLocalTransition(int i) const { Assert(i >= 0 && i < (numlocalnodes * numlocalnodes)); return (byte *)(((byte *)this) + localnodeindex) + i; };

	//public:
	int					EntryNode(int iSequence);
	int					ExitNode(int iSequence);
	char				*pszNodeName(int iNode);
	int					GetTransition(int iFrom, int iTo) const;

	int					numflexdesc;
	int					flexdescindex;
	inline mstudioflexdesc_t *pFlexdesc(int i) const { Assert(i >= 0 && i < numflexdesc); return (mstudioflexdesc_t *)(((byte *)this) + flexdescindex) + i; };

	int					numflexcontrollers;
	int					flexcontrollerindex;
	inline mstudioflexcontroller_t *pFlexcontroller(LocalFlexController_t i) const { Assert(numflexcontrollers == 0 || (i >= 0 && i < numflexcontrollers)); return (mstudioflexcontroller_t *)(((byte *)this) + flexcontrollerindex) + i; };

	int					numflexrules;
	int					flexruleindex;
	inline mstudioflexrule_t *pFlexRule(int i) const { Assert(i >= 0 && i < numflexrules); return (mstudioflexrule_t *)(((byte *)this) + flexruleindex) + i; };

	int					numikchains;
	int					ikchainindex;
	inline mstudioikchain_t *pIKChain(int i) const { Assert(i >= 0 && i < numikchains); return (mstudioikchain_t *)(((byte *)this) + ikchainindex) + i; };

	int					nummouths;
	int					mouthindex;
	inline mstudiomouth_t *pMouth(int i) const { Assert(i >= 0 && i < nummouths); return (mstudiomouth_t *)(((byte *)this) + mouthindex) + i; };

	//private:
	int					numlocalposeparameters;
	int					localposeparamindex;
	inline mstudioposeparamdesc_t *pLocalPoseParameter(int i) const { Assert(i >= 0 && i < numlocalposeparameters); return (mstudioposeparamdesc_t *)(((byte *)this) + localposeparamindex) + i; };
	//public:
	int					GetNumPoseParameters(void) const;
	const mstudioposeparamdesc_t &pPoseParameter(int i);
	int					GetSharedPoseParameter(int iSequence, int iLocalPose) const;

	int					surfacepropindex;
	inline char * const pszSurfaceProp(void) const { return ((char *)this) + surfacepropindex; }

	// Key values
	int					keyvalueindex;
	int					keyvaluesize;
	inline const char * KeyValueText(void) const { return keyvaluesize != 0 ? ((char *)this) + keyvalueindex : NULL; }

	int					numlocalikautoplaylocks;
	int					localikautoplaylockindex;
	inline mstudioiklock_t *pLocalIKAutoplayLock(int i) const { Assert(i >= 0 && i < numlocalikautoplaylocks); return (mstudioiklock_t *)(((byte *)this) + localikautoplaylockindex) + i; };

	int					GetNumIKAutoplayLocks(void) const;
	const mstudioiklock_t &pIKAutoplayLock(int i);
	int					CountAutoplaySequences() const;
	int					CopyAutoplaySequences(unsigned short *pOut, int outCount) const;
	int					GetAutoplayList(unsigned short **pOut) const;

	// The collision model mass that jay wanted
	float				mass;
	int					contents;

	// external animations, models, etc.
	int					numincludemodels;
	int					includemodelindex;
	inline mstudiomodelgroup_t *pModelGroup(int i) const { Assert(i >= 0 && i < numincludemodels); return (mstudiomodelgroup_t *)(((byte *)this) + includemodelindex) + i; };
	// implementation specific call to get a named model
	const studiohdr_t	*FindModel(void **cache, char const *modelname) const;

	// implementation specific back pointer to virtual data
	mutable void		*virtualModel;
	virtualmodel_t		*GetVirtualModel(void) const;

	// for demand loaded animation blocks
	int					szanimblocknameindex;
	inline char * const pszAnimBlockName(void) const { return ((char *)this) + szanimblocknameindex; }
	int					numanimblocks;
	int					animblockindex;
	inline mstudioanimblock_t *pAnimBlock(int i) const { Assert(i > 0 && i < numanimblocks); return (mstudioanimblock_t *)(((byte *)this) + animblockindex) + i; };
	mutable void		*animblockModel;
	byte *				GetAnimBlock(int i) const;

	int					bonetablebynameindex;
	inline const byte	*GetBoneTableSortedByName() const { return (byte *)this + bonetablebynameindex; }

	// used by tools only that don't cache, but persist mdl's peer data
	// engine uses virtualModel to back link to cache pointers
	void				*pVertexBase;
	void				*pIndexBase;

	// if STUDIOHDR_FLAGS_CONSTANT_DIRECTIONAL_LIGHT_DOT is set,
	// this value is used to calculate directional components of lighting 
	// on static props
	byte				constdirectionallightdot;

	// set during load of mdl data to track *desired* lod configuration (not actual)
	// the *actual* clamped root lod is found in studiohwdata
	// this is stored here as a global store to ensure the staged loading matches the rendering
	byte				rootLOD;

	// set in the mdl data to specify that lod configuration should only allow first numAllowRootLODs
	// to be set as root LOD:
	//	numAllowedRootLODs = 0	means no restriction, any lod can be set as root lod.
	//	numAllowedRootLODs = N	means that lod0 - lod(N-1) can be set as root lod, but not lodN or lower.
	byte				numAllowedRootLODs;

	byte				unused[1];

	int					unused4; // zero out if version < 47

	int					numflexcontrollerui;
	int					flexcontrolleruiindex;
	mstudioflexcontrollerui_t *pFlexControllerUI(int i) const { Assert(i >= 0 && i < numflexcontrollerui); return (mstudioflexcontrollerui_t *)(((byte *)this) + flexcontrolleruiindex) + i; }

	float				flVertAnimFixedPointScale;
	inline float		VertAnimFixedPointScale() const { return (flags & STUDIOHDR_FLAGS_VERT_ANIM_FIXED_POINT_SCALE) ? flVertAnimFixedPointScale : 1.0f / 4096.0f; }

	int					unused3[1];

	// FIXME: Remove when we up the model version. Move all fields of studiohdr2_t into studiohdr_t.
	int					studiohdr2index;
	studiohdr2_t*		pStudioHdr2() const { return (studiohdr2_t *)(((byte *)this) + studiohdr2index); }

	// Src bone transforms are transformations that will convert .dmx or .smd-based animations into .mdl-based animations
	int					NumSrcBoneTransforms() const { return studiohdr2index ? pStudioHdr2()->numsrcbonetransform : 0; }
	const mstudiosrcbonetransform_t* SrcBoneTransform(int i) const { Assert(i >= 0 && i < NumSrcBoneTransforms()); return (mstudiosrcbonetransform_t *)(((byte *)this) + pStudioHdr2()->srcbonetransformindex) + i; }

	inline int			IllumPositionAttachmentIndex() const { return studiohdr2index ? pStudioHdr2()->IllumPositionAttachmentIndex() : 0; }

	inline float		MaxEyeDeflection() const { return studiohdr2index ? pStudioHdr2()->MaxEyeDeflection() : 0.866f; } // default to cos(30) if not set

	inline mstudiolinearbone_t *pLinearBones() const { return studiohdr2index ? pStudioHdr2()->pLinearBones() : NULL; }

	inline int			BoneFlexDriverCount() const { return studiohdr2index ? pStudioHdr2()->m_nBoneFlexDriverCount : 0; }
	inline const mstudioboneflexdriver_t* BoneFlexDriver(int i) const { Assert(i >= 0 && i < BoneFlexDriverCount()); return studiohdr2index ? pStudioHdr2()->pBoneFlexDriver(i) : NULL; }

	// NOTE: No room to add stuff? Up the .mdl file format version 
	// [and move all fields in studiohdr2_t into studiohdr_t and kill studiohdr2_t],
	// or add your stuff to studiohdr2_t. See NumSrcBoneTransforms/SrcBoneTransform for the pattern to use.
	int					unused2[1];

	studiohdr_t() {}

private:
	// No copy constructors allowed
	studiohdr_t(const studiohdr_t& vOther);

	friend struct virtualmodel_t;
};

//-----------------------------------------------------------------------------
// Indicates the type of translucency of an unmodulated renderable
//-----------------------------------------------------------------------------
enum RenderableTranslucencyType_t
{
	RENDERABLE_IS_OPAQUE = 0,
	RENDERABLE_IS_TRANSLUCENT,
	RENDERABLE_IS_TWO_PASS,	// has both translucent and opaque sub-partsa
};

//-----------------------------------------------------------------------------
// Model info interface
//-----------------------------------------------------------------------------

abstract_class IVModelInfo
{
public:
	virtual							~IVModelInfo(void) { }
	virtual const model_t			*GetModel(int modelindex) const = 0;
	// Returns index of model by name
	virtual int						GetModelIndex(const char *name) const = 0;
	// Returns name of model
	virtual const char				*GetModelName(const model_t *model) const = 0;
private:
	virtual void					vpad0() {};
public:
	virtual vcollide_t				*GetVCollide(const model_t *model) const = 0;
	virtual vcollide_t				*GetVCollide(int modelindex) const = 0;
	virtual void					GetModelBounds(const model_t *model, Vector& mins, Vector& maxs) const = 0;
	virtual	void					GetModelRenderBounds(const model_t *model, Vector& mins, Vector& maxs) const = 0;
	virtual int						GetModelFrameCount(const model_t *model) const = 0;
	virtual int						GetModelType(const model_t *model) const = 0;
	virtual void					*GetModelExtraData(const model_t *model) = 0;
	virtual bool					ModelHasMaterialProxy(const model_t *model) const = 0;
	virtual bool					IsTranslucent(model_t const* model) const = 0;
	virtual bool					IsTranslucentTwoPass(const model_t *model) const = 0;
	virtual void					Unused0() {};
	virtual RenderableTranslucencyType_t ComputeTranslucencyType(const model_t *model, int nSkin, int nBody) = 0;
	virtual int						GetModelMaterialCount(const model_t* model) const = 0;
	virtual void					GetModelMaterials(const model_t *model, int count, IMaterial** ppMaterials) = 0;
	virtual bool					IsModelVertexLit(const model_t *model) const = 0;
	virtual const char				*GetModelKeyValueText(const model_t *model) = 0;
	virtual bool					GetModelKeyValue(const model_t *model, CUtlBuffer &buf) = 0; // supports keyvalue blocks in submodels
	virtual float					GetModelRadius(const model_t *model) = 0;

	virtual const studiohdr_t		*FindModel(const studiohdr_t *pStudioHdr, void **cache, const char *modelname) const = 0;
	virtual const studiohdr_t		*FindModel(void *cache) const = 0;
	virtual	virtualmodel_t			*GetVirtualModel(const studiohdr_t *pStudioHdr) const = 0;
	virtual byte					*GetAnimBlock(const studiohdr_t *pStudioHdr, int iBlock) const = 0;
	virtual bool					HasAnimBlockBeenPreloaded(studiohdr_t const*, int) const = 0;

	// Available on client only!!!
	virtual void					GetModelMaterialColorAndLighting(const model_t *model, Vector const& origin,
		QAngle const& angles, trace_t* pTrace,
		Vector& lighting, Vector& matColor) = 0;
	virtual void					GetIlluminationPoint(const model_t *model, IClientRenderable *pRenderable, Vector const& origin,
		QAngle const& angles, Vector* pLightingCenter) = 0;

	virtual int						GetModelContents(int modelIndex) const = 0;
private:
	virtual void vpad1() = 0;
public:
	virtual studiohdr_t				*GetStudioModel(const model_t *mod) = 0;
	virtual int						GetModelSpriteWidth(const model_t *model) const = 0;
	virtual int						GetModelSpriteHeight(const model_t *model) const = 0;

	// Sets/gets a map-specified fade range (client only)
	virtual void					SetLevelScreenFadeRange(float flMinSize, float flMaxSize) = 0;
	virtual void					GetLevelScreenFadeRange(float *pMinArea, float *pMaxArea) const = 0;

	// Sets/gets a map-specified per-view fade range (client only)
	virtual void					SetViewScreenFadeRange(float flMinSize, float flMaxSize) = 0;

	// Computes fade alpha based on distance fade + screen fade (client only)
	virtual unsigned char			ComputeLevelScreenFade(const Vector &vecAbsOrigin, float flRadius, float flFadeScale) const = 0;
	virtual unsigned char			ComputeViewScreenFade(const Vector &vecAbsOrigin, float flRadius, float flFadeScale) const = 0;

	// both client and server
	virtual int						GetAutoplayList(const studiohdr_t *pStudioHdr, unsigned short **pAutoplayList) const = 0;

	// Gets a virtual terrain collision model (creates if necessary)
	// NOTE: This may return NULL if the terrain model cannot be virtualized
	virtual CPhysCollide			*GetCollideForVirtualTerrain(int index) = 0;
	virtual bool					IsUsingFBTexture(const model_t *model, int nSkin, int nBody, void /*IClientRenderable*/ *pClientRenderable) const = 0;
	virtual const model_t			*FindOrLoadModel(const char *name) const = 0;
	virtual MDLHandle_t				GetCacheHandle(const model_t *model) const = 0;
	// Returns planes of non-nodraw brush model surfaces
	virtual int						GetBrushModelPlaneCount(const model_t *model) const = 0;
	virtual void					GetBrushModelPlane(const model_t *model, int nIndex, cplane_t &plane, Vector *pOrigin) const = 0;
	virtual int						GetSurfacepropsForVirtualTerrain(int index) = 0;
	virtual bool					UsesEnvCubemap(const model_t *model) const = 0;
	virtual bool					UsesStaticLighting(const model_t *model) const = 0;
};

SOURCE_END