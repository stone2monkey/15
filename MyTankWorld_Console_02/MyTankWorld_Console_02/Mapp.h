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

	// 返回值是坦克在vector中的位置, ERROR_RETURN_VALUE为失败
	int GetTankPosAtVec(CTank* pTank);
	//返回值是子弹在vector中的位置, ERROR_RETURN_VALUE为失败
	int GetCurrentBulletPosAtVec(CBullet* pBullet);

	// 得到我的坦克的对象的指针 
	CTank* GetMyselfTank();
	// 得到友方坦克对象的指针
	CTank* GetFriendTank();
};

