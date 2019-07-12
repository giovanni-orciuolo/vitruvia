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
#include "Vector.h"

SOURCE_INIT

class Quaternion;

class RadianEuler
{
public:
	inline RadianEuler(void) { }
	inline RadianEuler(float X, float Y, float Z) { x = X; y = Y; z = Z; }
	inline RadianEuler(Quaternion const &q);	// evil auto type promotion!!!
	inline RadianEuler(QAngle const &angles);	// evil auto type promotion!!!

	// Initialization
	inline void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f) { x = ix; y = iy; z = iz; }

	//	conversion to qangle
	QAngle ToQAngle(void) const;
	bool IsValid() const;
	void Invalidate();

	inline float *Base() { return &x; }
	inline const float *Base() const { return &x; }

	// array access...
	float operator[](int i) const;
	float& operator[](int i);

	float x, y, z;
};

class Quaternion				// same data-layout as engine's vec4_t,
{								//		which is a float[4]
public:
	inline Quaternion(void) {}
	inline Quaternion(float ix, float iy, float iz, float iw) : x(ix), y(iy), z(iz), w(iw) { }
	inline Quaternion(RadianEuler const &angle);	// evil auto type promotion!!!

	inline void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f, float iw = 0.0f) { x = ix; y = iy; z = iz; w = iw; }

	bool IsValid() const;
	void Invalidate();

	bool operator==(const Quaternion &src) const;
	bool operator!=(const Quaternion &src) const;

	float* Base() { return (float*)this; }
	const float* Base() const { return (float*)this; }

	// array access...
	float operator[](int i) const;
	float& operator[](int i);

	float x, y, z, w;
};

SOURCE_END