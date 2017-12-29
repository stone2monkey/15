#include "stdafx.h"
#include "BulletMoveManager.h"


CBulletMoveManager::CBulletMoveManager()
{
}


CBulletMoveManager::~CBulletMoveManager()
{
}

/************************************************************************************
1. 在子弹被创建的时候,需要碰撞判断
2. 在子弹移动后,需要碰撞判断
*/
void CBulletMoveManager::CollisionCheck(CBullet * left_pBullet, CMapp * pMapp)
{
	switch (left_pBullet->m_iIDFlag)
	{
	case MYSELF_WHITE_BULLET:
	case FRIEND_WITHE_BULLET:
		CollisionCheckOfMyself(left_pBullet, pMapp);
		break;
	case ENEMY_RED_BULLET:
	case ENEMY_YELLOW_BULLET:
	case ENEMY_GREEN_BULLET:
		CollisionCheckOfEnemy(left_pBullet, pMapp);
		break;
	default:
		assert(0);
		break;
	}
}

/************************************************************************************
1. 碰撞,不可见,	没死亡;
2. 碰撞,不可见,	死亡;
3. 未碰,可见,	没死亡;
-------------------------------------------------------------------------------------
// 1.Outside;
// 2.m_vecTank;
// 3.m_vecBullet;
// 4.m_vecBrick;
// 5.m_pKing;
// 6.no collision;
*/
void CBulletMoveManager::CollisionCheckOfMyself(CBullet * left_pBullet, CMapp * pMapp)
{
	int left_xPos = left_pBullet->m_xPos;
	int left_yPos = left_pBullet->m_yPos;

	// 0.当前位置是否有河流; *************************************
	for (int i = 0; i < (int)pMapp->m_vecBrick.size(); i++)
	{
		CBrick* right_pBrick = pMapp->m_vecBrick.at(i);
		int right_xPos = right_pBrick->m_xPos;
		int right_yPos = right_pBrick->m_yPos;
		if (left_xPos == right_xPos && left_yPos == right_yPos)		//条件成立就相碰撞了
		{
			left_pBullet->m_oCollisionInfo.m_bRiver = false;
			if (REVER_BLUE_BRICK == right_pBrick->m_iIDFlag)
			{
				left_pBullet->m_oCollisionInfo.m_bRiver = true;
				break;
			}
		}
	}

	// 1.Outside; ************************************************
	if ((left_xPos < LEFT_BULLET_WND) || (left_xPos > RIGHT_BULLET_WND) || (left_yPos < TOP_BULLET_WND) || (left_yPos > BOTTOM_BULLET_WND))
	{
		left_pBullet->m_oCollisionInfo.m_iCollisionCode = COLLISIONCODE_OUTSIDE;
		left_pBullet->m_oCollisionInfo.m_pBase = nullptr;
		left_pBullet->m_oCollisionInfo.m_bLeftVisible = false;
		left_pBullet->m_oCollisionInfo.m_bLeftDie = true;
		left_pBullet->m_oCollisionInfo.m_bRightVisible = false;
		left_pBullet->m_oCollisionInfo.m_bRightDie = false;
		return;
	}

	// 2.m_vecTank; ************************************************
	for (int i = 0; i < (int)pMapp->m_vecTank.size(); i++)
	{
		CTank* right_pTank = pMapp->m_vecTank.at(i);
		for (int j = 0; j < 6; j++)
		{
			right_pTank->FillTankSixPointPos();
			int right_xPos = right_pTank->m_vecTankSixPointPos.at(j).m_xPos;
			int right_yPos = right_pTank->m_vecTankSixPointPos.at(j).m_yPos;
			if (left_xPos == right_xPos && left_yPos == right_yPos)		//条件成立就相碰撞了
			{
				switch (right_pTank->m_iIDFlag)
				{
				case MYSELF_WHITE_TANK:
				case FRIEND_WITHE_TANK:
					left_pBullet->m_oCollisionInfo.m_iCollisionCode = MYSELF_WHITE_TANK;
					left_pBullet->m_oCollisionInfo.m_pBase = right_pTank;
					left_pBullet->m_oCollisionInfo.m_bLeftVisible = false;
					left_pBullet->m_oCollisionInfo.m_bLeftDie = false;
					left_pBullet->m_oCollisionInfo.m_bRightVisible = true;
					left_pBullet->m_oCollisionInfo.m_bRightDie = false;
					return;
					break;
				case ENEMY_RED_TANK:
				case ENEMY_YELLOW_TANK:
				case ENEMY_GREEN_TANK:
					left_pBullet->m_oCollisionInfo.m_iCollisionCode = ENEMY_RED_TANK;
					left_pBullet->m_oCollisionInfo.m_pBase = right_pTank;
					left_pBullet->m_oCollisionInfo.m_bLeftVisible = false;
					left_pBullet->m_oCollisionInfo.m_bLeftDie = true;
					left_pBullet->m_oCollisionInfo.m_bRightVisible = true;
					left_pBullet->m_oCollisionInfo.m_bRightDie = false;
					return;
					break;
				default:
					assert(0);
					break;
				}
			}
		}
	}

	// 3.m_vecBullet; ************************************************
	for (int i = 0; i < (int)pMapp->m_vecBullet.size(); i++)
	{
		CBullet* right_pBullet = pMapp->m_vecBullet.at(i);
		//排除自身;
		if (left_pBullet == right_pBullet)
			continue;

		int right_xPos = right_pBullet->m_xPos;
		int right_yPos = right_pBullet->m_yPos;
		if (left_xPos == right_xPos && left_yPos == right_yPos)		//条件成立就相碰撞了
		{
			switch (right_pBullet->m_iIDFlag)
			{
			case MYSELF_WHITE_BULLET:
			case FRIEND_WITHE_BULLET:
				left_pBullet->m_oCollisionInfo.m_iCollisionCode = MYSELF_WHITE_BULLET;
				left_pBullet->m_oCollisionInfo.m_pBase = right_pBullet;
				if(left_pBullet->m_oCollisionInfo.m_bRiver)
					left_pBullet->m_oCollisionInfo.m_bLeftVisible = false;
				else
					left_pBullet->m_oCollisionInfo.m_bLeftVisible = true;
				left_pBullet->m_oCollisionInfo.m_bLeftDie = false;
				left_pBullet->m_oCollisionInfo.m_bRightVisible = false;
				left_pBullet->m_oCollisionInfo.m_bRightDie = false;
				return;
				break;
			case ENEMY_RED_BULLET:
			case ENEMY_YELLOW_BULLET:
			case ENEMY_GREEN_BULLET:
				left_pBullet->m_oCollisionInfo.m_iCollisionCode = ENEMY_RED_BULLET;
				left_pBullet->m_oCollisionInfo.m_pBase = right_pBullet;
				left_pBullet->m_oCollisionInfo.m_bLeftVisible = false;
				left_pBullet->m_oCollisionInfo.m_bLeftDie = true;
				left_pBullet->m_oCollisionInfo.m_bRightVisible = false;
				left_pBullet->m_oCollisionInfo.m_bRightDie = true;
				return;
				break;
			default:
				assert(0);
				break;
			}
		}
	}

	// 4.m_vecBrick; ************************************************
	for (int i = 0; i < (int)pMapp->m_vecBrick.size(); i++)
	{
		CBrick* right_pBrick = pMapp->m_vecBrick.at(i);
		int right_xPos = right_pBrick->m_xPos;
		int right_yPos = right_pBrick->m_yPos;
		if (left_xPos == right_xPos && left_yPos == right_yPos)		//条件成立就相碰撞了
		{
			switch (right_pBrick->m_iIDFlag)
			{
			case STEEL_WHITE_BRICK:
				left_pBullet->m_oCollisionInfo.m_iCollisionCode = STEEL_WHITE_BRICK;
				left_pBullet->m_oCollisionInfo.m_pBase = right_pBrick;
				left_pBullet->m_oCollisionInfo.m_bLeftVisible = false;
				left_pBullet->m_oCollisionInfo.m_bLeftDie = true;
				left_pBullet->m_oCollisionInfo.m_bRightVisible = true;
				left_pBullet->m_oCollisionInfo.m_bRightDie = false;
				return;
				break;
			case REVER_BLUE_BRICK:
				left_pBullet->m_oCollisionInfo.m_iCollisionCode = REVER_BLUE_BRICK;
				left_pBullet->m_oCollisionInfo.m_pBase = right_pBrick;
				left_pBullet->m_oCollisionInfo.m_bLeftVisible = false;
				left_pBullet->m_oCollisionInfo.m_bLeftDie = false;
				left_pBullet->m_oCollisionInfo.m_bRightVisible = true;
				left_pBullet->m_oCollisionInfo.m_bRightDie = false;
				return;
				break;
			case NORMAL_PURPLE_BRICK:
				left_pBullet->m_oCollisionInfo.m_iCollisionCode = NORMAL_PURPLE_BRICK;
				left_pBullet->m_oCollisionInfo.m_pBase = right_pBrick;
				left_pBullet->m_oCollisionInfo.m_bLeftVisible = false;
				left_pBullet->m_oCollisionInfo.m_bLeftDie = true;
				left_pBullet->m_oCollisionInfo.m_bRightVisible = true;
				left_pBullet->m_oCollisionInfo.m_bRightDie = false;
				return;
				break;
			default:
				assert(0);
				break;
			}
		}
	}

	// 5.m_pKing; ************************************************
	CKing* right_pKing = pMapp->m_pKing;
	int right_xPos = right_pKing->m_xPos;
	int right_yPos = right_pKing->m_yPos;
	if (left_xPos == right_xPos && left_yPos == right_yPos)		//条件成立就相碰撞了
	{
		left_pBullet->m_oCollisionInfo.m_iCollisionCode = THE_RED_KING;
		left_pBullet->m_oCollisionInfo.m_pBase = right_pKing;
		left_pBullet->m_oCollisionInfo.m_bLeftVisible = false;
		left_pBullet->m_oCollisionInfo.m_bLeftDie = true;
		left_pBullet->m_oCollisionInfo.m_bRightVisible = true;
		left_pBullet->m_oCollisionInfo.m_bRightDie = false;
		return;
	}

	// 6.no collision; ************************************************
	left_pBullet->m_oCollisionInfo.m_iCollisionCode = COLLISIONCODE_NOCOLLISION;
	left_pBullet->m_oCollisionInfo.m_pBase = nullptr;
	left_pBullet->m_oCollisionInfo.m_bLeftVisible = true;
	left_pBullet->m_oCollisionInfo.m_bLeftDie = false;
	left_pBullet->m_oCollisionInfo.m_bRightVisible = true;
	left_pBullet->m_oCollisionInfo.m_bRightDie = false;
}

