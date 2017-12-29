#include "stdafx.h"
#include "Base.h"


CBase::CBase(int xPos, int yPos, int iIDFlag, int iLevel, int iDirection)
{
	m_xPos = xPos;
	m_yPos = yPos;
	m_iIDFlag = iIDFlag;
	m_iDirection = iDirection;
	m_iLevel = iLevel;

	InitAttributes();
}


CBase::~CBase()
{
}



void CBase::MoveToLeft()
{
	m_xPos--;
	m_iDirection = 0;
}


void CBase::MoveToTop()
{
	m_yPos--;
	m_iDirection = 1;
}


void CBase::MoveToRight()
{
	m_xPos++;
	m_iDirection = 2;
}


void CBase::MoveToBottom()
{
	m_yPos++;
	m_iDirection = 3;
}


void CBase::InitAttributes()
{
	//m_iColor ***********************************************
	switch (m_iIDFlag)
	{
		case MYSELF_WHITE_TANK:
		{
			m_iColor = WHITE;
		}break;
		case FRIEND_WITHE_TANK:
		{
			m_iColor = WHITE;
		}break;
		case ENEMY_RED_TANK:
		{
			m_iColor = RED;
		}break;
		case ENEMY_YELLOW_TANK:
		{
			m_iColor = YELLOW;
		}break;
		case ENEMY_GREEN_TANK:
		{
			m_iColor = GREEN;
		}break;
		case MYSELF_WHITE_BULLET:
		case FRIEND_WITHE_BULLET:
		case ENEMY_RED_BULLET:
		case ENEMY_YELLOW_BULLET:
		case ENEMY_GREEN_BULLET:
		{
			m_iColor = GREEN;
		}break;
		case STEEL_WHITE_BRICK:
		{
			m_iColor = WHITE;
		}break;
		case REVER_BLUE_BRICK:
		{
			m_iColor = BLUE;
		}break;
		case NORMAL_PURPLE_BRICK:
		{
			m_iColor = PURPLE;
		}break;
		case THE_RED_KING:
		{
			m_iColor = RED;
		}break;
		default:
		{
			assert(0);
			break;
		}
	}

	// m_iBlood && m_iForceValue && m_iMoveSpeed ****************************
	switch (m_iLevel)
	{
		case LEVEL0:
		{
			m_iBlood = 1;
			m_iForceValue = 1;
		}break;
		case LEVEL1:
		{
			m_iBlood = 2;
			m_iForceValue = 2;
		}break;
		case LEVEL2:
		{
			m_iBlood = 3;
			m_iForceValue = 3;
		}break;
		default:
		{
			assert(0);
			break;
		}
	}
}


//»æÖÆÍ¼Ïñ
void CBase::DrawMyself()
{
}


//Çå³ýÍ¼Ïñ
void CBase::ClearMyself()
{
}