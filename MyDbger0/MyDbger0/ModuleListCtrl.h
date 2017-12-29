#pragma once
#include "afxcmn.h"
class CModuleListCtrl :
	public CListCtrl
{
public:
	CModuleListCtrl();
	~CModuleListCtrl();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void PreSubclassWindow();
};

