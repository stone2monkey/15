#include "stdafx.h"
#include <conio.h>
#include "Interface.h"


CInterface::CInterface()
{
}


CInterface::~CInterface()
{
}


//���ô��ڱ���,�ߴ�,λ��
void CInterface::SetWindow()
{
	::SetConsoleTitleW(_T("̹�˴�ս"));
	::SetWindowPos(::GetConsoleWindow(), NULL, 400, 200, 850, 375, SWP_SHOWWINDOW);
}


// ��ӭ����
void CInterface::Welcome(CGameRule& oGameRule)
{
	if (!oGameRule.m_bInterfaceSelect[0][0])
		return;

	::WriteChar(6, 4, "̹�˴�ս:", RED);
	::WriteChar(8, 6, "1. ������Ϸ", GREEN);
	::WriteChar(8, 7, "2. ˫����Ϸ", GREEN);
	::WriteChar(8, 8, "3. ��Ϸѡͼ", GREEN);

	while (oGameRule.m_bInterfaceSelect[0][0])
		if (_kbhit())
			switch (_getch())
			{
				case '1':
				{
					oGameRule.m_bOnePlayer = true;
					oGameRule.m_bInterfaceSelect[0][0] = false;
					oGameRule.m_bInterfaceSelect[0][1] = true;
					oGameRule.m_bInterfaceSelect[0][2] = false;
				}break;
				case '2':
				{
					oGameRule.m_bOnePlayer = false;
					oGameRule.m_bInterfaceSelect[0][0] = false;
					oGameRule.m_bInterfaceSelect[0][1] = true;
					oGameRule.m_bInterfaceSelect[0][2] = false;
				}break;
				case '3':
				{

				}break;
			}
}


// ��Ϸ�н���
void CInterface::Gaming(CGameRule& oGameRule)
{
	if (!oGameRule.m_bInterfaceSelect[0][1])
		return;

	CMapp oMapp;
	CMapp* pMapp = &oMapp;
	CTankMoveManager oTankMoveManager;
	CBulletMoveManager oBulletMoveManager;

	pMapp->InitMappArray();

	while (oGameRule.m_bInterfaceSelect[0][1])
	{
		//�õ�����ѭ����ǰʱ��
		g_iCurrentTime = ::GetTickCount();

		//�Զ��ƶ��ӵ�;
		oBulletMoveManager.MoveBulletAutomatic(pMapp);
		//Sleep(80);
		oTankMoveManager.MoveAutomatic(pMapp);
		//Sleep(30);
		oBulletMoveManager.ShotBulletAutomaticForEnemy(pMapp);

		if (_kbhit())
		{
			//�ҵ�̹�˶����ָ��
			CTank* pMyselfTank = oMapp.GetMyselfTank();
			//�ѷ�̹�˶����ָ��
			CTank* pFriendTank = oMapp.GetFriendTank();

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
}


// ��Ϸ��������
void CInterface::Over(CGameRule& oGameRule)
{
	if (!oGameRule.m_bInterfaceSelect[0][2])
		return;

	while (oGameRule.m_bInterfaceSelect[0][2])
		if (_kbhit())
			switch (_getch())
			{
			case '1':
			{
			}break;
			case '2':
			{
			}break;
			case '3':
			{

			}break;
			}
}
