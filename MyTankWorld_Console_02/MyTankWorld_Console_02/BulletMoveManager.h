#pragma once
#include "Mapp.h"


/*
������������ǰ,�ɼ����ӵ�pBullet->:m_oCollision.bVisible������ʾ��;
���ƶ�ǰ,��Ҫ�ѿɼ����ӵ�����;
Ȼ���ƶ��ӵ�,����ײ�ж�,�ռ��Լ�����ײ����Ľṹ��pBullet->m_oCollision[2];
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

	// �ƶ��ӵ�
	void Move(CBullet* left_pBullet, CMapp* pMapp);
	// �Զ��ƶ��ӵ�
	void MoveBulletAutomatic(CMapp* pMapp);
	// ��̹�Զ������ӵ�
	bool ShotBulletAutomaticForEnemy(CMapp* pMapp);
	//attributions*********************************************************
	bool am_DrawLeftCollisioner(CBullet* left_pBullet);
	void am_DelAndEraseCollisioner(CBullet* left_pBullet, CMapp* pMapp);
	// �������ѪֵС�ڵ���0�Ķ���
	void ClearNoBloodElement(CMapp* pMapp);
};