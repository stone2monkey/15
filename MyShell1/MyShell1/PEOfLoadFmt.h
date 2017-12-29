#pragma once
class CPEOfLoadFmt
{
	// Attributions ***************************************************************************************
public:
	DWORD m_dwMod{};							// module address
	IMAGE_DOS_HEADER* m_pDosHdr{};
	IMAGE_NT_HEADERS* m_pNTHdrs{};
	IMAGE_FILE_HEADER* m_pFileHdr{};
	int m_iNumOfSection{};
	IMAGE_OPTIONAL_HEADER* m_pOptHdr{};
	IMAGE_DATA_DIRECTORY* m_pDataDirTable{};
	IMAGE_SECTION_HEADER* m_pSecHdrTable{};

	// Methods ********************************************************************************************
	void Init(DWORD dwMod);



public:
	CPEOfLoadFmt();
	~CPEOfLoadFmt();
};

