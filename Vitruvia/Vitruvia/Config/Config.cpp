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

#include "Config.h"

#define MAP(name) name##.m_strName, name##.m_Value
#define READCFG(sub, name, type) name##.m_Value = sub.at(name##.m_strName).get<type>()

namespace Vitruvia {
namespace Config {

// Helper to read a color config
__forceinline void READCFG_COLOR(const json& cfg, var_color& clr)
{
	json arr = cfg.at(enc("Colors")).at(clr.m_strName);
	clr.m_Value.SetColor(
		arr.at(0), arr.at(1), arr.at(2), 255
	);
}

//////////////////////////////////////////////////////////////////////////////////////

cLegit::cAimbot Aimbot;
cLegit::cRCS RCS;
cLegit::cTrigger Trigger;
std::unordered_map<Source::WeaponTypes, cLegit::cWeaponGroup*> WeaponGroups
{
	{ Source::WeaponTypes::WEAPONTYPE_RIFLE, new cLegit::cWeaponGroup(enc("Rifle")) },
	{ Source::WeaponTypes::WEAPONTYPE_PISTOL, new cLegit::cWeaponGroup(enc("Pistol")) },
	{ Source::WeaponTypes::WEAPONTYPE_SNIPER_RIFLE, new cLegit::cWeaponGroup(enc("Sniper")) },
	{ Source::WeaponTypes::WEAPONTYPE_SHOTGUN, new cLegit::cWeaponGroup(enc("Shotgun")) },
	{ Source::WeaponTypes::WEAPONTYPE_SUBMACHINEGUN, new cLegit::cWeaponGroup(enc("SMG")) },
	{ Source::WeaponTypes::WEAPONTYPE_MACHINEGUN, new cLegit::cWeaponGroup(enc("Heavy")) }
};
void ClearWeaponGroups()
{
	// This is called on ejecting, to clean-up pointers
	for (auto e : WeaponGroups)
	{
		delete e.second;
		e.second = nullptr;
	}
	WeaponGroups.clear();
}

//////////////////////////////////////////////////////////////////////////////////////

cRage::cRagebot Ragebot;
cRage::cAccuracy RageAccuracy;
cRage::cTarget RageTarget;

//////////////////////////////////////////////////////////////////////////////////////

cHvH::cAntiAim AntiAim;
cHvH::cResolver Resolver;
cHvH::cFakeLag FakeLag;
cHvH::cPlayerList PlayerList;

//////////////////////////////////////////////////////////////////////////////////////

cVisuals::cESP ESP;
cVisuals::cFilters VisualsFilters;
cVisuals::cRemovals Removals;
cVisuals::cChams Chams;
cVisuals::cOther VisualsOther;
cVisuals::cNotifications Notifications;

//////////////////////////////////////////////////////////////////////////////////////

cMisc Misc;
cMisc::cAutoStrafe AutoStrafe;

//////////////////////////////////////////////////////////////////////////////////////

cColors Colors;

//////////////////////////////////////////////////////////////////////////////////////

cSkins Skins;
cSkins::cPaintKits PaintKits;
cSkins::cKnifeGlove KnifeGlove;

//////////////////////////////////////////////////////////////////////////////////////

cMenuStyle MenuStyle;

//////////////////////////////////////////////////////////////////////////////////////

std::unordered_map<int, EconomyItem> SkinConfig {
	{ Source::WEAPON_AK47, EconomyItem("AK-47") },
	{ Source::WEAPON_AUG, EconomyItem("AUG") },
	{ Source::WEAPON_AWP, EconomyItem("AWP") },
	{ Source::WEAPON_CZ75A, EconomyItem("CZ75A") },
	{ Source::WEAPON_DEAGLE, EconomyItem("Deagle") },
	{ Source::WEAPON_ELITE, EconomyItem("Elite") },
	{ Source::WEAPON_FAMAS, EconomyItem("Famas") },
	{ Source::WEAPON_FIVESEVEN, EconomyItem("Five-SeveN") },
	{ Source::WEAPON_G3SG1, EconomyItem("G3SG1") },
	{ Source::WEAPON_GALILAR, EconomyItem("Galil AR") },
	{ Source::WEAPON_GLOCK, EconomyItem("Glock-18") },
	{ Source::WEAPON_M249, EconomyItem("M249") },
	{ Source::WEAPON_M4A1_SILENCER, EconomyItem("M4A1-S") },
	{ Source::WEAPON_M4A4, EconomyItem("M4A4") },
	{ Source::WEAPON_MAC10, EconomyItem("MAC-10") },
	{ Source::WEAPON_MAG7, EconomyItem("MAG-7") },
	{ Source::WEAPON_MP7, EconomyItem("MP7") },
	{ Source::WEAPON_MP9, EconomyItem("MP9") },
	{ Source::WEAPON_NEGEV, EconomyItem("Negev") },
	{ Source::WEAPON_NOVA, EconomyItem("Nova") },
	{ Source::WEAPON_HKP2000, EconomyItem("P2000") },
	{ Source::WEAPON_P250, EconomyItem("P250") },
	{ Source::WEAPON_P90, EconomyItem("P90") },
	{ Source::WEAPON_BIZON, EconomyItem("PP-Bizon") },
	{ Source::WEAPON_REVOLVER, EconomyItem("Revolver") },
	{ Source::WEAPON_SAWEDOFF, EconomyItem("Sawed-Off") },
	{ Source::WEAPON_SCAR20, EconomyItem("SCAR-20") },
	{ Source::WEAPON_SSG08, EconomyItem("SSG-08") },
	{ Source::WEAPON_SG556, EconomyItem("SG-553") },
	{ Source::WEAPON_TEC9, EconomyItem("Tec-9") },
	{ Source::WEAPON_UMP45, EconomyItem("UMP-45") },
	{ Source::WEAPON_USP_SILENCER, EconomyItem("USP-S") },
	{ Source::WEAPON_XM1014, EconomyItem("XM-1014") },
	{ Source::WEAPON_KNIFE_BAYONET, EconomyItem("Bayonet") },
	{ Source::WEAPON_KNIFE_FLIP, EconomyItem("Flip Knife") },
	{ Source::WEAPON_KNIFE_GUT, EconomyItem("Gut Knife") },
	{ Source::WEAPON_KNIFE_KARAMBIT, EconomyItem("Karambit") },
	{ Source::WEAPON_KNIFE_M9_BAYONET, EconomyItem("M9 Bayonet") },
	{ Source::WEAPON_KNIFE_TACTICAL, EconomyItem("Huntsman Knife") },
	{ Source::WEAPON_KNIFE_FALCHION, EconomyItem("Falchion Knife") },
	{ Source::WEAPON_KNIFE_BOWIE, EconomyItem("Bowie Knife") },
	{ Source::WEAPON_KNIFE_BUTTERFLY, EconomyItem("Butterfly Knife") },
	{ Source::WEAPON_KNIFE_PUSH, EconomyItem("Shadow Daggers") }
};

//////////////////////////////////////////////////////////////////////////////////////

void to_json(json& j, const var_int& cfg)
{
	j = json{ cfg.m_strName, cfg.m_Value };
}
void from_json(const json& j, var_int& cfg)
{
	cfg.m_Value = j.at(cfg.m_strName).get<int>();
}

void to_json(json& j, const var_float& cfg)
{
	j = json{ cfg.m_strName, cfg.m_Value };
}
void from_json(const json& j, var_float& cfg)
{
	cfg.m_Value = j.at(cfg.m_strName).get<float>();
}

void to_json(json& j, const var_bool& cfg)
{
	j = json{ cfg.m_strName, cfg.m_Value };
}
void from_json(const json& j, var_bool& cfg)
{
	cfg.m_Value = j.at(cfg.m_strName).get<int>();
}

void to_json(json& j, const var_string& cfg)
{
	j = json{ cfg.m_strName, cfg.m_Value };
}
void from_json(const json& j, var_string& cfg)
{
	cfg.m_Value = j.at(cfg.m_strName).get<std::string>();
}

void to_json(json& j, const var_color& config_color)
{
	j = json
	{config_color.m_strName, {
		config_color.m_Value.r(),
		config_color.m_Value.g(),
		config_color.m_Value.b()
	}};
}

void to_json(json& j, const EconomySticker& sticker_item)
{
	j = json
	{
		{enc("Index"), sticker_item.Index},
		{enc("Wear"), sticker_item.Wear},
		{enc("Scale"), sticker_item.Scale},
		{enc("Rotation"), sticker_item.Rotation},
		{enc("vector_index"), sticker_item.kit_vector_index}
	};
}
void to_json(json& j, const EconomyItem& economy_item)
{
	j = json
	{economy_item.WeaponName, {
		{enc("PaintKit"), economy_item.PaintKit},
		{enc("Stat-Trak"), economy_item.StatTrak},
		{enc("Wear"), economy_item.Wear},
		{enc("Seed"), economy_item.Seed},
		{enc("Custom Name"), economy_item.CustomName},
		{enc("Stickers"), {
			{enc("Slot 1"), economy_item.Stickers[0]},
			{enc("Slot 2"), economy_item.Stickers[1]},
			{enc("Slot 3"), economy_item.Stickers[2]},
			{enc("Slot 4"), economy_item.Stickers[3]},
			{enc("Slot 5"), economy_item.Stickers[4]},
		}},
		{enc("vector_index"), economy_item.pk_vector_index}
	}};
}
void from_json(const json& j, EconomySticker& sticker_item)
{
	sticker_item.Index = j.at(enc("Index")).get<int>();
	sticker_item.Wear = j.at(enc("Wear")).get<float>();
	sticker_item.Scale = j.at(enc("Scale")).get<float>();
	sticker_item.Rotation = j.at(enc("Rotation")).get<float>();
	sticker_item.kit_vector_index = j.at(enc("vector_index")).get<int>();
}
void from_json(const json& j, EconomyItem& economy_item)
{
	economy_item.PaintKit = j.at(enc("PaintKit")).get<int>();
	economy_item.StatTrak = j.at(enc("Stat-Trak")).get<float>();
	economy_item.Wear = j.at(enc("Wear")).get<float>();
	economy_item.Seed = j.at(enc("Seed")).get<int>();
	strcpy_s(economy_item.CustomName, j.at("Custom Name").get<std::string>().c_str());
	economy_item.Stickers[0] = j.at(enc("Stickers")).at(enc("Slot 1"));
	economy_item.Stickers[1] = j.at(enc("Stickers")).at(enc("Slot 2"));
	economy_item.Stickers[2] = j.at(enc("Stickers")).at(enc("Slot 3"));
	economy_item.Stickers[3] = j.at(enc("Stickers")).at(enc("Slot 4"));
	economy_item.Stickers[4] = j.at(enc("Stickers")).at(enc("Slot 5"));
	economy_item.pk_vector_index = j.at(enc("vector_index")).get<int>();
}

//////////////////////////////////////////////////////////////////////////////////////

void Save(const std::string& name)
{
	using Source::WeaponTypes;
	std::ofstream o(global->ConfigFolder().append(name));
	// Map a JSON object containing all the configs
	try
	{
		json cfg = json
		{
			{enc("Legit"), {
				{enc("Aimbot"), {
					Aimbot.Active,
					Aimbot.Mode,
					Aimbot.SortBy,
					Aimbot.AimKey,
					Aimbot.TeamMates,
					Aimbot.VisibleCheck,
					Aimbot.Hitbox,
					Aimbot.FOV,
					Aimbot.Smooth,
					Aimbot.Time,
					Aimbot.SmokeCheck,
					Aimbot.AwpBody,
					Aimbot.SmartAim,
					Aimbot.RCSFactor,
					Aimbot.Silent,
					Aimbot.UseGroups,
					Aimbot.Backtrack,
					Aimbot.AdaptiveBacktrackFOV,
				}},
				{enc("WeaponGroups"), {
					{enc("Rifle"), {
						WeaponGroups[WeaponTypes::WEAPONTYPE_RIFLE]->Active,
						WeaponGroups[WeaponTypes::WEAPONTYPE_RIFLE]->Hitbox,
						WeaponGroups[WeaponTypes::WEAPONTYPE_RIFLE]->FOV,
						WeaponGroups[WeaponTypes::WEAPONTYPE_RIFLE]->Smooth,
						WeaponGroups[WeaponTypes::WEAPONTYPE_RIFLE]->Time,
					}},
					{enc("Pistol"), {
						WeaponGroups[WeaponTypes::WEAPONTYPE_PISTOL]->Active,
						WeaponGroups[WeaponTypes::WEAPONTYPE_PISTOL]->Hitbox,
						WeaponGroups[WeaponTypes::WEAPONTYPE_PISTOL]->FOV,
						WeaponGroups[WeaponTypes::WEAPONTYPE_PISTOL]->Smooth,
						WeaponGroups[WeaponTypes::WEAPONTYPE_PISTOL]->Time,
					}},
					{enc("Sniper"), {
						WeaponGroups[WeaponTypes::WEAPONTYPE_SNIPER_RIFLE]->Active,
						WeaponGroups[WeaponTypes::WEAPONTYPE_SNIPER_RIFLE]->Hitbox,
						WeaponGroups[WeaponTypes::WEAPONTYPE_SNIPER_RIFLE]->FOV,
						WeaponGroups[WeaponTypes::WEAPONTYPE_SNIPER_RIFLE]->Smooth,
						WeaponGroups[WeaponTypes::WEAPONTYPE_SNIPER_RIFLE]->Time,
					}},
					{enc("Shotgun"),{
						WeaponGroups[WeaponTypes::WEAPONTYPE_SHOTGUN]->Active,
						WeaponGroups[WeaponTypes::WEAPONTYPE_SHOTGUN]->Hitbox,
						WeaponGroups[WeaponTypes::WEAPONTYPE_SHOTGUN]->FOV,
						WeaponGroups[WeaponTypes::WEAPONTYPE_SHOTGUN]->Smooth,
						WeaponGroups[WeaponTypes::WEAPONTYPE_SHOTGUN]->Time,
					}},
					{enc("SMG"),{
						WeaponGroups[WeaponTypes::WEAPONTYPE_SUBMACHINEGUN]->Active,
						WeaponGroups[WeaponTypes::WEAPONTYPE_SUBMACHINEGUN]->Hitbox,
						WeaponGroups[WeaponTypes::WEAPONTYPE_SUBMACHINEGUN]->FOV,
						WeaponGroups[WeaponTypes::WEAPONTYPE_SUBMACHINEGUN]->Smooth,
						WeaponGroups[WeaponTypes::WEAPONTYPE_SUBMACHINEGUN]->Time,
					}},
					{enc("Heavy"),{
						WeaponGroups[WeaponTypes::WEAPONTYPE_MACHINEGUN]->Active,
						WeaponGroups[WeaponTypes::WEAPONTYPE_MACHINEGUN]->Hitbox,
						WeaponGroups[WeaponTypes::WEAPONTYPE_MACHINEGUN]->FOV,
						WeaponGroups[WeaponTypes::WEAPONTYPE_MACHINEGUN]->Smooth,
						WeaponGroups[WeaponTypes::WEAPONTYPE_MACHINEGUN]->Time,
					}},
				}},
				{enc("RCS"), {
					RCS.Active,
					RCS.StartBullet,
					RCS.StopBullet,
					RCS.Smooth,
				}},
				{enc("Triggerbot"), {
					Trigger.Active,
					Trigger.Delay,
					Trigger.UseKey,
					Trigger.Key,
					Trigger.Hitchance,
					Trigger.Hitgroup,
					Trigger.SmokeCheck,
					Trigger.ThroughWalls,
					Trigger.MinWallDamage,
				}},
			}},
			{enc("Rage"), {
				{enc("Ragebot"), {
					Ragebot.Active,
					Ragebot.Mode,
					Ragebot.Key,
					Ragebot.FOV,
					Ragebot.Hitbox,
					Ragebot.Silent,
					Ragebot.AutoPistol,
					Ragebot.EnableSpeedLimit,
					Ragebot.SpeedLimit,
					Ragebot.AwpBody,
					Ragebot.Chickens,
				}},
				{enc("Accuracy"), {
					RageAccuracy.NoSpread,
					RageAccuracy.AutoShoot,
					RageAccuracy.AutoWall,
					RageAccuracy.AutoStop,
					RageAccuracy.AutoCrouch,
					RageAccuracy.AutoScope,
					RageAccuracy.AutoRevolver,
					RageAccuracy.HitChance,
				}},
				{enc("Target"), {
					RageTarget.SortBy,
					RageTarget.TeamMates,
					RageTarget.BodyAimAfterShots,
					RageTarget.BodyAimIfHPLessThan,
					RageTarget.BodyAimIfHPLessThanDamage,
					RageTarget.Hitscan,
					RageTarget.MinDamage,
					RageTarget.MultiPoint,
				}},
			}},
			{enc("HvH"), {
				{enc("AntiAim"), {
					AntiAim.Active,
					AntiAim.Pitch,
					AntiAim.CustomPitch,
					AntiAim.RealYaw,
					AntiAim.RealYawMod,
					AntiAim.FakeYaw,
					AntiAim.FakeYawMod,
					AntiAim.Edge,
					AntiAim.AtTargets,
					AntiAim.OnLadder,
					AntiAim.OnKnife,
					AntiAim.OnGrenades,
					AntiAim.FakeWalk,
					AntiAim.FakeWalkKey,
					AntiAim.JitterRange,
					AntiAim.SpinSpeed,
					AntiAim.LowerBodyDelta,
					//AntiAim.ShowRealAA, removed
					AntiAim.FakeAnglesChams,
					AntiAim.AnglesInfo,
				}},
				{enc("Resolver"), {
					Resolver.Active,
					Resolver.Type,
					//Resolver.Backtracking, removed
					Resolver.BacktrackSkeleton,
					Resolver.BacktrackPoint,
					Resolver.ResolverESP,
				}},
				{enc("PlayerList"), {
					PlayerList.Active,
				}},
				{enc("FakeLag"), {
					FakeLag.Active,
					FakeLag.UseKey,
					FakeLag.Key,
					FakeLag.Factor,
					FakeLag.Mode,
					FakeLag.OnShoot,
					FakeLag.AirOnly,
				}},
			}},
			{enc("Visuals"), {
				{enc("ESP"), {
					ESP.Active,
					ESP.BoxEnabled,
					ESP.BoxType,
					ESP.BoxOutline,
					ESP.BoxFilled,
					ESP.BoxFillAlpha,
					ESP.FadeSpeed,
					ESP.Health,
					ESP.Armor,
					ESP.BatteryBars,
					ESP.Name,
					ESP.Weapon,
					ESP.Ammo,
					ESP.Money,
					ESP.HasBomb,
					ESP.HasDefuser,
					ESP.IsDefusing,
					ESP.RevealRanks,
					ESP.BombTimer,
					ESP.Barrel,
					ESP.SnapLines,
					ESP.NadePrediction,
					ESP.NadeDamage,
					ESP.Skeleton,
					ESP.Hitbox,
					ESP.HeadPoint,
					ESP.AimSpot,
					ESP.SoundESP,
					ESP.Glow,
					ESP.GlowAlpha,
					ESP.Watermark,
					ESP.WatermarkAlpha,
					ESP.Overhead,
				}},
				{enc("Filters"), {
					VisualsFilters.All,
					VisualsFilters.TeamMates,
					VisualsFilters.Enemies,
					VisualsFilters.VisibleOnly,
					VisualsFilters.Guns,
					VisualsFilters.Grenades,
					VisualsFilters.Bomb,
					VisualsFilters.Hostages,
				}},
				{enc("Removals"), {
					Removals.NoVisualRecoil,
					Removals.NoScope,
					Removals.NoFlash,
					Removals.NoSmoke,
					Removals.NoSky,
					Removals.NoHands,
					Removals.NoWeapons,
					Removals.NoPostProcessing
				}},
				{enc("Chams"), {
					Chams.Active,
					Chams.EnemyOnly,
					Chams.XQZ,
					Chams.VisibleType,
					Chams.HiddenType,
					Chams.HandChams,
					Chams.WeaponChams,
				}},
				{enc("Other"), {
					VisualsOther.ToggleKey,
					VisualsOther.Lasers,
					VisualsOther.FootstepRings,
					VisualsOther.LaserType,
					VisualsOther.RenderMode,
					VisualsOther.Crosshair,
					VisualsOther.CrosshairType,
					VisualsOther.RecoilCrosshair,
					VisualsOther.SpreadCircle,
					VisualsOther.EngineRadar,
					VisualsOther.Radar,
					VisualsOther.RadarRange,
					VisualsOther.ColorWalls,
					VisualsOther.ThirdPerson,
					//VisualsOther.ThirdPersonSmart, removed
					VisualsOther.CustomFOV,
					VisualsOther.ViewModelFOV,
					VisualsOther.Hitmarker,
					VisualsOther.HurtLogs,
				}},
				{enc("Notifications"), {
					Notifications.EnableNotifications,
					Notifications.IgnoreLocal,
					Notifications.ItemPurchase,
					Notifications.PlayerDeath,
					Notifications.BombBeginPlant,
					Notifications.BombAbortPlant,
					Notifications.BombPlanted,
					Notifications.BombDefused,
					Notifications.BombExploded,
					Notifications.BombDropped,
					Notifications.BombPickup,
					Notifications.BombBeginDefuse,
					Notifications.BombAbortDefuse,
					Notifications.HostageHurt,
					Notifications.HostageKilled,
					Notifications.WeaponReload,
					Notifications.ItemPickup
				}}
			}},
			{enc("Misc"), {
				Misc.MenuKey,
				Misc.PanicKey,
				Misc.MenuFadeSpeed,
				Misc.BunnyHop,
				Misc.EdgeJump,
				Misc.AutoStrafe,
				Misc.KnifeBot,
				Misc.AirStuckKey,
				Misc.AutoAccept,
				Misc.MovementRecorder,
				Misc.ChatSpam,
				Misc.NameSpam,
				Misc.NameStealer,
				Misc.NoName,
				Misc.SpectatorList,
				Misc.ClanTag,
				{enc("AutoStrafe"), {
					AutoStrafe.Mode,
					AutoStrafe.AirStrafe,
					AutoStrafe.GroundStrafe,
					AutoStrafe.CircleStrafe,
					AutoStrafe.RetrackSpeed,
				}},
			}},
			{enc("Colors"), {
				Colors.BoxTeam,
				Colors.BoxEnemy,
				Colors.SkeletonTeam,
				Colors.SkeletonEnemy,
				Colors.HitboxTeam,
				Colors.HitboxEnemy,
				Colors.GlowTeam,
				Colors.GlowEnemy,
				Colors.Watermark,
				Colors.ChamsTeam,
				Colors.ChamsEnemy,
				Colors.ChamsTeam_XQZ,
				Colors.ChamsEnemy_XQZ,
				Colors.HandChams,
				Colors.WeaponChams,
				Colors.Crosshair,
				Colors.RadarBoxes,
				Colors.MenuText,
				Colors.Walls,
				Colors.Beams,
			}},
			{enc("Skins"), {
				Skins.Active,
				KnifeGlove.Knife,
				KnifeGlove.Glove
			}},
			{enc("Styles"), {
				MenuStyle.UseCustomStyle,
				MenuStyle.WindowPadding,
				MenuStyle.WindowRounding,
				MenuStyle.FramePadding,
				MenuStyle.FrameRounding,
				MenuStyle.ItemSpacing,
				MenuStyle.ItemInnerSpacing,
				MenuStyle.IndentSpacing,
				MenuStyle.ScrollbarSize,
				MenuStyle.ScrollbarRounding,
				MenuStyle.GrabRounding
			}}
		};

		o << std::setw(4) << cfg << std::endl;
		o.close();
	}
	catch (const std::exception& e)
	{
		Err(enc("Exception thrown at Config::Save(%s) -> %s"), name.c_str(), e.what());
		o.close();
		return;
	}
}
void Load(const std::string& name)
{
	using Source::WeaponTypes;
	// Read from file
	std::string file_name = global->ConfigFolder().append(name);

	std::ifstream i(file_name);
	if (!i.good())
		return;

	json cfg;

	try
	{
		cfg << i;
	}
	catch (const std::exception& e) {
		Err(enc("Exception thrown at Config::Load() -> %s"), e.what());
		i.close();
		return;
	}

	// Write to config based on json read

	try
	{
		READCFG(cfg.at(enc("Legit")).at(enc("Aimbot")), Aimbot.Active, bool);
		READCFG(cfg.at(enc("Legit")).at(enc("Aimbot")), Aimbot.Mode, int);
		READCFG(cfg.at(enc("Legit")).at(enc("Aimbot")), Aimbot.SortBy, int);
		READCFG(cfg.at(enc("Legit")).at(enc("Aimbot")), Aimbot.AimKey, int);
		READCFG(cfg.at(enc("Legit")).at(enc("Aimbot")), Aimbot.TeamMates, bool);
		READCFG(cfg.at(enc("Legit")).at(enc("Aimbot")), Aimbot.VisibleCheck, bool);
		READCFG(cfg.at(enc("Legit")).at(enc("Aimbot")), Aimbot.Hitbox, int);
		READCFG(cfg.at(enc("Legit")).at(enc("Aimbot")), Aimbot.FOV, float);
		READCFG(cfg.at(enc("Legit")).at(enc("Aimbot")), Aimbot.Smooth, float);
		READCFG(cfg.at(enc("Legit")).at(enc("Aimbot")), Aimbot.Time, float);
		READCFG(cfg.at(enc("Legit")).at(enc("Aimbot")), Aimbot.SmokeCheck, bool);
		READCFG(cfg.at(enc("Legit")).at(enc("Aimbot")), Aimbot.SmartAim, bool);
		READCFG(cfg.at(enc("Legit")).at(enc("Aimbot")), Aimbot.RCSFactor, float);
		READCFG(cfg.at(enc("Legit")).at(enc("Aimbot")), Aimbot.Silent, bool);
		READCFG(cfg.at(enc("Legit")).at(enc("Aimbot")), Aimbot.UseGroups, bool);
		READCFG(cfg.at(enc("Legit")).at(enc("WeaponGroups")).at(WeaponGroups.at(WeaponTypes::WEAPONTYPE_RIFLE)->NAME), WeaponGroups.at(WeaponTypes::WEAPONTYPE_RIFLE)->Active, bool);
		READCFG(cfg.at(enc("Legit")).at(enc("WeaponGroups")).at(WeaponGroups.at(WeaponTypes::WEAPONTYPE_RIFLE)->NAME), WeaponGroups.at(WeaponTypes::WEAPONTYPE_RIFLE)->Hitbox, int);
		READCFG(cfg.at(enc("Legit")).at(enc("WeaponGroups")).at(WeaponGroups.at(WeaponTypes::WEAPONTYPE_RIFLE)->NAME), WeaponGroups.at(WeaponTypes::WEAPONTYPE_RIFLE)->FOV, float);
		READCFG(cfg.at(enc("Legit")).at(enc("WeaponGroups")).at(WeaponGroups.at(WeaponTypes::WEAPONTYPE_RIFLE)->NAME), WeaponGroups.at(WeaponTypes::WEAPONTYPE_RIFLE)->Smooth, float);
		READCFG(cfg.at(enc("Legit")).at(enc("WeaponGroups")).at(WeaponGroups.at(WeaponTypes::WEAPONTYPE_RIFLE)->NAME), WeaponGroups.at(WeaponTypes::WEAPONTYPE_RIFLE)->Time, float);
		READCFG(cfg.at(enc("Legit")).at(enc("WeaponGroups")).at(WeaponGroups.at(WeaponTypes::WEAPONTYPE_PISTOL)->NAME), WeaponGroups.at(WeaponTypes::WEAPONTYPE_PISTOL)->Active, bool);
		READCFG(cfg.at(enc("Legit")).at(enc("WeaponGroups")).at(WeaponGroups.at(WeaponTypes::WEAPONTYPE_PISTOL)->NAME), WeaponGroups.at(WeaponTypes::WEAPONTYPE_PISTOL)->Hitbox, int);
		READCFG(cfg.at(enc("Legit")).at(enc("WeaponGroups")).at(WeaponGroups.at(WeaponTypes::WEAPONTYPE_PISTOL)->NAME), WeaponGroups.at(WeaponTypes::WEAPONTYPE_PISTOL)->FOV, float);
		READCFG(cfg.at(enc("Legit")).at(enc("WeaponGroups")).at(WeaponGroups.at(WeaponTypes::WEAPONTYPE_PISTOL)->NAME), WeaponGroups.at(WeaponTypes::WEAPONTYPE_PISTOL)->Smooth, float);
		READCFG(cfg.at(enc("Legit")).at(enc("WeaponGroups")).at(WeaponGroups.at(WeaponTypes::WEAPONTYPE_PISTOL)->NAME), WeaponGroups.at(WeaponTypes::WEAPONTYPE_PISTOL)->Time, float);
		READCFG(cfg.at(enc("Legit")).at(enc("WeaponGroups")).at(WeaponGroups.at(WeaponTypes::WEAPONTYPE_SNIPER_RIFLE)->NAME), WeaponGroups.at(WeaponTypes::WEAPONTYPE_SNIPER_RIFLE)->Active, bool);
		READCFG(cfg.at(enc("Legit")).at(enc("WeaponGroups")).at(WeaponGroups.at(WeaponTypes::WEAPONTYPE_SNIPER_RIFLE)->NAME), WeaponGroups.at(WeaponTypes::WEAPONTYPE_SNIPER_RIFLE)->Hitbox, int);
		READCFG(cfg.at(enc("Legit")).at(enc("WeaponGroups")).at(WeaponGroups.at(WeaponTypes::WEAPONTYPE_SNIPER_RIFLE)->NAME), WeaponGroups.at(WeaponTypes::WEAPONTYPE_SNIPER_RIFLE)->FOV, float);
		READCFG(cfg.at(enc("Legit")).at(enc("WeaponGroups")).at(WeaponGroups.at(WeaponTypes::WEAPONTYPE_SNIPER_RIFLE)->NAME), WeaponGroups.at(WeaponTypes::WEAPONTYPE_SNIPER_RIFLE)->Smooth, float);
		READCFG(cfg.at(enc("Legit")).at(enc("WeaponGroups")).at(WeaponGroups.at(WeaponTypes::WEAPONTYPE_SNIPER_RIFLE)->NAME), WeaponGroups.at(WeaponTypes::WEAPONTYPE_SNIPER_RIFLE)->Time, float);
		READCFG(cfg.at(enc("Legit")).at(enc("WeaponGroups")).at(WeaponGroups.at(WeaponTypes::WEAPONTYPE_SHOTGUN)->NAME), WeaponGroups.at(WeaponTypes::WEAPONTYPE_SHOTGUN)->Active, bool);
		READCFG(cfg.at(enc("Legit")).at(enc("WeaponGroups")).at(WeaponGroups.at(WeaponTypes::WEAPONTYPE_SHOTGUN)->NAME), WeaponGroups.at(WeaponTypes::WEAPONTYPE_SHOTGUN)->Hitbox, int);
		READCFG(cfg.at(enc("Legit")).at(enc("WeaponGroups")).at(WeaponGroups.at(WeaponTypes::WEAPONTYPE_SHOTGUN)->NAME), WeaponGroups.at(WeaponTypes::WEAPONTYPE_SHOTGUN)->FOV, float);
		READCFG(cfg.at(enc("Legit")).at(enc("WeaponGroups")).at(WeaponGroups.at(WeaponTypes::WEAPONTYPE_SHOTGUN)->NAME), WeaponGroups.at(WeaponTypes::WEAPONTYPE_SHOTGUN)->Smooth, float);
		READCFG(cfg.at(enc("Legit")).at(enc("WeaponGroups")).at(WeaponGroups.at(WeaponTypes::WEAPONTYPE_SHOTGUN)->NAME), WeaponGroups.at(WeaponTypes::WEAPONTYPE_SHOTGUN)->Time, float);
		READCFG(cfg.at(enc("Legit")).at(enc("WeaponGroups")).at(WeaponGroups.at(WeaponTypes::WEAPONTYPE_SUBMACHINEGUN)->NAME), WeaponGroups.at(WeaponTypes::WEAPONTYPE_SUBMACHINEGUN)->Active, bool);
		READCFG(cfg.at(enc("Legit")).at(enc("WeaponGroups")).at(WeaponGroups.at(WeaponTypes::WEAPONTYPE_SUBMACHINEGUN)->NAME), WeaponGroups.at(WeaponTypes::WEAPONTYPE_SUBMACHINEGUN)->Hitbox, int);
		READCFG(cfg.at(enc("Legit")).at(enc("WeaponGroups")).at(WeaponGroups.at(WeaponTypes::WEAPONTYPE_SUBMACHINEGUN)->NAME), WeaponGroups.at(WeaponTypes::WEAPONTYPE_SUBMACHINEGUN)->FOV, float);
		READCFG(cfg.at(enc("Legit")).at(enc("WeaponGroups")).at(WeaponGroups.at(WeaponTypes::WEAPONTYPE_SUBMACHINEGUN)->NAME), WeaponGroups.at(WeaponTypes::WEAPONTYPE_SUBMACHINEGUN)->Smooth, float);
		READCFG(cfg.at(enc("Legit")).at(enc("WeaponGroups")).at(WeaponGroups.at(WeaponTypes::WEAPONTYPE_SUBMACHINEGUN)->NAME), WeaponGroups.at(WeaponTypes::WEAPONTYPE_SUBMACHINEGUN)->Time, float);
		READCFG(cfg.at(enc("Legit")).at(enc("WeaponGroups")).at(WeaponGroups.at(WeaponTypes::WEAPONTYPE_MACHINEGUN)->NAME), WeaponGroups.at(WeaponTypes::WEAPONTYPE_MACHINEGUN)->Active, bool);
		READCFG(cfg.at(enc("Legit")).at(enc("WeaponGroups")).at(WeaponGroups.at(WeaponTypes::WEAPONTYPE_MACHINEGUN)->NAME), WeaponGroups.at(WeaponTypes::WEAPONTYPE_MACHINEGUN)->Hitbox, int);
		READCFG(cfg.at(enc("Legit")).at(enc("WeaponGroups")).at(WeaponGroups.at(WeaponTypes::WEAPONTYPE_MACHINEGUN)->NAME), WeaponGroups.at(WeaponTypes::WEAPONTYPE_MACHINEGUN)->FOV, float);
		READCFG(cfg.at(enc("Legit")).at(enc("WeaponGroups")).at(WeaponGroups.at(WeaponTypes::WEAPONTYPE_MACHINEGUN)->NAME), WeaponGroups.at(WeaponTypes::WEAPONTYPE_MACHINEGUN)->Smooth, float);
		READCFG(cfg.at(enc("Legit")).at(enc("WeaponGroups")).at(WeaponGroups.at(WeaponTypes::WEAPONTYPE_MACHINEGUN)->NAME), WeaponGroups.at(WeaponTypes::WEAPONTYPE_MACHINEGUN)->Time, float);
		READCFG(cfg.at(enc("Legit")).at(enc("RCS")), RCS.Active, bool);
		READCFG(cfg.at(enc("Legit")).at(enc("RCS")), RCS.StartBullet, int);
		READCFG(cfg.at(enc("Legit")).at(enc("RCS")), RCS.StopBullet, int);
		READCFG(cfg.at(enc("Legit")).at(enc("RCS")), RCS.Smooth, bool);
		READCFG(cfg.at(enc("Legit")).at(enc("Triggerbot")), Trigger.Active, bool);
		READCFG(cfg.at(enc("Legit")).at(enc("Triggerbot")), Trigger.Delay, float);
		READCFG(cfg.at(enc("Legit")).at(enc("Triggerbot")), Trigger.UseKey, bool);
		READCFG(cfg.at(enc("Legit")).at(enc("Triggerbot")), Trigger.Key, int);
		READCFG(cfg.at(enc("Legit")).at(enc("Triggerbot")), Trigger.Hitchance, float);
		READCFG(cfg.at(enc("Legit")).at(enc("Triggerbot")), Trigger.Hitgroup, int);
		READCFG(cfg.at(enc("Legit")).at(enc("Triggerbot")), Trigger.SmokeCheck, bool);
		READCFG(cfg.at(enc("Legit")).at(enc("Triggerbot")), Trigger.ThroughWalls, bool);
		READCFG(cfg.at(enc("Legit")).at(enc("Triggerbot")), Trigger.MinWallDamage, float);
		READCFG(cfg.at(enc("Rage")).at(enc("Ragebot")), Ragebot.Active, bool);
		READCFG(cfg.at(enc("Rage")).at(enc("Ragebot")), Ragebot.Mode, int);
		READCFG(cfg.at(enc("Rage")).at(enc("Ragebot")), Ragebot.Key, int);
		READCFG(cfg.at(enc("Rage")).at(enc("Ragebot")), Ragebot.FOV, float);
		READCFG(cfg.at(enc("Rage")).at(enc("Ragebot")), Ragebot.Silent, bool);
		READCFG(cfg.at(enc("Rage")).at(enc("Ragebot")), Ragebot.AutoPistol, bool);
		READCFG(cfg.at(enc("Rage")).at(enc("Ragebot")), Ragebot.SpeedLimit, float);
		READCFG(cfg.at(enc("Rage")).at(enc("Ragebot")), Ragebot.AwpBody, bool);
		READCFG(cfg.at(enc("Rage")).at(enc("Ragebot")), Ragebot.Chickens, bool);
		READCFG(cfg.at(enc("Rage")).at(enc("Accuracy")), RageAccuracy.NoSpread, bool);
		READCFG(cfg.at(enc("Rage")).at(enc("Accuracy")), RageAccuracy.AutoWall, bool);
		READCFG(cfg.at(enc("Rage")).at(enc("Accuracy")), RageAccuracy.AutoStop, bool);
		READCFG(cfg.at(enc("Rage")).at(enc("Accuracy")), RageAccuracy.AutoCrouch, bool);
		READCFG(cfg.at(enc("Rage")).at(enc("Accuracy")), RageAccuracy.AutoScope, bool);
		READCFG(cfg.at(enc("Rage")).at(enc("Accuracy")), RageAccuracy.AutoRevolver, bool);
		READCFG(cfg.at(enc("Rage")).at(enc("Accuracy")), RageAccuracy.HitChance, float);
		READCFG(cfg.at(enc("Rage")).at(enc("Target")), RageTarget.SortBy, int);
		READCFG(cfg.at(enc("Rage")).at(enc("Target")), RageTarget.TeamMates, bool);
		READCFG(cfg.at(enc("Rage")).at(enc("Target")), RageTarget.BodyAimAfterShots, int);
		READCFG(cfg.at(enc("Rage")).at(enc("Target")), RageTarget.BodyAimIfHPLessThan, int);
		READCFG(cfg.at(enc("Rage")).at(enc("Target")), RageTarget.Hitscan, bool);
		READCFG(cfg.at(enc("Rage")).at(enc("Target")), RageTarget.MinDamage, int);
		READCFG(cfg.at(enc("Rage")).at(enc("Target")), RageTarget.MultiPoint, int);
		READCFG(cfg.at(enc("HvH")).at(enc("AntiAim")), AntiAim.Active, bool);
		READCFG(cfg.at(enc("HvH")).at(enc("AntiAim")), AntiAim.Pitch, int);
		READCFG(cfg.at(enc("HvH")).at(enc("AntiAim")), AntiAim.CustomPitch, float);
		READCFG(cfg.at(enc("HvH")).at(enc("AntiAim")), AntiAim.RealYaw, int);
		READCFG(cfg.at(enc("HvH")).at(enc("AntiAim")), AntiAim.RealYawMod, float);
		READCFG(cfg.at(enc("HvH")).at(enc("AntiAim")), AntiAim.FakeYaw, int);
		READCFG(cfg.at(enc("HvH")).at(enc("AntiAim")), AntiAim.FakeYawMod, float);
		READCFG(cfg.at(enc("HvH")).at(enc("AntiAim")), AntiAim.Edge, bool);
		READCFG(cfg.at(enc("HvH")).at(enc("AntiAim")), AntiAim.AtTargets, bool);
		READCFG(cfg.at(enc("HvH")).at(enc("AntiAim")), AntiAim.OnLadder, bool);
		READCFG(cfg.at(enc("HvH")).at(enc("AntiAim")), AntiAim.OnKnife, bool);
		READCFG(cfg.at(enc("HvH")).at(enc("AntiAim")), AntiAim.OnGrenades, bool);
		READCFG(cfg.at(enc("HvH")).at(enc("AntiAim")), AntiAim.FakeWalk, bool);
		READCFG(cfg.at(enc("HvH")).at(enc("AntiAim")), AntiAim.JitterRange, float);
		READCFG(cfg.at(enc("HvH")).at(enc("AntiAim")), AntiAim.SpinSpeed, float);
		READCFG(cfg.at(enc("HvH")).at(enc("Resolver")), Resolver.Active, bool);
		READCFG(cfg.at(enc("HvH")).at(enc("Resolver")), Resolver.Type, int);
		//READCFG(cfg.at(enc("HvH")).at(enc("Resolver")), Resolver.Backtracking, bool);
		READCFG(cfg.at(enc("HvH")).at(enc("PlayerList")), PlayerList.Active, bool);
		READCFG(cfg.at(enc("HvH")).at(enc("FakeLag")), FakeLag.Active, bool);
		READCFG(cfg.at(enc("HvH")).at(enc("FakeLag")), FakeLag.Key, int);
		READCFG(cfg.at(enc("HvH")).at(enc("FakeLag")), FakeLag.Factor, int);
		READCFG(cfg.at(enc("HvH")).at(enc("FakeLag")), FakeLag.Mode, int);
		READCFG(cfg.at(enc("HvH")).at(enc("FakeLag")), FakeLag.OnShoot, bool);
		READCFG(cfg.at(enc("HvH")).at(enc("FakeLag")), FakeLag.AirOnly, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("ESP")), ESP.Active, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("ESP")), ESP.BoxEnabled, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("ESP")), ESP.BoxType, int);
		READCFG(cfg.at(enc("Visuals")).at(enc("ESP")), ESP.BoxFilled, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("ESP")), ESP.BoxFillAlpha, float);
		READCFG(cfg.at(enc("Visuals")).at(enc("ESP")), ESP.Health, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("ESP")), ESP.Armor, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("ESP")), ESP.Name, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("ESP")), ESP.Weapon, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("ESP")), ESP.Ammo, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("ESP")), ESP.Money, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("ESP")), ESP.HasBomb, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("ESP")), ESP.HasDefuser, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("ESP")), ESP.IsDefusing, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("ESP")), ESP.RevealRanks, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("ESP")), ESP.BombTimer, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("ESP")), ESP.Barrel, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("ESP")), ESP.SnapLines, int);
		READCFG(cfg.at(enc("Visuals")).at(enc("ESP")), ESP.NadePrediction, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("ESP")), ESP.Skeleton, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("ESP")), ESP.Hitbox, int);
		READCFG(cfg.at(enc("Visuals")).at(enc("ESP")), ESP.HeadPoint, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("ESP")), ESP.AimSpot, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("ESP")), ESP.SoundESP, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("ESP")), ESP.Glow, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("ESP")), ESP.GlowAlpha, float);
		READCFG(cfg.at(enc("Visuals")).at(enc("ESP")), ESP.Watermark, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("ESP")), ESP.WatermarkAlpha, float);
		READCFG(cfg.at(enc("Visuals")).at(enc("Filters")), VisualsFilters.All, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Filters")), VisualsFilters.TeamMates, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Filters")), VisualsFilters.Enemies, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Filters")), VisualsFilters.VisibleOnly, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Filters")), VisualsFilters.Guns, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Filters")), VisualsFilters.Grenades, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Filters")), VisualsFilters.Bomb, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Filters")), VisualsFilters.Hostages, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Removals")), Removals.NoVisualRecoil, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Removals")), Removals.NoScope, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Removals")), Removals.NoFlash, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Removals")), Removals.NoSmoke, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Removals")), Removals.NoSky, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Removals")), Removals.NoHands, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Removals")), Removals.NoWeapons, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Chams")), Chams.Active, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Chams")), Chams.XQZ, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Chams")), Chams.VisibleType, int);
		READCFG(cfg.at(enc("Visuals")).at(enc("Chams")), Chams.HiddenType, int);
		READCFG(cfg.at(enc("Visuals")).at(enc("Chams")), Chams.HandChams, int);
		READCFG(cfg.at(enc("Visuals")).at(enc("Chams")), Chams.WeaponChams, int);
		READCFG(cfg.at(enc("Visuals")).at(enc("Other")), VisualsOther.ToggleKey, int);
		READCFG(cfg.at(enc("Visuals")).at(enc("Other")), VisualsOther.Crosshair, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Other")), VisualsOther.RecoilCrosshair, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Other")), VisualsOther.EngineRadar, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Other")), VisualsOther.Radar, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Other")), VisualsOther.RadarRange, float);
		READCFG(cfg.at(enc("Visuals")).at(enc("Other")), VisualsOther.ColorWalls, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Other")), VisualsOther.ThirdPerson, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Other")), VisualsOther.CustomFOV, float);
		READCFG(cfg.at(enc("Visuals")).at(enc("Other")), VisualsOther.ViewModelFOV, float);
		READCFG(cfg.at(enc("Misc")), Misc.MenuKey, int);
		READCFG(cfg.at(enc("Misc")), Misc.PanicKey, int);
		READCFG(cfg.at(enc("Misc")), Misc.BunnyHop, bool);
		READCFG(cfg.at(enc("Misc")), Misc.EdgeJump, bool);
		READCFG(cfg.at(enc("Misc")), Misc.AutoStrafe, bool);
		READCFG(cfg.at(enc("Misc")), Misc.KnifeBot, bool);
		READCFG(cfg.at(enc("Misc")), Misc.AirStuckKey, int);
		READCFG(cfg.at(enc("Misc")), Misc.AutoAccept, bool);
		READCFG(cfg.at(enc("Misc")), Misc.MovementRecorder, bool);
		READCFG(cfg.at(enc("Misc")), Misc.ChatSpam, bool);
		READCFG(cfg.at(enc("Misc")), Misc.NameSpam, bool);
		READCFG(cfg.at(enc("Misc")), Misc.NameStealer, bool);
		READCFG(cfg.at(enc("Misc")), Misc.NoName, bool);
		READCFG(cfg.at(enc("Misc")), Misc.SpectatorList, bool);
		READCFG(cfg.at(enc("Misc")), Misc.ClanTag, int);
		READCFG(cfg.at(enc("Misc")).at(enc("AutoStrafe")), AutoStrafe.Mode, int);
		READCFG(cfg.at(enc("Misc")).at(enc("AutoStrafe")), AutoStrafe.AirStrafe, bool);
		READCFG(cfg.at(enc("Misc")).at(enc("AutoStrafe")), AutoStrafe.GroundStrafe, bool);
		READCFG(cfg.at(enc("Misc")).at(enc("AutoStrafe")), AutoStrafe.CircleStrafe, int);
		READCFG(cfg.at(enc("Misc")).at(enc("AutoStrafe")), AutoStrafe.RetrackSpeed, float);
		READCFG_COLOR(cfg, Colors.BoxTeam);
		READCFG_COLOR(cfg, Colors.BoxEnemy);
		READCFG_COLOR(cfg, Colors.SkeletonTeam);
		READCFG_COLOR(cfg, Colors.SkeletonEnemy);
		READCFG_COLOR(cfg, Colors.HitboxTeam);
		READCFG_COLOR(cfg, Colors.HitboxEnemy);
		READCFG_COLOR(cfg, Colors.GlowTeam);
		READCFG_COLOR(cfg, Colors.GlowEnemy);
		READCFG_COLOR(cfg, Colors.Watermark);
		READCFG_COLOR(cfg, Colors.ChamsTeam);
		READCFG_COLOR(cfg, Colors.ChamsEnemy);
		READCFG_COLOR(cfg, Colors.ChamsTeam_XQZ);
		READCFG_COLOR(cfg, Colors.ChamsEnemy_XQZ);
		READCFG_COLOR(cfg, Colors.HandChams);
		READCFG_COLOR(cfg, Colors.WeaponChams);
		READCFG_COLOR(cfg, Colors.Crosshair);
		READCFG_COLOR(cfg, Colors.RadarBoxes);
		READCFG_COLOR(cfg, Colors.MenuText);
		READCFG(cfg.at(enc("Skins")), Skins.Active, bool);
		READCFG(cfg.at(enc("Skins")), KnifeGlove.Knife, int);
		READCFG(cfg.at(enc("Skins")), KnifeGlove.Glove, int);
		// Add new shit here!
		READCFG(cfg.at(enc("Visuals")).at(enc("Removals")), Removals.NoPostProcessing, bool);
		READCFG_COLOR(cfg, Colors.Walls);
		READCFG(cfg.at(enc("Visuals")).at(enc("Chams")), Chams.EnemyOnly, bool);
		//READCFG(cfg.at(enc("Visuals")).at(enc("Other")), VisualsOther.ThirdPersonSmart, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Other")), VisualsOther.Hitmarker, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("ESP")), ESP.Overhead, int);
		READCFG(cfg.at(enc("Visuals")).at(enc("Other")), VisualsOther.CrosshairType, int);
		READCFG(cfg.at(enc("Rage")).at(enc("Ragebot")), Ragebot.EnableSpeedLimit, bool);
		READCFG(cfg.at(enc("Rage")).at(enc("Ragebot")), Ragebot.Hitbox, int);
		READCFG(cfg.at(enc("Rage")).at(enc("Accuracy")), RageAccuracy.AutoShoot, bool);
		//READCFG(cfg.at(enc("HvH")).at(enc("AntiAim")), AntiAim.ShowRealAA, bool);
		READCFG(cfg.at(enc("HvH")).at(enc("FakeLag")), FakeLag.UseKey, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Other")), VisualsOther.RenderMode, int);
		READCFG(cfg.at(enc("HvH")).at(enc("AntiAim")), AntiAim.FakeAnglesChams, bool);
		READCFG(cfg.at(enc("HvH")).at(enc("AntiAim")), AntiAim.FakeWalkKey, int);
		READCFG(cfg.at(enc("Rage")).at(enc("Target")), RageTarget.BodyAimIfHPLessThanDamage, bool);
		READCFG(cfg.at(enc("HvH")).at(enc("AntiAim")), AntiAim.AnglesInfo, bool);
		READCFG(cfg.at(enc("HvH")).at(enc("Resolver")), Resolver.ResolverESP, bool);
		READCFG(cfg.at(enc("Legit")).at(enc("Aimbot")), Aimbot.Backtrack, bool);
		READCFG(cfg.at(enc("Legit")).at(enc("Aimbot")), Aimbot.AwpBody, bool);
		READCFG(cfg.at(enc("HvH")).at(enc("Resolver")), Resolver.BacktrackSkeleton, bool);
		READCFG(cfg.at(enc("HvH")).at(enc("Resolver")), Resolver.BacktrackPoint, bool);
		READCFG(cfg.at(enc("Legit")).at(enc("Aimbot")), Aimbot.AdaptiveBacktrackFOV, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("ESP")), ESP.FadeSpeed, float);
		READCFG(cfg.at(enc("Misc")), Misc.MenuFadeSpeed, float);
		READCFG_COLOR(cfg, Colors.Beams);
		READCFG(cfg.at(enc("Visuals")).at(enc("Other")), VisualsOther.LaserType, int);
		READCFG(cfg.at(enc("Visuals")).at(enc("Other")), VisualsOther.Lasers, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("ESP")), ESP.NadeDamage, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Other")), VisualsOther.SpreadCircle, bool);
		READCFG(cfg.at(enc("HvH")).at(enc("AntiAim")), AntiAim.LowerBodyDelta, float);
		READCFG(cfg.at(enc("Visuals")).at(enc("ESP")), ESP.BatteryBars, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("ESP")), ESP.BoxOutline, bool);
		READCFG(cfg.at(enc("Styles")), MenuStyle.UseCustomStyle, bool);
		READCFG(cfg.at(enc("Styles")), MenuStyle.WindowPadding, float);
		READCFG(cfg.at(enc("Styles")), MenuStyle.WindowRounding, float);
		READCFG(cfg.at(enc("Styles")), MenuStyle.FramePadding, float);
		READCFG(cfg.at(enc("Styles")), MenuStyle.FrameRounding, float);
		READCFG(cfg.at(enc("Styles")), MenuStyle.ItemSpacing, float);
		READCFG(cfg.at(enc("Styles")), MenuStyle.ItemInnerSpacing, float);
		READCFG(cfg.at(enc("Styles")), MenuStyle.IndentSpacing, float);
		READCFG(cfg.at(enc("Styles")), MenuStyle.ScrollbarSize, float);
		READCFG(cfg.at(enc("Styles")), MenuStyle.ScrollbarRounding, float);
		READCFG(cfg.at(enc("Styles")), MenuStyle.GrabRounding, float);
		READCFG(cfg.at(enc("Visuals")).at(enc("Notifications")), Notifications.EnableNotifications, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Notifications")), Notifications.IgnoreLocal, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Notifications")), Notifications.ItemPurchase, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Notifications")), Notifications.BombBeginPlant, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Notifications")), Notifications.BombAbortPlant, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Notifications")), Notifications.BombPlanted, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Notifications")), Notifications.BombDefused, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Notifications")), Notifications.BombExploded, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Notifications")), Notifications.BombDropped, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Notifications")), Notifications.BombPickup, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Notifications")), Notifications.BombBeginDefuse, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Notifications")), Notifications.BombAbortDefuse, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Notifications")), Notifications.HostageHurt, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Notifications")), Notifications.HostageKilled, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Notifications")), Notifications.HostageRescued, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Notifications")), Notifications.WeaponReload, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Notifications")), Notifications.ItemPickup, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Other")), VisualsOther.HurtLogs, bool);
		READCFG(cfg.at(enc("Visuals")).at(enc("Other")), VisualsOther.FootstepRings, bool);
	}
	catch (const std::exception& e)
	{
		Err(enc("Exception thrown at Config::Load() -> %s"), e.what());
		i.close();
		return;
	}

	i.close();
}

