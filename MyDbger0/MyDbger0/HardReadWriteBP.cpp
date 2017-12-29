#include "stdafx.h"
#include "HardReadWriteBP.h"
#include "_MyCustomType.h"


int CHardReadWriteBP::AddBP(DWORD dwTID, DWORD dwAddr)
{
	// �õ��߳̾��
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, 0, dwTID);
	if (!hThread) { assert(0); }

	// �õ��̻߳���
	CONTEXT ct = { CONTEXT_DEBUG_REGISTERS };
	if (!::GetThreadContext(hThread, &ct)) { assert(0); }

	// �Ƿ��Ѿ�����,���ڷ���1
	if (dwAddr == ct.Dr0) { return 1; }
	if (dwAddr == ct.Dr1) { return 1; }
	if (dwAddr == ct.Dr2) { return 1; }
	if (dwAddr == ct.Dr3) { return 1; }

	// ����Ӳ����д�ϵ�
	MY_REG_DR7* pDR7 = (MY_REG_DR7*)&ct.Dr7;
	if ((!pDR7->L0) && (!ct.Dr0))
	{
		ct.Dr0 = dwAddr;
		pDR7->RW0 = 3;
		pDR7->LEN0 = 3;
		pDR7->L0 = 1;
	}
	else if (!pDR7->L1 && (!ct.Dr1))
	{
		ct.Dr1 = dwAddr;
		pDR7->RW1 = 3;
		pDR7->LEN1 = 3;
		pDR7->L1 = 1;
	}
	else if (!pDR7->L2 && (!ct.Dr2))
	{
		ct.Dr2 = dwAddr;
		pDR7->RW2 = 3;
		pDR7->LEN2 = 3;
		pDR7->L2 = 1;
	}
	else if (!pDR7->L3 && (!ct.Dr3))
	{
		ct.Dr3 = dwAddr;
		pDR7->RW3 = 3;
		pDR7->LEN3 = 3;
		pDR7->L3 = 1;
	}
	else
	{
		CloseHandle(hThread);
		return 2;	// �ϵ�����
	}

	// �����̻߳���
	if (!::SetThreadContext(hThread, &ct)) { assert(0); }

	// �ر��߳̾��
	CloseHandle(hThread);

	return 0;
}

BOOL CHardReadWriteBP::RemoveBP(DWORD dwTID, DWORD dwAddr)
{
	// �õ��߳̾��
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, 0, dwTID);
	if (!hThread) { assert(0); }

	// �õ��̻߳���
	CONTEXT ct = { CONTEXT_DEBUG_REGISTERS };
	if (!::GetThreadContext(hThread, &ct)) { assert(0); }

	// ɾ��Ӳ����д�ϵ�
	MY_REG_DR7* pDR7 = (MY_REG_DR7*)&ct.Dr7;
	if (ct.Dr0 == dwAddr)
	{
		ct.Dr0 = 0;
		pDR7->L0 = 0;
	}
	else if (ct.Dr1 == dwAddr)
	{
		ct.Dr1 = 0;
		pDR7->L1 = 0;
	}
	else if (ct.Dr2 == dwAddr)
	{
		ct.Dr2 = 0;
		pDR7->L2 = 0;
	}
	else if (ct.Dr3 == dwAddr)
	{
		ct.Dr3 = 0;
		pDR7->L3 = 0;
	}
	else
	{
		CloseHandle(hThread);
		return false;
	}

	// �����̻߳���
	if (!::SetThreadContext(hThread, &ct)) { assert(0); }

	// �ر��߳̾��
	CloseHandle(hThread);

	return TRUE;
}

void CHardReadWriteBP::AddConditionBP(DWORD dwTID, DWORD dwAddr, DWORD dwData)
{
	if (AddBP(dwTID, dwAddr)) { assert(0); }

	m_ConditionBP.dwAddr = dwAddr;
	m_ConditionBP.dwData = dwData;
}

CHardReadWriteBP::CHardReadWriteBP()
{
}


CHardReadWriteBP::~CHardReadWriteBP()
{
}
