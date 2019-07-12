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
#include "CRC\CRC.h"
#include "Maths\Vector.h"

#define	FL_ONGROUND					(1 << 0)	
#define FL_DUCKING					(1 << 1)	
#define	FL_WATERJUMP				(1 << 2)	
#define FL_ONTRAIN					(1 << 3)
#define FL_INRAIN					(1 << 4)	
#define FL_FROZEN					(1 << 5)
#define FL_ATCONTROLS				(1 << 6)
#define	FL_CLIENT					(1 << 7)	
#define FL_FAKECLIENT				(1 << 8)	
#define	FL_INWATER					(1 << 9)										
#define	FL_FLY						(1 << 10)	
#define	FL_SWIM						(1 << 11)
#define	FL_CONVEYOR					(1 << 12)
#define	FL_NPC						(1 << 13)
#define	FL_GODMODE					(1 << 14)
#define	FL_NOTARGET					(1 << 15)
#define	FL_AIMTARGET				(1 << 16)	
#define	FL_PARTIALGROUND			(1 << 17)	
#define FL_STATICPROP				(1 << 18)
#define FL_GRAPHED					(1 << 19) 
#define FL_GRENADE					(1 << 20)
#define FL_STEPMOVEMENT				(1 << 21)	
#define FL_DONTTOUCH				(1 << 22)
#define FL_BASEVELOCITY				(1 << 23)
#define FL_WORLDBRUSH				(1 << 24)	
#define FL_OBJECT					(1 << 25) 
#define FL_KILLME					(1 << 26)
#define FL_ONFIRE					(1 << 27)	
#define FL_DISSOLVING				(1 << 28)
#define FL_TRANSRAGDOLL				(1 << 29)
#define FL_UNBLOCKABLE_BY_PLAYER	(1 << 30) 
#define PLAYER_FLAG_BITS			10

#define IN_ATTACK			(1 << 0)
#define IN_JUMP				(1 << 1)
#define IN_DUCK				(1 << 2)
#define IN_FORWARD			(1 << 3)
#define IN_BACK				(1 << 4)
#define IN_USE				(1 << 5)
#define IN_CANCEL			(1 << 6)
#define IN_LEFT				(1 << 7)
#define IN_RIGHT			(1 << 8)
#define IN_MOVELEFT			(1 << 9)
#define IN_MOVERIGHT		(1 << 10)
#define IN_ATTACK2			(1 << 11)
#define IN_RUN				(1 << 12)
#define IN_RELOAD			(1 << 13)
#define IN_ALT1				(1 << 14)
#define IN_ALT2				(1 << 15)
#define IN_SCORE			(1 << 16)   // Used by client.dll for when scoreboard is held down
#define IN_SPEED			(1 << 17)	// Player is holding the speed key
#define IN_WALK				(1 << 18)	// Player holding walk key
#define IN_ZOOM				(1 << 19)	// Zoom key for HUD zoom
#define IN_WEAPON1			(1 << 20)	// weapon defines these bits
#define IN_WEAPON2			(1 << 21)	// weapon defines these bits
#define IN_BULLRUSH			(1 << 22)
#define IN_GRENADE1			(1 << 23)	// grenade 1
#define IN_GRENADE2			(1 << 24)	// grenade 2
#define IN_LOOKSPIN			(1 << 25)
#define MULTIPLAYER_BACKUP  150

SOURCE_INIT

class bf_read;
class bf_write;

class CUserCmd
{
public:
	CUserCmd(): command_number(0), tick_count(0), forwardmove(0), sidemove(0), upmove(0), buttons(0), impulse(0),
	            weaponselect(0), weaponsubtype(0), random_seed(0), mousedx(0), mousedy(0), hasbeenpredicted(false)
	{
	}

	virtual ~CUserCmd() {}

