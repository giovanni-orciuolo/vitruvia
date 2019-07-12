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
#define DECLARE_POINTER_HANDLE(name) struct name##__ { int unused; }; typedef struct name##__ *name

#define MODEL_KNIFE_T ("models/weapons/v_knife_default_t.mdl")
#define MODEL_KNIFE_CT ("models/weapons/v_knife_default_ct.mdl")
#define MODEL_KNIFE_BAYO ("models/weapons/v_knife_bayonet.mdl")
#define MODEL_KNIFE_FLIP ("models/weapons/v_knife_flip.mdl")
#define MODEL_KNIFE_GUT ("models/weapons/v_knife_gut.mdl")
#define MODEL_KNIFE_KARAM ("models/weapons/v_knife_karam.mdl")
#define MODEL_KNIFE_M9 ("models/weapons/v_knife_m9_bay.mdl")
#define MODEL_KNIFE_HUNTSMAN ("models/weapons/v_knife_tactical.mdl")
#define MODEL_KNIFE_FALCHION ("models/weapons/v_knife_falchion_advanced.mdl")
#define MODEL_KNIFE_BOWIE ("models/weapons/v_knife_survival_bowie.mdl")
#define MODEL_KNIFE_BUTTERFLY ("models/weapons/v_knife_butterfly.mdl")
#define MODEL_KNIFE_PUSH ("models/weapons/v_knife_push.mdl")

#define MAXSTUDIOSKINS		32		// total textures
#define MAXSTUDIOBONES		128		// total bones actually used
#define MAXSTUDIOFLEXDESC	1024	// maximum number of low level flexes (actual morph targets)
#define MAXSTUDIOFLEXCTRL	96		// maximum number of flexcontrollers (input sliders)
#define MAXSTUDIOPOSEPARAM	24
#define MAXSTUDIOBONECTRLS	4
#define MAXSTUDIOANIMBLOCKS 256

#define BONE_CALCULATE_MASK			0x1F
#define BONE_PHYSICALLY_SIMULATED	0x01	// bone is physically simulated when physics are active
#define BONE_PHYSICS_PROCEDURAL		0x02	// procedural when physics is active
#define BONE_ALWAYS_PROCEDURAL		0x04	// bone is always procedurally animated
#define BONE_SCREEN_ALIGN_SPHERE	0x08	// bone aligns to the screen, not constrained in motion.
#define BONE_SCREEN_ALIGN_CYLINDER	0x10	// bone aligns to the screen, constrained by it's own axis.

#define BONE_USED_MASK				0x0007FF00
#define BONE_USED_BY_ANYTHING		0x0007FF00
#define BONE_USED_BY_HITBOX			0x00000100	// bone (or child) is used by a hit box
#define BONE_USED_BY_ATTACHMENT		0x00000200	// bone (or child) is used by an attachment point
#define BONE_USED_BY_VERTEX_MASK	0x0003FC00
#define BONE_USED_BY_VERTEX_LOD0	0x00000400	// bone (or child) is used by the toplevel model via skinned vertex
#define BONE_USED_BY_VERTEX_LOD1	0x00000800	
#define BONE_USED_BY_VERTEX_LOD2	0x00001000  
#define BONE_USED_BY_VERTEX_LOD3	0x00002000
#define BONE_USED_BY_VERTEX_LOD4	0x00004000
#define BONE_USED_BY_VERTEX_LOD5	0x00008000
#define BONE_USED_BY_VERTEX_LOD6	0x00010000
#define BONE_USED_BY_VERTEX_LOD7	0x00020000
#define BONE_USED_BY_BONE_MERGE		0x00040000	// bone is available for bone merge to occur against it

#define BONE_USED_BY_VERTEX_AT_LOD(lod) ( BONE_USED_BY_VERTEX_LOD0 << (lod) )
#define BONE_USED_BY_ANYTHING_AT_LOD(lod) ( ( BONE_USED_BY_ANYTHING & ~BONE_USED_BY_VERTEX_MASK ) | BONE_USED_BY_VERTEX_AT_LOD(lod) )

#define MAX_NUM_LODS 8

#define BONE_TYPE_MASK				0x00F00000
#define BONE_FIXED_ALIGNMENT		0x00100000	// bone can't spin 360 degrees, all interpolation is normalized around a fixed orientation

#define BONE_HAS_SAVEFRAME_POS		0x00200000	// Vector48
#define BONE_HAS_SAVEFRAME_ROT64	0x00400000	// Quaternion64
#define BONE_HAS_SAVEFRAME_ROT32	0x00800000	// Quaternion32

#define	HITGROUP_GENERIC	0
#define	HITGROUP_HEAD		1
#define	HITGROUP_CHEST		2
#define	HITGROUP_STOMACH	3
#define HITGROUP_LEFTARM	4	
#define HITGROUP_RIGHTARM	5
#define HITGROUP_LEFTLEG	6
#define HITGROUP_RIGHTLEG	7
#define HITGROUP_GEAR		10			// alerts NPC, but doesn't do damage or bleed (1/100th damage)

// the command line param that tells the engine to use steam
#define STEAM_PARM					"-steam"
// the command line param to tell dedicated server to restart 
// if they are out of date
#define AUTO_RESTART "-autoupdate"

// the message a server sends when a clients steam login is expired
#define INVALID_STEAM_TICKET "Invalid STEAM UserID Ticket\n"
#define INVALID_STEAM_LOGON "No Steam logon\n"
#define INVALID_STEAM_VACBANSTATE "VAC banned from secure server\n"
#define INVALID_STEAM_LOGGED_IN_ELSEWHERE "This Steam account is being used in another location\n"

// This is the default, see shareddefs.h for mod-specific value, which can override this
#define DEFAULT_TICK_INTERVAL	(1.0f / 60.0f)				// 16.666667 msec is the default
#define MINIMUM_TICK_INTERVAL   (0.001)
#define MAXIMUM_TICK_INTERVAL	(0.1)

// This is the max # of players the engine can handle
// Note, must be power of 2
#define ABSOLUTE_PLAYER_LIMIT 255
#define ABSOLUTE_PLAYER_LIMIT_DW	( (ABSOLUTE_PLAYER_LIMIT/32) + 1 )

#define MAX_MAP_NAME				32	
#define	MAX_NETWORKID_LENGTH		64  // num chars for a network (i.e steam) ID

// BUGBUG: Reconcile with or derive this from the engine's internal definition!
// FIXME: I added an extra bit because I needed to make it signed
#define SP_MODEL_INDEX_BITS			11

// How many bits to use to encode an edict.
#define	MAX_EDICT_BITS				11			// # of bits needed to represent max edicts
// Max # of edicts in a level
#define	MAX_EDICTS					(1<<MAX_EDICT_BITS)

// How many bits to use to encode an server class index
#define MAX_SERVER_CLASS_BITS		9
// Max # of networkable server classes
#define MAX_SERVER_CLASSES			(1<<MAX_SERVER_CLASS_BITS)

// Networked ehandles use less bits to encode the serial number.
#define NUM_NETWORKED_EHANDLE_SERIAL_NUMBER_BITS	10
#define NUM_NETWORKED_EHANDLE_BITS					(MAX_EDICT_BITS + NUM_NETWORKED_EHANDLE_SERIAL_NUMBER_BITS)
#define INVALID_NETWORKED_EHANDLE_VALUE				((1 << NUM_NETWORKED_EHANDLE_BITS) - 1)

// This is the maximum amount of data a PackedEntity can have. Having a limit allows us
// to use static arrays sometimes instead of allocating memory all over the place.
#define MAX_PACKEDENTITY_DATA	(16384)

// This is the maximum number of properties that can be delta'd. Must be evenly divisible by 8.
#define MAX_PACKEDENTITY_PROPS	(4096)

// a client can have up to 4 customization files (logo, sounds, models, txt).
#define MAX_CUSTOM_FILES		4		// max 4 files
#define MAX_CUSTOM_FILE_SIZE	131072

SOURCE_INIT

//-----------------------------------------------------------------------------
// Skybox visibility
//-----------------------------------------------------------------------------
enum SkyboxVisibility_t
{
	SKYBOX_NOT_VISIBLE = 0,
	SKYBOX_3DSKYBOX_VISIBLE,
	SKYBOX_2DSKYBOX_VISIBLE,
};


enum EngineInputContextId_t
{
	ENGINE_INPUT_CONTEXT_GAME = 0,
	ENGINE_INPUT_CONTEXT_GAMEUI,
};

//-----------------------------------------------------------------------------
// Purpose: The engine reports to the client DLL what stage it's entering so the DLL can latch events
//  and make sure that certain operations only happen during the right stages.
// The value for each stage goes up as you move through the frame so you can check ranges of values
//  and if new stages get added in-between, the range is still valid.
//-----------------------------------------------------------------------------
enum ClientFrameStage_t
{
	// (haven't run any frames yet)
	FRAME_UNDEFINED = -1,
	FRAME_START,
	// A network packet is being recieved
	FRAME_NET_UPDATE_START,
	// Data has been received and we're going to start calling PostDataUpdate
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	// Data has been received and we've called PostDataUpdate on all data recipients
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	// We've received all packets, we can now do interpolation, prediction, etc..
	FRAME_NET_UPDATE_END,
	// We're about to start rendering the scene
	FRAME_RENDER_START,
	// We've finished rendering the scene.
	FRAME_RENDER_END
};

// Used by RenderView
enum RenderViewInfo_t
{
	RENDERVIEW_UNSPECIFIED = 0,
	RENDERVIEW_DRAWVIEWMODEL = (1 << 0),
	RENDERVIEW_DRAWHUD = (1 << 1),
	RENDERVIEW_SUPPRESSMONITORRENDERING = (1 << 2),
};

// Spectator Movement modes
enum ObserverMode_t
{
	OBSERVER_NONE = 0,	// not in spectator mode

	OBSERVER_DEATHCAM,	// special mode for death cam animation
	OBSERVER_FREEZECAM,	// zooms to a target, and freeze-frames on them
	OBSERVER_FIXED,		// view from a fixed camera position
	OBSERVER_IN_EYE,	// follow a player in first person view
	OBSERVER_CHASE,		// follow a player in third person view
	OBSERVER_ROAMING,	// free roaming

	OBSERVER_OTHER,
};

enum Hitboxes
{
	HITBOX_HEAD,
	HITBOX_NECK,
	HITBOX_LOWER_NECK,
	HITBOX_PELVIS,
	HITBOX_BODY,
	HITBOX_THORAX,
	HITBOX_CHEST,
	HITBOX_UPPER_CHEST,
	HITBOX_RIGHT_THIGH,
	HITBOX_LEFT_THIGH,
	HITBOX_RIGHT_CALF,
	HITBOX_LEFT_CALF,
	HITBOX_RIGHT_FOOT,
	HITBOX_LEFT_FOOT,
	HITBOX_RIGHT_HAND,
	HITBOX_LEFT_HAND,
	HITBOX_RIGHT_UPPER_ARM,
	HITBOX_RIGHT_FOREARM,
	HITBOX_LEFT_UPPER_ARM,
	HITBOX_LEFT_FOREARM,
	HITBOX_MAX
};
FORCEINLINE bool IsHitboxLowerBody(int hitbox) {
	switch (hitbox)
	{
	//case HITBOX_PELVIS:
	//case HITBOX_BODY:
	//case HITBOX_RIGHT_THIGH:
	//case HITBOX_LEFT_THIGH:
	case HITBOX_RIGHT_CALF:
	case HITBOX_LEFT_CALF:
	case HITBOX_RIGHT_FOOT:
	case HITBOX_LEFT_FOOT:
		return true;
	default: break;
	}
	return false;
}

enum Teams
{
	TEAM_NONE = 0,
	TEAM_SPEC = 1,
	TEAM_T = 2,
	TEAM_CT = 3
};

