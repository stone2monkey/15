#include "stdafx.h"
#include "MemListCtrl.h"
#include "Interface.h"


void CMemListCtrl::GetMemData(DWORD dwAddr, DWORD dwPID)
{
	// 0.参数有效性 ************************************************************

	// 1.修改内存地址处的属性 ************************************************************
	HANDLE hProc{};
	DWORD dwOldProtect{};
	DWORD dwSize = 15 * 16;
	CInterface::OpenProcAndAlterPage(hProc, dwPID, dwAddr, dwSize, dwOldProtect);

	// 2. ************************************************************
	BYTE btBuff[15 * 16]{};
	if (!ReadProcessMemory(hProc, (LPVOID)dwAddr, btBuff, sizeof(btBuff), 0)) { assert(0); }

	// 3. ************************************************************
	CStringW wcsTemp{};
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 17; j++)
		{
			if (16 == j) { wcsTemp.Format(L"%08x", dwAddr + i * 16); }
			else { wcsTemp.Format(L"%02x", btBuff[i * 16 + j]); }

			SetItemText(i, j, wcsTemp);
		}
	}

	// 4.恢复内存地址处的属性 ************************************************************
	CInterface::ResumePageAndClsHandle(hProc, dwAddr, dwSize, dwOldProtect);
}

CMemListCtrl::CMemListCtrl()
{
}


CMemListCtrl::~CMemListCtrl()
{
}


void CMemListCtrl::PreSubclassWindow()
{
	SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	CStringW wcsTemp{};
	for (DWORD i = 0; i < 16; i++)
	{	
		wcsTemp.Format(L"%02x", i);
		InsertColumn(i, wcsTemp, 0, 40);
	}
	wcsTemp = L"Addr";
	InsertColumn(16, wcsTemp, 0, 120);

	for (int i = 0; i < 15; i++)
	{
		InsertItem(i, L"");
	}

	CListCtrl::PreSubclassWindow();
}
