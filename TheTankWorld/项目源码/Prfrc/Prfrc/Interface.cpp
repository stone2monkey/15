#include "stdafx.h"
#include "Interface.h"
#include <conio.h>

CInterface::CInterface()
{
	for (int i = 0; i < 10; i++)
	{
		m_bInterfaceSwitch[i] = false;
	}
	m_bInterfaceSwitch[0] = true;
}


CInterface::~CInterface()
{
}


//************************************
// ����: WriteChar
// ����: ���ù��λ�ã�����ָ��λ�ô�ӡָ����ɫ���ַ���
// �̳�: private 
// ����: void
// ����: int Wide       ��
// ����: int High       ��
// ����: string pszChar �ַ���
// ����: WORD wArr      ��ɫ
//************************************
void CInterface::WriteChar(int x, int y, char* pszChar, WORD wColor)
{
	// ���ù������
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 1;
	cci.bVisible = FALSE;	        // �Ƿ���ʾ���
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
	// ��������
	COORD loc;
	loc.X = x * 2;               // x��ֵ��Wide��2��
	loc.Y = y;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), loc);
	printf(pszChar); //��ӡʱ��Ҫע����Щ�����ַ���ռ�����ֽ�
}

void CInterface::SetWindow()
{
	::SetConsoleTitleW(_T("Tank world :"));
	::SetWindowPos(::GetConsoleWindow(), NULL, 400, 200, 850, 375, SWP_SHOWWINDOW);
}

void CInterface::Welcome()
{
	// switch
	if (!m_bInterfaceSwitch[0])
		return;

	// welcome
	system("cls");
	WriteChar(4, 4, "Tank world :", WHITE);
	WriteChar(6, 7, "a. ѡ���ͼ", WHITE);
	//WriteChar(6, 8, "b. ��Ч�ѿ���", WHITE);
	WriteChar(6, 9, "c. DIY��ͼ", WHITE);

	// respond keyboard
	while (true)
	{
		if (_kbhit())
		{
			switch (_getch())
			{
				case 'a':
				case 'A':
				{
					for (int i = 0; i < 10; i++)
					{
						m_bInterfaceSwitch[i] = false;
					}
					m_bInterfaceSwitch[1] = true;
					return;
				}break;
				case 'b':
				/*case 'B':
				{
					static bool bMusic = true;

					if (bMusic)
						bMusic = false;
					else
						bMusic = true;

					if (bMusic)
					{
						m_oMusic.m_bMusic[0] = true;
						m_oMusic.m_bMusic[1] = true;
						m_oMusic.m_bMusic[2] = true;
						m_oMusic.m_bMusic[3] = true;
						m_oMusic.m_bMusic[4] = true;

						WriteChar(6, 8, "b. ��Ч�ѿ���", WHITE);
					}
					else
					{
						m_oMusic.m_bMusic[0] = false;
						m_oMusic.m_bMusic[1] = false;
						m_oMusic.m_bMusic[2] = false;
						m_oMusic.m_bMusic[3] = false;
						m_oMusic.m_bMusic[4] = false;

						WriteChar(6, 8, "b. ��Ч�ѹر�", WHITE);
					}
				}break;*/
				case 'c':
				case 'C':
				{
					for (int i = 0; i < 10; i++)
					{
						m_bInterfaceSwitch[i] = false;
					}
					m_bInterfaceSwitch[5] = true;
					return;
				}break;

				default:
					;
			}
		}
	}
}

void CInterface::SelectMapp()
{
	// switch
	if (!m_bInterfaceSwitch[1])
		return;

	// SelectMapp
	system("cls");
	WriteChar(4, 4, "ѡ���ͼ :", WHITE);
	WriteChar(6, 7, "a. ��ͼa", WHITE);
	WriteChar(6, 8, "b. ��ͼb", WHITE);
	WriteChar(6, 9, "c. ��ͼc", WHITE);

	// respond keyboard
	while (true)
	{
		if (_kbhit())
		{
			switch (_getch())
			{
				case 'a':
				case 'A':
				{
					m_oGameData.m_strMappFileName[m_oGameData.m_iSwitchCard] = "a.txt";

					for (int i = 0; i < 10; i++)
					{
						m_bInterfaceSwitch[i] = false;
					}
					m_bInterfaceSwitch[2] = true;
					return;
				}break;
				case 'b':
				case 'B':
				{
					m_oGameData.m_strMappFileName[m_oGameData.m_iSwitchCard] = "b.txt";

					for (int i = 0; i < 10; i++)
					{
						m_bInterfaceSwitch[i] = false;
					}
					m_bInterfaceSwitch[2] = true;
					return;
				}break;
				case 'c':
				case 'C':
				{
					m_oGameData.m_strMappFileName[m_oGameData.m_iSwitchCard] = "c.txt";

					for (int i = 0; i < 10; i++)
					{
						m_bInterfaceSwitch[i] = false;
					}
					m_bInterfaceSwitch[2] = true;
					return;
				}break;

				default:
					;
			}
		}
	}
}