enum Weapons
{
	WEAPON_DEAGLE = 1,
	WEAPON_ELITE = 2,
	WEAPON_FIVESEVEN = 3,
	WEAPON_GLOCK = 4,
	WEAPON_AK47 = 7,
	WEAPON_AUG = 8,
	WEAPON_AWP = 9,
	WEAPON_FAMAS = 10,
	WEAPON_G3SG1 = 11,
	WEAPON_GALILAR = 13,
	WEAPON_M249 = 14,
	WEAPON_M4A4 = 16,
	WEAPON_MAC10 = 17,
	WEAPON_P90 = 19,
	WEAPON_UMP45 = 24,
	WEAPON_XM1014 = 25,
	WEAPON_BIZON = 26,
	WEAPON_MAG7 = 27,
	WEAPON_NEGEV = 28,
	WEAPON_SAWEDOFF = 29,
	WEAPON_TEC9 = 30,
	WEAPON_TASER = 31,
	WEAPON_HKP2000 = 32,
	WEAPON_MP7 = 33,
	WEAPON_MP9 = 34,
	WEAPON_NOVA = 35,
	WEAPON_P250 = 36,
	WEAPON_SCAR20 = 38,
	WEAPON_SG556 = 39,
	WEAPON_SSG08 = 40,
	WEAPON_KNIFE = 42,
	WEAPON_FLASHBANG = 43,
	WEAPON_HEGRENADE = 44,
	WEAPON_SMOKEGRENADE = 45,
	WEAPON_MOLOTOV = 46,
	WEAPON_DECOY = 47,
	WEAPON_INCGRENADE = 48,
	WEAPON_C4 = 49,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER = 60,
	WEAPON_USP_SILENCER = 61,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER = 64,
	WEAPON_KNIFE_BAYONET = 500,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT = 506,
	WEAPON_KNIFE_KARAMBIT = 507,
	WEAPON_KNIFE_M9_BAYONET = 508,
	WEAPON_KNIFE_TACTICAL = 509,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY = 515,
	WEAPON_KNIFE_PUSH = 516,
	GLOVE_STUDDED_BLOODHOUND = 5027,
	GLOVE_T_SIDE = 5028,
	GLOVE_CT_SIDE = 5029,
	GLOVE_SPORTY = 5030,
	GLOVE_SLICK = 5031,
	GLOVE_LEATHER_WRAP = 5032,
	GLOVE_MOTORCYCLE = 5033,
	GLOVE_SPECIALIST = 5034,
	GLOVE_HYDRA = 5035
};

inline bool IsKnife(int i) { return (i >= WEAPON_KNIFE_BAYONET && i < GLOVE_STUDDED_BLOODHOUND) || i == WEAPON_KNIFE_T || i == WEAPON_KNIFE; }

enum SigOnState
{
	STATE_NONE = 0,
	CHALLENGE = 1,
	CONNECTED = 2,
	NEW = 3,
	PRESPAWN = 4,
	SPAWN = 5,
	FULL_CONNECTED = 6,
	CHANGELEVEL = 7
};

enum Knives
{
	KNIFE_DEFAULT,
	KNIFE_BAYONET,
	KNIFE_FLIP,
	KNIFE_GUT,
	KNIFE_KARAMBIT,
	KNIFE_M9,
	KNIFE_TACTICAL,
	KNIFE_FALCHION,
	KNIFE_BOWIE,
	KNIFE_BUTTERFLY,
	KNIFE_PUSH,

	MAX_KNIVES = KNIFE_PUSH // Should always be the last element
};

enum Qualities
{
	Q_NORMAL,
	Q_GENIUNE,
	Q_VINTAGE,
	Q_UNUSUAL,
	Q_DEFAULT,
	Q_COMMUNITY,
	Q_DEVELOPER,
	Q_SELFMADE,
	Q_CUSTOMIZED,
	Q_STRANGE,
	Q_COMPLETED,
	Q_SOUVENIR
};

enum class WeaponTypes
{
	WEAPONTYPE_KNIFE = 0,
	WEAPONTYPE_PISTOL,
	WEAPONTYPE_SUBMACHINEGUN,
	WEAPONTYPE_RIFLE,
	WEAPONTYPE_SHOTGUN,
	WEAPONTYPE_SNIPER_RIFLE,
	WEAPONTYPE_MACHINEGUN,
	WEAPONTYPE_C4,
	WEAPONTYPE_UNKNOWN,
	WEAPONTYPE_GRENADE,
};

enum class CSClasses // Must specify CSClasses::Whatever in order to use smth from here
					 // Last updated -- 29 November 2016 (Glove Update)
{
	CAI_BaseNPC = 0,
	CAK47 = 1,
	CBaseAnimating = 2,
	CBaseAnimatingOverlay = 3,
	CBaseAttributableItem = 4,
	CBaseButton = 5,
	CBaseCombatCharacter = 6,
	CBaseCombatWeapon = 7,
	CBaseCSGrenade = 8,
	CBaseCSGrenadeProjectile = 9,
	CBaseDoor = 10,
	CBaseEntity = 11,
	CBaseFlex = 12,
	CBaseGrenade = 13,
	CBaseParticleEntity = 14,
	CBasePlayer = 15,
	CBasePropDoor = 16,
	CBaseTeamObjectiveResource = 17,
	CBaseTempEntity = 18,
	CBaseToggle = 19,
	CBaseTrigger = 20,
	CBaseViewModel = 21,
	CBaseVPhysicsTrigger = 22,
	CBaseWeaponWorldModel = 23,
	CBeam = 24,
	CBeamSpotlight = 25,
	CBoneFollower = 26,
	CBreakableProp = 27,
	CBreakableSurface = 28,
	CC4 = 29,
	CCascadeLight = 30,
	CChicken = 31,
	CColorCorrection = 32,
	CColorCorrectionVolume = 33,
	CCSGameRulesProxy = 34,
	CCSPlayer = 35,
	CCSPlayerResource = 36,
	CCSRagdoll = 37,
	CCSTeam = 38,
	CDEagle = 39,
	CDecoyGrenade = 40,
	CDecoyProjectile = 41,
	CDynamicLight = 42,
	CDynamicProp = 43,
	CEconEntity = 44,
	CEconWearable = 45,
	CEmbers = 46,
	CEntityDissolve = 47,
	CEntityFlame = 48,
	CEntityFreezing = 49,
	CEntityParticleTrail = 50,
	CEnvAmbientLight = 51,
	CEnvDetailController = 52,
	CEnvDOFController = 53,
	CEnvParticleScript = 54,
	CEnvProjectedTexture = 55,
	CEnvQuadraticBeam = 56,
	CEnvScreenEffect = 57,
	CEnvScreenOverlay = 58,
	CEnvTonemapController = 59,
	CEnvWind = 60,
	CFEPlayerDecal = 61,
	CFireCrackerBlast = 62,
	CFireSmoke = 63,
	CFireTrail = 64,
	CFish = 65,
	CFlashbang = 66,
	CFogController = 67,
	CFootstepControl = 68,
	CFunc_Dust = 69,
	CFunc_LOD = 70,
	CFuncAreaPortalWindow = 71,
	CFuncBrush = 72,
	CFuncConveyor = 73,
	CFuncLadder = 74,
	CFuncMonitor = 75,
	CFuncMoveLinear = 76,
	CFuncOccluder = 77,
	CFuncReflectiveGlass = 78,
	CFuncRotating = 79,
	CFuncSmokeVolume = 80,
	CFuncTrackTrain = 81,
	CGameRulesProxy = 82,
	CHandleTest = 83,
	CHEGrenade = 84,
	CHostage = 85,
	CHostageCarriableProp = 86,
	CIncendiaryGrenade = 87,
	CInferno = 88,
	CInfoLadderDismount = 89,
	CInfoOverlayAccessor = 90,
	CItem_Healthshot = 91,
	CKnife = 92,
	CKnifeGG = 93,
	CLightGlow = 94,
	CMaterialModifyControl = 95,
	CMolotovGrenade = 96,
	CMolotovProjectile = 97,
	CMovieDisplay = 98,
	CParticleFire = 99,
	CParticlePerformanceMonitor = 100,
	CParticleSystem = 101,
	CPhysBox = 102,
	CPhysBoxMultiplayer = 103,
	CPhysicsProp = 104,
	CPhysicsPropMultiplayer = 105,
	CPhysMagnet = 106,
	CPlantedC4 = 107,
	CPlasma = 108,
	CPlayerResource = 109,
	CPointCamera = 110,
	CPointCommentaryNode = 111,
	CPoseController = 112,
	CPostProcessController = 113,
	CPrecipitation = 114,
	CPrecipitationBlocker = 115,
	CPredictedViewModel = 116,
	CProp_Hallucination = 117,
	CPropDoorRotating = 118,
	CPropJeep = 119,
	CPropVehicleDriveable = 120,
	CRagdollManager = 121,
	CRagdollProp = 122,
	CRagdollPropAttached = 123,
	CRopeKeyframe = 124,
	CSCAR17 = 125,
	CSceneEntity = 126,
	CSensorGrenade = 127,
	CSensorGrenadeProjectile = 128,
	CShadowControl = 129,
	CSlideshowDisplay = 130,
	CSmokeGrenade = 131,
	CSmokeGrenadeProjectile = 132,
	CSmokeStack = 133,
	CSpatialEntity = 134,
	CSpotlightEnd = 135,
	CSprite = 136,
	CSpriteOriented = 137,
	CSpriteTrail = 138,
	CStatueProp = 139,
	CSteamJet = 140,
	CSun = 141,
	CSunlightShadowControl = 142,
	CTeam = 143,
	CTeamplayRoundBasedRulesProxy = 144,
	CTEArmorRicochet = 145,
	CTEBaseBeam = 146,
	CTEBeamEntPoint = 147,
	CTEBeamEnts = 148,
	CTEBeamFollow = 149,
	CTEBeamLaser = 150,
	CTEBeamPoints = 151,
	CTEBeamRing = 152,
	CTEBeamRingPoint = 153,
	CTEBeamSpline = 154,
	CTEBloodSprite = 155,
	CTEBloodStream = 156,
	CTEBreakModel = 157,
	CTEBSPDecal = 158,
	CTEBubbles = 159,
	CTEBubbleTrail = 160,
	CTEClientProjectile = 161,
	CTEDecal = 162,
	CTEDust = 163,
	CTEDynamicLight = 164,
	CTEEffectDispatch = 165,
	CTEEnergySplash = 166,
	CTEExplosion = 167,
	CTEFireBullets = 168,
	CTEFizz = 169,
	CTEFootprintDecal = 170,
	CTEFoundryHelpers = 171,
	CTEGaussExplosion = 172,
	CTEGlowSprite = 173,
	CTEImpact = 174,
	CTEKillPlayerAttachments = 175,
	CTELargeFunnel = 176,
	CTEMetalSparks = 177,
	CTEMuzzleFlash = 178,
	CTEParticleSystem = 179,
	CTEPhysicsProp = 180,
	CTEPlantBomb = 181,
	CTEPlayerAnimEvent = 182,
	CTEPlayerDecal = 183,
	CTEProjectedDecal = 184,
	CTERadioIcon = 185,
	CTEShatterSurface = 186,
	CTEShowLine = 187,
	CTesla = 188,
	CTESmoke = 189,
	CTESparks = 190,
	CTESprite = 191,
	CTESpriteSpray = 192,
	CTest_ProxyToggle_Networkable = 193,
	CTestTraceline = 194,
	CTEWorldDecal = 195,
	CTriggerPlayerMovement = 196,
	CTriggerSoundOperator = 197,
	CVGuiScreen = 198,
	CVoteController = 199,
	CWaterBullet = 200,
	CWaterLODControl = 201,
	CWeaponAug = 202,
	CWeaponAWP = 203,
	CWeaponBaseItem = 204,
	CWeaponBizon = 205,
	CWeaponCSBase = 206,
	CWeaponCSBaseGun = 207,
	CWeaponCycler = 208,
	CWeaponElite = 209,
	CWeaponFamas = 210,
	CWeaponFiveSeven = 211,
	CWeaponG3SG1 = 212,
	CWeaponGalil = 213,
	CWeaponGalilAR = 214,
	CWeaponGlock = 215,
	CWeaponHKP2000 = 216,
	CWeaponM249 = 217,
	CWeaponM3 = 218,
	CWeaponM4A1 = 219,
	CWeaponMAC10 = 220,
	CWeaponMag7 = 221,
	CWeaponMP5Navy = 222,
	CWeaponMP7 = 223,
	CWeaponMP9 = 224,
	CWeaponNegev = 225,
	CWeaponNOVA = 226,
	CWeaponP228 = 227,
	CWeaponP250 = 228,
	CWeaponP90 = 229,
	CWeaponSawedoff = 230,
	CWeaponSCAR20 = 231,
	CWeaponScout = 232,
	CWeaponSG550 = 233,
	CWeaponSG552 = 234,
	CWeaponSG556 = 235,
	CWeaponSSG08 = 236,
	CWeaponTaser = 237,
	CWeaponTec9 = 238,
	CWeaponTMP = 239,
	CWeaponUMP45 = 240,
	CWeaponUSP = 241,
	CWeaponXM1014 = 242,
	CWorld = 243,
	DustTrail = 244,
	MovieExplosion = 245,
	ParticleSmokeGrenade = 246,
	RocketTrail = 247,
	SmokeTrail = 248,
	SporeExplosion = 249,
	SporeTrail = 250
};

