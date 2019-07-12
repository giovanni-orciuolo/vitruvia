// Class built with love by Vitruvia :*
// Build date: Friday 7/4/2017

#pragma once
#include "../Vitruvia.h"

SOURCE_INIT

class C_CSGameRulesProxy
{
public:
	MAKE_NETVAR(bool, m_bFreezePeriod, "m_bFreezePeriod", "DT_CSGameRulesProxy")
	MAKE_NETVAR(bool, m_bMatchWaitingForResume, "m_bMatchWaitingForResume", "DT_CSGameRulesProxy")
	MAKE_NETVAR(bool, m_bWarmupPeriod, "m_bWarmupPeriod", "DT_CSGameRulesProxy")
	MAKE_NETVAR(float, m_fWarmupPeriodEnd, "m_fWarmupPeriodEnd", "DT_CSGameRulesProxy")
	MAKE_NETVAR(float, m_fWarmupPeriodStart, "m_fWarmupPeriodStart", "DT_CSGameRulesProxy")
	MAKE_NETVAR(bool, m_bTerroristTimeOutActive, "m_bTerroristTimeOutActive", "DT_CSGameRulesProxy")
	MAKE_NETVAR(bool, m_bCTTimeOutActive, "m_bCTTimeOutActive", "DT_CSGameRulesProxy")
	MAKE_NETVAR(float, m_flTerroristTimeOutRemaining, "m_flTerroristTimeOutRemaining", "DT_CSGameRulesProxy")
	MAKE_NETVAR(float, m_flCTTimeOutRemaining, "m_flCTTimeOutRemaining", "DT_CSGameRulesProxy")
	MAKE_NETVAR(int, m_nTerroristTimeOuts, "m_nTerroristTimeOuts", "DT_CSGameRulesProxy")
	MAKE_NETVAR(int, m_nCTTimeOuts, "m_nCTTimeOuts", "DT_CSGameRulesProxy")
	MAKE_NETVAR(int, m_iRoundTime, "m_iRoundTime", "DT_CSGameRulesProxy")
	MAKE_NETVAR(int, m_gamePhase, "m_gamePhase", "DT_CSGameRulesProxy")
	MAKE_NETVAR(int, m_totalRoundsPlayed, "m_totalRoundsPlayed", "DT_CSGameRulesProxy")
	MAKE_NETVAR(int, m_nOvertimePlaying, "m_nOvertimePlaying", "DT_CSGameRulesProxy")
	MAKE_NETVAR(float, m_timeUntilNextPhaseStarts, "m_timeUntilNextPhaseStarts", "DT_CSGameRulesProxy")
	MAKE_NETVAR(float, m_flCMMItemDropRevealStartTime, "m_flCMMItemDropRevealStartTime", "DT_CSGameRulesProxy")
	MAKE_NETVAR(float, m_flCMMItemDropRevealEndTime, "m_flCMMItemDropRevealEndTime", "DT_CSGameRulesProxy")
	MAKE_NETVAR(float, m_fRoundStartTime, "m_fRoundStartTime", "DT_CSGameRulesProxy")
	MAKE_NETVAR(bool, m_bGameRestart, "m_bGameRestart", "DT_CSGameRulesProxy")
	MAKE_NETVAR(float, m_flRestartRoundTime, "m_flRestartRoundTime", "DT_CSGameRulesProxy")
	MAKE_NETVAR(float, m_flGameStartTime, "m_flGameStartTime", "DT_CSGameRulesProxy")
	MAKE_NETVAR(int, m_iHostagesRemaining, "m_iHostagesRemaining", "DT_CSGameRulesProxy")
	MAKE_NETVAR(bool, m_bAnyHostageReached, "m_bAnyHostageReached", "DT_CSGameRulesProxy")
	MAKE_NETVAR(bool, m_bMapHasBombTarget, "m_bMapHasBombTarget", "DT_CSGameRulesProxy")
	MAKE_NETVAR(bool, m_bMapHasRescueZone, "m_bMapHasRescueZone", "DT_CSGameRulesProxy")
	MAKE_NETVAR(bool, m_bMapHasBuyZone, "m_bMapHasBuyZone", "DT_CSGameRulesProxy")
	MAKE_NETVAR(bool, m_bIsQueuedMatchmaking, "m_bIsQueuedMatchmaking", "DT_CSGameRulesProxy")
	MAKE_NETVAR(bool, m_bIsValveDS, "m_bIsValveDS", "DT_CSGameRulesProxy")
	MAKE_NETVAR(bool, m_bIsQuestEligible, "m_bIsQuestEligible", "DT_CSGameRulesProxy")
	MAKE_NETVAR(bool, m_bLogoMap, "m_bLogoMap", "DT_CSGameRulesProxy")
	MAKE_NETVAR(int, m_iNumGunGameProgressiveWeaponsCT, "m_iNumGunGameProgressiveWeaponsCT", "DT_CSGameRulesProxy")
	MAKE_NETVAR(int, m_iNumGunGameProgressiveWeaponsT, "m_iNumGunGameProgressiveWeaponsT", "DT_CSGameRulesProxy")
	MAKE_NETVAR(int, m_iSpectatorSlotCount, "m_iSpectatorSlotCount", "DT_CSGameRulesProxy")
	MAKE_NETVAR(bool, m_bBombDropped, "m_bBombDropped", "DT_CSGameRulesProxy")
	MAKE_NETVAR(bool, m_bBombPlanted, "m_bBombPlanted", "DT_CSGameRulesProxy")
	MAKE_NETVAR(int, m_iRoundWinStatus, "m_iRoundWinStatus", "DT_CSGameRulesProxy")
	MAKE_NETVAR(int, m_eRoundWinReason, "m_eRoundWinReason", "DT_CSGameRulesProxy")
	MAKE_NETVAR(float, m_flDMBonusStartTime, "m_flDMBonusStartTime", "DT_CSGameRulesProxy")
	MAKE_NETVAR(float, m_flDMBonusTimeLength, "m_flDMBonusTimeLength", "DT_CSGameRulesProxy")
	MAKE_NETVAR(int, m_unDMBonusWeaponLoadoutSlot, "m_unDMBonusWeaponLoadoutSlot", "DT_CSGameRulesProxy")
	MAKE_NETVAR(bool, m_bDMBonusActive, "m_bDMBonusActive", "DT_CSGameRulesProxy")
	MAKE_NETVAR(bool, m_bTCantBuy, "m_bTCantBuy", "DT_CSGameRulesProxy")
	MAKE_NETVAR(bool, m_bCTCantBuy, "m_bCTCantBuy", "DT_CSGameRulesProxy")
	MAKE_NETVAR(float, m_flGuardianBuyUntilTime, "m_flGuardianBuyUntilTime", "DT_CSGameRulesProxy")
	MAKE_NETVAR(int, m_MatchDevice, "m_MatchDevice", "DT_CSGameRulesProxy")
	MAKE_NETVAR(bool, m_bHasMatchStarted, "m_bHasMatchStarted", "DT_CSGameRulesProxy")
	MAKE_NETVAR(int, m_nNextMapInMapgroup, "m_nNextMapInMapgroup", "DT_CSGameRulesProxy")
	MAKE_NETVAR(bool, m_bIsDroppingItems, "m_bIsDroppingItems", "DT_CSGameRulesProxy")
	MAKE_NETVAR(int, m_iActiveAssassinationTargetMissionID, "m_iActiveAssassinationTargetMissionID", "DT_CSGameRulesProxy")
	MAKE_NETVAR(float, m_fMatchStartTime, "m_fMatchStartTime", "DT_CSGameRulesProxy")
	MAKE_NETVAR_PTR(char, m_szTournamentEventName, "m_szTournamentEventName", "DT_CSGameRulesProxy") // Size = 260
	MAKE_NETVAR_PTR(char, m_szTournamentEventStage, "m_szTournamentEventStage", "DT_CSGameRulesProxy") // Size = 260
	MAKE_NETVAR_PTR(char, m_szTournamentPredictionsTxt, "m_szTournamentPredictionsTxt", "DT_CSGameRulesProxy") // Size = 260
	MAKE_NETVAR(int, m_nTournamentPredictionsPct, "m_nTournamentPredictionsPct", "DT_CSGameRulesProxy")
	MAKE_NETVAR_PTR(char, m_szMatchStatTxt, "m_szMatchStatTxt", "DT_CSGameRulesProxy") // Size = 260
	MAKE_NETVAR(int, m_nGuardianModeWaveNumber, "m_nGuardianModeWaveNumber", "DT_CSGameRulesProxy")
	MAKE_NETVAR(int, m_nGuardianModeSpecialKillsRemaining, "m_nGuardianModeSpecialKillsRemaining", "DT_CSGameRulesProxy")
	MAKE_NETVAR(int, m_nGuardianModeSpecialWeaponNeeded, "m_nGuardianModeSpecialWeaponNeeded", "DT_CSGameRulesProxy")
	MAKE_NETVAR(int, m_nHalloweenMaskListSeed, "m_nHalloweenMaskListSeed", "DT_CSGameRulesProxy")
	MAKE_NETVAR(int, m_numGlobalGiftsGiven, "m_numGlobalGiftsGiven", "DT_CSGameRulesProxy")
	MAKE_NETVAR(int, m_numGlobalGifters, "m_numGlobalGifters", "DT_CSGameRulesProxy")
	MAKE_NETVAR(int, m_numGlobalGiftsPeriodSeconds, "m_numGlobalGiftsPeriodSeconds", "DT_CSGameRulesProxy")
	MAKE_NETVAR(int, m_numBestOfMaps, "m_numBestOfMaps", "DT_CSGameRulesProxy")
};

SOURCE_END