/************************************************************************************
1. 碰撞,不可见,	没死亡;
2. 碰撞,不可见,	死亡;
3. 未碰,可见,	没死亡;
-------------------------------------------------------------------------------------
// 1.Outside;
// 2.m_vecTank;
// 3.m_vecBullet;
// 4.m_vecBrick;
// 5.m_pKing;
// 6.no collision;
*/
void CBulletMoveManager::CollisionCheckOfEnemy(CBullet * left_pBullet, CMapp * pMapp)
{
	int left_xPos = left_pBullet->m_xPos;
	int left_yPos = left_pBullet->m_yPos;

	// 0.当前位置是否有河流; *************************************
	for (int i = 0; i < (int)pMapp->m_vecBrick.size(); i++)
	{
		CBrick* right_pBrick = pMapp->m_vecBrick.at(i);
		int right_xPos = right_pBrick->m_xPos;
		int right_yPos = right_pBrick->m_yPos;
		if (left_xPos == right_xPos && left_yPos == right_yPos)		//条件成立就相碰撞了
		{
			left_pBullet->m_oCollisionInfo.m_bRiver = false;
			if (REVER_BLUE_BRICK == right_pBrick->m_iIDFlag)
			{
				left_pBullet->m_oCollisionInfo.m_bRiver = true;
				break;
			}
		}
	}

	// 1.Outside; ************************************************
	if ((left_xPos < LEFT_BULLET_WND) || (left_xPos > RIGHT_BULLET_WND) || (left_yPos < TOP_BULLET_WND) || (left_yPos > BOTTOM_BULLET_WND))
	{
		left_pBullet->m_oCollisionInfo.m_iCollisionCode = COLLISIONCODE_OUTSIDE;
		left_pBullet->m_oCollisionInfo.m_pBase = nullptr;
		left_pBullet->m_oCollisionInfo.m_bLeftVisible = false;
		left_pBullet->m_oCollisionInfo.m_bLeftDie = true;
		left_pBullet->m_oCollisionInfo.m_bRightVisible = false;
		left_pBullet->m_oCollisionInfo.m_bRightDie = false;
		return;
	}

	// 2.m_vecTank; ************************************************
	for (int i = 0; i < (int)pMapp->m_vecTank.size(); i++)
	{
		CTank* right_pTank = pMapp->m_vecTank.at(i);
		for (int j = 0; j < 6; j++)
		{
			right_pTank->FillTankSixPointPos();
			int right_xPos = right_pTank->m_vecTankSixPointPos.at(j).m_xPos;
			int right_yPos = right_pTank->m_vecTankSixPointPos.at(j).m_yPos;
			if (left_xPos == right_xPos && left_yPos == right_yPos)		//条件成立就相碰撞了
			{
				switch (right_pTank->m_iIDFlag)
				{
				case MYSELF_WHITE_TANK:
				case FRIEND_WITHE_TANK:
					left_pBullet->m_oCollisionInfo.m_iCollisionCode = MYSELF_WHITE_TANK;
					left_pBullet->m_oCollisionInfo.m_pBase = right_pTank;
					left_pBullet->m_oCollisionInfo.m_bLeftVisible = false;
					left_pBullet->m_oCollisionInfo.m_bLeftDie = true;
					left_pBullet->m_oCollisionInfo.m_bRightVisible = true;
					left_pBullet->m_oCollisionInfo.m_bRightDie = false;
					return;
					break;
				case ENEMY_RED_TANK:
				case ENEMY_YELLOW_TANK:
				case ENEMY_GREEN_TANK:
					left_pBullet->m_oCollisionInfo.m_iCollisionCode = ENEMY_RED_TANK;
					left_pBullet->m_oCollisionInfo.m_pBase = right_pTank;
					left_pBullet->m_oCollisionInfo.m_bLeftVisible = false;
					left_pBullet->m_oCollisionInfo.m_bLeftDie = false;
					left_pBullet->m_oCollisionInfo.m_bRightVisible = true;
					left_pBullet->m_oCollisionInfo.m_bRightDie = false;
					return;
					break;
				default:
					assert(0);
					break;
				}
			}
		}
	}

	// 3.m_vecBullet; ************************************************
	for (int i = 0; i < (int)pMapp->m_vecBullet.size(); i++)
	{
		CBullet* right_pBullet = pMapp->m_vecBullet.at(i);
		//排除自身;
		if (left_pBullet == right_pBullet)
			continue;

		int right_xPos = right_pBullet->m_xPos;
		int right_yPos = right_pBullet->m_yPos;
		if (left_xPos == right_xPos && left_yPos == right_yPos)		//条件成立就相碰撞了
		{
			switch (right_pBullet->m_iIDFlag)
			{
			case MYSELF_WHITE_BULLET:
			case FRIEND_WITHE_BULLET:
				left_pBullet->m_oCollisionInfo.m_iCollisionCode = MYSELF_WHITE_BULLET;
				left_pBullet->m_oCollisionInfo.m_pBase = right_pBullet;
				left_pBullet->m_oCollisionInfo.m_bLeftVisible = false;
				left_pBullet->m_oCollisionInfo.m_bLeftDie = true;
				left_pBullet->m_oCollisionInfo.m_bRightVisible = false;
				left_pBullet->m_oCollisionInfo.m_bRightDie = true;
				return;
				break;
			case ENEMY_RED_BULLET:
			case ENEMY_YELLOW_BULLET:
			case ENEMY_GREEN_BULLET:
				left_pBullet->m_oCollisionInfo.m_iCollisionCode = ENEMY_RED_BULLET;
				left_pBullet->m_oCollisionInfo.m_pBase = right_pBullet;
				if (left_pBullet->m_oCollisionInfo.m_bRiver)
					left_pBullet->m_oCollisionInfo.m_bLeftVisible = false;
				else
					left_pBullet->m_oCollisionInfo.m_bLeftVisible = true;
				left_pBullet->m_oCollisionInfo.m_bLeftDie = false;
				left_pBullet->m_oCollisionInfo.m_bRightVisible = false;
				left_pBullet->m_oCollisionInfo.m_bRightDie = false;
				return;
				break;
			default:
				assert(0);
				break;
			}
		}
	}

	// 4.m_vecBrick; ************************************************
	for (int i = 0; i < (int)pMapp->m_vecBrick.size(); i++)
	{
		CBrick* right_pBrick = pMapp->m_vecBrick.at(i);
		int right_xPos = right_pBrick->m_xPos;
		int right_yPos = right_pBrick->m_yPos;
		if (left_xPos == right_xPos && left_yPos == right_yPos)		//条件成立就相碰撞了
		{
			switch (right_pBrick->m_iIDFlag)
			{
			case STEEL_WHITE_BRICK:
				left_pBullet->m_oCollisionInfo.m_iCollisionCode = STEEL_WHITE_BRICK;
				left_pBullet->m_oCollisionInfo.m_pBase = right_pBrick;
				left_pBullet->m_oCollisionInfo.m_bLeftVisible = false;
				left_pBullet->m_oCollisionInfo.m_bLeftDie = true;
				left_pBullet->m_oCollisionInfo.m_bRightVisible = true;
				left_pBullet->m_oCollisionInfo.m_bRightDie = false;
				return;
				break;
			case REVER_BLUE_BRICK:
				left_pBullet->m_oCollisionInfo.m_iCollisionCode = REVER_BLUE_BRICK;
				left_pBullet->m_oCollisionInfo.m_pBase = right_pBrick;
				left_pBullet->m_oCollisionInfo.m_bLeftVisible = false;
				left_pBullet->m_oCollisionInfo.m_bLeftDie = false;
				left_pBullet->m_oCollisionInfo.m_bRightVisible = true;
				left_pBullet->m_oCollisionInfo.m_bRightDie = false;
				return;
				break;
			case NORMAL_PURPLE_BRICK:
				left_pBullet->m_oCollisionInfo.m_iCollisionCode = NORMAL_PURPLE_BRICK;
				left_pBullet->m_oCollisionInfo.m_pBase = right_pBrick;
				left_pBullet->m_oCollisionInfo.m_bLeftVisible = false;
				left_pBullet->m_oCollisionInfo.m_bLeftDie = true;
				left_pBullet->m_oCollisionInfo.m_bRightVisible = true;
				left_pBullet->m_oCollisionInfo.m_bRightDie = false;
				return;
				break;
			default:
				assert(0);
				break;
			}
		}
	}

	// 5.m_pKing; ************************************************
	CKing* right_pKing = pMapp->m_pKing;
	int right_xPos = right_pKing->m_xPos;
	int right_yPos = right_pKing->m_yPos;
	if (left_xPos == right_xPos && left_yPos == right_yPos)		//条件成立就相碰撞了
	{
		left_pBullet->m_oCollisionInfo.m_iCollisionCode = THE_RED_KING;
		left_pBullet->m_oCollisionInfo.m_pBase = right_pKing;
		left_pBullet->m_oCollisionInfo.m_bLeftVisible = false;
		left_pBullet->m_oCollisionInfo.m_bLeftDie = true;
		left_pBullet->m_oCollisionInfo.m_bRightVisible = true;
		left_pBullet->m_oCollisionInfo.m_bRightDie = false;
		return;
	}

	// 6.no collision; ************************************************
	left_pBullet->m_oCollisionInfo.m_iCollisionCode = COLLISIONCODE_NOCOLLISION;
	left_pBullet->m_oCollisionInfo.m_pBase = nullptr;
	left_pBullet->m_oCollisionInfo.m_bLeftVisible = true;
	left_pBullet->m_oCollisionInfo.m_bLeftDie = false;
	left_pBullet->m_oCollisionInfo.m_bRightVisible = true;
	left_pBullet->m_oCollisionInfo.m_bRightDie = false;
}

