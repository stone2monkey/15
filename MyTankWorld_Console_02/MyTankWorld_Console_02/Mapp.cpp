#include "stdafx.h"
#include "Mapp.h"
#include <fstream>


CMapp::CMapp()
{
	m_vecTank.reserve(20);
	m_vecBullet.reserve(20);
	m_vecBrick.reserve(20);
	m_pKing = nullptr;
}


CMapp::~CMapp()
{
	// m_vecTank **************************************
	if (!m_vecTank.empty())
	{
		int iSize = m_vecTank.size();
		for (int i = 0; i < iSize; i++)
		{
			delete m_vecTank.at(i);
			m_vecTank.at(i) = nullptr;
		}
	}
	// m_vecBullet **************************************
	if (!m_vecBullet.empty())
	{
		int iSize = m_vecBullet.size();
		for (int i = 0; i < iSize; i++)
		{
			delete m_vecBullet.at(i);
			m_vecBullet.at(i) = nullptr;
		}
	}
	// m_vecBrick **************************************
	if (!m_vecBrick.empty())
	{
		int iSize = m_vecBrick.size();
		for (int i = 0; i < iSize; i++)
		{
			delete m_vecBrick.at(i);
			m_vecBrick.at(i) = nullptr;
		}
	}
	// m_pKing **************************************
	if (nullptr != m_pKing)
	{
		delete m_pKing;
		m_pKing = nullptr;
	}
}


void CMapp::ReadFileDataIntom_arrMapp()
{
	ifstream iFile("0.txt", ios::in);
	assert(iFile);

	char buf[0xff];
	for (int i = 0; i < BOTTOM_BULLET_WND+1; i++)
	{
		//得到行数据
		iFile.getline(buf, RIGHT_BULLET_WND + 2);
		//读行入数组
		for (int j = 0; j < RIGHT_BULLET_WND+1; j++)
		{
			m_arrMapp[i][j] = buf[j];
		}
	}
	iFile.close();
}


