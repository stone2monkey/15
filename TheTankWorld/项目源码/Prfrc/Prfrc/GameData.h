#pragma once
#include <string>
#include <windows.h>

using namespace std;

class CGameData
{
public:
	CGameData();
	~CGameData();

	//Attributions ***********************************************************************************

	string m_strMappFileName[3] = { "a.txt", "b.txt", "c.txt" };
	int m_iSwitchCard = 0;	//关卡
	bool m_bOnePlayer = false;
	int m_iScore = 0;

	// m_iTankCount[0]		ONEPLAYER_WHITE_TANK
	// m_iTankCount[1]		TWOPLAYER_WHITE_TANK
	// m_iTankCount[2]		ENEMY_RED_TANK
	// m_iTankCount[3]		ENEMY_YELLOW_TANK
	// m_iTankCount[4]		ENEMY_GREEN_TANK
	int m_iTankCount[5] = { 5, 5, 10, 5 , 5 };		//可以创建的坦克数量 

	// m_bCreateTank[0]		ONEPLAYER_WHITE_TANK
	// m_bCreateTank[1]		TWOPLAYER_WHITE_TANK
	// m_bCreateTank[2]		ENEMY_RED_TANK
	// m_bCreateTank[3]		ENEMY_YELLOW_TANK
	// m_bCreateTank[4]		ENEMY_GREEN_TANK
	bool m_bCreateTank[5] = {false,	false, false, false, false};		//是否创建坦克
	UINT m_uCreateSpeed = 3000;

	UINT m_uTankDieTime[5]{};

	bool m_bGameOver = false;



	//Method *****************************************************************************************

	void ReSetForNewSwitchCard();
};

