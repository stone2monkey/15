#pragma once
#include "afxcmn.h"
class CStackListCtrl :
	public CListCtrl
{
	// Methods ********************************************************************************************
public:
	void GetStackData();




public:
	CStackListCtrl();
	~CStackListCtrl();
	virtual void PreSubclassWindow();
};

