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
#include "../../Classes/Maths/Vector.h"

// TODO: Complete this vtable

SOURCE_INIT

abstract_class IVDebugOverlay
{
public:
	void AddBoxOverlay(const Vector& origin, const Vector& mins, const Vector& max, QAngle const& orientation, int r, int g, int b, int a, float duration)
	{
		typedef void(__thiscall* OriginalFn)(void*, const Vector&, const Vector&, const Vector&, QAngle const&, int, int, int, int, float);
		CallVFunction<OriginalFn>(this, 3)(this, origin, mins, max, orientation, r, g, b, a, duration);
	}
	void AddLineOverlay(const Vector& origin, const Vector& dest, int r, int g, int b, bool noDepthTest, float duration)
	{
		typedef void(__thiscall* OriginalFn)(void*, const Vector& origin, const Vector& dest, int r, int g, int b, bool noDepthTest, float duration);
		CallVFunction<OriginalFn>(this, 5)(this, origin, dest, r, g, b, noDepthTest, duration);
	}
	void AddSweptBoxOverlay(const Vector& origin, const Vector& end, const Vector& mins, const Vector& max, QAngle const& orientation, int r, int g, int b, int a, float duration)
	{
		typedef void(__thiscall* OriginalFn)(void*, const Vector&, const Vector&, const Vector&, const Vector&, QAngle const&, int, int, int, int, float);
		CallVFunction<OriginalFn>(this, 9)(this, origin, end, mins, max, orientation, r, g, b, a, duration);
	}
	bool ScreenPosition(const Vector& vIn, Vector& vOut)
	{
		typedef bool(__thiscall* OriginalFn)(void*, const Vector&, Vector&);
		return CallVFunction<OriginalFn>(this, 13)(this, vIn, vOut);
	}
	// A pill is an hitbox capsule
	void AddPill(const Vector& mins, const Vector& max, float& diameter, int r, int g, int b, int a, float duration)
	{
		typedef void(__thiscall* OriginalFn)(void*, const Vector&, const Vector&, float&, int, int, int, int, float);
		CallVFunction<OriginalFn>(this, 23)(this, mins, max, diameter, r, g, b, a, duration);
	}
};

SOURCE_END