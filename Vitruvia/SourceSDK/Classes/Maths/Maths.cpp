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

#include "Maths.h"

SOURCE_INIT
MATHS_INIT

RandomSeedFn RandomSeed;
RandomFloatFn RandomFloat;
RandomFloatExpFn RandomFloatExp;
RandomIntFn RandomInt;
RandomGaussianFloatFn RandomGaussianFloat;

// TEAMGAMERFOOD TRIGGERED >:(((((((
inline void SinCos(float radians, float *sine, float *cosine)
{
	_asm
	{
		fld		DWORD PTR[radians]
		fsincos

		mov edx, DWORD PTR[cosine]
		mov eax, DWORD PTR[sine]

		fstp DWORD PTR[edx]
		fstp DWORD PTR[eax]
	}

	*sine = sin(radians);
	*cosine = cos(radians);
}

Vector MakeVector(const QAngle& angle)
{
	float pitch = float(angle[PITCH] * M_PI / 180);
	float yaw = float(angle[YAW] * M_PI / 180);
	float tmp = float(cos(pitch));
	return Vector(float(-tmp * -cos(yaw)), float(sin(yaw) * tmp), float(-sin(pitch)));
}

float DotProduct(const Vector& v1, const Vector& v2)
{
	return v1[PITCH] * v2[PITCH] + v1[YAW] * v2[YAW] + v1[ROLL] * v2[ROLL];
}

QAngle VectorAngles(const Vector& forward)
{
	QAngle angles;

	if (forward[PITCH] == 0.0f && forward[YAW] == 0.0f)
	{
		angles[PITCH] = (forward[ROLL] > 0.0f) ? 270.0f : 90.0f;
		angles[YAW] = 0.0f;
	}
	else
	{
		float len2d = sqrt(forward[PITCH] * forward[PITCH] + forward[YAW] * forward[YAW]);

		angles[0] = RAD2DEG(atan2f(-forward[ROLL], len2d));
		angles[1] = RAD2DEG(atan2f(forward[YAW], forward[PITCH]));

		if (angles[PITCH] < 0.0f) angles[PITCH] += 360.0f;
		if (angles[YAW] < 0.0f) angles[YAW] += 360.0f;
	}

	angles[ROLL] = 0.0f;
	return angles;
}

QAngle GetAngle(const Vector& source, const Vector& dest)
{
	return VectorAngles(dest - source);
}

float GetFov(const QAngle& angle, const Vector& source, const Vector& dest)
{
	Vector	ang = GetAngle(source, dest), 
			aim = MakeVector(angle);
	ang = MakeVector(ang);

	float mag = sqrt(pow(aim[PITCH], 2) + pow(aim[YAW], 2) + pow(aim[ROLL], 2));
	float u_dot_v = DotProduct(aim, ang);

	return RAD2DEG(acos(u_dot_v / (pow(mag, 2))));
}

float GetFov_Distance(const QAngle& angle, const Vector& source, const Vector& dest)
{
	float real_delta = 0.0f, player_distance = 0.0f, yaw_delta = 0.0f;
	Vector target_angles = GetAngle(source, dest);

	yaw_delta = angle.y - target_angles.y;
	yaw_delta = fabsf(yaw_delta);

	player_distance = source.DistTo(dest);

	return sin(DEG2RAD(yaw_delta)) * player_distance;
}

void AngleVectors(const QAngle& angle, Vector* forward, Vector* right, Vector* up)
{
	float sr, sp, sy, cr, cp, cy;

	SinCos(DEG2RAD(angle[PITCH]), &sp, &cp);
	SinCos(DEG2RAD(angle[YAW]),   &sy, &cy);
	SinCos(DEG2RAD(angle[ROLL]),  &sr, &cr);

	if (forward)
	{
		forward->x = cp*cy;
		forward->y = cp*sy;
		forward->z = -sp;
	}
	if (right)
	{
		right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
		right->y = (-1 * sr*sp*sy + -1 * cr*cy);
		right->z = -1 * sr*cp;
	}
	if (up)
	{
		up->x = (cr*sp*cy + -sr*-sy);
		up->y = (cr*sp*sy + -sr*cy);
		up->z = cr*cp;
	}
}

