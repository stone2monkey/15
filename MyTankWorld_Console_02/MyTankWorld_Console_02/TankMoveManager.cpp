#include "stdafx.h"
#include "TankMoveManager.h"


CTankMoveManager::CTankMoveManager()
{
}


CTankMoveManager::~CTankMoveManager()
{
}

bool CTankMoveManager::Move(CTank* pTank, CMapp* pMapp, int iMoveToFlag)
{
	//移动速度(时间间隔)是否够;
	if (g_iCurrentTime < pTank->m_iLastMoveTime)
		assert(0);
	if (!(g_iCurrentTime - pTank->m_iLastMoveTime >= pTank->m_iMoveSpeed))
		return false;

	//如果没有碰撞,清除原图,绘制移动后的图;否则,改变方向后是否碰撞;
	if (!CollisionCheck(pTank, pMapp, iMoveToFlag))
	{
		//可以移动时,修改
		pTank->m_iLastMoveTime = g_iCurrentTime;
		//清除原坦克位置的图
		pTank->ClearMyself();
		//根据方向移动
		switch (iMoveToFlag)
		{
		case LEFT_DIRECTION:
			pTank->MoveToLeft();
			break;
		case TOP_DIRECTION:
			pTank->MoveToTop();
			break;
		case RIGHT_DIRECTION:
			pTank->MoveToRight();
			break;
		case BOTTOM_DIRECTION:
			pTank->MoveToBottom();
			break;
		default:
			assert(0);
			break;
		}
		//绘制现在的坦克位置的图
		pTank->DrawMyself();
		return true;
	}
	else if (!CollisionCheckOfMyselfAfterAdjustingDirection(pTank, pMapp, iMoveToFlag))
	{
		//可以"方向"时,修改
		pTank->m_iLastMoveTime = g_iCurrentTime;

		//清除原坦克位置的图
		pTank->ClearMyself();
		//根据方向"方向"
		switch (iMoveToFlag)
		{
		case LEFT_DIRECTION:
			pTank->m_iDirection = LEFT_DIRECTION;
			break;
		case TOP_DIRECTION:
			pTank->m_iDirection = TOP_DIRECTION;
			break;
		case RIGHT_DIRECTION:
			pTank->m_iDirection = RIGHT_DIRECTION;
			break;
		case BOTTOM_DIRECTION:
			pTank->m_iDirection = BOTTOM_DIRECTION;
			break;
		default:
			assert(0);
			break;
		}
		//绘制现在的坦克位置的图
		pTank->DrawMyself();
		return true;
	}

	return false;
}

