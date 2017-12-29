#include "stdafx.h"
#include "StackListCtrl.h"
#include "_CGlobal.h"
#include "Interface.h"


void CStackListCtrl::GetStackData()
{
	static DWORD dwPreEsp = 0;

	// 0.找到ESP ************************************************************
	DWORD dwEsp{};
	{
		// 得到线程句柄
		HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, 0, g_pProcDbgedInfo->dwTID);
		if (!hThread) { assert(0); }

		// 得到线程环境
		CONTEXT ct{};
		ct.ContextFlags = CONTEXT_ALL;
		if (!::GetThreadContext(hThread, &ct)) { assert(0); }

		// ESP
		dwEsp = ct.Esp;

		// 关闭线程句柄
		CloseHandle(hThread);
	}

	if (!dwEsp) { return; }
	if (dwPreEsp == dwEsp) { return; }
	dwPreEsp = dwEsp;

	// 1.读数据 ************************************************************
	DWORD dwArrBuff[100]{};
	{
		//// 1.修改内存地址处的属性 ************************************************************
		//HANDLE hProc{};
		//DWORD dwOldProtect{};
		//DWORD dwSize = sizeof(dwArrBuff);
		//CInterface::OpenProcAndAlterPage(hProc, g_pProcDbgedInfo->dwPID, dwEsp, dwSize, dwOldProtect);

		//// 读数据
		//if (!ReadProcessMemory(hProc, (LPVOID)dwEsp, dwArrBuff, dwSize, 0)) { assert(0); }

		//// 4.恢复内存地址处的属性 ************************************************************
		//CInterface::ResumePageAndClsHandle(hProc, dwEsp, dwSize, dwOldProtect);


		HANDLE hProc = ::OpenProcess(PROCESS_ALL_ACCESS, 0, g_pProcDbgedInfo->dwPID);
		if (!hProc) { assert(0); }

		if (!ReadProcessMemory(hProc, (LPVOID)dwEsp, dwArrBuff, sizeof(dwArrBuff), 0)) { assert(0); }

		if (!::CloseHandle(hProc)) { assert(0); }
	}

	// 2.到控件 ************************************************************
	CStringW wcsTemp{};
	{
		for (int i = 0; i < 100; i++)
		{
			wcsTemp.Format(L"%08x", dwEsp + i * 4);
			SetItemText(i, 0, wcsTemp);

			wcsTemp.Format(L"%08x", dwArrBuff[i]);
			SetItemText(i, 1, wcsTemp);
		}
	}
}

CStackListCtrl::CStackListCtrl()
{
}


CStackListCtrl::~CStackListCtrl()
{
}


void CStackListCtrl::PreSubclassWindow()
{
	SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	InsertColumn(0, L"Addr", LVCFMT_LEFT, 110);
	InsertColumn(1, L"Data", LVCFMT_LEFT, 110);

	for (int i = 0; i < 100; i++)
	{
		InsertItem(i, L"");
	}

	CListCtrl::PreSubclassWindow();
}
