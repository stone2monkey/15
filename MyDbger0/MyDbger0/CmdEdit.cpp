#include "stdafx.h"
#include "CmdEdit.h"
#include "_CGlobal.h"
#include "MyDbger0Dlg.h"
#include "HardWriteBP.h"
#include "MyDbger0Dlg.h"
#include "Resource.h"
#include "HardReadWriteBP.h"
#include "MemBP.h"


void CCmdEdit::OnSeekSyms()
{
	CStringW wcsTemp{};

	DWORD iLoop = g_vctWcsSymbol.size();
	for (DWORD i = 0; i < iLoop; i++)
	{
		wcsTemp.AppendFormat(L"%-11s%s\r\n", g_vctWcsSymbol.at(i), g_vctWcsAddrOfSymbol.at(i));
	}

	g_pMyDbger0Dlg->SetDlgItemTextW(IDEDT_LOG, wcsTemp);
}

void CCmdEdit::OnSeekMem()
{
	// 得到字符串
	CStringW wcsTemp{};
	GetWindowText(wcsTemp);

	// 得到地址
	DWORD dwLen = wcslen(L"seek mem 0x");
	wcsTemp = wcsTemp.Mid(dwLen);
	DWORD dwAddr{};
	WCHAR* pwcEnd = L"\0";
	dwAddr = wcstol(wcsTemp, &pwcEnd, 16);

	// 
	g_pMyDbger0Dlg->m_lcMem.GetMemData(dwAddr, g_pProcDbgedInfo->dwPID);
}

void CCmdEdit::OnSetHardWriteBP()
{
	// 得到字符串
	CStringW wcsTemp{};
	GetWindowText(wcsTemp);

	// 得到地址
	DWORD dwLen = wcslen(L"hw bp 0x");
	wcsTemp = wcsTemp.Mid(dwLen);
	DWORD dwAddr{};
	WCHAR* pwcEnd = L"\0";
	dwAddr = wcstol(wcsTemp, &pwcEnd, 16);

	// 
	CHardReadWriteBP o{};
	if (0 == o.AddBP(g_pProcDbgedInfo->dwTID, dwAddr))
	{
		wcsTemp.Format(L"添加硬件写断点:0x%08x地址处断点成功!", dwAddr);
		::MessageBoxW(g_pMyDbger0Dlg->m_hWnd, wcsTemp, L"", 0);
	}
	else if (1 == o.AddBP(g_pProcDbgedInfo->dwTID, dwAddr))
	{
		if (o.RemoveBP(g_pProcDbgedInfo->dwTID, dwAddr))
		{
			wcsTemp.Format(L"删除硬件写断点:0x%08x地址处断点成功!", dwAddr);
			::MessageBoxW(g_pMyDbger0Dlg->m_hWnd, wcsTemp, L"", 0);
		}
		else { assert(0); }
	}
	else if (2 == o.AddBP(g_pProcDbgedInfo->dwTID, dwAddr))
	{
		wcsTemp.Format(L"硬件断点满了,不能添加,地址处也没有硬件断点可删:0x%08x!", dwAddr);
		::MessageBoxW(g_pMyDbger0Dlg->m_hWnd, wcsTemp, L"", 0);
	}	
}

void CCmdEdit::OnSetHardReadWriteBP()
{
	// 得到字符串
	CStringW wcsTemp{};
	GetWindowText(wcsTemp);

	// 得到地址
	DWORD dwLen = wcslen(L"hrw bp 0x");
	wcsTemp = wcsTemp.Mid(dwLen);
	DWORD dwAddr{};
	WCHAR* pwcEnd = L"\0";
	dwAddr = wcstol(wcsTemp, &pwcEnd, 16);

	// 
	CHardWriteBP o{};
	if (0 == o.AddBP(g_pProcDbgedInfo->dwTID, dwAddr))
	{
		wcsTemp.Format(L"添加硬件读写断点:0x%08x地址处断点成功!", dwAddr);
		::MessageBoxW(g_pMyDbger0Dlg->m_hWnd, wcsTemp, L"", 0);
	}
	else if (1 == o.AddBP(g_pProcDbgedInfo->dwTID, dwAddr))
	{
		if (o.RemoveBP(g_pProcDbgedInfo->dwTID, dwAddr))
		{
			wcsTemp.Format(L"删除硬件读写断点:0x%08x地址处断点成功!", dwAddr);
			::MessageBoxW(g_pMyDbger0Dlg->m_hWnd, wcsTemp, L"", 0);
		}
		else { assert(0); }
	}
	else if (2 == o.AddBP(g_pProcDbgedInfo->dwTID, dwAddr))
	{
		wcsTemp.Format(L"硬件断点满了,不能添加,地址处也没有硬件断点可删:0x%08x!", dwAddr);
		::MessageBoxW(g_pMyDbger0Dlg->m_hWnd, wcsTemp, L"", 0);
	}
}

