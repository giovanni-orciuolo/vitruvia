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

#include "Offsets.h"

// Sigs that I'm not using
#define SIG_TraceToExit		*rcast<offset_t*>(FindPattern(ENGINE_DLL, enc("53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 81 EC ? ? ? ? 8B 43 10"))
#define SIG_InterfaceList   FindPattern(CLIENT_DLL, enc("8B 35 ? ? ? ? 57 85 F6 74 ? 8B 7D 08 8B 4E 04 8B C7 8A 11 3A 10")) - CLIENT_BASE // Retrieves s_pInterfaceRegs only for client.dll
#define SIG_ClientState		*rcast<offset_t*>(FindPattern(ENGINE_DLL, enc("A1 ? ? ? ? 33 D2 6A 00 6A 00 33 C9 89 B0")) + 1) - ENGINE_BASE
#define SIG_LoadFromBuffer  FindPattern(CLIENT_DLL, enc("E8 ? ? ? ? 80 7D F8 00 ? ? 85 DB")); // ptr to KeyValues::LoadFromBuffer
#define SIG_HUD				*rcast<offset_t*>(FindPattern(CLIENT_DLL, enc("B9 ? ? ? ? 0F 94 C0 0F B6 C0 50 68")) + 1)
#define SIG_HUD_FindElement	FindPattern(CLIENT_DLL, enc("55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28"))
#define SIG_SvTeamIdOverheadReturn  FindPattern(CLIENT_DLL, enc("85 C0 0F 84 ? ? ? ? 8B 0D ? ? ? ? 81 F9 ? ? ? ? 75 0F"))
#define SIG_SendPackets				(FindPattern(ENGINE_DLL, enc("B3 01 8B 01 8B 40 10 FF D0 84 C0 74 0F 80 BF ? ? ? ? ? 0F 84")) + 1) - ENGINE_BASE;

// Sigs currently being scanned by pCheat xd
#define SIG_PlayerResource	*rcast<offset_t*>(FindPattern(CLIENT_DLL, enc("8B 3D ? ? ? ? 85 FF 0F 84 ? ? ? ? 81 C7")) + 2) - CLIENT_BASE
#define SIG_GameRules		*rcast<offset_t*>(FindPattern(CLIENT_DLL, enc("A1 ? ? ? ? 85 C0 0F 84 ? ? ? ? 80 B8 ? ? ? ? ? 0F 84 ? ? ? ? 0F 10 05")) + 1) - CLIENT_BASE
#define SIG_LGTS			FindPattern(CLIENT_DLL, enc("55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0")) // LineGoesThroughSmoke ptr
#define SIG_D3D_Device		**rcast<offset_t**>(FindPattern(enc("shaderapidx9.dll"), enc("A1 ? ? ? ? 50 8B 08 FF 51 0C")) + 1); // d3d device ptr
#define SIG_GetItemName		FindPattern(CLIENT_DLL, enc("55 8B EC 56 8B F1 E8 ? ? ? ? 8B 8E"))
#define SIG_RankRevealAll	FindPattern(CLIENT_DLL, enc("55 8B EC 8B 0D ? ? ? ? 68"))
#define SIG_GlowObject		*rcast<offset_t**>(FindPattern(CLIENT_DLL, enc("0F 11 05 ? ? ? ? 83 C8 01")) + 3)
#define SIG_SmokeCount		*rcast<offset_t*>(FindPattern(CLIENT_DLL, enc("A3 ? ? ? ? 57 8B CB")) + 1)
#define SIG_IsReady			FindPattern(CLIENT_DLL, enc("55 8B EC 83 E4 F8 83 EC 08 56 8B 35 ? ? ? ? 57 83 BE"))
#define SIG_SetClanTag		FindPattern(ENGINE_DLL, enc("53 56 57 8B DA 8B F9 FF 15")) - ENGINE_BASE
#define SIG_IsEntityBreakable		FindPattern(CLIENT_DLL, enc("55 8B EC 51 56 8B F1 85 F6 74 68"))
#define SIG_HandleBulletPenetration FindPattern(CLIENT_DLL, enc("53 8B DC 83 EC 08 83 E4 F8 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 83 EC 78 8B 53 14"))
#define SIG_ClipTraceToPlayers		FindPattern(CLIENT_DLL, enc("53 8B DC 83 EC ? 83 E4 ? 83 C4 ? 55 8B 6B 04 89 6C 24 ? 8B EC 81 EC ? ? ? ? 8B 43 10 56 57 52 F3 0F 10 40"))
#define SIG_CustomMaterials			*rcast<offset_t*>(FindPattern(CLIENT_DLL, enc("..83 BE ? ? ? ? ? 7F 67") + 2)) - 12 //83
#define SIG_CustomMaterialInit		*rcast<offset_t*>(FindPattern(CLIENT_DLL, enc("C6 86 ? ? ? ? ? FF 50 04")) + 2)
#define SIG_GetSequenceActivity		FindPattern(CLIENT_DLL, enc("55 8B EC 83 7D 08 FF 56 8B F1 74 3D"))
#define SIG_CreateAnimationState	FindPattern(CLIENT_DLL, enc("55 8B EC 56 8B F1 B9 ? ? ? ? C7 46"))
#define SIG_ResetAnimationState		FindPattern(CLIENT_DLL, enc("56 6A 01 68 ? ? ? ? 8B F1"))
#define SIG_UpdateAnimationState    FindPattern(CLIENT_DLL, enc("55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24"))
#define SIG_CL_SendMove				FindPattern(ENGINE_DLL, enc("55 8B EC A1 ? ? ? ? 81 EC ? ? ? ? B9 ? ? ? ? 53 8B 98"))
#define SIG_WriteUserCmd			FindPattern(CLIENT_DLL, enc("55 8B EC 83 E4 F8 51 53 56 8B D9 8B 0D"))

