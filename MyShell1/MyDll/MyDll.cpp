// MyDll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <windows.h>
#include "MyDll.h"
#include <afxcom_.h>
#pragma comment(linker, "/merge:.data=.text") 
#pragma comment(linker, "/merge:.rdata=.text")
#pragma comment(linker, "/section:.text,RWE")


int GetStrLen(char* str0)
{
	__asm
	{
		push ecx;
		push edi;

		mov ecx, 0xffff;
		mov al, 0;
		mov edi, str0;
		repnz scasb;
		mov eax, 0xffff;
		sub eax, ecx;
		dec eax;

		pop edi;
		pop ecx;
	}
}

bool StrCmp(char* str0, char* str1)
{
	int iLen0, iLen1;
	iLen0 = GetStrLen(str0);
	iLen1 = GetStrLen(str1);
	if (iLen0 != iLen1)
		return false;

	_asm
	{
		push ecx;
		push edi;
		push esi;

		mov eax, 0;
		mov ecx, iLen0;
		mov edi, str0;
		mov esi, str1;

		repz cmpsb;
		jnz LabelEnd;
		mov eax, 1;

	LabelEnd:
		pop esi;
		pop edi;
		pop ecx;
	}
}

DWORD GetCurMod()
{
	__asm
	{
		// fs:0 --> _TEB
		// fs:[30] --> _PEB
		mov eax, fs:[0x30];
		// *(pPEB+0xC) --> _PEB_LDR_DATA
		mov eax, [eax + 0xC];
		// *(pPEB_LDR_DATA + 0xC) --> _LDR_DATA_TABLE_ENTRY --> 本文件PE信息
		mov eax, [eax + 0xC];
		// DLLBASE
		mov eax, dword ptr ds : [eax + 0x18];
	}
}

DWORD GetKernel32Mod()
{
	__asm
	{
		// fs:0 --> _TEB
		// fs:[30] --> _PEB
		mov eax, fs:[0x30];
		// *(pPEB+0xC) --> _PEB_LDR_DATA
		mov eax, [eax + 0xC];
		// *(pPEB_LDR_DATA + 0xC) --> _LDR_DATA_TABLE_ENTRY --> 本文件PE信息
		mov eax, [eax + 0xC];
		// _LDR_DATA_TABLE_ENTRY --> _LDR_DATA_TABLE_ENTRY --> ntdll.dll信息
		mov eax, [eax];
		// _LDR_DATA_TABLE_ENTRY --> _LDR_DATA_TABLE_ENTRY --> kernel32.dll信息
		mov eax, [eax];
		// kernel32.dll DLLBASE
		mov eax, dword ptr ds : [eax + 0x18];
	}
}

DWORD GetFnAddr(DWORD dwMod, char* strFnName)
{
	IMAGE_DOS_HEADER* pDosHdr = (IMAGE_DOS_HEADER*)dwMod;
	IMAGE_NT_HEADERS* pNtHdr = (IMAGE_NT_HEADERS32*)(dwMod + pDosHdr->e_lfanew);
	IMAGE_OPTIONAL_HEADER* pOptHdr = (IMAGE_OPTIONAL_HEADER*)&pNtHdr->OptionalHeader;
	IMAGE_DATA_DIRECTORY* pExportTable = pOptHdr->DataDirectory;
	IMAGE_EXPORT_DIRECTORY* pIED = (IMAGE_EXPORT_DIRECTORY*)(pExportTable->VirtualAddress + dwMod);

	// 函数地址表
	DWORD* pFnAddrTable = (DWORD*)(pIED->AddressOfFunctions + dwMod);
	// 函数序号表
	WORD* pFnOrdinalTable = (WORD*)(pIED->AddressOfNameOrdinals + dwMod);
	// 函数名称表
	DWORD* pFnNameTable = (DWORD*)(pIED->AddressOfNames + dwMod);
	DWORD dwNumOfFnName = pIED->NumberOfNames;

	for (DWORD i = 0; i < dwNumOfFnName; i++)
	{
		// 函数名字符串
		char* strInFnNameTable{};
		strInFnNameTable = (char*)(pFnNameTable[i] + dwMod);
		//cmp
		if (StrCmp(strFnName, strInFnNameTable))
		{
			WORD wSubScriptInFnAddrTable = pFnOrdinalTable[i];
			return pFnAddrTable[wSubScriptInFnAddrTable] + dwMod;
		}
	}

	return 0;
}



LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hEdtCtrl{};

	switch (uMsg)
	{
		case WM_CREATE:
			{
				HINSTANCE hInstance = (HINSTANCE)GetCurMod();
				hEdtCtrl = CreateWindowExW(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 0, 0, 300, 100, hWnd, (HMENU)0x1000, hInstance, NULL);
			}break;
		case WM_KEYDOWN:
			{
				if (wParam == VK_RETURN)
				{
					WCHAR wcs[MAX_PATH]{};
					GetWindowTextW(hEdtCtrl, wcs, MAX_PATH);
					if (0 == wcscmp(wcs, L"123456"))
					{
						DestroyWindow(hWnd);
					}
					else
					{
						ExitProcess(0);
					}
				}
			}break;
		case WM_DESTROY:
			{
				PostQuitMessage(0);
			}break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

__declspec(naked) void NakedFn()
{
	// 0.特征码 ***********************************************
	__asm
	{
		mov ax, ax;	//66 8B C0
		mov sp, sp;	//66 8B E4
		__emit 0xFF;
		__emit 0xFF;
	}

	// 1. ***********************************************
	NonNakedFn();

	// 2. __jmp OrgOEP ***********************************************
	GetCurMod();
	__asm
	{
		push ecx;

		mov ecx, eax;
		add ecx, 0x14;
		mov ecx, dword ptr ds:[ecx];
		add eax, ecx;

		pop ecx;

		jmp eax;
	}
}

void __stdcall NonNakedFn()
{
	// 0.得到当前hMod ***********************************************
	DWORD dwMod = GetCurMod();

	// 3.密码弹窗 ***********************************************
	PopWnd((HINSTANCE)dwMod);

	// 5.解压缩.text段 ***********************************************

	// 4.解密.text段 ***********************************************
	IMAGE_DOS_HEADER* pDosHdr = (IMAGE_DOS_HEADER*)dwMod;
	IMAGE_NT_HEADERS* pNTHdrs = (IMAGE_NT_HEADERS*)(dwMod + pDosHdr->e_lfanew);
	IMAGE_SECTION_HEADER* pSecHdr = (IMAGE_SECTION_HEADER*)(pNTHdrs + 1);
	DWORD* pAddr = (DWORD*)(pSecHdr->VirtualAddress + dwMod);
	DWORD dwTime = pSecHdr->SizeOfRawData / 4;
	DecodeExeTextSec(pAddr, 0x12345678, dwTime);

	// 1.修复BRB指向的数据 ***********************************************
	RepairBRT(dwMod);

	// 2.修复IIT ***********************************************
	RepairIIT(dwMod);

	// 5. ***********************************************
	// 6. ***********************************************
	// 7. ***********************************************
	// 8. ***********************************************
	// 9. ***********************************************
}

void RepairBRT(DWORD dwMod)
{
	// 得到BRT
	IMAGE_DATA_DIRECTORY BRT{};
	BRT.VirtualAddress = *((DWORD*)(dwMod + 0xa));
	BRT.Size = *((DWORD*)(dwMod + 0xE));

	if (!BRT.VirtualAddress)
		ASSERT(0);
	if (!BRT.Size)
		ASSERT(0);

	// 得到IBR
	IMAGE_BASE_RELOCATION* pIBR = (IMAGE_BASE_RELOCATION*)(BRT.VirtualAddress + dwMod);

	// 修复
	typedef struct _BR_DATA
	{
		WORD dwOff : 12;
		WORD dwType : 4;
	}BR_DATA;

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
					DWORD* pData = (DWORD*)(pDataOfCurRelocBlock[i].dwOff + dwRvaOfCurRelocBlock + dwMod);

					DWORD dwOldProtection{};
					if (!VirtualProtect(pData, 1, PAGE_EXECUTE_READWRITE, &dwOldProtection))
						ASSERT(0);

					*pData -= 0x400000 - dwMod;
				}
			}
			break;
		}

		dwCurSize += dwSizeOfCurRelocBlock;

		if (dwCurSize == BRT.Size)
			break;

		if (dwCurSize > BRT.Size)
			ASSERT(0);

		pIBR = (IMAGE_BASE_RELOCATION*)((DWORD)pIBR + dwSizeOfCurRelocBlock);
	}
}