	CRC::CRC32_t GetChecksum(void) const
	{
		using namespace CRC;
		CRC32_t crc;
		CRC32_Init(&crc);

		CRC32_ProcessBuffer(&crc, &command_number, sizeof(command_number));
		CRC32_ProcessBuffer(&crc, &tick_count, sizeof(tick_count));
		CRC32_ProcessBuffer(&crc, &viewangles, sizeof(viewangles));
		CRC32_ProcessBuffer(&crc, &aimdirection, sizeof(aimdirection));
		CRC32_ProcessBuffer(&crc, &forwardmove, sizeof(forwardmove));
		CRC32_ProcessBuffer(&crc, &sidemove, sizeof(sidemove));
		CRC32_ProcessBuffer(&crc, &upmove, sizeof(upmove));
		CRC32_ProcessBuffer(&crc, &buttons, sizeof(buttons));
		CRC32_ProcessBuffer(&crc, &impulse, sizeof(impulse));
		CRC32_ProcessBuffer(&crc, &weaponselect, sizeof(weaponselect));
		CRC32_ProcessBuffer(&crc, &weaponsubtype, sizeof(weaponsubtype));
		CRC32_ProcessBuffer(&crc, &random_seed, sizeof(random_seed));
		CRC32_ProcessBuffer(&crc, &mousedx, sizeof(mousedx));
		CRC32_ProcessBuffer(&crc, &mousedy, sizeof(mousedy));

		CRC32_Final(&crc);
		return crc;
	}

	int       command_number;     // 0x04
	int       tick_count;         // 0x08
	Vector    viewangles;         // 0x0C
	Vector    aimdirection;       // 0x18
	float     forwardmove;        // 0x24 -> x
	float     sidemove;           // 0x28 -> y
	float     upmove;             // 0x2C -> z
	int       buttons;            // 0x30
	byte      impulse;            // 0x34
	int       weaponselect;       // 0x38
	int       weaponsubtype;      // 0x3C
	int       random_seed;        // 0x40
	short     mousedx;            // 0x44
	short     mousedy;            // 0x46
	bool      hasbeenpredicted;   // 0x48
	char      pad_0x4C[0x18];     // 0x4C
};

class CVerifiedUserCmd
{
public:
	CUserCmd		m_cmd;
	CRC::CRC32_t	m_crc;
};

class Input_t
{
private:
	void*               pvftable;                     //0x00
public:
	bool                m_fTrackIRAvailable;          //0x04
	bool                m_fMouseInitialized;          //0x05
	bool                m_fMouseActive;               //0x06
	bool                m_fJoystickAdvancedInit;      //0x07
private:
	char                pad_0x08[0x2C];               //0x08
public:
	void*               m_pKeys;                      //0x34
private:
	char                pad_0x38[0x64];               //0x38
	int					pad_0x41;					  //0x9C
	int					pad_0x42;					  //0xA0
public:
	bool                m_fCameraInterceptingMouse;   //0xA1
	bool                m_fCameraInThirdPerson;       //0xA2
	bool                m_fCameraMovingWithMouse;     //0xA3
	Vector				m_vecCameraOffset;            //0xA4
	char				pad_hurensohn[3];			  //0xB0
	bool                m_fCameraDistanceMove;        //0xB3
	int                 m_nCameraOldX;                //0xB4
	int                 m_nCameraOldY;                //0xB8
	int                 m_nCameraX;                   //0xBC
	int                 m_nCameraY;                   //0xC0
	bool                m_CameraIsOrthographic;       //0xC4
	Vector              m_angPreviousViewAngles;      //0xC8
	Vector              m_angPreviousViewAnglesTilt;  //0xD4
	float               m_flLastForwardMove;          //0xE0
	int                 m_nClearInputState;           //0xE4
	CUserCmd*           m_pCommands;                  //0xE8
	CVerifiedUserCmd*   m_pVerifiedCommands;          //0xEC

	// Helper methods
	CUserCmd* GetUserCmd(int sequence_number)
	{
		CUserCmd* cmd = &m_pCommands[sequence_number % MULTIPLAYER_BACKUP];
		if (cmd->command_number != sequence_number)
			return nullptr;
		return cmd;
	}
	CVerifiedUserCmd* GetVerifiedUserCmd(int sequence_number)
	{
		return &m_pVerifiedCommands[sequence_number % MULTIPLAYER_BACKUP];
	}
	void ValidateCommand(CUserCmd* cmd, CVerifiedUserCmd* verified_cmd)
	{
		verified_cmd->m_cmd = *cmd;
		verified_cmd->m_crc = cmd->GetChecksum();
	}

	// Virtual functions
	CUserCmd* GetUserCmd(int slot, int sequence_number)
	{
		return CallVFunction<CUserCmd*(__thiscall*)(void*,int,int)>(this, 8)(this, slot, sequence_number);
	}
};

SOURCE_END