//***********************************************************
// 名称: CTankMoveManager::CollisionCheck
// 功能: Judge that whether or not can moving;
// 继承: 
// 返回: bool
// para: left-value
// para: right-value
// para: direction
//***********************************************************
bool CTankMoveManager::CollisionCheck(CTank* pTank, CMapp* pMapp, int iMoveToFlag)
{
	//逻辑是先创建左值副本,左上右下移动左值;先排除原左值,再与各对象比较;  这样的好处是不用先移动再判断,达到先判断再移动.
	CTank left_Tank = *pTank;	//CTank中没有指针,所以可以拷贝构造;
								//Judge the moving direction;
	switch (iMoveToFlag)
	{
	case LEFT_DIRECTION:
		left_Tank.MoveToLeft();
		break;
	case TOP_DIRECTION:
		left_Tank.MoveToTop();
		break;
	case RIGHT_DIRECTION:
		left_Tank.MoveToRight();
		break;
	case BOTTOM_DIRECTION:
		left_Tank.MoveToBottom();
		break;
	default:
		assert(0);
		break;
	}

	return am_TankCollisionCheckOfMyself(left_Tank, pMapp, pTank);
}
// //不能位移后,判断是否可以改变方向
bool CTankMoveManager::CollisionCheckOfMyselfAfterAdjustingDirection(CTank* pTank, CMapp* pMapp, int iMoveToFlag)
{
	//逻辑是先创建左值副本,左移左值;先排除原左值,再与各对象比较;  这样的好处是不用先移动再判断,达到先判断再移动.
	CTank left_Tank = *pTank;	//CTank中没有指针,所以可以拷贝构造;

	//如果同向,就相当于坦克不能移动,"方向",也就是说nothing happend,return false;
	if (iMoveToFlag == left_Tank.m_iDirection)
		return true;

	//Judge the moving direction;
	switch (iMoveToFlag)
	{
	case LEFT_DIRECTION:
		left_Tank.m_iDirection = LEFT_DIRECTION;
		break;
	case TOP_DIRECTION:
		left_Tank.m_iDirection = TOP_DIRECTION;
		break;
	case RIGHT_DIRECTION:
		left_Tank.m_iDirection = RIGHT_DIRECTION;
		break;
	case BOTTOM_DIRECTION:
		left_Tank.m_iDirection = BOTTOM_DIRECTION;
		break;
	default:
		assert(0);
		break;
	}

	return am_TankCollisionCheckOfMyself(left_Tank, pMapp, pTank);
}
// //确定好坦克数据后,直接做碰撞检测了
// left_Tank: 处理好的坦克副本;
// pTank: 原坦克的指针;主要用来排除自身;
bool CTankMoveManager::am_TankCollisionCheckOfMyself(CTank& left_Tank, CMapp* pMapp, CTank* pTank)
{
	//0. Outside; ***************
	//1. m_vecTank; **************
	//2. m_vecBullet; **************
	//3. m_vecBrick; **************
	//4. m_pKing; **************

	vector<CPoint> left_vecTankSixPointPos = left_Tank.FillTankSixPointPos();

	//0. Outside; ***********************************************************************
	//可以judge每一个点是否出界,也可以如下面实际:	
	int xMovedTank_xPos = left_Tank.m_xPos;
	int yMovedTank_yPos = left_Tank.m_yPos;
	if ((xMovedTank_xPos < LEFT_BULLET_WND + 1) || (xMovedTank_xPos > RIGHT_BULLET_WND - 1) ||
		(yMovedTank_yPos < TOP_BULLET_WND + 1) || (yMovedTank_yPos > BOTTOM_BULLET_WND - 1))
		return true;

	//1. m_vecTank; *************************************************************************
	for (int i = 0; i < 6; i++)
	{
		int left_xPos = left_vecTankSixPointPos.at(i).m_xPos;
		int left_yPos = left_vecTankSixPointPos.at(i).m_yPos;
		int iSize = pMapp->m_vecTank.size();
		for (int j = 0; j < iSize; j++)
		{
			//排除自身
			if (pTank == pMapp->m_vecTank.at(j))
				continue;

			vector<CPoint> right_vecTankSixPointPos = pMapp->m_vecTank.at(j)->FillTankSixPointPos();
			for (int k = 0; k < 6; k++)
			{
				int right_xPos = right_vecTankSixPointPos.at(k).m_xPos;
				int right_yPos = right_vecTankSixPointPos.at(k).m_yPos;
				if (left_xPos == right_xPos && left_yPos == right_yPos)
					return true;
			}
		}
	}
	//2. m_vecBullet; *************************************************************************
	for (int i = 0; i < 6; i++)
	{
		int left_xPos = left_vecTankSixPointPos.at(i).m_xPos;
		int left_yPos = left_vecTankSixPointPos.at(i).m_yPos;
		int iSize = pMapp->m_vecBullet.size();
		for (int j = 0; j < iSize; j++)
		{
			int right_xPos = pMapp->m_vecBullet.at(j)->m_xPos;
			int right_yPos = pMapp->m_vecBullet.at(j)->m_yPos;
			if (left_xPos == right_xPos && left_yPos == right_yPos)
				return true;
		}
	}
	//3. m_vecBrick; *************************************************************************
	for (int i = 0; i < 6; i++)
	{
		int left_xPos = left_vecTankSixPointPos.at(i).m_xPos;
		int left_yPos = left_vecTankSixPointPos.at(i).m_yPos;
		int iSize = pMapp->m_vecBrick.size();
		for (int j = 0; j < iSize; j++)
		{
			int right_xPos = pMapp->m_vecBrick.at(j)->m_xPos;
			int right_yPos = pMapp->m_vecBrick.at(j)->m_yPos;
			if (left_xPos == right_xPos && left_yPos == right_yPos)
				return true;
		}
	}
	//4. m_pKing; *************************************************************************
	for (int i = 0; i < 6; i++)
	{
		int left_xPos = left_vecTankSixPointPos.at(i).m_xPos;
		int left_yPos = left_vecTankSixPointPos.at(i).m_yPos;
		int right_xPos = pMapp->m_pKing->m_xPos;
		int right_yPos = pMapp->m_pKing->m_yPos;
		if (left_xPos == right_xPos && left_yPos == right_yPos)
			return true;
	}

	return false;
}


