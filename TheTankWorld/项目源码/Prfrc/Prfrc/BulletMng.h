#pragma once
#include "GameData.h"

//#include "Mapp.h"

class CBullet;
class CMapp;
class CTank;
class CBrick;
class CKing;
/*
子弹的移动是一往无前的,所以没有用副本;
子弹的移动与碰撞判断是不用bool的
对象的死亡只用血量为0就Ok了
移动后,改变碰撞双方的属性就好了
Move完成后,处理血量为0的对象就好了.
*/

class CBulletMng
{
public:
	CBulletMng(CGameData& rGameData);
	~CBulletMng();


	//Attributions ***********************************************************************************

	CGameData& m_rGameData;



	//Method *****************************************************************************************

	void Move(CBullet* pSelf, CMapp& rMapp);	//移动bool与碰撞bool只是代表子弹是否还活着
	void CollidedJudgement(CBullet* pSelf, CMapp& rMapp);	//移动bool与碰撞bool只是代表子弹是否还活着
	void am_CollidedWithTank(CBullet* pSelf, CTank* pTank);
	void am_CollidedWithBullet(CBullet* pSelf, CBullet* pBullet);
	void am_CollidedWithBrick(CBullet* pSelf, CBrick* pBrick);
	void am_CollidedWithKing(CBullet* pSelf, CKing* pKing);

	void RespondAfterMove(CMapp& rMapp);

	void MoveAutomatic(CMapp& rMapp);
};

