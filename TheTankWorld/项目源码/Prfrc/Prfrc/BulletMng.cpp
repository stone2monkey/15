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
	//�ƶ�ʱ�����ж�
	if (::GetTickCount() - pSelf->m_uLastMoveTime < pSelf->m_uMoveSpeed)
		return;

	// 0. validity of params
	// 1. �ƶ�֮ǰҪ��Ҫ����ײ�ж�,�������,�������ǲ���,Ҳ����Ӧ���ӵ���ײ�жϵ�ʱ��,�ڸ�����ʱ������
	// ���ܸı䱻��ײ��������� -xxx- 2. �ƶ�֮ǰ�õ���ǰ��ͼ����,����ȥ����ײ�ж�(��ͼ�е������ǲ�Ӱ���жϵ�,no care)
	// 3. �ƶ�
	pSelf->Move(pSelf->m_iDirect);
	pSelf->m_uLastMoveTime = ::GetTickCount();

	// 4. ��ײ�ж��Լ����ݵı��
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

			if (x == xTank && y == yTank)				//�����ײ
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

		if (x == xBullet && y == yBullet)				//�����ײ
			am_CollidedWithBullet(pSelf, pBullet);
	}

	// 3. brick
	iSize = (int)rMapp.m_vecBrick.size();
	for (int i = 0; i < iSize; i++)
	{
		CBrick* pBrick = rMapp.m_vecBrick.at(i);
		int xBrick = pBrick->m_xPos;
		int yBrick = pBrick->m_yPos;

		if (x == xBrick && y == yBrick)					//�����ײ
			am_CollidedWithBrick(pSelf, pBrick);		
	}

	// 4. king
	if (nullptr != rMapp.m_pKing)
	{
		int xKing = rMapp.m_pKing->m_xPos;
		int yKing = rMapp.m_pKing->m_yPos;

		if (x == xKing && y == yKing)					//�����ײ
			am_CollidedWithKing(pSelf, rMapp.m_pKing);
	}
}

void CBulletMng::am_CollidedWithTank(CBullet * pSelf, CTank * pTank)
{
	if (ONEPLAYER_WHITE_BULLET == pSelf->m_iID || TWOPLAYER_WHITE_BULLET == pSelf->m_iID)	//�ѷ��ӵ�
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
				// ��̹��Ѫ
				// �Լ�����
				// ����
				pTank->m_iBlood -= pSelf->m_iForce;
				pTank->m_iLevel -= pSelf->m_iForce;
				pSelf->m_iBlood = 0;
				return;
			}break;
			default:
				assert(0);
		}
	}
	else																					//�з��ӵ�
	{
		switch (pTank->m_iID)
		{
			case ONEPLAYER_WHITE_TANK:
			case TWOPLAYER_WHITE_TANK:
			{
				// ��̹��Ѫ
				// �Լ�����
				// ����
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
	if (ONEPLAYER_WHITE_BULLET == pSelf->m_iID || TWOPLAYER_WHITE_BULLET == pSelf->m_iID)	//�ѷ��ӵ�
	{
		//�ų������,��ʵ��Ҳû��ϵ
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
				// �з�����
				// �Լ�����
				// ����
				pBullet->m_iBlood = 0;
				pSelf->m_iBlood = 0;
				return;
			}break;
			default:
				assert(0);
		}
	}
	else																					//�з��ӵ�
	{
		//�ų������,��ʵ��Ҳû��ϵ
		if (pSelf == pBullet)
			return;

		switch (pBullet->m_iID)
		{
			case ONEPLAYER_WHITE_BULLET:
			case TWOPLAYER_WHITE_BULLET:
			{
				// �ѷ�����
				// �Լ�����
				// ����
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
	if (ONEPLAYER_WHITE_BULLET == pSelf->m_iID && STEEL_WHITE_BRICK == pBrick->m_iID)	//OnePlay���Դ���ǽ
		return;

	switch (pBrick->m_iID)
	{
		case STEEL_WHITE_BRICK:
			pSelf->m_iBlood = 0;	//�Լ�����
			return;
			break;
		case REVER_BLUE_BRICK:
			return;
			break;
		case NORMAL_PURPLE_BRICK:
			pBrick->m_iBlood -= pSelf->m_iForce;	//��ͨ�����Ѫ
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
// Qualifier: 1. GameData�����������0ֵ��ͼ�������Ӧ
// Parameter: CMapp & rMapp
//************************************
void CBulletMng::RespondAfterMove(CMapp& rMapp)
{
	// �ı�m_rGameData
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

		CollidedJudgement(pBullet, rMapp);		//�ӵ��ƶ�ǰ������ײ�ж�
		RespondAfterMove(rMapp);

		Move(pBullet, rMapp);					//�ƶ�
		RespondAfterMove(rMapp);
	}
}