void CCmdEdit::OnSetConditionHardReadWriteBP()
{
	// 得到字符串
	CStringW wcsTemp{};
	GetWindowText(wcsTemp);

	// 得到地址
	DWORD dwLen = wcslen(L"hcrw bp 0x");
	wcsTemp = wcsTemp.Mid(dwLen);
	DWORD dwAddr{};
	WCHAR* pwcEnd = L"\x20";
	dwAddr = wcstol(wcsTemp, &pwcEnd, 16);

	// 得到条件(就是一个4字节数据)
	GetWindowText(wcsTemp);
	wcsTemp = wcsTemp.Mid(19);
	DWORD dwData{};
	pwcEnd = L"\0";
	dwData = wcstol(wcsTemp, &pwcEnd, 16);

	//
	g_pHardReadWriteBP->AddConditionBP(g_pProcDbgedInfo->dwTID, dwAddr, dwData);
	wcsTemp.Format(L"条件硬件读写断点: [[0x%08x] == 0x%08x] 添加成功!", dwAddr, dwData);
	::MessageBoxW(g_pMyDbger0Dlg->m_hWnd, wcsTemp, L"", 0);
}

void CCmdEdit::OnSetMemBP()
{
	// 得到字符串
	CStringW wcsTemp{};
	GetWindowText(wcsTemp);

	// 得到地址
	DWORD dwLen = wcslen(L"mem bp 0x");
	wcsTemp = wcsTemp.Mid(dwLen, 8);
	DWORD dwAddr{};
	WCHAR* pwcEnd = L"\x20";
	dwAddr = wcstol(wcsTemp, &pwcEnd, 16);

	// 得到类型
	GetWindowText(wcsTemp);
	wcsTemp = wcsTemp.Mid(dwLen + 9, 1);
	DWORD dwType{};
	pwcEnd = L"\0";
	dwType = wcstol(wcsTemp, &pwcEnd, 16);

	// 
	int iRes = g_pMemBP->AddBP(g_pProcDbgedInfo->dwPID, dwAddr, dwType);
	if (0 == iRes)
	{
		wcsTemp.Format(L"软件断点添加成功[地址:0x%08x] [类型:%d]!", dwAddr, dwType);
		::MessageBoxW(g_pMyDbger0Dlg->m_hWnd, wcsTemp, L"", 0);
	}
	else
	{
		g_pMemBP->RemoveBP();
		wcsTemp.Format(L"没有软件断点了!", dwAddr, dwType);
		::MessageBoxW(g_pMyDbger0Dlg->m_hWnd, wcsTemp, L"", 0);
	}
}

CCmdEdit::CCmdEdit()
{
}


CCmdEdit::~CCmdEdit()
{
}


BOOL CCmdEdit::PreTranslateMessage(MSG* pMsg)
{
	// enter
	if (WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam)
	{	
		CStringW wcsTemp{};
		GetWindowText(wcsTemp);
		wcsTemp.MakeLower();

		if (-1 != wcsTemp.Find(L"seek syms")) { OnSeekSyms(); }	// seek syms
		if (-1 != wcsTemp.Find(L"hw bp 0x")) { OnSetHardWriteBP(); }	// hw bp 0x********
		if (-1 != wcsTemp.Find(L"hrw bp 0x")) { OnSetHardReadWriteBP(); }	// hrw bp 0x********
		if (-1 != wcsTemp.Find(L"hcrw bp 0x")) { OnSetConditionHardReadWriteBP(); }	// hcrw bp 0x******** ****
		if (-1 != wcsTemp.Find(L"mem bp 0x")) { OnSetMemBP(); }	
		if (-1 != wcsTemp.Find(L"seek mem 0x")) { OnSeekMem(); }	// seek mem 0x********

		SetWindowTextW(L"");
	}

	return CEdit::PreTranslateMessage(pMsg);
}
