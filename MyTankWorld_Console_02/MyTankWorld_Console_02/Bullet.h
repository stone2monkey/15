#pragma once
#include "Base.h"
#include <atltime.h>


class CBullet :
	public CBase
{
public:
	CBullet(int xPos, int yPos, int iIDFlag, int iLevel, int iDirection = 1) : CBase(xPos, yPos, iIDFlag, iLevel, iDirection) 
	{
		m_oCollisionInfo.m_iCollisionCode = ERROR_RETURN_VALUE;
		m_oCollisionInfo.m_pBase = nullptr;
		m_oCollisionInfo.m_bLeftVisible = false;
		m_oCollisionInfo.m_bLeftDie = false;
		m_oCollisionInfo.m_bRightVisible = false;
		m_oCollisionInfo.m_bRightDie = false;
		m_oCollisionInfo.m_bRiver = false;

		switch (iLevel)
		{
		case 0:
			m_iMoveSpeed = 300;
			break;
		case 1:
			m_iMoveSpeed = 100;
			break;
		case 2:
			m_iMoveSpeed = 0;
			break;
		default:
			assert(0);
			break;
		}
	}
	~CBullet();

	void DrawMyself();
	void ClearMyself();

	COLLISIONINFO m_oCollisionInfo;	
	int m_iMoveSpeed;		//子弹的移动速度;
	int m_iLastMoveTime;	//子弹的最后移动时间点;
};

