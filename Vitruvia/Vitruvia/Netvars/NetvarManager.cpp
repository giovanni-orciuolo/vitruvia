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

#include "NetvarManager.h"

namespace Vitruvia {

V_NetvarManager* g_pNetvars;

std::string PropTypeToString(Source::SendPropType PropType, int Elements, int StringBufferCount)
{
	using namespace Source;
	switch (PropType) {
	case Source::SendPropType::DPT_Int:
		return enc("int");
	case Source::SendPropType::DPT_Float:
		return enc("float");
	case Source::SendPropType::DPT_Vector:
		return enc("Vec3");
	case Source::SendPropType::DPT_VectorXY:
		return enc("Vec2");
	case Source::SendPropType::DPT_String:
		return enc("char[") + std::to_string(StringBufferCount) + enc("]");
	case Source::SendPropType::DPT_Array:
		return enc("[") + std::to_string(Elements) + enc("]");
	case Source::SendPropType::DPT_DataTable:
		return enc("void*");
	default:
		return enc("");
	}
}

std::string PropTypeToType(Source::SendPropType PropType, int Elements, int StringBufferCount)
{
	using namespace Source;
	switch (PropType) {
	case Source::SendPropType::DPT_Int:
		return enc("int");
	case Source::SendPropType::DPT_Float:
		return enc("float");
	case Source::SendPropType::DPT_Vector:
		return enc("Vector");
	case Source::SendPropType::DPT_VectorXY:
		return enc("Vector2");
	case Source::SendPropType::DPT_String:
		return enc("char*");
	case Source::SendPropType::DPT_Array:
		return enc("[") + std::to_string(Elements) + enc("]");
	case Source::SendPropType::DPT_DataTable:
		return enc("void*");
	default:
		return enc("");
	}
}

Source::RecvTable* V_NetvarManager::GetTable(const char* TableName)
{
	if (m_vecTables.empty())
	{
		Err(enc("ERROR: Failed to find table: %s (m_vecTables is empty)"), TableName);
		return 0;
	}

	for (auto table : m_vecTables)
	{
		if (!table)
			continue;
		if (_stricmp(table->m_pNetTableName, TableName) == 0)
			return table;
	}
	return 0;
}

int V_NetvarManager::GetRecvProp(Source::RecvTable* RecvTable, const char* PropName, Source::RecvProp** Prop)
{
	int extraOffset = 0;
	for (int i = 0; i < RecvTable->m_nProps; ++i)
	{
		Source::RecvProp* RecvProp = &RecvTable->m_pProps[i];

		Source::RecvTable* child = RecvProp->m_pDataTable;

		if (child && (child->m_nProps > 0))
		{
			int tmp = GetRecvProp(child, PropName, Prop);
			if (tmp)
				extraOffset += (RecvProp->m_Offset + tmp);
		}

		if (_stricmp(RecvProp->m_pVarName, PropName))
			continue;
		if (Prop)
			*Prop = RecvProp;
		return (RecvProp->m_Offset + extraOffset);
	}
	return extraOffset;
}

int V_NetvarManager::GetRecvProp(const char* TableName, const char* PropName, Source::RecvProp** Prop)
{
	Source::RecvTable* RecvTable = GetTable(TableName);
	if (!RecvTable)
	{
		Err(enc("ERROR: Failed to find table: %s"), TableName);
		return 0;
	}
	int offset = GetRecvProp(RecvTable, PropName, Prop);
	if (!offset)
	{
		Err(enc("ERROR: Failed to find offset for prop: %s from table: %s"), PropName, TableName);
		return 0;
	}

	return offset;
}

V_NetvarManager::V_NetvarManager(Source::ClientClass* ClassHead)
{
	m_vecTables.clear();

	Source::ClientClass* ClientClass = ClassHead;
	if (!ClientClass)
	{
		Err(enc("ERROR: ClientClass was not found\n"));
		return;
	}

	while (ClientClass)
	{
		Source::RecvTable* RecvTable = ClientClass->m_pRecvTable;

		m_vecTables.push_back(RecvTable);

		ClientClass = ClientClass->m_pNext;
	}
}

int V_NetvarManager::GetOffset(const char* TableName, const char* PropName)
{
	int offset = GetRecvProp(TableName, PropName);
	if (!offset)
	{
		Err(enc("ERROR: Failed to find offset for prop: %s from table: %s"), PropName, TableName);
		return 0;
	}
	return offset;
}

Source::RecvVarProxyFn V_NetvarManager::GetProxyFunction(const char* TableName, const char* PropName)
{
	Source::RecvProp* RecvProp = nullptr;
	GetRecvProp(TableName, PropName, &RecvProp);
	if (!RecvProp)
		return nullptr;
	return RecvProp->m_ProxyFn;
}

void V_NetvarManager::SetProxyFunction(const char* TableName, const char* PropName, Source::RecvVarProxyFn Function)
{
	Source::RecvProp* RecvProp = nullptr;
	GetRecvProp(TableName, PropName, &RecvProp);
	if (!RecvProp)
		return;
	RecvProp->m_ProxyFn = Function;
}

std::string V_NetvarManager::DumpTable(Source::RecvTable* RecvTable, int Depth)
{
	std::string pre(enc(""));
	std::stringstream ss;

	for (int i = 0; i < Depth; i++)
		pre.append(enc("\t"));

	ss << pre << RecvTable->m_pNetTableName << enc("\n");

	for (int i = 0; i < RecvTable->m_nProps; ++i) 
	{
		Source::RecvProp* prop = &RecvTable->m_pProps[i];
		if (!prop)
			continue;

		std::string varName(prop->m_pVarName);

		if (varName.find(enc("baseclass")) == 0 
			|| varName.find(enc("0")) == 0 
			|| varName.find(enc("1")) == 0 
			|| varName.find(enc("2")) == 0)
			continue;

		ss << pre << enc("\t") 
			<< varName 
			<< enc(" [0x") << std::hex << std::uppercase << prop->m_Offset << std::nouppercase << std::dec 
			<< enc("] (") << PropTypeToString(prop->m_RecvType, prop->m_nElements, prop->m_StringBufferSize) << enc(")\n");

		if (prop->m_pDataTable)
			ss << DumpTable(prop->m_pDataTable, Depth + 1);
	}

	return ss.str();
}

void V_NetvarManager::DumpNetvars(Source::ClientClass* ClientClassHead)
{
	std::stringstream ss;

	for (Source::ClientClass* pClass = ClientClassHead; pClass != nullptr; pClass = pClass->m_pNext)
	{
		Source::RecvTable* table = pClass->m_pRecvTable;
		ss << DumpTable(table, 0);
	}

	std::ofstream of;
	of.open(GetAppDataFolder().append(enc("NetvarsDump.txt")), std::ios::trunc);

	if (of.is_open())
	{
		of << enc("<< NETVARS DUMP >>\n") << enc(">> Dumped with love by Vitruvia :*\n")
			<< enc("Dump Date: ") << GetCurrentDay() << " " << GetDateString() << enc("\n")
			<< enc("WARNING: Offsets are separated from the inherited table!") << enc("\n\n")
			<< ss.str();
	}
	else
	{
		Err(enc("Failed to open file: %s"), GetAppDataFolder().append(enc("NetvarsDump.txt")).c_str());
	}

	of.close();
}

void ScanTable(std::ofstream& of, Source::RecvTable* table, const char* original_table_name)
{
	// Special behaviour for DT_PlayerResource and DT_CSPlayerResource
	if (!strcmp(original_table_name, enc("DT_PlayerResource")) 
		|| !strcmp(original_table_name, enc("DT_CSPlayerResource")))
	{
		for (int i = 0; i < table->m_nProps; ++i)
		{
			Source::RecvProp* prop = &table->m_pProps[i];
			if (!prop)
				continue;

			std::string PropName(prop->m_pVarName);

			if (PropName.find(enc("baseclass")) == 0 
				|| PropName.find(enc("0")) == 0 
				|| PropName.find(enc("1")) == 0 
				|| PropName.find(enc("2")) == 0)
				continue;

			if (PropTypeToType(prop->m_RecvType, prop->m_nElements, prop->m_StringBufferSize) == enc("void*"))
			{
				// Property is a data table
				std::string prop_type = enc("");

				if (PropName.find(enc("m_b")) != std::string::npos || PropName.find(enc("m_is")) != std::string::npos)
					prop_type = enc("bool");
				else if (PropName.find(enc("m_i")) != std::string::npos 
					|| PropName.find(enc("m_hostage")) != std::string::npos 
					|| PropName.find(enc("m_n")) != std::string::npos)
					prop_type = enc("int"); // or float if m_hostage?
				else if (PropName.find(enc("m_sz")) != std::string::npos)
					prop_type = enc("char*");
				else if (PropName.find(enc("m_h")) != std::string::npos)
					prop_type = enc("EHANDLE");

				if (prop_type == "char*")
					of << "\tMAKE_NETVAR_PTR_ARRAY(char*, " << PropName 
					<< enc(", ") << enc("\"") 
					<< PropName << enc("\"") 
					<< enc(", ") << enc("\"") << original_table_name << enc("\")") << std::endl;
				else
					of << enc("\tMAKE_NETVAR_ARRAY(") << prop_type 
					<< enc(", ") << PropName 
					<< enc(", ") << "\"" 
					<< PropName << enc("\"") << enc(", ") << enc("\"") 
					<< original_table_name << enc("\")") << std::endl;
			}
			else
			{
				of << enc("\tMAKE_NETVAR_ARRAY(") 
					<< PropTypeToType(prop->m_RecvType, prop->m_nElements, prop->m_StringBufferSize) 
					<< enc(", ") << PropName << enc(", ") << enc("\"") << PropName 
					<< enc("\"") << enc(", ") << enc("\"") << original_table_name << enc("\")") << std::endl;
			}
		}

		return;
	}

	for (int i = 0; i < table->m_nProps; ++i)
	{
		Source::RecvProp* prop = &table->m_pProps[i];
		if (!prop)
			continue;

		std::string PropName(prop->m_pVarName);

		if (PropName.find(enc("baseclass")) == 0
			|| PropName.find(enc("0")) == 0
			|| PropName.find(enc("1")) == 0
			|| PropName.find(enc("2")) == 0)
			continue;

		std::string FuncName = PropName;
		if (FuncName.find(enc("[")) != std::string::npos && FuncName.find(enc("]")) != std::string::npos)
		{
			FuncName.replace(FuncName.find(enc("[")), 1, enc("_"));
			FuncName.replace(FuncName.find(enc("]")), 1, enc(""));
		}
		if (FuncName.find(enc(".")) != std::string::npos)
			FuncName.replace(FuncName.find(enc(".")), 1, enc("_"));

		if (prop->m_pDataTable == nullptr)
		{
			if (PropTypeToType(prop->m_RecvType, prop->m_nElements, prop->m_StringBufferSize) == enc("char*"))
				of << enc("\tMAKE_NETVAR_PTR(char, ") << FuncName << enc(", ") 
				<< enc("\"") 
				<< PropName << enc("\"") << enc(", ") 
				<< enc("\"") << original_table_name << enc("\")") << enc(" // Size = ") 
				<< prop->m_StringBufferSize << std::endl;
			else
			{
				std::string prop_type = PropTypeToType(prop->m_RecvType, prop->m_nElements, prop->m_StringBufferSize);

				if (PropName.find(enc("m_b")) != std::string::npos
					|| PropName.find(enc("m_is")) != std::string::npos)
					prop_type = enc("bool");
				else if (PropName.find(enc("m_h")) != std::string::npos)
					prop_type = enc("EHANDLE");

				of << enc("\tMAKE_NETVAR(") << prop_type << enc(", ") << FuncName << enc(", ") 
					<< enc("\"") << PropName << enc("\"") << enc(", ") << enc("\"") 
					<< original_table_name << enc("\"") << enc(")") << std::endl;
			}
		}
		else
		{
			ScanTable(of, prop->m_pDataTable, original_table_name);
		}
	}
}

void V_NetvarManager::BuildTable(const char* TableName, const char* Inheritance)
{
	Log(enc("Building table %s..."), TableName);
	Source::RecvTable* table = GetTable(TableName);
	if (table)
	{
		std::string ClassName = std::string(TableName);
		ClassName.replace(0, 2, enc("C"));
		std::ofstream of;
		of.open(GetAppDataFolder().append(ClassName).append(enc(".h")), std::ios::trunc);
		if (of.is_open())
		{
			if (!strcmp(Inheritance, ""))
			{
				of << enc("// Class built with love by Vitruvia :*") << std::endl
				<< enc("// Build date: ") << GetCurrentDay() << " " << GetDateString() << std::endl << std::endl
				<< enc("#pragma once") << std::endl
				<< enc("#include \"../Vitruvia.h\"") << std::endl << std::endl
				<< enc("SOURCE_INIT") << std::endl << std::endl
				<< enc("class ") << ClassName
				<< enc("{") << std::endl
				<< enc("public:") << std::endl;
			}
			else
			{
				of << enc("// Class built with love by Vitruvia :*") << std::endl
				<< enc("// Build date: ") << GetCurrentDay() << " " << GetDateString() << std::endl << std::endl
				<< enc("#pragma once") << std::endl
				<< enc("#include \"") << Inheritance << enc(".h\"") << std::endl << std::endl
				<< enc("SOURCE_INIT") << std::endl << std::endl
				<< enc("class ") << ClassName
				<< enc(" : public ") << Inheritance << std::endl
				<< enc("{") << std::endl
				<< enc("public:") << std::endl;
			}
			ScanTable(of, table, table->m_pNetTableName);
			of	<< enc("};") << std::endl << std::endl
				<< enc("SOURCE_END") << std::endl;
		}
		else
		{
			Err(enc("Failed to open file: %s"), GetAppDataFolder().append(ClassName).append(enc(".h")).c_str());
		}
		of.close();
	}
	Log(enc("Done."));
}

}
