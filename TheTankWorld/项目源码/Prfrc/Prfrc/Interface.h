#pragma once
#include "Mapp.h"
#include "GameData.h"
#include "DrawGaming.h"
#include "TankMng.h"
#include "BulletMng.h"
#include "GameDataMng.h"
//#include "Music.h"

class CInterface
{
public:
	CInterface();
	~CInterface();


	//Attributions ***********************************************************************************

	// m_bInterfaceSwitch[0]	Welcome
	// m_bInterfaceSwitch[1]	SelectMapp
	// m_bInterfaceSwitch[2]	OnePlayer
	// ---------------------
	// m_bInterfaceSwitch[3]	Gaming
	// m_bInterfaceSwitch[4]	am_FixedFrame
	// m_bInterfaceSwitch[5]	DIYMapp
	// m_bInterfaceSwitch[6]	GameOver
	// ---------------------
	// m_bInterfaceSwitch[7]	
	// m_bInterfaceSwitch[8]
	// m_bInterfaceSwitch[9]
	bool m_bInterfaceSwitch[10]{};

	CGameData m_oGameData;
	/*CMusic m_oMusic;*/



	//Method *****************************************************************************************

	void WriteChar(int x, int y, char* pszChar, WORD wColor);

	void SetWindow();

	void Welcome();
	void SelectMapp();
	void OnePlayer();
	// ---------------
	void Gaming(string strMappFileName, bool bOnePlayer = false);
	void am_FixedFrame();
	// ---------------
	void DIYMapp();
	void GameOver();
};

