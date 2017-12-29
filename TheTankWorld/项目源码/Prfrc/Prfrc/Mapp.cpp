#include "stdafx.h"
#include <fstream>
#include "Mapp.h"

using namespace std;

CMapp::CMapp(string strMappFileName, bool bOnePlayer) :
	am_strMappFileName(strMappFileName), am_bOnePlayer(bOnePlayer)
{
	Init();
}


CMapp::~CMapp()
{
	int iSize = (int)m_vecTank.size();
	for (int i = 0; i < iSize; i++)
	{
		if (nullptr == m_vecTank.at(i))
			assert(0);

		delete m_vecTank.at(i);
	}

	iSize = (int)m_vecBullet.size();
	for (int i = 0; i < iSize; i++)
	{
		if (nullptr == m_vecBullet.at(i))
			assert(0);

		delete m_vecBullet.at(i);
	}

	iSize = (int)m_vecBrick.size();
	for (int i = 0; i < iSize; i++)
	{
		if (nullptr == m_vecBrick.at(i))
			assert(0);

		delete m_vecBrick.at(i);
	}

	if (nullptr != m_pKing)
	{
		delete m_pKing;
		m_pKing = nullptr;
	}
}

void CMapp::Init()
{
	m_vecTank.reserve(10);
	m_vecBullet.reserve(10);
	m_vecBrick.reserve(10);
	m_pKing = nullptr;

	InitInitMappArr();
	InitObj();
}

void CMapp::InitInitMappArr()
{
	am_FillInitMappArrViaFile();
	am_OnePlayer();
}

void CMapp::am_FillInitMappArrViaFile()
{
	ifstream iFile(am_strMappFileName, ios::in);
	assert(iFile);

	// 读文件
	char buff[0xff] = {};
	for (int i = 0; i < BOTTOM_BULLET_WND + 1; i++)
	{
		// 得到相应行数据
		iFile.getline(buff, RIGHT_BULLET_WND + 2);

		//读行入数组
		for (int j = 0; j < RIGHT_BULLET_WND + 1; j++)
		{
			am_CharToInt(buff[j]);
			m_arrInitMapp[i][j] = buff[j];
		}
	}
}

void CMapp::am_CharToInt(char& ch)
{
	if (ch >= '0' && ch <= '9')
	{
		ch -= '0';
	}
	else
	{
		ch -= ('a' - 10);
	}
}

void CMapp::am_OnePlayer()
{
	if (!am_bOnePlayer)
		return;

	for (int i = 0; i < BOTTOM_BULLET_WND + 1; i++)
	{
		for (int j = 0; j < RIGHT_BULLET_WND + 1; j++)
		{
			if (2 == m_arrInitMapp[i][j])
			{
				m_arrInitMapp[i][j] = 0;
				return;
			}
		}
	}
}

void CMapp::InitObj()
{
	for (int i = 0; i < BOTTOM_BULLET_WND + 1; i++)
	{
		for (int j = 0; j < RIGHT_BULLET_WND + 1; j++)
		{
			switch (m_arrInitMapp[i][j])
			{
				case NULL_SPACE:
					break;
					// --------------------------
				case ONEPLAYER_WHITE_TANK:
					m_vecTank.push_back(new CTank(j, i, ONEPLAYER_WHITE_TANK, 3));
					break;
				case TWOPLAYER_WHITE_TANK:
					m_vecTank.push_back(new CTank(j, i, TWOPLAYER_WHITE_TANK, 2));
					break;
				case ENEMY_RED_TANK:
					m_vecTank.push_back(new CTank(j, i, ENEMY_RED_TANK, 1, 3));
					break;
				case ENEMY_YELLOW_TANK:
					m_vecTank.push_back(new CTank(j, i, ENEMY_YELLOW_TANK, 2, 3));
					break;
				case ENEMY_GREEN_TANK:
					m_vecTank.push_back(new CTank(j, i, ENEMY_GREEN_TANK, 3, 3));
					break;
					// --------------------------
				case ONEPLAYER_WHITE_BULLET:
					m_vecBullet.push_back(new CBullet(j, i, ONEPLAYER_WHITE_BULLET));
					break;
				case TWOPLAYER_WHITE_BULLET:
					m_vecBullet.push_back(new CBullet(j, i, TWOPLAYER_WHITE_BULLET));
					break;
				case ENEMY_RED_BULLET:
					m_vecBullet.push_back(new CBullet(j, i, ENEMY_RED_BULLET));
					break;
				case ENEMY_YELLOW_BULLET:
					m_vecBullet.push_back(new CBullet(j, i, ENEMY_YELLOW_BULLET));
					break;
				case ENEMY_GREEN_BULLET:
					m_vecBullet.push_back(new CBullet(j, i, ENEMY_GREEN_BULLET));
					break;
					// --------------------------
				case STEEL_WHITE_BRICK:
					m_vecBrick.push_back(new CBrick(j, i, STEEL_WHITE_BRICK));
					break;
				case REVER_BLUE_BRICK:
					m_vecBrick.push_back(new CBrick(j, i, REVER_BLUE_BRICK));
					break;
				case NORMAL_PURPLE_BRICK:
					m_vecBrick.push_back(new CBrick(j, i, NORMAL_PURPLE_BRICK));
					break;
					// --------------------------
				case THE_RED_KING:
					m_pKing = new CKing(j, i, THE_RED_KING);
					break;
					// --------------------------
				default:
					assert(0);
			}
		}
	}
}

