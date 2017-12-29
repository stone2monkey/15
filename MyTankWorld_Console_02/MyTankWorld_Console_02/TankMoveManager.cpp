#include "stdafx.h"
#include "TankMoveManager.h"


CTankMoveManager::CTankMoveManager()
{
}


CTankMoveManager::~CTankMoveManager()
{
}

bool CTankMoveManager::Move(CTank* pTank, CMapp* pMapp, int iMoveToFlag)
{
	//�ƶ��ٶ�(ʱ����)�Ƿ�;
	if (g_iCurrentTime < pTank->m_iLastMoveTime)
		assert(0);
	if (!(g_iCurrentTime - pTank->m_iLastMoveTime >= pTank->m_iMoveSpeed))
		return false;

	//���û����ײ,���ԭͼ,�����ƶ����ͼ;����,�ı䷽����Ƿ���ײ;
	if (!CollisionCheck(pTank, pMapp, iMoveToFlag))
	{
		//�����ƶ�ʱ,�޸�
		pTank->m_iLastMoveTime = g_iCurrentTime;
		//���ԭ̹��λ�õ�ͼ
		pTank->ClearMyself();
		//���ݷ����ƶ�
		switch (iMoveToFlag)
		{
		case LEFT_DIRECTION:
			pTank->MoveToLeft();
			break;
		case TOP_DIRECTION:
			pTank->MoveToTop();
			break;
		case RIGHT_DIRECTION:
			pTank->MoveToRight();
			break;
		case BOTTOM_DIRECTION:
			pTank->MoveToBottom();
			break;
		default:
			assert(0);
			break;
		}
		//�������ڵ�̹��λ�õ�ͼ
		pTank->DrawMyself();
		return true;
	}
	else if (!CollisionCheckOfMyselfAfterAdjustingDirection(pTank, pMapp, iMoveToFlag))
	{
		//����"����"ʱ,�޸�
		pTank->m_iLastMoveTime = g_iCurrentTime;

		//���ԭ̹��λ�õ�ͼ
		pTank->ClearMyself();
		//���ݷ���"����"
		switch (iMoveToFlag)
		{
		case LEFT_DIRECTION:
			pTank->m_iDirection = LEFT_DIRECTION;
			break;
		case TOP_DIRECTION:
			pTank->m_iDirection = TOP_DIRECTION;
			break;
		case RIGHT_DIRECTION:
			pTank->m_iDirection = RIGHT_DIRECTION;
			break;
		case BOTTOM_DIRECTION:
			pTank->m_iDirection = BOTTOM_DIRECTION;
			break;
		default:
			assert(0);
			break;
		}
		//�������ڵ�̹��λ�õ�ͼ
		pTank->DrawMyself();
		return true;
	}

	return false;
}

