#include "stdafx.h"
#include "Brick.h"


CBrick::~CBrick()
{
}


void CBrick::DrawMyself()
{
	if (STEEL_WHITE_BRICK  == m_iIDFlag)
		WriteChar(m_xPos, m_yPos, "¡ö", m_iColor);
	else if (STEEL_WHITE_BRICK == m_iIDFlag)
		WriteChar(m_xPos, m_yPos, "¡Ö", m_iColor);
	else if (STEEL_WHITE_BRICK == m_iIDFlag)
		WriteChar(m_xPos, m_yPos, "¨ˆ", m_iColor);
}


void CBrick::ClearMyself()
{
	WriteChar(m_xPos, m_yPos, "  ", m_iColor);
}