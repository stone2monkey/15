#pragma once
#include "afxcmn.h"
class CDisasmListCtrl :
	public CListCtrl
{
	// Methods ********************************************************************************************
public:
	void GetDisasmData(DWORD dwAddr);

public:
	CDisasmListCtrl();
	~CDisasmListCtrl();
	virtual void PreSubclassWindow();
	DECLARE_MESSAGE_MAP()
	// ��ӦPAGE_UP PAGE_DOWN ���·�ҳ
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