void CMapp::FillCurMappArr()
{
	// 0. 清空当前地图
	// 1. 填充不带子弹的当前地图数组
	// 2. 填充子弹(子弹可见性确定好了)
	::memset(m_arrCurMapp, 0, sizeof(m_arrCurMapp));
	FillWithoutBullets();
	FillBullets();
}

void CMapp::FillWithoutBullets()
{
	// 0. Tank
	int iSize = (int)m_vecTank.size();
	for (int i = 0; i < iSize ; i++)
	{
		CTank* pTank = m_vecTank.at(i);
		am_FillTankSixPtIntoCurMappArr(pTank);
	}

	// 1. Brick
	iSize = (int)m_vecBrick.size();
	for (int i = 0; i < iSize; i++)
	{
		CBrick* pBrick = m_vecBrick.at(i);
		int x = pBrick->m_xPos;
		int y = pBrick->m_yPos;
		m_arrCurMapp[y][x] = pBrick->m_iID;
	}

	// 2. King
	if (nullptr != m_pKing)
	{
		int x = m_pKing->m_xPos;
		int y = m_pKing->m_yPos;
		m_arrCurMapp[y][x] = m_pKing->m_iID;
	}
}

void CMapp::am_FillTankSixPtIntoCurMappArr(CTank * pTank)
{
	pTank->FillSixPtArr();
	for (int i = 1; i < 6 ; i++)
	{
		int x = pTank->m_arrSixPt[i].m_x;
		int y = pTank->m_arrSixPt[i].m_y;
		m_arrCurMapp[y][x] = pTank->m_iID;
	}
	m_arrCurMapp[pTank->m_yPos][pTank->m_xPos] = TANK_CENTER_POINT;
}

void CMapp::FillBullets()
{
	int iSize = (int)m_vecBullet.size();
	for (int i = 0; i < iSize ; i++)
	{
		CBullet* pBullet = m_vecBullet.at(i);
		am_VisibilityOfBullets(pBullet);
	}
}

void CMapp::am_VisibilityOfBullets(CBullet* pBullet)
{
	int x = pBullet->m_xPos;
	int y = pBullet->m_yPos;

	if (0 == m_arrCurMapp[y][x])
	{
		m_arrCurMapp[y][x] = pBullet->m_iID;
	}
}

CTank * CMapp::GetTank(int iID)
{
	// 0. validity of param
	if (iID<ONEPLAYER_WHITE_TANK || iID>ENEMY_GREEN_TANK)
		assert(0);

	// 1. find and return
	int iSize = (int)m_vecTank.size();
	for (int i = 0; i < iSize ; i++)
	{
		CTank* pTank = m_vecTank.at(i);
		if (iID == pTank->m_iID)
			return pTank;
	}

	return nullptr;
}

void CMapp::deleteAndErase(CBullet * pBullet, int iIndex)
{
	delete pBullet;
	m_vecBullet.erase(m_vecBullet.begin() + iIndex);
}

void CMapp::deleteAndErase(CTank * pTank, int iIndex)
{
	delete pTank;
	m_vecTank.erase(m_vecTank.begin() + iIndex);
}

void CMapp::deleteAndErase(CBrick * pBrick, int iIndex)
{
	delete pBrick;
	m_vecBrick.erase(m_vecBrick.begin() + iIndex);
}

void CMapp::ClearWhoNoBlood()
{
	//tank
	for (int i = 0; i < (int)m_vecTank.size(); i++)
	{
		CTank * pTank = m_vecTank.at(i);
		if (0 >= pTank->m_iBlood)
		{
			deleteAndErase(pTank, i);
			i--;
		}
	}

	//bullet
	for (int i = 0; i < (int)m_vecBullet.size(); i++)
	{
		CBullet* pBullet = m_vecBullet.at(i);
		if (0 >= pBullet->m_iBlood)
		{
			deleteAndErase(pBullet, i);
			i--;
		}
	}

	//brick
	for (int i = 0; i < (int)m_vecBrick.size(); i++)
	{
		CBrick * pBrick = m_vecBrick.at(i);
		if (0 >= pBrick->m_iBlood)
		{
			deleteAndErase(pBrick, i);
			i--;
		}
	}

	//king
	if (0 >= m_pKing->m_iBlood)
	{
		delete m_pKing;
		m_pKing = nullptr;
	}
}
