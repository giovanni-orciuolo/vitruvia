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
#include "../IFeature.h"
#include "../Rage/Rage.h"
#include "../../SourceClasses/C_CSPlayer.h"

namespace Vitruvia {

class F_HvH : public V_IFeature
{
public:
	F_HvH() : m_bFlip(false), 
		m_FakeAngles(Source::QAngle()), 
		m_RealAngles(Source::QAngle()) 
	{}

	void FixMovement(Source::CUserCmd* cmd, Source::QAngle oldAngles, float oldForward, float oldSide);

	float GetPitch(Source::CUserCmd* cmd);
	float GetRealYaw(Source::CUserCmd* cmd);
	float GetFakeYaw(Source::CUserCmd* cmd);
	void AtTargets(Source::C_CSPlayer* local);
	void EdgeAntiAim(Source::CUserCmd* cmd, Source::C_CSPlayer* local);
	float GetSmartYaw(); // made by C0re

	void Choke(Source::CUserCmd* cmd, Source::C_CSPlayer* local);
	void RunAA(Source::CUserCmd* cmd, Source::C_CSPlayer* local);
	void FakeWalk(Source::CUserCmd* cmd, Source::C_CSPlayer* local);

	void OnCreateMove(Source::CUserCmd* cmd) override;
	void OnFrameStageNotify(Source::ClientFrameStage_t stage) override;
	void OnPaintTraverse() override;
	void OnSceneEnd() override;
	void OnLBYProxy(Source::C_CSPlayer* updater, float newValue) override;
	void OnListenedEvent(Source::IGameEvent* game_event) override;

	bool GetFlip() { return m_bFlip; }
	Source::QAngle GetFakeAngles() { return m_FakeAngles; }
	Source::QAngle GetRealAngles() { return m_RealAngles; }

private:
	bool m_bFlip;
	Source::QAngle m_FakeAngles, m_RealAngles;
	float m_flLastLBYUpdate; // Last time LBY did update (local simulation time)
};

struct ResolveInfo_t
{
	Source::Vector Origin;
	std::string ResolveStage;
	float LBYDelta;
	bool UsingFakeAngles;
	bool FakeWalking;
	bool ShouldBodyAim;

	ResolveInfo_t()
		: Origin(Source::Vector()),
		ResolveStage(enc("None")),
		LBYDelta(0.f),
		UsingFakeAngles(false),
		FakeWalking(false),
		ShouldBodyAim(false)
	{}

	ResolveInfo_t(const Source::Vector& origin, const std::string& resolve_stage, float lby_delta, bool using_fake, bool fake_walking, bool should_body)
		: Origin(origin),
		ResolveStage(resolve_stage),
		LBYDelta(lby_delta),
		UsingFakeAngles(using_fake),
		FakeWalking(fake_walking),
		ShouldBodyAim(should_body)
	{}
};

struct TickRecord
{
	Source::Vector Origin;
	Source::QAngle* Angles;
	float SimulationTime;
	float LowerBodyYaw;
	std::array<float, Source::C_BaseAnimating::NUM_POSEPAREMETERS> PoseParameters;
	std::array<Source::C_AnimationLayer, Source::C_BaseAnimatingOverlay::MAX_OVERLAYS> AnimationLayers;
	int Flags;
	int TickCount;
	BoneMatrix BoneMatrix;
	Source::Vector HeadPos;
	bool LBYUpdated;
	bool IsMoving;
	Source::Vector Velocity;
	bool Resolved;

	TickRecord()
		: Origin(Source::Vector()),
		Angles(nullptr),
		SimulationTime(0),
		LowerBodyYaw(0),
		PoseParameters({}),
		AnimationLayers({}),
		Flags(0),
		TickCount(0),
		BoneMatrix({}),
		HeadPos(Source::Vector()),
		LBYUpdated(false),
		IsMoving(false),
		Velocity(Source::Vector()),
		Resolved(false)
	{}

