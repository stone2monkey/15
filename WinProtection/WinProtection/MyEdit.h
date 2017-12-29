#pragma once
#include "afxwin.h"
class CMyEdit :
	public CEdit
{
	// Attributions ***************************************************************************************
public:
	CStringW m_wcsFolderSelected{};
	CStringW m_wcsPostfix{};
	CStringW m_wcsFileSelected{};


	
public:
	CMyEdit();
	~CMyEdit();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

