/*
	____   ____.__  __                     .__        
	\   \ /   /|__|/  |________ __ _____  _|__|____   
	 \   Y   / |  \   __\_  __ \  |  \  \/ /  \__  \  
	  \     /  |  ||  |  |  | \/  |  /\   /|  |/ __ \_
	   \___/   |__||__|  |__|  |____/  \_/ |__(____  /
												   \/ 
	Premium Internal Multihack for Counter-Strike: Global Offensive
	Made with love by double v - 2017
*/

// Credits: pycache bae <3

#include "ThirdPerson.h"
using namespace Source;

bool ThirdPerson = false;

void ToggleThirdPerson(bool on, bool kill = false) 
{
	static SpoofedConvar* sv_cheats;
	if (kill) 
	{
		if (sv_cheats)
			delete sv_cheats;
		return;
	}

	static bool oldon = false;
	if (oldon == on) 
		return;
	else
		oldon = on;

	if (on) {
		if (!sv_cheats) {
			sv_cheats = new SpoofedConvar(Source::CVar, enc("sv_cheats"));
			sv_cheats->SetBool(true);
			sv_cheats->SetInt(1);
		}
		Source::Engine->ClientCmd_Unrestricted(enc("thirdperson"));
	}
	else Source::Engine->ClientCmd_Unrestricted(enc("firstperson"));
}

float MoveToward(float cur, float goal, float lag) {
	if (cur != goal) 
	{
		if (abs(cur - goal) > 180.0) 
		{
			if (cur < goal)
				cur += 360.0;
			else
				cur -= 360.0;
		}
		if (cur < goal) 
		{
			if (cur < goal - 1.0)
				cur += (goal - cur) / lag;
			else
				cur = goal;
		}
		else 
		{
			if (cur > goal + 1.0)
				cur -= (cur - goal) / lag;
			else
				cur = goal;
		}
	}
	// bring cur back into range
	if (cur < 0)
		cur += 360.0;
	else if (cur >= 360)
		cur -= 360;
	return cur;
}

__forceinline void VectorMA(const Vector& start, float scale, const Vector& direction, Vector& dest) 
{
	dest.x = start.x + direction.x*scale;
	dest.y = start.y + direction.y*scale;
	dest.z = start.z + direction.z*scale;
}

float RemapValClamped(float val, float A, float B, float C, float D) 
{
	if (A == B)
		return val >= B ? D : C;
	float cVal = (val - A) / (B - A);
	cVal = clamp<float, float>(cVal, 0.0f, 1.0f);

	return C + (D - C) * cVal;
}

void Vitruvia::Tools::CThirdpersonManager::Start()
{
	camAngles = Source::QAngle(0, 0, 0);
	camOrigin = Source::Vector(0, 0, 0);
	m_vecCameraOffset = Source::Vector(0, 0, 0);
	m_vecDesiredCameraOffset = Source::Vector(0, 0, 0);
	m_flFraction = 0.f;
	m_flUpFraction = 0.f;
	m_flTargetFraction = 0.f;
	m_flTargetUpFraction = 0.f;
	m_flLerpTime = 0.f;
	m_flUpLerpTime = 0.f;
	idealpitch = 0.f;
	idealyaw = 0.f;
	idealdist = 150.f;
	cl_thirdperson = false;
	m_bOverrideThirdPerson = false;

	cam_idealdist = nullptr;
	cam_idealdistright = nullptr;
	cam_idealdistup = nullptr;
	cam_ideallag = nullptr;
	cam_idealyaw = nullptr;
	cam_idealpitch = nullptr;
	cam_collision = nullptr;
	cam_snapto = nullptr;
	cam_idealdelta = nullptr;
	c_minyaw = nullptr;
	c_maxyaw = nullptr;
	c_minpitch = nullptr;
	c_maxpitch = nullptr;
	c_mindistance = nullptr;
	c_maxdistance = nullptr;
}

