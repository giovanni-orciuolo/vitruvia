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
#include "Quaternion.h"

#define PITCH	0
#define YAW		1
#define ROLL	2

extern "C" typedef int(__cdecl* RandomSeedFn)(int iSeed);
extern "C" typedef float(__cdecl* RandomFloatFn)(float flMinVal, float flMaxVal);
extern "C" typedef float(__cdecl* RandomFloatExpFn)(float flMinVal, float flMaxVal, float flExponent);
extern "C" typedef int(__cdecl* RandomIntFn)(int iMinVal, int iMaxVal);
extern "C" typedef float(__cdecl* RandomGaussianFloatFn)(float flMean, float flStdDev);

SOURCE_INIT
MATHS_INIT

extern RandomSeedFn RandomSeed;
extern RandomFloatFn RandomFloat;
extern RandomFloatExpFn RandomFloatExp;
extern RandomIntFn RandomInt;
extern RandomGaussianFloatFn RandomGaussianFloat;

template <class T = float>
__forceinline T Lerp(float flPercent, T const& A, T const& B) {
	return A + (B - A) * flPercent;
}
__forceinline void VectorScale(const Vector& in, float scale, Vector& out)
{
	out[0] = in[0] * scale;
	out[1] = in[1] * scale;
	out[2] = in[2] * scale;
}

// Make vector based on an angle
extern Vector MakeVector(const QAngle& angle);
// Dot product between two vectors
extern float DotProduct(const Vector& v1, const Vector& v2);
// Convert a vector into an angle
extern QAngle VectorAngles(const Vector& forward);
// Calculate the angle between a source and a destination using VectorAngles
extern QAngle GetAngle(const Vector& source, const Vector& dest);
// Calculate the fov
extern float GetFov(const QAngle& angle, const Vector& source, const Vector& dest);
// Calculate a more accurate distance-based fov
extern float GetFov_Distance(const QAngle& angle, const Vector& source, const Vector& dest);
// Based on an angle calculate the forward, left and right vector. Just pass nullptr if you don't want to calc smth
extern void AngleVectors(const QAngle& angle, Vector* forward, Vector* right = nullptr, Vector* up = nullptr);
// Normalize an angle. This prevents untrusted angles from being set
extern Vector NormalizeAngle(const Vector& angle);
// Normalize a float value. This can be used to normalize only a X or a Y element
extern float NormalizeValue(float value);
// Smooth angles so the aim lerps to a destination. The less the amount, the more it smooths
extern void SmoothAngles(QAngle& view_angle, QAngle& dest_angles, float amount);
// Convert an angle into a direction
extern Vector AngleDirections(QAngle angle);
// Transform a vector through a matrix
extern Vector VectorTransform(const Vector& in, const matrix3x4_t& matrix);
// Convert a matrix into a vector
extern Vector MatrixVector(const matrix3x4_t& matrix);
// Convert a matrix into angles
extern QAngle MatrixAngles(const matrix3x4_t& matrix);
// Convert angles into a matrix
extern void AngleMatrix(const QAngle &angles, matrix3x4_t& matrix);
extern void AngleMatrix(const QAngle& angles, const Vector& position, matrix3x4_t& matrix);
// Copy a matrix into another
void MatrixCopy(const matrix3x4_t& src, matrix3x4_t& dst);
// Multiply 2 matrixes and return the new matrix
extern void MatrixMultiply(const matrix3x4_t& src1, const matrix3x4_t& src2, matrix3x4_t& dst);
// Set column of a matrix
extern void MatrixSetColumn(const Vector& in, int column, matrix3x4_t& out);
// Rotate a vector based on a matrix. Returns the rotated vector
extern Vector RotateVector(const Vector& vec, const matrix3x4_t& matrix);

MATHS_END
SOURCE_END