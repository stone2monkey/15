#include "stdafx.h"
#include "PEOfLoadFmt.h"


void CPEOfLoadFmt::Init(DWORD dwMod)
{
	m_dwMod = dwMod;
	m_pDosHdr = (IMAGE_DOS_HEADER*)m_dwMod;
	m_pNTHdrs = (IMAGE_NT_HEADERS*)(m_dwMod + m_pDosHdr->e_lfanew);
	m_pFileHdr = (IMAGE_FILE_HEADER*)(&m_pNTHdrs->FileHeader);
	m_iNumOfSection = m_pFileHdr->NumberOfSections;
	m_pOptHdr = (IMAGE_OPTIONAL_HEADER*)(&m_pNTHdrs->OptionalHeader);
	m_pDataDirTable = m_pOptHdr->DataDirectory;
	m_pSecHdrTable = (IMAGE_SECTION_HEADER*)(m_pNTHdrs + 1);
}

CPEOfLoadFmt::CPEOfLoadFmt()
{
}


CPEOfLoadFmt::~CPEOfLoadFmt()
{
}
