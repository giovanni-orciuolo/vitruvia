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

#include "C_BaseAnimatingOverlay.h"

SOURCE_INIT

C_AnimationLayer& C_BaseAnimatingOverlay::GetAnimOverlay(int i)
{
	static offset_t field = this->FindDataMapOffset(this->GetPredDescMap(), enc("m_AnimOverlay"));
	return (*rcast<C_AnimationLayer**>(rcast<offset_t>(this) + 0x2970))[i];
}

Activity C_BaseAnimatingOverlay::GetSequenceActivity(int sequence)
{
	model_t* mdl = this->GetRenderable()->GetModel();
	if (mdl)
	{
		studiohdr_t* hdr = Source::ModelInfo->GetStudioModel(mdl);
		if (hdr)
		{
			static auto get_sequence_activity = rcast<int(__fastcall*)(void*, studiohdr_t*, int)>(Vitruvia::Offsets::GetSequenceActivity);
			return scast<Activity>(get_sequence_activity(this, hdr, sequence));
		}
	}
	return Activity::ACT_RESET;
}

std::string C_BaseAnimatingOverlay::DumpAnimOverlay(int i)
{
	if (i >= 0)
	{
#ifdef _DEBUG
		C_AnimationLayer& animLayer = GetAnimOverlay(i);
		std::stringstream ss;
		ss << enc("m_AnimOverlay[") << i << enc("] ->")
			<< enc("\tm_nOrder: ") << animLayer.m_nOrder << enc("\n")
			<< enc("\tm_nSequence: ") << animLayer.m_nSequence << enc("\n")
			<< enc("\tm_flPrevCycle: ") << animLayer.m_flPrevCycle << enc("\n")
			<< enc("\tm_flWeight: ") << animLayer.m_flWeight << enc("\n")
			<< enc("\tm_flWeightDeltaRate: ") << animLayer.m_flWeightDeltaRate << enc("\n")
			<< enc("\tm_flPlaybackRate: ") << animLayer.m_flPlaybackRate << enc("\n")
			<< enc("\tm_flCycle: ") << animLayer.m_flCycle << enc("\n")
			<< enc("\tACTIVITY: ") << scast<int>(GetSequenceActivity(animLayer.m_nSequence)) << enc("\n");
		return ss.str();
#endif
	}
	return enc("");
}

SOURCE_END