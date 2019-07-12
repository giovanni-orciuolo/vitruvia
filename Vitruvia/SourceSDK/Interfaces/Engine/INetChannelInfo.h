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

#define FLOW_OUTGOING	0		
#define FLOW_INCOMING	1
#define MAX_FLOWS		2		// in & out

abstract_class INetChannelInfo
{
public:
	enum
	{
		GENERIC = 0,	// must be first and is default group
		LOCALPLAYER,	// bytes for local player entity update
		OTHERPLAYERS,	// bytes for other players update
		ENTITIES,		// all other entity bytes
		SOUNDS,			// game sounds
		EVENTS,			// event messages
		USERMESSAGES,	// user messages
		ENTMESSAGES,	// entity messages
		VOICE,			// voice data
		STRINGTABLE,	// a stringtable update
		MOVE,			// client move cmds
		STRINGCMD,		// string command
		SIGNON,			// various signondata
		TOTAL,			// must be last and is not a real group
	};

	virtual const char  *GetName(void) const = 0;	// get channel name
	virtual const char  *GetAddress(void) const = 0; // get channel IP address as string
	virtual float		GetTime(void) const = 0;	// current net time
	virtual float		GetTimeConnected(void) const = 0;	// get connection time in seconds
	virtual int			GetBufferSize(void) const = 0;	// netchannel packet history size
	virtual int			GetDataRate(void) const = 0; // send data rate in byte/sec

	virtual bool		IsLoopback(void) const = 0;	// true if loopback channel
	virtual bool		IsTimingOut(void) const = 0;	// true if timing out
	virtual bool		IsPlayback(void) const = 0;	// true if demo playback

	virtual float		GetLatency(int flow) const = 0;	 // current latency (RTT), more accurate but jittering
	virtual float		GetAvgLatency(int flow) const = 0; // average packet latency in seconds
	virtual float		GetAvgLoss(int flow) const = 0;	 // avg packet loss[0..1]
	virtual float		GetAvgChoke(int flow) const = 0;	 // avg packet choke[0..1]
	virtual float		GetAvgData(int flow) const = 0;	 // data flow in bytes/sec
	virtual float		GetAvgPackets(int flow) const = 0; // avg packets/sec
	virtual int			GetTotalData(int flow) const = 0;	 // total flow in/out in bytes
	virtual int			GetSequenceNr(int flow) const = 0;	// last send seq number
	virtual bool		IsValidPacket(int flow, int frame_number) const = 0; // true if packet was not lost/dropped/chocked/flushed
	virtual float		GetPacketTime(int flow, int frame_number) const = 0; // time when packet was send
	virtual int			GetPacketBytes(int flow, int frame_number, int group) const = 0; // group size of this packet
	virtual bool		GetStreamProgress(int flow, int *received, int *total) const = 0;  // TCP progress if transmitting
	virtual float		GetTimeSinceLastReceived(void) const = 0;	// get time since last recieved packet in seconds
	virtual	float		GetCommandInterpolationAmount(int flow, int frame_number) const = 0;
	virtual void		GetPacketResponseLatency(int flow, int frame_number, int *pnLatencyMsecs, int *pnChoke) const = 0;
	virtual void		GetRemoteFramerate(float *pflFrameTime, float *pflFrameTimeStdDeviation) const = 0;

	virtual float		GetTimeoutSeconds() const = 0;
};

struct INetMessage {
	virtual ~INetMessage();
};

template<typename T>
class CNetMessagePB : public INetMessage, public T {};

class CCLCMsg_Move {
	char __PAD0[0x8];
public:
	int numBackupCommands;
	int numNewCommands;
};

using CCLCMsg_Move_t = CNetMessagePB<CCLCMsg_Move>;

class CNetChannel
{
public:
	int __vtable;								//0x0000 
	void* msgbinder1;							//0x0004 
	void* msgbinder2;							//0x0008 
	void* msgbinder3;							//0x000C 
	void* msgbinder4;							//0x0010 
	unsigned char m_bProcessingMessages;		//0x0014 
	unsigned char m_bShouldDelete;				//0x0015 
	char pad_0x0016[0x2];						//0x0016
	int m_nOutSequenceNr;						//0x0018
	int m_nInSequenceNr;						//0x001C
	int m_nOutSequenceNrAck;					//0x0020
	int m_nOutReliableState;					//0x0024
	int m_nInReliableState;						//0x0028
	int m_nChokedPackets;						//0x002C
	char pad_0030[108];							//0x0030
	int m_Socket;								//0x009C
	int m_StreamSocket;							//0x00A0
	int m_MaxReliablePayloadSize;				//0x00A4
	char pad_00A8[100];							//0x00A8
	float last_received;						//0x010C
	float connect_time;							//0x0110
	char pad_0114[4];							//0x0114
	int m_Rate;									//0x0118
	char pad_011C[4];							//0x011C
	float m_fClearTime;							//0x0120
	char pad_0124[16688];						//0x0124
	char m_Name[32];							//0x4254
	unsigned int m_ChallengeNr;					//0x4274
	float m_flTimeout;							//0x4278
	char pad_427C[32];							//0x427C
	float m_flInterpolationAmount;				//0x429C
	float m_flRemoteFrameTime;					//0x42A0
	float m_flRemoteFrameTimeStdDeviation;		//0x42A4
	int m_nMaxRoutablePayloadSize;				//0x42A8
	int m_nSplitPacketSequence;					//0x42AC
	char pad_42B0[40];							//0x42B0
	bool m_bIsValveDS;							//0x42D8
	char pad_42D9[65];							//0x42D9
	bool Transmit(bool only_reliable)
	{
		return CallVFunction<bool(__thiscall*)(void*, bool)>(this, 49)(this, only_reliable);
	}
};	//Size: 0x431A

SOURCE_END