void SaveSkins()
{
	using Source::Weapons;
	std::ofstream o(global->ConfigFolder().append(enc("skins.json")));

	if (!o.good())
		return;

	json skins;

	// Map a json object containing all the skins configs
	try
	{
		skins = json
		{
			SkinConfig.at(Weapons::WEAPON_AK47),
			SkinConfig.at(Weapons::WEAPON_AUG),
			SkinConfig.at(Weapons::WEAPON_AWP),
			SkinConfig.at(Weapons::WEAPON_CZ75A),
			SkinConfig.at(Weapons::WEAPON_DEAGLE),
			SkinConfig.at(Weapons::WEAPON_ELITE),
			SkinConfig.at(Weapons::WEAPON_FAMAS),
			SkinConfig.at(Weapons::WEAPON_FIVESEVEN),
			SkinConfig.at(Weapons::WEAPON_G3SG1),
			SkinConfig.at(Weapons::WEAPON_GALILAR),
			SkinConfig.at(Weapons::WEAPON_GLOCK),
			SkinConfig.at(Weapons::WEAPON_M249),
			SkinConfig.at(Weapons::WEAPON_M4A1_SILENCER),
			SkinConfig.at(Weapons::WEAPON_M4A4),
			SkinConfig.at(Weapons::WEAPON_MAC10),
			SkinConfig.at(Weapons::WEAPON_MAG7),
			SkinConfig.at(Weapons::WEAPON_MP7),
			SkinConfig.at(Weapons::WEAPON_MP9),
			SkinConfig.at(Weapons::WEAPON_NEGEV),
			SkinConfig.at(Weapons::WEAPON_NOVA),
			SkinConfig.at(Weapons::WEAPON_HKP2000),
			SkinConfig.at(Weapons::WEAPON_P250),
			SkinConfig.at(Weapons::WEAPON_P90),
			SkinConfig.at(Weapons::WEAPON_BIZON),
			SkinConfig.at(Weapons::WEAPON_REVOLVER),
			SkinConfig.at(Weapons::WEAPON_SAWEDOFF),
			SkinConfig.at(Weapons::WEAPON_SCAR20),
			SkinConfig.at(Weapons::WEAPON_SSG08),
			SkinConfig.at(Weapons::WEAPON_SG556),
			SkinConfig.at(Weapons::WEAPON_TEC9),
			SkinConfig.at(Weapons::WEAPON_UMP45),
			SkinConfig.at(Weapons::WEAPON_USP_SILENCER),
			SkinConfig.at(Weapons::WEAPON_XM1014),
			SkinConfig.at(Weapons::WEAPON_KNIFE_BAYONET),
			SkinConfig.at(Weapons::WEAPON_KNIFE_FLIP),
			SkinConfig.at(Weapons::WEAPON_KNIFE_GUT),
			SkinConfig.at(Weapons::WEAPON_KNIFE_KARAMBIT),
			SkinConfig.at(Weapons::WEAPON_KNIFE_M9_BAYONET),
			SkinConfig.at(Weapons::WEAPON_KNIFE_TACTICAL),
			SkinConfig.at(Weapons::WEAPON_KNIFE_FALCHION),
			SkinConfig.at(Weapons::WEAPON_KNIFE_BOWIE),
			SkinConfig.at(Weapons::WEAPON_KNIFE_BUTTERFLY),
			SkinConfig.at(Weapons::WEAPON_KNIFE_PUSH)
		};
	} 
	catch (const std::exception& e)
	{
		Err(enc("Exception thrown at Config::SaveSkins() -> %s"), e.what());
		o.close();
		return;
	}

	o << std::setw(4) << skins << std::endl;
	o.close();
}
void LoadSkins()
{
	using Source::Weapons;
	std::ifstream i(global->ConfigFolder().append(enc("skins.json")));
	// Retrive json object from file
	json skins;

	try
	{
		skins << i;
		SkinConfig.at(Weapons::WEAPON_AK47) = skins.at(SkinConfig.at(Weapons::WEAPON_AK47).WeaponName);
		SkinConfig.at(Weapons::WEAPON_AK47).WeaponName = enc("AK-47");
		SkinConfig.at(Weapons::WEAPON_AUG) = skins.at(SkinConfig.at(Weapons::WEAPON_AUG).WeaponName);
		SkinConfig.at(Weapons::WEAPON_AUG).WeaponName = enc("AUG");
		SkinConfig.at(Weapons::WEAPON_AWP) = skins.at(SkinConfig.at(Weapons::WEAPON_AWP).WeaponName);
		SkinConfig.at(Weapons::WEAPON_AWP).WeaponName = enc("AWP");
		SkinConfig.at(Weapons::WEAPON_CZ75A) = skins.at(SkinConfig.at(Weapons::WEAPON_CZ75A).WeaponName);
		SkinConfig.at(Weapons::WEAPON_CZ75A).WeaponName = enc("CZ75A");
		SkinConfig.at(Weapons::WEAPON_DEAGLE) = skins.at(SkinConfig.at(Weapons::WEAPON_DEAGLE).WeaponName);
		SkinConfig.at(Weapons::WEAPON_DEAGLE).WeaponName = enc("Deagle");
		SkinConfig.at(Weapons::WEAPON_ELITE) = skins.at(SkinConfig.at(Weapons::WEAPON_ELITE).WeaponName);
		SkinConfig.at(Weapons::WEAPON_ELITE).WeaponName = enc("Elite");
		SkinConfig.at(Weapons::WEAPON_FAMAS) = skins.at(SkinConfig.at(Weapons::WEAPON_FAMAS).WeaponName);
		SkinConfig.at(Weapons::WEAPON_FAMAS).WeaponName = enc("Famas");
		SkinConfig.at(Weapons::WEAPON_FIVESEVEN) = skins.at(SkinConfig.at(Weapons::WEAPON_FIVESEVEN).WeaponName);
		SkinConfig.at(Weapons::WEAPON_FIVESEVEN).WeaponName = enc("Five-SeveN");
		SkinConfig.at(Weapons::WEAPON_G3SG1) = skins.at(SkinConfig.at(Weapons::WEAPON_G3SG1).WeaponName);
		SkinConfig.at(Weapons::WEAPON_G3SG1).WeaponName = enc("G3SG1");
		SkinConfig.at(Weapons::WEAPON_GALILAR) = skins.at(SkinConfig.at(Weapons::WEAPON_GALILAR).WeaponName);
		SkinConfig.at(Weapons::WEAPON_GALILAR).WeaponName = enc("Galil AR");
		SkinConfig.at(Weapons::WEAPON_GLOCK) = skins.at(SkinConfig.at(Weapons::WEAPON_GLOCK).WeaponName);
		SkinConfig.at(Weapons::WEAPON_GLOCK).WeaponName = enc("Glock-18");
		SkinConfig.at(Weapons::WEAPON_M249) = skins.at(SkinConfig.at(Weapons::WEAPON_M249).WeaponName);
		SkinConfig.at(Weapons::WEAPON_M249).WeaponName = enc("M249");
		SkinConfig.at(Weapons::WEAPON_M4A1_SILENCER) = skins.at(SkinConfig.at(Weapons::WEAPON_M4A1_SILENCER).WeaponName);
		SkinConfig.at(Weapons::WEAPON_M4A1_SILENCER).WeaponName = enc("M4A1-S");
		SkinConfig.at(Weapons::WEAPON_M4A4) = skins.at(SkinConfig.at(Weapons::WEAPON_M4A4).WeaponName);
		SkinConfig.at(Weapons::WEAPON_M4A4).WeaponName = enc("M4A4");
		SkinConfig.at(Weapons::WEAPON_MAC10) = skins.at(SkinConfig.at(Weapons::WEAPON_MAC10).WeaponName);
		SkinConfig.at(Weapons::WEAPON_MAC10).WeaponName = enc("MAC-10");
		SkinConfig.at(Weapons::WEAPON_MAG7) = skins.at(SkinConfig.at(Weapons::WEAPON_MAG7).WeaponName);
		SkinConfig.at(Weapons::WEAPON_MAG7).WeaponName = enc("MAG-7");
		SkinConfig.at(Weapons::WEAPON_MP7) = skins.at(SkinConfig.at(Weapons::WEAPON_MP7).WeaponName);
		SkinConfig.at(Weapons::WEAPON_MP7).WeaponName = enc("MP7");
		SkinConfig.at(Weapons::WEAPON_MP9) = skins.at(SkinConfig.at(Weapons::WEAPON_MP9).WeaponName);
		SkinConfig.at(Weapons::WEAPON_MP9).WeaponName = enc("MP9");
		SkinConfig.at(Weapons::WEAPON_NEGEV) = skins.at(SkinConfig.at(Weapons::WEAPON_NEGEV).WeaponName);
		SkinConfig.at(Weapons::WEAPON_NEGEV).WeaponName = enc("Negev");
		SkinConfig.at(Weapons::WEAPON_NOVA) = skins.at(SkinConfig.at(Weapons::WEAPON_NOVA).WeaponName);
		SkinConfig.at(Weapons::WEAPON_NOVA).WeaponName = enc("Nova");
		SkinConfig.at(Weapons::WEAPON_HKP2000) = skins.at(SkinConfig.at(Weapons::WEAPON_HKP2000).WeaponName);
		SkinConfig.at(Weapons::WEAPON_HKP2000).WeaponName = enc("P2000");
		SkinConfig.at(Weapons::WEAPON_P250) = skins.at(SkinConfig.at(Weapons::WEAPON_P250).WeaponName);
		SkinConfig.at(Weapons::WEAPON_P250).WeaponName = enc("P250");
		SkinConfig.at(Weapons::WEAPON_P90) = skins.at(SkinConfig.at(Weapons::WEAPON_P90).WeaponName);
		SkinConfig.at(Weapons::WEAPON_P90).WeaponName = enc("P90");
		SkinConfig.at(Weapons::WEAPON_BIZON) = skins.at(SkinConfig.at(Weapons::WEAPON_BIZON).WeaponName);
		SkinConfig.at(Weapons::WEAPON_BIZON).WeaponName = enc("PP-Bizon");
		SkinConfig.at(Weapons::WEAPON_REVOLVER) = skins.at(SkinConfig.at(Weapons::WEAPON_REVOLVER).WeaponName);
		SkinConfig.at(Weapons::WEAPON_REVOLVER).WeaponName = enc("Revolver");
		SkinConfig.at(Weapons::WEAPON_SAWEDOFF) = skins.at(SkinConfig.at(Weapons::WEAPON_SAWEDOFF).WeaponName);
		SkinConfig.at(Weapons::WEAPON_SAWEDOFF).WeaponName = enc("Sawed-Off");
		SkinConfig.at(Weapons::WEAPON_SCAR20) = skins.at(SkinConfig.at(Weapons::WEAPON_SCAR20).WeaponName);
		SkinConfig.at(Weapons::WEAPON_SCAR20).WeaponName = enc("SCAR-20");
		SkinConfig.at(Weapons::WEAPON_SSG08) = skins.at(SkinConfig.at(Weapons::WEAPON_SSG08).WeaponName);
		SkinConfig.at(Weapons::WEAPON_SSG08).WeaponName = enc("SSG-08");
		SkinConfig.at(Weapons::WEAPON_SG556) = skins.at(SkinConfig.at(Weapons::WEAPON_SG556).WeaponName);
		SkinConfig.at(Weapons::WEAPON_SG556).WeaponName = enc("SG-553");
		SkinConfig.at(Weapons::WEAPON_TEC9) = skins.at(SkinConfig.at(Weapons::WEAPON_TEC9).WeaponName);
		SkinConfig.at(Weapons::WEAPON_TEC9).WeaponName = enc("Tec-9");
		SkinConfig.at(Weapons::WEAPON_UMP45) = skins.at(SkinConfig.at(Weapons::WEAPON_UMP45).WeaponName);
		SkinConfig.at(Weapons::WEAPON_UMP45).WeaponName = enc("UMP-45");
		SkinConfig.at(Weapons::WEAPON_USP_SILENCER) = skins.at(SkinConfig.at(Weapons::WEAPON_USP_SILENCER).WeaponName);
		SkinConfig.at(Weapons::WEAPON_USP_SILENCER).WeaponName = enc("USP-S");
		SkinConfig.at(Weapons::WEAPON_XM1014) = skins.at(SkinConfig.at(Weapons::WEAPON_XM1014).WeaponName);
		SkinConfig.at(Weapons::WEAPON_XM1014).WeaponName = enc("XM-1014");
		SkinConfig.at(Weapons::WEAPON_KNIFE_BAYONET) = skins.at(SkinConfig.at(Weapons::WEAPON_KNIFE_BAYONET).WeaponName);
		SkinConfig.at(Weapons::WEAPON_KNIFE_BAYONET).WeaponName = enc("Bayonet");
		SkinConfig.at(Weapons::WEAPON_KNIFE_FLIP) = skins.at(SkinConfig.at(Weapons::WEAPON_KNIFE_FLIP).WeaponName);
		SkinConfig.at(Weapons::WEAPON_KNIFE_FLIP).WeaponName = enc("Flip Knife");
		SkinConfig.at(Weapons::WEAPON_KNIFE_GUT) = skins.at(SkinConfig.at(Weapons::WEAPON_KNIFE_GUT).WeaponName);
		SkinConfig.at(Weapons::WEAPON_KNIFE_GUT).WeaponName = enc("Gut Knife");
		SkinConfig.at(Weapons::WEAPON_KNIFE_KARAMBIT) = skins.at(SkinConfig.at(Weapons::WEAPON_KNIFE_KARAMBIT).WeaponName);
		SkinConfig.at(Weapons::WEAPON_KNIFE_KARAMBIT).WeaponName = enc("Karambit");
		SkinConfig.at(Weapons::WEAPON_KNIFE_M9_BAYONET) = skins.at(SkinConfig.at(Weapons::WEAPON_KNIFE_M9_BAYONET).WeaponName);
		SkinConfig.at(Weapons::WEAPON_KNIFE_M9_BAYONET).WeaponName = enc("M9 Bayonet");
		SkinConfig.at(Weapons::WEAPON_KNIFE_TACTICAL) = skins.at(SkinConfig.at(Weapons::WEAPON_KNIFE_TACTICAL).WeaponName);
		SkinConfig.at(Weapons::WEAPON_KNIFE_TACTICAL).WeaponName = enc("Huntsman Knife");
		SkinConfig.at(Weapons::WEAPON_KNIFE_FALCHION) = skins.at(SkinConfig.at(Weapons::WEAPON_KNIFE_FALCHION).WeaponName);
		SkinConfig.at(Weapons::WEAPON_KNIFE_FALCHION).WeaponName = enc("Falchion Knife");
		SkinConfig.at(Weapons::WEAPON_KNIFE_BOWIE) = skins.at(SkinConfig.at(Weapons::WEAPON_KNIFE_BOWIE).WeaponName);
		SkinConfig.at(Weapons::WEAPON_KNIFE_BOWIE).WeaponName = enc("Bowie Knife");
		SkinConfig.at(Weapons::WEAPON_KNIFE_BUTTERFLY) = skins.at(SkinConfig.at(Weapons::WEAPON_KNIFE_BUTTERFLY).WeaponName);
		SkinConfig.at(Weapons::WEAPON_KNIFE_BUTTERFLY).WeaponName = enc("Butterfly Knife");
		SkinConfig.at(Weapons::WEAPON_KNIFE_PUSH) = skins.at(SkinConfig.at(Weapons::WEAPON_KNIFE_PUSH).WeaponName);
		SkinConfig.at(Weapons::WEAPON_KNIFE_PUSH).WeaponName = enc("Shadow Daggers");
	}
	catch (const std::exception& e) {
		Err(enc("Exception thrown at Config::LoadSkins() -> %s"), e.what());
		i.close();
		return;
	}
}

}
}
