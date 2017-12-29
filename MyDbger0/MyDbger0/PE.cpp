#include "stdafx.h"
#include "PE.h"
#include "Interface.h"


void CPE::Init(DWORD dwMod, DWORD dwPID)
{
	// 0.������Ч�� ************************************************************
	if (!dwMod) { assert(0); }

	// 1.�޸��ڴ��ַ�������� ************************************************************
	HANDLE hProc{};
	DWORD dwOldProtect{};
	DWORD dwSize = 0x1000;
	CInterface::OpenProcAndAlterPage(hProc, dwPID, dwMod, dwSize, dwOldProtect);

	// 2.��ȡPEͷ������,��ʼ��PE ************************************************************
	m_dwMod = dwMod;

	m_pDosHeader = (IMAGE_DOS_HEADER*)dwMod;

	DWORD e_lfanew{};
	if (!ReadProcessMemory(hProc, (LPVOID)(m_dwMod + sizeof(IMAGE_DOS_HEADER) - sizeof(LONG)), &e_lfanew, 4, 0)) { assert(0); }
	m_pNTHeaders = (IMAGE_NT_HEADERS*)(dwMod + e_lfanew);

	m_pFileHeader = (IMAGE_FILE_HEADER*)((DWORD)m_pNTHeaders + 4);

	m_pOptionalHeader = (IMAGE_OPTIONAL_HEADER*)((DWORD)m_pFileHeader + sizeof(IMAGE_FILE_HEADER));

	if (!ReadProcessMemory(hProc, (LPVOID)((DWORD)m_pOptionalHeader + 4*4), &m_dwOEP, 4, 0)) { assert(0); }
	m_dwOEP += m_dwMod;

	if (!ReadProcessMemory(hProc, (LPVOID)((DWORD)m_pOptionalHeader + 14*4), &m_dwSizeOfMod, 4, 0)) { assert(0); }

	if (!ReadProcessMemory(hProc, (LPVOID)((DWORD)m_pOptionalHeader + 5 * 4), &m_dwBaseOfCode, 4, 0)) { assert(0); }
	m_dwBaseOfCode += m_dwMod;

	if (!ReadProcessMemory(hProc, (LPVOID)((DWORD)m_pOptionalHeader + 1 * 4), &m_dwSizeOfCode, 4, 0)) { assert(0); }

	// ����PE�Ƿ񱻾���(���:��CREATE_PROCESS_DEBUG_EVENTʱ,PE������)
	//char ch[20]{};
	//if (!ReadProcessMemory(hProc, (LPVOID)m_dwOEP, ch, 20, 0)) { assert(0); }

	// 3.�ж϶�ȡ��PE�Ƿ���ȷ ************************************************************
	// MZ
	WORD e_magic{};
	if (!ReadProcessMemory(hProc, (LPVOID)m_dwMod, &e_magic, 2, 0)) { assert(0); }
	if (0x5a4d != e_magic) { assert(0); }

	//PE
	DWORD Signature{};
	if (!ReadProcessMemory(hProc, (LPVOID)m_pNTHeaders, &Signature, 4, 0)) { assert(0); }
	if (0x4550 != Signature) { assert(0); }

	// 4.�ָ��ڴ��ַ�������� ************************************************************
	CInterface::ResumePageAndClsHandle(hProc, dwMod, dwSize, dwOldProtect);
}

CPE::CPE()
{
}


CPE::~CPE()
{
}
