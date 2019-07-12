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

#pragma once

#include "../Vitruvia.h"
#include "../SourceClasses/C_CSPlayer.h"
#include "../Renderer/D3D/D3D_Renderer.h"
#include "../Config/Config.h"

#define ThirdPersonManager CThirdpersonManager::GetInstance()

const float CAM_HULL_OFFSET = 14.0f;
const float CAM_MIN_DIST = 30.0f;
const float CAM_ANGLE_MOVE = 0.5f;
const float CAMERA_UP_OFFSET = 25.0f;
const float CAMERA_OFFSET_LERP_TIME = 0.5f;
const Source::Vector CAM_HULL_MIN = Source::Vector(-CAM_HULL_OFFSET, -CAM_HULL_OFFSET, -CAM_HULL_OFFSET);
const Source::Vector CAM_HULL_MAX = Source::Vector(CAM_HULL_OFFSET, CAM_HULL_OFFSET, CAM_HULL_OFFSET);

namespace Vitruvia
{
	namespace Tools
	{
		class CThirdpersonManager
		{
			IMPLEMENT_SINGLETON(CThirdpersonManager);
		public:
			void Start();
			void UpdateThirdPerson(Source::CViewSetup* vs);

			inline void SetDesiredCameraOffset(const Source::Vector& vecOffset) { m_vecDesiredCameraOffset = vecOffset; }
			inline const Source::Vector& GetDesiredCameraOffset(void) { return m_vecDesiredCameraOffset; }
			inline void SetCameraOffsetAngles(const Source::Vector& vecOffset) { m_vecCameraOffset = vecOffset; }
			inline const Source::Vector& GetCameraOffsetAngles(void) { return m_vecCameraOffset; }
		private:
			void CAM_ToFirstPerson();
			void CAM_ToThirdPerson();
			void GetCameraCommands();
			Source::Vector GetDistanceFraction();
			Source::Vector GetFinalCameraOffset();
			void CAM_Think();
			void PositionCamera(Source::C_CSPlayer* player, const Source::QAngle& angles);

			Source::QAngle camAngles;
			Source::Vector camOrigin;
			Source::Vector m_vecCameraOffset;
			Source::Vector m_vecDesiredCameraOffset;

			float m_flFraction;
			float m_flUpFraction;
			float m_flTargetFraction;
			float m_flTargetUpFraction;
			float m_flLerpTime;
			float m_flUpLerpTime;
			float idealpitch;
			float idealyaw;
			float idealdist;
			bool cl_thirdperson = false;
			bool m_bOverrideThirdPerson = false;

			Source::ConVar* cam_idealdist;
			Source::ConVar* cam_idealdistright;
			Source::ConVar* cam_idealdistup;
			Source::ConVar* cam_ideallag;
			Source::ConVar* cam_idealyaw;
			Source::ConVar* cam_idealpitch;
			Source::ConVar* cam_collision;
			Source::ConVar* cam_snapto;
			Source::ConVar* cam_idealdelta;
			Source::ConVar* c_minyaw;
			Source::ConVar* c_maxyaw;
			Source::ConVar* c_minpitch;
			Source::ConVar* c_maxpitch;
			Source::ConVar* c_mindistance;
			Source::ConVar* c_maxdistance;
		};
	}
}