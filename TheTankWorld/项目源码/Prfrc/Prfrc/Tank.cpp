#include "stdafx.h"
#include "Tank.h"


CTank::CTank(int xPos, int yPos, int iID, int iLevel, int iDirect) :
	CBase(xPos, yPos, iID, iLevel)
{
	Init(iDirect);
}


CTank::~CTank()
{
}

void CTank::Init(int iDirect)
{
	// 1. validity of params
	if (iDirect <LEFT_DIRECTION || iDirect >BOTTOM_DIRECTION)
		assert(0);

	m_iDirect = iDirect;

	switch (m_iLevel)
	{
		case LEVEL1:
			m_uMoveSpeed = 300;
			m_uShotSpeed = 1000;
			break;
		case LEVEL2:
			m_uMoveSpeed = 150;
			m_uShotSpeed = 600;
			break;
		case LEVEL3:
			m_uMoveSpeed = 50;
			m_uShotSpeed = 200;
			break;
		default:
			assert(0);
	}
}

void CTank::Move(int iDirect)
{
	switch (iDirect)
	{
		case LEFT_DIRECTION:
			am_MoveToLeft();
			break;
		case TOP_DIRECTION:
			am_MoveToTop();
			break;
		case RIGHT_DIRECTION:
			am_MoveToRight();
			break;
		case BOTTOM_DIRECTION:
			am_MoveToBottom();
			break;
		default:
			assert(0);
	}
}

void CTank::am_MoveToLeft()
{
	m_xPos--;
	m_iDirect = LEFT_DIRECTION;
}

void CTank::am_MoveToTop()
{
	m_yPos--;
	m_iDirect = TOP_DIRECTION;
}

void CTank::am_MoveToRight()
{
	m_xPos++;
	m_iDirect = RIGHT_DIRECTION;
}

void CTank::am_MoveToBottom()
{
	m_yPos++;
	m_iDirect = BOTTOM_DIRECTION;
}

bool CTank::JustChangeDirict(int iDirect)
{
	if (m_iDirect == iDirect)
		return false;

	switch (iDirect)
	{
		case LEFT_DIRECTION:
			m_iDirect = LEFT_DIRECTION;
			return true;
			break;
		case TOP_DIRECTION:
			m_iDirect = TOP_DIRECTION;
			return true;
			break;
		case RIGHT_DIRECTION:
			m_iDirect = RIGHT_DIRECTION;
			return true;
			break;
		case BOTTOM_DIRECTION:
			m_iDirect = BOTTOM_DIRECTION;
			return true;
			break;
		default:
			assert(0);
	}

	return false;
}

void CTank::FillSixPtArr()
{
	switch (m_iDirect)
	{
		//	0  1  1
		//	1  1  0
		//	0  1  1
		case LEFT_DIRECTION:
		{
			int arr[9] = { 0, 1, 1, 1, 1, 0, 0, 1, 1 };
			am_FillSixPtArrViaArr(arr);
		}break;

		//	0  1  0
		//	1  1  1
		//	1  0  1
		case TOP_DIRECTION:
		{
			int arr[9] = { 0, 1, 0, 1, 1, 1, 1, 0, 1 };
			am_FillSixPtArrViaArr(arr);
		}break;

		//	1  1  0
		//	0  1  1
		//	1  1  0
		case RIGHT_DIRECTION:
		{
			int arr[9] = { 1, 1, 0, 0, 1, 1, 1, 1, 0 };
			am_FillSixPtArrViaArr(arr);
		}break;

		//	1  0  1
		//	1  1  1
		//	0  1  0
		case BOTTOM_DIRECTION:
		{
			int arr[9] = { 1, 0, 1, 1, 1, 1, 0, 1, 0 };
			am_FillSixPtArrViaArr(arr);
		}break;

		default:
			assert(0);
	}
}

void CTank::am_FillSixPtArrViaArr(int arr[10])
{
	//m_arrSixPt[0] ~ m_arrSixPt[5]
	int i = 1;
	if (arr[0])
	{
		m_arrSixPt[i] = { m_xPos - 1, m_yPos - 1 };
		i++;
	}
	if (arr[1])
	{
		m_arrSixPt[i] = { m_xPos, m_yPos - 1 };
		i++;
	}
	if (arr[2])
	{
		m_arrSixPt[i] = { m_xPos + 1, m_yPos - 1 };
		i++;
	}
	if (arr[3])
	{
		m_arrSixPt[i] = { m_xPos - 1, m_yPos };
		i++;
	}
	if (arr[4])
	{
		m_arrSixPt[0] = { m_xPos, m_yPos };
	}
	if (arr[5])
	{
		m_arrSixPt[i] = { m_xPos + 1, m_yPos };
		i++;
	}
	if (arr[6])
	{
		m_arrSixPt[i] = { m_xPos - 1, m_yPos + 1 };
		i++;
	}
	if (arr[7])
	{
		m_arrSixPt[i] = { m_xPos, m_yPos + 1 };
		i++;
	}
	if (arr[8])
	{
		m_arrSixPt[i] = { m_xPos + 1, m_yPos + 1 };
		i++;
	}
}

CBullet* CTank::ShotBullet()
{
	int x, y, iID, iLevel, iDirect;
	iID = m_iID + 5;	//每个类型的坦克+5,刚好是他类型的子弹
	iLevel = m_iLevel;
	iDirect = m_iDirect;

	switch (iDirect)
	{
		case LEFT_BULLET_WND:
			x = m_xPos - 2;
			y = m_yPos;
			break;
		case TOP_DIRECTION:
			x = m_xPos;
			y = m_yPos - 2;
			break;
		case RIGHT_DIRECTION:
			x = m_xPos + 2;
			y = m_yPos;
			break;
		case BOTTOM_DIRECTION:
			x = m_xPos;
			y = m_yPos + 2;
			break;

		default:
			assert(0);
	}

	if (x < LEFT_BULLET_WND || x> RIGHT_BULLET_WND || y< TOP_BULLET_WND || y > BOTTOM_BULLET_WND)
		return nullptr;

	return new CBullet(x, y, iID, iLevel, iDirect);
}