void CTankMoveManager::MoveAutomatic(CMapp* pMapp)
{
	srand((UINT)time(NULL));

	int iSize = (int)pMapp->m_vecTank.size();
	for (int i = 0; i < iSize; i++)
	{
		CTank* pTank = pMapp->m_vecTank.at(i);
		//排除友方
		if (pMapp->m_vecTank.at(i)->m_iIDFlag == MYSELF_WHITE_TANK ||
			pMapp->m_vecTank.at(i)->m_iIDFlag == FRIEND_WITHE_TANK)
			continue;
		
		//每个坦克,只有移动为真时,才退出,也就是说:碰撞之后,找到了能移动的方向,或能"方向"的方向为真(如果方向不为自己的方向,就为真);
		//int iDirection = pTank->m_iDirection;
		if (Move(pTank, pMapp, pTank->m_iDirection))
			continue;

		for (int i = 0; i < 3; i++)
		{
			int iDirection = rand() % 4;
			if (Move(pTank, pMapp, iDirection))
				break;
		}
	}		
	Sleep(50);
	
}



//bool CTankMoveManager::MoveToLeft(CTank* pTank, CMapp* pMapp)
//{
//	//如果没有碰撞,清除原图,绘制移动后的图;否则,改变方向后是否碰撞;
//	if (!CollisionCheck(pTank, pMapp, LEFT_DIRECTION))
//	{
//		pTank->ClearMyself();
//		pTank->MoveToLeft();
//		pTank->DrawMyself();
//		return true;
//	}
//	else if (!CollisionCheckOfMyselfAfterAdjustingDirection(pTank, pMapp, LEFT_DIRECTION))
//	{
//		pTank->ClearMyself();
//		pTank->m_iDirection = LEFT_DIRECTION;
//		pTank->DrawMyself();
//	}
//
//	return false;
//}
//bool CTankMoveManager::MoveToTop(CTank* pTank, CMapp* pMapp)
//{
//	//如果没有碰撞,清除原图,绘制移动后的图;否则,清除原图,绘制现在方向的图
//	if (!CollisionCheck(pTank, pMapp, TOP_DIRECTION))
//	{
//		pTank->ClearMyself();
//		pTank->MoveToTop();
//		pTank->DrawMyself();
//		return true;
//	}
//	else if (!CollisionCheckOfMyselfAfterAdjustingDirection(pTank, pMapp, TOP_DIRECTION))
//	{
//		pTank->ClearMyself();
//		pTank->m_iDirection = TOP_DIRECTION;
//		pTank->DrawMyself();
//	}
//
//	return false;
//}
//bool CTankMoveManager::MoveToRight(CTank* pTank, CMapp* pMapp)
//{
//	//如果没有碰撞,清除原图,绘制移动后的图;否则,清除原图,绘制现在方向的图
//	if (!CollisionCheck(pTank, pMapp, RIGHT_DIRECTION))
//	{
//		pTank->ClearMyself();
//		pTank->MoveToRight();
//		pTank->DrawMyself();
//		return true;
//	}
//	else if (!CollisionCheckOfMyselfAfterAdjustingDirection(pTank, pMapp, RIGHT_DIRECTION))
//	{
//		pTank->ClearMyself();
//		pTank->m_iDirection = RIGHT_DIRECTION;
//		pTank->DrawMyself();
//	}
//
//	return false;
//}
//bool CTankMoveManager::MoveToBottom(CTank* pTank, CMapp* pMapp)
//{
//	//如果没有碰撞,清除原图,绘制移动后的图;否则,清除原图,绘制现在方向的图
//	if (!CollisionCheck(pTank, pMapp, BOTTOM_DIRECTION))
//	{
//		pTank->ClearMyself();
//		pTank->MoveToBottom();
//		pTank->DrawMyself();
//		return true;
//	}
//	else if (!CollisionCheckOfMyselfAfterAdjustingDirection(pTank, pMapp, BOTTOM_DIRECTION))
//	{
//		pTank->ClearMyself();
//		pTank->m_iDirection = BOTTOM_DIRECTION;
//		pTank->DrawMyself();
//	}
//
//	return false;
//}
