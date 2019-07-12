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
#include "../SourceCommon.h"

SOURCE_INIT

static constexpr int			NUM_ENT_ENTRY_BITS =			(11 + 2);
static constexpr int			NUM_ENT_ENTRIES =				(1 << NUM_ENT_ENTRY_BITS);
static constexpr unsigned int	INVALID_EHANDLE_INDEX =			0xFFFFFFFF;
static constexpr int			NUM_SERIAL_NUM_BITS =			16;
static constexpr int			NUM_SERIAL_NUM_SHIFT_BITS =		(32 - NUM_SERIAL_NUM_BITS);
static constexpr unsigned int	ENT_ENTRY_MASK =				((1 << NUM_SERIAL_NUM_BITS) - 1); // 0xFFF

class CBaseHandle;

abstract_class IHandleEntity
{
public:
	virtual					~IHandleEntity() {}
	virtual void			SetRefEHandle(const CBaseHandle& Handle) = 0;
	virtual CBaseHandle&	GetRefEHandle() const = 0;
};

class CBaseHandle
{
	friend class CBaseEntityList;
protected:
	unsigned long m_Index;
public:
	CBaseHandle() { m_Index = INVALID_EHANDLE_INDEX; };
	CBaseHandle(const CBaseHandle& other) { m_Index = other.m_Index; }
	CBaseHandle(unsigned long value) { m_Index = value; }
	CBaseHandle(int iEntry, int iSerialNumber) { Init(iEntry, iSerialNumber); }

	void Init(int iEntry, int iSerialNumber) { m_Index = (unsigned long)(iEntry | (iSerialNumber << NUM_SERIAL_NUM_SHIFT_BITS)); }
	void Term() { m_Index = INVALID_EHANDLE_INDEX; };

	bool IsValid() const { return m_Index != INVALID_EHANDLE_INDEX; }

	int GetEntryIndex() const
	{
		if (!IsValid()) return NUM_ENT_ENTRIES - 1;
		return m_Index & ENT_ENTRY_MASK;
	}
	int GetSerialNumber() const { return m_Index >> NUM_SERIAL_NUM_SHIFT_BITS; }
	IHandleEntity* Get() const { return nullptr; }

	// Assign a value to the handle.
	const CBaseHandle& Set(const IHandleEntity *pEntity)
	{
		if (pEntity)
			*this = pEntity->GetRefEHandle();
		else
			m_Index = INVALID_EHANDLE_INDEX;

		return *this;
	}

	int ToInt() const { return (int)m_Index; }
	bool operator !=(const CBaseHandle &other) const { return m_Index != other.m_Index; }
	bool operator ==(const CBaseHandle &other) const { return m_Index == other.m_Index; }
	bool operator ==(const IHandleEntity* pEnt) const { return Get() == pEnt; }
	bool operator !=(const IHandleEntity* pEnt) const { return Get() != pEnt; }
	bool operator <(const CBaseHandle &other) const { return m_Index < other.m_Index; }
	bool operator <(const IHandleEntity* pEnt) const 
	{
		unsigned long otherIndex = (pEnt) ? pEnt->GetRefEHandle().m_Index : INVALID_EHANDLE_INDEX;
		return m_Index < otherIndex;
	}
};

template<class T>
class CHandle : public CBaseHandle
{
public:
	CHandle() {}
	CHandle(int iEntry, int iSerialNumber) { Init(iEntry, iSerialNumber); }
	CHandle(const CBaseHandle& handle) : CBaseHandle(handle) {}
	CHandle(T* pVal) { Term(); Set(pVal); }

	static CHandle<T> FromIndex(int index)
	{
		CHandle<T> ret;
		ret.m_Index = index;
		return ret;
	}

	inline T* Get() const { return (T*)CBaseHandle::Get(); }
	void Set(const T* pVal) { CBaseHandle::Set(reinterpret_cast<const IHandleEntity*>(pVal)); }

	operator T*() { return Get(); }
	operator T*() const { return Get(); }

	inline bool operator !() const { return !Get(); }
	inline bool operator==(T *val) const { return Get() == val };
	inline bool operator!=(T *val) const { return Get() != val };
	inline const CBaseHandle& operator=(const T *val) { Set(val); return *this; }

	inline T* operator->() const { return Get(); }
};

using EHANDLE = unsigned int;

SOURCE_END