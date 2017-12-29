// Prfrc.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Interface.h"
#pragma comment(lib,"User32.lib")
#pragma comment(lib, "winmm.lib")

int main()
{
	CInterface oInterface;
	oInterface.SetWindow();

	while (true)
	{
		oInterface.Welcome();
		oInterface.SelectMapp();
		oInterface.OnePlayer();
		oInterface.Gaming(oInterface.m_oGameData.m_strMappFileName[oInterface.m_oGameData.m_iSwitchCard], 
			oInterface.m_oGameData.m_bOnePlayer);
		oInterface.DIYMapp();
		oInterface.GameOver();
	}

    return 0;
}

