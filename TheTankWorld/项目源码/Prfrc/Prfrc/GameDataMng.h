#pragma once

class CMapp;
class CGameData;

class CGameDataMng
{
public:
	CGameDataMng(CMapp& rMapp, CGameData& rGameData);
	~CGameDataMng();

	//Attributions ***********************************************************************************

	CMapp& m_rMapp;
	CGameData& m_rGameData;


	//Method *****************************************************************************************

	void ReSetForNewSwitchCard();

	void GameOver();

	bool NextSwitchCard();
};

