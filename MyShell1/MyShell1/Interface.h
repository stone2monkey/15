#pragma once
class CInterface
{
	// Methods ********************************************************************************************
public:
	//************************************
	// Name: CInterface::RvaInWhichSec
	// args: DWORD dwRva
	// args: DWORD * dwSecRange
	// Retn: int
	// return 0 --> .text
	// return 1 --> .idata
	// return 2 --> .reloc
	// return -1 --> unimportance
	//************************************
	static int RvaInWhichSec(DWORD dwRva, DWORD* dwSecRange);

	static void SecRange(_In_ DWORD* dwSecRange, DWORD dwTextRva, DWORD dwSizeOfText, DWORD dwIDataRva, DWORD dwSizeOfIData, DWORD dwRelocRva, DWORD dwSizeOfReloc);


	//************************************
	// Name: CInterface::FindDllCharacteristicCode
	// Funn: 在dll代码段找到特征码 + 8,也就是new OEP的VA
	// args: DWORD dw0CharCode0
	// args: DWORD dw0CharCode1
	// args: char * szBufOfCode
	// args: DWORD dwSize
	// Retn: DWORD
	//************************************
	static DWORD FindDllCharacteristicCode(DWORD dw0CharCode0, DWORD dw0CharCode1, char* szBufOfCode, DWORD dwSize);

	//************************************
	// Name: CInterface::EncodeExeTextSec
	// Funn: 异或exe的.text段
	// args: DWORD * pAddr
	// args: DWORD dwXorData
	// args: DWORD dwTime
	// Retn: void
	//************************************
	static void EncodeExeTextSec(DWORD* pAddr, DWORD dwXorData, DWORD dwTime);

public:
	CInterface();
	~CInterface();
};

