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

// Credits: polak + FuckFace32 for various calculations

#include "SpreadManager.h"
using namespace Source;

void Vitruvia::Tools::CSpreadManager::GetSpreads(Source::CUserCmd* cmd, Source::C_WeaponCSBase* weapon, float& spread_x, float& spread_y)
{
	cmd->random_seed = MD5::MD5_PseudoRandom(cmd->command_number) & 0x7FFFFFFF;
	Maths::RandomSeed((cmd->random_seed & 0xFF) + 1);

	float fRand1 = Maths::RandomFloat(0.f, 1.f);
	float fRandPi1 = Maths::RandomFloat(0.f, 2.f * (float)M_PI);
	float fRand2 = Maths::RandomFloat(0.f, 1.f);
	float fRandPi2 = Maths::RandomFloat(0.f, 2.f * (float)M_PI);

	int m_iItemDefinitionIndex = *weapon->m_iItemDefinitionIndex();
	float m_flRecoilIndex = weapon->m_flRecoilIndex();

	if (m_iItemDefinitionIndex == WEAPON_REVOLVER)
	{
		if (cmd->buttons & IN_ATTACK2)
		{
			fRand1 = 1.f - fRand1 * fRand1;
			fRand2 = 1.f - fRand2 * fRand2;
		}
	}
	else if (m_iItemDefinitionIndex == WEAPON_NEGEV && m_flRecoilIndex < 3.f)
	{
		for (int i = 3; i > m_flRecoilIndex; --i)
		{
			fRand1 *= fRand1;
			fRand2 *= fRand2;
		}

		fRand1 = 1.f - fRand1;
		fRand2 = 1.f - fRand2;
	}

	float fRandInaccuracy = fRand1 * weapon->GetInaccuracy();
	float fRandSpread = fRand2 * weapon->GetSpread();

	spread_x = cos(fRandPi1) * fRandInaccuracy + cos(fRandPi2) * fRandSpread;
	spread_y = sin(fRandPi1) * fRandInaccuracy + sin(fRandPi2) * fRandSpread;
}

void Vitruvia::Tools::CSpreadManager::ApplySpreadFactor(Source::CUserCmd* cmd, float spread_x, float spread_y)
{
	cmd->viewangles.x -= RAD2DEG(atan(sqrt(spread_x * spread_x + spread_y * spread_y)));
	cmd->viewangles.z = RAD2DEG(atan2(spread_x, spread_y));
}