void CBulletMoveManager::RespondCollision(CBullet * left_pBullet, CMapp * pMapp)
{
	//碰撞逻辑判断,碰撞发起者得到了m_oCollisionInfo结构体;
	CollisionCheck(left_pBullet, pMapp);

	//做出相应的响应
	switch (left_pBullet->m_iIDFlag)
	{
	case MYSELF_WHITE_BULLET:
	case FRIEND_WITHE_BULLET:
		return RespondCollisionOfMyself(left_pBullet, pMapp);
		break;
	case ENEMY_RED_BULLET:
	case ENEMY_YELLOW_BULLET:
	case ENEMY_GREEN_BULLET:
		return RespondCollisionOfEnemy(left_pBullet, pMapp);
		break;
	default:
		assert(0);
		break;
	}
}


/******************************************
可见	死亡	改变右操作数
******************************************/
void CBulletMoveManager::RespondCollisionOfMyself(CBullet * left_pBullet, CMapp * pMapp)
{
	switch (left_pBullet->m_oCollisionInfo.m_iCollisionCode)
	{
	case MYSELF_WHITE_TANK:
		break;
	case ENEMY_RED_TANK:
		//敌坦减血
		left_pBullet->m_oCollisionInfo.m_pBase->m_iBlood -= left_pBullet->m_iForceValue;
		am_DelAndEraseCollisioner(left_pBullet, pMapp);
		break;
	case MYSELF_WHITE_BULLET:
		if (!left_pBullet->m_oCollisionInfo.m_bRiver)
			left_pBullet->DrawMyself();
		break;
	case ENEMY_RED_BULLET:
		//二颗子弹都要死亡 还要去掉被碰撞的显示
	{
		CBullet* right_pBullet = (CBullet*)(left_pBullet->m_oCollisionInfo.m_pBase);
		//if (right_pBullet->m_oCollisionInfo.m_bLeftVisible)
		//	right_pBullet->ClearMyself();
		am_DelAndEraseCollisioner(right_pBullet, pMapp);
		am_DelAndEraseCollisioner(left_pBullet, pMapp);
	}
		break;
	case STEEL_WHITE_BRICK:
		am_DelAndEraseCollisioner(left_pBullet, pMapp);
		break;
	case REVER_BLUE_BRICK:
		break;
	case NORMAL_PURPLE_BRICK:
		//普通方块减血
		left_pBullet->m_oCollisionInfo.m_pBase->m_iBlood -= left_pBullet->m_iForceValue;
		am_DelAndEraseCollisioner(left_pBullet, pMapp);
		break;
	case THE_RED_KING:
		//国王减血
		left_pBullet->m_oCollisionInfo.m_pBase->m_iBlood -= left_pBullet->m_iForceValue;
		am_DelAndEraseCollisioner(left_pBullet, pMapp);
		break;
	case COLLISIONCODE_OUTSIDE:
		am_DelAndEraseCollisioner(left_pBullet, pMapp);
		break;
	case COLLISIONCODE_NOCOLLISION:
		left_pBullet->DrawMyself();
		break;
	default:
		assert(0);
		break;
	}
}

