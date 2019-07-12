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

#include "InterfaceList.h"

#define IFACE_GET(iface, module, iface_name, type) iface = GetInterface<type>(module, iface_name)

SOURCE_INIT

// Little helper function that I use to log interfaces that I don't grab using GetInterface()
auto LOG_INTERFACE(const std::string& module_name, const std::string& interface_name, void* interface_ptr) -> void
{
	if (interface_ptr == nullptr)
	{
		Err(enc("%s not found in %s"), interface_name.c_str(), module_name.c_str());
	}
	else
	{
		Log(enc("Found %s [0x%X] in %s"), interface_name.c_str(), reinterpret_cast<DWORD>(interface_ptr), module_name.c_str());
	}
}

InterfaceReg::InterfaceReg(InstantiateInterfaceFn fn, const char* pName) :
	m_pName(pName)
{
	m_CreateFn = fn;
	m_pNext = this;
}

template<typename T>
T* GetInterface(const std::string& strModuleName, std::string strInterfaceName)
{
	// So I can store the ptr
	T* Interface = nullptr;

	// HACKY SHIT INBOUND!
	// Basically, if the interface name contains no numbers, there is a risk that the loop grabs the wrong interface
	// For example "VEngineClient" may grab "VEngineClientStringTable00X" instead of the actual shit
	// So, I just have to append a 0 to the interface name if there are no numbers inside the interface name already
	if (strInterfaceName.find_first_of(enc("0123456789")) == std::string::npos)
		strInterfaceName.append(enc("0"));

	// Find the beginning of interface list for the module
	auto interface_list = FindInterfaceList(strModuleName);
	if (!interface_list)
	{
		Err(enc("InterfaceList head is nullptr in %s"), strModuleName.c_str());
		return nullptr;
	}

	// For each interface
	for (auto ir = interface_list; ir; ir = ir->m_pNext)
	{
		// If the name is contained in the string
		if (strstr(ir->m_pName, strInterfaceName.c_str()))
		{
			// Interface found!
			Interface = (T*)ir->m_CreateFn();
			Log(enc("Found %s [0x%X] in %s"), ir->m_pName, reinterpret_cast<DWORD>(Interface), strModuleName.c_str());
			return Interface;
		}
	}

	// Interface not found
	Err(enc("%s not found in %s"), strInterfaceName.c_str(), strModuleName.c_str());
	return nullptr;
};

IFACE(IBaseClientDLL, BaseClient)
IFACE(IClientEntityList, EntityList)
IFACE(ICvar, CVar)
IFACE(IGameMovement, GameMovement)
IFACE(IPrediction, Prediction)
IFACE(IEngineTrace, EngineTrace)
IFACE(IGameEventManager2, EventManager)
IFACE(IVDebugOverlay, DebugOverlay)
IFACE(IVEngineClient, Engine)
IFACE(IVModelCache, ModelCache)
IFACE(IVModelInfo, ModelInfo)
IFACE(IVModelRender, ModelRender)
IFACE(IPhysicsSurfaceProps, Physics)
IFACE(IVRenderView, RenderView)
IFACE(IInputSystem, InputSystem)
IFACE(IMaterialSystem, MaterialSystem)
IFACE(VGUI::IPanel, Panel)
IFACE(VGUI::ISurface, Surface)
IFACE(CGlobalVarsBase, Globals)
IFACE(IClientMode, ClientMode)
IFACE(Input_t, Input)
IFACE(CClientState, ClientState)
IFACE(C_CSPlayerResource, PlayerResource)
IFACE(C_CSGameRulesProxy, GameRules)
IFACE(ILocalize, Localize)
IFACE(ISteamHTTP, SteamHTTP) // Not implemented!
IFACE(IViewRender, ViewRender)
IFACE(CCSWeaponSystem, CSWeaponSystem)
IFACE(IMoveHelper, MoveHelper)
IFACE(IEngineSound, EngineSound)
IFACE(IViewRenderBeams, ViewRenderBeams)
IFACE(VGUI::CCenterPrint, CenterPrint)
IFACE(CRender, Render)
IFACE(C_PlayerInventory, PlayerInventory)

