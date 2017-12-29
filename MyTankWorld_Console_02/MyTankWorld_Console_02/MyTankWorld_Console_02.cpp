// MyTankWorld_Console_02.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <conio.h>
#include "Interface.h"
#include "Mapp.h"
#include "TankMoveManager.h"
#include "BulletMoveManager.h"
#include "GameRule.h"



int main()
{
	//����̨��Init **************************************************************************
	CGameRule oGameRule;
	CInterface oInterface;
	oInterface.SetWindow();

	//����ѭ�� ***************************************************************
	for (;;) 
	{
		oInterface.Welcome(oGameRule);
		oInterface.Gaming(oGameRule);
		oInterface.Over(oGameRule);
	}










	/*
	//����̨��Init **************************************************************************
	CMapp oMapp;
	CMapp* pMapp = &oMapp;
	CPaint oPaint;
	oPaint.m_pMapp = pMapp;
	CTankMoveManager oTankMoveManager;
	CBulletMoveManager oBulletMoveManager;
	g_iCurrentTime = 0;

	oPaint.SetWindowRect();
	oPaint.DrawGamingFromEveryElement();

	int bRes = true;
	while (bRes)
	{
		//�õ�����ѭ����ǰʱ��
		g_iCurrentTime = ::GetTickCount();

		//�Զ��ƶ��ӵ�;
		oBulletMoveManager.MoveBulletAutomatic(pMapp, &oPaint);
		Sleep(30);
		oTankMoveManager.MoveAutomatic(pMapp);
		Sleep(30);
		oBulletMoveManager.ShotBulletAutomaticForEnemy(pMapp);
		Sleep(30);

		if (_kbhit())
		{
			//�ҵ�̹�˶����ָ��
			static int iMyselfPosInm_vecTank = 0;		//static ���εı���,����ֻ����һ��;`oo`
			iMyselfPosInm_vecTank = pMapp->GetMyselfPosInm_vecTank();
			CTank* pMyselfTank;
			if (ERROR_RETURN_VALUE == iMyselfPosInm_vecTank)
				pMyselfTank = nullptr;
			else
				pMyselfTank = pMapp->m_vecTank.at(iMyselfPosInm_vecTank);

			//�ѷ�̹�˶����ָ��
			static int iFriendPosInm_vecTank = 0;
			iFriendPosInm_vecTank = pMapp->GetFriendPosInm_vecTank();
			CTank* pFriendTank;
			if (ERROR_RETURN_VALUE == iFriendPosInm_vecTank)
				pFriendTank = nullptr;
			else
				pFriendTank = pMapp->m_vecTank.at(iFriendPosInm_vecTank);

			switch (_getch())
			{
			case 'a':
			case 'A':
				if (nullptr != pMyselfTank)
					oTankMoveManager.Move(pMyselfTank, pMapp, LEFT_DIRECTION);
				break;
			case 'w':
			case 'W':
				if (nullptr != pMyselfTank)	
					oTankMoveManager.Move(pMyselfTank, pMapp, TOP_DIRECTION);
				break;
			case 'd':
			case 'D':
				if (nullptr != pMyselfTank)
					oTankMoveManager.Move(pMyselfTank, pMapp, RIGHT_DIRECTION);
				break;
			case 's':
			case 'S':
				if (nullptr != pMyselfTank)
					oTankMoveManager.Move(pMyselfTank, pMapp, BOTTOM_DIRECTION);
				break;
			case 'q':
			case 'Q':
			{
				if (nullptr != pMyselfTank)
				{
					//�����ӵ�����;
					g_iCurrentTime = ::GetTickCount();
					CBullet* pBullet = pMyselfTank->ShotBulletPro(g_iCurrentTime);
					//���ӵ����󴴽��ɹ�ʱ,�ж��Ƿ���Ч,����;
					if (pBullet != nullptr)
					{
						pMapp->m_vecBullet.push_back(pBullet);
						oBulletMoveManager.RespondCollision(pBullet, pMapp);
					}
				}
		}
				break;
			case 'j':
			case 'J':
				if (nullptr != pFriendTank)
					oTankMoveManager.Move(pFriendTank, pMapp, LEFT_DIRECTION);
				break;
			case 'i':
			case 'I':
				if (nullptr != pFriendTank)
					oTankMoveManager.Move(pFriendTank, pMapp, TOP_DIRECTION);
				break;
			case 'l':
			case 'L':
				if (nullptr != pFriendTank)
					oTankMoveManager.Move(pFriendTank, pMapp, RIGHT_DIRECTION);
				break;
			case 'k':
			case 'K':
				if (nullptr != pFriendTank)
					oTankMoveManager.Move(pFriendTank, pMapp, BOTTOM_DIRECTION);
				break;
			case 'u':
			case 'U': 
			{
				if (nullptr != pFriendTank)
				{
					g_iCurrentTime = ::GetTickCount();
					CBullet* pBullet = pFriendTank->ShotBulletPro(g_iCurrentTime);
					if (pBullet != nullptr)
					{
						pMapp->m_vecBullet.push_back(pBullet);
						oBulletMoveManager.RespondCollision(pBullet, pMapp);
					}
				}
			}
				break;
			default:
				;
			}
		}
	}
	*/

	return 0;
}