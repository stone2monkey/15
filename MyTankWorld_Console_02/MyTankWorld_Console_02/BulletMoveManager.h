#pragma once
#include "Mapp.h"


/*
在这个对象调用前,可见的子弹pBullet->:m_oCollision.bVisible都是显示的;
在移动前,需要把可见的子弹擦除;
然后移动子弹,做碰撞判断,收集自己与碰撞对象的结构体pBullet->m_oCollision[2];
*/
class CBulletMoveManager
{
public:
	CBulletMoveManager();
	~CBulletMoveManager();
	
	//methods************************************************************
	void CollisionCheck(CBullet* left_pBullet, CMapp* pMapp);			
	void CollisionCheckOfMyself(CBullet* left_pBullet, CMapp* pMapp);
	void CollisionCheckOfEnemy(CBullet* left_pBullet, CMapp* pMapp);

	void RespondCollision(CBullet* left_pBullet, CMapp* pMapp);
	void RespondCollisionOfMyself(CBullet* left_pBullet, CMapp* pMapp);
	void RespondCollisionOfEnemy(CBullet* left_pBullet, CMapp* pMapp);

	// 移动子弹
	void Move(CBullet* left_pBullet, CMapp* pMapp);
	// 自动移动子弹
	void MoveBulletAutomatic(CMapp* pMapp);
	// 敌坦自动发射子弹
	bool ShotBulletAutomaticForEnemy(CMapp* pMapp);
	//attributions*********************************************************
	bool am_DrawLeftCollisioner(CBullet* left_pBullet);
	void am_DelAndEraseCollisioner(CBullet* left_pBullet, CMapp* pMapp);
	// 清除所有血值小于等于0的对象
	void ClearNoBloodElement(CMapp* pMapp);
};