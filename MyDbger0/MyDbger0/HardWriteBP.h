#pragma once


class CHardWriteBP
{
	// Attributions ***************************************************************************************
public:



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











public:
	CHardWriteBP();
	~CHardWriteBP();
};