void Vitruvia::Tools::CThirdpersonManager::UpdateThirdPerson(Source::CViewSetup* vs)
{
	if (!c_maxpitch) 
	{ 
		// Get all camera commands once
		GetCameraCommands();
	}

	auto local = C_CSPlayer::GetLocal();
	if (local == nullptr)
		return;

	static int delay_tick = 100;

	// If the local player is not alive, it's useless to apply thirdperson
	if (!local->IsAlive() || !Config::VisualsOther.ThirdPerson)
	{
		static ConVar* cam_idealyaw = Source::CVar->FindVar(enc("cam_idealyaw"));
		static ConVar* cam_idealpitch = Source::CVar->FindVar(enc("cam_idealpitch"));
		cam_idealyaw->SetValue(0);
		cam_idealpitch->SetValue(0);
		if (ThirdPerson)
		{
			*local->m_iObserverModePtr() = OBSERVER_NONE;
			CAM_ToFirstPerson();
			ThirdPerson = false;
		}
		ToggleThirdPerson(false);
		return;
	}
	
	*local->m_iObserverModePtr() = OBSERVER_NONE;

	if (!Config::VisualsOther.ThirdPerson) 
	{
		ThirdPerson = false;
		return;
	}

	if (local->IsAlive()) 
	{
		delay_tick = 1000;
		ToggleThirdPerson(true);
		return;
	}

	if (!ThirdPerson) 
	{
		CAM_ToThirdPerson();
		ThirdPerson = true;
	}

	if (!ThirdPerson) 
	{
		return;
	}

	if (delay_tick > 0)
		delay_tick--;

	if (delay_tick <= 0)
	{
		return;
	}

	C_BasePlayer* camera_target = local;
	C_BasePlayer* observer_target = local->GetObserverTarget();

	if (observer_target) 
	{
		camera_target = observer_target;
	}

	vs->origin = rcast<C_CSPlayer*>(camera_target)->GetEyePosition();

	CAM_Think();

	const Vector cam_ofs = GetCameraOffsetAngles();
	Vector cam_ofs_distance = GetFinalCameraOffset();

	cam_ofs_distance *= GetDistanceFraction();

	Vector camForward, camRight, camUp;

	if (m_bOverrideThirdPerson == false) 
	{
		Source::Engine->GetViewAngles(camAngles);
	}
	else 
	{
		camAngles = QAngle(GetCameraOffsetAngles()[PITCH], GetCameraOffsetAngles()[YAW], 0.0f);
	}

	// get the forward vector
	Maths::AngleVectors(camAngles, &camForward, &camRight, &camUp);

	VectorMA(vs->origin, -cam_ofs_distance[PITCH], camForward, vs->origin);
	VectorMA(vs->origin, cam_ofs_distance[YAW], camRight, vs->origin);
	VectorMA(vs->origin, cam_ofs_distance[ROLL], camUp, vs->origin);

	camOrigin = vs->origin;

	// Override angles from third person camera
	vs->angles = camAngles.Clamped();
}

int original_observer_mode = OBSERVER_NONE;
void Vitruvia::Tools::CThirdpersonManager::CAM_ToFirstPerson()
{
	auto local = C_CSPlayer::GetLocal();
	if (local)
	{
		if (cl_thirdperson == false)
			m_bOverrideThirdPerson = false;
		SetDesiredCameraOffset(Vector(0, 0, 0));
		*local->m_iObserverModePtr() = original_observer_mode;
	}
}

void Vitruvia::Tools::CThirdpersonManager::CAM_ToThirdPerson()
{
	auto local = C_CSPlayer::GetLocal();
	if (local)
	{
		if (cl_thirdperson == false) {
			SetDesiredCameraOffset(Vector(idealdist, cam_idealdistright->GetFloat(), cam_idealdistup->GetFloat()));
			m_bOverrideThirdPerson = true;
		}
		QAngle va;
		Source::Engine->GetViewAngles(va);
		SetCameraOffsetAngles(Vector(va[YAW], va[PITCH], CAM_MIN_DIST));
		*local->m_iObserverModePtr() = 1;
	}
}

