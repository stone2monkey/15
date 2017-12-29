#include "stdafx.h"
#include "DisasmListCtrl.h"
#include "Interface.h"
#include "_CGlobal.h"


void CDisasmListCtrl::GetDisasmData(DWORD dwAddr)
{
	// 0. ************************************************************
	// 1. ************************************************************
	// 2. ************************************************************
	// 3. ************************************************************
	// 4. ************************************************************
	// 5. ************************************************************
	// 6. ************************************************************
	// 7. ************************************************************
	// 8. ************************************************************
}

CDisasmListCtrl::CDisasmListCtrl()
{
}


CDisasmListCtrl::~CDisasmListCtrl()
{
}


void CDisasmListCtrl::PreSubclassWindow()
{
	SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);



	InsertColumn(0, L"Addr", LVCFMT_LEFT, 120);
	InsertColumn(1, L"OpCode", LVCFMT_LEFT, 200);
	InsertColumn(2, L"Original disasm", LVCFMT_LEFT, 400);
	InsertColumn(3, L"Disasm symboled", LVCFMT_LEFT, 400);
	InsertColumn(4, L"Comment", LVCFMT_LEFT, 500);

	int iItemCountEveryPage = 37;
	for (int i = 0; i < iItemCountEveryPage; i++)
	{
		InsertItem(i, L"");
	}

	CListCtrl::PreSubclassWindow();
}
BEGIN_MESSAGE_MAP(CDisasmListCtrl, CListCtrl)
END_MESSAGE_MAP()


BOOL CDisasmListCtrl::PreTranslateMessage(MSG* pMsg)
{
	// �Ϸ�ҳ
	if (WM_KEYDOWN == pMsg->message && VK_PRIOR == pMsg->wParam)
	{
		//// �����ǰ��������ǰ��37��,�����Ϸ�ҳ,��ֻ�ܰ�������0
		//if (0 <= g_dwIndexOfDisasmList && 36 >= g_dwIndexOfDisasmList)
		//{
		//	if (0 == g_dwIndexOfDisasmList)
		//	{
		//		return CListCtrl::PreTranslateMessage(pMsg);;
		//	}

		//	g_dwIndexOfDisasmList = 0;
		//	CInterface::DisasmListPageChanged(g_vctDisasmListItemInfo.at(0)->dwAddr, 0);
		//	return CListCtrl::PreTranslateMessage(pMsg);;
		//}
		g_dwIndexOfDisasmList -= 37;
		if ((int)g_dwIndexOfDisasmList < 0) { g_dwIndexOfDisasmList = 0; }

		CInterface::DisasmListPageChanged(g_vctDisasmListItemInfo.at(g_dwIndexOfDisasmList)->dwAddr, 0);
	}
	// �·�ҳ
	else if (WM_KEYDOWN == pMsg->message && VK_NEXT == pMsg->wParam)
	{
		//// �����ǰ������������37��,�����·�ҳ,��ֻ�ܰ�������ΪMax - 37 - 1;
		//if (g_vctDisasmListItemInfo.size() - 1 - 37 <= g_dwIndexOfDisasmList && g_vctDisasmListItemInfo.size() - 1 >= g_dwIndexOfDisasmList)
		//{
		//	if (g_vctDisasmListItemInfo.size() - 1 - 37 == g_dwIndexOfDisasmList)
		//	{
		//		return CListCtrl::PreTranslateMessage(pMsg);;
		//	}
		//	g_dwIndexOfDisasmList = g_vctDisasmListItemInfo.size() - 1 - 37;
		//	CInterface::DisasmListPageChanged(g_vctDisasmListItemInfo.at(g_dwIndexOfDisasmList)->dwAddr, 0);
		//	return CListCtrl::PreTranslateMessage(pMsg);;
		//}
		g_dwIndexOfDisasmList += 37;
		if (g_dwIndexOfDisasmList > (g_vctDisasmListItemInfo.size()-37)) { g_dwIndexOfDisasmList = g_vctDisasmListItemInfo.size() - 37; }

		CInterface::DisasmListPageChanged(g_vctDisasmListItemInfo.at(g_dwIndexOfDisasmList)->dwAddr, 0);
	}

	return CListCtrl::PreTranslateMessage(pMsg);
}
