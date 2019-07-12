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
#include "../../SourceCommon.h"
#include "../../Classes/Color.h"
#include "../IAppSystem.h"

#pragma warning(disable : 4172)

#define FCVAR_NONE  0 
#define FCVAR_UNREGISTERED					(1<<0)    // If this is set, don't add to linked list, etc.
#define FCVAR_DEVELOPMENTONLY				(1<<1)    // Hidden in released products. Flag is removed automatically if ALLOW_DEVELOPMENT_CVARS is defined.
#define FCVAR_GAMEDLL						(1<<2)    // defined by the game DLL
#define FCVAR_CLIENTDLL						(1<<3)  // defined by the client DLL
#define FCVAR_HIDDEN						(1<<4)    // Hidden. Doesn't appear in find or autocomplete. Like DEVELOPMENTONLY, but can't be compiled out.

// ConVar only
#define FCVAR_PROTECTED						(1<<5)  // It's a server cvar, but we don't send the data since it's a password, etc.  Sends 1 if it's not bland/zero, 0 otherwise as value
#define FCVAR_SPONLY						(1<<6)  // This cvar cannot be changed by clients connected to a multiplayer server.
#define FCVAR_ARCHIVE						(1<<7)    // set to cause it to be saved to vars.rc
#define FCVAR_NOTIFY						(1<<8)    // notifies players when changed
#define FCVAR_USERINFO						(1<<9)    // changes the client's info string
#define FCVAR_CHEAT							(1<<14) // Only useable in singleplayer / debug / multiplayer & sv_cheats

#define FCVAR_PRINTABLEONLY					(1<<10)  // This cvar's string cannot contain unprintable characters ( e.g., used for player name etc ).
#define FCVAR_UNLOGGED						(1<<11)  // If this is a FCVAR_SERVER, don't log changes to the log file / console if we are creating a log
#define FCVAR_NEVER_AS_STRING				(1<<12)  // never try to print that cvar

// It's a ConVar that's shared between the client and the server.
// At signon, the values of all such ConVars are sent from the server to the client (skipped for local
//  client, of course )
// If a change is requested it must come from the console (i.e., no remote client changes)
// If a value is changed while a server is active, it's replicated to all connected clients
#define FCVAR_REPLICATED					(1<<13)   // server setting enforced on clients, TODO rename to FCAR_SERVER at some time
#define FCVAR_DEMO							(1<<16)  // record this cvar when starting a demo file
#define FCVAR_DONTRECORD					(1<<17)  // don't record these command in demofiles

#define FCVAR_NOT_CONNECTED					(1<<22)   // cvar cannot be changed by a client that is connected to a server

#define FCVAR_ARCHIVE_XBOX					(1<<24) // cvar written to config.cfg on the Xbox

#define FCVAR_SERVER_CAN_EXECUTE			(1<<28)// the server is allowed to execute this command on clients via ClientCommand/NET_StringCmd/CBaseClientState::ProcessStringCmd.
#define FCVAR_SERVER_CANNOT_QUERY			(1<<29)// If this is set, then the server is not allowed to query this cvar's value (via IServerPluginHelpers::StartQueryCvarValue).
#define FCVAR_CLIENTCMD_CAN_EXECUTE			(1<<30)   // IVEngineClient::ClientCmd is allowed to execute this command. 

SOURCE_INIT

class IConVar;
class CCommand;

typedef void(*FnChangeCallback_t)(IConVar *var, const char *pOldValue, float flOldValue);

abstract_class IConVar
{
public:
	virtual void        SetValue(const char *pValue) = 0;
	virtual void        SetValue(float flValue) = 0;
	virtual void        SetValue(int nValue) = 0;
	virtual const char* GetName(void) const = 0;
	virtual bool        IsFlagSet(int nFlag) const = 0;
};

class ConCommandBase;
class ConCommand;
class ConVar;

typedef int CVarDLLIdentifier_t;

class IConsoleDisplayFunc
{
public:
	virtual void ColorPrint(const Color& clr, const char *pMessage) = 0;
	virtual void Print(const char *pMessage) = 0;
	virtual void DPrint(const char *pMessage) = 0;
};

abstract_class ICvar : IAppSystem
{
public:
	virtual CVarDLLIdentifier_t        AllocateDLLIdentifier() = 0; // 9
	virtual void                       RegisterConCommand(ConCommandBase *pCommandBase) = 0; //10
	virtual void                       UnregisterConCommand(ConCommandBase *pCommandBase) = 0;
	virtual void                       UnregisterConCommands(CVarDLLIdentifier_t id) = 0;
	virtual const char*                GetCommandLineValue(const char *pVariableName) = 0; //11
	virtual ConCommandBase*            FindCommandBase(const char *name) = 0; //12
	virtual const ConCommandBase*      FindCommandBase(const char *name) const = 0;
	virtual ConVar*                    FindVar(const char *var_name) = 0;
	virtual const ConVar*              FindVar(const char *var_name) const = 0;
	virtual ConCommand*                FindCommand(const char *name) = 0;
	virtual const ConCommand*          FindCommand(const char *name) const = 0; //17
	virtual void                       InstallGlobalChangeCallback(FnChangeCallback_t callback) = 0;
	virtual void                       RemoveGlobalChangeCallback(FnChangeCallback_t callback) = 0;
	virtual void                       CallGlobalChangeCallbacks(ConVar *var, const char *pOldString, float flOldValue) = 0;
	virtual void                       InstallConsoleDisplayFunc(IConsoleDisplayFunc* pDisplayFunc) = 0;
	virtual void                       RemoveConsoleDisplayFunc(IConsoleDisplayFunc* pDisplayFunc) = 0;
	virtual void                       ConsoleColorPrintf(const Color& clr, const char *pFormat, ...) const = 0;
	virtual void                       ConsolePrintf(const char *pFormat, ...) const = 0;
	virtual void                       ConsoleDPrintf(const char *pFormat, ...) const = 0;
	virtual void                       RevertFlaggedConVars(int nFlag) = 0;
};

