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
#include "../IAppSystem.h"
#include "../../Classes/CGlobalVarsBase.h"
#include "../../Classes/Recv/ClientClass.h"

SOURCE_INIT

abstract_class IBaseClientDLL
{
public:
	virtual int              Connect(CreateInterfaceFn appSystemFactory, CGlobalVarsBase* pGlobals) = 0;
	virtual int              Disconnect() = 0;
	virtual int              Init(CreateInterfaceFn appSystemFactory, CGlobalVarsBase* pGlobals) = 0;
	virtual void             PostInit() = 0;
	virtual void             Shutdown() = 0;
	virtual void             LevelInitPreEntity(char const* pMapName) = 0;
	virtual void             LevelInitPostEntity() = 0;
	virtual void             LevelShutdown() = 0;
	virtual ClientClass*     GetAllClasses() = 0;
};

SOURCE_END