Vector NormalizeAngle(const Vector& angle)
{
	Vector normalized = angle;

	if (normalized[PITCH] > 89.0f && normalized[PITCH] <= 180.0f)
		normalized.x = 89.0f;
	while (normalized[PITCH] > 180.f)
		normalized.x -= 360.f;
	while (normalized[PITCH] < -89.0f)
		normalized.x = -89.0f;
	while (normalized[YAW] > 180.f)
		normalized.y -= 360.f;
	while (normalized[YAW] < -180.f)
		normalized.y += 360.f;

	normalized.z = 0.f;
	return normalized;
}

float NormalizeValue(float value)
{
	float f = value;
	while (f > 180.f)
		f -= 360.f;
	while (f < -180.f)
		f += 360.f;
	return f;
}

void SmoothAngles(QAngle& view_angle, QAngle& dest_angles, float amount)
{
	QAngle delta = view_angle - dest_angles;
	//NormalizeAngle(delta);
	dest_angles = view_angle - delta / 100.0f * amount;
}

Vector AngleDirections(QAngle angle)
{
	angle.x = float(DEG2RAD(angle[PITCH]));
	angle.y = float(DEG2RAD(angle[YAW]));

	float sinPitch = sin(angle[PITCH]);
	float cosPitch = cos(angle[PITCH]);

	float sinYaw = sin(angle[YAW]);
	float cosYaw = cos(angle[YAW]);

	Vector direction;
	direction.x = cosPitch * cosYaw;
	direction.y = cosPitch * sinYaw;
	direction.z = -sinPitch;

	return direction;
}

