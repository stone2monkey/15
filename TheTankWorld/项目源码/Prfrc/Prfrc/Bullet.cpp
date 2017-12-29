#include "stdafx.h"
#include "Bullet.h"


CBullet::CBullet(int xPos, int yPos, int iID, int iLevel, int iDirect) :
	CBase(xPos, yPos, iID, iLevel)
{
	Init(iDirect);
}


CBullet::~CBullet()
{
}


void CBullet::Init(int iDirect)
{
	// 1. validity of params
	if (iDirect <LEFT_DIRECTION || iDirect >BOTTOM_DIRECTION)
		assert(0);

	m_iDirect = iDirect;

	switch (m_iLevel)
	{
		case LEVEL1:
			m_iForce = 1;
			m_uMoveSpeed = 400;
			break;
		case LEVEL2:
			m_iForce = 2;
			m_uMoveSpeed = 200;
			break;
		case LEVEL3:
			m_iForce = 3;
			m_uMoveSpeed = 50;
			break;
		default:
			assert(0);
	}
}


void CBullet::Move(int iDirect)
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

void CBullet::am_MoveToLeft()
{
	m_xPos--;
}

void CBullet::am_MoveToTop()
{
	m_yPos--;
}

void CBullet::am_MoveToRight()
{
	m_xPos++;
}

void CBullet::am_MoveToBottom()
{
	m_yPos++;
}
