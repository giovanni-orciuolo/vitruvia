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

#include "../Hooks.h"

SOURCE_INIT
namespace Hooks
{

int  __fastcall hk_EmitSound(IEngineSound* _this, void* edx, IRecipientFilter& filter, int ent_index, int channel,
	const char* sound_entry, unsigned int sound_entry_hash,
	const char* sample, float volume, float attenuation, int seed, int flags, int pitch, const Vector* origin,
	const Vector* direction, CUtlVector<Vector>* vec_origins, bool update_positions, float sound_time, int speaker_entity)
{
	Vector vecOrigin;
	if (origin) vecOrigin = *origin; else vecOrigin = Vector(0, 0, 0);

	// Block crash lagger (b1g p2c feature)
	if (strstr(sample, enc("null")))
		sample = "";

	// Call OnEmitSound for each feature
	for (auto f : Vitruvia::Features)
		f->OnEmitSound(ent_index, sound_entry, sample, vecOrigin, vec_origins, sound_time);

	return EngineSoundTable->GetFunction<EmitSound_t>(I_EmitSound)(_this, filter, ent_index, channel, sound_entry, 
		sound_entry_hash, sample, volume, attenuation, seed, flags, pitch, origin, direction, vec_origins, 
		update_positions, sound_time, speaker_entity);
}

}
SOURCE_END