FORCEINLINE float DotFloat(const float *a, const float *b)
{
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

Vector VectorTransform(const Vector& in, const matrix3x4_t& matrix)
{
	Vector out;
	out.x = DotFloat(in.Base(), matrix[0]) + matrix[0][3];
	out.y = DotFloat(in.Base(), matrix[1]) + matrix[1][3];
	out.z = DotFloat(in.Base(), matrix[2]) + matrix[2][3];
	return out;
}

Vector MatrixVector(const matrix3x4_t& matrix)
{
	return Vector(matrix[0][3], matrix[1][3], matrix[2][3]);
}

QAngle MatrixAngles(const matrix3x4_t& matrix)
{
	QAngle angles;
	float forward[3];
	float left[3];
	float up[3];

	forward[PITCH] = matrix[0][0];
	forward[YAW] = matrix[1][0];
	forward[ROLL] = matrix[2][0];

	left[PITCH] = matrix[0][1];
	left[YAW] = matrix[1][1];
	left[ROLL] = matrix[2][1];

	up[ROLL] = matrix[2][2];

	float xyDist = sqrtf(forward[PITCH] * forward[PITCH] + forward[YAW] * forward[YAW]);

	if (xyDist > 0.001f)
	{
		angles.x = RAD2DEG(atan2f(-forward[ROLL], xyDist));
		angles.y = RAD2DEG(atan2f(forward[YAW], forward[PITCH]));
		angles.z = RAD2DEG(atan2f(left[ROLL], up[ROLL]));
	}
	else
	{
		angles.x = RAD2DEG(atan2f(-forward[ROLL], xyDist));
		angles.y = RAD2DEG(atan2f(-left[PITCH], left[YAW]));
		angles.z = 0.f;
	}

	return angles;
}

void AngleMatrix(const QAngle& angles, matrix3x4_t& matrix)
{
	float sr, sp, sy, cr, cp, cy;
	SinCos(DEG2RAD(angles[YAW]), &sy, &cy);
	SinCos(DEG2RAD(angles[PITCH]), &sp, &cp);
	SinCos(DEG2RAD(angles[ROLL]), &sr, &cr);

	// matrix = (YAW * PITCH) * ROLL
	matrix[0][0] = cp*cy;
	matrix[1][0] = cp*sy;
	matrix[2][0] = -sp;

	float crcy = cr*cy;
	float crsy = cr*sy;
	float srcy = sr*cy;
	float srsy = sr*sy;
	matrix[0][1] = sp*srcy - crsy;
	matrix[1][1] = sp*srsy + crcy;
	matrix[2][1] = sr*cp;

	matrix[0][2] = (sp*crcy + srsy);
	matrix[1][2] = (sp*crsy - srcy);
	matrix[2][2] = cr*cp;

	matrix[0][3] = 0.0f;
	matrix[1][3] = 0.0f;
	matrix[2][3] = 0.0f;
}

void AngleMatrix(const QAngle& angles, const Vector& position, matrix3x4_t& matrix)
{
	AngleMatrix(angles, matrix);
	MatrixSetColumn(position, 3, matrix);
}

void MatrixCopy(const matrix3x4_t& src, matrix3x4_t& dst)
{
	if (&src != &dst)
	{
		memcpy(dst, src, 16 * sizeof(float));
	}
}

void MatrixMultiply(const matrix3x4_t& src1, const matrix3x4_t& src2, matrix3x4_t& dst)
{
	// Make sure it works if src1 == dst or src2 == dst
	VMatrix tmp1, tmp2;
	typedef float VMatrixRaw_t[4];
	const VMatrixRaw_t* s1 = (&src1 == &dst) ? tmp1 : src1;
	const VMatrixRaw_t* s2 = (&src2 == &dst) ? tmp2 : src2;

	if (&src1 == &dst)
	{
		MatrixCopy(src1, tmp1);
	}
	if (&src2 == &dst)
	{
		MatrixCopy(src2, tmp2);
	}

	dst[0][0] = s1[0][0] * s2[0][0] + s1[0][1] * s2[1][0] + s1[0][2] * s2[2][0] + s1[0][3] * s2[3][0];
	dst[0][1] = s1[0][0] * s2[0][1] + s1[0][1] * s2[1][1] + s1[0][2] * s2[2][1] + s1[0][3] * s2[3][1];
	dst[0][2] = s1[0][0] * s2[0][2] + s1[0][1] * s2[1][2] + s1[0][2] * s2[2][2] + s1[0][3] * s2[3][2];
	dst[0][3] = s1[0][0] * s2[0][3] + s1[0][1] * s2[1][3] + s1[0][2] * s2[2][3] + s1[0][3] * s2[3][3];

	dst[1][0] = s1[1][0] * s2[0][0] + s1[1][1] * s2[1][0] + s1[1][2] * s2[2][0] + s1[1][3] * s2[3][0];
	dst[1][1] = s1[1][0] * s2[0][1] + s1[1][1] * s2[1][1] + s1[1][2] * s2[2][1] + s1[1][3] * s2[3][1];
	dst[1][2] = s1[1][0] * s2[0][2] + s1[1][1] * s2[1][2] + s1[1][2] * s2[2][2] + s1[1][3] * s2[3][2];
	dst[1][3] = s1[1][0] * s2[0][3] + s1[1][1] * s2[1][3] + s1[1][2] * s2[2][3] + s1[1][3] * s2[3][3];

	dst[2][0] = s1[2][0] * s2[0][0] + s1[2][1] * s2[1][0] + s1[2][2] * s2[2][0] + s1[2][3] * s2[3][0];
	dst[2][1] = s1[2][0] * s2[0][1] + s1[2][1] * s2[1][1] + s1[2][2] * s2[2][1] + s1[2][3] * s2[3][1];
	dst[2][2] = s1[2][0] * s2[0][2] + s1[2][1] * s2[1][2] + s1[2][2] * s2[2][2] + s1[2][3] * s2[3][2];
	dst[2][3] = s1[2][0] * s2[0][3] + s1[2][1] * s2[1][3] + s1[2][2] * s2[2][3] + s1[2][3] * s2[3][3];

	dst[3][0] = s1[3][0] * s2[0][0] + s1[3][1] * s2[1][0] + s1[3][2] * s2[2][0] + s1[3][3] * s2[3][0];
	dst[3][1] = s1[3][0] * s2[0][1] + s1[3][1] * s2[1][1] + s1[3][2] * s2[2][1] + s1[3][3] * s2[3][1];
	dst[3][2] = s1[3][0] * s2[0][2] + s1[3][1] * s2[1][2] + s1[3][2] * s2[2][2] + s1[3][3] * s2[3][2];
	dst[3][3] = s1[3][0] * s2[0][3] + s1[3][1] * s2[1][3] + s1[3][2] * s2[2][3] + s1[3][3] * s2[3][3];
}

void MatrixSetColumn(const Vector & in, int column, matrix3x4_t & out)
{
	out[0][column] = in.x;
	out[1][column] = in.y;
	out[2][column] = in.z;
}

Vector RotateVector(const Vector& vec, const matrix3x4_t& matrix)
{
	Vector o;
	o.x = vec.Dot(Vector(matrix[0][0], matrix[0][1], matrix[0][2]));
	o.y = vec.Dot(Vector(matrix[1][0], matrix[1][1], matrix[1][2]));
	o.z = vec.Dot(Vector(matrix[2][0], matrix[2][1], matrix[2][2]));
	return o;
}

MATHS_END
SOURCE_END
