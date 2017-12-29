#include "stdafx.h"
#include <ctime>
#include "TankMng.h"
#include "BulletMng.h"
#include "Mapp.h"

CTankMng::CTankMng(CGameData& rGameData) : m_rGameData(rGameData)
{
}


CTankMng::~CTankMng()
{
}

bool CTankMng::Move(CTank*& pTank, CMapp & rMapp, int iDirect)
{
	// move speed
	UINT uCurTime = ::GetTickCount();
	if (uCurTime - pTank->m_uLastMoveTime < pTank->m_uMoveSpeed)	//时间间隔不够
		return false;

	// 0. validity of params
	if (nullptr == pTank)
		assert(0);
	if (iDirect < LEFT_DIRECTION || iDirect > BOTTOM_DIRECTION)
		assert(0);

	// 1. 是否可以移动,就移动
	// 2. 如果不能移动,是否可以方向,就方向
	if (MoveOfPos(pTank, rMapp, iDirect))
	{
		pTank->Move(iDirect);
		UINT uCurTime = ::GetTickCount();
		pTank->m_uLastMoveTime = uCurTime;
		return true;
	}
	else if (MoveOfDirect(pTank, rMapp, iDirect))
	{
		pTank->JustChangeDirict(iDirect);
		UINT uCurTime = ::GetTickCount();
		pTank->m_uLastMoveTime = uCurTime;
		return true;
	}
	
	return false;
}

bool CTankMng::MoveOfPos(CTank*& pTank, CMapp & rMapp, int iDirect)
{
	// 0. 建立副本并移动
	CTank oCopy = *pTank;		//这里进行了拷贝构造,不过没有指针,是没有问题的浅拷贝
	CTank* pCopy = &oCopy;
	pCopy->Move(iDirect);		//移动

	// 1.碰撞判断
	if (CollidedJudgement(pCopy, rMapp, pTank))		//碰撞了
		return false;

	return true;
}

bool CTankMng::MoveOfDirect(CTank*& pTank, CMapp & rMapp, int iDirect)
{
	// 0. 建立副本并方向
	CTank oCopy = *pTank;		//这里进行了拷贝构造,不过没有指针,是没有问题的浅拷贝
	CTank* pCopy = &oCopy;
	if (!pCopy->JustChangeDirict(iDirect))		//方向不成功,(不能移动,还同方向,也就是什么没变)
		return false;

	// 1.碰撞判断
	if (CollidedJudgement(pCopy, rMapp, pTank))		//碰撞了
		return false;

	return true;
}

bool CTankMng::CollidedJudgement(CTank * pCopy, CMapp & rMapp, CTank*& pOriginal)
{
	// 0. Outside
	int xCopy = pCopy->m_xPos;
	int yCopy = pCopy->m_yPos;
	if (xCopy <LEFT_BULLET_WND + 1 || xCopy>RIGHT_BULLET_WND - 1 || yCopy<TOP_BULLET_WND + 1 || yCopy>BOTTOM_BULLET_WND - 1)	//出界了
		return true;

	// 1. Tank
	int iSize = (int)rMapp.m_vecTank.size();
	for (int i = 0; i < iSize ; i++)
	{
		CTank* pTank = rMapp.m_vecTank.at(i);

		if (pTank == pOriginal)		//self
			continue;

		if (am_CollidedWithTank(pCopy, pTank))		//碰撞了
			return true;
	}

	// 3. Brick
	iSize = (int)rMapp.m_vecBrick.size();
	for (int i = 0; i < iSize; i++)
	{
		CBrick* pBrick = rMapp.m_vecBrick.at(i);

		if (am_CollidedWithOther(pCopy, pBrick))		//碰撞了
			return true;
	}

	// 4. King 
	if (nullptr != rMapp.m_pKing)
	{
		if (am_CollidedWithOther(pCopy, rMapp.m_pKing))		//碰撞了
			return true;
	}

	// 2. Bullet 这里很有趣,不过太破坏程序可读性了(到了这一步就说明是可以移动的了)

	return false;
}

bool CTankMng::am_CollidedWithTank(CTank * pCopy, CTank * pOther)
{
	pCopy->FillSixPtArr();
	for (int i = 0; i < 6 ; i++)
	{
		int xCopy = pCopy->m_arrSixPt[i].m_x;
		int yCopy = pCopy->m_arrSixPt[i].m_y;

		pOther->FillSixPtArr();
		for (int j = 0; j < 6; j++)
		{
			int xOther = pOther->m_arrSixPt[j].m_x;
			int yOther = pOther->m_arrSixPt[j].m_y;

			if (xCopy == xOther && yCopy == yOther)			//碰撞了
				return true;
		}
	}

	return false;
}

bool CTankMng::am_CollidedWithOther(CTank * pCopy, CBase * pBase)
{
	for (int i = 0; i < 6; i++)
	{
		pCopy->FillSixPtArr();
		int xCopy = pCopy->m_arrSixPt[i].m_x;
		int yCopy = pCopy->m_arrSixPt[i].m_y;

		if (xCopy == pBase->m_xPos && yCopy == pBase->m_yPos)			//碰撞了
			return true;
	}

	return false;
}