enum class EventType
{
	BOOL,
	SHORT,
	LONG,
	FLOAT,
	STRING,
	WSTRING
};

// NOTE: All of these are commented out; NotifyShouldTransmit actually
// has all these in them. Left it as an enum in case we want to go back though
enum DataUpdateType_t
{
	DATA_UPDATE_CREATED = 0,	// indicates it was created +and+ entered the pvs
	//	DATA_UPDATE_ENTERED_PVS,
	DATA_UPDATE_DATATABLE_CHANGED,
	//	DATA_UPDATE_LEFT_PVS,
	//	DATA_UPDATE_DESTROYED,		// FIXME: Could enable this, but it's a little worrying
									// since it changes a bunch of existing code
};

//-----------------------------------------------------------------------------
// Lightcache entry handle
//-----------------------------------------------------------------------------
DECLARE_POINTER_HANDLE(LightCacheHandle_t);

//-----------------------------------------------------------------------------
// Occlusion parameters
//-----------------------------------------------------------------------------
struct OcclusionParams_t
{
	float	m_flMaxOccludeeArea;
	float	m_flMinOccluderArea;
};

// edict->movetype values
enum MoveType_t
{
	MOVETYPE_NONE = 0,			// never moves
	MOVETYPE_ISOMETRIC,			// For players -- in TF2 commander view, etc.
	MOVETYPE_WALK,				// Player only - moving on the ground
	MOVETYPE_STEP,				// gravity, special edge handling -- monsters use this
	MOVETYPE_FLY,				// No gravity, but still collides with stuff
	MOVETYPE_FLYGRAVITY,		// flies through the air + is affected by gravity
	MOVETYPE_VPHYSICS,			// uses VPHYSICS for simulation
	MOVETYPE_PUSH,				// no clip to world, push and crush
	MOVETYPE_NOCLIP,			// No gravity, no collisions, still do velocity/avelocity
	MOVETYPE_LADDER,			// Used by players only when going onto a ladder
	MOVETYPE_OBSERVER,			// Observer movement, depends on player's observer mode
	MOVETYPE_CUSTOM,			// Allows the entity to describe its own physics

	// should always be defined as the last item in the list
	MOVETYPE_LAST = MOVETYPE_CUSTOM,

	MOVETYPE_MAX_BITS = 4
};

// edict->movecollide values
enum MoveCollide_t
{
	MOVECOLLIDE_DEFAULT = 0,

	// These ones only work for MOVETYPE_FLY + MOVETYPE_FLYGRAVITY
	MOVECOLLIDE_FLY_BOUNCE,	// bounces, reflects, based on elasticity of surface and object - applies friction (adjust velocity)
	MOVECOLLIDE_FLY_CUSTOM,	// Touch() will modify the velocity however it likes
	MOVECOLLIDE_FLY_SLIDE,  // slides along surfaces (no bounce) - applies friciton (adjusts velocity)

	MOVECOLLIDE_COUNT,		// Number of different movecollides

	// When adding new movecollide types, make sure this is correct
	MOVECOLLIDE_MAX_BITS = 3
};

// edict->solid values
// NOTE: Some movetypes will cause collisions independent of SOLID_NOT/SOLID_TRIGGER when the entity moves
// SOLID only effects OTHER entities colliding with this one when they move - UGH!

// Solid type basically describes how the bounding volume of the object is represented
// NOTE: SOLID_BBOX MUST BE 2, and SOLID_VPHYSICS MUST BE 6
// NOTE: These numerical values are used in the FGD by the prop code (see prop_dynamic)
enum SolidType_t
{
	SOLID_NONE = 0,	// no solid model
	SOLID_BSP = 1,	// a BSP tree
	SOLID_BBOX = 2,	// an AABB
	SOLID_OBB = 3,	// an OBB (not implemented yet)
	SOLID_OBB_YAW = 4,	// an OBB, constrained so that it can only yaw
	SOLID_CUSTOM = 5,	// Always call into the entity for tests
	SOLID_VPHYSICS = 6,	// solid vphysics object, get vcollide from the model and collide with that
	SOLID_LAST,
};

enum SolidFlags_t
{
	FSOLID_CUSTOMRAYTEST = 0x0001,			// Ignore solid type + always call into the entity for ray tests
	FSOLID_CUSTOMBOXTEST = 0x0002,			// Ignore solid type + always call into the entity for swept box tests
	FSOLID_NOT_SOLID = 0x0004,				// Are we currently not solid?
	FSOLID_TRIGGER = 0x0008,				// This is something may be collideable but fires touch functions
											// even when it's not collideable (when the FSOLID_NOT_SOLID flag is set)
	FSOLID_NOT_STANDABLE = 0x0010,			// You can't stand on this
	FSOLID_VOLUME_CONTENTS = 0x0020,		// Contains volumetric contents (like water)
	FSOLID_FORCE_WORLD_ALIGNED = 0x0040,	// Forces the collision rep to be world-aligned even if it's SOLID_BSP or SOLID_VPHYSICS
	FSOLID_USE_TRIGGER_BOUNDS = 0x0080,		// Uses a special trigger bounds separate from the normal OBB
	FSOLID_ROOT_PARENT_ALIGNED = 0x0100,	// Collisions are defined in root parent's local coordinate space
	FSOLID_TRIGGER_TOUCH_DEBRIS = 0x0200,	// This trigger will touch debris objects
	FSOLID_TRIGGER_TOUCH_PLAYER = 0x0400,	// This trigger will touch only players
	FSOLID_NOT_MOVEABLE = 0x0800,			// Assume this object will not move

	FSOLID_MAX_BITS = 12
};

//-----------------------------------------------------------------------------
// A couple of inline helper methods
//-----------------------------------------------------------------------------
inline bool IsSolid(SolidType_t solidType, int nSolidFlags)
{
	return (solidType != SOLID_NONE) && ((nSolidFlags & FSOLID_NOT_SOLID) == 0);
}

// m_lifeState values
#define	LIFE_ALIVE				0 // alive
#define	LIFE_DYING				1 // playing death animation or still falling off of a ledge waiting to hit ground
#define	LIFE_DEAD				2 // dead. lying still.
#define LIFE_RESPAWNABLE		3
#define LIFE_DISCARDBODY		4

// entity effects
enum
{
	EF_BONEMERGE = 0x001,			// Performs bone merge on client side
	EF_BRIGHTLIGHT = 0x002,			// DLIGHT centered at entity origin
	EF_DIMLIGHT = 0x004,			// player flashlight
	EF_NOINTERP = 0x008,			// don't interpolate the next frame
	EF_NOSHADOW = 0x010,			// Don't cast no shadow
	EF_NODRAW = 0x020,				// don't draw entity
	EF_NORECEIVESHADOW = 0x040,		// Don't receive no shadow
	EF_BONEMERGE_FASTCULL = 0x080,	// For use with EF_BONEMERGE. If this is set, then it places this ent's origin at its
									// parent and uses the parent's bbox + the max extents of the aiment.
									// Otherwise, it sets up the parent's bones every frame to figure out where to place
									// the aiment, which is inefficient because it'll setup the parent's bones even if
									// the parent is not in the PVS.
	EF_ITEM_BLINK = 0x100,			// blink an item so that the user notices it.
	EF_PARENT_ANIMATES = 0x200,		// always assume that the parent entity is animating

	EF_MARKED_FOR_FAST_REFLECTION = 0x400,
	EF_NOSHADOWDEPTH = 0x800,
	EF_NOFLASHLIGHT = 0x1000,

	EF_MAX_BITS = 10
};

#define EF_PARITY_BITS	3
#define EF_PARITY_MASK  ((1<<EF_PARITY_BITS)-1)

// How many bits does the muzzle flash parity thing get?
#define EF_MUZZLEFLASH_BITS 2

// plats
#define	PLAT_LOW_TRIGGER	1

// Trains
#define	SF_TRAIN_WAIT_RETRIGGER	1
#define SF_TRAIN_PASSABLE		8		// Train is not solid -- used to make water trains

// view angle update types for CPlayerState::fixangle
#define FIXANGLE_NONE			0
#define FIXANGLE_ABSOLUTE		1
#define FIXANGLE_RELATIVE		2

// Break Model Defines

#define BREAK_GLASS		0x01
#define BREAK_METAL		0x02
#define BREAK_FLESH		0x04
#define BREAK_WOOD		0x08

#define BREAK_SMOKE		0x10
#define BREAK_TRANS		0x20
#define BREAK_CONCRETE	0x40

// If this is set, then we share a lighting origin with the last non-slave breakable sent down to the client
#define BREAK_SLAVE		0x80

// Colliding temp entity sounds

#define BOUNCE_GLASS	BREAK_GLASS
#define	BOUNCE_METAL	BREAK_METAL
#define BOUNCE_FLESH	BREAK_FLESH
#define BOUNCE_WOOD		BREAK_WOOD
#define BOUNCE_SHRAP	0x10
#define BOUNCE_SHELL	0x20
#define	BOUNCE_CONCRETE BREAK_CONCRETE
#define BOUNCE_SHOTSHELL 0x80

// Temp entity bounce sound types
#define TE_BOUNCE_NULL		0
#define TE_BOUNCE_SHELL		1
#define TE_BOUNCE_SHOTSHELL	2

// Rendering constants
// if this is changed, update common/MaterialSystem/Sprite.cpp
enum RenderMode_t
{
	kRenderNormal = 0,		// src
	kRenderTransColor,		// c*a+dest*(1-a)
	kRenderTransTexture,	// src*a+dest*(1-a)
	kRenderGlow,			// src*a+dest -- No Z buffer checks -- Fixed size in screen space
	kRenderTransAlpha,		// src*srca+dest*(1-srca)
	kRenderTransAdd,		// src*a+dest
	kRenderEnvironmental,	// not drawn, used for environmental effects
	kRenderTransAddFrameBlend, // use a fractional frame value to blend between animation frames
	kRenderTransAlphaAdd,	// src + dest*(1-a)
	kRenderWorldGlow,		// Same as kRenderGlow but not fixed size in screen space
	kRenderNone,			// Don't render.

	kRenderModeCount,		// must be last
};

enum RenderFx_t
{
	kRenderFxNone = 0,
	kRenderFxPulseSlow,
	kRenderFxPulseFast,
	kRenderFxPulseSlowWide,
	kRenderFxPulseFastWide,

	kRenderFxFadeSlow,
	kRenderFxFadeFast,
	kRenderFxSolidSlow,
	kRenderFxSolidFast,
	kRenderFxStrobeSlow,

	kRenderFxStrobeFast,
	kRenderFxStrobeFaster,
	kRenderFxFlickerSlow,
	kRenderFxFlickerFast,
	kRenderFxNoDissipation,

	kRenderFxFadeOut,
	kRenderFxFadeIn,
	kRenderFxPulseFastWider,
	kRenderFxGlowShell,			// Glowing Shell

	kRenderFxMax
};

