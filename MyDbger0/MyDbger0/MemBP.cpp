#include "stdafx.h"
#include "MemBP.h"
#include "Interface.h"


int CMemBP::AddBP(DWORD dwPID, DWORD dwAddr, DWORD dwType)
{
	if (0 != m_MyMemBP.dwAddr) { return 1; }

	// 0.������Ч�� ************************************************************
	if (!dwAddr) { assert(0); }

	// 1.�޸��ڴ��ַ��������Ϊ���� ************************************************************
	// �õ��㹻Ȩ�޵Ľ��̾��
	m_hProc = ::OpenProcess(PROCESS_ALL_ACCESS | PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, 0, dwPID);
	if (!m_hProc) { assert(0); }
	// �޸��ڴ��ַ������ҳ
	if (!::VirtualProtectEx(m_hProc, (LPVOID)dwAddr, 1, PAGE_NOACCESS, &m_MyMemBP.dwOldProtect)) { assert(0); }

	// 2.�������� ************************************************************
	m_MyMemBP.dwAddr = dwAddr;
	m_MyMemBP.dwType = dwType;

	return 0;
}

int CMemBP::RemoveBP()
{
	if (0 == m_MyMemBP.dwAddr) { return 1; }

	ResumeMemProtection();

	m_hProc = 0;
	::memset(&m_MyMemBP, 0, sizeof(MY_MEM_BP));

	return 0;
}

void CMemBP::ResumeMemProtection()
{
	if (!::VirtualProtectEx(m_hProc, (LPVOID)m_MyMemBP.dwAddr, 1, m_MyMemBP.dwOldProtect, &m_MyMemBP.dwOldProtect)) { assert(0); }
}

void CMemBP::AlterMemProtection()
{
	if (!::VirtualProtectEx(m_hProc, (LPVOID)m_MyMemBP.dwAddr, 1, PAGE_NOACCESS, &m_MyMemBP.dwOldProtect)) { assert(0); }
}

CMemBP::CMemBP()
{
}


CMemBP::~CMemBP()
{
}
