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
#include "../../Vitruvia.h"

#define PAGE_WRITEABLE		(PAGE_READWRITE | PAGE_EXECUTE_READWRITE)
#define PAGE_READABLE		(PAGE_WRITEABLE |PAGE_READONLY | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_WRITECOPY)
#define PAGE_OFFLIMITS		(PAGE_GUARD | PAGE_NOACCESS)

namespace Vitruvia {

class V_HookManager
{
public:
	// Default constructor. Does nothing
	V_HookManager() { ZeroMemory(this, sizeof(V_HookManager)); }
	// Accepts a valid pointer to an interface as parameter and the interface name
	V_HookManager(const std::string interface_name, uintptr_t** pp_class, bool needs_rtti = false);
	// Unhooks the table
	~V_HookManager();
	// Hooks the table
	void Hook();
	// Unhooks the table
	void UnHook();
	// Gets a pointer to the vfunc based on its index
	template<typename T>
	T GetFunction(int index)
	{
		if (!this)
			return nullptr;
		if (index >= 0 && index < m_iCount)
		{
			// Returns the vfunc at index
			return (T)rcast<uintptr_t*>(m_pOriginalTable)[index];
		}
		return nullptr;
	}
	// Hooks the vfunc based on its index
	void HookFunction(const std::string& function_name, int index, void* hook_function)
	{
		if (index >= 0 && index < m_iCount)
		{
			// Hook the function
			rcast<uintptr_t*>(m_pCopyTable)[index] = rcast<uintptr_t>(hook_function);
		}
		Log(enc("Hooked %s::%s"), m_strInterfaceName.c_str(), function_name.c_str());
	}
private:
	std::string m_strInterfaceName;
	uintptr_t* m_pObject;
	uintptr_t** m_ppClass;
	uintptr_t* m_pOriginalTable, *m_pCopyTable;
	int m_iCount;
	// Returns the amount of vfuncs that the table contains
	int GetFunctionCount(const void* table);
	bool m_bNeedsRTTI;
};

class V_RecvPropHook
{
public:
	V_RecvPropHook(const std::string& table, const std::string& prop_name, Source::RecvVarProxyFn hooked_proxy)
		: table_name(table), prop_name(prop_name), hooked_proxy_fn(hooked_proxy)
	{
		original_proxy_fn = g_pNetvars->GetProxyFunction(table.c_str(), prop_name.c_str());
		SetProxyFunction(hooked_proxy);
		Log(enc("Set up proxy hook for prop %s from table %s [original = 0x%X]"), prop_name.c_str(), table.c_str(), rcast<DWORD>(original_proxy_fn));
	}

	~V_RecvPropHook()
	{
		SetProxyFunction(original_proxy_fn);
	}

	Source::RecvVarProxyFn GetOriginalFunction() const
	{
		return original_proxy_fn;
	}

	void SetProxyFunction(Source::RecvVarProxyFn proxy_fn) const
	{
		g_pNetvars->SetProxyFunction(table_name.c_str(), prop_name.c_str(), proxy_fn);
	}

private:
	std::string table_name;
	std::string prop_name;
	Source::RecvVarProxyFn hooked_proxy_fn;
	Source::RecvVarProxyFn original_proxy_fn;
};

}