bool CTankMng::MoveEnemyAutomatic(CMapp& rMapp)
{
	int iSize = (int)rMapp.m_vecTank.size();
	for (int i = 0; i < iSize ; i++)
	{
		CTank* pTank = rMapp.m_vecTank.at(i);

		if (pTank->m_iID == ONEPLAYER_WHITE_TANK || pTank->m_iID == TWOPLAYER_WHITE_TANK)	//友方坦克
			continue;

		if (Move(pTank, rMapp, pTank->m_iDirect))	//如果移动成功,进行下一个;
			break;

		//如果第一次不成功,给4次机会;
		srand((int)time(NULL));
		for (int j = 0; j < 4; j++)	
		{
			int iDirect = rand() % 4;

			if (Move(pTank, rMapp, iDirect))	
				return true;
		}
	}

	return false;
}

//************************************
// Method:    ShotBullet
// FullName:  CTankMng::ShotBullet
// Access:    public 
// Returns:   bool
// Qualifier:
// Parameter: CTank * pTank
// Parameter: CMapp & rMapp
// Parameter: UINT uDelay			这里是给子弹发射速度m_uShotSpeed加延时的,主要给敌坦加的
//************************************
bool CTankMng::ShotBullet(CTank * pTank, CMapp & rMapp, UINT uDelay)
{
	// 发射时间间隔 
	if (::GetTickCount() - pTank->m_uLastShotTime < pTank->m_uShotSpeed + uDelay)
		return false;

	// 创建子弹不成功,主要是tank类里排除出界
	CBullet* pBullet = pTank->ShotBullet();
	if (nullptr == pBullet)
		return false;

	// 压入子弹
	pTank->m_uLastShotTime = ::GetTickCount();
	rMapp.m_vecBullet.push_back(pBullet);

	// 碰撞判断 响应		
	CBulletMng o(m_rGameData);
	o.CollidedJudgement(pBullet, rMapp);
	o.RespondAfterMove(rMapp);

	return true;
}

void CTankMng::ShotEnemyAutomatic(CMapp & rMapp)
{
	int iSize = (int)rMapp.m_vecTank.size();
	for (int i = 0; i < iSize ; i++)
	{
		CTank* pTank = rMapp.m_vecTank.at(i);

		//排除友方
		if (pTank->m_iID == ONEPLAYER_WHITE_TANK || pTank->m_iID == TWOPLAYER_WHITE_TANK)
			continue;

		ShotBullet(pTank, rMapp, 3000);
	}

}

void CTankMng::CreatTank(int iID, CMapp & rMapp)
{
	int x = 0, y = 0;	//创建点
	int iIndex = 0;		// m_bCreateTank[iIndex]
	int iLevel = 0;		// m_iLevel

	// 通过ID确定 
	switch (iID)
	{
		case ONEPLAYER_WHITE_TANK:
			iIndex = 0;
			iLevel = 3;
			break;
		case TWOPLAYER_WHITE_TANK:
			x = 25;
			y = 18;
			iIndex = 1;
			iLevel = 1;
			break;
		case ENEMY_RED_TANK:
			x = 1;
			y = 1;
			iIndex = 2;
			iLevel = 1;
			break;
		case ENEMY_YELLOW_TANK:
			x = 15;
			y = 1;
			iIndex = 3;
			iLevel = 2;
			break;
		case ENEMY_GREEN_TANK:
			x = 30;
			y = 1;
			iIndex = 4;
			iLevel = 3;
			break;
		default:
			;
	}

	if (0 == m_rGameData.m_iTankCount[iIndex])			//坦克数量没有了
	{
		m_rGameData.m_bCreateTank[iIndex] = false;
		return;
	}
	if (false == m_rGameData.m_bCreateTank[iIndex])		//坦克创建标识为假
		return;
	{
		static UINT uCurTime = 0;	//得到当前时间
		uCurTime = ::GetTickCount();

		if (uCurTime - m_rGameData.m_uTankDieTime[iIndex] < m_rGameData.m_uCreateSpeed)	//如果死亡时间小于1秒,不能创建
			return;
	}


	// 假如创建点(x,y,下)非空,退出;但是创建属性还是真,所以下次还会判断;	
	{
		CTank* pTank = new CTank(x, y, iID, iLevel, 3);		//	这里用坦克吧,不用计算点 `~`

		rMapp.FillCurMappArr();							//填充当前地图数组

		int arr[9] = { 1, 0, 1, 1, 1, 1, 0, 1, 0 };		//得到6点
		pTank->am_FillSixPtArrViaArr(arr);
		for (int i = 0; i < 6; i++)					//6点与地图比较,当全为0时,说明坦克可以创建了,有地了	`~`
		{
			int x = pTank->m_arrSixPt[i].m_x;
			int y = pTank->m_arrSixPt[i].m_y;

			if (0 != rMapp.m_arrCurMapp[y][x])
			{
				delete pTank;
				pTank = nullptr;
				return;
			}
		}
		delete pTank;
		pTank = nullptr;
	}

	
	//创建 压入地图vector
	CTank* pTank = new CTank(x, y, iID, iLevel, 3);
	rMapp.m_vecTank.push_back(pTank);

	m_rGameData.m_iTankCount[iIndex]--;
	m_rGameData.m_bCreateTank[iIndex] = false;
}

void CTankMng::CreatTanks(CMapp & rMapp)
{
		for (int i = 0; i < 5; i++)
		{
			if (m_rGameData.m_bCreateTank[i])
				CreatTank(i + 1, rMapp);		// i + 1刚好是坦克对应ID
		}
}
