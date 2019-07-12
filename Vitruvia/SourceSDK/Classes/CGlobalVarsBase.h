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
#include "../SourceCommon.h"

SOURCE_INIT

class CGlobalVarsBase
{
public:
	// Absolute time (per frame still - Use Plat_FloatTime() for a high precision real time 
	//  perf clock, but not that it doesn't obey host_timescale/host_framerate)
	float			real_time;
	// Absolute frame counter
	int				frame_count;
	// Non-paused frametime
	float			absolute_frametime;
	float			absolute_framestarttimestddev;
	// Current time 
	//
	// On the client, this (along with tickcount) takes a different meaning based on what
	// piece of code you're in:
	// 
	//   - While receiving network packets (like in PreDataUpdate/PostDataUpdate and proxies),
	//     this is set to the SERVER TICKCOUNT for that packet. There is no interval between
	//     the server ticks.
	//     [server_current_Tick * tick_interval]
	//
	//   - While rendering, this is the exact client clock 
	//     [client_current_tick * tick_interval + interpolation_amount]
	//
	//   - During prediction, this is based on the client's current tick:
	//     [client_current_tick * tick_interval]
	float			cur_time;
	// Time spent on last server or client frame (has nothing to do with think intervals)
	float			frame_time;
	// current maxplayers setting
	int				max_clients;
	// Simulation ticks
	int				tick_count;
	// Simulation tick interval
	float			interval_per_tick;
	// interpolation amount ( client-only ) based on fraction of next tick which has elapsed
	float			interpolation_amount;
	int				sim_ticks_this_frame;
	int				network_protocol;
	// current saverestore data
	void*			save_data;
	// Set to true in client code.
	bool			is_clientcode;
	bool			is_remoteclient;
	// 100 (i.e., tickcount is rounded down to this base and then the "delta" from this base is networked
	int				timestamp_networking_base;
	// 32 (entindex() % timestamp_randomize_window ) is subtracted from gpGlobals->tick_count to set the networking basis, prevents
	//  all of the entities from forcing a new PackedEntity on the same tick (i.e., prevents them from getting lockstepped on this)
	int				timestamp_randomize_window;
};

SOURCE_END