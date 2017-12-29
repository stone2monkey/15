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
	// œÏ”¶PAGE_UP PAGE_DOWN …œœ¬∑≠“≥
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

