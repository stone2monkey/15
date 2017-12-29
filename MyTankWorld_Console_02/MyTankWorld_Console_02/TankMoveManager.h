#pragma once
#include "Mapp.h"


//CTankMoveManager��ǰ����:��ײ���Ķ���,��û�з��ƶ�ǰ,��������,Ȼ���ƶ�,�ø���ȥ����ײ���(���������ǿ�������,ע��ָ����߼�,����),��ȻҪ�ų�����;
//�������ƶ�ʱ,Ҫ��������ļ����,��ʽ����������;
class CTankMoveManager
{
public:
	CTankMoveManager();
	~CTankMoveManager();

//methods************************************************************
public:
	bool CTankMoveManager::Move(CTank* pTank, CMapp* pMapp, int iMoveToFlag);
	//bool MoveToLeft(CTank* pTank, CMapp* pMapp);
	//bool MoveToTop(CTank* pTank, CMapp* pMapp);
	//bool MoveToRight(CTank* pTank, CMapp* pMapp);
	//bool MoveToBottom(CTank* pTank, CMapp* pMapp);

	bool CollisionCheck(CTank* pTank, CMapp* pMapp, int iMoveToFlag);
	//����λ�ƺ�,�ж��Ƿ���Ըı䷽��
	bool CollisionCheckOfMyselfAfterAdjustingDirection(CTank* left_Tank, CMapp* pMapp, int iMoveToFlag);
	 //ȷ����̹�����ݺ�,ֻ������ײ�����
	bool am_TankCollisionCheckOfMyself(CTank& Tank, CMapp* pMapp, CTank* pTank);
	void MoveAutomatic(CMapp* pMapp);

	//attributions************************************************************
};

