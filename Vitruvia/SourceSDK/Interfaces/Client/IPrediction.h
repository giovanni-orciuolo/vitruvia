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
#include "IGameMovement.h"

SOURCE_INIT

class C_BasePlayer;

abstract_class IMoveHelper
{
public:
	virtual void pad() = 0;
	virtual void SetHost(C_BasePlayer* pEntityHost) = 0;
};

abstract_class IPrediction 
{
private:
	virtual void pad0() = 0;
	virtual void pad1() = 0;
	virtual void pad2() = 0;
	virtual void pad3() = 0;
	virtual void pad4() = 0;
	virtual void pad5() = 0;
	virtual void pad6() = 0;
	virtual void pad7() = 0;
	virtual void pad8() = 0;
	virtual void pad9() = 0;
	virtual void pad10() = 0;
	virtual void pad11() = 0;
	virtual void pad12() = 0;
	virtual void pad13() = 0;
public:
	virtual bool InPrediction() = 0; // 14
private:
	virtual void pad15() = 0;
	virtual void pad16() = 0;
public:
	virtual void GetLocalViewAngles(QAngle& ang) = 0; // 17
	virtual void SetLocalViewAngles(QAngle& ang) = 0; // 18
	virtual void RunCommand(C_BasePlayer* pEntity, void* uCmd, IMoveHelper* pHelper) = 0; // 19
	virtual void SetupMove(C_BasePlayer* pEntity, void* uCmd, IMoveHelper* pHelper, CMoveData* pMoveData) = 0; //20
	virtual void FinishMove(C_BasePlayer* pEntity, void* uCmd, CMoveData* pMoveData) = 0; // 21
};

SOURCE_END