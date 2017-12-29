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

		m_iLastShotTime = ERROR_RETURN_VALUE;		//С���ܱ�֤��һ��100%����;
		m_iLastMoveTime = ERROR_RETURN_VALUE;		//С���ܱ�֤��һ��100%����;
	}
	~CTank();

	// ***************************************************************************
	vector<CPoint> m_vecTankSixPointPos;
	int m_iShotSpeed;		//�����ӵ����ٶ�
	int m_iLastShotTime;	//�����ӵ������ʱ���;
	int m_iMoveSpeed;	//�ӵ�,̹�˵��ƶ��ٶ�;
	int m_iLastMoveTime;	//̹�˵�����ƶ�ʱ���;

	// ***************************************************************************
	vector<CPoint>& FillTankSixPointPos();
	CBullet* ShotBullet();
	CBullet* ShotBulletPro(int g_iCurrentTime);
	void DrawMyself();
	void ClearMyself();
};

