#include "stdafx.h"
#include "King.h"


CKing::~CKing()
{
}


void CKing::DrawMyself()
{
	WriteChar(m_xPos, m_yPos, "Íõ", m_iColor);
}


void CKing::ClearMyself()
{
	WriteChar(m_xPos, m_yPos, "  ", m_iColor);
}


void CKing::DeleteMyself()
{
	delete this;
}