	TickRecord(const Source::Vector& origin, Source::QAngle* angles, float simulation_time, float lower_body_yaw,
		const std::array<float, Source::C_BaseAnimating::NUM_POSEPAREMETERS>& pose_parameters,
		const std::array<Source::C_AnimationLayer, Source::C_BaseAnimatingOverlay::MAX_OVERLAYS>& animation_layers, int flags,
		int tick_count, const std::array<matrix3x4_t, 128>& bone_matrix, const Source::Vector& head_pos, bool lby_updated,
		bool is_moving, const Source::Vector& velocity, bool resolved)
		: Origin(origin),
		  Angles(angles),
		  SimulationTime(simulation_time),
		  LowerBodyYaw(lower_body_yaw),
		  PoseParameters(pose_parameters),
		  AnimationLayers(animation_layers),
		  Flags(flags),
		  TickCount(tick_count),
		  BoneMatrix(bone_matrix),
		  HeadPos(head_pos),
		  LBYUpdated(lby_updated),
		  IsMoving(is_moving),
	      Velocity(velocity),
		  Resolved(resolved)
	{}

	friend bool operator==(const TickRecord& lhs, const TickRecord& rhs)
	{
		return lhs.SimulationTime == rhs.SimulationTime;
	}
	friend bool operator!=(const TickRecord& lhs, const TickRecord& rhs)
	{
		return !(lhs == rhs);
	}

	TickRecord(const TickRecord& other)
		: Origin(other.Origin),
		  Angles(other.Angles),
		  SimulationTime(other.SimulationTime),
		  LowerBodyYaw(other.LowerBodyYaw),
		  PoseParameters(other.PoseParameters),
		  AnimationLayers(other.AnimationLayers),
		  Flags(other.Flags),
		  TickCount(other.TickCount),
		  BoneMatrix(other.BoneMatrix),
		  HeadPos(other.HeadPos),
		  LBYUpdated(other.LBYUpdated),
		  IsMoving(other.IsMoving),
		  Velocity(other.Velocity),
	      Resolved(other.Resolved)
	{}

	TickRecord(TickRecord&& other) noexcept
		: Origin(std::move(other.Origin)),
		  Angles(std::move(other.Angles)),
		  SimulationTime(other.SimulationTime),
		  LowerBodyYaw(other.LowerBodyYaw),
		  PoseParameters(std::move(other.PoseParameters)),
		  AnimationLayers(std::move(other.AnimationLayers)),
		  Flags(other.Flags),
		  TickCount(other.TickCount),
		  BoneMatrix(std::move(other.BoneMatrix)),
		  HeadPos(std::move(other.HeadPos)),
		  LBYUpdated(other.LBYUpdated),
		  IsMoving(other.IsMoving),
		  Velocity(std::move(other.Velocity)),
	      Resolved(other.Resolved)
	{}

	TickRecord& operator=(const TickRecord& other)
	{
		if (this == &other)
			return *this;
		Origin = other.Origin;
		Angles = other.Angles;
		SimulationTime = other.SimulationTime;
		LowerBodyYaw = other.LowerBodyYaw;
		PoseParameters = other.PoseParameters;
		AnimationLayers = other.AnimationLayers;
		Flags = other.Flags;
		TickCount = other.TickCount;
		BoneMatrix = other.BoneMatrix;
		HeadPos = other.HeadPos;
		LBYUpdated = other.LBYUpdated;
		IsMoving = other.IsMoving;
		Velocity = other.Velocity;
		Resolved = other.Resolved;
		return *this;
	}

	TickRecord& operator=(TickRecord&& other) noexcept
	{
		if (this == &other)
			return *this;
		Origin = std::move(other.Origin);
		Angles = std::move(other.Angles);
		SimulationTime = other.SimulationTime;
		LowerBodyYaw = other.LowerBodyYaw;
		PoseParameters = std::move(other.PoseParameters);
		AnimationLayers = std::move(other.AnimationLayers);
		Flags = other.Flags;
		TickCount = other.TickCount;
		BoneMatrix = std::move(other.BoneMatrix);
		HeadPos = std::move(other.HeadPos);
		LBYUpdated = other.LBYUpdated;
		IsMoving = other.IsMoving;
		Velocity = std::move(other.Velocity);
		Resolved = other.Resolved;
		return *this;
	}
};

class F_Resolver : public V_IFeature
{
public:
	F_Resolver()
		: m_iFiredBullets(0), BacktrackedTicks({})
	{}

	void OnFrameStageNotify(Source::ClientFrameStage_t stage) override;
	void OnFrameStageNotifyPostOriginal(Source::ClientFrameStage_t stage) override;
	void OnListenedEvent(Source::IGameEvent* game_event) override;
	void OnCreateMove(Source::CUserCmd* cmd) override;
	void OnPaintTraverse() override;
	void OnLBYProxy(Source::C_CSPlayer* updater, float newValue) override;