abstract_class IConCommandBaseAccessor 
{
public:
	virtual bool RegisterConCommandBase(ConCommandBase *pVar) = 0;
};
void ConVar_Register(int nCVarFlag, IConCommandBaseAccessor *pAccessor);

abstract_class ConCommandBase 
{
public:
	ConCommandBase(void);
	ConCommandBase(const char *pName, const char *pHelpString = 0, int flags = 0);

	virtual                            ~ConCommandBase(void);
	virtual   bool                     IsCommand(void) const;
	virtual bool                       IsFlagSet(int flag) const;
	virtual void                       AddFlags(int flags);
	virtual void                       RemoveFlags(int flags);
	virtual int                        GetFlags() const;
	virtual const char*                GetName(void) const;
	virtual const char*                GetHelpText(void) const;
	virtual bool                       IsRegistered(void) const;
	virtual int                        GetDLLIdentifier() const;
	virtual void                       Create(const char *pName, const char *pHelpString = 0, int flags = 0);
	virtual void                       Init();

public:
	ConCommandBase*                    m_pNext;            //0x4
	bool                               m_bRegistered;      //0x8
	const char*                        m_pszName;          //0xC
	const char*                        m_pszHelpString;    //0x10
	int                                m_nFlags;           //0x14

public:
	static ConCommandBase*             s_pConCommandBases;
	static IConCommandBaseAccessor*    s_pAccessor;
};

abstract_class ConVar : public ConCommandBase, public IConVar 
{
public:

	typedef ConCommandBase BaseClass;

	ConVar(const char *pName, const char *pDefaultValue, int flags = 0);

	ConVar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString);
	ConVar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString, bool bMin, float fMin, bool bMax, float fMax);
	ConVar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString, FnChangeCallback_t callback);
	ConVar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString, bool bMin, float fMin, bool bMax, float fMax, FnChangeCallback_t callback);

	virtual                            ~ConVar(void);
	virtual bool                       IsFlagSet(int flag) const;
	virtual const char*                GetName(void) const;
	virtual const char*                GetBaseName(void) const;
	virtual int                        GetSplitScreenPlayerSlot() const;
	virtual bool					   IsCommand(void) const;
	virtual void                       SetValue(const char *value);
	virtual void                       SetValue(float value);
	virtual void                       SetValue(int value);
	virtual void                       SetValue(Color value);
	virtual void                       InternalSetValue(const char *value);
	virtual void                       InternalSetFloatValue(float fNewValue);
	virtual void                       InternalSetIntValue(int nValue);
	virtual void                       InternalSetColorValue(Color value);
	virtual bool                       ClampValue(float& value);
	virtual void                       ChangeStringValue(const char *tempVal, float flOldValue);
	virtual void                       Create(const char *pName, const char *pDefaultValue, int flags = 0,
		const char *pHelpString = 0, bool bMin = false, float fMin = 0.0,
		bool bMax = false, float fMax = 0.0f, FnChangeCallback_t callback = 0);

	//----------------------------
	// Non-virtual helper methods
	//----------------------------
	float GetFloat(void) const
	{
		auto temp = *(int*)(&this->m_fValue);
		auto temp_result = (int)(temp ^ (DWORD)this);
		return *(float*)(&temp_result);
	}
	int	GetInt(void) const
	{
		auto temp = *(int*)(&this->m_nValue);
		auto temp_result = (int)(temp ^ (DWORD)this);
		return *(int*)(&temp_result);
	}
	Color GetColor(void) const
	{
		// Always returns white cuz dunno how to treat this particular type
		return Color::White();
	}
	const char*	GetString(void) const
	{
		auto temp = *(int*)(&this->m_pszString);
		auto temp_result = (int)(temp ^ (DWORD)this);
		return (const char*)(&temp_result);
	}
	const char*	GetDefault(void) const
	{
		auto temp = *(int*)(&this->m_pszDefaultValue);
		auto temp_result = (int)(temp ^ (DWORD)this);
		return (const char*)(&temp_result);
	}

	ConVar*                            m_pParent;          //0x1C
	const char*                        m_pszDefaultValue;  //0x20
	char*                              m_pszString;        //0x24
	int                                m_StringLength;     //0x28
	float                              m_fValue;           //0x2C
	int                                m_nValue;           //0x30
	bool                               m_bHasMin;          //0x34
	float                              m_fMinVal;          //0x38
	bool                               m_bHasMax;          //0x3C
	float                              m_fMaxVal;          //0x40
	FnChangeCallback_t                 m_fnChangeCallback; //0x44
};

// Credits: MarkHC
class SpoofedConvar 
{
public:
	SpoofedConvar(ICvar* pInterface, const char* szCVar);
	SpoofedConvar(ICvar* pInterface, ConVar* pCVar);

	~SpoofedConvar();

	bool           IsSpoofed();
	void           Spoof();

	void           SetFlags(int flags);
	int            GetFlags();

	void           SetBool(bool bValue);
	void           SetInt(int iValue);
	void           SetFloat(float flValue);
	void           SetString(const char* szValue);

	int            GetInt();
	float          GetFloat();
	const char*    GetString();

private:
	ICvar* m_pInterface = nullptr;

	ConVar* m_pOriginalCVar = nullptr;
	ConVar* m_pDummyCVar = nullptr;

	char m_szDummyName[128];
	char m_szDummyValue[128];
	char m_szOriginalName[128];
	char m_szOriginalValue[128];
	int m_iOriginalFlags;
};

SOURCE_END