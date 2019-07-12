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
#include "../../Classes/KeyValues.h"

/*
	The GameEventManager keeps track and fires of all global game events. Game events
	are fired by game.dll for events like player death or team wins. Each event has a
	unique name and comes with a KeyValue structure providing informations about this
	event. Some events are generated also by the engine.
	Events are networked to connected clients and invoked there to. Therefore you
	have to specify all data fields and there data types in an public resource
	file which is parsed by server and broadcasted to it's clients. A typical game
	event is defined like this:
	"game_start"				// a new game starts
	{
	"roundslimit"	"long"		// max round
	"timelimit"		"long"		// time limit
	"fraglimit"		"long"		// frag limit
	"objective"		"string"	// round objective
	}
	All events must have unique names (case sensitive) and may have a list
	of data fields. each data field must specify a data type, so the engine
	knows how to serialize/unserialize that event for network transmission.
	Valid data types are string, float, long, short, byte & bool. If a
	data field should not be broadcasted to clients, use the type "local".
*/


#define MAX_EVENT_NAME_LENGTH	32		// max game event name length
#define MAX_EVENT_BITS			9		// max bits needed for an event index
#define MAX_EVENT_NUMBER		(1<<MAX_EVENT_BITS)		// max number of events allowed
#define MAX_EVENT_BYTES			1024	// max size in bytes for a serialized event
#define EVENT_DEBUG_ID_INIT		42
#define EVENT_DEBUG_ID_SHUTDOWN 13

#ifdef CreateEvent
#undef CreateEvent
#endif

SOURCE_INIT

class bf_read;
class bf_write;

abstract_class IGameEvent
{
public:
	virtual ~IGameEvent() {};
	virtual const char *GetName() const = 0;    // get event name
	virtual bool  IsReliable() const = 0; // if event handled reliable
	virtual bool  IsLocal() const = 0; // if event is never networked
	virtual bool  IsEmpty(const char *keyName = NULL) = 0; // check if data field exists
														   // Data access
	virtual bool					GetBool(const char *keyName = NULL, bool defaultValue = false) = 0;
	virtual int						GetInt(const char *keyName = NULL, int defaultValue = 0) = 0;
	virtual unsigned long long		GetUint64(char const *keyName = NULL, unsigned long long defaultValue = 0) = 0;
	virtual float					GetFloat(const char *keyName = NULL, float defaultValue = 0.0f) = 0;
	virtual const char				*GetString(const char *keyName = NULL, const char *defaultValue = "") = 0;
	virtual const wchar_t			*GetWString(char const *keyName = NULL, const wchar_t *defaultValue = L"") = 0;
	virtual const void				*GetPtr(const char* keyname = NULL, const void* default_values = NULL) = 0;

	virtual void SetBool(const char *keyName, bool value) = 0;
	virtual void SetInt(const char *keyName, int value) = 0;
	virtual void SetUInt64(const char *keyName, unsigned long long value) = 0;
	virtual void SetFloat(const char *keyName, float value) = 0;
	virtual void SetString(const char *keyName, const char *value) = 0;
	virtual void SetWString(const char *keyName, const wchar_t *value) = 0;
	virtual void SetPtr(const char* keyname, const void* value) = 0;
};

abstract_class IGameEventListener2
{
public:
	virtual	~IGameEventListener2(void) {};
	// FireEvent is called by EventManager if event just occured
	// KeyValue memory will be freed by manager if not needed anymore
	virtual void FireGameEvent(IGameEvent* pEvent) = 0;
	virtual int GetEventDebugID(void) = 0;
};

abstract_class IGameEventManager2
{
public:
	virtual	~IGameEventManager2(void) {};

	// load game event descriptions from a file eg "resource\gameevents.res"
	virtual int LoadEventsFromFile(const char *filename) = 0;

	// removes all and anything
	virtual void  Reset() = 0;

	// adds a listener for a particular event
	virtual bool AddListener(IGameEventListener2 *listener, const char *name, bool bServerSide) = 0;

	// returns true if this listener is listens to given event
	virtual bool FindListener(IGameEventListener2 *listener, const char *name) = 0;

	// removes a listener 
	virtual void RemoveListener(IGameEventListener2 *listener) = 0;

	// adds a global listener for a particular event
	virtual void AddListenerGlobal(IGameEventListener2* listener, bool serverside) = 0;

	// create an event by name, but doesn't fire it. returns NULL is event is not
	// known or no listener is registered for it. bForce forces the creation even if no listener is active
	virtual IGameEvent *CreateEvent(const char *name, bool bForce = false) = 0;

	// fires a server event created earlier, if bDontBroadcast is set, event is not send to clients
	virtual bool FireEvent(IGameEvent *event, bool bDontBroadcast = false) = 0;

	// fires an event for the local client only, should be used only by client code
	virtual bool FireEventClientSide(IGameEvent *pEvent) = 0;

	// create a new copy of this event, must be free later
	virtual IGameEvent *DuplicateEvent(IGameEvent *pEvent) = 0;

	// if an event was created but not fired for some reason, it has to bee freed, same UnserializeEvent
	virtual void FreeEvent(IGameEvent *pEvent) = 0;

	// write/read event to/from bitbuffer
	virtual bool SerializeEvent(IGameEvent *pEvent, bf_write *buf) = 0;
	virtual IGameEvent *UnserializeEvent(bf_read *buf) = 0; // create new KeyValues, must be deleted

	// returns event data types
	virtual KeyValues* GetEventDataTypes(IGameEvent* event) = 0;
};

SOURCE_END