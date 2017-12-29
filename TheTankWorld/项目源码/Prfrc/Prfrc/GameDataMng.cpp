#include "stdafx.h"
#include "GameDataMng.h"
#include "Mapp.h"
#include "GameData.h"


CGameDataMng::CGameDataMng(CMapp& rMapp, CGameData& rGameData) :
	m_rMapp(rMapp), m_rGameData(rGameData)
{
}


CGameDataMng::~CGameDataMng()
{
}

void CGameDataMng::ReSetForNewSwitchCard()
{
	m_rGameData.ReSetForNewSwitchCard();
}

void CGameDataMng::GameOver()
{
	if (0 == m_rGameData.m_iTankCount[0] && 0 == m_rGameData.m_iTankCount[1] &&
		false == m_rGameData.m_bCreateTank[0] && false == m_rGameData.m_bCreateTank[1] &&
		nullptr == m_rMapp.GetTank(ONEPLAYER_WHITE_TANK) &&
		nullptr == m_rMapp.GetTank(TWOPLAYER_WHITE_TANK))
	{
		m_rGameData.m_bGameOver = true;
	}
}

bool CGameDataMng::NextSwitchCard()
{
	if (0 == m_rGameData.m_iTankCount[2] &&
		0 == m_rGameData.m_iTankCount[3] &&
		0 == m_rGameData.m_iTankCount[4] &&
		false == m_rGameData.m_bCreateTank[2] &&
		false == m_rGameData.m_bCreateTank[3] &&
		false == m_rGameData.m_bCreateTank[4] &&
		nullptr == m_rMapp.GetTank(ENEMY_RED_TANK) &&
		nullptr == m_rMapp.GetTank(ENEMY_YELLOW_TANK) &&
		nullptr == m_rMapp.GetTank(ENEMY_GREEN_TANK))
	{
		//可以进入下一关了
		if (++m_rGameData.m_iSwitchCard > 2)
			m_rGameData.m_iSwitchCard = 0;

		return true;
	}

	return false;
}