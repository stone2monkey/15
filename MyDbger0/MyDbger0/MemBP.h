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
	// Func: ����ڴ�ϵ�
	// Args: DWORD dwPID
	// Args: DWORD dwAddr	��ַ
	// Args: DWORD dwType	����(0 1 8)
	// Retn: int
	// return 0; �ɹ�
	// return 1; �ϵ�����(�ڴ�ϵ�ֻ��һ��) 
	//****************************************************
	int AddBP(DWORD dwPID, DWORD dwAddr, DWORD dwType);
	//****************************************************
	// Name: CMemBP::RemoveBP
	// Func: 
	// Retn: int
	// return 0; �ɹ�
	// return 1; û�жϵ�
	//****************************************************
	int RemoveBP();
	void ResumeMemProtection();
	void AlterMemProtection();













public:
	CMemBP();
	~CMemBP();
};

