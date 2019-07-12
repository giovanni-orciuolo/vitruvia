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

#include "GameHelper.h"
#include "../../SourceClasses/C_CSPlayer.h"

Vitruvia::CGameHelper Vitruvia::g_GameHelper;

int Vitruvia::CGameHelper::GetPing()
{
	auto* netChannel = Source::Engine->GetNetChannelInfo();
	if (netChannel != nullptr)
	{
		float avgLatency = netChannel->GetAvgLatency(FLOW_OUTGOING);
		avgLatency = max(0, avgLatency);
		return scast<int>(avgLatency * 1000);
	}
	return -1;
}

int Vitruvia::CGameHelper::GetFPS()
{
	const float FRAMERATE_AVG_FRAC = 0.9f;
	static float FrameRate = 0.f;
	FrameRate = FRAMERATE_AVG_FRAC * FrameRate + (1.0 - FRAMERATE_AVG_FRAC) * Source::Globals->absolute_frametime;

	if (FrameRate <= 0.0f)
		FrameRate = 1.0f;

	return scast<int>(1.0f / FrameRate);
}

bool Vitruvia::CGameHelper::IsPointVisible(const Source::Vector& point)
{
	using namespace Source;
	auto local = C_CSPlayer::GetLocal();
	if (local == nullptr) return false;

	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;
	filter.pSkip = local;

	ray.Init(local->GetEyePosition(), point);
	Source::EngineTrace->TraceRay(ray, MASK_VISIBLE, &filter, &tr);

	return tr.fraction > 0.99f;
}