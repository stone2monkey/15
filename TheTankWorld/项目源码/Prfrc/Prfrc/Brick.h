#pragma once
#include "Base.h"
class CBrick :
	public CBase
{
public:
	CBrick(int xPos, int yPos, int iID, int iLevel = 1);
	~CBrick();
};