/******************************************
可见	死亡	改变右操作数
******************************************/
void CBulletMoveManager::RespondCollisionOfEnemy(CBullet * left_pBullet, CMapp * pMapp)
{
	switch (left_pBullet->m_oCollisionInfo.m_iCollisionCode)
	{
	case MYSELF_WHITE_TANK:
		//友坦减血
		left_pBullet->m_oCollisionInfo.m_pBase->m_iBlood -= left_pBullet->m_iForceValue;
		am_DelAndEraseCollisioner(left_pBullet, pMapp);
		break;
	case ENEMY_RED_TANK:
		break;
	case MYSELF_WHITE_BULLET:
		//二颗子弹都要死亡
	{
		CBullet* right_pBullet = (CBullet*)(left_pBullet->m_oCollisionInfo.m_pBase);
		//if (right_pBullet->m_oCollisionInfo.m_bLeftVisible)
		//	right_pBullet->ClearMyself();
		am_DelAndEraseCollisioner(right_pBullet, pMapp);
		am_DelAndEraseCollisioner(left_pBullet, pMapp);
	}
	break;
	case ENEMY_RED_BULLET:
	{
		if (!left_pBullet->m_oCollisionInfo.m_bRiver)
			left_pBullet->DrawMyself();
	}
		break;
	case STEEL_WHITE_BRICK:
		am_DelAndEraseCollisioner(left_pBullet, pMapp);
		break;
	case REVER_BLUE_BRICK:
		break;
	case NORMAL_PURPLE_BRICK:
		//普通方块减血
		left_pBullet->m_oCollisionInfo.m_pBase->m_iBlood -= left_pBullet->m_iForceValue;
		am_DelAndEraseCollisioner(left_pBullet, pMapp);
		break;
	case THE_RED_KING:
		//国王减血
		left_pBullet->m_oCollisionInfo.m_pBase->m_iBlood -= left_pBullet->m_iForceValue;
		am_DelAndEraseCollisioner(left_pBullet, pMapp);
		break;
	case COLLISIONCODE_OUTSIDE:
		am_DelAndEraseCollisioner(left_pBullet, pMapp);
		break;
	case COLLISIONCODE_NOCOLLISION:
		left_pBullet->DrawMyself();
		break;
	default:
		assert(0);
		break;
	}
}


