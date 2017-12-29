#pragma once
#include "_Global.h"

class CBase
{
public:
	CBase(int xPos, int yPos, int iID, int iLevel);
	~CBase();



	//Attributions ***********************************************************************************
	int m_xPos;
	int m_yPos;

	int m_iID;
	int m_iColor;

	int m_iLevel;
	int m_iBlood;

	//Method *****************************************************************************************


	void Init(int xPos, int yPos, int iID, int iLevel);
};

