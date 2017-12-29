#include "stdafx.h"
#include "Interface.h"
#include "_CGlobal.h"
#include "PE.h"
#include "MyDbger0Dlg.h"
#include "DisasmListCtrl.h"
#include "ThreadProcOfBug.h"


void CInterface::SelectExeDbged()
{
	// 0. ************************************************************
	if (g_pProcDbgedInfo) { assert(0); }
	g_pProcDbgedInfo = new PROC_DBGED_INFO();

	// 1.����CFileDialog�����,��ѡ���ļ� ************************************************************
	CFileDialog oFD(1);
	if (IDOK != oFD.DoModal()) { return; }

	// 2.ȫ�� ************************************************************
	g_pProcDbgedInfo->wcsFullName =  oFD.GetPathName();
	if (0 == g_pProcDbgedInfo->wcsFullName.CompareNoCase(L"")) { assert(0); }

	// 3.·�� ************************************************************
	g_pProcDbgedInfo->wcsPath = oFD.GetFolderPath();
	if (0 == g_pProcDbgedInfo->wcsPath.CompareNoCase(L"")) { assert(0); }

	// 4.�ļ��� ************************************************************
	g_pProcDbgedInfo->wcsName = oFD.GetFileName();
	if (0 == g_pProcDbgedInfo->wcsName.CompareNoCase(L"")) { assert(0); }

	// 5.������ ************************************************************
	g_pProcDbgedInfo->wcsTitle = oFD.GetFileTitle();
	if (0 == g_pProcDbgedInfo->wcsTitle.CompareNoCase(L"")) { assert(0); }

	// 6.��չ�� ************************************************************
	g_pProcDbgedInfo->wcsExt = oFD.GetFileExt();
	if (0 == g_pProcDbgedInfo->wcsExt.CompareNoCase(L"")) { assert(0); }
}

void CInterface::OpenProcAndAlterPage(HANDLE & hProc, DWORD dwPID, DWORD dwAddrAltered, DWORD dwSize, DWORD & dwOldProtect)
{	
	// �õ��㹻Ȩ�޵Ľ��̾��
	hProc = ::OpenProcess(PROCESS_ALL_ACCESS | PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, 0, dwPID);
	if (!hProc) { assert(0); }

	// �޸��ڴ��ַ������ҳ
	if (!::VirtualProtectEx(hProc, (LPVOID)dwAddrAltered, dwSize, PAGE_EXECUTE_READWRITE, &dwOldProtect)) { assert(0); }
}

void CInterface::ResumePageAndClsHandle(_In_ HANDLE hProc, DWORD dwAddrAltered, DWORD dwSize, _In_ DWORD dwNewProtect)
{
	// �ָ��ڴ��ַ������ҳ
	if (!::VirtualProtectEx(hProc, (LPVOID)dwAddrAltered, dwSize, dwNewProtect, &dwNewProtect)) { assert(0); }

	//�رվ��
	if (!::CloseHandle(hProc)) { assert(0); }
}

void CInterface::BinaryToWChar(BYTE * pBinary, int iSize, CStringW & wcsBinaryString)
{

}

