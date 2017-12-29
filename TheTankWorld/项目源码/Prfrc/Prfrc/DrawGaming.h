#pragma once
#include "Mapp.h"
#include "GameData.h"

class CDrawGaming
{
public:
	CDrawGaming(CMapp& rMapp, CGameData& rGameData);
	~CDrawGaming();

	//Attributions ***********************************************************************************

	CMapp& m_rMapp;
	int m_arrUpdateMapp[BOTTOM_BULLET_WND + 1][RIGHT_BULLET_WND + 1] = {};
	CGameData& m_rGameData;

	//Method *****************************************************************************************

	void WriteChar(int x, int y, char* pszChar, WORD wColor);

	void UpdateMapp();
	void am_FillUpdateMappArr();	//CMapp的旧地图数组与当前地图数组比较,相等'=',否,用当前地图填充
	void am_DrawUpdateViaArr();
	void am_DrawTankLevel();
	void am_UpdateOldMappArr();

};

