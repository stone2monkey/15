#include "stdafx.h"
#include "BulletMng.h"
#include "Mapp.h"


CBulletMng::CBulletMng(CGameData& rGameData) : m_rGameData(rGameData)
{
}


CBulletMng::~CBulletMng()
{
}

void CBulletMng::Move(CBullet * pSelf, CMapp & rMapp)
{
	//移动时间间隔判断
	if (::GetTickCount() - pSelf->m_uLastMoveTime < pSelf->m_uMoveSpeed)
		return;

	// 0. validity of params
	// 1. 移动之前要不要做碰撞判断,可以设计,我这里是不做,也就是应该子弹碰撞判断的时候,在该做的时候做了
	// 不能改变被碰撞对象的数据 -xxx- 2. 移动之前拿到当前地图数组,用他去做碰撞判断(地图中的自身是不影响判断的,no care)
	// 3. 移动
	pSelf->Move(pSelf->m_iDirect);
	pSelf->m_uLastMoveTime = ::GetTickCount();

	// 4. 碰撞判断以及数据的变更
	CollidedJudgement(pSelf, rMapp);
}

void CBulletMng::CollidedJudgement(CBullet * pSelf, CMapp & rMapp)
{
	// 0. Outside
	int x = pSelf->m_xPos;
	int y = pSelf->m_yPos;
	if (x <LEFT_BULLET_WND || x >RIGHT_BULLET_WND || y <TOP_BULLET_WND || y>BOTTOM_BULLET_WND)
	{
		pSelf->m_iBlood = 0;
		return;
	}
	// 1. tank
	int iSize = (int)rMapp.m_vecTank.size();
	for (int i = 0; i < iSize ; i++)
	{
		CTank* pTank = rMapp.m_vecTank.at(i);
		pTank->FillSixPtArr();
		for (int j = 0; j < 6; j++)
		{
			int xTank = pTank->m_arrSixPt[j].m_x;
			int yTank = pTank->m_arrSixPt[j].m_y;

			if (x == xTank && y == yTank)				//如果碰撞
				am_CollidedWithTank(pSelf, pTank);
		}
	}

	// 2. bullet
	iSize = (int)rMapp.m_vecBullet.size();
	for (int i = 0; i < iSize; i++)
	{
		CBullet* pBullet = rMapp.m_vecBullet.at(i);
		int xBullet = pBullet->m_xPos;
		int yBullet = pBullet->m_yPos;

		if (x == xBullet && y == yBullet)				//如果碰撞
			am_CollidedWithBullet(pSelf, pBullet);
	}

	// 3. brick
	iSize = (int)rMapp.m_vecBrick.size();
	for (int i = 0; i < iSize; i++)
	{
		CBrick* pBrick = rMapp.m_vecBrick.at(i);
		int xBrick = pBrick->m_xPos;
		int yBrick = pBrick->m_yPos;

		if (x == xBrick && y == yBrick)					//如果碰撞
			am_CollidedWithBrick(pSelf, pBrick);		
	}

	// 4. king
	if (nullptr != rMapp.m_pKing)
	{
		int xKing = rMapp.m_pKing->m_xPos;
		int yKing = rMapp.m_pKing->m_yPos;

		if (x == xKing && y == yKing)					//如果碰撞
			am_CollidedWithKing(pSelf, rMapp.m_pKing);
	}
}

void CBulletMng::am_CollidedWithTank(CBullet * pSelf, CTank * pTank)
{
	if (ONEPLAYER_WHITE_BULLET == pSelf->m_iID || TWOPLAYER_WHITE_BULLET == pSelf->m_iID)	//友方子弹
	{
		switch (pTank->m_iID)
		{
			case ONEPLAYER_WHITE_TANK:	
			case TWOPLAYER_WHITE_TANK:
				return;
				break;
			case ENEMY_RED_TANK:
			case ENEMY_YELLOW_TANK:
			case ENEMY_GREEN_TANK:
			{
				// 敌坦减血
				// 自己死亡
				// 返回
				pTank->m_iBlood -= pSelf->m_iForce;
				pTank->m_iLevel -= pSelf->m_iForce;
				pSelf->m_iBlood = 0;
				return;
			}break;
			default:
				assert(0);
		}
	}
	else																					//敌方子弹
	{
		switch (pTank->m_iID)
		{
			case ONEPLAYER_WHITE_TANK:
			case TWOPLAYER_WHITE_TANK:
			{
				// 友坦减血
				// 自己死亡
				// 返回
				pTank->m_iBlood -= pSelf->m_iForce;
				pSelf->m_iBlood = 0;
				return;
			}break;
			case ENEMY_RED_TANK:
			case ENEMY_YELLOW_TANK:
			case ENEMY_GREEN_TANK:
				return;
				break;
			default:
				assert(0);
		}
	}

	assert(0);
}

