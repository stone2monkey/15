#pragma once
#include "Base.h"
#include "Bullet.h"


class CTank :
	public CBase
{
public:
	CTank(int xPos, int yPos, int iIDFlag, int iLevel, int iDirection = 1) : CBase(xPos, yPos, iIDFlag, iLevel, iDirection) 
	{
		m_vecTankSixPointPos.reserve(6);

		switch (iLevel)
		{
		case 0:
			m_iMoveSpeed = 500;
			m_iShotSpeed = 1300;
			break;
		case 1:
			m_iMoveSpeed = 200;
			m_iShotSpeed = 800;
			break;
		case 2:
			m_iMoveSpeed = 0;
			m_iShotSpeed = 300;
			break;
		default:
			assert(0);
			break;
		}

		m_iLastShotTime = ERROR_RETURN_VALUE;		//小于能保证第一次100%运行;
		m_iLastMoveTime = ERROR_RETURN_VALUE;		//小于能保证第一次100%运行;
	}
	~CTank();

	// ***************************************************************************
	vector<CPoint> m_vecTankSixPointPos;
	int m_iShotSpeed;		//发射子弹的速度
	int m_iLastShotTime;	//发射子弹的最后时间点;
	int m_iMoveSpeed;	//子弹,坦克的移动速度;
	int m_iLastMoveTime;	//坦克的最后移动时间点;

	// ***************************************************************************
	vector<CPoint>& FillTankSixPointPos();
	CBullet* ShotBullet();
	CBullet* ShotBulletPro(int g_iCurrentTime);
	void DrawMyself();
	void ClearMyself();
};

