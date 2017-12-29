#include "stdafx.h"
#include "Cint3BP.h"
#include "_CGlobal.h"
#include "_MyCustomType.h"
#include "PE.h"
#include "Interface.h"


void Cint3BP::AddBP(DWORD dwAddr)
{
	// 1.修改内存地址处的属性 ************************************************************
	HANDLE hProc{};
	DWORD dwOldProtect{};
	DWORD dwSize = 1;
	CInterface::OpenProcAndAlterPage(hProc, g_pProcDbgedInfo->dwPID, dwAddr, dwSize, dwOldProtect);


	//
	MY_INT3_BP* p = new MY_INT3_BP();

	// read
	if (!ReadProcessMemory(hProc, (LPVOID)dwAddr, p->btData, 1, 0)) { assert(0); }
	p->dwAddr = dwAddr;

	// save
	m_vctMyInt3BK.push_back(p);

	// write int3
	BYTE bt = '\xcc';
	if (!WriteProcessMemory(hProc, (LPVOID)dwAddr, &bt, 1, 0)) { assert(0); }

	// 4.恢复内存地址处的属性 ************************************************************
	CInterface::ResumePageAndClsHandle(hProc, dwAddr, dwSize, dwOldProtect);
}

void Cint3BP::RepairCode(DWORD dwAddr)
{
	// 1.修改内存地址处的属性 ************************************************************
	HANDLE hProc{};
	DWORD dwOldProtect{};
	DWORD dwSize = 1;
	CInterface::OpenProcAndAlterPage(hProc, g_pProcDbgedInfo->dwPID, dwAddr, dwSize, dwOldProtect);


	// 0.找到dwAddr在vct中的索引 ************************************************************
	BOOL bIsFound{};
	DWORD dwIndex{}, dwCount{};
	dwCount = m_vctMyInt3BK.size();
	for (DWORD i = 0; i < dwCount; i++)
	{
		if (m_vctMyInt3BK.at(i)->dwAddr == dwAddr)
		{
			bIsFound = TRUE;
			dwIndex = i;
			break;
		}
	}

	if (!bIsFound) { assert(0); }

	// 1.修正 ************************************************************
	if (!WriteProcessMemory(hProc, (LPVOID)dwAddr, m_vctMyInt3BK.at(dwIndex)->btData, 1, 0)) { assert(0); }

	// 4.恢复内存地址处的属性 ************************************************************
	CInterface::ResumePageAndClsHandle(hProc, dwAddr, dwSize, dwOldProtect);
}

void Cint3BP::RepairEip(DWORD dwTID)
{
	// 得到线程句柄
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, 0, dwTID);
	if (!hThread) { assert(0); }

	// 得到线程环境
	CONTEXT ct{};
	ct.ContextFlags = CONTEXT_ALL;
	if (!::GetThreadContext(hThread, &ct)) { assert(0); }

	// 修改Eip
	ct.Eip--;

	// 设置线程环境
	if (!::SetThreadContext(hThread, &ct)) { assert(0); }

	// 关闭线程句柄
	CloseHandle(hThread);
}

void Cint3BP::RepairBK()
{
	// 0. 修正 ********************************************************
	DWORD dwCount{};
	dwCount = m_vctMyInt3BK.size();
	for (DWORD i = 0; i < dwCount; i++)
	{
		if (m_vctMyInt3BK.at(i)->bNeedToRepair)
		{
			// 1.修改内存地址处的属性 ************************************************************
			HANDLE hProc{};
			DWORD dwOldProtect{};
			DWORD dwSize = 1;
			DWORD dwAddr = m_vctMyInt3BK.at(i)->dwAddr;
			CInterface::OpenProcAndAlterPage(hProc, g_pProcDbgedInfo->dwPID, dwAddr, dwSize, dwOldProtect);

			// write int3
			BYTE bt = '\xcc';
			if (!WriteProcessMemory(hProc, (LPVOID)dwAddr, &bt, 1, 0)) { assert(0); }
			// 同步数据
			m_vctMyInt3BK.at(i)->bNeedToRepair = FALSE;

			// 4.恢复内存地址处的属性 ************************************************************
			CInterface::ResumePageAndClsHandle(hProc, dwAddr, dwSize, dwOldProtect);
		}
	}
}

void Cint3BP::RemoveBP(DWORD dwAddr)
{
	// 0.找到dwAddr在vct中的索引,删除 ************************************************************
	BOOL bIsFound{};
	DWORD dwCount{};
	dwCount = m_vctMyInt3BK.size();
	for (DWORD i = 0; i < dwCount; i++)
	{
		if (m_vctMyInt3BK.at(i)->dwAddr == dwAddr)
		{
			bIsFound = TRUE;
			delete m_vctMyInt3BK.at(i);
			m_vctMyInt3BK.erase(m_vctMyInt3BK.begin() + i);
			break;
		}
	}

	if (!bIsFound) { assert(0); }
}

BOOL Cint3BP::SeekBP(DWORD dwAddr, DWORD & dwIndex)
{
	DWORD dwCount = m_vctMyInt3BK.size();
	for (DWORD i = 0; i < dwCount; i++)
	{
		if (m_vctMyInt3BK.at(i)->dwAddr == dwAddr)
		{
			dwIndex = i;
			return TRUE;
		}
	}


	return 0;
}

Cint3BP::Cint3BP()
{
}


Cint3BP::~Cint3BP()
{
}