void CInterface::OnePlayer()
{
	// switch
	if (!m_bInterfaceSwitch[2])
		return;

	//OnePlayer
	system("cls");
	WriteChar(4, 4, "OnePlayer ?", WHITE);
	WriteChar(6, 7, "a. true", WHITE);
	WriteChar(6, 8, "b. false", WHITE);

	// respond keyboard
	while (true)
	{
		if (_kbhit())
		{
			switch (_getch())
			{
				case 'a':
				case 'A':
				{
					m_oGameData.m_bOnePlayer = true;

					for (int i = 0; i < 10; i++)
					{
						m_bInterfaceSwitch[i] = false;
					}
					m_bInterfaceSwitch[3] = true;
					m_bInterfaceSwitch[4] = true;
					return;
				}break;
				case 'b':
				case 'B':
				{
					m_oGameData.m_bOnePlayer = false;

					for (int i = 0; i < 10; i++)
					{
						m_bInterfaceSwitch[i] = false;
					}
					m_bInterfaceSwitch[3] = true;
					m_bInterfaceSwitch[4] = true;
					return;
				}break;

				default:
					;
			}
		}
	}
}

void CInterface::Gaming(string strMappFileName, bool bOnePlayer)
{
	// switch
	if (!m_bInterfaceSwitch[3])
		return;

	// Gaming
	system("cls");
	am_FixedFrame();

	CMapp oMapp(strMappFileName, bOnePlayer);
	CDrawGaming oDrawGaming(oMapp, m_oGameData);
	CTankMng oTankMng(m_oGameData);
	CBulletMng oBulletMng(m_oGameData);
	CGameDataMng oGameDataMng(oMapp, m_oGameData);

	// ����m_oGameData��һЩ����
	oGameDataMng.ReSetForNewSwitchCard();
	oDrawGaming.UpdateMapp();	//���Ƶ�ͼ

	::PlaySound(_TEXT("Music\\START_MUSIC.wav"), NULL, SND_FILENAME | SND_ASYNC);
	//::PlaySound(_TEXT("Music\\BACKGROUND_MUSIC.wav"), NULL, SND_FILENAME | SND_ASYNC);
	//::Sleep(4000);
	
	while (m_bInterfaceSwitch[3])
	{
		oDrawGaming.UpdateMapp();	//���Ƶ�ͼ
		oTankMng.MoveEnemyAutomatic(oMapp);
		//oDrawGaming.UpdateMapp();	//���Ƶ�ͼ
		oBulletMng.MoveAutomatic(oMapp);
		oTankMng.ShotEnemyAutomatic(oMapp);

		if (_kbhit())
		{
			switch (_getch())
			{
				case 'a':
				case 'A':
				{
					CTank* pOnePlayer = oMapp.GetTank(ONEPLAYER_WHITE_TANK);
					if (nullptr != pOnePlayer)
					{
						oTankMng.Move(pOnePlayer, oMapp, LEFT_DIRECTION);
					}
				}break;
				case 'w':
				case 'W':
				{
					CTank* pOnePlayer = oMapp.GetTank(ONEPLAYER_WHITE_TANK);
					if (nullptr != pOnePlayer)
					{
						oTankMng.Move(pOnePlayer, oMapp, TOP_DIRECTION);
					}
				}break;
				case 'd':
				case 'D':
				{
					CTank* pOnePlayer = oMapp.GetTank(ONEPLAYER_WHITE_TANK);
					if (nullptr != pOnePlayer)
					{
						oTankMng.Move(pOnePlayer, oMapp, RIGHT_DIRECTION);
					}
				}break;
				case 's':
				case 'S':
				{
					CTank* pOnePlayer = oMapp.GetTank(ONEPLAYER_WHITE_TANK);
					if (nullptr != pOnePlayer)
					{
						oTankMng.Move(pOnePlayer, oMapp, BOTTOM_DIRECTION);
					}
				}break;

				case 'j':
				case 'J':
				{
					CTank* pTwoPlayer = oMapp.GetTank(TWOPLAYER_WHITE_TANK);
					if (nullptr != pTwoPlayer)
					{
						oTankMng.Move(pTwoPlayer, oMapp, LEFT_DIRECTION);
					}
				}break;
				case 'i':
				case 'I':
				{
					CTank* pTwoPlayer = oMapp.GetTank(TWOPLAYER_WHITE_TANK);
					if (nullptr != pTwoPlayer)
					{
						oTankMng.Move(pTwoPlayer, oMapp, TOP_DIRECTION);
					}
				}break;
				case 'l':
				case 'L':
				{
					CTank* pTwoPlayer = oMapp.GetTank(TWOPLAYER_WHITE_TANK);
					if (nullptr != pTwoPlayer)
					{
						oTankMng.Move(pTwoPlayer, oMapp, RIGHT_DIRECTION);
					}
				}break;
				case 'k':
				case 'K':
				{
					CTank* pTwoPlayer = oMapp.GetTank(TWOPLAYER_WHITE_TANK);
					if (nullptr != pTwoPlayer)
					{
						oTankMng.Move(pTwoPlayer, oMapp, BOTTOM_DIRECTION);
					}
				}break;
				case 'q':
				case 'Q':
				{
					CTank* pOnePlayer = oMapp.GetTank(ONEPLAYER_WHITE_TANK);
					if (nullptr != pOnePlayer)
					{
						if (oTankMng.ShotBullet(pOnePlayer, oMapp))
							::PlaySound(_TEXT("Music\\SHOT_BULLET_MUSIC.wav"), NULL, SND_FILENAME | SND_ASYNC);
					}
				}break;
				case 'u':
				case 'U':
				{
					CTank* pTwoPlayer = oMapp.GetTank(TWOPLAYER_WHITE_TANK);
					if (nullptr != pTwoPlayer)
					{
						if (oTankMng.ShotBullet(pTwoPlayer, oMapp))
							::PlaySound(_TEXT("Music\\SHOT_BULLET_MUSIC.wav"), NULL, SND_FILENAME | SND_ASYNC);
					}
				}break;
				case ' ':
				{
					_getch();
				}

				default:
					;
			}
		}

		// �Ƿ�Ҫ����̹��		
		oTankMng.CreatTanks(oMapp);	

		// �Ƿ���Ϸʧ��
		oGameDataMng.GameOver();
		if (m_oGameData.m_bGameOver)
		{
			m_bInterfaceSwitch[6] = true;
			return;
		}

		// ��Ϸͨ�� ,��һ�ؿ�
		if (oGameDataMng.NextSwitchCard())
			return;
	}
}

