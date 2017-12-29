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
	void FillWithoutBullets();	//�ѳ����ӵ������ж����ID��Ӧ����ǰ��ͼ������,ע��̹����6����
	void am_FillTankSixPtIntoCurMappArr(CTank* pTank);
	// --------------------
	void FillBullets();
	void am_VisibilityOfBullets(CBullet* pBullet);	//������"û���ӵ��ĵ�ǰ��ͼ����"��,Ϊ0�ĵط�������ʾ�ӵ�;�����ʱ,Ҳ�͵�һ����ʾ��
	
	CTank* GetTank(int iID);	//����õ�̹�˶���ķ���ֻ�����ѷ�,��Ϊ���Ƕ�ֻ��һ��

	void ClearWhoNoBlood();
	void deleteAndErase(CBullet* pBullet, int iIndex);
	void deleteAndErase(CTank* pTank, int iIndex);
	void deleteAndErase(CBrick* pBrick, int iIndex);
};

