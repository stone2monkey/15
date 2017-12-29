#include "stdafx.h"
#include "DrawGaming.h"


CDrawGaming::CDrawGaming(CMapp& rMapp, CGameData& rGameData) : m_rMapp(rMapp), m_rGameData(rGameData)
{
}


CDrawGaming::~CDrawGaming()
{
}


//************************************
// ����: WriteChar
// ����: ���ù��λ�ã�����ָ��λ�ô�ӡָ����ɫ���ַ���
// �̳�: private 
// ����: void
// ����: int Wide       ��
// ����: int High       ��
// ����: string pszChar �ַ���
// ����: WORD wArr      ��ɫ
//************************************
void CDrawGaming::WriteChar(int x, int y, char* pszChar, WORD wColor)
{
	// ���ù������
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 1;
	cci.bVisible = FALSE;	        // �Ƿ���ʾ���
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
	// ��������
	COORD loc;
	loc.X = x * 2;               // x��ֵ��Wide��2��
	loc.Y = y;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), loc);
	printf(pszChar); //��ӡʱ��Ҫע����Щ�����ַ���ռ�����ֽ�
}

void CDrawGaming::UpdateMapp()
{
	// 0. �����µ�ͼ����
	// 1. ͨ�����µ�ͼ�����ͼ
	// 2. ����̹�˵ȼ�
	// 3. ���¾ɵĵ�ͼ����
	// 4. ���Ʒ���

	am_FillUpdateMappArr();
	am_DrawUpdateViaArr();
	am_DrawTankLevel();
	am_UpdateOldMappArr();

	//���Ʒ���
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
	//���µ�ǰ��ͼ����
	m_rMapp.FillCurMappArr();

	//�Ƚ�
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
					WriteChar(j, i, "��", WHITE);
					break;
				case TWOPLAYER_WHITE_TANK:
					WriteChar(j, i, "��", WHITE);
					break;
				case ENEMY_RED_TANK:
					WriteChar(j, i, "��", RED);
					break;
				case ENEMY_YELLOW_TANK:
					WriteChar(j, i, "��", YELLOW);
					break;
				case ENEMY_GREEN_TANK:
					WriteChar(j, i, "��", GREEN);
					break;

				case ONEPLAYER_WHITE_BULLET:
					WriteChar(j, i, "��", WHITE);
					break;
				case TWOPLAYER_WHITE_BULLET:
					WriteChar(j, i, "��", WHITE);
					break;
				case ENEMY_RED_BULLET:
					WriteChar(j, i, "��", RED);
					break;
				case ENEMY_YELLOW_BULLET:
					WriteChar(j, i, "��", YELLOW);
					break;
				case ENEMY_GREEN_BULLET:
					WriteChar(j, i, "��", GREEN);
					break;

				case STEEL_WHITE_BRICK:
					WriteChar(j, i, "��", WHITE);
					break;
				case REVER_BLUE_BRICK:
					WriteChar(j, i, "��", BLUE);
					break;
				case NORMAL_PURPLE_BRICK:
					WriteChar(j, i, "��", PURPLE);
					break;

				case THE_RED_KING:
					WriteChar(j, i, "��", RED);
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
				str = "��";
				break;
			case LEVEL2:
				str = "��";
				break;
			case LEVEL3:
				str = "��";
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