void Vitruvia::Tools::CThirdpersonManager::GetCameraCommands()
{
	cam_idealdist = Source::CVar->FindVar(enc("cam_idealdist"));
	cam_idealdistright = Source::CVar->FindVar(enc("cam_idealdistright"));
	cam_idealdistup = Source::CVar->FindVar(enc("cam_idealdistup"));
	cam_ideallag = Source::CVar->FindVar(enc("cam_ideallag"));
	cam_idealpitch = Source::CVar->FindVar(enc("cam_idealpitch"));
	cam_idealyaw = Source::CVar->FindVar(enc("cam_idealyaw"));
	cam_collision = Source::CVar->FindVar(enc("cam_collision"));
	cam_snapto = Source::CVar->FindVar(enc("cam_snapto"));
	c_minyaw = Source::CVar->FindVar(enc("c_minyaw"));
	c_maxyaw = Source::CVar->FindVar(enc("c_maxyaw"));
	c_minpitch = Source::CVar->FindVar(enc("c_minpitch"));
	c_maxpitch = Source::CVar->FindVar(enc("c_maxpitch"));
	c_mindistance = Source::CVar->FindVar(enc("c_mindistance"));
	c_maxdistance = Source::CVar->FindVar(enc("c_maxdistance"));
	cam_idealdelta = Source::CVar->FindVar(enc("cam_idealdelta"));
}

Source::Vector Vitruvia::Tools::CThirdpersonManager::GetDistanceFraction()
{
	if (m_bOverrideThirdPerson == true) 
	{
		return Vector(m_flTargetFraction, m_flTargetFraction, m_flTargetFraction);
	}

	float flFraction = m_flFraction;
	float flUpFraction = m_flUpFraction;

	float flFrac = RemapValClamped(Source::Globals->cur_time - m_flLerpTime, 0, CAMERA_OFFSET_LERP_TIME, 0, 1);

	flFraction = Maths::Lerp(flFrac, m_flFraction, m_flTargetFraction);

	if (flFrac == 1.0f) 
	{
		m_flFraction = m_flTargetFraction;
	}

	flFrac = RemapValClamped(Source::Globals->cur_time - m_flUpLerpTime, 0, CAMERA_OFFSET_LERP_TIME, 0, 1);

	flUpFraction = 1.0f - Maths::Lerp(flFrac, m_flUpFraction, m_flTargetUpFraction);

	if (flFrac == 1.0f) 
	{
		m_flUpFraction = m_flTargetUpFraction;
	}

	return Vector(flFraction, flFraction, flUpFraction);
}

Source::Vector Vitruvia::Tools::CThirdpersonManager::GetFinalCameraOffset()
{
	Vector desired = GetDesiredCameraOffset();

	if (m_flUpFraction != 1.0f) 
	{
		desired.z += CAMERA_UP_OFFSET;
	}

	return desired;
}