InterfaceReg* FindInterfaceList(const std::string& module_name)
{
	// Working
	void* create_interface = GetProcAddress(GetModuleHandle(module_name.c_str()), enc("CreateInterface"));
	// If CreateInterface isn't found, return nullptr
	if (create_interface == nullptr)
		return nullptr;
	size_t jmp_instruction = (size_t)(create_interface) + 4;
	size_t jmp_target = jmp_instruction + *(size_t*)(jmp_instruction + 1) + 5;
	InterfaceReg* interface_list = **(InterfaceReg***)(jmp_target + 6);
	return interface_list;
}
void SetupInterfaces()
{
	// Get interface by name
	IFACE_GET(BaseClient, enc("client.dll"), enc("VClient"), IBaseClientDLL);
	IFACE_GET(EntityList, enc("client.dll"), enc("VClientEntityList"), IClientEntityList);
	IFACE_GET(CVar, enc("vstdlib.dll"), enc("VEngineCvar"), ICvar);
	IFACE_GET(GameMovement, enc("client.dll"), enc("GameMovement001"), IGameMovement);
	IFACE_GET(Prediction, enc("client.dll"), enc("VClientPrediction"), IPrediction);
	IFACE_GET(EngineTrace, enc("engine.dll"), enc("EngineTraceClient"), IEngineTrace);
	IFACE_GET(EventManager, enc("engine.dll"), enc("GAMEEVENTSMANAGER002"), IGameEventManager2);
	IFACE_GET(DebugOverlay, enc("engine.dll"), enc("VDebugOverlay"), IVDebugOverlay);
	IFACE_GET(Engine, enc("engine.dll"), enc("VEngineClient"), IVEngineClient);
	IFACE_GET(ModelCache, enc("datacache.dll"), enc("MDLCache004"), IVModelCache);
	IFACE_GET(ModelInfo, enc("engine.dll"), enc("VModelInfoClient"), IVModelInfo);
	IFACE_GET(ModelRender, enc("engine.dll"), enc("VEngineModel"), IVModelRender);
	IFACE_GET(Physics, enc("vphysics.dll"), enc("VPhysicsSurfaceProps"), IPhysicsSurfaceProps);
	IFACE_GET(RenderView, enc("engine.dll"), enc("VEngineRenderView"), IVRenderView);
	IFACE_GET(InputSystem, enc("inputsystem.dll"), enc("InputSystemVersion"), IInputSystem);
	IFACE_GET(MaterialSystem, enc("materialsystem.dll"), "VMaterialSystem", IMaterialSystem);
	IFACE_GET(Panel, enc("vgui2.dll"), enc("VGUI_Panel"), VGUI::IPanel);
	IFACE_GET(Surface, enc("vguimatsurface.dll"), enc("VGUI_Surface"), VGUI::ISurface);
	IFACE_GET(Localize, enc("localize.dll"), enc("Localize_"), ILocalize);
	IFACE_GET(EngineSound, enc("engine.dll"), enc("IEngineSoundClient"), IEngineSound);

	// Get interfaces manually
	Globals = **(CGlobalVarsBase***)((*(DWORD**)BaseClient)[0] + 0x1B);
	LOG_INTERFACE(CLIENT_DLL, enc("Globals"), Globals);
	ClientMode = **(IClientMode***)((*(DWORD**)BaseClient)[10] + 0x5);
	LOG_INTERFACE(CLIENT_DLL, enc("ClientMode"), ClientMode);
	Input = *(Input_t**)(((DWORD*)*(DWORD*)BaseClient)[15] + 0x1);
	LOG_INTERFACE(CLIENT_DLL, enc("Input"), Input);
	ClientState = **(CClientState***)((*(DWORD**)Engine)[20] + 0x1);
	LOG_INTERFACE(ENGINE_DLL, enc("ClientState"), ClientState);

	// Get pattern-based interfaces or static pointers
	PlayerResource = *(C_CSPlayerResource**)(CLIENT_BASE + Vitruvia::Offsets::PlayerResource);
	LOG_INTERFACE(CLIENT_DLL, enc("PlayerResource"), PlayerResource);
	GameRules = *(C_CSGameRulesProxy**)(CLIENT_BASE + Vitruvia::Offsets::GameRules);
	LOG_INTERFACE(CLIENT_DLL, enc("GameRules"), GameRules);
	ViewRender = *(IViewRender**)(FindPattern(CLIENT_DLL, enc("A1 ? ? ? ? B9 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? FF 10")) + 1);
	LOG_INTERFACE(CLIENT_DLL, enc("ViewRender"), ViewRender);
	CSWeaponSystem = *(CCSWeaponSystem**)(FindPattern(CLIENT_DLL, enc("8B 35 ? ? ? ? FF 10 0F B7 C0")) + 2);
	LOG_INTERFACE(CLIENT_DLL, enc("CSWeaponSystem"), CSWeaponSystem);
	MoveHelper = **(IMoveHelper***)(FindPattern(CLIENT_DLL, enc("8B 0D ? ? ? ? 8B 46 08 68")) + 2);
	LOG_INTERFACE(CLIENT_DLL, enc("MoveHelper"), MoveHelper);
	ViewRenderBeams = rcast<IViewRenderBeams*>(*rcast<offset_t*>(FindPattern(CLIENT_DLL, enc("A1 ? ? ? ? 56 8B F1 B9 ? ? ? ? FF 50 08")) + 1));
	LOG_INTERFACE(CLIENT_DLL, enc("ViewRenderBeams"), ViewRenderBeams);
	CenterPrint = *rcast<VGUI::CCenterPrint**>(FindPattern(CLIENT_DLL, enc("A1 ? ? ? ? B9 ? ? ? ? FF 50 1C E8"))) + 6;
	LOG_INTERFACE(CLIENT_DLL, enc("CenterPrint"), CenterPrint);
	Render = *rcast<CRender**>(FindPattern(ENGINE_DLL, enc("56 57 0F 57 C0 C7 05 ? ? ? ? ? ? ? ? 51")) + 0x7);
	LOG_INTERFACE(ENGINE_DLL, enc("Render"), Render);
	PlayerInventory = **rcast<C_PlayerInventory***>(FindPattern(CLIENT_DLL, enc("8B 3D ? ? ? ? 85 FF 74 1A")) + 0x2);
	LOG_INTERFACE(CLIENT_DLL, enc("PlayerInventory"), PlayerInventory);
}
std::vector<InterfaceReg*> DumpInterfaces(const std::string& module_name)
{
	// This is gonna save shit
	std::vector<InterfaceReg*> vec{};

	// Instantiate the beginning of linked list
	InterfaceReg* interface_list = FindInterfaceList(module_name);
	if (!interface_list)
		return vec; // Return empty vector if interface_list is nullptr

	// Walk through the linked list and fill up the vector
	for (InterfaceReg* ir = interface_list; ir; ir = ir->m_pNext)
		vec.push_back(ir);

	// Return it to print or do shit with it
	return vec;
}

SOURCE_END