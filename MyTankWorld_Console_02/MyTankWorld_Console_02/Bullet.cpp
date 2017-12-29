#include "stdafx.h"
#include "Bullet.h"


CBullet::~CBullet()
{
	if (m_oCollisionInfo.m_bLeftVisible)
		ClearMyself();
}


void CBullet::DrawMyself()
{
	WriteChar(m_xPos, m_yPos, "£À", m_iColor);
}


void CBullet::ClearMyself()
{
	WriteChar(m_xPos, m_yPos, "  ", m_iColor);
}