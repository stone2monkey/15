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
	// Func: ��Ӷϵ�
	// Args: DWORD dwTID
	// Args: DWORD dwAddr
	// Retn: int
	// return 0; �ɹ�
	// return 1; �ϵ����
	// return 2; �ϵ�����
	//****************************************************
	int AddBP(DWORD dwTID, DWORD dwAddr);
	BOOL RemoveBP(DWORD dwTID, DWORD dwAddr);

	void AddConditionBP(DWORD dwTID, DWORD dwAddr, DWORD dwData);



public:
	CHardReadWriteBP();
	~CHardReadWriteBP();
};

