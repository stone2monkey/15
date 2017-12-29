#pragma once
#include <windows.h>

#define BACKGROUND_MUSIC 0
#define START_MUSIC 1
#define SHOT_BULLET_MUSIC 2
#define TANK_DIETH_MUSIC 3
#define GAME_OVER_MUSIC 4

/*
::PlaySound����̫��,�Ͳ��������������
*/

class CMusic
{
public:
	CMusic();
	~CMusic();


	//Attributions ***********************************************************************************

	// m_bMusic[0]		background music
	// m_bMusic[1]		start music
	// m_bMusic[2]		shot bullet music
	// m_bMusic[3]		tank died music	
	// m_bMusic[4]		game over music

	bool m_bMusic[5]{};


	//Method *****************************************************************************************

	void PlaySound(int iMusicID, DWORD fdwSound = SND_FILENAME | SND_ASYNC);	//��playsound�ķ�װ

};

