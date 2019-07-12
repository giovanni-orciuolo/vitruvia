// Class built with love by Vitruvia :*
// Build date: Monday 24/4/2017

#pragma once
#include "C_PlayerResource.h"

SOURCE_INIT

struct WeaponInfo_t;

class CCSWeaponSystem
{
	virtual void unused0() = 0;
	virtual void unused1() = 0;
public:
	virtual const WeaponInfo_t* GetWeaponInfo(int item_definition_index) = 0;
};

class C_CSPlayerResource : public C_PlayerResource
{
public:
	MAKE_NETVAR_ARRAY(int, m_iPlayerC4, "m_iPlayerC4", "DT_CSPlayerResource")
	MAKE_NETVAR_ARRAY(int, m_iPlayerVIP, "m_iPlayerVIP", "DT_CSPlayerResource")
	MAKE_NETVAR_ARRAY(bool, m_bHostageAlive, "m_bHostageAlive", "DT_CSPlayerResource")
	MAKE_NETVAR_ARRAY(bool, m_isHostageFollowingSomeone, "m_isHostageFollowingSomeone", "DT_CSPlayerResource")
	MAKE_NETVAR_ARRAY(int, m_iHostageEntityIDs, "m_iHostageEntityIDs", "DT_CSPlayerResource")
	MAKE_NETVAR_ARRAY(Vector, m_bombsiteCenterA, "m_bombsiteCenterA", "DT_CSPlayerResource")
	MAKE_NETVAR_ARRAY(Vector, m_bombsiteCenterB, "m_bombsiteCenterB", "DT_CSPlayerResource")
	MAKE_NETVAR_ARRAY(int, m_hostageRescueX, "m_hostageRescueX", "DT_CSPlayerResource")
	MAKE_NETVAR_ARRAY(int, m_hostageRescueY, "m_hostageRescueY", "DT_CSPlayerResource")
	MAKE_NETVAR_ARRAY(int, m_hostageRescueZ, "m_hostageRescueZ", "DT_CSPlayerResource")
	MAKE_NETVAR_ARRAY(int, m_iMVPs, "m_iMVPs", "DT_CSPlayerResource")
	MAKE_NETVAR_ARRAY(int, m_iArmor, "m_iArmor", "DT_CSPlayerResource")
	MAKE_NETVAR_ARRAY(bool, m_bHasHelmet, "m_bHasHelmet", "DT_CSPlayerResource")
	MAKE_NETVAR_ARRAY(bool, m_bHasDefuser, "m_bHasDefuser", "DT_CSPlayerResource")
	MAKE_NETVAR_ARRAY(int, m_iScore, "m_iScore", "DT_CSPlayerResource")
	MAKE_NETVAR_ARRAY(int, m_iCompetitiveRanking, "m_iCompetitiveRanking", "DT_CSPlayerResource")
	MAKE_NETVAR_ARRAY(int, m_iCompetitiveWins, "m_iCompetitiveWins", "DT_CSPlayerResource")
	MAKE_NETVAR_ARRAY(int, m_iCompTeammateColor, "m_iCompTeammateColor", "DT_CSPlayerResource")
	MAKE_NETVAR_ARRAY(bool, m_bControllingBot, "m_bControllingBot", "DT_CSPlayerResource")
	MAKE_NETVAR_ARRAY(int, m_iControlledPlayer, "m_iControlledPlayer", "DT_CSPlayerResource")
	MAKE_NETVAR_ARRAY(int, m_iControlledByPlayer, "m_iControlledByPlayer", "DT_CSPlayerResource")
	MAKE_NETVAR_ARRAY(int, m_iBotDifficulty, "m_iBotDifficulty", "DT_CSPlayerResource")
	//MAKE_NETVAR_PTR_ARRAY(char*, m_szClan, "m_szClan", "DT_CSPlayerResource")
	MAKE_NETVAR_ARRAY(int, m_iTotalCashSpent, "m_iTotalCashSpent", "DT_CSPlayerResource")
	MAKE_NETVAR_ARRAY(int, m_iCashSpentThisRound, "m_iCashSpentThisRound", "DT_CSPlayerResource")
	MAKE_NETVAR_ARRAY(int, m_nEndMatchNextMapVotes, "m_nEndMatchNextMapVotes", "DT_CSPlayerResource")
	MAKE_NETVAR_ARRAY(int, m_bEndMatchNextMapAllVoted, "m_bEndMatchNextMapAllVoted", "DT_CSPlayerResource")
	MAKE_NETVAR_ARRAY(int, m_nActiveCoinRank, "m_nActiveCoinRank", "DT_CSPlayerResource")
	MAKE_NETVAR_ARRAY(int, m_nMusicID, "m_nMusicID", "DT_CSPlayerResource")
	MAKE_NETVAR_ARRAY(int, m_nPersonaDataPublicLevel, "m_nPersonaDataPublicLevel", "DT_CSPlayerResource")
	MAKE_NETVAR_ARRAY(int, m_nPersonaDataPublicCommendsLeader, "m_nPersonaDataPublicCommendsLeader", "DT_CSPlayerResource")
	MAKE_NETVAR_ARRAY(int, m_nPersonaDataPublicCommendsTeacher, "m_nPersonaDataPublicCommendsTeacher", "DT_CSPlayerResource")
	MAKE_NETVAR_ARRAY(int, m_nPersonaDataPublicCommendsFriendly, "m_nPersonaDataPublicCommendsFriendly", "DT_CSPlayerResource")
};

SOURCE_END
