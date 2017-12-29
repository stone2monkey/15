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

	void RepairHardExecuteCoed(DWORD dwTID, DWORD dwIndex);









public:
	CHardExecuteBP();
	~CHardExecuteBP();
};

