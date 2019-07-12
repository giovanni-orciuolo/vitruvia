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

#include "../Hooks.h"
#include "../../SourceClasses/C_CSPlayer.h"
#include "../../Tools/EnginePrediction.h"
#include "../../Config/Config.h"
#include "../../Features/HvH/HvH.h"

bool g_bSendPackets = false;

SOURCE_INIT
namespace Hooks {

bool __fastcall hk_CMS_CreateMove(IClientMode* _this, void* edx, float sample_frametime, CUserCmd* cmd)
{
	// Get frame pointer from stack
	uintptr_t* framePointer;
	__asm mov framePointer, ebp;

	// Save the current tick count
	Vitruvia::global->set_CurrentTickCount(cmd->tick_count);

	// Set the viewangles to be the original viewangles so I can avoid stutters on 144hz
	Source::Engine->SetViewAngles(cmd->viewangles);

	if (cmd->command_number == 0 || !Source::Engine->IsInGame())
		return ClientModeTable->GetFunction<CMS_CreateMove_t>(I_CMS_CreateMove)(_this, sample_frametime, cmd);

	// Reset my bSendPackets and reset Aimbotting() global
	g_bSendPackets = true;
	Vitruvia::global->set_Aimbotting(false);

	// Before performing features, I need to prevent Rage.Active and Legit.Active at the same time
	if (Vitruvia::Config::Aimbot.Active && Vitruvia::Config::Ragebot.Active)
		Vitruvia::Config::Ragebot.Active.m_Value = false; // You can't activate Ragebot if Legitbot is active!

	// Adjust cur_time

	// Perform CreateMove before prediction
	// Usually misc features are done before prediction occurs
	for (auto f : Vitruvia::Features)
		f->OnCreateMovePrePrediction(cmd);

	// Get originals (after having modified them through Misc features!)
	QAngle orgViewAngles = cmd->viewangles;
	float orgForward = cmd->forwardmove;
	float orgSide = cmd->sidemove;

	// Apply engine prediction
	//
	Vitruvia::Tools::EnginePrediction->Predict(cmd);

	// Perform CreateMove behaviour
	// Legitbot, Ragebot, a lot of stuff is going on here
	for (auto f : Vitruvia::Features)
		f->OnCreateMove(cmd);

	// After the CreateMove behaviour is done, call Backtrack to overwrite cmd tick count
	cmd->tick_count = Vitruvia::Backtrack->GetBacktrackTick();

	// Fix movement
	Vitruvia::HvH->FixMovement(cmd, orgViewAngles, orgForward, orgSide);

	// End engine prediction
	//
	Vitruvia::Tools::EnginePrediction->EndPrediction(cmd);

	// Perform CreateMove after prediction
	for (auto f : Vitruvia::Features)
		f->OnCreateMovePostPrediction(cmd);

	// Fake my angles if Ragebot is enabled and I'm aimbotting
	if (Vitruvia::Config::Ragebot.Active 
		&& Vitruvia::global->Aimbotting() 
		&& cmd->buttons & IN_ATTACK
		&& Vitruvia::Config::FakeLag.OnShoot) 
	{
		// Set bSendPackets (located at frame - 28) to false
		*(bool*)(*framePointer - 0x1C) = false;
	}
	// Othewrise just set it to be my g_bSendPackets value
	*(bool*)(*framePointer - 0x1C) = g_bSendPackets;

	// Normalize the cmd viewangles before returning
	cmd->viewangles = Maths::NormalizeAngle(cmd->viewangles);

	// Clamp cmd move values before returning
	cmd->forwardmove = clamp<float,float>(cmd->forwardmove, -450.f, 450.f);
	cmd->sidemove = clamp<float,float>(cmd->sidemove, -450.f, 450.f);
	cmd->upmove = clamp<float,float>(cmd->upmove, -320.f, 320.f);

	if (Vitruvia::Config::FakeLag.Active && !g_bSendPackets)
		Vitruvia::global->set_VisualAngles(cmd->viewangles); // The user will see his real angles in TP

	return false;
}

// I don't hook CHLClient::CreateMove anymore because it was a real pain in the ass to access bSendPackets (I had to setup a naked gate)
void __fastcall hk_CreateMove(IBaseClientDLL* _this, void* edx, int sequence_number, float input_sample_frametime, bool active)
{
	// Kept here for legacy purposes
	/*
	// Call the original
	BaseClientTable->GetFunction<CreateMove_t>(I_CreateMove)(_this, sequence_number, input_sample_frametime, active);

	// Grab pointers to cmd and verified cmd based on sequence number
	CUserCmd* pCmd = Source::Input->GetUserCmd(sequence_number);
	CVerifiedUserCmd* pVerifiedCmd = Source::Input->GetVerifiedUserCmd(sequence_number);

	// Validity checks
	if (pCmd && pVerifiedCmd)
	{
		// Before performing features, I need to prevent Rage.Active and Legit.Active at the same time
		if (Vitruvia::Config::Aimbot.Active && Vitruvia::Config::Ragebot.Active)
			Vitruvia::Config::Ragebot.Active.m_Value = false; // You can't activate Ragebot if Legitbot is active!

		// Perform CreateMove before prediction
		for (auto f : Vitruvia::Features)
			f->OnCreateMovePrePrediction(pCmd);

		// Apply engine prediction
		//
		Vitruvia::Tools::EnginePrediction->Predict(pCmd);

		// Perform CreateMove behaviour
		for (auto f : Vitruvia::Features)
			f->OnCreateMove(pCmd);

		// End engine prediction
		//
		Vitruvia::Tools::EnginePrediction->EndPrediction(pCmd);

		// Perform CreateMove after prediction
		for (auto f : Vitruvia::Features)
			f->OnCreateMovePostPrediction(pCmd);

		// Normalize the cmd viewangles before validating
		pCmd->viewangles = Maths::NormalizeAngle(pCmd->viewangles);

		// Clamp cmd move values before validating
		pCmd->forwardmove = clamp<float, float>(pCmd->forwardmove, -450.f, 450.f);
		pCmd->sidemove = clamp<float, float>(pCmd->sidemove, -450.f, 450.f);
		pCmd->upmove = clamp<float, float>(pCmd->upmove, -320.f, 320.f);

		// Validate command through CRC
		Source::Input->ValidateCommand(pCmd, pVerifiedCmd);
	}
	*/
}

}
SOURCE_END