void CInterface::DisasmListPageChanged(DWORD dwZerothAddr, int iSelection)
{
	// 0.������ģ����뷶Χ ************************************************************
	DWORD dwBaseOfCode = g_vctModuleDbgedInfo.at(0)->pPE->m_dwBaseOfCode;
	DWORD dwSizeOfCode = g_vctModuleDbgedInfo.at(0)->pPE->m_dwSizeOfCode;

	// 1.�жϲ�������Ч�� ************************************************************
	// ������,Ҳû�д������ʾ
	if (
		(dwZerothAddr < dwBaseOfCode) ||
		(dwZerothAddr >= dwBaseOfCode + dwSizeOfCode)
		)
		return;

	if (iSelection < 0 || iSelection >36) { assert(0); }

	// 2.�ҵ���vct�е����� ************************************************************
	BOOL bFound{};
	DWORD dwIndex{};
	DWORD dwCount = g_vctDisasmListItemInfo.size();
	// �Ż���
	DWORD i = (dwZerothAddr - dwBaseOfCode) / 16;
	for (i; i < dwCount; i++)
	{
		if (dwZerothAddr == g_vctDisasmListItemInfo.at(i)->dwAddr)
		{
			bFound = TRUE;
			dwIndex = i;
			break;
		}
	}

	if (!bFound) { return; }

	// 3.�������Է��� ************************************************************
	for (DWORD i = 0; i < 37; i++)
	{
		if (dwIndex + i >= g_vctDisasmListItemInfo.size()) { break; }
		CThreadProcOfBug::MakeAddrToSymbol(g_vctDisasmListItemInfo.at(dwIndex + i)->wcsDisasmSymboled);
	}

	// 4.��������� ************************************************************
	for (int i = 0; i < 37; i++)
	{
		// ���vct�������˾��˳�
		if (i + dwIndex >= g_vctDisasmListItemInfo.size())
			return;

		DISASM_LIST_ITEM_INOF* p = g_vctDisasmListItemInfo.at(i + dwIndex);
		CStringW wcsTemp{};

		wcsTemp.Format(L"%08x", p->dwAddr);
		g_pMyDbger0Dlg->m_lcDisasm.SetItemText(i, 0, wcsTemp);

		wcsTemp = L"";
		for (DWORD j = 0; j < p->dwLen; j++)
		{
			wcsTemp.AppendFormat(L"%02x ", p->pBtOpcode[j]);
		}
		g_pMyDbger0Dlg->m_lcDisasm.SetItemText(i, 1, wcsTemp);

		g_pMyDbger0Dlg->m_lcDisasm.SetItemText(i, 2, p->wcsOrginalDisasm);
		g_pMyDbger0Dlg->m_lcDisasm.SetItemText(i, 3, p->wcsDisasmSymboled);
		g_pMyDbger0Dlg->m_lcDisasm.SetItemText(i, 4, p->wcsComment);
	}

	// 5.���ñ�ѡ���� ************************************************************
	for (int i = 0; i < 37; i++)
	{
		g_pMyDbger0Dlg->m_lcDisasm.SetItemState(i, 0, LVIS_SELECTED);
	}

	g_pMyDbger0Dlg->m_lcDisasm.SetItemState(iSelection, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
	g_pMyDbger0Dlg->m_lcDisasm.SetSelectionMark(iSelection);

	// 6.��һ������ȫ�ֵ�ǰ���� ************************************************************
	if (-1 == g_dwIndexOfDisasmList)
	{
		g_dwIndexOfDisasmList = dwIndex;
	}
}

int CInterface::IsHardBP(DWORD dwTID, _Out_ DWORD& dwAddr, _Out_ DWORD& dwBPType, _Out_ DWORD& dwIndex)
{
	// �õ��߳̾��
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, 0, dwTID);
	if (!hThread) { assert(0); }

	// �õ��̻߳���
	CONTEXT ct = { CONTEXT_DEBUG_REGISTERS };
	if (!::GetThreadContext(hThread, &ct)) { assert(0); }

	// ȷ���ϵ�����
	MY_REG_DR7* pDR7 = (MY_REG_DR7*)&ct.Dr7;
	MY_REG_DR6* pDR6 = (MY_REG_DR6*)&ct.Dr6;

	// TF�ϵ�
	if (pDR6->BS)
		return 0;	

	// Ӳ���ϵ�
	if (pDR6->B0)
	{
		dwAddr = ct.Dr0;
		dwBPType = pDR7->RW0;
		dwIndex = 0;
		return 1;	
	}
	else if (pDR6->B1)
	{
		dwAddr = ct.Dr1;
		dwBPType = pDR7->RW1;
		dwIndex = 1;
		return 1;	// Ӳ���ϵ�
	}
	else if (pDR6->B2)
	{
		dwAddr = ct.Dr2;
		dwBPType = pDR7->RW2;
		dwIndex = 2;
		return 1;	// Ӳ���ϵ�
	}
	else if (pDR6->B3)
	{
		dwAddr = ct.Dr3;
		dwBPType = pDR7->RW3;	
		dwIndex = 3;
		return 1;	// Ӳ���ϵ�
	}

	// �ر��߳̾��
	CloseHandle(hThread);
	return -1;
}

void CInterface::RepairHardBP(DWORD dwTID)
{
	// �õ��߳̾��
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, 0, dwTID);
	if (!hThread) { assert(0); }

	// �õ��̻߳���
	CONTEXT ct = { CONTEXT_DEBUG_REGISTERS };
	if (!::GetThreadContext(hThread, &ct)) { assert(0); }

	// �޸�Ӳ���ϵ�
	MY_REG_DR7* pDR7 = (MY_REG_DR7*)&ct.Dr7;
	if (ct.Dr0 && (pDR7->L0 == 0))
	{
		pDR7->L0 = 1;
	}
	if (ct.Dr1 && (pDR7->L1 == 0))
	{
		pDR7->L1 = 1;
	}
	if (ct.Dr2 && (pDR7->L2 == 0))
	{
		pDR7->L2 = 1;
	}
	if (ct.Dr3 && (pDR7->L3 == 0))
	{
		pDR7->L3 = 1;
	}

	// �����̻߳���
	if (!::SetThreadContext(hThread, &ct)) { assert(0); }

	// �ر��߳̾��
	CloseHandle(hThread);
}


CInterface::CInterface()
{
}


CInterface::~CInterface()
{
}