void Vitruvia::Tools::CThirdpersonManager::CAM_Think()
{
	auto local = C_CSPlayer::GetLocal();
	if (local)
	{
		Vector idealAngles = Vector(0.0f, 0.0f, idealdist);

#if 0
		Vector idealAngles = Vector(idealpitch, idealyaw, idealdist);
		POINT cp;
		//get windows cursor position
		GetCursorPos(&cp);
		const int m_nCameraX = cp.x;
		const int m_nCameraY = cp.y;
		const int x = CenterOfScreen.x;
		const int y = CenterOfScreen.y;
		//keep the camera within certain limits around the player (ie avoid certain bad viewing angles)  
		if (m_nCameraX > x) {
			//if ((idealAngles[YAW]>=225.0)||(idealAngles[YAW]<135.0))
			if (idealAngles[1] < c_maxyaw->GetFloat()) {
				idealAngles[1] += CAM_ANGLE_MOVE *((m_nCameraX - x) / 2);
			}
			if (idealAngles[1] > c_maxyaw->GetFloat()) {
				idealAngles[1] = c_maxyaw->GetFloat();
			}
		}
		else if (m_nCameraX < x) {
			//if ((idealAngles[YAW]<=135.0)||(idealAngles[YAW]>225.0))
			if (idealAngles[1] > c_minyaw->GetFloat()) {
				idealAngles[1] -= CAM_ANGLE_MOVE * ((x - m_nCameraX) / 2);
			}
			if (idealAngles[1] < c_minyaw->GetFloat()) {
				idealAngles[1] = c_minyaw->GetFloat();
			}
		}
		//check for y delta values and adjust accordingly
		//eventually adjust PITCH based on amount of movement
		//also make sure camera is within bounds
		if (m_nCameraY > y) {
			if (idealAngles[0] < c_maxpitch->GetFloat()) {
				idealAngles[0] += (CAM_ANGLE_MOVE)* ((m_nCameraY - y) / 2);
			}
			if (idealAngles[0] > c_maxpitch->GetFloat()) {
				idealAngles[0] = c_maxpitch->GetFloat();
			}
		}
		else if (m_nCameraY < y) {
			if (idealAngles[0] > c_minpitch->GetFloat()) {
				idealAngles[0] -= (CAM_ANGLE_MOVE)*((y - m_nCameraY) / 2);
			}
			if (idealAngles[0] < c_minpitch->GetFloat()) {
				idealAngles[0] = c_minpitch->GetFloat();
			}
		}
		//Nathan code here
		if ((GetAsyncKeyState(VK_NUMPAD8) & (1 << 16)))
			idealAngles[0] += cam_idealdelta->GetFloat();
		else if ((GetAsyncKeyState(VK_NUMPAD2) & (1 << 16)))
			idealAngles[0] -= cam_idealdelta->GetFloat();
		if ((GetAsyncKeyState(VK_NUMPAD4) & (1 << 16)))
			idealAngles[1] -= cam_idealdelta->GetFloat();
		else if ((GetAsyncKeyState(VK_NUMPAD6) & (1 << 16)))
			idealAngles[1] += cam_idealdelta->GetFloat();
		if ((GetAsyncKeyState(VK_ADD) & (1 << 16))) {
			idealAngles[2] -= 2 * cam_idealdelta->GetFloat();
			if (idealAngles[2] < CAM_MIN_DIST) {
				// If we go back into first person, reset the angle
				idealAngles[0] = 0;
				idealAngles[1] = 0;
				idealAngles[2] = CAM_MIN_DIST;
			}
		}
		else if ((GetAsyncKeyState(VK_SUBTRACT) & (1 << 16)))
			idealAngles[2] += 2 * cam_idealdelta->GetFloat();
#endif

		QAngle viewangles;
		Source::Engine->GetViewAngles(viewangles);

#if 0
		// bring the pitch values back into a range that MoveToward can handle
		if (idealAngles[0] > 180)
			idealAngles[0] -= 360;
		else if (idealAngles[0] < -180)
			idealAngles[0] += 360;
		// bring the yaw values back into a range that MoveToward can handle
		// --
		// Vitaliy: going with >= 180 and <= -180.
		// This introduces a potential discontinuity when looking directly at model face
		// as camera yaw will be jumping from +180 to -180 and back, but when working with
		// the camera allows smooth rotational transitions from left to right and back.
		// Otherwise one of the transitions that has ">"-comparison will be locked.
		// --
		if (idealAngles[1] >= 180)
			idealAngles[1] -= 360;
		else if (idealAngles[1] <= -180)
			idealAngles[1] += 360;
		// clamp pitch, yaw and dist...
		idealAngles[0] = std::clamp(idealAngles[0], c_minpitch->GetFloat(), c_maxpitch->GetFloat());
		idealAngles[1] = std::clamp(idealAngles[1], c_minyaw->GetFloat(), c_maxyaw->GetFloat());
		idealAngles[2] = std::clamp(idealAngles[2], c_mindistance->GetFloat(), c_maxdistance->GetFloat());
#endif

		// update ideal angles
		idealpitch = idealAngles[0];
		idealyaw = idealAngles[1];
		idealdist = idealAngles[2];

		// Move the CameraOffset "towards" the idealAngles
		// Note: CameraOffset = viewangle + idealAngle
		Vector camOffset;

		if (cam_snapto->GetInt()) {
			camOffset[1] = idealyaw + viewangles[1];
			camOffset[0] = idealpitch + viewangles[0];
			camOffset[2] = idealdist;
		}
		else {
			float lag = max(1, 1 + cam_ideallag->GetFloat());
			camOffset = GetCameraOffsetAngles();

			if (camOffset[1] - viewangles[1] != idealyaw)
				camOffset[1] = MoveToward(camOffset[1], idealyaw + viewangles[1], lag);

			if (camOffset[0] - viewangles[0] != idealpitch)
				camOffset[0] = MoveToward(camOffset[0], idealpitch + viewangles[0], lag);

			if (abs(camOffset[2] - idealdist) < 2.0)
				camOffset[2] = idealdist;
			else
				camOffset[2] += (idealdist - camOffset[2]) / lag;
		}

		if (cam_collision->GetInt()) {
			QAngle desiredCamAngles;
			if (m_bOverrideThirdPerson == false) {
				desiredCamAngles = viewangles;
			}
			else {
				desiredCamAngles = QAngle(camOffset[0], camOffset[1], camOffset[2]);
			}
			C_BasePlayer* camera_target = local;
			C_BasePlayer* observer_target = local->GetObserverTarget();
			if (observer_target)
			{
				camera_target = observer_target;
			}
			PositionCamera(rcast<C_CSPlayer*>(camera_target), desiredCamAngles);
		}

		SetCameraOffsetAngles(camOffset);
	}
}