	int GetFiredBullets() { return m_iFiredBullets; }
	TickRecord GetBacktrackedTick(int index) { return BacktrackedTicks[index]; }
	void ResetBacktrackedTick(int index) { BacktrackedTicks[index] = TickRecord(); }
	void ResetAllBacktrackedTicks() { BacktrackedTicks.fill(TickRecord()); }

	// Resolver types
	float RESOLVE_SmartYaw(Source::C_CSPlayer* local, Source::C_CSPlayer* target); // Type 0

private:
	int m_iFiredBullets;
	static Source::CUserCmd* UserCmd;
	std::array<TickRecord, MAX_CLIENTS> BacktrackedTicks;
};

struct PlayerRecord
{
	Source::C_CSPlayer* Player;
	int HitgroupHit;
	float HeadshotAngle;
	std::list<float> BlacklistedDeltas;
	ResolveInfo_t ResolveInfo;
	std::deque<TickRecord> Records;
	TickRecord m_prevTickRecord;
	TickRecord m_currTickRecord;
	Source::QAngle m_angMovingDirection;
	int ChokedFrames;
	bool Faking;
	int MissedShots;

	PlayerRecord()
		: Player(nullptr), HitgroupHit(0), HeadshotAngle(0.f),
		BlacklistedDeltas({}), Records({}), m_prevTickRecord(TickRecord()),
		m_currTickRecord(TickRecord()), m_angMovingDirection(Source::QAngle()),
		ChokedFrames(0), Faking(false), MissedShots(0)
	{}

	PlayerRecord(Source::C_CSPlayer* player, int hitgroup_hit, float headshot_angle, const std::list<float>& blacklisted_deltas, 
		const std::deque<TickRecord>& tick_records, const TickRecord& prev_tick, const TickRecord& curr_tick, const Source::QAngle& moving_dir,
		int choked_frames, bool faking, int missed_shots)
		: Player(player),
		  HitgroupHit(hitgroup_hit),
		  HeadshotAngle(headshot_angle),
		  BlacklistedDeltas(blacklisted_deltas),
		  Records(tick_records),
		  m_prevTickRecord(prev_tick),
		  m_currTickRecord(curr_tick),
	      m_angMovingDirection(moving_dir),
	      ChokedFrames(choked_frames),
	      Faking(faking),
		  MissedShots(missed_shots)
	{}
};
extern std::unordered_map<INDEX, PlayerRecord> PlayerList;

class F_Backtrack : public V_IFeature
{
public:
	static void GrabServerVars(); // Grab the list of server convars
	static float GetLerpTime(); // Grab the interpolation time (0 if interpolation is disabled)
	static float GetLatency(); // Get the current latency
	static bool IsTimeValid(float time);
	static bool IsRecordValid(const TickRecord& record);

	F_Backtrack()
		: m_flOldLowerBodyYaw(0.f), m_iMaxBacktrackTick(12), m_iBestHitbox(-1)
	{}
	void StorePlayer(Source::C_CSPlayer* player); // Will add this player to the playerlist
	int GetMaxBacktrackTick() { return m_iMaxBacktrackTick; }
	int GetBacktrackTick() { return m_iTickToBacktrack; }
	void BacktrackToTick(int tick) { m_iTickToBacktrack = tick; }
	void SetBestHitbox(int h) { this->m_iBestHitbox = h; }

	void OnFrameStageNotify(Source::ClientFrameStage_t stage) override;
	void OnPaintTraverse() override;
	void OnCreateMove(Source::CUserCmd* cmd) override;
	void OnListenedEvent(Source::IGameEvent* game_event) override;
private:
	float m_flOldLowerBodyYaw;
	float m_iMaxBacktrackTick;
	static Source::CUserCmd* UserCmd;
	int m_iBestHitbox;
	// Server ConVars
	static Source::ConVar* sv_maxunlag;
	static Source::ConVar* cl_interp_ratio;
	static Source::ConVar* cl_updaterate;
	static Source::ConVar* sv_client_min_interp_ratio;
	static Source::ConVar* sv_client_max_interp_ratio;
	static Source::ConVar* sv_minupdaterate;
	static Source::ConVar* sv_maxupdaterate;
	static Source::ConVar* cl_interp;
	static Source::ConVar* cl_interpolate;
	// Tick count to overwrite
	int m_iTickToBacktrack;
};

extern F_HvH* HvH;
extern F_Resolver* Resolver;
extern F_Backtrack* Backtrack;

}