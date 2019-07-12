/*
	____   ____.__  __                     .__        
	\   \ /   /|__|/  |________ __ _____  _|__|____   
	 \   Y   / |  \   __\_  __ \  |  \  \/ /  \__  \  
	  \     /  |  ||  |  |  | \/  |  /\   /|  |/ __ \_
	   \___/   |__||__|  |__|  |____/  \_/ |__(____  /
												   \/ 
	Premium Internal Multihack for Counter-Strike: Global Offensive
	Made with love by double v - 2017
*/

#pragma once
#include "../Vitruvia.h"
#include "../Globals/VitruviaGlobals.h"
#include "../Skins/KitParser/KitParser.h"
#include "../Skins/ItemDefinitions/ItemDefinitions.h"

// For JSON-based configs
#include "json.hpp"
using json = nlohmann::json;

namespace Vitruvia {
namespace Config {

const int default_int = 0;
const float default_float = 0.f;
const bool default_bool = false;
const std::string default_string = "";
const Source::Color default_color = Source::Color(255, 255, 255, 255);

// This is an abstract class to define a config
class ConfigVar_Int
{
public:
	ConfigVar_Int()
		: m_strName(enc("null")),
		m_Value(default_int)
	{
		Warn(enc("Unexpected empty config var built. DEBUG THIS"));
	}
	ConfigVar_Int(const std::string& var_name)
		: m_strName(var_name)
		, m_Value(default_int)
	{}
	std::string m_strName; // Name of the variable
	int m_Value; // The value stored

	operator int() { return m_Value; }
	ConfigVar_Int& operator=(int value) { m_Value = value; return *this; }
};
#define var_int ConfigVar_Int
class ConfigVar_Float
{
public:
	ConfigVar_Float()
		: m_strName(enc("null")),
		m_Value(default_float)
	{
		Warn(enc("Unexpected empty config var built. DEBUG THIS"));
	}
	ConfigVar_Float(const std::string& var_name)
		: m_strName(var_name)
		, m_Value(default_float)
	{}
	std::string m_strName; // Name of the variable
	float m_Value; // The value stored

	operator float() { return m_Value; }
	ConfigVar_Float& operator=(float value) { m_Value = value; return *this; }
};
#define var_float ConfigVar_Float
class ConfigVar_Bool
{
public:
	ConfigVar_Bool()
		: m_strName(enc("null")),
		m_Value(default_bool)
	{
		Warn(enc("Unexpected empty config var built. DEBUG THIS"));
	}
	ConfigVar_Bool(const std::string& var_name)
		: m_strName(var_name)
		, m_Value(default_bool)
	{}
	std::string m_strName; // Name of the variable
	bool m_Value; // The value stored

	operator bool() { return m_Value; }
	ConfigVar_Bool& operator=(bool value) { m_Value = value; return *this; }
};
#define var_bool ConfigVar_Bool
class ConfigVar_String
{
public:
	ConfigVar_String()
		: m_strName(enc("null")),
		m_Value(default_string)
	{
		Warn(enc("Unexpected empty config var built. DEBUG THIS"));
	}
	ConfigVar_String(const std::string& var_name)
		: m_strName(var_name)
		, m_Value(default_string)
	{}
	std::string m_strName; // Name of the variable
	std::string m_Value; // The value stored

	operator std::string() { return m_Value; }
	ConfigVar_String& operator=(std::string value) { m_Value = value; return *this; }
};
#define var_string ConfigVar_String
class ConfigVar_Color
{
public:
	ConfigVar_Color()
		: m_strName(enc("null")),
		m_Value(default_color)
	{
		Warn(enc("Unexpected empty config var built. DEBUG THIS"));
	}
	ConfigVar_Color(const std::string& var_name)
		: m_strName(var_name)
		, m_Value(default_color)
	{}
	std::string m_strName; // Name of the variable
	Source::Color m_Value; // The value stored