//***********************************************************
// ����: CTankMoveManager::CollisionCheck
// ����: Judge that whether or not can moving;
// �̳�: 
// ����: bool
// para: left-value
// para: right-value
// para: direction
//***********************************************************
bool CTankMoveManager::CollisionCheck(CTank* pTank, CMapp* pMapp, int iMoveToFlag)
{
	//�߼����ȴ�����ֵ����,���������ƶ���ֵ;���ų�ԭ��ֵ,���������Ƚ�;  �����ĺô��ǲ������ƶ����ж�,�ﵽ���ж����ƶ�.
	CTank left_Tank = *pTank;	//CTank��û��ָ��,���Կ��Կ�������;
								//Judge the moving direction;
	switch (iMoveToFlag)
	{
	case LEFT_DIRECTION:
		left_Tank.MoveToLeft();
		break;
	case TOP_DIRECTION:
		left_Tank.MoveToTop();
		break;
	case RIGHT_DIRECTION:
		left_Tank.MoveToRight();
		break;
	case BOTTOM_DIRECTION:
		left_Tank.MoveToBottom();
		break;
	default:
		assert(0);
		break;
	}

	return am_TankCollisionCheckOfMyself(left_Tank, pMapp, pTank);
}
// //����λ�ƺ�,�ж��Ƿ���Ըı䷽��
bool CTankMoveManager::CollisionCheckOfMyselfAfterAdjustingDirection(CTank* pTank, CMapp* pMapp, int iMoveToFlag)
{
	//�߼����ȴ�����ֵ����,������ֵ;���ų�ԭ��ֵ,���������Ƚ�;  �����ĺô��ǲ������ƶ����ж�,�ﵽ���ж����ƶ�.
	CTank left_Tank = *pTank;	//CTank��û��ָ��,���Կ��Կ�������;

	//���ͬ��,���൱��̹�˲����ƶ�,"����",Ҳ����˵nothing happend,return false;
	if (iMoveToFlag == left_Tank.m_iDirection)
		return true;

	//Judge the moving direction;
	switch (iMoveToFlag)
	{
	case LEFT_DIRECTION:
		left_Tank.m_iDirection = LEFT_DIRECTION;
		break;
	case TOP_DIRECTION:
		left_Tank.m_iDirection = TOP_DIRECTION;
		break;
	case RIGHT_DIRECTION:
		left_Tank.m_iDirection = RIGHT_DIRECTION;
		break;
	case BOTTOM_DIRECTION:
		left_Tank.m_iDirection = BOTTOM_DIRECTION;
		break;
	default:
		assert(0);
		break;
	}

	return am_TankCollisionCheckOfMyself(left_Tank, pMapp, pTank);
}
// //ȷ����̹�����ݺ�,ֱ������ײ�����
// left_Tank: ����õ�̹�˸���;
// pTank: ԭ̹�˵�ָ��;��Ҫ�����ų�����;
bool CTankMoveManager::am_TankCollisionCheckOfMyself(CTank& left_Tank, CMapp* pMapp, CTank* pTank)
{
	//0. Outside; ***************
	//1. m_vecTank; **************
	//2. m_vecBullet; **************
	//3. m_vecBrick; **************
	//4. m_pKing; **************

	vector<CPoint> left_vecTankSixPointPos = left_Tank.FillTankSixPointPos();

	//0. Outside; ***********************************************************************
	//����judgeÿһ�����Ƿ����,Ҳ����������ʵ��:	
	int xMovedTank_xPos = left_Tank.m_xPos;
	int yMovedTank_yPos = left_Tank.m_yPos;
	if ((xMovedTank_xPos < LEFT_BULLET_WND + 1) || (xMovedTank_xPos > RIGHT_BULLET_WND - 1) ||
		(yMovedTank_yPos < TOP_BULLET_WND + 1) || (yMovedTank_yPos > BOTTOM_BULLET_WND - 1))
		return true;

	//1. m_vecTank; *************************************************************************
	for (int i = 0; i < 6; i++)
	{
		int left_xPos = left_vecTankSixPointPos.at(i).m_xPos;
		int left_yPos = left_vecTankSixPointPos.at(i).m_yPos;
		int iSize = pMapp->m_vecTank.size();
		for (int j = 0; j < iSize; j++)
		{
			//�ų�����
			if (pTank == pMapp->m_vecTank.at(j))
				continue;

			vector<CPoint> right_vecTankSixPointPos = pMapp->m_vecTank.at(j)->FillTankSixPointPos();
			for (int k = 0; k < 6; k++)
			{
				int right_xPos = right_vecTankSixPointPos.at(k).m_xPos;
				int right_yPos = right_vecTankSixPointPos.at(k).m_yPos;
				if (left_xPos == right_xPos && left_yPos == right_yPos)
					return true;
			}
		}
	}
	//2. m_vecBullet; *************************************************************************
	for (int i = 0; i < 6; i++)
	{
		int left_xPos = left_vecTankSixPointPos.at(i).m_xPos;
		int left_yPos = left_vecTankSixPointPos.at(i).m_yPos;
		int iSize = pMapp->m_vecBullet.size();
		for (int j = 0; j < iSize; j++)
		{
			int right_xPos = pMapp->m_vecBullet.at(j)->m_xPos;
			int right_yPos = pMapp->m_vecBullet.at(j)->m_yPos;
			if (left_xPos == right_xPos && left_yPos == right_yPos)
				return true;
		}
	}
	//3. m_vecBrick; *************************************************************************
	for (int i = 0; i < 6; i++)
	{
		int left_xPos = left_vecTankSixPointPos.at(i).m_xPos;
		int left_yPos = left_vecTankSixPointPos.at(i).m_yPos;
		int iSize = pMapp->m_vecBrick.size();
		for (int j = 0; j < iSize; j++)
		{
			int right_xPos = pMapp->m_vecBrick.at(j)->m_xPos;
			int right_yPos = pMapp->m_vecBrick.at(j)->m_yPos;
			if (left_xPos == right_xPos && left_yPos == right_yPos)
				return true;
		}
	}
	//4. m_pKing; *************************************************************************
	for (int i = 0; i < 6; i++)
	{
		int left_xPos = left_vecTankSixPointPos.at(i).m_xPos;
		int left_yPos = left_vecTankSixPointPos.at(i).m_yPos;
		int right_xPos = pMapp->m_pKing->m_xPos;
		int right_yPos = pMapp->m_pKing->m_yPos;
		if (left_xPos == right_xPos && left_yPos == right_yPos)
			return true;
	}

	return false;
}


