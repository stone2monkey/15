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
	// �õ��ַ���
	CStringW wcsTemp{};
	GetWindowText(wcsTemp);

	// �õ���ַ
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
	// �õ��ַ���
	CStringW wcsTemp{};
	GetWindowText(wcsTemp);

	// �õ���ַ
	DWORD dwLen = wcslen(L"hw bp 0x");
	wcsTemp = wcsTemp.Mid(dwLen);
	DWORD dwAddr{};
	WCHAR* pwcEnd = L"\0";
	dwAddr = wcstol(wcsTemp, &pwcEnd, 16);

	// 
	CHardReadWriteBP o{};
	if (0 == o.AddBP(g_pProcDbgedInfo->dwTID, dwAddr))
	{
		wcsTemp.Format(L"���Ӳ��д�ϵ�:0x%08x��ַ���ϵ�ɹ�!", dwAddr);
		::MessageBoxW(g_pMyDbger0Dlg->m_hWnd, wcsTemp, L"", 0);
	}
	else if (1 == o.AddBP(g_pProcDbgedInfo->dwTID, dwAddr))
	{
		if (o.RemoveBP(g_pProcDbgedInfo->dwTID, dwAddr))
		{
			wcsTemp.Format(L"ɾ��Ӳ��д�ϵ�:0x%08x��ַ���ϵ�ɹ�!", dwAddr);
			::MessageBoxW(g_pMyDbger0Dlg->m_hWnd, wcsTemp, L"", 0);
		}
		else { assert(0); }
	}
	else if (2 == o.AddBP(g_pProcDbgedInfo->dwTID, dwAddr))
	{
		wcsTemp.Format(L"Ӳ���ϵ�����,�������,��ַ��Ҳû��Ӳ���ϵ��ɾ:0x%08x!", dwAddr);
		::MessageBoxW(g_pMyDbger0Dlg->m_hWnd, wcsTemp, L"", 0);
	}	
}

void CCmdEdit::OnSetHardReadWriteBP()
{
	// �õ��ַ���
	CStringW wcsTemp{};
	GetWindowText(wcsTemp);

	// �õ���ַ
	DWORD dwLen = wcslen(L"hrw bp 0x");
	wcsTemp = wcsTemp.Mid(dwLen);
	DWORD dwAddr{};
	WCHAR* pwcEnd = L"\0";
	dwAddr = wcstol(wcsTemp, &pwcEnd, 16);

	// 
	CHardWriteBP o{};
	if (0 == o.AddBP(g_pProcDbgedInfo->dwTID, dwAddr))
	{
		wcsTemp.Format(L"���Ӳ����д�ϵ�:0x%08x��ַ���ϵ�ɹ�!", dwAddr);
		::MessageBoxW(g_pMyDbger0Dlg->m_hWnd, wcsTemp, L"", 0);
	}
	else if (1 == o.AddBP(g_pProcDbgedInfo->dwTID, dwAddr))
	{
		if (o.RemoveBP(g_pProcDbgedInfo->dwTID, dwAddr))
		{
			wcsTemp.Format(L"ɾ��Ӳ����д�ϵ�:0x%08x��ַ���ϵ�ɹ�!", dwAddr);
			::MessageBoxW(g_pMyDbger0Dlg->m_hWnd, wcsTemp, L"", 0);
		}
		else { assert(0); }
	}
	else if (2 == o.AddBP(g_pProcDbgedInfo->dwTID, dwAddr))
	{
		wcsTemp.Format(L"Ӳ���ϵ�����,�������,��ַ��Ҳû��Ӳ���ϵ��ɾ:0x%08x!", dwAddr);
		::MessageBoxW(g_pMyDbger0Dlg->m_hWnd, wcsTemp, L"", 0);
	}
}

void CCmdEdit::OnSetConditionHardReadWriteBP()
{
	// �õ��ַ���
	CStringW wcsTemp{};
	GetWindowText(wcsTemp);

	// �õ���ַ
	DWORD dwLen = wcslen(L"hcrw bp 0x");
	wcsTemp = wcsTemp.Mid(dwLen);
	DWORD dwAddr{};
	WCHAR* pwcEnd = L"\x20";
	dwAddr = wcstol(wcsTemp, &pwcEnd, 16);

	// �õ�����(����һ��4�ֽ�����)
	GetWindowText(wcsTemp);
	wcsTemp = wcsTemp.Mid(19);
	DWORD dwData{};
	pwcEnd = L"\0";
	dwData = wcstol(wcsTemp, &pwcEnd, 16);

	//
	g_pHardReadWriteBP->AddConditionBP(g_pProcDbgedInfo->dwTID, dwAddr, dwData);
	wcsTemp.Format(L"����Ӳ����д�ϵ�: [[0x%08x] == 0x%08x] ��ӳɹ�!", dwAddr, dwData);
	::MessageBoxW(g_pMyDbger0Dlg->m_hWnd, wcsTemp, L"", 0);
}

void CCmdEdit::OnSetMemBP()
{
	// �õ��ַ���
	CStringW wcsTemp{};
	GetWindowText(wcsTemp);

	// �õ���ַ
	DWORD dwLen = wcslen(L"mem bp 0x");
	wcsTemp = wcsTemp.Mid(dwLen, 8);
	DWORD dwAddr{};
	WCHAR* pwcEnd = L"\x20";
	dwAddr = wcstol(wcsTemp, &pwcEnd, 16);

	// �õ�����
	GetWindowText(wcsTemp);
	wcsTemp = wcsTemp.Mid(dwLen + 9, 1);
	DWORD dwType{};
	pwcEnd = L"\0";
	dwType = wcstol(wcsTemp, &pwcEnd, 16);

	// 
	int iRes = g_pMemBP->AddBP(g_pProcDbgedInfo->dwPID, dwAddr, dwType);
	if (0 == iRes)
	{
		wcsTemp.Format(L"����ϵ���ӳɹ�[��ַ:0x%08x] [����:%d]!", dwAddr, dwType);
		::MessageBoxW(g_pMyDbger0Dlg->m_hWnd, wcsTemp, L"", 0);
	}
	else
	{
		g_pMemBP->RemoveBP();
		wcsTemp.Format(L"û������ϵ���!", dwAddr, dwType);
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
