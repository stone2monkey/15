#include "stdafx.h"
#include "DrawGaming.h"


CDrawGaming::CDrawGaming(CMapp& rMapp, CGameData& rGameData) : m_rMapp(rMapp), m_rGameData(rGameData)
{
}


CDrawGaming::~CDrawGaming()
{
}


//************************************
// 名称: WriteChar
// 功能: 设置光标位置，并在指定位置打印指定颜色的字符串
// 继承: private 
// 返回: void
// 参数: int Wide       行
// 参数: int High       列
// 参数: string pszChar 字符串
// 参数: WORD wArr      颜色
//************************************
void CDrawGaming::WriteChar(int x, int y, char* pszChar, WORD wColor)
{
	// 设置光标属性
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 1;
	cci.bVisible = FALSE;	        // 是否显示光标
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
	// 设置坐标
	COORD loc;
	loc.X = x * 2;               // x的值是Wide的2倍
	loc.Y = y;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), loc);
	printf(pszChar); //打印时需要注意有些特殊字符是占两个字节
}

void CDrawGaming::UpdateMapp()
{
	// 0. 填充更新地图数组
	// 1. 通过更新地图数组绘图
	// 2. 绘制坦克等级
	// 3. 更新旧的地图数组
	// 4. 绘制分数

	am_FillUpdateMappArr();
	am_DrawUpdateViaArr();
	am_DrawTankLevel();
	am_UpdateOldMappArr();

	//绘制分数
	static int iScore = m_rGameData.m_iScore;
	if (iScore != m_rGameData.m_iScore)
	{
		iScore = m_rGameData.m_iScore;
		WriteChar(40, 3, "      ", WHITE);
		char ch[10] = {};
		::_itoa_s(m_rGameData.m_iScore, ch, 10);
		WriteChar(40, 3, ch, WHITE);
	}
}

void CDrawGaming::am_FillUpdateMappArr()
{
	//更新当前地图数组
	m_rMapp.FillCurMappArr();

	//比较
	for (int i = 0; i < BOTTOM_BULLET_WND + 1; i++)
	{
		for (int j = 0; j < RIGHT_BULLET_WND + 1; j++)
		{
			if (m_rMapp.m_arrCurMapp[i][j] == m_rMapp.m_arrOldMapp[i][j])
				m_arrUpdateMapp[i][j] = '=';
			else
				m_arrUpdateMapp[i][j] = m_rMapp.m_arrCurMapp[i][j];
		}
	}
}

void CDrawGaming::am_DrawUpdateViaArr()
{
	for (int i = 0; i < BOTTOM_BULLET_WND + 1; i++)
	{
		for (int j = 0; j < RIGHT_BULLET_WND + 1; j++)
		{
			switch (m_arrUpdateMapp[i][j])
			{
				case '=':
					break;

				case NULL_SPACE:
					WriteChar(j, i, "  ", WHITE);
					break;

				case ONEPLAYER_WHITE_TANK:
					WriteChar(j, i, "■", WHITE);
					break;
				case TWOPLAYER_WHITE_TANK:
					WriteChar(j, i, "■", WHITE);
					break;
				case ENEMY_RED_TANK:
					WriteChar(j, i, "■", RED);
					break;
				case ENEMY_YELLOW_TANK:
					WriteChar(j, i, "■", YELLOW);
					break;
				case ENEMY_GREEN_TANK:
					WriteChar(j, i, "■", GREEN);
					break;

				case ONEPLAYER_WHITE_BULLET:
					WriteChar(j, i, "＠", WHITE);
					break;
				case TWOPLAYER_WHITE_BULLET:
					WriteChar(j, i, "＠", WHITE);
					break;
				case ENEMY_RED_BULLET:
					WriteChar(j, i, "＠", RED);
					break;
				case ENEMY_YELLOW_BULLET:
					WriteChar(j, i, "＠", YELLOW);
					break;
				case ENEMY_GREEN_BULLET:
					WriteChar(j, i, "＠", GREEN);
					break;

				case STEEL_WHITE_BRICK:
					WriteChar(j, i, "■", WHITE);
					break;
				case REVER_BLUE_BRICK:
					WriteChar(j, i, "≈", BLUE);
					break;
				case NORMAL_PURPLE_BRICK:
					WriteChar(j, i, "▓", PURPLE);
					break;

				case THE_RED_KING:
					WriteChar(j, i, "王", RED);
					break;

				case TANK_CENTER_POINT:
					break;

				default:
					assert(0);
			}
		}
	}
}

void CDrawGaming::am_DrawTankLevel()
{
	int iSize = (int)m_rMapp.m_vecTank.size();
	for (int i = 0; i < iSize; i++)
	{
		CTank* pTank = m_rMapp.m_vecTank.at(i);
		int x = pTank->m_xPos;
		int y = pTank->m_yPos;

		char* str;
		switch (pTank->m_iLevel)
		{
			case LEVEL1:
				str = "①";
				break;
			case LEVEL2:
				str = "②";
				break;
			case LEVEL3:
				str = "③";
				break;

			default:
				assert(0);
		}

		WriteChar(x, y, str, pTank->m_iColor);
	}
}

void CDrawGaming::am_UpdateOldMappArr()
{
	::memmove_s(m_rMapp.m_arrOldMapp, sizeof(m_rMapp.m_arrOldMapp),m_rMapp.m_arrCurMapp, sizeof(m_rMapp.m_arrCurMapp));
}
