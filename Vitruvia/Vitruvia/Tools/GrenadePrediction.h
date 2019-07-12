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

#define GrenadePrediction CGrenadePrediction::GetInstance()

namespace Vitruvia
{
	namespace Tools
	{
		class CGrenadePrediction {
			IMPLEMENT_SINGLETON(CGrenadePrediction);
		public:
			void Tick(int buttons);
			void PredictGrenade();
			void PaintGrenadeTrajectory(Render::D3D::DrawManager* graphics);
		private:
			void Setup(Source::C_CSPlayer* pl, Source::Vector& vecSrc, Source::Vector& vecThrow, const Source::QAngle& angEyeAngles);
			void Simulate();
			int  Step(Source::Vector& vecSrc, Source::Vector& vecThrow, int tick, float interval);
			bool CheckDetonate(const Source::Vector& vecThrow, const Source::CGameTrace& tr, int tick, float interval);
			void AddGravityMove(Source::Vector& move, Source::Vector& vel, float frametime, bool onground);
			void HullTrace(Source::Vector src, Source::Vector end, Source::ITraceFilter* filter, Source::CGameTrace& tr);
			void PushEntity(Source::Vector& src, const Source::Vector& move, Source::CGameTrace& tr);
			void ResolveFlyCollisionCustom(Source::CGameTrace& tr, Source::Vector& vecVelocity, float interval);
			unsigned char PhysicsClipVelocity(const Source::Vector& in, const Source::Vector& normal, Source::Vector& out, float overbounce);
		};
	}
}