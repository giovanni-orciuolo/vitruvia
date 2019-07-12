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

SOURCE_INIT

enum class SendPropType
{
	DPT_Int = 0,
	DPT_Float,
	DPT_Vector,
	DPT_VectorXY,	// Only encodes the XY of a vector, ignores Z
	DPT_String,
	DPT_Array,		// An array of the base types (can't be of datatables).
	DPT_DataTable,
	DPT_Int64,
	DPT_NUMSendPropTypes
};

class DVariant
{
public:
	union
	{
		float	m_Float;
		long	m_Int;
		char	*m_pString;
		void	*m_pData;
		float	m_Vector[3];
	};
};

class RecvTable;
class RecvProp;

class CRecvProxyData
{
public:
	const RecvProp	*m_pRecvProp;		// The property it's receiving.
	char			_pad[4];			// CS:GO pad only because fuck yes :)
	DVariant		m_Value;			// The value given to you to store.
	int				m_iElement;			// Which array element you're getting.
	int				m_ObjectID;			// The object being referred to.
};

using RecvVarProxyFn = void(__cdecl*)(const CRecvProxyData*, void*, void*);

class RecvProp
{
public:
	char*                    m_pVarName;
	SendPropType             m_RecvType;
	int                      m_Flags;
	int                      m_StringBufferSize;
	bool                     m_bInsideArray;
	const void*              m_pExtraData;
	RecvProp*                m_pArrayProp;
	void*                    m_ArrayLengthProxy;
	RecvVarProxyFn           m_ProxyFn;
	void*                    m_DataTableProxyFn;
	RecvTable*               m_pDataTable;
	int                      m_Offset;
	int                      m_ElementStride;
	int                      m_nElements;
	const char*              m_pParentArrayPropName;
};

class RecvTable
{
public:
	RecvProp*                m_pProps;
	int                      m_nProps;
	void*                    m_pDecoder;
	char*                    m_pNetTableName;
	bool                     m_bInitialized;
	bool                     m_bInMainList;
};

SOURCE_END