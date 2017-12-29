#pragma once


class CPE
{
	// Attributions ***************************************************************************************
public:
	DWORD m_dwMod{};
	DWORD m_dwSizeOfMod{};

	DWORD m_dwBaseOfCode{};
	DWORD m_dwSizeOfCode{};

	DWORD m_dwOEP{};

	IMAGE_DOS_HEADER* m_pDosHeader{};
	IMAGE_NT_HEADERS* m_pNTHeaders{};
	IMAGE_FILE_HEADER* m_pFileHeader{};
	IMAGE_OPTIONAL_HEADER* m_pOptionalHeader{};


	// Methods ********************************************************************************************
public:
	void Init(DWORD hMod, DWORD dwPID);
















public:
	CPE();
	~CPE();
};

