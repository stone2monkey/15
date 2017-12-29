#include "stdafx.h"
#include "ModuleListCtrl.h"


CModuleListCtrl::CModuleListCtrl()
{
}


CModuleListCtrl::~CModuleListCtrl()
{
}


BOOL CModuleListCtrl::PreTranslateMessage(MSG* pMsg)
{


	return CListCtrl::PreTranslateMessage(pMsg);
}


void CModuleListCtrl::PreSubclassWindow()
{

	SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	InsertColumn(0, L"ModName", LVCFMT_LEFT, 200);
	InsertColumn(1, L"ModBase", LVCFMT_LEFT, 200);
	InsertColumn(2, L"ModSize", LVCFMT_LEFT, 200);
	InsertColumn(3, L"CodeBase", LVCFMT_LEFT, 200);
	InsertColumn(4, L"CodeSize", LVCFMT_LEFT, 200);
	InsertColumn(5, L"OEP", LVCFMT_LEFT, 200);
	InsertColumn(6, L"FullName", LVCFMT_LEFT, 600);

	CListCtrl::PreSubclassWindow();
}
