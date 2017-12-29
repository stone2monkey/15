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
	if (uCurTime - pTank->m_uLastMoveTime < pTank->m_uMoveSpeed)	//ʱ��������
		return false;

	// 0. validity of params
	if (nullptr == pTank)
		assert(0);
	if (iDirect < LEFT_DIRECTION || iDirect > BOTTOM_DIRECTION)
		assert(0);

	// 1. �Ƿ�����ƶ�,���ƶ�
	// 2. ��������ƶ�,�Ƿ���Է���,�ͷ���
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
	// 0. �����������ƶ�
	CTank oCopy = *pTank;		//��������˿�������,����û��ָ��,��û�������ǳ����
	CTank* pCopy = &oCopy;
	pCopy->Move(iDirect);		//�ƶ�

	// 1.��ײ�ж�
	if (CollidedJudgement(pCopy, rMapp, pTank))		//��ײ��
		return false;

	return true;
}

bool CTankMng::MoveOfDirect(CTank*& pTank, CMapp & rMapp, int iDirect)
{
	// 0. ��������������
	CTank oCopy = *pTank;		//��������˿�������,����û��ָ��,��û�������ǳ����
	CTank* pCopy = &oCopy;
	if (!pCopy->JustChangeDirict(iDirect))		//���򲻳ɹ�,(�����ƶ�,��ͬ����,Ҳ����ʲôû��)
		return false;

	// 1.��ײ�ж�
	if (CollidedJudgement(pCopy, rMapp, pTank))		//��ײ��
		return false;

	return true;
}

bool CTankMng::CollidedJudgement(CTank * pCopy, CMapp & rMapp, CTank*& pOriginal)
{
	// 0. Outside
	int xCopy = pCopy->m_xPos;
	int yCopy = pCopy->m_yPos;
	if (xCopy <LEFT_BULLET_WND + 1 || xCopy>RIGHT_BULLET_WND - 1 || yCopy<TOP_BULLET_WND + 1 || yCopy>BOTTOM_BULLET_WND - 1)	//������
		return true;

	// 1. Tank
	int iSize = (int)rMapp.m_vecTank.size();
	for (int i = 0; i < iSize ; i++)
	{
		CTank* pTank = rMapp.m_vecTank.at(i);

		if (pTank == pOriginal)		//self
			continue;

		if (am_CollidedWithTank(pCopy, pTank))		//��ײ��
			return true;
	}

	// 3. Brick
	iSize = (int)rMapp.m_vecBrick.size();
	for (int i = 0; i < iSize; i++)
	{
		CBrick* pBrick = rMapp.m_vecBrick.at(i);

		if (am_CollidedWithOther(pCopy, pBrick))		//��ײ��
			return true;
	}

	// 4. King 
	if (nullptr != rMapp.m_pKing)
	{
		if (am_CollidedWithOther(pCopy, rMapp.m_pKing))		//��ײ��
			return true;
	}

	// 2. Bullet �������Ȥ,����̫�ƻ�����ɶ�����(������һ����˵���ǿ����ƶ�����)

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

			if (xCopy == xOther && yCopy == yOther)			//��ײ��
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

		if (xCopy == pBase->m_xPos && yCopy == pBase->m_yPos)			//��ײ��
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

		if (pTank->m_iID == ONEPLAYER_WHITE_TANK || pTank->m_iID == TWOPLAYER_WHITE_TANK)	//�ѷ�̹��
			continue;

		if (Move(pTank, rMapp, pTank->m_iDirect))	//����ƶ��ɹ�,������һ��;
			break;

		//�����һ�β��ɹ�,��4�λ���;
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
// Parameter: UINT uDelay			�����Ǹ��ӵ������ٶ�m_uShotSpeed����ʱ��,��Ҫ����̹�ӵ�
//************************************
bool CTankMng::ShotBullet(CTank * pTank, CMapp & rMapp, UINT uDelay)
{
	// ����ʱ���� 
	if (::GetTickCount() - pTank->m_uLastShotTime < pTank->m_uShotSpeed + uDelay)
		return false;

	// �����ӵ����ɹ�,��Ҫ��tank�����ų�����
	CBullet* pBullet = pTank->ShotBullet();
	if (nullptr == pBullet)
		return false;

	// ѹ���ӵ�
	pTank->m_uLastShotTime = ::GetTickCount();
	rMapp.m_vecBullet.push_back(pBullet);

	// ��ײ�ж� ��Ӧ		
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

		//�ų��ѷ�
		if (pTank->m_iID == ONEPLAYER_WHITE_TANK || pTank->m_iID == TWOPLAYER_WHITE_TANK)
			continue;

		ShotBullet(pTank, rMapp, 3000);
	}

}

void CTankMng::CreatTank(int iID, CMapp & rMapp)
{
	int x = 0, y = 0;	//������
	int iIndex = 0;		// m_bCreateTank[iIndex]
	int iLevel = 0;		// m_iLevel

	// ͨ��IDȷ�� 
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

	if (0 == m_rGameData.m_iTankCount[iIndex])			//̹������û����
	{
		m_rGameData.m_bCreateTank[iIndex] = false;
		return;
	}
	if (false == m_rGameData.m_bCreateTank[iIndex])		//̹�˴�����ʶΪ��
		return;
	{
		static UINT uCurTime = 0;	//�õ���ǰʱ��
		uCurTime = ::GetTickCount();

		if (uCurTime - m_rGameData.m_uTankDieTime[iIndex] < m_rGameData.m_uCreateSpeed)	//�������ʱ��С��1��,���ܴ���
			return;
	}


	// ���紴����(x,y,��)�ǿ�,�˳�;���Ǵ������Ի�����,�����´λ����ж�;	
	{
		CTank* pTank = new CTank(x, y, iID, iLevel, 3);		//	������̹�˰�,���ü���� `~`

		rMapp.FillCurMappArr();							//��䵱ǰ��ͼ����

		int arr[9] = { 1, 0, 1, 1, 1, 1, 0, 1, 0 };		//�õ�6��
		pTank->am_FillSixPtArrViaArr(arr);
		for (int i = 0; i < 6; i++)					//6�����ͼ�Ƚ�,��ȫΪ0ʱ,˵��̹�˿��Դ�����,�е���	`~`
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

	
	//���� ѹ���ͼvector
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
				CreatTank(i + 1, rMapp);		// i + 1�պ���̹�˶�ӦID
		}
}
