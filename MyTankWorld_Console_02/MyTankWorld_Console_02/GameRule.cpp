#include "stdafx.h"
#include "GameRule.h"


CGameRule::CGameRule()
{
	m_bInterfaceSelect[0][0] = true;
	m_bInterfaceSelect[0][1] = false;
	m_bInterfaceSelect[0][2] = false;

	m_bOnePlayer = false;
}


CGameRule::~CGameRule()
{
}