namespace Vitruvia {
namespace Offsets {

OFFSET(PlayerResource)
OFFSET(GameRules)
OFFSET(LineGoesThroughSmoke)
OFFSET(D3D_Device)
OFFSET(GetItemName)
OFFSET(RankRevealAll)
OFFSET_PTR(GlowObject)
OFFSET(SmokeCount)
OFFSET(IsReady)
OFFSET(SetClanTag)
OFFSET(IsEntityBreakable)
OFFSET(HandleBulletPenetration)
OFFSET(ClipTraceToPlayers)
OFFSET(CustomMaterials)
OFFSET(CustomMaterialInitialized)
OFFSET(GetSequenceActivity)
OFFSET(CreateAnimationState)
OFFSET(ResetAnimationState)
OFFSET(UpdateAnimationState)
OFFSET(CL_SendMove)
OFFSET(WriteUserCmd)

void LOG_OFFSET(const std::string& module_name, const std::string& offset_name, const offset_t offset_value, bool skip_base)
{
#ifdef ENABLE_CONSOLE
	if (!skip_base)
		Log(enc("%s found! --> [0x%X ~ 0x%X]"), offset_name.c_str(), offset_value, MODULE_BASE(module_name.c_str()) + offset_value);
	else
		Log(enc("%s found! --> [0x%X]"), offset_name.c_str(), offset_value);
#endif
}

void ScanOffsets()
{
	Log(enc("Starting offsets scan..."));

	PlayerResource = SIG_PlayerResource;
	LOG_OFFSET(CLIENT_DLL, enc("PlayerResource"), PlayerResource, false);

	GameRules = SIG_GameRules;
	LOG_OFFSET(CLIENT_DLL, enc("GameRules"), GameRules, false);

	LineGoesThroughSmoke = SIG_LGTS;
	LOG_OFFSET(CLIENT_DLL, enc("LineGoesThroughSmoke"), LineGoesThroughSmoke);

	D3D_Device = SIG_D3D_Device;
	LOG_OFFSET(enc("shaderapidx9.dll"), enc("D3D_Device"), D3D_Device);

	GetItemName = SIG_GetItemName;
	LOG_OFFSET(CLIENT_DLL, enc("GetItemName"), GetItemName);

	RankRevealAll = SIG_RankRevealAll;
	LOG_OFFSET(CLIENT_DLL, enc("RankRevealAll"), RankRevealAll);

	GlowObject = SIG_GlowObject;
	LOG_OFFSET(CLIENT_DLL, enc("GlowObject"), rcast<offset_t>(GlowObject));

	SmokeCount = SIG_SmokeCount;
	LOG_OFFSET(CLIENT_DLL, enc("SmokeCount"), SmokeCount);

	IsReady = SIG_IsReady;
	LOG_OFFSET(CLIENT_DLL, enc("IsReady"), IsReady);

	SetClanTag = SIG_SetClanTag;
	LOG_OFFSET(ENGINE_DLL, enc("SetClanTag"), SetClanTag, false);

	IsEntityBreakable = SIG_IsEntityBreakable;
	LOG_OFFSET(CLIENT_DLL, enc("IsEntityBreakable"), IsEntityBreakable);

	HandleBulletPenetration = SIG_HandleBulletPenetration;
	LOG_OFFSET(CLIENT_DLL, enc("HandleBulletPenetration"), HandleBulletPenetration);

	ClipTraceToPlayers = SIG_ClipTraceToPlayers;
	LOG_OFFSET(CLIENT_DLL, enc("ClipTraceToPlayers"), ClipTraceToPlayers);

	CustomMaterials = SIG_CustomMaterials;
	LOG_OFFSET(CLIENT_DLL, enc("CustomMaterials"), CustomMaterials);

	CustomMaterialInitialized = SIG_CustomMaterialInit;
	LOG_OFFSET(CLIENT_DLL, enc("CustomMaterialInitialized"), CustomMaterialInitialized);

	GetSequenceActivity = SIG_GetSequenceActivity;
	LOG_OFFSET(CLIENT_DLL, enc("GetSequenceActivity"), GetSequenceActivity);

	CreateAnimationState = SIG_CreateAnimationState;
	LOG_OFFSET(CLIENT_DLL, enc("CreateAnimationState"), CreateAnimationState);

	ResetAnimationState = SIG_ResetAnimationState;
	LOG_OFFSET(CLIENT_DLL, enc("ResetAnimationState"), ResetAnimationState);

	UpdateAnimationState = SIG_UpdateAnimationState;
	LOG_OFFSET(CLIENT_DLL, enc("UpdateAnimationState"), UpdateAnimationState);

	CL_SendMove = SIG_CL_SendMove;
	LOG_OFFSET(ENGINE_DLL, enc("CL_SendMove"), CL_SendMove);

	WriteUserCmd = SIG_WriteUserCmd;
	LOG_OFFSET(CLIENT_DLL, enc("WriteUserCmd"), WriteUserCmd);

	Log(enc("Done."));
}

}
}
