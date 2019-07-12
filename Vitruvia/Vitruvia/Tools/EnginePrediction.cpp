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

//~ ~ ~ ~  Engine Prediction  ~ ~ ~ ~//
/*
Credits:

Coded by: (c)Valve
Referenced: zbe
Reversed by: _xAE^, Flaw
with help from pre-existing reversed material

Summary:
Engine Prediction is the functionality of RunCommand reversed and used during Createmove to get higher accuracy while moving at high velocities.
It basically predicts our own next moves (as localplayer)

What does the code do:
-Back up m_fFlags
-Back up cmd->buttons
-Set m_pCurrentCommand to cmd
-Set SetHost to Localplayer
-Set PredictionRandomSeed to the MD5 of cmd->command_number & 0x7FFFFFFF
-Set PredictionPlayer to localplayer
-Set curtime to tickbase * interval_per_tick
-Set frametime to interval_per_tick
-Set m_nButtons |= (localplayer) + 0x3301
-Check for cmd->impulse and if true, set m_nImpulse to cmd->impulse
-Set C_MoveData.m_nButtons to cmd->buttons
-Create a new integer named buttonsChanged and set it to cmd->buttons ^(xor) (local) + 0x31E8
-Set (localplayer) + 0x31DC to (localplayer) + 0x31E8
-Set (localplayer) + 0x31E8 to cmd->buttons
-Set (localplayer) + 0x31E0 to cmd->buttons & buttonsChanged
-Set (localplayer) + 0x31E4 to buttonsChanged & ~cmd->buttons
-Create a static char to store movedata and use calloc or malloc and use the sizeof(CMoveData) for it's memory allocation
-Set StartTrackPredictionErrors to localplayer
-Back up Tickbase
-Set SetupMove to use the static char that we made earlier that allocates the memory of the SizeOf(CMoveData)
-Set ProcessMove to the same as SetupMove
-Set FinishMove to the same as SetupMove
-Override tickbase to saved tickbase
-Set FinishTrackPredictionErrors to localplayer
-Set m_pCurrentCommand to nullptr
-Set PredictionRandomSeed to -1
-Set PredictionPlayer to 0
-Set SetHost to 0
-Set m_fFlags to backup m_fFlags
-Set cmd->buttons to backup cmd->buttons

References:
https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/game/shared/baseplayer_shared.cpp#L753
zbe's TF2 Base

Special thanks to:
zbe		 - Notifying me of errors
friendly - Helping me with a signature (Was reading mov by accident)
Heep     - Helped me overall with some things
*/

#include "EnginePrediction.h"
using namespace Source;

static char* cMoveData = nullptr;

namespace Vitruvia
{
	namespace Tools
	{
		float GetCurTime(CUserCmd* ucmd, C_BasePlayer* local) 
		{
			static int s_tick = 0;
			static CUserCmd* last_cmd = nullptr;
			if (!last_cmd || last_cmd->hasbeenpredicted) {
				s_tick = local->m_nTickBase();
			}
			else {
				// Required because prediction only runs on frames, not ticks
				// So if your framerate goes below tickrate, m_nTickBase won't update every tick
				++s_tick;
			}
			last_cmd = ucmd;
			return s_tick * Globals->interval_per_tick;
		}

