#pragma once
#include "Tank.h"
#include "Bullet.h"
#include "Brick.h"
#include "King.h"

class CMapp
{
public:
	CMapp();
	~CMapp();

	//Attributions *****************************************************************************
	vector<CTank*> m_vecTank;
	vector<CBullet*> m_vecBullet;
	vector<CBrick*> m_vecBrick;
	CKing* m_pKing;

	int m_arrMapp[BOTTOM_BULLET_WND+1][RIGHT_BULLET_WND+1];

	//methods*****************************************************************************
	void ReadFileDataIntom_arrMapp();
	void InitMappArray();

	// ����ֵ��̹����vector�е�λ��, ERROR_RETURN_VALUEΪʧ��
	int GetTankPosAtVec(CTank* pTank);
	//����ֵ���ӵ���vector�е�λ��, ERROR_RETURN_VALUEΪʧ��
	int GetCurrentBulletPosAtVec(CBullet* pBullet);

	// �õ��ҵ�̹�˵Ķ����ָ�� 
	CTank* GetMyselfTank();
	// �õ��ѷ�̹�˶����ָ��
	CTank* GetFriendTank();
};