void CBulletMoveManager::Move(CBullet * left_pBullet, CMapp * pMapp)
{
	//如果在这里做,每次子弹判断都要清除所有子弹,不好,放在外面吧;
	////不显示所有子弹
	//int iSize = (int)pMapp->m_vecBullet.size();
	//for (int i = 0; i < iSize; i++)
	//{
	//	CBullet* pBullet = pMapp->m_vecBullet.at(i);
	//	WriteChar(pBullet->m_xPos, pBullet->m_yPos, "  ", pBullet->m_iColor);
	//}

	//移动子弹
	switch (left_pBullet->m_iDirection)
	{
	case LEFT_DIRECTION:
		left_pBullet->MoveToLeft();
		break;
	case TOP_DIRECTION:
		left_pBullet->MoveToTop();
		break;
	case RIGHT_DIRECTION:
		left_pBullet->MoveToRight();
		break;
	case BOTTOM_DIRECTION:
		left_pBullet->MoveToBottom();
		break;
	default:
		assert(0);
		break;
	}

	//Judge Respond
	RespondCollision(left_pBullet, pMapp);
}

void CBulletMoveManager::MoveBulletAutomatic(CMapp* pMapp)
{
	//所有子弹不显示自己
	for (int i = 0; i < (int)pMapp->m_vecBullet.size(); i++)
	{
		CBullet* pBullet = pMapp->m_vecBullet.at(i);
		if (pBullet->m_oCollisionInfo.m_bLeftVisible)
			WriteChar(pBullet->m_xPos, pBullet->m_yPos, "  ", pBullet->m_iColor);
	}

	//所有子弹移动
	for (int i = 0; i < (int)pMapp->m_vecBullet.size(); i++)
	{
		CBullet* pBullet = pMapp->m_vecBullet.at(i);
		Move(pBullet, pMapp);
	}

	//清除所有血量小于等于0的对象
	ClearNoBloodElement(pMapp);
}

