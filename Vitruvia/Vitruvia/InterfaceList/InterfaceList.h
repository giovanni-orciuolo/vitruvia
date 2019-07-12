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
#include "../SourceClasses/C_CSPlayerResource.h"
#include "../SourceClasses/C_CSGameRulesProxy.h"
#include "../SourceClasses/C_PlayerInventory.h"

#define IFACE_EX(type, name) extern type* name;
#define IFACE(type, name) type* name = nullptr;

SOURCE_INIT

typedef void*(*InstantiateInterfaceFn)();
// Used internally to register classes.
class InterfaceReg
{
public:
	InterfaceReg(InstantiateInterfaceFn fn, const char *pName);
public:
	InstantiateInterfaceFn	m_CreateFn;
	const char				*m_pName;
	InterfaceReg			*m_pNext; // For the global list.
};

IFACE_EX(IBaseClientDLL, BaseClient)
IFACE_EX(IClientEntityList, EntityList)
IFACE_EX(ICvar, CVar)
IFACE_EX(IGameMovement, GameMovement)
IFACE_EX(IPrediction, Prediction)
IFACE_EX(IEngineTrace, EngineTrace)
IFACE_EX(IGameEventManager2, EventManager)
IFACE_EX(IVDebugOverlay, DebugOverlay)
IFACE_EX(IVEngineClient, Engine)
IFACE_EX(IVModelCache, ModelCache)
IFACE_EX(IVModelInfo, ModelInfo)
IFACE_EX(IVModelRender, ModelRender)
IFACE_EX(IPhysicsSurfaceProps, Physics)
IFACE_EX(IVRenderView, RenderView)
IFACE_EX(IInputSystem, InputSystem)
IFACE_EX(IMaterialSystem, MaterialSystem)
IFACE_EX(VGUI::IPanel, Panel)
IFACE_EX(VGUI::ISurface, Surface)
IFACE_EX(CGlobalVarsBase, Globals)
IFACE_EX(IClientMode, ClientMode)
IFACE_EX(Input_t, Input)
IFACE_EX(CClientState, ClientState)
IFACE_EX(C_CSPlayerResource, PlayerResource)
IFACE_EX(C_CSGameRulesProxy, GameRules)
IFACE_EX(ILocalize, Localize)
IFACE_EX(ISteamHTTP, SteamHTTP) // Not implemented!
IFACE_EX(IViewRender, ViewRender)
IFACE_EX(CCSWeaponSystem, CSWeaponSystem)
IFACE_EX(IMoveHelper, MoveHelper)
IFACE_EX(IEngineSound, EngineSound)
IFACE_EX(IViewRenderBeams, ViewRenderBeams)
IFACE_EX(VGUI::CCenterPrint, CenterPrint)
IFACE_EX(CRender, Render)
IFACE_EX(C_PlayerInventory, PlayerInventory)

// Finds InterfaceList
extern InterfaceReg* FindInterfaceList(const std::string& module_name);
// Setup interfaces
extern void SetupInterfaces();
// Returns a vector of InterfaceRegs easy to use xd
extern std::vector<InterfaceReg*> DumpInterfaces(const std::string& module_name);

SOURCE_END