enum Collision_Group_t
{
	COLLISION_GROUP_NONE = 0,
	COLLISION_GROUP_DEBRIS,				// Collides with nothing but world and static stuff
	COLLISION_GROUP_DEBRIS_TRIGGER,		// Same as debris, but hits triggers
	COLLISION_GROUP_INTERACTIVE_DEBRIS,	// Collides with everything except other interactive debris or debris
	COLLISION_GROUP_INTERACTIVE,		// Collides with everything except interactive debris or debris
	COLLISION_GROUP_PLAYER,
	COLLISION_GROUP_BREAKABLE_GLASS,
	COLLISION_GROUP_VEHICLE,
	COLLISION_GROUP_PLAYER_MOVEMENT,	// For HL2, same as Collision_Group_Player, for
										// TF2, this filters out other players and CBaseObjects
	COLLISION_GROUP_NPC,				// Generic NPC group
	COLLISION_GROUP_IN_VEHICLE,			// for any entity inside a vehicle
	COLLISION_GROUP_WEAPON,				// for any weapons that need collision detection
	COLLISION_GROUP_VEHICLE_CLIP,		// vehicle clip brush to restrict vehicle movement
	COLLISION_GROUP_PROJECTILE,			// Projectiles!
	COLLISION_GROUP_DOOR_BLOCKER,		// Blocks entities not permitted to get near moving doors
	COLLISION_GROUP_PASSABLE_DOOR,		// Doors that the player shouldn't collide with
	COLLISION_GROUP_DISSOLVING,			// Things that are dissolving are in this group
	COLLISION_GROUP_PUSHAWAY,			// Nonsolid on client and server, pushaway in player code
	
	COLLISION_GROUP_NPC_ACTOR,			// Used so NPCs in scripts ignore the player.
	COLLISION_GROUP_NPC_SCRIPTED,		// USed for NPCs in scripts that should not collide with each other
	COLLISION_GROUP_PZ_CLIP,
	
	COLLISION_GROUP_DEBRIS_BLOCK_PROJECTILE, // Only collides with bullets
	LAST_SHARED_COLLISION_GROUP
};

#define SOUND_NORMAL_CLIP_DIST	1000.0f

//-----------------------------------------------------------------------------
// Base light indices to avoid index collision
//-----------------------------------------------------------------------------

enum
{
	LIGHT_INDEX_TE_DYNAMIC = 0x10000000,
	LIGHT_INDEX_PLAYER_BRIGHT = 0x20000000,
	LIGHT_INDEX_MUZZLEFLASH = 0x40000000,
	LIGHT_INDEX_LOW_PRIORITY = 0x64000000,
};

// How many networked area portals do we allow?
#define MAX_AREA_STATE_BYTES		32
#define MAX_AREA_PORTAL_STATE_BYTES 24

// user message max payload size (note, this value is used by the engine, so MODs cannot change it)
#define MAX_USER_MSG_BITS 12
#define MAX_USER_MSG_DATA ( ( 1 << ( MAX_USER_MSG_BITS - 3 ) ) - 1 )
#define MAX_ENTITY_MSG_DATA 255

enum ECstrike15UserMessages
{
	CS_UM_VGUIMenu = 1,
	CS_UM_Geiger = 2,
	CS_UM_Train = 3,
	CS_UM_HudText = 4,
	CS_UM_SayText = 5,
	CS_UM_SayText2 = 6,
	CS_UM_TextMsg = 7,
	CS_UM_HudMsg = 8,
	CS_UM_ResetHud = 9,
	CS_UM_GameTitle = 10,
	CS_UM_Shake = 12,
	CS_UM_Fade = 13,			// fade HUD in/out
	CS_UM_Rumble = 14,
	CS_UM_CloseCaption = 15,
	CS_UM_CloseCaptionDirect = 16,
	CS_UM_SendAudio = 17,
	CS_UM_RawAudio = 18,
	CS_UM_VoiceMask = 19,
	CS_UM_RequestState = 20,
	CS_UM_Damage = 21,
	CS_UM_RadioText = 22,
	CS_UM_HintText = 23,
	CS_UM_KeyHintText = 24,
	CS_UM_ProcessSpottedEntityUpdate = 25,
	CS_UM_ReloadEffect = 26,
	CS_UM_AdjustMoney = 27,
	CS_UM_UpdateTeamMoney = 28,
	CS_UM_StopSpectatorMode = 29,
	CS_UM_KillCam = 30,
	CS_UM_DesiredTimescale = 31,
	CS_UM_CurrentTimescale = 32,
	CS_UM_AchievementEvent = 33,
	CS_UM_MatchEndConditions = 34,
	CS_UM_DisconnectToLobby = 35,
	CS_UM_DisplayInventory = 37,
	CS_UM_WarmupHasEnded = 38,
	CS_UM_ClientInfo = 39,
	CS_UM_CallVoteFailed = 45,
	CS_UM_VoteStart = 46,
	CS_UM_VotePass = 47,
	CS_UM_VoteFailed = 48,
	CS_UM_VoteSetup = 49,
	CS_UM_SendLastKillerDamageToClient = 51,
	CS_UM_ItemPickup = 53,
	CS_UM_ShowMenu = 54,			// show hud menu
	CS_UM_BarTime = 55,			// For the C4 progress bar.
	CS_UM_AmmoDenied = 56,
	CS_UM_MarkAchievement = 57,
	CS_UM_ItemDrop = 59,
	CS_UM_GlowPropTurnOff = 60
};

//-----------------------------------------------------------------------------
// Flags passed in with view setup
//-----------------------------------------------------------------------------
enum ClearFlags_t
{
	VIEW_CLEAR_COLOR = 0x1,
	VIEW_CLEAR_DEPTH = 0x2,
	VIEW_CLEAR_FULL_TARGET = 0x4,
	VIEW_NO_DRAW = 0x8,
	VIEW_CLEAR_OBEY_STENCIL = 0x10, // Draws a quad allowing stencil test to clear through portals
	VIEW_CLEAR_STENCIL = 0x20,
};

enum StereoEye_t
{
	STEREO_EYE_MONO = 0,
	STEREO_EYE_LEFT = 1,
	STEREO_EYE_RIGHT = 2,
	STEREO_EYE_MAX = 3,
};