// 敌坦自动发射子弹
bool CBulletMoveManager::ShotBulletAutomaticForEnemy(CMapp* pMapp)
{
	int iSize = (int)pMapp->m_vecTank.size();
	for (int i = 0; i < iSize; i++)
	{
		CTank* pTank = pMapp->m_vecTank.at(i);
		//跳过友坦
		if (pTank->m_iIDFlag == MYSELF_WHITE_TANK || pTank->m_iIDFlag == FRIEND_WITHE_TANK)
			continue;
		//发射 
		//得到当前时间
		g_iCurrentTime = ::GetTickCount();
		//创建子弹对象;
		CBullet* pBullet = pTank->ShotBulletPro(g_iCurrentTime);
		//当子弹对象创建成功时,判断是否有效,可显;
		if (pBullet != nullptr)
		{
			pMapp->m_vecBullet.push_back(pBullet);
			RespondCollision(pBullet, pMapp);
			return true;
		}
	}

	return false;
}


bool CBulletMoveManager::am_DrawLeftCollisioner(CBullet* left_pBullet)
{
	if (left_pBullet->m_oCollisionInfo.m_bLeftVisible)
	{
		left_pBullet->DrawMyself();
		return true;
	}
	return false;
}


void CBulletMoveManager::am_DelAndEraseCollisioner(CBullet* left_pBullet, CMapp* pMapp)
{
	delete left_pBullet;
	int iCurrentBulletPosInm_vecBullet = pMapp->GetCurrentBulletPosAtVec(left_pBullet);
	if (ERROR_RETURN_VALUE == iCurrentBulletPosInm_vecBullet)
		assert(0);
	pMapp->m_vecBullet.erase(pMapp->m_vecBullet.begin() + iCurrentBulletPosInm_vecBullet);
	left_pBullet = nullptr;
}

