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

enum InitReturnVal_t
{
	INIT_FAILED = 0,
	INIT_OK,
	INIT_LAST_VAL,
};

struct AppSystemInfo_t
{
	const char* m_pModuleName;
	const char* m_pInterfaceName;
};

typedef void*(*CreateInterfaceFn)(const char* szName, int* iReturnCode);
using pfnDemoCustomDataCallback = void(unsigned char* pData, size_t iSize);

abstract_class IBaseInterface
{
public:
	virtual	~IBaseInterface() {}
};

abstract_class IAppSystem
{
public:
	virtual bool			Connect(CreateInterfaceFn Factory) = 0;
	virtual void			Disconnect() = 0;
	virtual void*			QueryInterface(const char* pInterfaceName) = 0;
	virtual InitReturnVal_t Init() = 0;
	virtual void			Shutdown() = 0;
	virtual AppSystemInfo_t GetDependencies() = 0;
	virtual int				GetTier() = 0;
	virtual void			Reconnect(CreateInterfaceFn Factory, const char* pInterfaceName) = 0;
	virtual void			UnknownFunction() = 0;
};

abstract_class IRefCounted
{
private:
	volatile long refCount;
public:
	virtual void destructor(char bDelete) = 0;
	virtual bool OnFinalRelease() = 0;
	void unreference() {
		if (InterlockedDecrement(&refCount) == 0 && OnFinalRelease()) {
			destructor(1);
		}
	}
};

SOURCE_END