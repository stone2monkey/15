#pragma once
#include <vector>
using std::vector;



class CHardExecuteBP
{
	// Attributions ***************************************************************************************
public:
	


	// Methods ********************************************************************************************
public:
	//****************************************************
	// Name: CHardExecuteBP::AddBP
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

	void RepairHardExecuteCoed(DWORD dwTID, DWORD dwIndex);









public:
	CHardExecuteBP();
	~CHardExecuteBP();
};

