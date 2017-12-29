#include "stdafx.h"
#include "Pack.h"
#include "PEOfUnloadFmt.h"
#include "PEOfLoadFmt.h"
#include "Interface.h"

// ʹ�ø�ѹ���������Ҫ����3������Ԥ����ָ��
#define ZLIB_WINAPI
#include "zlib/zlib.h"
#pragma comment(lib,"zlib/zlibstat.lib")


void CPack::Init(char* szFullFileName)
{
	if (!m_szFullFileName)
	{
		int iLen = strlen(szFullFileName);
		m_szFullFileName = new char[iLen + 1]{};
		::memcpy(m_szFullFileName, szFullFileName, iLen + 1);
	}
}

bool CPack::OnPack()
{
	// 0.Ҫ�ӿǵ��ļ��ǲ�����ЧPE�ļ� ***********************************************
	CPEOfUnloadFmt oPEToPacked;
	if (!oPEToPacked.IsPEFmt(m_szFullFileName))
		ASSERT(0);

	// 1.Ҫ�ӿǵ��ļ�PEͷ�Ƿ������������� ***********************************************
	oPEToPacked.Init(m_szFullFileName);
	DWORD dwPosOfHeaderValidDataForOrgExe = oPEToPacked.m_pDosHdr->e_lfanew + sizeof(IMAGE_NT_HEADERS) + sizeof(IMAGE_SECTION_HEADER) * oPEToPacked.m_iNumOfSection;
	DWORD dwAvalableSpace = oPEToPacked.m_pOptHdr->SizeOfHeaders - dwPosOfHeaderValidDataForOrgExe;
	if (sizeof(IMAGE_SECTION_HEADER) * 3 > dwAvalableSpace)
		ASSERT(0);

	// 2.��DLL���ص��ڴ�, �����θ���Mytext Myidata Myreloc, ���������ڴ��еĶγߴ� ***********************************************
	// ��DLL���ص��ڴ�
	HMODULE hDllMod = LoadLibraryA("MyDll.dll");
	if (!hDllMod)
		ASSERT(0);

	// �����θ���Mytext Myidata Myreloc
	CPEOfLoadFmt oPEofDllLoaded;
	oPEofDllLoaded.Init((DWORD)hDllMod);
	DWORD dwOldProtection{};
	if (!VirtualProtect(oPEofDllLoaded.m_pSecHdrTable[0].Name, 1, PAGE_EXECUTE_READWRITE, &dwOldProtection))
		ASSERT(0);
	::memcpy(oPEofDllLoaded.m_pSecHdrTable[0].Name, "Mytext", sizeof("Mytext"));
	::memcpy(oPEofDllLoaded.m_pSecHdrTable[1].Name, "Myidata", sizeof("Myidata"));
	::memcpy(oPEofDllLoaded.m_pSecHdrTable[4].Name, "Myreloc", sizeof("Myreloc"));

	// ���������ڴ��еĶγߴ�
	DWORD dwSizeOfMytext{}, dwSizeOfMyidata{}, dwSizeOfMyreloc{};
	DWORD dwOrgRvaOfIDataSecForDll = oPEofDllLoaded.m_pSecHdrTable[1].VirtualAddress;
	dwSizeOfMytext = oPEofDllLoaded.m_pSecHdrTable[1].VirtualAddress - oPEofDllLoaded.m_pSecHdrTable[0].VirtualAddress;
	dwSizeOfMyidata = oPEofDllLoaded.m_pSecHdrTable[2].VirtualAddress - oPEofDllLoaded.m_pSecHdrTable[1].VirtualAddress;
	dwSizeOfMyreloc =oPEofDllLoaded.m_pOptHdr->SizeOfImage - oPEofDllLoaded.m_pSecHdrTable[4].VirtualAddress;

	// �޸�Dll��������,������õ�
	if (!VirtualProtect((LPVOID)(oPEofDllLoaded.m_pSecHdrTable[0].VirtualAddress + oPEofDllLoaded.m_dwMod), dwSizeOfMytext, PAGE_EXECUTE_READWRITE, &dwOldProtection))
		ASSERT(0);
	if (!VirtualProtect((LPVOID)(oPEofDllLoaded.m_pSecHdrTable[1].VirtualAddress + oPEofDllLoaded.m_dwMod), dwSizeOfMyidata, PAGE_EXECUTE_READWRITE, &dwOldProtection))
		ASSERT(0);
	if (!VirtualProtect((LPVOID)(oPEofDllLoaded.m_pSecHdrTable[4].VirtualAddress + oPEofDllLoaded.m_dwMod), dwSizeOfMyreloc, PAGE_EXECUTE_READWRITE, &dwOldProtection))
		ASSERT(0);

	// 3.����exe���ڴ��е�buf,�ô�С���Է��������� ***********************************************
	DWORD dwSizeOfOrgExe = oPEToPacked.m_dwSizeOfOrgExe;
	DWORD dwSizeOfCurBuf = dwSizeOfOrgExe + dwSizeOfMytext + dwSizeOfMyidata + dwSizeOfMyreloc;

	char* szBuf = new char[dwSizeOfCurBuf]{};
	::memcpy(szBuf, oPEToPacked.m_szPE, oPEToPacked.m_dwSizeOfOrgExe);
	oPEToPacked.InitPE((DWORD)szBuf, dwSizeOfCurBuf);
	oPEToPacked.m_dwSizeOfOrgExe = dwSizeOfOrgExe;
	delete oPEToPacked.m_szPE;
	oPEToPacked.m_szPE = szBuf;


	
	// 4.�޸������� ***********************************************
	// newһ���ռ�(��Ҫ�ı�dll����)
	IMAGE_SECTION_HEADER arrSecHdr[3]{};
	::memcpy(arrSecHdr, oPEofDllLoaded.m_pSecHdrTable, sizeof(IMAGE_SECTION_HEADER) * 2);
	::memcpy(arrSecHdr + 2, oPEofDllLoaded.m_pSecHdrTable + 4, sizeof(IMAGE_SECTION_HEADER));

	// �޸����н����ֶ�
	arrSecHdr[0].Characteristics |= 0xE0000000;
	arrSecHdr[1].Characteristics |= 0xE0000000;
	arrSecHdr[2].Characteristics |= 0xE0000000;

	arrSecHdr[0].Misc.VirtualSize = dwSizeOfMytext;
	arrSecHdr[0].VirtualAddress = oPEToPacked.m_pOptHdr->SizeOfImage;
	arrSecHdr[0].SizeOfRawData = dwSizeOfMytext;
	arrSecHdr[0].PointerToRawData = oPEToPacked.m_dwSizeOfOrgExe;

	arrSecHdr[1].Misc.VirtualSize = dwSizeOfMyidata;
	arrSecHdr[1].VirtualAddress = oPEToPacked.m_pOptHdr->SizeOfImage + dwSizeOfMytext;
	arrSecHdr[1].SizeOfRawData = dwSizeOfMyidata;
	arrSecHdr[1].PointerToRawData = oPEToPacked.m_dwSizeOfOrgExe + dwSizeOfMytext;

	arrSecHdr[2].Misc.VirtualSize = dwSizeOfMyreloc;
	arrSecHdr[2].VirtualAddress = oPEToPacked.m_pOptHdr->SizeOfImage + dwSizeOfMytext + dwSizeOfMyreloc;
	arrSecHdr[2].SizeOfRawData = dwSizeOfMyreloc;
	arrSecHdr[2].PointerToRawData = oPEToPacked.m_dwSizeOfOrgExe + dwSizeOfMytext + dwSizeOfMyreloc;

	//��ƫ��,������
	DWORD dwOffOfTextSec = arrSecHdr[0].VirtualAddress - oPEofDllLoaded.m_pSecHdrTable[0].VirtualAddress;
	DWORD dwOffOfIDataSec = arrSecHdr[1].VirtualAddress - dwOrgRvaOfIDataSecForDll;
	DWORD dwOffOfRelocSec = arrSecHdr[2].VirtualAddress - oPEofDllLoaded.m_pSecHdrTable[4].VirtualAddress;

	// copy������exe ***********************************************
	::memcpy(oPEToPacked.m_szPE + dwPosOfHeaderValidDataForOrgExe, arrSecHdr, sizeof(IMAGE_SECTION_HEADER) * 3);

	// 5.�޸������ ***********************************************
	// û�е����
	if ((oPEofDllLoaded.m_pDataDirTable + 1)->VirtualAddress)
		ASSERT(0);
	if ((oPEofDllLoaded.m_pDataDirTable + 1)->Size)
		ASSERT(0);

	// �ҵ�����������ָ��
	IMAGE_IMPORT_DESCRIPTOR* pIID = (IMAGE_IMPORT_DESCRIPTOR*)((oPEofDllLoaded.m_pDataDirTable + 1)->VirtualAddress + oPEofDllLoaded.m_dwMod);

	// �޸�INT����Ԫ�����RVA ,// �޸�����������
	while (true)
	{
		// û�е�����������
		if (!pIID->Name)
			break;

		// �޸�INT����Ԫ�����RVA
		DWORD* dwINT = (DWORD*)(pIID->OriginalFirstThunk + oPEofDllLoaded.m_dwMod);
		while (true)
		{
			// ��������������
			if (!*dwINT)
				break;

			if (0 == ((*dwINT)>>31))
				*dwINT += dwOffOfIDataSec;

			dwINT++;
		}

		// �޸�����������1
		pIID->OriginalFirstThunk += dwOffOfIDataSec;
		pIID->Name += dwOffOfIDataSec;
		pIID->FirstThunk += dwOffOfIDataSec;

		pIID++;
	}

	// copy
	::memcpy(oPEToPacked.m_szPE + oPEToPacked.m_dwSizeOfOrgExe + dwSizeOfMytext, (char*)(oPEofDllLoaded.m_pSecHdrTable[1].VirtualAddress + oPEofDllLoaded.m_dwMod), dwSizeOfMyidata);

	// ����exe������ַ,�޸�IIT,�����ĳ�Dll�����
	::memcpy(oPEToPacked.m_szPE + 2, oPEToPacked.m_pDataDirTable + 1, 8);
	oPEToPacked.m_pDataDirTable[1].VirtualAddress = oPEofDllLoaded.m_pDataDirTable[1].VirtualAddress + dwOffOfIDataSec;
	oPEToPacked.m_pDataDirTable[1].Size = oPEofDllLoaded.m_pDataDirTable[1].Size;

	// 6.�޸�BRBָ��ĵ�ַ������,�޸�BRB, �޸�BRT ***********************************************
	// û��BRT
	if ((oPEofDllLoaded.m_pDataDirTable + 5)->VirtualAddress)
		ASSERT(0);
	if ((oPEofDllLoaded.m_pDataDirTable + 5)->Size)
		ASSERT(0);

	// �ҵ���ַ�ض�λָ��
	IMAGE_BASE_RELOCATION* pIBR = (IMAGE_BASE_RELOCATION*)((oPEofDllLoaded.m_pDataDirTable + 5)->VirtualAddress + oPEofDllLoaded.m_dwMod);

	// �޸�BRBָ��ĵ�ַ������,�޸�BRB
	typedef struct _BR_DATA
	{
		WORD dwOff : 12;
		WORD dwType : 4;
	}BR_DATA;

	DWORD dwArrSecRange[6]{};
	CInterface::SecRange(dwArrSecRange, oPEofDllLoaded.m_pSecHdrTable[0].VirtualAddress, 
		dwSizeOfMytext, oPEofDllLoaded.m_pSecHdrTable[1].VirtualAddress, dwSizeOfMyidata, oPEofDllLoaded.m_pSecHdrTable[4].VirtualAddress, dwSizeOfMyreloc);

	DWORD dwCurSize{};
	while (true)
	{
		DWORD dwRvaOfCurRelocBlock = pIBR->VirtualAddress;
		DWORD dwSizeOfCurRelocBlock = pIBR->SizeOfBlock;
		BR_DATA* pDataOfCurRelocBlock = (BR_DATA*)((DWORD)pIBR + 8);
		while (true)
		{
			for (DWORD i = 0; i < (dwSizeOfCurRelocBlock - 8) / 2; i++)
			{
				if (3 == pDataOfCurRelocBlock[i].dwType)
				{
					// DWORD j = pDataOfCurRelocBlock[i].dwOff + dwRvaOfCurRelocBlock + oPEofDllLoaded.m_dwMod;
					DWORD* pData = (DWORD*)(pDataOfCurRelocBlock[i].dwOff + dwRvaOfCurRelocBlock + oPEofDllLoaded.m_dwMod);
					
					//�ж���Ҫ�ض�λ������(ָ��)���ĸ���, �޸�BRBָ��ĵ�ַ������
					int iRes = CInterface::RvaInWhichSec(*pData - oPEofDllLoaded.m_dwMod, dwArrSecRange);
					if (0 == iRes)
						*pData -= oPEofDllLoaded.m_dwMod - 0x400000 - dwOffOfTextSec;
					else if (1 == iRes)
						*pData -= oPEofDllLoaded.m_dwMod - 0x400000 - dwOffOfIDataSec;
					else if (2 == iRes)
						*pData -= oPEofDllLoaded.m_dwMod - 0x400000 - dwOffOfRelocSec;
				}
			}
			break;
		}

		//�޸�BRB
		//�ж���Ҫ�ض�λ���RVA���ĸ���, �޸�BRB
		int iRes = CInterface::RvaInWhichSec(pIBR->VirtualAddress, dwArrSecRange);
		if (0 == iRes)
			pIBR->VirtualAddress += dwOffOfTextSec;
		else if (1 == iRes)
			pIBR->VirtualAddress += dwOffOfIDataSec;
		else if (2 == iRes)
			pIBR->VirtualAddress += dwOffOfRelocSec;

		dwCurSize += dwSizeOfCurRelocBlock;

		if (dwCurSize == (oPEofDllLoaded.m_pDataDirTable + 5)->Size)
			break;

		if (dwCurSize > (oPEofDllLoaded.m_pDataDirTable + 5)->Size)
			ASSERT(0);

		pIBR = (IMAGE_BASE_RELOCATION*)((DWORD)pIBR + dwSizeOfCurRelocBlock);
	}

	// copy
	::memcpy(oPEToPacked.m_szPE + oPEToPacked.m_dwSizeOfOrgExe + dwSizeOfMytext + dwSizeOfMyidata, 
		(char*)(oPEofDllLoaded.m_pSecHdrTable[4].VirtualAddress + oPEofDllLoaded.m_dwMod), dwSizeOfMyreloc);

	// ����exe�ض�λ���ַ,�޸�BRT,�ض�λ��ĳ�Dll�ض�λ��
	::memcpy(oPEToPacked.m_szPE + 10, oPEToPacked.m_pDataDirTable + 5, 8);
	oPEToPacked.m_pDataDirTable[5].VirtualAddress = oPEofDllLoaded.m_pDataDirTable[5].VirtualAddress + dwOffOfRelocSec;
	oPEToPacked.m_pDataDirTable[5].Size = oPEofDllLoaded.m_pDataDirTable[5].Size;



	// 7.����Mytext��exe ***********************************************
	::memcpy(oPEToPacked.m_szPE + oPEToPacked.m_dwSizeOfOrgExe,
		(char*)(oPEofDllLoaded.m_pSecHdrTable[0].VirtualAddress + oPEofDllLoaded.m_dwMod), dwSizeOfMytext);

	// 8.�޸�exe PEͷ�е�����,��ƥ�������޸� ***********************************************
	oPEToPacked.m_pFileHdr->NumberOfSections += 3;
	oPEToPacked.m_pOptHdr->SizeOfCode += dwSizeOfMytext;
	oPEToPacked.m_pOptHdr->SizeOfImage += dwSizeOfMytext + dwSizeOfMyidata + dwSizeOfMyreloc;

	// 9.����OEP ***********************************************
	// ���� exe OEP
	::memcpy((char*)(oPEToPacked.m_dwMod + 0x14), &(oPEToPacked.m_pOptHdr->AddressOfEntryPoint), 4);
	// �ҵ�Dll����RVA,�������exe�е�RVA
	DWORD dwNewOEP{};
	// 66 8B C0 66 8B E4 00 00
	dwNewOEP = CInterface::FindDllCharacteristicCode(0x66C08B66, 0xFFFFE48B, (char*)(oPEofDllLoaded.m_pSecHdrTable[0].VirtualAddress + oPEofDllLoaded.m_dwMod), dwSizeOfMytext);
	dwNewOEP -= oPEofDllLoaded.m_dwMod;
	dwNewOEP += dwOffOfTextSec;
	// �޸�OEP
	oPEToPacked.m_pOptHdr->AddressOfEntryPoint = dwNewOEP;

	// a.����exe��.text�� ***********************************************
	DWORD dwSizeOfTextSec = oPEToPacked.m_pSecHdrTable[0].SizeOfRawData;
	DWORD* pPosOfTextSec = (DWORD*)(oPEToPacked.m_pSecHdrTable->PointerToRawData + oPEToPacked.m_dwMod);
	DWORD dwTime = dwSizeOfTextSec / 4;
	CInterface::EncodeExeTextSec(pPosOfTextSec, 0x12345678, dwTime);


	//// b.ѹ��exe��.text�� ***********************************************
	//// ѹ��
	//uLong uSizeOfTextSec = oPEToPacked.m_pSecHdrTable[0].SizeOfRawData;
	//byte* btPosOfTextSec = (byte*)(oPEToPacked.m_pSecHdrTable->PointerToRawData + oPEToPacked.m_dwMod);

	//uLong uSizeNeedOfCompressedTextSec = compressBound(uSizeOfTextSec);
	//byte* btPosOfCompressedTextSec = new byte[uSizeNeedOfCompressedTextSec]{};
	//if (!btPosOfCompressedTextSec)
	//	ASSERT(0);

	//if (Z_OK != compress(btPosOfCompressedTextSec, &uSizeNeedOfCompressedTextSec, btPosOfTextSec, uSizeOfTextSec))
	//	ASSERT(0);

	//if (!uSizeNeedOfCompressedTextSec)
	//	ASSERT(0);

	//// ����
	//DWORD dwSizeOfAlignment{};
	//
	//if (uSizeNeedOfCompressedTextSec % 0x200)
	//	dwSizeOfAlignment = uSizeNeedOfCompressedTextSec / 0x200 * 0x200 + 0x200;
	//else
	//	dwSizeOfAlignment = uSizeNeedOfCompressedTextSec;

	//DWORD dwOffToOtherSec = uSizeOfTextSec - dwSizeOfAlignment;		// .text�κ���Ķ�,��desk��,��Ҫ�ƶ����ƫ��
	//
	////  copyѹ������, �ƶ����������
	//::memset(btPosOfTextSec, 0, uSizeOfTextSec);
	//::memcpy(btPosOfTextSec, btPosOfCompressedTextSec, dwSizeOfAlignment);
	//DWORD dwCopySize = oPEToPacked.m_dwSizeOfCurBuf - oPEToPacked.m_pOptHdr->SizeOfHeaders - uSizeOfTextSec;
	//::memmove(btPosOfTextSec + uSizeOfTextSec, btPosOfTextSec + uSizeOfTextSec, dwCopySize);

	//// ��������Ӱ�������
	//oPEToPacked.m_dwSizeOfCurBuf -= dwOffToOtherSec;

	//WORD wNumOfSec = oPEToPacked.m_pFileHdr->NumberOfSections;
	//for (WORD i = 1; i < wNumOfSec; i++)
	//	oPEToPacked.m_pSecHdrTable[i].PointerToRawData -= dwOffToOtherSec;



	// 0. ***********************************************
	// 0. ***********************************************
	// 0. ***********************************************
	// 0. ***********************************************
	// 0. ***********************************************
	HANDLE hNewFile = CreateFileA("..Packed.exe", GENERIC_ALL, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hNewFile)
		ASSERT(0);
	if (!WriteFile(hNewFile, oPEToPacked.m_szPE, oPEToPacked.m_dwSizeOfCurBuf, 0, 0))
		ASSERT(0);
	CloseHandle(hNewFile);

	return true;
}

CPack::CPack()
{
}


CPack::~CPack()
{
}