void CInterface::am_FixedFrame()
{
	// switch
	if (!m_bInterfaceSwitch[4])
		return;

	// ��ͼ���
	for (int i = 0; i <= RIGHT_BULLET_WND + 1; i++)
		WriteChar(i, 20, "��", WHITE);

	for (int i = 0; i <= BOTTOM_BULLET_WND; i++)
		WriteChar(32, i, "��", WHITE);

	//��Ϸ�е������
	WriteChar(37, 1, (char*)m_oGameData.m_strMappFileName[m_oGameData.m_iSwitchCard].c_str(), WHITE);

	WriteChar(37, 3, "����: 0", WHITE);

	WriteChar(37, 6, "���һ�Ĳ�����ʽ: ", WHITE);
	WriteChar(37, 7, "    W ", WHITE);
	WriteChar(42, 8, "�����ӵ�:Q", WHITE);
	WriteChar(37, 8, "   ASD ", WHITE);

	WriteChar(37, 10, "��Ҷ��Ĳ�����ʽ: ", WHITE);
	WriteChar(37, 11, "    I ", WHITE);
	WriteChar(42, 12, "�����ӵ�:U", WHITE);
	WriteChar(37, 12, "   JKL ", WHITE);
}

void CInterface::DIYMapp()
{
	// switch
	if (!m_bInterfaceSwitch[5])
		return;

	//DIYMapp
	system("cls");

	//
	while (true)
	{

	}
}

void CInterface::GameOver()
{
	if (!m_bInterfaceSwitch[6])
		return;

	system("cls");
	WriteChar(6, 6, "ʧ����,'M'��ȥ���˵�,�������˳� ... ", WHITE);
	char ch = _getch();
	if ('m' == ch || 'M' == ch)
	{
		for (int i = 0; i < 10; i++)
		{
			m_bInterfaceSwitch[i] = false;
		}
		m_bInterfaceSwitch[0] = true;
	}
	else
		exit(0);
}
