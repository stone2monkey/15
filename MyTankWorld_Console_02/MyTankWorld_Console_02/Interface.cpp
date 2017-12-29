#include "stdafx.h"
#include <conio.h>
#include "Interface.h"


CInterface::CInterface()
{
}


CInterface::~CInterface()
{
}


//设置窗口标题,尺寸,位置
void CInterface::SetWindow()
{
	::SetConsoleTitleW(_T("坦克大战"));
	::SetWindowPos(::GetConsoleWindow(), NULL, 400, 200, 850, 375, SWP_SHOWWINDOW);
}


// 欢迎界面
void CInterface::Welcome(CGameRule& oGameRule)
{
	if (!oGameRule.m_bInterfaceSelect[0][0])
		return;

	::WriteChar(6, 4, "坦克大战:", RED);
	::WriteChar(8, 6, "1. 单人游戏", GREEN);
	::WriteChar(8, 7, "2. 双人游戏", GREEN);
	::WriteChar(8, 8, "3. 游戏选图", GREEN);

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


// 游戏中界面
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
		//得到本次循环当前时间
		g_iCurrentTime = ::GetTickCount();

		//自动移动子弹;
		oBulletMoveManager.MoveBulletAutomatic(pMapp);
		//Sleep(80);
		oTankMoveManager.MoveAutomatic(pMapp);
		//Sleep(30);
		oBulletMoveManager.ShotBulletAutomaticForEnemy(pMapp);

		if (_kbhit())
		{
			//我的坦克对象的指针
			CTank* pMyselfTank = oMapp.GetMyselfTank();
			//友方坦克对象的指针
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
					//创建子弹对象;
					g_iCurrentTime = ::GetTickCount();
					CBullet* pBullet = pMyselfTank->ShotBulletPro(g_iCurrentTime);
					//当子弹对象创建成功时,判断是否有效,可显;
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


// 游戏结束界面
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