void CBulletMng::am_CollidedWithBullet(CBullet * pSelf, CBullet * pBullet)
{
	if (ONEPLAYER_WHITE_BULLET == pSelf->m_iID || TWOPLAYER_WHITE_BULLET == pSelf->m_iID)	//友方子弹
	{
		//排除自身吧,其实不也没关系
		if (pSelf == pBullet)
			return;

		switch (pBullet->m_iID)
		{
			case ONEPLAYER_WHITE_BULLET:
			case TWOPLAYER_WHITE_BULLET:
				return;
				break;
			case ENEMY_RED_BULLET:
			case ENEMY_YELLOW_BULLET:
			case ENEMY_GREEN_BULLET:
			{
				// 敌方死亡
				// 自己死亡
				// 返回
				pBullet->m_iBlood = 0;
				pSelf->m_iBlood = 0;
				return;
			}break;
			default:
				assert(0);
		}
	}
	else																					//敌方子弹
	{
		//排除自身吧,其实不也没关系
		if (pSelf == pBullet)
			return;

		switch (pBullet->m_iID)
		{
			case ONEPLAYER_WHITE_BULLET:
			case TWOPLAYER_WHITE_BULLET:
			{
				// 友方死亡
				// 自己死亡
				// 返回
				pBullet->m_iBlood = 0;
				pSelf->m_iBlood = 0;
				return;
			}break;
			case ENEMY_RED_BULLET:
			case ENEMY_YELLOW_BULLET:
			case ENEMY_GREEN_BULLET:
				return;
				break;
			default:
				assert(0);
		}
	}

	assert(0);
}

void CBulletMng::am_CollidedWithBrick(CBullet * pSelf, CBrick * pBrick)
{
	if (ONEPLAYER_WHITE_BULLET == pSelf->m_iID && STEEL_WHITE_BRICK == pBrick->m_iID)	//OnePlay可以穿钢墙
		return;

	switch (pBrick->m_iID)
	{
		case STEEL_WHITE_BRICK:
			pSelf->m_iBlood = 0;	//自己死亡
			return;
			break;
		case REVER_BLUE_BRICK:
			return;
			break;
		case NORMAL_PURPLE_BRICK:
			pBrick->m_iBlood -= pSelf->m_iForce;	//普通方块减血
			pSelf->m_iBlood = 0;
			return;
			break;
		default:
			assert(0);
	}

	assert(0);
}

void CBulletMng::am_CollidedWithKing(CBullet * pSelf, CKing * pKing)
{
	if (nullptr != pKing)
	{
		pKing->m_iBlood -= pSelf->m_iForce;
		pSelf->m_iBlood = 0;
		return;
	}
	else
		return;

	assert(0);
}

//************************************
// Method:    RespondAfterMove
// FullName:  CBulletMng::RespondAfterMove
// Access:    public 
// Returns:   void
// Qualifier: 1. GameData对象的数据与0值地图对象的响应
// Parameter: CMapp & rMapp
//************************************
void CBulletMng::RespondAfterMove(CMapp& rMapp)
{
	// 改变m_rGameData
	int iSize = (int)rMapp.m_vecTank.size();
	for (int i = 0; i < iSize; i++)
	{
		CTank* pTank = rMapp.m_vecTank.at(i);
		if (0 >= pTank->m_iBlood)
		{
			switch (pTank->m_iID)
			{
				case ONEPLAYER_WHITE_TANK:
					pTank->m_iBlood += 10;
					break;
				case TWOPLAYER_WHITE_TANK:
					::PlaySound(_TEXT("Music\\TANK_DIETH_MUSIC.wav"), NULL, SND_FILENAME | SND_ASYNC);
					m_rGameData.m_uTankDieTime[1] = ::GetTickCount();
					m_rGameData.m_bCreateTank[1] = true;
					break;
				case ENEMY_RED_TANK:
					::PlaySound(_TEXT("Music\\TANK_DIETH_MUSIC.wav"), NULL, SND_FILENAME | SND_ASYNC);
					m_rGameData.m_iScore++;
					m_rGameData.m_uTankDieTime[2] = ::GetTickCount();
					m_rGameData.m_bCreateTank[2] = true;
					break;
				case ENEMY_YELLOW_TANK:
					::PlaySound(_TEXT("Music\\TANK_DIETH_MUSIC.wav"), NULL, SND_FILENAME | SND_ASYNC);
					m_rGameData.m_iScore++;
					m_rGameData.m_uTankDieTime[3] = ::GetTickCount();
					m_rGameData.m_bCreateTank[3] = true;
					break;
				case ENEMY_GREEN_TANK:
					::PlaySound(_TEXT("Music\\TANK_DIETH_MUSIC.wav"), NULL, SND_FILENAME | SND_ASYNC);
					m_rGameData.m_iScore++;
					m_rGameData.m_uTankDieTime[4] = ::GetTickCount();
					m_rGameData.m_bCreateTank[4] = true;
					break;
				default:
					assert(0);
			}
		}
	}

	if (0 >= rMapp.m_pKing->m_iBlood)		
		m_rGameData.m_bGameOver = true;

	rMapp.ClearWhoNoBlood();
}

void CBulletMng::MoveAutomatic(CMapp & rMapp)
{
	for (int i = 0; i < (int)rMapp.m_vecBullet.size(); i++)
	{
		CBullet* pBullet = rMapp.m_vecBullet.at(i);

		CollidedJudgement(pBullet, rMapp);		//子弹移动前做下碰撞判断
		RespondAfterMove(rMapp);

		Move(pBullet, rMapp);					//移动
		RespondAfterMove(rMapp);
	}
}
