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
#include <Windows.h>
#include <smmintrin.h>
#include <cstdint>
#include <cmath>
#include <limits>
#include <iostream>
#include <sstream>

#define SOURCE_INIT namespace Source {
#define SOURCE_END }

#define VGUI_INIT namespace VGUI {
#define VGUI_END }

#define MATHS_INIT namespace Maths {
#define MATHS_END }

#define abstract_class class
#define Assert(_exp) ((void)0)
using matrix3x4_t = float[3][4];
using VMatrix = matrix3x4_t;
using VPANEL = unsigned int;
using HFont = unsigned long;
using HScheme = unsigned long;
using HTexture = unsigned long;
using HCursor = unsigned long;
using HPanel = unsigned long;

static constexpr HPanel INVALID_PANEL = 0xFFFFFFFF;
static constexpr HFont INVALID_FONT = 0; // the value of an invalid font handle
static constexpr int MAX_CLIENTS = 64;

template<typename FuncType>
inline FuncType CallVFunction(void* ppClass, DWORD index)
{
	PDWORD pVTable = *(PDWORD*)ppClass;
	DWORD dwAddress = pVTable[index];
	return (FuncType)(dwAddress);
}
inline DWORD GetVFunctionAddress(void* ppClass, DWORD index)
{
	PDWORD pVTable = *(PDWORD*)ppClass;
	DWORD dwAddress = pVTable[index];
	return dwAddress;
}

#define IsInfinite(x)	(!_finite(x))
#define IsFinite(x)		(_finite(x))

static constexpr float M_PI = 3.14159265358979323846f;
static constexpr float M_PI_F = (static_cast<float>(M_PI));
static constexpr float M_RADPI = 57.295779513082f;
static constexpr float M_RADPI_F = (static_cast<float>(M_RADPI));

#define RAD2DEG(x)	( (float)(x) * (float)(180.f / M_PI_F) )
#define DEG2RAD(x)	( (float)(x) * (float)(M_PI_F / 180.f) )

template< class T = int, class Y = int >
inline T clamp(T const &val, Y const &minVal, Y const &maxVal)
{
	if (val < minVal)
		return minVal;
	else if (val > maxVal)
		return maxVal;
	else
		return val;
}

using vec_t = float;
using int8 = __int8;
using uint8 = unsigned __int8;
using int16 = __int16;
using uint16 = unsigned __int16;
using int32 = __int32;
using uint32 = unsigned __int32;
using int64 = __int64;
using uint64 = unsigned __int64;

inline uint32& FloatBits(vec_t& f)
{
	return *reinterpret_cast<uint32*>((char*)(&f));
}

inline uint32 const FloatBits(const vec_t &f)
{
	union Convertor_t
	{
		vec_t f;
		uint32 ul;
	}tmp;
	tmp.f = f;
	return tmp.ul;
}

inline vec_t BitsToFloat(uint32 i)
{
	union Convertor_t
	{
		vec_t f;
		unsigned long ul;
	}tmp;
	tmp.ul = i;
	return tmp.f;
}

inline float FloatNegate(vec_t f)
{
	return BitsToFloat(FloatBits(f) ^ 0x80000000);
}

static constexpr uint32 FLOAT32_NAN_BITS = (uint32)0x7FC00000; // not a number!
#define FLOAT32_NAN BitsToFloat( FLOAT32_NAN_BITS )

template<typename T>
constexpr T minof()
{
	return (T)1 << (sizeof(T) * 8 - 1);
}

template<typename T>
constexpr T maxof()
{
	return ~((T)1 << (sizeof(T) * 8 - 1));
}

template<typename T = int>
constexpr T RandomRange(const T& low, const T& high)
{
	return (rand() % (high - low + 1) + low);
}

#pragma warning(disable : 4244)

#include "SourceEnums.h"