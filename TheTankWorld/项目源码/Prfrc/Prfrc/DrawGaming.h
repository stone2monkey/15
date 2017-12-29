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
	void am_FillUpdateMappArr();	//CMapp�ľɵ�ͼ�����뵱ǰ��ͼ����Ƚ�,���'=',��,�õ�ǰ��ͼ���
	void am_DrawUpdateViaArr();
	void am_DrawTankLevel();
	void am_UpdateOldMappArr();

};

