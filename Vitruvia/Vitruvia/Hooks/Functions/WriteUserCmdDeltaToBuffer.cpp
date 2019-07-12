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

int g_iTickBaseShift = 0;

SOURCE_INIT
namespace Hooks
{

template<typename R, typename C, typename D, typename ...Args>
R __declspec(naked) __cdecl weirdCall(void* func, C ecx, D edx, Args... args) {
    __asm {
        push ebp
        mov ebp, esp
    }
    reinterpret_cast<R(__fastcall*)(C, D, Args...)>(func)(ecx, edx, args...);
    __asm {
        mov esp, ebp
        pop ebp
        ret
    }
}

template<typename R, typename C>
R weirdCall(void* func, C ecx) {
    return weirdCall(func, ecx, nullptr);
}

template<typename T>
struct WeirdFunc;

template<typename R, typename ...Args>
struct WeirdFunc<R(Args...)> {
    __forceinline R operator()(Args... args) {
        return weirdCall<R, Args...>(this, args...);
    }
};

bool inSendMove = false, firstSendMovePack = false;
using CL_SendMove_t = void(__fastcall*)();
using WriteUserCmd_t = void(__fastcall*)(void*, CUserCmd*, CUserCmd*); // WRONG! WriteUserCmd uses a "special convention"
//https://www.unknowncheats.me/forum/1973111-post73.html

const int MAX_USERCMD_LOSS = 10;
const int MAX_USERCMDS_SEND = 62;

void WriteUserCmd(void* buf, CUserCmd* to, CUserCmd* from)
{
	static auto WriteUserCmdFn = reinterpret_cast<WriteUserCmd_t>(Vitruvia::Offsets::WriteUserCmd);
	__asm
	{
		mov ecx, buf;
		mov edx, to;
		push from;
		call WriteUserCmdFn;
		add esp, 4h;
	}
}

bool __fastcall hk_WriteUserCmdDeltaToBuffer(IBaseClientDLL* _this, void* edx, int slot, void* buf, int from, int to, bool is_new_cmd)
{
	using namespace Vitruvia;
	if (g_iTickBaseShift <= 0)
	{
		// I don't need to shift, call the original normally
		return BaseClientTable->GetFunction<WriteUserCmdDeltaToBuffer_t>(I_WriteUserCmdDeltaToBuffer)(_this, slot, buf, from, to, is_new_cmd);
	}
	if (from != -1)
	{
		// Skip this but send it no matter what
		return true;
	}

	auto msg = reinterpret_cast<CCLCMsg_Move_t*>(getStackBase() + 0xFCC);
	auto netChan = ClientState->m_NetChannel;
	auto CL_SendMove = rcast<CL_SendMove_t>(Offsets::CL_SendMove);
	int newCommands = msg->numNewCommands;

	// Call CL_SendMove multiple times - split fake move commands between packets to bypass 62 limit
	if (!inSendMove) 
	{
		if (newCommands <= 0)
			return false;
		inSendMove = true;
		firstSendMovePack = true;
		g_iTickBaseShift += newCommands;

		while (g_iTickBaseShift > 0) 
		{
			CL_SendMove();
			netChan->Transmit(false);
			firstSendMovePack = false;
		}

		inSendMove = false;
		return false; // Stop current CL_SendMove from sending
	}

	// Pack 10 more ticks by faking packet loss

	if (!firstSendMovePack) 
	{
		int loss = min(g_iTickBaseShift, MAX_USERCMD_LOSS);
		g_iTickBaseShift -= loss;
		netChan->m_nOutSequenceNr += loss;
	}

	// Manipulate CLC_Move

	int nextCmdNr = ClientState->m_nLastOutgoingCommand + ClientState->m_nChokedCommands + 1;
	int totalNewCommands = min(g_iTickBaseShift, MAX_USERCMDS_SEND);
	g_iTickBaseShift -= totalNewCommands;

	from = -1;
	msg->numNewCommands = totalNewCommands;
	msg->numBackupCommands = 0;

	// Write real commands

	for (to = nextCmdNr - newCommands + 1; to <= nextCmdNr; to++) 
	{
		if (!BaseClientTable->GetFunction<WriteUserCmdDeltaToBuffer_t>(I_WriteUserCmdDeltaToBuffer)(_this, slot, buf, from, to, is_new_cmd)) 
		{
			return false;
		}
		from = to;
	}

	// Write fake commands

	CUserCmd* lastRealCmd = Input->GetUserCmd(slot, from);
	CUserCmd fromCmd;
	if (lastRealCmd)
		fromCmd = *lastRealCmd;

	CUserCmd toCmd = fromCmd;
	toCmd.command_number++;
	toCmd.tick_count += 200; // Prevent server from executing fake commands sometimes

	for (int i = newCommands; i <= totalNewCommands; i++) 
	{
		WriteUserCmd(buf, &toCmd, &fromCmd);
		fromCmd = toCmd;
		toCmd.command_number++;
		toCmd.tick_count++;
	}
	return true;
}

}
SOURCE_END