// 清除所有血值小于等于0的对象
void CBulletMoveManager::ClearNoBloodElement(CMapp* pMapp)
{
	//m_vecTank
	for (int i = 0; i < (int)pMapp->m_vecTank.size(); i++)
	{
		CTank* pTank = pMapp->m_vecTank.at(i);
		if (pTank->m_iBlood <= 0)
		{
			//清除显示
			pTank->ClearMyself();
			//释放
			delete pTank;
			//vector erase
			pMapp->m_vecTank.erase(pMapp->m_vecTank.begin() + i);

			//防止因为erase遗漏
			i--;
		}
	}
	//m_vecBrick
	for (int i = 0; i < (int)pMapp->m_vecBrick.size(); i++)
	{
		CBrick* pBrick = pMapp->m_vecBrick.at(i);
		if (pBrick->m_iBlood <= 0)
		{
			//清除显示
			pBrick->ClearMyself();
			//释放
			delete pBrick;
			//vector erase
			pMapp->m_vecBrick.erase(pMapp->m_vecBrick.begin() + i);

			//防止因为erase遗漏
			i--;
		}
	}
	//m_pKing
	//这里要Gameover了
	if (pMapp->m_pKing <= 0)
	{
		system("cls");
		WriteChar(6, 10, "Game over!", RED);
	}
}
