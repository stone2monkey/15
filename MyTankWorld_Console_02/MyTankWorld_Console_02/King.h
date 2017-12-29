#pragma once
#include "Base.h"
class CKing :
	public CBase
{
public:
	CKing(int xPos, int yPos, int iIDFlag, int iLevel, int iDirection = 1) : CBase(xPos, yPos, iIDFlag, iLevel, iDirection) {}
	~CKing();

	void DrawMyself();
	void ClearMyself();
	void DeleteMyself();
};

