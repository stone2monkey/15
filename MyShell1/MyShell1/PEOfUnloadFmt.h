#pragma once
class CPEOfUnloadFmt
{
	// Attributions ***************************************************************************************
public:
	char* m_szFullFileName{};
	char* m_szPE{};
	DWORD m_dwSizeOfOrgExe{};	// 原始exe的尺寸
	DWORD m_dwSizeOfCurBuf{};	// 当前Buf的尺寸

	DWORD m_dwMod{};							// module address
	IMAGE_DOS_HEADER* m_pDosHdr{};
	IMAGE_NT_HEADERS* m_pNTHdrs{};
	IMAGE_FILE_HEADER* m_pFileHdr{};
	int m_iNumOfSection{};
	IMAGE_OPTIONAL_HEADER* m_pOptHdr{};
	IMAGE_DATA_DIRECTORY* m_pDataDirTable{};
	IMAGE_SECTION_HEADER* m_pSecHdrTable{};


	// Methods ********************************************************************************************
	bool IsPEFmt(char* szFullFileName);
	void Init(char* szFullFileName);
	void InitPE(DWORD dwMod, DWORD dwSize);


public:
	CPEOfUnloadFmt();
	~CPEOfUnloadFmt();
};

