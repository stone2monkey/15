#pragma once
#include "afxcmn.h"
class CMemListCtrl :
	public CListCtrl
{
	// Methods ********************************************************************************************
public:
	//****************************************************
	// Name: CMemListCtrl::GetMemData
	// Func: �õ�ָ�����̵�ַ����,15 * 16 ���ֽ�����
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