void CMapp::InitMappArray()
{
	//数据的初始化 *****************************************************************
	ReadFileDataIntom_arrMapp();
	for (int i = 0; i < BOTTOM_BULLET_WND+1; i++)
		for (int j = 0; j < RIGHT_BULLET_WND+1; j++)
		{
			switch (m_arrMapp[i][j])
			{
				case NULL_SPACE:
					break;
				//***********************************************************
				case MYSELF_WHITE_TANK:
				{
					CTank* pTank = new CTank(j, i, MYSELF_WHITE_TANK, 2);
					m_vecTank.push_back(pTank);
				}break;
				case FRIEND_WITHE_TANK:
				{
					CTank* pTank = new CTank(j, i, FRIEND_WITHE_TANK, 0);
					m_vecTank.push_back(pTank);
				}break;
				case ENEMY_RED_TANK:
				{
					CTank* pTank = new CTank(j, i, ENEMY_RED_TANK, 0, 3);
					m_vecTank.push_back(pTank);
				}break;
				case ENEMY_YELLOW_TANK:
				{
					CTank* pTank = new CTank(j, i, ENEMY_YELLOW_TANK, 1);
					m_vecTank.push_back(pTank);
				}break;
				case ENEMY_GREEN_TANK:
				{
					CTank* pTank = new CTank(j, i, ENEMY_GREEN_TANK, 2);
					m_vecTank.push_back(pTank);
				}break;
				//***********************************************************
				case STEEL_WHITE_BRICK:
				{
					CBrick* pBrick = new CBrick(j, i, STEEL_WHITE_BRICK, 0);
					m_vecBrick.push_back(pBrick);
				}break;
				case REVER_BLUE_BRICK:
				{
					CBrick* pBrick = new CBrick(j, i, REVER_BLUE_BRICK, 0);
					m_vecBrick.push_back(pBrick);
				}break;
				case NORMAL_PURPLE_BRICK:
				{
					CBrick* pBrick = new CBrick(j, i, NORMAL_PURPLE_BRICK, 0);
					m_vecBrick.push_back(pBrick);
				}break;
				//***********************************************************
				case THE_RED_KING:
				{
					CKing* pKing = new CKing(j, i, THE_RED_KING, 0);
					m_pKing = pKing;
				}break;
				default:
				{
					assert(0);
					break;
				}
			}
		}

	//游戏界面的初始化;
	system("cls");
	//0. m_vecTank; ****************************************************
	int iSize = (int)m_vecTank.size();
	for (int i = 0; i < iSize; i++)
	{
		m_vecTank.at(i)->DrawMyself();
	}
	//1. m_vecBullet; ****************************************************
	iSize = (int)m_vecBullet.size();
	for (int i = 0; i < iSize; i++)
	{
		int xPos = m_vecBullet.at(i)->m_xPos;
		int yPos = m_vecBullet.at(i)->m_yPos;
		WriteChar(xPos, yPos, "＠", m_vecBullet.at(i)->m_iColor);
	}
	//2. m_vecBrick; ****************************************************
	iSize = (int)m_vecBrick.size();
	for (int i = 0; i < iSize; i++)
	{
		int xPos = m_vecBrick.at(i)->m_xPos;
		int yPos = m_vecBrick.at(i)->m_yPos;

		char* str = "     ";
		switch (m_vecBrick.at(i)->m_iIDFlag)
		{
		case STEEL_WHITE_BRICK:
		{
			str = "■";
		}break;
		case REVER_BLUE_BRICK:
		{
			str = "≈";
		}break;
		case NORMAL_PURPLE_BRICK:
		{
			str = "▓";
		}break;
		default:
		{
			assert(0);
		}break;
		}
		WriteChar(xPos, yPos, str, m_vecBrick.at(i)->m_iColor);
	}
	//3. m_pKing; ****************************************************
	if (nullptr != m_pKing)
		WriteChar(m_pKing->m_xPos, m_pKing->m_yPos, "王", m_pKing->m_iColor);
	//4. boundary bridk; ****************************************************
	for (int i = 0; i < BOTTOM_BULLET_WND + 2; i++)
		WriteChar(RIGHT_BULLET_WND + 1, i, "■", WHITE);
	for (int i = 0; i < RIGHT_BULLET_WND + 1; i++)
		WriteChar(i, BOTTOM_BULLET_WND + 1, "■", WHITE);
}

// 返回值是坦克在vector中的位置
int CMapp::GetTankPosAtVec(CTank* pTank)
{
	int iSize = (int)m_vecTank.size();
	for (int i = 0; i < iSize; i++)
	{
		if (pTank == m_vecTank.at(i))
			return i;
	}

	return ERROR_RETURN_VALUE;
}


//返回值是子弹在vector中的位置, ERROR_RETURN_VALUE为失败
int CMapp::GetCurrentBulletPosAtVec(CBullet* left_pBullet)
{
	int iSize = (int)m_vecBullet.size();
	for (int i = 0; i < iSize; i++)
	{
		if (left_pBullet == m_vecBullet.at(i))
			return i;
	}

	return ERROR_RETURN_VALUE;
}

// 得到我的坦克的对象的指针,没有找到则nullptr;
CTank* CMapp::GetMyselfTank()
{
	int iSize = (int)m_vecTank.size();
	for (int i = 0; i < iSize; i++)
	{
		CTank* pMyseftTank = m_vecTank.at(i);
		if (MYSELF_WHITE_TANK == pMyseftTank->m_iIDFlag)
			return pMyseftTank;		
	}

	return nullptr;
}


// 得到友方坦克对象的指针,没有找到则nullptr;
CTank* CMapp::GetFriendTank()
{
	int iSize = (int)m_vecTank.size();
	for (int i = 0; i < iSize; i++)
	{
		CTank* pFriendTank = m_vecTank.at(i);
		if (FRIEND_WITHE_TANK == pFriendTank->m_iIDFlag)
			return pFriendTank;
	}

	return nullptr;
}