	operator Source::Color() { return m_Value; }
	ConfigVar_Color operator=(Source::Color value) { m_Value = value; return *this; }
};
#define var_color ConfigVar_Color

// All the config variables are stored inside structs divided by type of vars

//////////////////////////////////////////////////////////////////////////////////////

struct cLegit
{
	std::string NAME = enc("Legit");
	// General configs, will apply to all the weapongroups
	struct cAimbot
	{
		std::string NAME = enc("Aimbot");
		var_bool Active = var_bool(enc("Active"));
		var_int Mode = var_int(enc("Mode"));
		var_int SortBy = var_int(enc("Sort by"));
		var_int AimKey = var_int(enc("On Key"));
		var_bool TeamMates = var_bool(enc("Friendly Fire"));
		var_bool VisibleCheck = var_bool(enc("Visible Check"));
		var_int Hitbox = var_int(enc("Hitbox"));
		var_float FOV = var_float(enc("FOV"));
		var_float Smooth = var_float(enc("Smooth"));
		var_float Time = var_float(enc("Time"));
		var_bool SmokeCheck = var_bool(enc("Smoke Check"));
		var_bool AwpBody = var_bool(enc("AWP Body"));
		var_bool SmartAim = var_bool(enc("Smart Aim"));
		var_float RCSFactor = var_float(enc("RCS Factor"));
		var_bool Silent = var_bool(enc("Silent"));
		var_bool UseGroups = var_bool(enc("Weapon Groups"));
		var_bool Backtrack = var_bool(enc("Backtracking"));
		var_bool AdaptiveBacktrackFOV = var_bool(enc("Adaptive Backtracking FOV"));
	};
	struct cRCS
	{
		std::string NAME = enc("RCS");
		var_bool Active = var_bool(enc("Standalone RCS"));
		var_int StartBullet = var_int(enc("Start Bullet"));
		var_int StopBullet = var_int(enc("Stop Bullet"));
		var_bool Smooth = var_bool(enc("RCS Smooth"));
	};
	struct cTrigger
	{
		std::string NAME = enc("Triggerbot");
		var_bool Active = var_bool(enc("Active"));
		var_int Delay = var_int(enc("Delay"));
		var_bool UseKey = var_bool(enc("On Key"));
		var_int Key = var_int(enc("Key"));
		var_int Hitchance = var_int(enc("Hitchance"));
		var_int Hitgroup = var_int(enc("Hitgroup Filter"));
		var_bool SmokeCheck = var_bool(enc("Smoke Check"));
		var_bool ThroughWalls = var_bool(enc("Through Walls"));
		var_float MinWallDamage = var_float(enc("Min Damage"));
	};
	// Config specific for one single weapongroup
	struct cWeaponGroup
	{
		cWeaponGroup(const std::string& name) { NAME = name; }
		std::string NAME;
		var_bool Active = var_bool(enc("Active"));
		var_int Hitbox = var_int(enc("Hitbox"));
		var_float FOV = var_float(enc("FOV"));
		var_float Smooth = var_float(enc("Smooth"));
		var_float Time = var_float(enc("Time"));
	};
};

extern cLegit::cAimbot Aimbot;
extern cLegit::cRCS RCS;
extern cLegit::cTrigger Trigger;
extern std::unordered_map<Source::WeaponTypes, cLegit::cWeaponGroup*> WeaponGroups;
extern void ClearWeaponGroups();

//////////////////////////////////////////////////////////////////////////////////////

struct cRage
{
	std::string NAME = enc("Rage");
	// Ragebot specific configs
	struct cRagebot
	{
		std::string NAME = enc("Ragebot");
		var_bool Active = var_bool(enc("Active"));
		var_int Mode = var_int(enc("Mode"));
		var_int Key = var_int(enc("On Key"));
		var_float FOV = var_float(enc("FOV Range"));
		var_int Hitbox = var_int(enc("Hitbox"));
		var_bool Silent = var_bool(enc("Silent"));
		var_bool AutoPistol = var_bool(enc("Auto Pistol"));
		var_bool EnableSpeedLimit = var_bool(enc("Enable Speed Limit"));
		var_float SpeedLimit = var_float(enc("Speed Limit"));
		var_bool AwpBody = var_bool(enc("Awp Body"));
		var_bool Chickens = var_bool(enc("Target Chickens"));
	};
	// Accuracy specific configs
	struct cAccuracy
	{
		std::string NAME = enc("Accuracy");
		var_bool NoSpread = var_bool(enc("No Spread"));
		var_bool AutoShoot = var_bool(enc("Auto Shoot"));
		var_bool AutoWall = var_bool(enc("Auto Wall"));
		var_bool AutoStop = var_bool(enc("Auto Stop"));
		var_bool AutoCrouch = var_bool(enc("Auto Crouch"));
		var_bool AutoScope = var_bool(enc("Auto Scope"));
		var_bool AutoRevolver = var_bool(enc("Auto Revolver"));
		var_int HitChance = var_int(enc("Hit Chance"));
	};
	// Target specific configs
	struct cTarget
	{
		std::string NAME = enc("Target");
		var_int SortBy = var_int(enc("Selection"));
		var_bool TeamMates = var_bool(enc("Friendly Fire"));
		var_int BodyAimAfterShots = var_int(enc("Bodyaim after X shots"));
		var_int BodyAimIfHPLessThan = var_int(enc("Bodyaim if HP less than"));
		var_bool BodyAimIfHPLessThanDamage = var_bool(enc("Bodyaim if HP less than damage"));
		var_bool Hitscan = var_bool(enc("Hit Scan"));
		var_int MinDamage = var_int(enc("Min Damage"));
		var_bool MultiPoint = var_bool(enc("Multi Point"));
	};
};

extern cRage::cRagebot Ragebot;
extern cRage::cTarget RageTarget;
extern cRage::cAccuracy RageAccuracy;

//////////////////////////////////////////////////////////////////////////////////////

struct cHvH
{
	std::string NAME = enc("HvH");
	// Anti-Aim specific configs
	struct cAntiAim
	{
		var_bool Active = var_bool(enc("Active"));
		var_int Pitch = var_int(enc("Pitch"));
		var_float CustomPitch = var_float(enc("Custom Pitch"));
		var_int RealYaw = var_int(enc("Real Yaw"));
		var_float RealYawMod = var_float(enc("Real Yaw Modifier"));
		var_int FakeYaw = var_int(enc("Fake Yaw"));
		var_float FakeYawMod = var_float(enc("Fake Yaw Modifier"));
		var_bool Edge = var_bool(enc("Wall Detection"));
		var_bool AtTargets = var_bool(enc("At Targets"));
		var_bool OnLadder = var_bool(enc("On Ladder"));
		var_bool OnKnife = var_bool(enc("On Knife"));
		var_bool OnGrenades = var_bool(enc("On Grenades"));
		var_bool FakeWalk = var_bool(enc("Fake Walk"));
		var_int FakeWalkKey = var_int(enc("Fake Walk Key"));
		var_float JitterRange = var_float(enc("Jitter Range"));
		var_float SpinSpeed = var_float(enc("Spinbot Speed"));
		var_float LowerBodyDelta = var_float(enc("Lower Body Delta"));
		//var_bool ShowRealAA = var_bool(enc("Show ThirdPerson AA"));
		var_bool FakeAnglesChams = var_bool(enc("Fake Angles Chams"));
		var_bool AnglesInfo = var_bool(enc("Show Angles Info"));
	};
	// Resolver specific configs
	struct cResolver
	{
		var_bool Active = var_bool(enc("Active"));
		var_int Type = var_int(enc("Type"));
		//var_bool Backtracking = var_bool(enc("Backtracking"));
		var_bool BacktrackSkeleton = var_bool(enc("Backtrack Skeleton"));
		var_bool BacktrackPoint = var_bool(enc("Backtrack Point"));
		var_bool ResolverESP = var_bool(enc("Resolver ESP"));
	};
	// FakeLag specific configs
	struct cFakeLag
	{
		var_bool Active = var_bool(enc("Active"));
		var_bool UseKey = var_bool(enc("Use Key"));
		var_int Key = var_int(enc("On Key"));
		var_int Factor = var_int(enc("Factor"));
		var_int Mode = var_int(enc("Mode"));
		var_bool OnShoot = var_bool(enc("On Shoot"));
		var_bool AirOnly = var_bool(enc("In Air Only"));
	};
	// PlayerList specific configs
	struct cPlayerList
	{
		var_bool Active = var_bool(enc("Active"));
	};
};

extern cHvH::cAntiAim AntiAim;
extern cHvH::cResolver Resolver;
extern cHvH::cFakeLag FakeLag;
extern cHvH::cPlayerList PlayerList;

//////////////////////////////////////////////////////////////////////////////////////

struct cVisuals
{
	std::string NAME = enc("Visuals");
	// ESP specific configs
	struct cESP
	{
		var_bool Active = var_bool(enc("Active"));
		var_bool BoxEnabled = var_bool(enc("Enable Box"));
		var_int BoxType = var_int(enc("Box Type"));
		var_bool BoxOutline = var_bool(enc("Outlines"));
		var_bool BoxFilled = var_bool(enc("Box Filled"));
		var_int BoxFillAlpha = var_int(enc("Filled Alpha"));
		var_float FadeSpeed = var_float(enc("Fade Speed"));
		var_bool Health = var_bool(enc("Health"));
		var_bool Armor = var_bool(enc("Armor"));
		var_bool BatteryBars = var_bool(enc("Battery Bars"));
		var_bool Name = var_bool(enc("Name"));
		var_bool Weapon = var_bool(enc("Weapon"));
		var_bool Ammo = var_bool(enc("Ammo"));
		var_bool Money = var_bool(enc("Money"));
		var_bool HasBomb = var_bool(enc("Has Bomb"));
		var_bool HasDefuser = var_bool(enc("Has Defuser"));
		var_bool IsDefusing = var_bool(enc("Is Defusing"));
		var_bool RevealRanks = var_bool(enc("Reveal Ranks"));
		var_bool BombTimer = var_bool(enc("Bomb Timer"));
		var_bool Barrel = var_bool(enc("Barrel"));
		var_int SnapLines = var_int(enc("Snap Lines"));
		var_bool NadePrediction = var_bool(enc("Nade Prediction"));
		var_bool NadeDamage = var_bool(enc("Nade Damage"));
		var_bool Skeleton = var_bool(enc("Skeleton"));
		var_int Hitbox = var_int(enc("Hitbox"));
		var_bool HeadPoint = var_bool(enc("Head Point"));
		var_bool AimSpot = var_bool(enc("Aim Spot"));
		var_bool SoundESP = var_bool(enc("Sound ESP"));
		var_bool Glow = var_bool(enc("Glow"));
		var_float GlowAlpha = var_float(enc("Glow Alpha"));
		var_bool Watermark = var_bool(enc("Watermark"));
		var_int WatermarkAlpha = var_int(enc("Watermark Alpha"));
		var_int Overhead = var_int(enc("Overhead"));
	};
	// Filter specific configs
	struct cFilters
	{
		var_bool All = var_bool(enc("All"));
		var_bool TeamMates = var_bool(enc("Team Mates"));
		var_bool Enemies = var_bool(enc("Enemies"));
		var_bool VisibleOnly = var_bool(enc("Visible Only"));
		var_bool Guns = var_bool(enc("Weapons"));
		var_bool Grenades = var_bool(enc("Grenades"));
		var_bool Bomb = var_bool(enc("Bomb"));
		var_bool Hostages = var_bool(enc("Hostages"));
	};
	// Removals specific configs
	struct cRemovals
	{
		var_bool NoVisualRecoil = var_bool(enc("No Visual Recoil"));
		var_bool NoScope = var_bool(enc("No Scope"));
		var_bool NoFlash = var_bool(enc("No Flash"));
		var_bool NoSmoke = var_bool(enc("No Smoke"));
		var_bool NoSky = var_bool(enc("No Sky"));
		var_bool NoHands = var_bool(enc("No Hands"));
		var_bool NoWeapons = var_bool(enc("No Weapons"));
		var_bool NoPostProcessing = var_bool(enc("No PostProcessing"));
	};
	// Chams specific configs
	struct cChams
	{
		var_bool Active = var_bool(enc("Active"));
		var_bool EnemyOnly = var_bool(enc("Enemy Only"));
		var_bool XQZ = var_bool(enc("XQZ"));
		var_int VisibleType = var_int(enc("Chams Visible"));
		var_int HiddenType = var_int(enc("Chams Hidden"));
		var_int HandChams = var_int(enc("Hand Chams"));
		var_int WeaponChams = var_int(enc("Weapon Chams"));
	};
	// Other visual stuff
	struct cOther
	{
		var_int ToggleKey = var_int(enc("Toggle Key"));
		var_bool Lasers = var_bool(enc("Beams Bullet Tracers"));
		var_bool FootstepRings = var_bool(enc("Footstep Rings"));
		var_int LaserType = var_int(enc("Beams Type"));
		var_int RenderMode = var_int(enc("Render Mode"));
		var_bool Crosshair = var_bool(enc("Crosshair"));
		var_int CrosshairType = var_int(enc("Crosshair Type"));
		var_bool RecoilCrosshair = var_bool(enc("Recoil Crosshair"));
		var_bool SpreadCircle = var_bool(enc("Spread Circle"));
		var_bool EngineRadar = var_bool(enc("Engine Radar"));
		var_bool Radar = var_bool(enc("Radar"));
		var_float RadarRange = var_float(enc("Radar Range"));
		var_bool ColorWalls = var_bool(enc("Color Walls"));
		var_bool ThirdPerson = var_bool(enc("Third Person"));
		//var_bool ThirdPersonSmart = var_bool(enc("Smart Third Person")); removed
		var_float CustomFOV = var_float(enc("Custom FOV"));
		var_float ViewModelFOV = var_float(enc("View Model FOV"));
		var_bool Hitmarker = var_bool(enc("Hitmarker"));
		var_bool HurtLogs = var_bool(enc("Hurt Logs"));
	};
	// Notifications
	struct cNotifications
	{
		var_bool EnableNotifications = var_bool(enc("Enable Notifications"));
		var_bool IgnoreLocal = var_bool(enc("Ignore Local Events"));
		var_bool ItemPurchase = var_bool(enc("Item Purchase"));
		var_bool PlayerDeath = var_bool(enc("Player Death"));
		var_bool BombBeginPlant = var_bool(enc("Bomb Begin Plant"));
		var_bool BombAbortPlant = var_bool(enc("Bomb Abort Plant"));
		var_bool BombPlanted = var_bool(enc("Bomb Planted"));
		var_bool BombDefused = var_bool(enc("Bomb Defused"));
		var_bool BombExploded = var_bool(enc("Bomb Exploded"));
		var_bool BombDropped = var_bool(enc("Bomb Dropped"));
		var_bool BombPickup = var_bool(enc("Bomb Pickup"));
		var_bool BombBeginDefuse = var_bool(enc("Bomb Begin Defuse"));
		var_bool BombAbortDefuse = var_bool(enc("Bomb Abort Defuse"));
		var_bool HostageHurt = var_bool(enc("Hostage Hurt"));
		var_bool HostageKilled = var_bool(enc("Hostage Killed"));
		var_bool HostageRescued = var_bool(enc("Hostage Rescued"));
		var_bool WeaponReload = var_bool(enc("Weapon Reload"));
		var_bool ItemPickup = var_bool(enc("Item Pickup"));
		void ListenAll()
		{
			ItemPurchase.m_Value = true;
			PlayerDeath.m_Value = true;
			BombBeginPlant.m_Value = true;
			BombAbortPlant.m_Value = true;
			BombPlanted.m_Value = true;
			BombDefused.m_Value = true;
			BombExploded.m_Value = true;
			BombDropped.m_Value = true;
			BombPickup.m_Value = true;
			BombBeginDefuse.m_Value = true;
			BombAbortDefuse.m_Value = true;
			HostageHurt.m_Value = true;
			HostageKilled.m_Value = true;
			HostageRescued.m_Value = true;
			WeaponReload.m_Value = true;
			ItemPickup.m_Value = true;
		}
		void ListenNone()
		{
			ItemPurchase.m_Value = false;
			PlayerDeath.m_Value = false;
			BombBeginPlant.m_Value = false;
			BombAbortPlant.m_Value = false;
			BombPlanted.m_Value = false;
			BombDefused.m_Value = false;
			BombExploded.m_Value = false;
			BombDropped.m_Value = false;
			BombPickup.m_Value = false;
			BombBeginDefuse.m_Value = false;
			BombAbortDefuse.m_Value = false;
			HostageHurt.m_Value = false;
			HostageKilled.m_Value = false;
			HostageRescued.m_Value = false;
			WeaponReload.m_Value = false;
			ItemPickup.m_Value = false;
		}
	};
};

extern cVisuals::cESP ESP;
extern cVisuals::cFilters VisualsFilters;
extern cVisuals::cRemovals Removals;
extern cVisuals::cChams Chams;
extern cVisuals::cOther VisualsOther;
extern cVisuals::cNotifications Notifications;

//////////////////////////////////////////////////////////////////////////////////////

struct cMisc
{
	std::string NAME = enc("Misc");
	// Misc configs
	var_int MenuKey = var_int(enc("Menu Key"));
	var_int PanicKey = var_int(enc("Panic Key"));
	var_float MenuFadeSpeed = var_float(enc("Menu Fade Speed"));
	var_bool BunnyHop = var_bool(enc("Bunny Hop"));
	var_bool EdgeJump = var_bool(enc("Edge Jump"));
	var_bool AutoStrafe = var_bool(enc("Auto Strafe"));
	var_bool KnifeBot = var_bool(enc("Knife Bot"));
	var_int AirStuckKey = var_int(enc("Air Stuck"));
	var_bool AutoAccept = var_bool(enc("Auto Accept"));
	var_bool MovementRecorder = var_bool(enc("Movement Recorder"));
	var_bool ChatSpam = var_bool(enc("Chat Spam"));
	var_bool NameSpam = var_bool(enc("Name Spam"));
	var_bool NameStealer = var_bool(enc("Name Stealer"));
	var_bool NoName = var_bool(enc("No Name"));
	var_bool SpectatorList = var_bool(enc("Spectator List"));
	var_int ClanTag = var_int(enc("ClanTag"));
	// AutoStrafe specific configs
	struct cAutoStrafe
	{
		var_int Mode = var_int(enc("Mode"));
		var_bool AirStrafe = var_bool(enc("Air Strafe"));
		var_bool GroundStrafe = var_bool(enc("Ground Strafe"));
		var_int CircleStrafe = var_int(enc("Circle Strafe"));
		var_float RetrackSpeed = var_float(enc("Retrack Speed"));
	};
};

extern cMisc Misc;
extern cMisc::cAutoStrafe AutoStrafe;

//////////////////////////////////////////////////////////////////////////////////////

struct cColors
{
	// Color configs
	var_color BoxTeam = var_color(enc("Box Team"));
	var_color BoxEnemy = var_color(enc("Box Enemy"));
	var_color SkeletonTeam = var_color(enc("Skeleton Team"));
	var_color SkeletonEnemy = var_color(enc("Skeleton Enemy"));
	var_color HitboxTeam = var_color(enc("Hitbox Team"));
	var_color HitboxEnemy = var_color(enc("Hitbox Enemy"));
	var_color GlowTeam = var_color(enc("Glow Team"));
	var_color GlowEnemy = var_color(enc("Glow Enemy"));
	var_color Watermark = var_color(enc("Watermark"));
	var_color ChamsTeam = var_color(enc("Chams Team"));
	var_color ChamsEnemy = var_color(enc("Chams Enemy"));
	var_color ChamsTeam_XQZ = var_color(enc("Chams Team XQZ"));
	var_color ChamsEnemy_XQZ = var_color(enc("Chams Enemy XQZ"));
	var_color HandChams = var_color(enc("Hand Chams"));
	var_color WeaponChams = var_color(enc("Weapon Chams"));
	var_color Crosshair = var_color(enc("Crosshair"));
	var_color RadarBoxes = var_color(enc("Radar Boxes"));
	var_color MenuText = var_color(enc("Menu Text"));
	var_color Walls = var_color(enc("Walls"));
	var_color Beams = var_color(enc("Beams"));
	// Call once to init all colors to their default values
	void InitColors()
	{
		BoxTeam.m_Value = Source::Color::Blue();
		BoxEnemy.m_Value = Source::Color::Red();
		SkeletonTeam.m_Value = Source::Color::White();
		SkeletonEnemy.m_Value = Source::Color::White();
		HitboxTeam.m_Value = BoxTeam.m_Value;
		HitboxEnemy.m_Value = BoxEnemy.m_Value;
		GlowTeam.m_Value = Source::Color::Lime();
		GlowEnemy.m_Value = Source::Color::Orange();
		Watermark.m_Value = Source::Color::White();
		ChamsTeam.m_Value = Source::Color::Lime();
		ChamsEnemy.m_Value = Source::Color::Yellow();
		ChamsTeam_XQZ.m_Value = Source::Color::Blue();
		ChamsEnemy_XQZ.m_Value = Source::Color::Orange();
		HandChams.m_Value = Source::Color::White();
		WeaponChams.m_Value = Source::Color::White();
		Crosshair.m_Value = Source::Color::White();
		RadarBoxes.m_Value = Source::Color::Red();
		MenuText.m_Value = Source::Color::Yellow();
		Walls.m_Value = Source::Color::DarkViolet();
		Beams.m_Value = Source::Color::Magenta();
	}
};

extern cColors Colors;

//////////////////////////////////////////////////////////////////////////////////////

struct cSkins
{
	var_bool Active = var_bool(enc("Active"));
	struct cPaintKits
	{
		var_bool Active = var_bool(enc("Enabled"));
		var_int Weapon = var_int(enc("Weapon"));
		var_int PaintKit = var_int(enc("Paint Kit"));
	};
	struct cKnifeGlove
	{
		var_int Knife = var_int(enc("Knife Index"));
		var_int Glove = var_int(enc("Glove Index"));
	};
};

// Used to store a sticker config
struct EconomySticker
{
	int Index = 0;
	float Wear = 0.0f;
	float Scale = 1.0f;
	float Rotation = 0.0f;
	// THESE BELOW ARE INTERNAL MEMBERS. NEVER LOAD OR SAVE THEM. PLZ
	int kit_vector_index = 0;
	void UpdateItem()
	{
		kit_vector_index = kit_vector_index < k_stickers.size() ? kit_vector_index : k_stickers.size() - 1;
		Index = k_stickers.at(kit_vector_index).index;
	}
	void UpdateIds()
	{
		kit_vector_index = std::find_if(k_stickers.begin(), k_stickers.end(), [this](const Kit_t& x)
		{
			return this->Index == x.index;
		}) - k_stickers.begin();
	}
};

// Used to store a weapon skin config
struct EconomyItem
{
	EconomyItem() :
		WeaponName(""),
		PaintKit(0),
		StatTrak(-1),
		Wear(0.00001f),
		Seed(0),
		Stickers({})
	{
		strcpy_s(CustomName, "");
	}
	EconomyItem(const std::string& weapon_name) : WeaponName(weapon_name),
		PaintKit(0),
		StatTrak(-1),
		Wear(0.00001f),
		Seed(0),
		Stickers({}) 
	{
		strcpy_s(CustomName, "");
	}
	// These are members that I'm using, notice the CamelCase notation
	std::string WeaponName = ""; // Fancy weapon name (FANCY, use Localize to translate)
	int PaintKit;
	int StatTrak;
	float Wear;
	int Seed;
	char CustomName[32];
	std::array<EconomySticker, 5> Stickers;
	// THESE BELOW ARE INTERNAL MEMBERS. NEVER LOAD OR SAVE THEM. PLZ
	int idi_vector_index = 0;
	int item_definition_index = 0;
	int pk_vector_index = 0;
	int idi_override_vector_index = 0;
	int idi_override_index = 0;
	// Makes the internal values actual values
	void UpdateItem()
	{
		// Update item definition index
		idi_vector_index = idi_vector_index < k_weapon_names.size() ? idi_vector_index : k_weapon_names.size() - 1;
		item_definition_index = k_weapon_names.at(idi_vector_index).definition_index;

		// Update paintkit vector. Based on what it's selected, either from k_skins or k_gloves
		const auto& skin_set = item_definition_index == Source::GLOVE_T_SIDE ? k_gloves : k_skins;
		const auto& name_set = item_definition_index == Source::GLOVE_T_SIDE ? k_glove_names : k_knife_names;

		pk_vector_index = pk_vector_index < skin_set.size() ? pk_vector_index : skin_set.size() - 1;
		PaintKit = skin_set.at(pk_vector_index).index;

		idi_override_vector_index = idi_override_vector_index < name_set.size() ? idi_override_vector_index : name_set.size() - 1;
		idi_override_index = name_set.at(idi_override_vector_index).definition_index;

		for (auto& sticker : Stickers)
			sticker.UpdateItem();
	}
	void UpdateIds()
	{
		idi_vector_index = std::find_if(k_weapon_names.begin(), k_weapon_names.end(), [this](const WeaponName_t& x)
		{
			return this->item_definition_index == x.definition_index;
		}) - k_weapon_names.begin();

		const auto& skin_set = item_definition_index == Source::GLOVE_T_SIDE ? k_gloves : k_skins;

		pk_vector_index = std::find_if(skin_set.begin(), skin_set.end(), [this](const Kit_t& x)
		{
			return this->PaintKit == x.index;
		}) - skin_set.begin();

		const auto& override_set = item_definition_index == Source::GLOVE_T_SIDE ? k_glove_names : k_knife_names;

		idi_override_vector_index = find_if(override_set.begin(), override_set.end(), [this](const WeaponName_t& x)
		{
			return this->idi_override_index == x.definition_index;
		}) - override_set.begin();

		for (auto& sticker : Stickers)
			sticker.UpdateIds();
	}
};

extern cSkins Skins;
extern cSkins::cPaintKits PaintKits;
extern cSkins::cKnifeGlove KnifeGlove;

extern std::unordered_map<int, EconomyItem> SkinConfig;

//////////////////////////////////////////////////////////////////////////////////////

struct cMenuStyle
{
	var_bool UseCustomStyle = var_bool(enc("Use Custom Style"));
	var_float WindowPadding = var_float(enc("Window Padding"));
	var_float WindowRounding = var_float(enc("Window Rounding"));
	var_float FramePadding = var_float(enc("Frame Padding"));
	var_float FrameRounding = var_float(enc("Frame Rounding"));
	var_float ItemSpacing = var_float(enc("Item Spacing"));
	var_float ItemInnerSpacing = var_float(enc("Item Inner Spacing"));
	var_float IndentSpacing = var_float(enc("Indent Spacing"));
	var_float ScrollbarSize = var_float(enc("Scrollbar Size"));
	var_float ScrollbarRounding = var_float(enc("Scrollbar Rounding"));
	var_float GrabRounding = var_float(enc("Grab Rounding"));
	void ResetStyle()
	{
		WindowPadding.m_Value = 10.f;
		WindowRounding.m_Value = 5.0f;
		FramePadding.m_Value = 5.f;
		FrameRounding.m_Value = 4.0f;
		ItemSpacing.m_Value = 8.f;
		ItemInnerSpacing.m_Value = 6.f;
		IndentSpacing.m_Value = 25.0f;
		ScrollbarSize.m_Value = 15.0f;
		ScrollbarRounding.m_Value = 9.0f;
		GrabRounding.m_Value = 3.0f;
	}
};

extern cMenuStyle MenuStyle;

// Save the config to config_path/name.json
extern void Save(const std::string& name);
// Load the config from config_path/name.json
extern void Load(const std::string& name);

// Save skins to config_path/skins.json
extern void SaveSkins();
// Load skins from config_path/skins.json
extern void LoadSkins();

}
}