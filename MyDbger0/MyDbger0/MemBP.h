#pragma once
#include <vector>
#include "_MyCustomType.h"
using std::vector;


class CMemBP
{
	// Attributions ***************************************************************************************
public:
	MY_MEM_BP m_MyMemBP{};
	HANDLE m_hProc{};



	// Methods ******************************************************************************************** 
public:
	//****************************************************
	// Name: CMemBP::AddBP
	// Func: 添加内存断点
	// Args: DWORD dwPID
	// Args: DWORD dwAddr	地址
	// Args: DWORD dwType	类型(0 1 8)
	// Retn: int
	// return 0; 成功
	// return 1; 断点满了(内存断点只有一个) 
	//****************************************************
	int AddBP(DWORD dwPID, DWORD dwAddr, DWORD dwType);
	//****************************************************
	// Name: CMemBP::RemoveBP
	// Func: 
	// Retn: int
	// return 0; 成功
	// return 1; 没有断点
	//****************************************************
	int RemoveBP();
	void ResumeMemProtection();
	void AlterMemProtection();













public:
	CMemBP();
	~CMemBP();
};

