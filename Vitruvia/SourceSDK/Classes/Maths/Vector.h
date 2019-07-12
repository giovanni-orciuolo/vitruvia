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

#include "../../../Vitruvia/GUI/ImGui/imgui.h"
#include "../../../Vitruvia/GUI/ImGui/imgui_internal.h"
#include "../../../Vitruvia/GUI/ImGui/DX9/imgui_impl_dx9.h"

SOURCE_INIT

class Vector2;

class Vector
{
public:
	float x, y, z;

	float*	Base() { return (float*)this; }
	float*	Base() const { return (float*)this; }

	inline void Init(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }

	Vector() { Init(0, 0, 0); };
	Vector(float _x, float _y, float _z = 0) { Init(_x, _y, _z); };
	Vector(float* v) { Init(v[0], v[1], v[2]); }
	Vector(const float* v) { Init(v[0], v[1], v[2]); }
	Vector(const Vector& v) { Init(v.x, v.y, v.z); }

	inline Vector& operator= (const Vector& v) { x = v.x; y = v.y; z = v.z; return *this; }
	inline Vector& operator= (float f) { x = f; y = f; z = f; return *this; }

	inline float& operator[] (int i) { return (Base())[i]; }
	inline float operator[]	 (int i) const { return (Base())[i]; }

	inline Vector& operator+=(const Vector& v) { x += v.x; y += v.y; z += v.z; return *this; }
	inline Vector& operator-=(const Vector& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
	inline Vector& operator*=(const Vector& v) { x *= v.x; y *= v.y; z *= v.z; return *this; }
	inline Vector& operator/=(const Vector& v) { x /= v.x; y /= v.y; z /= v.z; return *this; }

	inline Vector& operator+=(float f) { x += f; y += f; z += f; return *this; }
	inline Vector& operator-=(float f) { x -= f; y -= f; z -= f; return *this; }
	inline Vector& operator*=(float f) { x *= f; y *= f; z *= f; return *this; }
	inline Vector& operator/=(float f) { x /= f; y /= f; z /= f; return *this; }

	inline Vector operator+  (const Vector& v) const { return Vector(x + v.x, y + v.y, z + v.z); }
	inline Vector operator-  (const Vector& v) const { return Vector(x - v.x, y - v.y, z - v.z); }
	inline Vector operator*  (const Vector& v) const { return Vector(x * v.x, y * v.y, z * v.z); }
	inline Vector operator/  (const Vector& v) const { return Vector(x / v.x, y / v.y, z / v.z); }

	inline Vector operator+  (float f) const { return Vector(x + f, y + f, z + f); }
	inline Vector operator-  (float f) const { return Vector(x - f, y - f, z - f); }
	inline Vector operator*  (float f) const { return Vector(x * f, y * f, z * f); }
	inline Vector operator/  (float f) const { return Vector(x / f, y / f, z / f); }

	inline bool operator==	 (const Vector& v) const { return (x == v.x && y == v.y && z == v.z); }
	inline bool operator!=	 (const Vector& v) const { return (x != v.x && y != v.y && z != v.z); }

	inline bool operator==	 (float f) const { return (x == f && y == f && z == f); }
	inline bool operator!=	 (float f) const { return (x != f && y != f && z != f); }

	inline bool IsZero() const { return (x > -0.01f && x < 0.01f && y > -0.01f && y < 0.01f && z > -0.01f && z < 0.01f); }
	inline bool IsNull() const { return (x == 0 && y == 0 && z == 0); }
	inline void Reset() { Init(0, 0, 0); }

	inline float LengthSqr() { return (x*x + y*y + z*z); }
	inline float Length() { return sqrtf(x*x + y*y + z*z); }
	inline float Length2DSqr() { return (x*x + y*y); }
	inline float Length2D() { return sqrtf(x*x + y*y); }
	inline float DistTo(const Vector& v) const { return (*this - v).Length(); }
	inline float DistToSqr(const Vector& v) const { return (*this - v).LengthSqr(); }
	inline float Dot(const Vector& v) const { return (x * v.x + y * v.y + z * v.z); }

	inline void Print(const std::string& vector_name) { std::cout << vector_name.c_str() << ": [" << x << "," << y << "," << z << "]" << std::endl; }

	auto Normalize() {
		(*this) /= Length();
	}
	Vector Normalized()
	{
		Vector vector;
		float length = this->Length();
		(length != 0) ?
			vector /= length : vector.Reset();
		return vector;
	}
	void Rotate(float f)
	{
		float _x, _y;
		float s = sinf(DEG2RAD(f)), c = cosf(DEG2RAD(f));
		_x = x;
		_y = y;
		x = (_x * c) - (_y * s);
		y = (_x * s) + (_y * c);
	}

	std::string ToString() const
	{
		std::stringstream ss;
		ss << "[ x = " << x << ", y = " << y << ", z = " << z << " ]";
		return ss.str();
	}

	void MultAdd(const Vector& vOther, float fl) 
	{
		x += fl * vOther.x;
		y += fl * vOther.y;
		z += fl * vOther.z;
	}

	void Clamp()
	{
		while (this->x < -89.0f)
			this->x += 89.0f;

		if (this->x > 89.0f)
			this->x = 89.0f;

		while (this->y < -180.0f)
			this->y += 360.0f;

		while (this->y > 180.0f)
			this->y -= 360.0f;

		this->z = 0.0f;
	}

	Vector Clamped() 
	{
		Vector tmp = *this;
		tmp.Clamp();
		return tmp;
	}

	operator ImVec2() { return ImVec2(x, y); }

};

class Vector2
{
public:
	float x, y;

