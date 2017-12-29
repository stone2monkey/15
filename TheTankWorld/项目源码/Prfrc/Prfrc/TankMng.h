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
	
	bool Move(CTank*& pTank, CMapp& rMapp, int iDirect);		//�ø����ж�
	bool MoveOfPos(CTank*& pTank, CMapp& rMapp, int iDirect);
	bool MoveOfDirect(CTank*& pTank, CMapp& rMapp, int iDirect);
	bool CollidedJudgement(CTank* pCopy, CMapp& rMapp, CTank*& pOriginal);	//pOriginal��ԭ̹�˵�ָ��,�����ų�����
	bool am_CollidedWithTank(CTank* pCopy, CTank* pOther);	//������̹�˵���ײ
	bool am_CollidedWithOther(CTank* pCopy, CBase* pBase);	//�������̹�˵���ײ

	bool MoveEnemyAutomatic(CMapp& rMapp);

	bool ShotBullet(CTank* pTank, CMapp& rMapp, UINT uDelay = 0);

	void ShotEnemyAutomatic(CMapp& rMapp);

	void CreatTank(int iID, CMapp & rMapp);
	void CreatTanks(CMapp & rMapp);
};

