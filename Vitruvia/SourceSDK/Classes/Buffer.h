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

// Credits: beakers

class bf_read
{
public:
	uintptr_t base_address;
	uintptr_t cur_offset;

	bf_read::bf_read(uintptr_t addr)
	{
		base_address = addr;
		cur_offset = 0;
	}

	void bf_read::SetOffset(uintptr_t offset)
	{
		cur_offset = offset;
	}

	void bf_read::Skip(uintptr_t length)
	{
		cur_offset += length;
	}

	int bf_read::ReadByte()
	{
		auto val = *reinterpret_cast<char*>(base_address + cur_offset);
		++cur_offset;
		return val;
	}

	bool bf_read::ReadBool()
	{
		auto val = *reinterpret_cast<bool*>(base_address + cur_offset);
		++cur_offset;
		return val;
	}

	std::string bf_read::ReadString()
	{
		char buffer[256];
		auto str_length = *reinterpret_cast<char*>(base_address + cur_offset);
		++cur_offset;
		memcpy(buffer, reinterpret_cast<void*>(base_address + cur_offset), str_length > 255 ? 255 : str_length);
		buffer[str_length > 255 ? 255 : str_length] = '\0';
		cur_offset += str_length + 1;
		return std::string(buffer);
	}
};

SOURCE_END