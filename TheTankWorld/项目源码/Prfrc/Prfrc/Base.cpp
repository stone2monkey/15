#include "stdafx.h"
#include "Base.h"


CBase::CBase(int xPos, int yPos, int iID, int iLevel)
{
	Init(xPos, yPos, iID, iLevel);
}


CBase::~CBase()
{
}

void CBase::Init(int xPos, int yPos, int iID, int iLevel)
{
	// 1. validity of params
	if (xPos < LEFT_BULLET_WND || xPos > RIGHT_BULLET_WND)
		assert(0);
	if (yPos < TOP_BULLET_WND || yPos> BOTTOM_BULLET_WND)
		assert(0);
	if (iID > THE_RED_KING || iID < ONEPLAYER_WHITE_TANK)
		assert(0);
	if (iLevel > LEVEL3 || iLevel < LEVEL1)
		assert(0);

	m_xPos = xPos;
	m_yPos = yPos;
	m_iID = iID;
	m_iLevel = iLevel;

	//m_iColor
	switch (m_iID)
	{
		case ONEPLAYER_WHITE_TANK:
			m_iColor = WHITE;
			break;
		case TWOPLAYER_WHITE_TANK:
			m_iColor = WHITE;
			break;
		case ENEMY_RED_TANK:
			m_iColor = RED;
			break;
		case ENEMY_YELLOW_TANK:
			m_iColor = YELLOW;
			break;
		case ENEMY_GREEN_TANK:
			m_iColor = GREEN;
			break;
			// ------------------------------
		case ONEPLAYER_WHITE_BULLET:
			m_iColor = WHITE;
			break;
		case TWOPLAYER_WHITE_BULLET:
			m_iColor = WHITE;
			break;
		case ENEMY_RED_BULLET:
			m_iColor = RED;
			break;
		case ENEMY_YELLOW_BULLET:
			m_iColor = YELLOW;
			break;
		case ENEMY_GREEN_BULLET:
			m_iColor = GREEN;
			break;
			// ------------------------------
		case STEEL_WHITE_BRICK:
			m_iColor = WHITE;
			break;
		case REVER_BLUE_BRICK:
			m_iColor = BLUE;
			break;
		case NORMAL_PURPLE_BRICK:
			m_iColor = PURPLE;
			break;
			// ------------------------------
		case THE_RED_KING:
			m_iColor = RED;
			break;
			// ------------------------------
		default:
			assert(0);
	}

	//m_iBlood
	switch (m_iLevel)
	{
		case LEVEL1:
			m_iBlood = 1;
			break;
		case LEVEL2:
			m_iBlood = 2;
			break;
		case LEVEL3:
			m_iBlood = 3;
			break;
		default:
			assert(0);
	}
}
