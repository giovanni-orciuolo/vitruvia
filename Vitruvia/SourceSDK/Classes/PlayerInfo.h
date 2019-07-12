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

#define MAX_PLAYER_NAME_LENGTH 32
#define SIGNED_GUID_LEN 32

SOURCE_INIT

typedef struct player_info_s 
{
private:
	int64_t unknown;
public:
	union {
		int64_t xuid;
		struct {
			int xuid_low;
			int xuid_high;
		};
	};
	char name[MAX_PLAYER_NAME_LENGTH + 96];
	int userid;
	int m_nUserID;
	char guid[SIGNED_GUID_LEN + 1];
	unsigned int friendsid;
	char friendsname[MAX_PLAYER_NAME_LENGTH + 96];
	bool fakeplayer;
	bool ishltv;
	unsigned int customfiles[4];
	unsigned char filesdownloaded;
} player_info_t;

SOURCE_END