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
#include "../Vitruvia.h"

namespace Vitruvia {

class V_NetvarManager
{
public:
	V_NetvarManager(Source::ClientClass* ClientClassHead);
	int GetOffset( const char* TableName, const char* PropName );
	Source::RecvVarProxyFn GetProxyFunction(const char* TableName, const char* PropName);
	void SetProxyFunction(const char* TableName, const char* PropName, Source::RecvVarProxyFn Function);
	std::string DumpTable(Source::RecvTable* RecvTable, int Depth);
	void DumpNetvars(Source::ClientClass* ClientClassHead);
	void BuildTable(const char* TableName, const char* Inheritance);
private:
	int GetRecvProp(const char* TableName, const char* PropName, Source::RecvProp** Prop = 0);
	int GetRecvProp(Source::RecvTable* RecvTable, const char* PropName, Source::RecvProp** Prop = 0);
	Source::RecvTable* GetTable(const char* TableName);
	std::vector<Source::RecvTable*> m_vecTables;
};

extern V_NetvarManager* g_pNetvars;

#define GET_NETVAR(TableName, PropName) Vitruvia::g_pNetvars->GetOffset(TableName, PropName)

#define MAKE_NETVAR(prop_type, func_name, prop_name, table_name)						\
prop_type func_name()																	\
{																						\
    static offset_t netvar = GET_NETVAR(table_name, prop_name);							\
	return *MAKE_PTR<prop_type>(this, netvar);													\
}
#define MAKE_NETVAR_PTR(prop_type, func_name, prop_name, table_name)					\
prop_type* func_name()																	\
{																						\
	static offset_t netvar = GET_NETVAR(table_name, prop_name);							\
	return MAKE_PTR<prop_type>(this, netvar);													\
}

#define MAKE_NETVAR_ARRAY(prop_type, func_name, prop_name, table_name)					\
prop_type func_name(int index)															\
{																						\
    static offset_t netvar = GET_NETVAR(table_name, prop_name);							\
	return *MAKE_PTR<prop_type>(this, netvar + index * sizeof(prop_type));						\
}
#define MAKE_NETVAR_PTR_ARRAY(prop_type, func_name, prop_name, table_name)				\
prop_type func_name(int index)															\
{																						\
    static offset_t netvar = GET_NETVAR(table_name, prop_name);							\
	return MAKE_PTR<prop_type>(this, netvar + index * sizeof(prop_type));						\
}

#define BUILD_TABLE(table, inheritance) g_pNetvars->BuildTable(table, inheritance);

}
