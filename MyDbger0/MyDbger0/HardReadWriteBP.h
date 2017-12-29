#pragma once
#include "_MyCustomType.h"


class CHardReadWriteBP
{
	// Attributions ***************************************************************************************
public:
	MY_CONDITION_HRW_BP m_ConditionBP{};





	// Methods ********************************************************************************************
public:
	//****************************************************
	// Name: CHardWriteBP::AddBP
	// Func: 添加断点
	// Args: DWORD dwTID
	// Args: DWORD dwAddr
	// Retn: int
	// return 0; 成功
	// return 1; 断点存在
	// return 2; 断点满了
	//****************************************************
	int AddBP(DWORD dwTID, DWORD dwAddr);
	BOOL RemoveBP(DWORD dwTID, DWORD dwAddr);

	void AddConditionBP(DWORD dwTID, DWORD dwAddr, DWORD dwData);



public:
	CHardReadWriteBP();
	~CHardReadWriteBP();
};

