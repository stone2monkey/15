#pragma once
#include "Mapp.h"


//CTankMoveManager的前提是:碰撞检测的对象,在没有发移动前,创建副本,然后移动,用副本去做碰撞检测(这里做的是拷贝构造,注意指针的逻辑,处理),当然要排除自身;
//当不能移动时,要做换方向的检测了,方式与上面类似;
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
	//不能位移后,判断是否可以改变方向
	bool CollisionCheckOfMyselfAfterAdjustingDirection(CTank* left_Tank, CMapp* pMapp, int iMoveToFlag);
	 //确定好坦克数据后,只接做碰撞检测了
	bool am_TankCollisionCheckOfMyself(CTank& Tank, CMapp* pMapp, CTank* pTank);
	void MoveAutomatic(CMapp* pMapp);

	//attributions************************************************************
};

