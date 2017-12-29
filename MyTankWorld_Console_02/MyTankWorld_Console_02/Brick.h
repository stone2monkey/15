#pragma once
#include "Base.h"
class CBrick :
	public CBase
{
public:
	CBrick(int xPos, int yPos, int iIDFlag, int iLevel, int iDirection = 1) : CBase(xPos, yPos, iIDFlag, iLevel, iDirection) {}
	~CBrick();

	void DrawMyself();
	void ClearMyself();
};