	float*	Base() { return (float*)this; }
	float*	Base() const { return (float*)this; }

	inline void Init(float _x, float _y) { x = _x; y = _y; }

	Vector2() { Init(0, 0); }
	Vector2(float _x, float _y) { Init(_x, _y); }
	Vector2(float* v) { Init(v[0], v[1]); }
	Vector2(const float* v) { Init(v[0], v[1]); }
	Vector2(const Vector2& v) { Init(v.x, v.y); }
	Vector2(const Vector& v) { Init(v.x, v.y); }

	inline Vector2& operator= (const Vector& v) { x = v.x; y = v.y; return *this; }
	inline Vector2& operator= (float f) { x = f; y = f; return *this; }

	inline float& operator[] (int i) { return (Base())[i]; }
	inline float operator[]	 (int i) const { return (Base())[i]; }

	inline Vector2& operator+=(const Vector2& v) { x += v.x; y += v.y; return *this; }
	inline Vector2& operator-=(const Vector2& v) { x -= v.x; y -= v.y; return *this; }
	inline Vector2& operator*=(const Vector2& v) { x *= v.x; y *= v.y; return *this; }
	inline Vector2& operator/=(const Vector2& v) { x /= v.x; y /= v.y; return *this; }

	inline Vector2& operator+=(float f) { x += f; y += f; return *this; }
	inline Vector2& operator-=(float f) { x -= f; y -= f; return *this; }
	inline Vector2& operator*=(float f) { x *= f; y *= f; return *this; }
	inline Vector2& operator/=(float f) { x /= f; y /= f; return *this; }

	inline Vector2 operator+  (const Vector2& v) const { return Vector2(x + v.x, y + v.y); }
	inline Vector2 operator-  (const Vector2& v) const { return Vector2(x - v.x, y - v.y); }
	inline Vector2 operator*  (const Vector2& v) const { return Vector2(x * v.x, y * v.y); }
	inline Vector2 operator/  (const Vector2& v) const { return Vector2(x / v.x, y / v.y); }

	inline Vector2 operator+  (float f) const { return Vector2(x + f, y + f); }
	inline Vector2 operator-  (float f) const { return Vector2(x - f, y - f); }
	inline Vector2 operator*  (float f) const { return Vector2(x * f, y * f); }
	inline Vector2 operator/  (float f) const { return Vector2(x / f, y / f); }

	inline bool operator==	  (const Vector2& v) const { return (x == v.x && y == v.y); }
	inline bool operator!=	  (const Vector2& v) const { return (x != v.x && y != v.y); }

	inline bool operator==	  (float f) const { return (x == f && y == f); }
	inline bool operator!=	  (float f) const { return (x != f && y != f); }

	inline bool IsZero() const { return (x > -0.01f && x < 0.01f && y > -0.01f && y < 0.01f); }
	inline bool IsNull() const { return (x == 0 && y == 0); }
	inline void Reset() { Init(0, 0); }

	inline float LengthSqr() { return (x*x + y*y); }
	inline float Length() { return sqrtf(x*x + y*y); }
	inline float DistTo(Vector2 v) const { return (*this - v).Length(); }
	inline float DistToSqr(Vector2 v) const { return (*this - v).LengthSqr(); }
	inline float Dot(Vector2 v) const { return (x * v.x + y * v.y); }

	inline void Print() { std::cout << "[ " << x << "," << y << "]"; }

	Vector2 Normalized()
	{
		Vector2 vector;
		float length = this->Length();
		(length != 0) ?
			vector /= length : vector.Reset();
		return vector;
	}
	void Rotate(float f)
	{
		float _x, _y;
		float s = sinf(DEG2RAD(f)), c = cosf(DEG2RAD(c));
		_x = x;
		_y = y;
		x = (_x * c) - (_y * s);
		y = (_x * s) + (_y * c);
	}
};

using QAngle = Vector;

class Vector4
{
	float x, y, z, w;
};

SOURCE_END