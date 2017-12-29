#pragma once
#include "afxcmn.h"
class CMemListCtrl :
	public CListCtrl
{
	// Methods ********************************************************************************************
public:
	//****************************************************
	// Name: CMemListCtrl::GetMemData
	// Func: 得到指定进程地址处起,15 * 16 个字节数据
	// Args: DWORD dwAddr
	// Args: DWORD dwPID
	// Retn: void
	//****************************************************
	void GetMemData(DWORD dwAddr, DWORD dwPID);





public:
	CMemListCtrl();
	~CMemListCtrl();
	virtual void PreSubclassWindow();
};