enum class Activity : int
{
	ACT_RESET,
	ACT_IDLE,
	ACT_TRANSITION,
	ACT_COVER,
	ACT_COVER_MED,
	ACT_COVER_LOW,
	ACT_WALK,
	ACT_WALK_AIM,
	ACT_WALK_CROUCH,
	ACT_WALK_CROUCH_AIM,
	ACT_RUN,
	ACT_RUN_AIM,
	ACT_RUN_CROUCH,
	ACT_RUN_CROUCH_AIM,
	ACT_RUN_PROTECTED,
	ACT_SCRIPT_CUSTOM_MOVE,
	ACT_RANGE_ATTACK1,
	ACT_RANGE_ATTACK2,
	ACT_RANGE_ATTACK1_LOW,
	ACT_RANGE_ATTACK2_LOW,
	ACT_DIESIMPLE,
	ACT_DIEBACKWARD,
	ACT_DIEFORWARD,
	ACT_DIEVIOLENT,
	ACT_DIERAGDOLL,
	ACT_FLY,
	ACT_HOVER,
	ACT_GLIDE,
	ACT_SWIM,
	ACT_JUMP,
	ACT_HOP,
	ACT_LEAP,
	ACT_LAND,
	ACT_CLIMB_UP,
	ACT_CLIMB_DOWN,
	ACT_CLIMB_DISMOUNT,
	ACT_SHIPLADDER_UP,
	ACT_SHIPLADDER_DOWN,
	ACT_STRAFE_LEFT,
	ACT_STRAFE_RIGHT,
	ACT_ROLL_LEFT,
	ACT_ROLL_RIGHT,
	ACT_TURN_LEFT,
	ACT_TURN_RIGHT,
	ACT_CROUCH,
	ACT_CROUCHIDLE,
	ACT_STAND,
	ACT_USE,
	ACT_ALIEN_BURROW_IDLE,
	ACT_ALIEN_BURROW_OUT,
	ACT_SIGNAL1,
	ACT_SIGNAL2,
	ACT_SIGNAL3,
	ACT_SIGNAL_ADVANCE,
	ACT_SIGNAL_FORWARD,
	ACT_SIGNAL_GROUP,
	ACT_SIGNAL_HALT,
	ACT_SIGNAL_LEFT,
	ACT_SIGNAL_RIGHT,
	ACT_SIGNAL_TAKECOVER,
	ACT_LOOKBACK_RIGHT,
	ACT_LOOKBACK_LEFT,
	ACT_COWER,
	ACT_SMALL_FLINCH,
	ACT_BIG_FLINCH,
	ACT_MELEE_ATTACK1,
	ACT_MELEE_ATTACK2,
	ACT_RELOAD,
	ACT_RELOAD_START,
	ACT_RELOAD_FINISH,
	ACT_RELOAD_LOW,
	ACT_ARM,
	ACT_DISARM,
	ACT_DROP_WEAPON,
	ACT_DROP_WEAPON_SHOTGUN,
	ACT_PICKUP_GROUND,
	ACT_PICKUP_RACK,
	ACT_IDLE_ANGRY,
	ACT_IDLE_RELAXED,
	ACT_IDLE_STIMULATED,
	ACT_IDLE_AGITATED,
	ACT_IDLE_STEALTH,
	ACT_IDLE_HURT,
	ACT_WALK_RELAXED,
	ACT_WALK_STIMULATED,
	ACT_WALK_AGITATED,
	ACT_WALK_STEALTH,
	ACT_RUN_RELAXED,
	ACT_RUN_STIMULATED,
	ACT_RUN_AGITATED,
	ACT_RUN_STEALTH,
	ACT_IDLE_AIM_RELAXED,
	ACT_IDLE_AIM_STIMULATED,
	ACT_IDLE_AIM_AGITATED,
	ACT_IDLE_AIM_STEALTH,
	ACT_WALK_AIM_RELAXED,
	ACT_WALK_AIM_STIMULATED,
	ACT_WALK_AIM_AGITATED,
	ACT_WALK_AIM_STEALTH,
	ACT_RUN_AIM_RELAXED,
	ACT_RUN_AIM_STIMULATED,
	ACT_RUN_AIM_AGITATED,
	ACT_RUN_AIM_STEALTH,
	ACT_CROUCHIDLE_STIMULATED,
	ACT_CROUCHIDLE_AIM_STIMULATED,
	ACT_CROUCHIDLE_AGITATED,
	ACT_WALK_HURT,
	ACT_RUN_HURT,
	ACT_SPECIAL_ATTACK1,
	ACT_SPECIAL_ATTACK2,
	ACT_COMBAT_IDLE,
	ACT_WALK_SCARED,
	ACT_RUN_SCARED,
	ACT_VICTORY_DANCE,
	ACT_DIE_HEADSHOT,
	ACT_DIE_CHESTSHOT,
	ACT_DIE_GUTSHOT,
	ACT_DIE_BACKSHOT,
	ACT_FLINCH_HEAD,
	ACT_FLINCH_CHEST,
	ACT_FLINCH_STOMACH,
	ACT_FLINCH_LEFTARM,
	ACT_FLINCH_RIGHTARM,
	ACT_FLINCH_LEFTLEG,
	ACT_FLINCH_RIGHTLEG,
	ACT_FLINCH_PHYSICS,
	ACT_FLINCH_HEAD_BACK,
	ACT_FLINCH_HEAD_LEFT,
	ACT_FLINCH_HEAD_RIGHT,
	ACT_FLINCH_CHEST_BACK,
	ACT_FLINCH_STOMACH_BACK,
	ACT_FLINCH_CROUCH_FRONT,
	ACT_FLINCH_CROUCH_BACK,
	ACT_FLINCH_CROUCH_LEFT,
	ACT_FLINCH_CROUCH_RIGHT,
	ACT_IDLE_ON_FIRE,
	ACT_WALK_ON_FIRE,
	ACT_RUN_ON_FIRE,
	ACT_RAPPEL_LOOP,
	ACT_180_LEFT,
	ACT_180_RIGHT,
	ACT_90_LEFT,
	ACT_90_RIGHT,
	ACT_STEP_LEFT,
	ACT_STEP_RIGHT,
	ACT_STEP_BACK,
	ACT_STEP_FORE,
	ACT_GESTURE_RANGE_ATTACK1,
	ACT_GESTURE_RANGE_ATTACK2,
	ACT_GESTURE_MELEE_ATTACK1,
	ACT_GESTURE_MELEE_ATTACK2,
	ACT_GESTURE_RANGE_ATTACK1_LOW,
	ACT_GESTURE_RANGE_ATTACK2_LOW,
	ACT_MELEE_ATTACK_SWING_GESTURE,
	ACT_GESTURE_SMALL_FLINCH,
	ACT_GESTURE_BIG_FLINCH,
	ACT_GESTURE_FLINCH_BLAST,
	ACT_GESTURE_FLINCH_BLAST_SHOTGUN,
	ACT_GESTURE_FLINCH_BLAST_DAMAGED,
	ACT_GESTURE_FLINCH_BLAST_DAMAGED_SHOTGUN,
	ACT_GESTURE_FLINCH_HEAD,
	ACT_GESTURE_FLINCH_CHEST,
	ACT_GESTURE_FLINCH_STOMACH,
	ACT_GESTURE_FLINCH_LEFTARM,
	ACT_GESTURE_FLINCH_RIGHTARM,
	ACT_GESTURE_FLINCH_LEFTLEG,
	ACT_GESTURE_FLINCH_RIGHTLEG,
	ACT_GESTURE_TURN_LEFT,
	ACT_GESTURE_TURN_RIGHT,
	ACT_GESTURE_TURN_LEFT45,
	ACT_GESTURE_TURN_RIGHT45,
	ACT_GESTURE_TURN_LEFT90,
	ACT_GESTURE_TURN_RIGHT90,
	ACT_GESTURE_TURN_LEFT45_FLAT,
	ACT_GESTURE_TURN_RIGHT45_FLAT,
	ACT_GESTURE_TURN_LEFT90_FLAT,
	ACT_GESTURE_TURN_RIGHT90_FLAT,
	ACT_BARNACLE_HIT,
	ACT_BARNACLE_PULL,
	ACT_BARNACLE_CHOMP,
	ACT_BARNACLE_CHEW,
	ACT_DO_NOT_DISTURB,
	ACT_SPECIFIC_SEQUENCE,
	ACT_VM_DRAW,
	ACT_VM_HOLSTER,
	ACT_VM_IDLE,
	ACT_VM_FIDGET,
	ACT_VM_PULLBACK,
	ACT_VM_PULLBACK_HIGH,
	ACT_VM_PULLBACK_LOW,
	ACT_VM_THROW,
	ACT_VM_PULLPIN,
	ACT_VM_PRIMARYATTACK,
	ACT_VM_SECONDARYATTACK,
	ACT_VM_RELOAD,
	ACT_VM_DRYFIRE,
	ACT_VM_HITLEFT,
	ACT_VM_HITLEFT2,
	ACT_VM_HITRIGHT,
	ACT_VM_HITRIGHT2,
	ACT_VM_HITCENTER,
	ACT_VM_HITCENTER2,
	ACT_VM_MISSLEFT,
	ACT_VM_MISSLEFT2,
	ACT_VM_MISSRIGHT,
	ACT_VM_MISSRIGHT2,
	ACT_VM_MISSCENTER,
	ACT_VM_MISSCENTER2,
	ACT_VM_HAULBACK,
	ACT_VM_SWINGHARD,
	ACT_VM_SWINGMISS,
	ACT_VM_SWINGHIT,
	ACT_VM_IDLE_TO_LOWERED,
	ACT_VM_IDLE_LOWERED,
	ACT_VM_LOWERED_TO_IDLE,
	ACT_VM_RECOIL1,
	ACT_VM_RECOIL2,
	ACT_VM_RECOIL3,
	ACT_VM_PICKUP,
	ACT_VM_RELEASE,
	ACT_VM_ATTACH_SILENCER,
	ACT_VM_DETACH_SILENCER,
	ACT_VM_EMPTY_FIRE,
	ACT_VM_EMPTY_RELOAD,
	ACT_VM_EMPTY_DRAW,
	ACT_VM_EMPTY_IDLE,
	ACT_SLAM_STICKWALL_IDLE,
	ACT_SLAM_STICKWALL_ND_IDLE,
	ACT_SLAM_STICKWALL_ATTACH,
	ACT_SLAM_STICKWALL_ATTACH2,
	ACT_SLAM_STICKWALL_ND_ATTACH,
	ACT_SLAM_STICKWALL_ND_ATTACH2,
	ACT_SLAM_STICKWALL_DETONATE,
	ACT_SLAM_STICKWALL_DETONATOR_HOLSTER,
	ACT_SLAM_STICKWALL_DRAW,
	ACT_SLAM_STICKWALL_ND_DRAW,
	ACT_SLAM_STICKWALL_TO_THROW,
	ACT_SLAM_STICKWALL_TO_THROW_ND,
	ACT_SLAM_STICKWALL_TO_TRIPMINE_ND,
	ACT_SLAM_THROW_IDLE,
	ACT_SLAM_THROW_ND_IDLE,
	ACT_SLAM_THROW_THROW,
	ACT_SLAM_THROW_THROW2,
	ACT_SLAM_THROW_THROW_ND,
	ACT_SLAM_THROW_THROW_ND2,
	ACT_SLAM_THROW_DRAW,
	ACT_SLAM_THROW_ND_DRAW,
	ACT_SLAM_THROW_TO_STICKWALL,
	ACT_SLAM_THROW_TO_STICKWALL_ND,
	ACT_SLAM_THROW_DETONATE,
	ACT_SLAM_THROW_DETONATOR_HOLSTER,
	ACT_SLAM_THROW_TO_TRIPMINE_ND,
	ACT_SLAM_TRIPMINE_IDLE,
	ACT_SLAM_TRIPMINE_DRAW,
	ACT_SLAM_TRIPMINE_ATTACH,
	ACT_SLAM_TRIPMINE_ATTACH2,
	ACT_SLAM_TRIPMINE_TO_STICKWALL_ND,
	ACT_SLAM_TRIPMINE_TO_THROW_ND,
	ACT_SLAM_DETONATOR_IDLE,
	ACT_SLAM_DETONATOR_DRAW,
	ACT_SLAM_DETONATOR_DETONATE,
	ACT_SLAM_DETONATOR_HOLSTER,
	ACT_SLAM_DETONATOR_STICKWALL_DRAW,
	ACT_SLAM_DETONATOR_THROW_DRAW,
	ACT_SHOTGUN_RELOAD_START,
	ACT_SHOTGUN_RELOAD_FINISH,
	ACT_SHOTGUN_PUMP,
	ACT_SMG2_IDLE2,
	ACT_SMG2_FIRE2,
	ACT_SMG2_DRAW2,
	ACT_SMG2_RELOAD2,
	ACT_SMG2_DRYFIRE2,
	ACT_SMG2_TOAUTO,
	ACT_SMG2_TOBURST,
	ACT_PHYSCANNON_UPGRADE,
	ACT_RANGE_ATTACK_AR1,
	ACT_RANGE_ATTACK_AR2,
	ACT_RANGE_ATTACK_AR2_LOW,
	ACT_RANGE_ATTACK_AR2_GRENADE,
	ACT_RANGE_ATTACK_HMG1,
	ACT_RANGE_ATTACK_ML,
	ACT_RANGE_ATTACK_SMG1,
	ACT_RANGE_ATTACK_SMG1_LOW,
	ACT_RANGE_ATTACK_SMG2,
	ACT_RANGE_ATTACK_SHOTGUN,
	ACT_RANGE_ATTACK_SHOTGUN_LOW,
	ACT_RANGE_ATTACK_PISTOL,
	ACT_RANGE_ATTACK_PISTOL_LOW,
	ACT_RANGE_ATTACK_SLAM,
	ACT_RANGE_ATTACK_TRIPWIRE,
	ACT_RANGE_ATTACK_THROW,
	ACT_RANGE_ATTACK_SNIPER_RIFLE,
	ACT_RANGE_ATTACK_RPG,
	ACT_MELEE_ATTACK_SWING,
	ACT_RANGE_AIM_LOW,
	ACT_RANGE_AIM_SMG1_LOW,
	ACT_RANGE_AIM_PISTOL_LOW,
	ACT_RANGE_AIM_AR2_LOW,
	ACT_COVER_PISTOL_LOW,
	ACT_COVER_SMG1_LOW,
	ACT_GESTURE_RANGE_ATTACK_AR1,
	ACT_GESTURE_RANGE_ATTACK_AR2,
	ACT_GESTURE_RANGE_ATTACK_AR2_GRENADE,
	ACT_GESTURE_RANGE_ATTACK_HMG1,
	ACT_GESTURE_RANGE_ATTACK_ML,
	ACT_GESTURE_RANGE_ATTACK_SMG1,
	ACT_GESTURE_RANGE_ATTACK_SMG1_LOW,
	ACT_GESTURE_RANGE_ATTACK_SMG2,
	ACT_GESTURE_RANGE_ATTACK_SHOTGUN,
	ACT_GESTURE_RANGE_ATTACK_PISTOL,
	ACT_GESTURE_RANGE_ATTACK_PISTOL_LOW,
	ACT_GESTURE_RANGE_ATTACK_SLAM,
	ACT_GESTURE_RANGE_ATTACK_TRIPWIRE,
	ACT_GESTURE_RANGE_ATTACK_THROW,
	ACT_GESTURE_RANGE_ATTACK_SNIPER_RIFLE,
	ACT_GESTURE_MELEE_ATTACK_SWING,
	ACT_IDLE_RIFLE,
	ACT_IDLE_SMG1,
	ACT_IDLE_ANGRY_SMG1,
	ACT_IDLE_PISTOL,
	ACT_IDLE_ANGRY_PISTOL,
	ACT_IDLE_ANGRY_SHOTGUN,
	ACT_IDLE_STEALTH_PISTOL,
	ACT_IDLE_PACKAGE,
	ACT_WALK_PACKAGE,
	ACT_IDLE_SUITCASE,
	ACT_WALK_SUITCASE,
	ACT_IDLE_SMG1_RELAXED,
	ACT_IDLE_SMG1_STIMULATED,
	ACT_WALK_RIFLE_RELAXED,
	ACT_RUN_RIFLE_RELAXED,
	ACT_WALK_RIFLE_STIMULATED,
	ACT_RUN_RIFLE_STIMULATED,
	ACT_IDLE_AIM_RIFLE_STIMULATED,
	ACT_WALK_AIM_RIFLE_STIMULATED,
	ACT_RUN_AIM_RIFLE_STIMULATED,
	ACT_IDLE_SHOTGUN_RELAXED,
	ACT_IDLE_SHOTGUN_STIMULATED,
	ACT_IDLE_SHOTGUN_AGITATED,
	ACT_WALK_ANGRY,
	ACT_POLICE_HARASS1,
	ACT_POLICE_HARASS2,
	ACT_IDLE_MANNEDGUN,
	ACT_IDLE_MELEE,
	ACT_IDLE_ANGRY_MELEE,
	ACT_IDLE_RPG_RELAXED,
	ACT_IDLE_RPG,
	ACT_IDLE_ANGRY_RPG,
	ACT_COVER_LOW_RPG,
	ACT_WALK_RPG,
	ACT_RUN_RPG,
	ACT_WALK_CROUCH_RPG,
	ACT_RUN_CROUCH_RPG,
	ACT_WALK_RPG_RELAXED,
	ACT_RUN_RPG_RELAXED,
	ACT_WALK_RIFLE,
	ACT_WALK_AIM_RIFLE,
	ACT_WALK_CROUCH_RIFLE,
	ACT_WALK_CROUCH_AIM_RIFLE,
	ACT_RUN_RIFLE,
	ACT_RUN_AIM_RIFLE,
	ACT_RUN_CROUCH_RIFLE,
	ACT_RUN_CROUCH_AIM_RIFLE,
	ACT_RUN_STEALTH_PISTOL,
	ACT_WALK_AIM_SHOTGUN,
	ACT_RUN_AIM_SHOTGUN,
	ACT_WALK_PISTOL,
	ACT_RUN_PISTOL,
	ACT_WALK_AIM_PISTOL,
	ACT_RUN_AIM_PISTOL,
	ACT_WALK_STEALTH_PISTOL,
	ACT_WALK_AIM_STEALTH_PISTOL,
	ACT_RUN_AIM_STEALTH_PISTOL,
	ACT_RELOAD_PISTOL,
	ACT_RELOAD_PISTOL_LOW,
	ACT_RELOAD_SMG1,
	ACT_RELOAD_SMG1_LOW,
	ACT_RELOAD_SHOTGUN,
	ACT_RELOAD_SHOTGUN_LOW,
	ACT_GESTURE_RELOAD,
	ACT_GESTURE_RELOAD_PISTOL,
	ACT_GESTURE_RELOAD_SMG1,
	ACT_GESTURE_RELOAD_SHOTGUN,
	ACT_BUSY_LEAN_LEFT,
	ACT_BUSY_LEAN_LEFT_ENTRY,
	ACT_BUSY_LEAN_LEFT_EXIT,
	ACT_BUSY_LEAN_BACK,
	ACT_BUSY_LEAN_BACK_ENTRY,
	ACT_BUSY_LEAN_BACK_EXIT,
	ACT_BUSY_SIT_GROUND,
	ACT_BUSY_SIT_GROUND_ENTRY,
	ACT_BUSY_SIT_GROUND_EXIT,
	ACT_BUSY_SIT_CHAIR,
	ACT_BUSY_SIT_CHAIR_ENTRY,
	ACT_BUSY_SIT_CHAIR_EXIT,
	ACT_BUSY_STAND,
	ACT_BUSY_QUEUE,
	ACT_DUCK_DODGE,
	ACT_DIE_BARNACLE_SWALLOW,
	ACT_GESTURE_BARNACLE_STRANGLE,
	ACT_PHYSCANNON_DETACH,
	ACT_PHYSCANNON_ANIMATE,
	ACT_PHYSCANNON_ANIMATE_PRE,
	ACT_PHYSCANNON_ANIMATE_POST,
	ACT_DIE_FRONTSIDE,
	ACT_DIE_RIGHTSIDE,
	ACT_DIE_BACKSIDE,
	ACT_DIE_LEFTSIDE,
	ACT_DIE_CROUCH_FRONTSIDE,
	ACT_DIE_CROUCH_RIGHTSIDE,
	ACT_DIE_CROUCH_BACKSIDE,
	ACT_DIE_CROUCH_LEFTSIDE,
	ACT_OPEN_DOOR,
	ACT_DI_ALYX_ZOMBIE_MELEE,
	ACT_DI_ALYX_ZOMBIE_TORSO_MELEE,
	ACT_DI_ALYX_HEADCRAB_MELEE,
	ACT_DI_ALYX_ANTLION,
	ACT_DI_ALYX_ZOMBIE_SHOTGUN64,
	ACT_DI_ALYX_ZOMBIE_SHOTGUN26,
	ACT_READINESS_RELAXED_TO_STIMULATED,
	ACT_READINESS_RELAXED_TO_STIMULATED_WALK,
	ACT_READINESS_AGITATED_TO_STIMULATED,
	ACT_READINESS_STIMULATED_TO_RELAXED,
	ACT_READINESS_PISTOL_RELAXED_TO_STIMULATED,
	ACT_READINESS_PISTOL_RELAXED_TO_STIMULATED_WALK,
	ACT_READINESS_PISTOL_AGITATED_TO_STIMULATED,
	ACT_READINESS_PISTOL_STIMULATED_TO_RELAXED,
	ACT_IDLE_CARRY,
	ACT_WALK_CARRY,
	ACT_STARTDYING,
	ACT_DYINGLOOP,
	ACT_DYINGTODEAD,
	ACT_RIDE_MANNED_GUN,
	ACT_VM_SPRINT_ENTER,
	ACT_VM_SPRINT_IDLE,
	ACT_VM_SPRINT_LEAVE,
	ACT_FIRE_START,
	ACT_FIRE_LOOP,
	ACT_FIRE_END,
	ACT_CROUCHING_GRENADEIDLE,
	ACT_CROUCHING_GRENADEREADY,
	ACT_CROUCHING_PRIMARYATTACK,
	ACT_OVERLAY_GRENADEIDLE,
	ACT_OVERLAY_GRENADEREADY,
	ACT_OVERLAY_PRIMARYATTACK,
	ACT_OVERLAY_SHIELD_UP,
	ACT_OVERLAY_SHIELD_DOWN,
	ACT_OVERLAY_SHIELD_UP_IDLE,
	ACT_OVERLAY_SHIELD_ATTACK,
	ACT_OVERLAY_SHIELD_KNOCKBACK,
	ACT_SHIELD_UP,
	ACT_SHIELD_DOWN,
	ACT_SHIELD_UP_IDLE,
	ACT_SHIELD_ATTACK,
	ACT_SHIELD_KNOCKBACK,
	ACT_CROUCHING_SHIELD_UP,
	ACT_CROUCHING_SHIELD_DOWN,
	ACT_CROUCHING_SHIELD_UP_IDLE,
	ACT_CROUCHING_SHIELD_ATTACK,
	ACT_CROUCHING_SHIELD_KNOCKBACK,
	ACT_TURNRIGHT45,
	ACT_TURNLEFT45,
	ACT_TURN,
	ACT_OBJ_ASSEMBLING,
	ACT_OBJ_DISMANTLING,
	ACT_OBJ_STARTUP,
	ACT_OBJ_RUNNING,
	ACT_OBJ_IDLE,
	ACT_OBJ_PLACING,
	ACT_OBJ_DETERIORATING,
	ACT_OBJ_UPGRADING,
	ACT_DEPLOY,
	ACT_DEPLOY_IDLE,
	ACT_UNDEPLOY,
	ACT_CROSSBOW_DRAW_UNLOADED,
	ACT_GAUSS_SPINUP,
	ACT_GAUSS_SPINCYCLE,
	ACT_VM_PRIMARYATTACK_SILENCED,
	ACT_VM_RELOAD_SILENCED,
	ACT_VM_DRYFIRE_SILENCED,
	ACT_VM_IDLE_SILENCED,
	ACT_VM_DRAW_SILENCED,
	ACT_VM_IDLE_EMPTY_LEFT,
	ACT_VM_DRYFIRE_LEFT,
	ACT_VM_IS_DRAW,
	ACT_VM_IS_HOLSTER,
	ACT_VM_IS_IDLE,
	ACT_VM_IS_PRIMARYATTACK,
	ACT_PLAYER_IDLE_FIRE,
	ACT_PLAYER_CROUCH_FIRE,
	ACT_PLAYER_CROUCH_WALK_FIRE,
	ACT_PLAYER_WALK_FIRE,
	ACT_PLAYER_RUN_FIRE,
	ACT_IDLETORUN,
	ACT_RUNTOIDLE,
	ACT_VM_DRAW_DEPLOYED,
	ACT_HL2MP_IDLE_MELEE,
	ACT_HL2MP_RUN_MELEE,
	ACT_HL2MP_IDLE_CROUCH_MELEE,
	ACT_HL2MP_WALK_CROUCH_MELEE,
	ACT_HL2MP_GESTURE_RANGE_ATTACK_MELEE,
	ACT_HL2MP_GESTURE_RELOAD_MELEE,
	ACT_HL2MP_JUMP_MELEE,
	ACT_VM_FIZZLE,
	ACT_MP_STAND_IDLE,
	ACT_MP_CROUCH_IDLE,
	ACT_MP_CROUCH_DEPLOYED_IDLE,
	ACT_MP_CROUCH_DEPLOYED,
	ACT_MP_DEPLOYED_IDLE,
	ACT_MP_RUN,
	ACT_MP_WALK,
	ACT_MP_AIRWALK,
	ACT_MP_CROUCHWALK,
	ACT_MP_SPRINT,
	ACT_MP_JUMP,
	ACT_MP_JUMP_START,
	ACT_MP_JUMP_FLOAT,
	ACT_MP_JUMP_LAND,
	ACT_MP_JUMP_IMPACT_N,
	ACT_MP_JUMP_IMPACT_E,
	ACT_MP_JUMP_IMPACT_W,
	ACT_MP_JUMP_IMPACT_S,
	ACT_MP_JUMP_IMPACT_TOP,
	ACT_MP_DOUBLEJUMP,
	ACT_MP_SWIM,
	ACT_MP_DEPLOYED,
	ACT_MP_SWIM_DEPLOYED,
	ACT_MP_VCD,
	ACT_MP_ATTACK_STAND_PRIMARYFIRE,
	ACT_MP_ATTACK_STAND_PRIMARYFIRE_DEPLOYED,
	ACT_MP_ATTACK_STAND_SECONDARYFIRE,
	ACT_MP_ATTACK_STAND_GRENADE,
	ACT_MP_ATTACK_CROUCH_PRIMARYFIRE,
	ACT_MP_ATTACK_CROUCH_PRIMARYFIRE_DEPLOYED,
	ACT_MP_ATTACK_CROUCH_SECONDARYFIRE,
	ACT_MP_ATTACK_CROUCH_GRENADE,
	ACT_MP_ATTACK_SWIM_PRIMARYFIRE,
	ACT_MP_ATTACK_SWIM_SECONDARYFIRE,
	ACT_MP_ATTACK_SWIM_GRENADE,
	ACT_MP_ATTACK_AIRWALK_PRIMARYFIRE,
	ACT_MP_ATTACK_AIRWALK_SECONDARYFIRE,
	ACT_MP_ATTACK_AIRWALK_GRENADE,
	ACT_MP_RELOAD_STAND,
	ACT_MP_RELOAD_STAND_LOOP,
	ACT_MP_RELOAD_STAND_END,
	ACT_MP_RELOAD_CROUCH,
	ACT_MP_RELOAD_CROUCH_LOOP,
	ACT_MP_RELOAD_CROUCH_END,
	ACT_MP_RELOAD_SWIM,
	ACT_MP_RELOAD_SWIM_LOOP,
	ACT_MP_RELOAD_SWIM_END,
	ACT_MP_RELOAD_AIRWALK,
	ACT_MP_RELOAD_AIRWALK_LOOP,
	ACT_MP_RELOAD_AIRWALK_END,
	ACT_MP_ATTACK_STAND_PREFIRE,
	ACT_MP_ATTACK_STAND_POSTFIRE,
	ACT_MP_ATTACK_STAND_STARTFIRE,
	ACT_MP_ATTACK_CROUCH_PREFIRE,
	ACT_MP_ATTACK_CROUCH_POSTFIRE,
	ACT_MP_ATTACK_SWIM_PREFIRE,
	ACT_MP_ATTACK_SWIM_POSTFIRE,
	ACT_MP_STAND_PRIMARY,
	ACT_MP_CROUCH_PRIMARY,
	ACT_MP_RUN_PRIMARY,
	ACT_MP_WALK_PRIMARY,
	ACT_MP_AIRWALK_PRIMARY,
	ACT_MP_CROUCHWALK_PRIMARY,
	ACT_MP_JUMP_PRIMARY,
	ACT_MP_JUMP_START_PRIMARY,
	ACT_MP_JUMP_FLOAT_PRIMARY,
	ACT_MP_JUMP_LAND_PRIMARY,
	ACT_MP_SWIM_PRIMARY,
	ACT_MP_DEPLOYED_PRIMARY,
	ACT_MP_SWIM_DEPLOYED_PRIMARY,
	ACT_MP_ATTACK_STAND_PRIMARY,
	ACT_MP_ATTACK_STAND_PRIMARY_DEPLOYED,
	ACT_MP_ATTACK_CROUCH_PRIMARY,
	ACT_MP_ATTACK_CROUCH_PRIMARY_DEPLOYED,
	ACT_MP_ATTACK_SWIM_PRIMARY,
	ACT_MP_ATTACK_AIRWALK_PRIMARY,
	ACT_MP_RELOAD_STAND_PRIMARY,
	ACT_MP_RELOAD_STAND_PRIMARY_LOOP,
	ACT_MP_RELOAD_STAND_PRIMARY_END,
	ACT_MP_RELOAD_CROUCH_PRIMARY,
	ACT_MP_RELOAD_CROUCH_PRIMARY_LOOP,
	ACT_MP_RELOAD_CROUCH_PRIMARY_END,
	ACT_MP_RELOAD_SWIM_PRIMARY,
	ACT_MP_RELOAD_SWIM_PRIMARY_LOOP,
	ACT_MP_RELOAD_SWIM_PRIMARY_END,
	ACT_MP_RELOAD_AIRWALK_PRIMARY,
	ACT_MP_RELOAD_AIRWALK_PRIMARY_LOOP,
	ACT_MP_RELOAD_AIRWALK_PRIMARY_END,
	ACT_MP_ATTACK_STAND_GRENADE_PRIMARY,
	ACT_MP_ATTACK_CROUCH_GRENADE_PRIMARY,
	ACT_MP_ATTACK_SWIM_GRENADE_PRIMARY,
	ACT_MP_ATTACK_AIRWALK_GRENADE_PRIMARY,
	ACT_MP_STAND_SECONDARY,
	ACT_MP_CROUCH_SECONDARY,
	ACT_MP_RUN_SECONDARY,
	ACT_MP_WALK_SECONDARY,
	ACT_MP_AIRWALK_SECONDARY,
	ACT_MP_CROUCHWALK_SECONDARY,
	ACT_MP_JUMP_SECONDARY,
	ACT_MP_JUMP_START_SECONDARY,
	ACT_MP_JUMP_FLOAT_SECONDARY,
	ACT_MP_JUMP_LAND_SECONDARY,
	ACT_MP_SWIM_SECONDARY,
	ACT_MP_ATTACK_STAND_SECONDARY,
	ACT_MP_ATTACK_CROUCH_SECONDARY,
	ACT_MP_ATTACK_SWIM_SECONDARY,
	ACT_MP_ATTACK_AIRWALK_SECONDARY,
	ACT_MP_RELOAD_STAND_SECONDARY,
	ACT_MP_RELOAD_STAND_SECONDARY_LOOP,
	ACT_MP_RELOAD_STAND_SECONDARY_END,
	ACT_MP_RELOAD_CROUCH_SECONDARY,
	ACT_MP_RELOAD_CROUCH_SECONDARY_LOOP,
	ACT_MP_RELOAD_CROUCH_SECONDARY_END,
	ACT_MP_RELOAD_SWIM_SECONDARY,
	ACT_MP_RELOAD_SWIM_SECONDARY_LOOP,
	ACT_MP_RELOAD_SWIM_SECONDARY_END,
	ACT_MP_RELOAD_AIRWALK_SECONDARY,
	ACT_MP_RELOAD_AIRWALK_SECONDARY_LOOP,
	ACT_MP_RELOAD_AIRWALK_SECONDARY_END,
	ACT_MP_ATTACK_STAND_GRENADE_SECONDARY,
	ACT_MP_ATTACK_CROUCH_GRENADE_SECONDARY,
	ACT_MP_ATTACK_SWIM_GRENADE_SECONDARY,
	ACT_MP_ATTACK_AIRWALK_GRENADE_SECONDARY,
	ACT_MP_STAND_MELEE,
	ACT_MP_CROUCH_MELEE,
	ACT_MP_RUN_MELEE,
	ACT_MP_WALK_MELEE,
	ACT_MP_AIRWALK_MELEE,
	ACT_MP_CROUCHWALK_MELEE,
	ACT_MP_JUMP_MELEE,
	ACT_MP_JUMP_START_MELEE,
	ACT_MP_JUMP_FLOAT_MELEE,
	ACT_MP_JUMP_LAND_MELEE,
	ACT_MP_SWIM_MELEE,
	ACT_MP_ATTACK_STAND_MELEE,
	ACT_MP_ATTACK_STAND_MELEE_SECONDARY,
	ACT_MP_ATTACK_CROUCH_MELEE,
	ACT_MP_ATTACK_CROUCH_MELEE_SECONDARY,
	ACT_MP_ATTACK_SWIM_MELEE,
	ACT_MP_ATTACK_AIRWALK_MELEE,
	ACT_MP_ATTACK_STAND_GRENADE_MELEE,
	ACT_MP_ATTACK_CROUCH_GRENADE_MELEE,
	ACT_MP_ATTACK_SWIM_GRENADE_MELEE,
	ACT_MP_ATTACK_AIRWALK_GRENADE_MELEE,
	ACT_MP_STAND_ITEM1,
	ACT_MP_CROUCH_ITEM1,
	ACT_MP_RUN_ITEM1,
	ACT_MP_WALK_ITEM1,
	ACT_MP_AIRWALK_ITEM1,
	ACT_MP_CROUCHWALK_ITEM1,
	ACT_MP_JUMP_ITEM1,
	ACT_MP_JUMP_START_ITEM1,
	ACT_MP_JUMP_FLOAT_ITEM1,
	ACT_MP_JUMP_LAND_ITEM1,
	ACT_MP_SWIM_ITEM1,
	ACT_MP_ATTACK_STAND_ITEM1,
	ACT_MP_ATTACK_STAND_ITEM1_SECONDARY,
	ACT_MP_ATTACK_CROUCH_ITEM1,
	ACT_MP_ATTACK_CROUCH_ITEM1_SECONDARY,
	ACT_MP_ATTACK_SWIM_ITEM1,
	ACT_MP_ATTACK_AIRWALK_ITEM1,
	ACT_MP_STAND_ITEM2,
	ACT_MP_CROUCH_ITEM2,
	ACT_MP_RUN_ITEM2,
	ACT_MP_WALK_ITEM2,
	ACT_MP_AIRWALK_ITEM2,
	ACT_MP_CROUCHWALK_ITEM2,
	ACT_MP_JUMP_ITEM2,
	ACT_MP_JUMP_START_ITEM2,
	ACT_MP_JUMP_FLOAT_ITEM2,
	ACT_MP_JUMP_LAND_ITEM2,
	ACT_MP_SWIM_ITEM2,
	ACT_MP_ATTACK_STAND_ITEM2,
	ACT_MP_ATTACK_STAND_ITEM2_SECONDARY,
	ACT_MP_ATTACK_CROUCH_ITEM2,
	ACT_MP_ATTACK_CROUCH_ITEM2_SECONDARY,
	ACT_MP_ATTACK_SWIM_ITEM2,
	ACT_MP_ATTACK_AIRWALK_ITEM2,
	ACT_MP_GESTURE_FLINCH,
	ACT_MP_GESTURE_FLINCH_PRIMARY,
	ACT_MP_GESTURE_FLINCH_SECONDARY,
	ACT_MP_GESTURE_FLINCH_MELEE,
	ACT_MP_GESTURE_FLINCH_ITEM1,
	ACT_MP_GESTURE_FLINCH_ITEM2,
	ACT_MP_GESTURE_FLINCH_HEAD,
	ACT_MP_GESTURE_FLINCH_CHEST,
	ACT_MP_GESTURE_FLINCH_STOMACH,
	ACT_MP_GESTURE_FLINCH_LEFTARM,
	ACT_MP_GESTURE_FLINCH_RIGHTARM,
	ACT_MP_GESTURE_FLINCH_LEFTLEG,
	ACT_MP_GESTURE_FLINCH_RIGHTLEG,
	ACT_MP_GRENADE1_DRAW,
	ACT_MP_GRENADE1_IDLE,
	ACT_MP_GRENADE1_ATTACK,
	ACT_MP_GRENADE2_DRAW,
	ACT_MP_GRENADE2_IDLE,
	ACT_MP_GRENADE2_ATTACK,
	ACT_MP_PRIMARY_GRENADE1_DRAW,
	ACT_MP_PRIMARY_GRENADE1_IDLE,
	ACT_MP_PRIMARY_GRENADE1_ATTACK,
	ACT_MP_PRIMARY_GRENADE2_DRAW,
	ACT_MP_PRIMARY_GRENADE2_IDLE,
	ACT_MP_PRIMARY_GRENADE2_ATTACK,
	ACT_MP_SECONDARY_GRENADE1_DRAW,
	ACT_MP_SECONDARY_GRENADE1_IDLE,
	ACT_MP_SECONDARY_GRENADE1_ATTACK,
	ACT_MP_SECONDARY_GRENADE2_DRAW,
	ACT_MP_SECONDARY_GRENADE2_IDLE,
	ACT_MP_SECONDARY_GRENADE2_ATTACK,
	ACT_MP_MELEE_GRENADE1_DRAW,
	ACT_MP_MELEE_GRENADE1_IDLE,
	ACT_MP_MELEE_GRENADE1_ATTACK,
	ACT_MP_MELEE_GRENADE2_DRAW,
	ACT_MP_MELEE_GRENADE2_IDLE,
	ACT_MP_MELEE_GRENADE2_ATTACK,
	ACT_MP_ITEM1_GRENADE1_DRAW,
	ACT_MP_ITEM1_GRENADE1_IDLE,
	ACT_MP_ITEM1_GRENADE1_ATTACK,
	ACT_MP_ITEM1_GRENADE2_DRAW,
	ACT_MP_ITEM1_GRENADE2_IDLE,
	ACT_MP_ITEM1_GRENADE2_ATTACK,
	ACT_MP_ITEM2_GRENADE1_DRAW,
	ACT_MP_ITEM2_GRENADE1_IDLE,
	ACT_MP_ITEM2_GRENADE1_ATTACK,
	ACT_MP_ITEM2_GRENADE2_DRAW,
	ACT_MP_ITEM2_GRENADE2_IDLE,
	ACT_MP_ITEM2_GRENADE2_ATTACK,
	ACT_MP_STAND_BUILDING,
	ACT_MP_CROUCH_BUILDING,
	ACT_MP_RUN_BUILDING,
	ACT_MP_WALK_BUILDING,
	ACT_MP_AIRWALK_BUILDING,
	ACT_MP_CROUCHWALK_BUILDING,
	ACT_MP_JUMP_BUILDING,
	ACT_MP_JUMP_START_BUILDING,
	ACT_MP_JUMP_FLOAT_BUILDING,
	ACT_MP_JUMP_LAND_BUILDING,
	ACT_MP_SWIM_BUILDING,
	ACT_MP_ATTACK_STAND_BUILDING,
	ACT_MP_ATTACK_CROUCH_BUILDING,
	ACT_MP_ATTACK_SWIM_BUILDING,
	ACT_MP_ATTACK_AIRWALK_BUILDING,
	ACT_MP_ATTACK_STAND_GRENADE_BUILDING,
	ACT_MP_ATTACK_CROUCH_GRENADE_BUILDING,
	ACT_MP_ATTACK_SWIM_GRENADE_BUILDING,
	ACT_MP_ATTACK_AIRWALK_GRENADE_BUILDING,
	ACT_MP_STAND_PDA,
	ACT_MP_CROUCH_PDA,
	ACT_MP_RUN_PDA,
	ACT_MP_WALK_PDA,
	ACT_MP_AIRWALK_PDA,
	ACT_MP_CROUCHWALK_PDA,
	ACT_MP_JUMP_PDA,
	ACT_MP_JUMP_START_PDA,
	ACT_MP_JUMP_FLOAT_PDA,
	ACT_MP_JUMP_LAND_PDA,
	ACT_MP_SWIM_PDA,
	ACT_MP_ATTACK_STAND_PDA,
	ACT_MP_ATTACK_SWIM_PDA,
	ACT_MP_GESTURE_VC_HANDMOUTH,
	ACT_MP_GESTURE_VC_FINGERPOINT,
	ACT_MP_GESTURE_VC_FISTPUMP,
	ACT_MP_GESTURE_VC_THUMBSUP,
	ACT_MP_GESTURE_VC_NODYES,
	ACT_MP_GESTURE_VC_NODNO,
	ACT_MP_GESTURE_VC_HANDMOUTH_PRIMARY,
	ACT_MP_GESTURE_VC_FINGERPOINT_PRIMARY,
	ACT_MP_GESTURE_VC_FISTPUMP_PRIMARY,
	ACT_MP_GESTURE_VC_THUMBSUP_PRIMARY,
	ACT_MP_GESTURE_VC_NODYES_PRIMARY,
	ACT_MP_GESTURE_VC_NODNO_PRIMARY,
	ACT_MP_GESTURE_VC_HANDMOUTH_SECONDARY,
	ACT_MP_GESTURE_VC_FINGERPOINT_SECONDARY,
	ACT_MP_GESTURE_VC_FISTPUMP_SECONDARY,
	ACT_MP_GESTURE_VC_THUMBSUP_SECONDARY,
	ACT_MP_GESTURE_VC_NODYES_SECONDARY,
	ACT_MP_GESTURE_VC_NODNO_SECONDARY,
	ACT_MP_GESTURE_VC_HANDMOUTH_MELEE,
	ACT_MP_GESTURE_VC_FINGERPOINT_MELEE,
	ACT_MP_GESTURE_VC_FISTPUMP_MELEE,
	ACT_MP_GESTURE_VC_THUMBSUP_MELEE,
	ACT_MP_GESTURE_VC_NODYES_MELEE,
	ACT_MP_GESTURE_VC_NODNO_MELEE,
	ACT_MP_GESTURE_VC_HANDMOUTH_ITEM1,
	ACT_MP_GESTURE_VC_FINGERPOINT_ITEM1,
	ACT_MP_GESTURE_VC_FISTPUMP_ITEM1,
	ACT_MP_GESTURE_VC_THUMBSUP_ITEM1,
	ACT_MP_GESTURE_VC_NODYES_ITEM1,
	ACT_MP_GESTURE_VC_NODNO_ITEM1,
	ACT_MP_GESTURE_VC_HANDMOUTH_ITEM2,
	ACT_MP_GESTURE_VC_FINGERPOINT_ITEM2,
	ACT_MP_GESTURE_VC_FISTPUMP_ITEM2,
	ACT_MP_GESTURE_VC_THUMBSUP_ITEM2,
	ACT_MP_GESTURE_VC_NODYES_ITEM2,
	ACT_MP_GESTURE_VC_NODNO_ITEM2,
	ACT_MP_GESTURE_VC_HANDMOUTH_BUILDING,
	ACT_MP_GESTURE_VC_FINGERPOINT_BUILDING,
	ACT_MP_GESTURE_VC_FISTPUMP_BUILDING,
	ACT_MP_GESTURE_VC_THUMBSUP_BUILDING,
	ACT_MP_GESTURE_VC_NODYES_BUILDING,
	ACT_MP_GESTURE_VC_NODNO_BUILDING,
	ACT_MP_GESTURE_VC_HANDMOUTH_PDA,
	ACT_MP_GESTURE_VC_FINGERPOINT_PDA,
	ACT_MP_GESTURE_VC_FISTPUMP_PDA,
	ACT_MP_GESTURE_VC_THUMBSUP_PDA,
	ACT_MP_GESTURE_VC_NODYES_PDA,
	ACT_MP_GESTURE_VC_NODNO_PDA,
	ACT_VM_UNUSABLE,
	ACT_VM_UNUSABLE_TO_USABLE,
	ACT_VM_USABLE_TO_UNUSABLE,
	ACT_PRIMARY_VM_DRAW,
	ACT_PRIMARY_VM_HOLSTER,
	ACT_PRIMARY_VM_IDLE,
	ACT_PRIMARY_VM_PULLBACK,
	ACT_PRIMARY_VM_PRIMARYATTACK,
	ACT_PRIMARY_VM_SECONDARYATTACK,
	ACT_PRIMARY_VM_RELOAD,
	ACT_PRIMARY_VM_DRYFIRE,
	ACT_PRIMARY_VM_IDLE_TO_LOWERED,
	ACT_PRIMARY_VM_IDLE_LOWERED,
	ACT_PRIMARY_VM_LOWERED_TO_IDLE,
	ACT_SECONDARY_VM_DRAW,
	ACT_SECONDARY_VM_HOLSTER,
	ACT_SECONDARY_VM_IDLE,
	ACT_SECONDARY_VM_PULLBACK,
	ACT_SECONDARY_VM_PRIMARYATTACK,
	ACT_SECONDARY_VM_SECONDARYATTACK,
	ACT_SECONDARY_VM_RELOAD,
	ACT_SECONDARY_VM_DRYFIRE,
	ACT_SECONDARY_VM_IDLE_TO_LOWERED,
	ACT_SECONDARY_VM_IDLE_LOWERED,
	ACT_SECONDARY_VM_LOWERED_TO_IDLE,
	ACT_MELEE_VM_DRAW,
	ACT_MELEE_VM_HOLSTER,
	ACT_MELEE_VM_IDLE,
	ACT_MELEE_VM_PULLBACK,
	ACT_MELEE_VM_PRIMARYATTACK,
	ACT_MELEE_VM_SECONDARYATTACK,
	ACT_MELEE_VM_RELOAD,
	ACT_MELEE_VM_DRYFIRE,
	ACT_MELEE_VM_IDLE_TO_LOWERED,
	ACT_MELEE_VM_IDLE_LOWERED,
	ACT_MELEE_VM_LOWERED_TO_IDLE,
	ACT_PDA_VM_DRAW,
	ACT_PDA_VM_HOLSTER,
	ACT_PDA_VM_IDLE,
	ACT_PDA_VM_PULLBACK,
	ACT_PDA_VM_PRIMARYATTACK,
	ACT_PDA_VM_SECONDARYATTACK,
	ACT_PDA_VM_RELOAD,
	ACT_PDA_VM_DRYFIRE,
	ACT_PDA_VM_IDLE_TO_LOWERED,
	ACT_PDA_VM_IDLE_LOWERED,
	ACT_PDA_VM_LOWERED_TO_IDLE,
	ACT_ITEM1_VM_DRAW,
	ACT_ITEM1_VM_HOLSTER,
	ACT_ITEM1_VM_IDLE,
	ACT_ITEM1_VM_PULLBACK,
	ACT_ITEM1_VM_PRIMARYATTACK,
	ACT_ITEM1_VM_SECONDARYATTACK,
	ACT_ITEM1_VM_RELOAD,
	ACT_ITEM1_VM_DRYFIRE,
	ACT_ITEM1_VM_IDLE_TO_LOWERED,
	ACT_ITEM1_VM_IDLE_LOWERED,
	ACT_ITEM1_VM_LOWERED_TO_IDLE,
	ACT_ITEM2_VM_DRAW,
	ACT_ITEM2_VM_HOLSTER,
	ACT_ITEM2_VM_IDLE,
	ACT_ITEM2_VM_PULLBACK,
	ACT_ITEM2_VM_PRIMARYATTACK,
	ACT_ITEM2_VM_SECONDARYATTACK,
	ACT_ITEM2_VM_RELOAD,
	ACT_ITEM2_VM_DRYFIRE,
	ACT_ITEM2_VM_IDLE_TO_LOWERED,
	ACT_ITEM2_VM_IDLE_LOWERED,
	ACT_ITEM2_VM_LOWERED_TO_IDLE,
	ACT_RELOAD_SUCCEED,
	ACT_RELOAD_FAIL,
	ACT_WALK_AIM_AUTOGUN,
	ACT_RUN_AIM_AUTOGUN,
	ACT_IDLE_AUTOGUN,
	ACT_IDLE_AIM_AUTOGUN,
	ACT_RELOAD_AUTOGUN,
	ACT_CROUCH_IDLE_AUTOGUN,
	ACT_RANGE_ATTACK_AUTOGUN,
	ACT_JUMP_AUTOGUN,
	ACT_IDLE_AIM_PISTOL,
	ACT_WALK_AIM_DUAL,
	ACT_RUN_AIM_DUAL,
	ACT_IDLE_DUAL,
	ACT_IDLE_AIM_DUAL,
	ACT_RELOAD_DUAL,
	ACT_CROUCH_IDLE_DUAL,
	ACT_RANGE_ATTACK_DUAL,
	ACT_JUMP_DUAL,
	ACT_IDLE_SHOTGUN,
	ACT_IDLE_AIM_SHOTGUN,
	ACT_CROUCH_IDLE_SHOTGUN,
	ACT_JUMP_SHOTGUN,
	ACT_IDLE_AIM_RIFLE,
	ACT_RELOAD_RIFLE,
	ACT_CROUCH_IDLE_RIFLE,
	ACT_RANGE_ATTACK_RIFLE,
	ACT_JUMP_RIFLE,
	ACT_SLEEP,
	ACT_WAKE,
	ACT_FLICK_LEFT,
	ACT_FLICK_LEFT_MIDDLE,
	ACT_FLICK_RIGHT_MIDDLE,
	ACT_FLICK_RIGHT,
	ACT_SPINAROUND,
	ACT_PREP_TO_FIRE,
	ACT_FIRE,
	ACT_FIRE_RECOVER,
	ACT_SPRAY,
	ACT_PREP_EXPLODE,
	ACT_EXPLODE,
	ACT_DOTA_IDLE,
	ACT_DOTA_RUN,
	ACT_DOTA_ATTACK,
	ACT_DOTA_ATTACK_EVENT,
	ACT_DOTA_DIE,
	ACT_DOTA_FLINCH,
	ACT_DOTA_DISABLED,
	ACT_DOTA_CAST_ABILITY_1,
	ACT_DOTA_CAST_ABILITY_2,
	ACT_DOTA_CAST_ABILITY_3,
	ACT_DOTA_CAST_ABILITY_4,
	ACT_DOTA_OVERRIDE_ABILITY_1,
	ACT_DOTA_OVERRIDE_ABILITY_2,
	ACT_DOTA_OVERRIDE_ABILITY_3,
	ACT_DOTA_OVERRIDE_ABILITY_4,
	ACT_DOTA_CHANNEL_ABILITY_1,
	ACT_DOTA_CHANNEL_ABILITY_2,
	ACT_DOTA_CHANNEL_ABILITY_3,
	ACT_DOTA_CHANNEL_ABILITY_4,
	ACT_DOTA_CHANNEL_END_ABILITY_1,
	ACT_DOTA_CHANNEL_END_ABILITY_2,
	ACT_DOTA_CHANNEL_END_ABILITY_3,
	ACT_DOTA_CHANNEL_END_ABILITY_4,
	ACT_MP_RUN_SPEEDPAINT,
	ACT_MP_LONG_FALL,
	ACT_MP_TRACTORBEAM_FLOAT,
	ACT_MP_DEATH_CRUSH,
	ACT_MP_RUN_SPEEDPAINT_PRIMARY,
	ACT_MP_DROWNING_PRIMARY,
	ACT_MP_LONG_FALL_PRIMARY,
	ACT_MP_TRACTORBEAM_FLOAT_PRIMARY,
	ACT_MP_DEATH_CRUSH_PRIMARY,
	ACT_DIE_STAND,
	ACT_DIE_STAND_HEADSHOT,
	ACT_DIE_CROUCH,
	ACT_DIE_CROUCH_HEADSHOT,
	ACT_CSGO_NULL,
	ACT_CSGO_DEFUSE,
	ACT_CSGO_DEFUSE_WITH_KIT,
	ACT_CSGO_FLASHBANG_REACTION,
	ACT_CSGO_FIRE_PRIMARY,
	ACT_CSGO_FIRE_PRIMARY_OPT_1,
	ACT_CSGO_FIRE_PRIMARY_OPT_2,
	ACT_CSGO_FIRE_SECONDARY,
	ACT_CSGO_FIRE_SECONDARY_OPT_1,
	ACT_CSGO_FIRE_SECONDARY_OPT_2,
	ACT_CSGO_RELOAD,
	ACT_CSGO_RELOAD_START,
	ACT_CSGO_RELOAD_LOOP,
	ACT_CSGO_RELOAD_END,
	ACT_CSGO_OPERATE,
	ACT_CSGO_DEPLOY,
	ACT_CSGO_CATCH,
	ACT_CSGO_SILENCER_DETACH,
	ACT_CSGO_SILENCER_ATTACH,
	ACT_CSGO_TWITCH,
	ACT_CSGO_TWITCH_BUYZONE,
	ACT_CSGO_PLANT_BOMB,
	ACT_CSGO_IDLE_TURN_BALANCEADJUST, // ACTUALLY THE INFAMOUS ACT 979
	ACT_CSGO_IDLE_ADJUST_STOPPEDMOVING,
	ACT_CSGO_ALIVE_LOOP,
	ACT_CSGO_FLINCH,
	ACT_CSGO_FLINCH_HEAD,
	ACT_CSGO_FLINCH_MOLOTOV,
	ACT_CSGO_JUMP,
	ACT_CSGO_FALL,
	ACT_CSGO_CLIMB_LADDER,
	ACT_CSGO_LAND_LIGHT,
	ACT_CSGO_LAND_HEAVY,
	ACT_CSGO_EXIT_LADDER_TOP,
	ACT_CSGO_EXIT_LADDER_BOTTOM,
};

SOURCE_END