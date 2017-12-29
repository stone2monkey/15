#pragma once
#include <windows.h>
#include "GameData.h"

class CTank; 
class CMapp;
class CBase;

class CTankMng
{
public:
	CTankMng(CGameData& rGameData);
	~CTankMng();


	//Attributions ***********************************************************************************

	CGameData& m_rGameData;



	//Method *****************************************************************************************
	
	bool Move(CTank*& pTank, CMapp& rMapp, int iDirect);		//用副本判断
	bool MoveOfPos(CTank*& pTank, CMapp& rMapp, int iDirect);
	bool MoveOfDirect(CTank*& pTank, CMapp& rMapp, int iDirect);
	bool CollidedJudgement(CTank* pCopy, CMapp& rMapp, CTank*& pOriginal);	//pOriginal是原坦克的指针,用于排除自身
	bool am_CollidedWithTank(CTank* pCopy, CTank* pOther);	//副本与坦克的碰撞
	bool am_CollidedWithOther(CTank* pCopy, CBase* pBase);	//副本与非坦克的碰撞

	bool MoveEnemyAutomatic(CMapp& rMapp);

	bool ShotBullet(CTank* pTank, CMapp& rMapp, UINT uDelay = 0);

	void ShotEnemyAutomatic(CMapp& rMapp);

	void CreatTank(int iID, CMapp & rMapp);
	void CreatTanks(CMapp & rMapp);
};

