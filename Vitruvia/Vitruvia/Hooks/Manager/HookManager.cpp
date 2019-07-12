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

#include "HookManager.h"

namespace Vitruvia {

inline bool CanRead(void* table)
{
	MEMORY_BASIC_INFORMATION mbi;
	if (!table) 
		return false;
	if (!VirtualQuery(table, &mbi, sizeof(mbi))) 
		return false;
	if (mbi.Protect & PAGE_OFFLIMITS) 
		return false;
	return (mbi.Protect & PAGE_READABLE);
}
inline bool CanWrite(void* table)
{
	MEMORY_BASIC_INFORMATION mbi;
	if (!table)
		return false;
	if (!VirtualQuery(table, &mbi, sizeof(mbi)))
		return false;
	if (mbi.Protect & PAGE_OFFLIMITS)
		return false;
	return (mbi.Protect & PAGE_WRITEABLE);
}

V_HookManager::V_HookManager(const std::string interface_name, uintptr_t** pp_class, bool needs_rtti)
{
	m_bNeedsRTTI = needs_rtti;

	m_ppClass = pp_class;

	// Get a pointer to the original vtable
	m_pOriginalTable = *pp_class;

	// Count vfuncs
	m_iCount = GetFunctionCount(m_pOriginalTable);

	if (!m_bNeedsRTTI)
	{
		// Allocate enough space for the copy table and copy the original table there
		m_pCopyTable = new uintptr_t[m_iCount];
		memcpy(m_pCopyTable, m_pOriginalTable, sizeof(uintptr_t*) * m_iCount);
	}
	else
	{
		// Allocate enough space for the copy table and copy the original table there
		// But now I need to account for RTTI, so copy starting from index 1
		m_pCopyTable = new uintptr_t[m_iCount + 1];
		memcpy(&m_pCopyTable[1], m_pOriginalTable, sizeof(uintptr_t*) * m_iCount);
		m_pCopyTable[0] = m_pOriginalTable[-1];
	}

	// Instantiate m_pObject
	m_pObject = rcast<uintptr_t*>(pp_class);

	// Log everything
	m_strInterfaceName = interface_name;
	Log(enc("Initialized hook manager for table %s [%d functions]"), m_strInterfaceName.c_str(), m_iCount);

	/*
		m_ppClass = pp_class;
		m_pOriginalTable = *pp_class;
		m_iCount = GetFunctionCount( m_pOriginalTable );
		m_pCopyTable = new DWORD[ m_iCount + 1 ];
		memcpy( &m_pCopyTable[1], m_pOriginalTable, sizeof(void*) * m_iCount );
		m_pCopyTable[0] = m_OldVT[ -1 ]; ???????????????
		*ppdwClassBase = &m_NewVT[ 1 ];
	 */
}

V_HookManager::~V_HookManager()
{
	Log(enc("Destroying %s table..."), m_strInterfaceName.c_str());
	UnHook();
	delete[] m_pCopyTable;
}

void V_HookManager::Hook()
{
	//Log(enc("Redirecting vtable %s..."), m_strInterfaceName.c_str());
	if (this != nullptr 
		&& m_pObject != nullptr 
		&& m_pCopyTable != nullptr)
	{
		// Write the table pointer to be our table pointer
		if (!m_bNeedsRTTI)
		{
			*m_pObject = rcast<uintptr_t>(m_pCopyTable);
		}
		else
		{
			*m_pObject = rcast<uintptr_t>(&m_pCopyTable[1]);
		}
	}
	//Log(enc("Done."));
}

void V_HookManager::UnHook()
{
	//Log(enc("Redirecting vtable %s to the original vtable..."), m_strInterfaceName.c_str());
	if (m_pObject != nullptr 
		&& m_pOriginalTable != nullptr)
	{
		// Write the table pointer to be the original table pointer
		*m_pObject = rcast<uintptr_t>(m_pOriginalTable);
		/*if (!m_bNeedsRTTI)
		{
			*m_pObject = rcast<uintptr_t>(m_pOriginalTable);
		}
		else
		{
			*m_pObject = rcast<uintptr_t>(&m_pOriginalTable[1]);
		}*/
	}
	//Log(enc("Done."));
}

int V_HookManager::GetFunctionCount(const void* table_ptr)
{
	// Initialize table pointer and iterator
	void** table = (void**)table_ptr;
	int i = 0;

	// Iterate through the vtable until we can find a valid function
	for (void* fn; (fn = table[i]) != nullptr; i++)
	{
		// If we can't read the ptr it surely means that we've reached the end
		if (!CanRead(fn))
			break; // We can break and return i
	}

	return i;
}

}
