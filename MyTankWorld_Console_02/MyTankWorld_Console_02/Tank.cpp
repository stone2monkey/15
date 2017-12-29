#include "stdafx.h"
#include "Tank.h"


CTank::~CTank() {}


vector<CPoint>& CTank::FillTankSixPointPos()
{
	//清除上次的6点坐标
	m_vecTankSixPointPos.clear();

	CPoint pt;
	switch (m_iDirection)
	{
			//0, 1, 1,		
			//1, 1, 0,
			//0, 1, 1,
		case LEFT_DIRECTION:
		{
			pt = {m_xPos, m_yPos };
			m_vecTankSixPointPos.push_back(pt);
			pt = { m_xPos, m_yPos - 1 };
			m_vecTankSixPointPos.push_back(pt);
			pt = { m_xPos + 1, m_yPos - 1 };
			m_vecTankSixPointPos.push_back(pt);
			pt = { m_xPos - 1, m_yPos };
			m_vecTankSixPointPos.push_back(pt);
			pt = { m_xPos, m_yPos + 1 };
			m_vecTankSixPointPos.push_back(pt);
			pt = { m_xPos + 1, m_yPos + 1 };
			m_vecTankSixPointPos.push_back(pt);
		}break;
		//0, 1, 0,
		//1, 1, 1,
		//1, 0, 1,
		case TOP_DIRECTION:
		{
			pt = { m_xPos, m_yPos };
			m_vecTankSixPointPos.push_back(pt);
			pt = { m_xPos, m_yPos - 1 };
			m_vecTankSixPointPos.push_back(pt);
			pt = { m_xPos - 1, m_yPos };
			m_vecTankSixPointPos.push_back(pt);
			pt = { m_xPos + 1, m_yPos };
			m_vecTankSixPointPos.push_back(pt);
			pt = { m_xPos - 1, m_yPos + 1 };
			m_vecTankSixPointPos.push_back(pt);
			pt = { m_xPos + 1, m_yPos + 1 };
			m_vecTankSixPointPos.push_back(pt);
		}break;
		//1, 1, 0,
		//0, 1, 1,
		//1, 1, 0,
		case RIGHT_DIRECTION:
		{
			pt = { m_xPos, m_yPos };
			m_vecTankSixPointPos.push_back(pt);
			pt = { m_xPos - 1, m_yPos - 1 };
			m_vecTankSixPointPos.push_back(pt);
			pt = { m_xPos, m_yPos - 1 };
			m_vecTankSixPointPos.push_back(pt);
			pt = { m_xPos + 1, m_yPos };
			m_vecTankSixPointPos.push_back(pt);
			pt = { m_xPos - 1, m_yPos + 1 };
			m_vecTankSixPointPos.push_back(pt);
			pt = { m_xPos, m_yPos + 1 };
			m_vecTankSixPointPos.push_back(pt);
		}break;
		//1, 0, 1,
		//1, 1, 1,
		//0, 1, 0,
		case BOTTOM_DIRECTION:
		{
			pt = { m_xPos, m_yPos };
			m_vecTankSixPointPos.push_back(pt);
			pt = { m_xPos - 1, m_yPos - 1 };
			m_vecTankSixPointPos.push_back(pt);
			pt = { m_xPos + 1, m_yPos - 1 };
			m_vecTankSixPointPos.push_back(pt);
			pt = { m_xPos - 1, m_yPos };
			m_vecTankSixPointPos.push_back(pt);
			pt = { m_xPos + 1, m_yPos };
			m_vecTankSixPointPos.push_back(pt);
			pt = { m_xPos, m_yPos + 1 };
			m_vecTankSixPointPos.push_back(pt);
		}break;
		default:
		{
			assert(0);
			break;
		}
	}

	return m_vecTankSixPointPos;
}


CBullet* CTank::ShotBullet()
{
	int xPos, yPos, iIDFlag, iLevel, iDirection;
	if (LEFT_DIRECTION == m_iDirection)
	{
		xPos = m_xPos - 2;
		yPos = m_yPos;
		iIDFlag = m_iIDFlag + 5;
		iLevel = m_iLevel;
		iDirection = m_iDirection;
	}
	else if (TOP_DIRECTION == m_iDirection)
	{
		xPos = m_xPos;
		yPos = m_yPos - 2;
		iIDFlag = m_iIDFlag + 5;
		iLevel = m_iLevel;
		iDirection = m_iDirection;
	}
	else if (RIGHT_DIRECTION == m_iDirection)
	{
		xPos = m_xPos + 2;
		yPos = m_yPos;
		iIDFlag = m_iIDFlag + 5;
		iLevel = m_iLevel;
		iDirection = m_iDirection;
	}
	else
	{
		xPos = m_xPos;
		yPos = m_yPos + 2;
		iIDFlag = m_iIDFlag + 5;
		iLevel = m_iLevel;
		iDirection = m_iDirection;
	}

	if ('5' == m_iIDFlag)
		iIDFlag = 'a';

	return new CBullet(xPos, yPos, iIDFlag, iLevel, iDirection);
}


void CTank::DrawMyself()
{
	FillTankSixPointPos();

	char* str;
	switch (m_iLevel)
	{
		case LEVEL0:
			str = "①";
			break;
		case LEVEL1:
			str = "②";
			break;
		case LEVEL2:
			str = "③";
			break;
		default:
			assert(0);
			break;
	}

	WriteChar(m_xPos, m_yPos, str, m_iColor);
	for (int i = 1; i < 6; i++)
		WriteChar(m_vecTankSixPointPos.at(i).m_xPos, m_vecTankSixPointPos.at(i).m_yPos, "■", m_iColor);
}


void CTank::ClearMyself()
{
	FillTankSixPointPos();
	for (int i = 0; i < 6; i++)
		WriteChar(m_vecTankSixPointPos.at(i).m_xPos, m_vecTankSixPointPos.at(i).m_yPos, "  ", m_iColor);
}


CBullet* CTank::ShotBulletPro(int g_iCurrentTime)
{
	//创建子弹对象,并放入vector;
	if (g_iCurrentTime < m_iLastShotTime)
		assert(0);
	//如果发射间隔不够不能发射;
	if (g_iCurrentTime - m_iLastShotTime > m_iShotSpeed)
	{		
		//当前时间给最后创建时间
		m_iLastShotTime = g_iCurrentTime;
		return ShotBullet();
	}

	return nullptr;
}
