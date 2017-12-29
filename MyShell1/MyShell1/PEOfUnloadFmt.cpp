#include "stdafx.h"
#include "PEOfUnloadFmt.h"


bool CPEOfUnloadFmt::IsPEFmt(char* szFullFileName)
{
	// 0. Judge whether the file object ptr is valid ***********************************************
	if (!szFullFileName)
		ASSERT(0);

	CStringA strFileFullName = szFullFileName;
	if (3 > strFileFullName.GetLength())
		ASSERT(0);

	int iLen = strFileFullName.GetLength();
	m_szFullFileName = new char[iLen + 1]{};
	::memcpy(m_szFullFileName, szFullFileName, iLen+1);

	// 1. Judge whether the file extention name is valid ***********************************************
	char szValidExtention[3][4] = { "exe", "dll", "sys" };
	int bMatching[3]{};

	strFileFullName.MakeLower();
	int i = 3;
	while (i--)
		bMatching[i] = strFileFullName.Find(szValidExtention[i], 0);

	if (bMatching[0] < 0 && bMatching[1] < 0 && bMatching[2] < 0)
		ASSERT(0);

	WCHAR wszFullFileName[MAX_PATH]{};
	MultiByteToWideChar(CP_ACP, 0, m_szFullFileName, iLen, wszFullFileName, MAX_PATH);
	CFile oFile(wszFullFileName, CFile::modeRead | CFile::shareDenyWrite);
	if (!oFile)
		ASSERT(0);

	// 2. Judge whether the DOS header`s 'MZ'(word) is valid ***********************************************
	m_pDosHdr = new IMAGE_DOS_HEADER{};
	oFile.Read(m_pDosHdr, sizeof(IMAGE_DOS_HEADER));
	if (0x5A4D != m_pDosHdr->e_magic)
		ASSERT(0);

	// 3. Judge whether the NT header`s 'PE'(dword) is valid***********************************************
	m_pNTHdrs = new IMAGE_NT_HEADERS{};
	oFile.Seek(m_pDosHdr->e_lfanew, CFile::begin);
	oFile.Read(m_pNTHdrs, sizeof(IMAGE_NT_HEADERS));
	if (0x4550 != m_pNTHdrs->Signature)
		ASSERT(0);

	// 4.end ***********************************************
	delete m_pDosHdr;
	m_pDosHdr = 0;
	delete m_pNTHdrs;
	m_pNTHdrs = 0;
	return true;
}

void CPEOfUnloadFmt::Init(char* szFullFileName)
{
	HANDLE hFile = CreateFileA(szFullFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
		ASSERT(0);
	DWORD dwLow{}, dwHigh{};
	dwLow = GetFileSize(hFile, &dwHigh);
	if (dwHigh)
		ASSERT(0);

	m_szPE = new char[dwLow] {};
	if (!ReadFile(hFile, m_szPE, dwLow, &dwHigh, 0))
		ASSERT(0);

	CloseHandle(hFile);

	InitPE(DWORD(m_szPE), dwLow);
}

void CPEOfUnloadFmt::InitPE(DWORD dwMod, DWORD dwSize)
{
	m_dwMod = dwMod;
	m_dwSizeOfOrgExe = dwSize;
	m_dwSizeOfCurBuf = m_dwSizeOfOrgExe;
	m_pDosHdr = (IMAGE_DOS_HEADER*)m_dwMod;
	m_pNTHdrs = (IMAGE_NT_HEADERS*)(m_dwMod + m_pDosHdr->e_lfanew);
	m_pFileHdr = (IMAGE_FILE_HEADER*)(&m_pNTHdrs->FileHeader);
	m_iNumOfSection = m_pFileHdr->NumberOfSections;
	m_pOptHdr = (IMAGE_OPTIONAL_HEADER*)(&m_pNTHdrs->OptionalHeader);
	m_pDataDirTable = m_pOptHdr->DataDirectory;
	m_pSecHdrTable = (IMAGE_SECTION_HEADER*)(m_pNTHdrs + 1);
}

CPEOfUnloadFmt::CPEOfUnloadFmt()
{
}


CPEOfUnloadFmt::~CPEOfUnloadFmt()
{
}