		// -----------------
		// Starts prediction routine
		// -----------------
		void CPredictionSystem::Predict(Source::CUserCmd* cmd)
		{
			auto local = C_CSPlayer::GetLocal();
			if (local)
			{
				//Nullptr'ing is fun!
				if (!Source::MoveHelper || !cmd)
					return;

				CMoveData C_MoveData;

				//Let's back things up for later use
				m_iFlagsBackup = local->m_fFlags();
				m_iButtonsBackup = cmd->buttons;

				//Set m_pCurrentCommand to cmd. We'll set this to nullptr later.
				*local->m_pCurrentCommand() = cmd;

				//Let's get some signatures now
				if (!m_pPredictionRandomSeed || !m_pSetPredictionPlayer)
				{
					m_pPredictionRandomSeed = *reinterpret_cast<offset_t**>(
						FindPattern(CLIENT_DLL, enc("A3 ? ? ? ? 66 0F 6E 86")) + 1);
					m_pSetPredictionPlayer = *reinterpret_cast<offset_t**>(
						FindPattern(CLIENT_DLL, enc("89 35 ? ? ? ? F3 0F 10 48")) + 2);
				}

				//We don't want to be calling this every tick, bad for framerate.
				//Thanks, zbe and Heep
				if (!cMoveData)
					cMoveData = (char*)(calloc(1, sizeof(CMoveData)));

				//Let's set it up so that it predicts ourselves
				Source::MoveHelper->SetHost(local);
				*m_pPredictionRandomSeed = MD5::MD5_PseudoRandom(cmd->command_number) & 0x7FFFFFFF;
				*m_pSetPredictionPlayer = uintptr_t(local);

				//Let's set some global variables
				Source::Globals->cur_time = GetCurTime(cmd, local);//local->m_nTickBase() * Source::Globals->interval_per_tick;
				Source::Globals->frame_time = Source::Globals->interval_per_tick;

				//Don't know is for exactly, but here's the relevant IDA picture of it. https://i.imgur.com/hT6caQV.png
				cmd->buttons |= *reinterpret_cast< uint8_t* >(uintptr_t(local) + 0x3301);

				//This is for flashlights in older Source games, Thanks, friendly for the info
				if (cmd->impulse)
					*reinterpret_cast< uint8_t* >(uintptr_t(local) + 0x31EC) = cmd->impulse;

				//Here we're doing CBasePlayer::UpdateButtonState
				C_MoveData.m_nButtons = cmd->buttons;
				int buttonsChanged = cmd->buttons ^ *reinterpret_cast<int*>(uintptr_t(local) + 0x31E8);
				*reinterpret_cast<int*>(uintptr_t(local) + 0x31DC) = (uintptr_t(local) + 0x31E8);
				*reinterpret_cast<int*>(uintptr_t(local) + 0x31E8) = cmd->buttons;
				*reinterpret_cast<int*>(uintptr_t(local) + 0x31E0) = cmd->buttons & buttonsChanged;  //m_afButtonPressed ~ The changed ones still down are "pressed"
				*reinterpret_cast<int*>(uintptr_t(local) + 0x31E4) = buttonsChanged & ~cmd->buttons; //m_afButtonReleased ~ The ones not down are "released"

				Source::GameMovement->StartTrackPredictionErrors(local);

				m_iTickBaseBackup = local->m_nTickBase();

				Source::Prediction->SetupMove(local, cmd, Source::MoveHelper, reinterpret_cast< CMoveData* >(cMoveData));
				Source::GameMovement->ProcessMovement(local, reinterpret_cast< CMoveData* >(cMoveData));
				Source::Prediction->FinishMove(local, cmd, reinterpret_cast< CMoveData* >(cMoveData));
			}
		}
		void CPredictionSystem::EndPrediction(Source::CUserCmd* cmd)
		{
			auto local = C_CSPlayer::GetLocal();
			if (local)
			{
				//Let's override our tickbase with the backed up tickbase
				*local->m_nTickBasePtr() = m_iTickBaseBackup;

				Source::GameMovement->FinishTrackPredictionErrors(local);

				//Let's nullify these here
				*local->m_pCurrentCommand() = nullptr;
				*m_pPredictionRandomSeed = -1;
				*m_pSetPredictionPlayer = NULL;
				Source::MoveHelper->SetHost(nullptr);

				//Last but not least, set these to their backups 
				*local->m_fFlagsPtr() = m_iFlagsBackup;
				//cmd->buttons = m_iButtonsBackup; DO NOT SET BUTTONS TO BACKUP! This is cuz I'm modifying them
			}
		}
	}
}