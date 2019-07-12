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

namespace CRC
{
	using CRC32_t = unsigned long;

	void			CRC32_Init(CRC32_t* pulCRC);
	void			CRC32_ProcessBuffer(CRC32_t *pulCRC, const void *p, int len);
	void			CRC32_Final(CRC32_t *pulCRC);
	CRC32_t			CRC32_GetTableEntry(unsigned int slot);

	inline CRC32_t	CRC32_ProcessSingleBuffer(const void *p, int len)
	{
		CRC32_t crc;
		CRC32_Init(&crc);
		CRC32_ProcessBuffer(&crc, p, len);
		CRC32_Final(&crc);
		return crc;
	}
}

SOURCE_END