void RepairIIT(DWORD dwMod)
{
	// 得到导入表
	IMAGE_DATA_DIRECTORY IIT{};
	IIT.VirtualAddress = *((DWORD*)(dwMod + 0x2));
	IIT.Size = *((DWORD*)(dwMod + 0x6));

	if (!IIT.VirtualAddress)
		ASSERT(0);
	if (!IIT.Size)
		ASSERT(0);

	// 得到导入描述符指针
	IMAGE_IMPORT_DESCRIPTOR* pIID = (IMAGE_IMPORT_DESCRIPTOR*)(IIT.VirtualAddress + dwMod);

	// 修复IAT
	while (pIID->Name)
	{
		char* szDllName = (char*)(pIID->Name + dwMod);
		HMODULE hDllMod = LoadLibraryA(szDllName);

		DWORD* pINT = (DWORD*)(pIID->OriginalFirstThunk + dwMod);
		DWORD* pIAT = (DWORD*)(pIID->FirstThunk + dwMod);
		for (DWORD i = 0; i < 0x10000; i++)
		{
			if (0 == pINT[i])
				break;

			if (pINT[i] >> 31)	// 是序号
			{
				DWORD dwOldProtection{};
				if (!VirtualProtect(pIAT + i, 1, PAGE_EXECUTE_READWRITE, &dwOldProtection))
					ASSERT(0);

				WORD wOrdinal = pINT[i] << 16 >> 16;
				pIAT[i] = (DWORD)GetProcAddress(hDllMod, (char*)wOrdinal);
			}
			else    // 是名称
			{
				DWORD dwOldProtection{};
				if (!VirtualProtect(pIAT + i, 1, PAGE_EXECUTE_READWRITE, &dwOldProtection))
					ASSERT(0);

				IMAGE_IMPORT_BY_NAME* pIIBN = (IMAGE_IMPORT_BY_NAME*)(pINT[i] + dwMod);
				char* szFnName = pIIBN->Name;
				pIAT[i] = (DWORD)GetProcAddress(hDllMod, szFnName);
			}
		}

		pIID++;
	}
}

void PopWnd(HINSTANCE hInstance)
{
	WNDCLASS wc;
	wc.hInstance = (HINSTANCE)GetCurMod();
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = 0;
	wc.hCursor = LoadCursorW(0, IDC_ARROW);
	wc.hIcon = LoadIconW(wc.hInstance, IDI_APPLICATION);
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = L"CN_CLASS";
	wc.lpszMenuName = 0;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClassW(&wc);
	HWND hWnd;
	hWnd = CreateWindowExW(0, wc.lpszClassName, L"WN_NAME", WS_OVERLAPPEDWINDOW, 400, 300, 300, 100, 0, 0, wc.hInstance, 0);
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, 0, 0, 0))
	{
		if (msg.message == WM_KEYDOWN && msg.wParam == VK_RETURN)
			msg.hwnd = hWnd;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void DecodeExeTextSec(DWORD* pAddr, DWORD dwXorData, DWORD dwTime)
{
	for (DWORD i = 0; i < dwTime; i++)
	{
		if (!pAddr[i])
			continue;

		DWORD dwOldProtection{};
		if (!VirtualProtect(pAddr + i, 1, PAGE_EXECUTE_READWRITE, &dwOldProtection))
			ASSERT(0);

		pAddr[i] ^= dwXorData;
	}
}