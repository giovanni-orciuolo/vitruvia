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

#define MD5_DIGEST_LENGTH 16  // 16 bytes == 128 bit digest

SOURCE_INIT

namespace MD5
{
	// MD5 Hash
	typedef struct
	{
		unsigned int buf[4];
		unsigned int bits[2];
		unsigned char in[64];
	} MD5Context_t;
	extern void MD5Init(MD5Context_t *context);
	extern void MD5Update(MD5Context_t *context, unsigned char const *buf, unsigned int len);
	extern void MD5Final(unsigned char digest[MD5_DIGEST_LENGTH], MD5Context_t *context);
	extern void MD5Transform(unsigned int buf[4], unsigned int const in[16]);
	extern unsigned int MD5_PseudoRandom(unsigned int nSeed);
}

SOURCE_END