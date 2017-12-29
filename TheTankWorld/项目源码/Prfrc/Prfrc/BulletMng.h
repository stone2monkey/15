#pragma once
#include "GameData.h"

//#include "Mapp.h"

class CBullet;
class CMapp;
class CTank;
class CBrick;
class CKing;
/*
�ӵ����ƶ���һ����ǰ��,����û���ø���;
�ӵ����ƶ�����ײ�ж��ǲ���bool��
���������ֻ��Ѫ��Ϊ0��Ok��
�ƶ���,�ı���ײ˫�������Ծͺ���
Move��ɺ�,����Ѫ��Ϊ0�Ķ���ͺ���.
*/

class CBulletMng
{
public:
	CBulletMng(CGameData& rGameData);
	~CBulletMng();


	//Attributions ***********************************************************************************

	CGameData& m_rGameData;



	//Method *****************************************************************************************

	void Move(CBullet* pSelf, CMapp& rMapp);	//�ƶ�bool����ײboolֻ�Ǵ����ӵ��Ƿ񻹻���
	void CollidedJudgement(CBullet* pSelf, CMapp& rMapp);	//�ƶ�bool����ײboolֻ�Ǵ����ӵ��Ƿ񻹻���
	void am_CollidedWithTank(CBullet* pSelf, CTank* pTank);
	void am_CollidedWithBullet(CBullet* pSelf, CBullet* pBullet);
	void am_CollidedWithBrick(CBullet* pSelf, CBrick* pBrick);
	void am_CollidedWithKing(CBullet* pSelf, CKing* pKing);

	void RespondAfterMove(CMapp& rMapp);

	void MoveAutomatic(CMapp& rMapp);
};

