#include "stdafx.h"
#include "GameData.h"


CGameData::CGameData()
{
}


CGameData::~CGameData()
{
}

void CGameData::ReSetForNewSwitchCard()
{
	m_iScore = 0;
	m_bGameOver = false;
	for (int i = 0; i < 5; i++)
	{
		m_iTankCount[i] = 5;
		m_bCreateTank[i] = false;
		m_bCreateTank[i] = false;
	}
	m_iTankCount[2] = 10;
}


