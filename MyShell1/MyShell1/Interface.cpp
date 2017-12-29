#include "stdafx.h"
#include "Interface.h"


int CInterface::RvaInWhichSec(DWORD dwRva, DWORD* dwSecRange)
{
	if (dwRva >= dwSecRange[0] && dwRva < dwSecRange[1])
		return 0;
	if (dwRva >= dwSecRange[2] && dwRva < dwSecRange[3])
		return 1;
	if (dwRva >= dwSecRange[4] && dwRva < dwSecRange[5])
		return 2;

	return -1;
}

void CInterface::SecRange(_In_ DWORD* dwSecRange, DWORD dwTextRva, DWORD dwSizeOfText, DWORD dwIDataRva, DWORD dwSizeOfIData, DWORD dwRelocRva, DWORD dwSizeOfReloc)
{
	dwSecRange[0] = dwTextRva;
	dwSecRange[1] = dwTextRva + dwSizeOfText;
	dwSecRange[2] = dwIDataRva;
	dwSecRange[3] = dwIDataRva + dwSizeOfIData;
	dwSecRange[4] = dwRelocRva;
	dwSecRange[5] = dwRelocRva + dwSizeOfReloc;
}

DWORD CInterface::FindDllCharacteristicCode(DWORD dwCharCode0, DWORD dwCharCode1, char* szBufOfCode, DWORD dwSize)
{
	for (DWORD i = 0; i < dwSize - 8; i++)
	{
		DWORD* pCharCode0 = (DWORD*)(szBufOfCode + i);
		if(*pCharCode0 != dwCharCode0)
			continue;;

		DWORD* pCharCode1 = (DWORD*)(szBufOfCode + i + 4);
		if (*pCharCode1 != dwCharCode1)
			continue;

		DWORD dwNewOEP = (DWORD)pCharCode0;
		return dwNewOEP + 8;
	}

	return 0;
}

void CInterface::EncodeExeTextSec(DWORD* pAddr, DWORD dwXorData, DWORD dwTime)
{
	for (DWORD i = 0; i < dwTime; i++)
	{
		if (!pAddr[i])
			continue;

		pAddr[i] ^= dwXorData;
	}
}

CInterface::CInterface()
{
}


CInterface::~CInterface()
{
}