void CTankMoveManager::MoveAutomatic(CMapp* pMapp)
{
	srand((UINT)time(NULL));

	int iSize = (int)pMapp->m_vecTank.size();
	for (int i = 0; i < iSize; i++)
	{
		CTank* pTank = pMapp->m_vecTank.at(i);
		//�ų��ѷ�
		if (pMapp->m_vecTank.at(i)->m_iIDFlag == MYSELF_WHITE_TANK ||
			pMapp->m_vecTank.at(i)->m_iIDFlag == FRIEND_WITHE_TANK)
			continue;
		
		//ÿ��̹��,ֻ���ƶ�Ϊ��ʱ,���˳�,Ҳ����˵:��ײ֮��,�ҵ������ƶ��ķ���,����"����"�ķ���Ϊ��(�������Ϊ�Լ��ķ���,��Ϊ��);
		//int iDirection = pTank->m_iDirection;
		if (Move(pTank, pMapp, pTank->m_iDirection))
			continue;

		for (int i = 0; i < 3; i++)
		{
			int iDirection = rand() % 4;
			if (Move(pTank, pMapp, iDirection))
				break;
		}
	}		
	Sleep(50);
	
}



//bool CTankMoveManager::MoveToLeft(CTank* pTank, CMapp* pMapp)
//{
//	//���û����ײ,���ԭͼ,�����ƶ����ͼ;����,�ı䷽����Ƿ���ײ;
//	if (!CollisionCheck(pTank, pMapp, LEFT_DIRECTION))
//	{
//		pTank->ClearMyself();
//		pTank->MoveToLeft();
//		pTank->DrawMyself();
//		return true;
//	}
//	else if (!CollisionCheckOfMyselfAfterAdjustingDirection(pTank, pMapp, LEFT_DIRECTION))
//	{
//		pTank->ClearMyself();
//		pTank->m_iDirection = LEFT_DIRECTION;
//		pTank->DrawMyself();
//	}
//
//	return false;
//}
//bool CTankMoveManager::MoveToTop(CTank* pTank, CMapp* pMapp)
//{
//	//���û����ײ,���ԭͼ,�����ƶ����ͼ;����,���ԭͼ,�������ڷ����ͼ
//	if (!CollisionCheck(pTank, pMapp, TOP_DIRECTION))
//	{
//		pTank->ClearMyself();
//		pTank->MoveToTop();
//		pTank->DrawMyself();
//		return true;
//	}
//	else if (!CollisionCheckOfMyselfAfterAdjustingDirection(pTank, pMapp, TOP_DIRECTION))
//	{
//		pTank->ClearMyself();
//		pTank->m_iDirection = TOP_DIRECTION;
//		pTank->DrawMyself();
//	}
//
//	return false;
//}
//bool CTankMoveManager::MoveToRight(CTank* pTank, CMapp* pMapp)
//{
//	//���û����ײ,���ԭͼ,�����ƶ����ͼ;����,���ԭͼ,�������ڷ����ͼ
//	if (!CollisionCheck(pTank, pMapp, RIGHT_DIRECTION))
//	{
//		pTank->ClearMyself();
//		pTank->MoveToRight();
//		pTank->DrawMyself();
//		return true;
//	}
//	else if (!CollisionCheckOfMyselfAfterAdjustingDirection(pTank, pMapp, RIGHT_DIRECTION))
//	{
//		pTank->ClearMyself();
//		pTank->m_iDirection = RIGHT_DIRECTION;
//		pTank->DrawMyself();
//	}
//
//	return false;
//}
//bool CTankMoveManager::MoveToBottom(CTank* pTank, CMapp* pMapp)
//{
//	//���û����ײ,���ԭͼ,�����ƶ����ͼ;����,���ԭͼ,�������ڷ����ͼ
//	if (!CollisionCheck(pTank, pMapp, BOTTOM_DIRECTION))
//	{
//		pTank->ClearMyself();
//		pTank->MoveToBottom();
//		pTank->DrawMyself();
//		return true;
//	}
//	else if (!CollisionCheckOfMyselfAfterAdjustingDirection(pTank, pMapp, BOTTOM_DIRECTION))
//	{
//		pTank->ClearMyself();
//		pTank->m_iDirection = BOTTOM_DIRECTION;
//		pTank->DrawMyself();
//	}
//
//	return false;
//}
