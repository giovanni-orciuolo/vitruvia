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

#include "../Vitruvia.h"
#include "../SourceClasses/C_CSPlayer.h"

#define EnginePrediction CPredictionSystem::GetInstance()

namespace Vitruvia
{
	namespace Tools
	{
		class CPredictionSystem {
			IMPLEMENT_SINGLETON_INITIALIZED(CPredictionSystem, Setup);
		public:
			void Setup() {
				m_iFlagsBackup = 0;
				m_iButtonsBackup = 0;
				m_pPredictionRandomSeed = nullptr;
				m_pSetPredictionPlayer = nullptr;
				m_iTickBaseBackup = 0;
			}
			void Predict(Source::CUserCmd* cmd);
			void EndPrediction(Source::CUserCmd* cmd);
		private:
			int m_iFlagsBackup;
			int m_iButtonsBackup;
			offset_t* m_pPredictionRandomSeed;
			offset_t* m_pSetPredictionPlayer;
			int m_iTickBaseBackup;
		};
	}
}