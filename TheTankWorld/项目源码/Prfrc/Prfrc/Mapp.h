#pragma once
#include <vector>
#include "Tank.h"
#include "Bullet.h"
#include "Brick.h"
#include "King.h"

using namespace std;

class CMapp
{
private:

	CMapp(CMapp& rMapp);
	void operator=(CMapp& rMapp);

public:
	CMapp(string strMappFileName, bool bOnePlayer = false);
	~CMapp();


	//Attributions ***********************************************************************************

	vector<CTank*> m_vecTank;
	vector<CBullet*> m_vecBullet;
	vector<CBrick*> m_vecBrick;
	CKing* m_pKing;

	int m_arrInitMapp[BOTTOM_BULLET_WND + 1][RIGHT_BULLET_WND + 1] = {};
	int m_arrOldMapp[BOTTOM_BULLET_WND + 1][RIGHT_BULLET_WND + 1] = {};
	int m_arrCurMapp[BOTTOM_BULLET_WND + 1][RIGHT_BULLET_WND + 1] = {};

	string am_strMappFileName = {};
	bool am_bOnePlayer = false;

	//Method *****************************************************************************************

	void Init();
	// --------------------
	void InitInitMappArr();
	void am_FillInitMappArrViaFile();
	void am_CharToInt(char& ch);
	void am_OnePlayer();
	// --------------------
	void InitObj();

	void FillCurMappArr();
	// --------------------
	void FillWithoutBullets();	//把除开子弹的所有对象的ID对应到当前地图数组上,注意坦克是6个点
	void am_FillTankSixPtIntoCurMappArr(CTank* pTank);
	// --------------------
	void FillBullets();
	void am_VisibilityOfBullets(CBullet* pBullet);	//在填充后"没有子弹的当前地图数组"上,为0的地方才能显示子弹;当多颗时,也就第一个显示了
	
	CTank* GetTank(int iID);	//这个得到坦克对象的方法只能找友方,因为他们都只有一个

	void ClearWhoNoBlood();
	void deleteAndErase(CBullet* pBullet, int iIndex);
	void deleteAndErase(CTank* pTank, int iIndex);
	void deleteAndErase(CBrick* pBrick, int iIndex);
};