void Vitruvia::Tools::CThirdpersonManager::PositionCamera(Source::C_CSPlayer* player, const Source::QAngle& angles)
{
	if (player) 
	{
		trace_t trace;

		Vector camForward, camRight, camUp;

		// find our player's origin, and from there, the eye position
		Vector origin = player->m_vecOrigin();
		origin += player->m_vecViewOffset();

		Maths::AngleVectors((QAngle)angles, &camForward, &camRight, &camUp); //+ punchangle

		Vector endPos = origin;

		Vector vecCamOffset = endPos + (camForward * -GetDesiredCameraOffset()[0]) + (camRight * GetDesiredCameraOffset()[1]) + (camUp * GetDesiredCameraOffset()[2]);

		// use our previously #defined hull to collision trace
		CTraceFilter traceFilter;
		traceFilter.pSkip = (IHandleEntity*)player;
		Ray_t ray;
		ray.Init(endPos, vecCamOffset, CAM_HULL_MIN, CAM_HULL_MAX);
		Source::EngineTrace->TraceRay(ray, MASK_SOLID & ~CONTENTS_MONSTER, (ITraceFilter*)&traceFilter, &trace);

		if (trace.fraction != m_flTargetFraction) {
			m_flLerpTime = Source::Globals->cur_time;
		}

		m_flTargetFraction = trace.fraction;
		m_flTargetUpFraction = 1.0f;

		//If we're getting closer to a wall snap the fraction right away.
		if (m_flTargetFraction < m_flFraction) {
			m_flFraction = m_flTargetFraction;
			m_flLerpTime = Source::Globals->cur_time;
		}

		// move the camera closer if it hit something
		if (trace.fraction < 1.0) {
			m_vecCameraOffset[2] *= trace.fraction;

			Ray_t ray2;
			ray2.Init(endPos, endPos + (camForward * (-GetDesiredCameraOffset()[0])), CAM_HULL_MIN, CAM_HULL_MAX);

			Source::EngineTrace->TraceRay(ray2, MASK_SOLID & ~CONTENTS_MONSTER, &traceFilter, &trace);

			if (trace.fraction != 1.0f) {
				if (trace.fraction != m_flTargetUpFraction) {
					m_flUpLerpTime = Source::Globals->cur_time;
				}

				m_flTargetUpFraction = trace.fraction;

				if (m_flTargetUpFraction < m_flUpFraction) {
					m_flUpFraction = trace.fraction;
					m_flUpLerpTime = Source::Globals->cur_time;
				}
			}
		}
	}
}