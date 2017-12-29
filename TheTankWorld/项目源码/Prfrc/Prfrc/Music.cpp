#include "stdafx.h"
#include <cassert>
#include "Music.h"


CMusic::CMusic()
{
}


CMusic::~CMusic()
{
}

void CMusic::PlaySound(int iMusicID, DWORD fdwSound)
{
	if (!m_bMusic[iMusicID])
		return;

	LPCWSTR pszSound;
	switch (iMusicID)
	{
		case BACKGROUND_MUSIC:
			pszSound = _T("Music\\BACKGROUND_MUSIC.mp3");
			break;
		case START_MUSIC:
			pszSound = _T("Music\\START_MUSIC.wav");
			break;
		case SHOT_BULLET_MUSIC:
			pszSound = _T("Music\\SHOT_BULLET_MUSIC.wav");
			break;
		case TANK_DIETH_MUSIC:
			pszSound = _T("Music\\TANK_DIETH_MUSIC.wav");
			break;
		case GAME_OVER_MUSIC:
			pszSound = _T("Music\\GAME_OVER_MUSIC.wav");
			break;
		default:
			assert(0);
	}

	::PlaySoundW(pszSound, NULL, fdwSound);
}

