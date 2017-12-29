#pragma once
#include "_CGlobal.h"

class CBase
{
public:
	CBase(int xPos, int yPos, int iIDFlag, int iLevel, int iDirection = 1);
	~CBase();

	int m_xPos;
	int m_yPos;

	int m_iIDFlag;
	int m_iDirection;
	int m_iColor;

	int m_iLevel;
	int m_iBlood;
	int m_iForceValue;

	void MoveToLeft();
	void MoveToTop();
	void MoveToRight();
	void MoveToBottom();
	void InitAttributes();

	virtual void DrawMyself();
	virtual void ClearMyself();
};

