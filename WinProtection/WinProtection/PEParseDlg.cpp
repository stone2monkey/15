// PEParseDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WinProtection.h"
#include "PEParseDlg.h"
#include "afxdialogex.h"
#include "_CGlobal.h"
#include "Interface.h"


// CPEParseDlg 对话框

IMPLEMENT_DYNAMIC(CPEParseDlg, CDialogEx)

void CPEParseDlg::OnDllChangedInImportTable()
{
	// 1.得到当前选择的DLL名称 ************************************************************
	CStringW wcsDllNameSelected{};
	m_cb0.GetLBText(m_cb0.GetCurSel(), wcsDllNameSelected);
	m_cb0.SetCurSel(m_cb0.GetCurSel());

	// 2.找到该DLL在IT中ID数组中的索引 ************************************************************
	CStringW wcsTemp{};
	int iIndex = -1;
	IMAGE_IMPORT_DESCRIPTOR* p = (IMAGE_IMPORT_DESCRIPTOR*)((m_pPEData->m_pDDT)[1].VirtualAddress + (DWORD)m_hCopyMod);
	while (p->Name)
	{
		wcsTemp = (char*)(p->Name + (DWORD)m_hCopyMod);

		if (0 == wcscmp(wcsDllNameSelected, wcsTemp)) { iIndex++; break; }

		iIndex++;
		p++;
	}

	// 3.找到了,要清空列表数据 ************************************************************
	m_lc0.DeleteAllItems();

	// 4.IAT ************************************************************
	DWORD* IAT = (DWORD*)(p->FirstThunk + (DWORD)m_hCopyMod);
	int iTemp{};
	while (IAT[iTemp])
	{
		wcsTemp.Format(L"0x%08x", IAT[iTemp]);
		m_lc0.InsertItem(iTemp, wcsTemp);

		iTemp++;
	}

	// 5.函数名&序号(INT) ************************************************************
	DWORD* INT = (DWORD*)(p->OriginalFirstThunk + (DWORD)m_hCopyMod);
	iTemp = 0;
	while (INT[iTemp])		// 如果这个地址的属性页没有开放读权限,bang,bang,bang.
	{
		if (INT[iTemp] & 0x80000000)	// 是序号导入
		{
			wcsTemp.Format(L"0x%04x", INT[iTemp] & 0xffff);
			m_lc0.SetItemText(iTemp, 1, wcsTemp);
		}
		else    // 函数名导入
		{
			IMAGE_IMPORT_BY_NAME* pIBN = (IMAGE_IMPORT_BY_NAME*)(INT[iTemp] + (DWORD)m_hCopyMod);
			wcsTemp = pIBN->Name;
			m_lc0.SetItemText(iTemp, 2, wcsTemp);
		}

		iTemp++;
	}
}

void CPEParseDlg::OnTypeChangedInResourceTable()
{
	// 1.得到当前选择的资源type名称 ************************************************************
	CStringW wcsTypeNameSelected{};
	m_cb0.GetLBText(m_cb0.GetCurSel(), wcsTypeNameSelected);
	m_cb0.SetCurSel(m_cb0.GetCurSel());

	// 2.匹配 ************************************************************
	CStringW wcsTemp{};
	// 资源目录
	IMAGE_RESOURCE_DIRECTORY* pRD1 = (IMAGE_RESOURCE_DIRECTORY*)((m_pPEData->m_pDDT)[2].VirtualAddress + (DWORD)m_hCopyMod);
	DWORD dwTypeCount = pRD1->NumberOfIdEntries + pRD1->NumberOfNamedEntries;
	IMAGE_RESOURCE_DIRECTORY_ENTRY* pRDE = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pRD1 + 1);
	DWORD dwIndexOfRDE{};
	BOOL bFound{};
	for (DWORD i = 0; i < dwTypeCount; i++)
	{
		if ((pRDE + i)->NameIsString)	// 如果是字符串
		{
			IMAGE_RESOURCE_DIR_STRING_U* pRDS = (IMAGE_RESOURCE_DIR_STRING_U*)((pRDE + i)->NameOffset + (DWORD)pRD1);
			WCHAR* wcsName = new WCHAR[pRDS->Length + 1]{};
			memcpy_s(wcsName, pRDS->Length + 1, pRDS->NameString, pRDS->Length);
			
			if (0 == wcscmp(wcsTypeNameSelected, wcsName))
			{ 
				dwIndexOfRDE = i;
				bFound = 1;
				break;
			}

			delete[] wcsName;
		}
		else
		{
			wcsTemp.Format(L"0x%04x", (pRDE + i)->Id);
			if (0 == wcscmp(wcsTypeNameSelected, wcsTemp))
			{
				dwIndexOfRDE = i;
				bFound = 1;
				break;
			}
		}
	}

	if (!bFound) { return; }

	// 3.找到了,要清空列表数据 ************************************************************
	m_lc0.DeleteAllItems();

	// 3.第二层资源目录与入口 ************************************************************
	IMAGE_RESOURCE_DIRECTORY_ENTRY* pRDESeleted = (pRDE + dwIndexOfRDE);		// 被选择的类型的资源目录入口
	IMAGE_RESOURCE_DIRECTORY* pRD2 = (IMAGE_RESOURCE_DIRECTORY*)(pRDESeleted->OffsetToDirectory + (DWORD)pRD1);
	IMAGE_RESOURCE_DIRECTORY_ENTRY* pRDE2 = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pRD2 + 1);
	DWORD dwIdCount = pRD2->NumberOfIdEntries + pRD2->NumberOfNamedEntries;
	for (DWORD i = 0; i < dwIdCount; i++)
	{
		// 解析第二层入口 *****************************************
		if ((pRDE2 + i)->NameIsString)	// 如果是字符串
		{
			IMAGE_RESOURCE_DIR_STRING_U* pRDS = (IMAGE_RESOURCE_DIR_STRING_U*)((pRDE2 + i)->NameOffset + (DWORD)pRD1);
			WCHAR* wcsName = new WCHAR[pRDS->Length + 1]{};
			memcpy_s(wcsName, pRDS->Length + 1, pRDS->NameString, pRDS->Length);
			m_lc0.InsertItem(i, wcsTemp);
			delete[] wcsName;
		}
		else
		{
			wcsTemp.Format(L"0x%04x", (pRDE + i)->Id);
			m_lc0.InsertItem(i, wcsTemp);
		}

		// 解析第三层入口 *****************************************
		IMAGE_RESOURCE_DIRECTORY* pRD3 = (IMAGE_RESOURCE_DIRECTORY*)((pRDE2 + i)->OffsetToDirectory + (DWORD)pRD1);
		IMAGE_RESOURCE_DIRECTORY_ENTRY* pRDE3 = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pRD3 + 1);
		IMAGE_RESOURCE_DATA_ENTRY* pRDE = (IMAGE_RESOURCE_DATA_ENTRY*)(pRDE3->OffsetToData + (DWORD)pRD1);
		wcsTemp.Format(L"0x%08x", pRDE->OffsetToData + (DWORD)pRD1 + ((DWORD)m_hOriginalMod - (DWORD)m_hCopyMod));
		m_lc0.SetItemText(i, 1, wcsTemp);
		wcsTemp.Format(L"0x%08x", pRDE->Size);
		m_lc0.SetItemText(i, 2, wcsTemp);

	}
}

void CPEParseDlg::OnStartVAChangedInRelocationTable()
{
	// 1.得到当前选择的重定位VA名称 ************************************************************
	CStringW wcsBRVASelected{};
	m_cb0.GetLBText(m_cb0.GetCurSel(), wcsBRVASelected);
	m_cb0.SetCurSel(m_cb0.GetCurSel());

	// 2.匹配 ************************************************************
	CStringW wcsTemp{};

	// 重定位表首指针
	IMAGE_BASE_RELOCATION* pBR = (IMAGE_BASE_RELOCATION*)((m_pPEData->m_pDDT)[5].VirtualAddress + (DWORD)m_hCopyMod);
	DWORD dwSize = (m_pPEData->m_pDDT)[5].Size;

	IMAGE_BASE_RELOCATION* p = pBR;
	DWORD dwCurSize{};
	BOOL bFound{};
	while (dwCurSize < dwSize)
	{
		wcsTemp.Format(L"0x%08x", p->VirtualAddress + (DWORD)m_hOriginalMod);
		if (0 == wcscmp(wcsBRVASelected, wcsTemp))
		{
			bFound = TRUE;
			break;
		}

		dwCurSize += p->SizeOfBlock;
		p = (IMAGE_BASE_RELOCATION*)((BYTE*)p + p->SizeOfBlock);
	}

	// 3.找到了,要清空列表数据 ************************************************************
	if (bFound) { m_lc0.DeleteAllItems(); }

	// 4.显示 ************************************************************
	WORD* pData = (WORD*)p;
	DWORD dwSizeOfBlock = p->SizeOfBlock;
	for (DWORD i = 0; i < dwSizeOfBlock - 4; i++)
	{
		DWORD dwVA = (pData[i + 4] & 0x0fff) + (DWORD)p + ((DWORD)m_hOriginalMod - (DWORD)m_hCopyMod);
		wcsTemp.Format(L"0x%08x", dwVA);
		m_lc0.InsertItem(i, wcsTemp);

		DWORD dwType = pData[i + 4] & 0xf000;
		dwType >>= 12;
		wcsTemp.Format(L"0x%08x", dwType);
		m_lc0.SetItemText(i, 1, wcsTemp);
	}
}

CPEParseDlg::CPEParseDlg(DWORD PID, CMyModuleData* pMyModuleData, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDDLG_PEPARSE, pParent)
{
	m_pMyModuleData = pMyModuleData;
	m_dwPID = PID;
	m_hOriginalMod = (HMODULE)m_pMyModuleData->m_dwBaseOfDll;
}

CPEParseDlg::~CPEParseDlg()
{
}

void CPEParseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDLC_0, m_lc0);
	DDX_Control(pDX, IDCB_0, m_cb0);
}


BEGIN_MESSAGE_MAP(CPEParseDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CPEParseDlg::OnBnClickedOk)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_RBUTTONUP()
	ON_NOTIFY(NM_RCLICK, IDLC_0, &CPEParseDlg::OnRclickIdlc0)
	ON_COMMAND(IDSUBMENU4_DH, &CPEParseDlg::OnDh)
	ON_COMMAND(IDSUBMENU4_NT, &CPEParseDlg::OnNt)
	ON_COMMAND(IDSUBMENU4_FH, &CPEParseDlg::OnFh)
	ON_COMMAND(IDSUBMENU4_OH, &CPEParseDlg::OnOh)
	ON_COMMAND(IDSUBMENU4_DDT, &CPEParseDlg::OnDdt)
	ON_COMMAND(IDSUBMENU4_SHT, &CPEParseDlg::OnSht)
	ON_COMMAND(IDSUBMENU4_VIEWET, &CPEParseDlg::OnViewet)
	ON_COMMAND(IDSUBMENU4_VIEWIT, &CPEParseDlg::OnViewit)
	ON_COMMAND(IDSUBMENU4_VIEWRT, &CPEParseDlg::OnViewrt)
	ON_COMMAND(IDSUBMENU4_VIEWBRT, &CPEParseDlg::OnViewbrt)
	ON_COMMAND(IDSUBMENU4_VIEWTLST, &CPEParseDlg::OnViewtlst)
	ON_COMMAND(IDSUBMENU4_VIEWDLT, &CPEParseDlg::OnViewdlt)
	ON_CBN_SELCHANGE(IDCB_0, &CPEParseDlg::OnSelchangeIdcb0)
END_MESSAGE_MAP()


// CPEParseDlg 消息处理程序


void CPEParseDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	// CDialogEx::OnOK();
}


BOOL CPEParseDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SYSTEM_INFO si{};
	::GetSystemInfo(&si);
	m_dwPageSize = si.dwPageSize;
	HANDLE hProc = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, 0, m_dwPID);

	{	// 得到PE头信息
		BYTE* ch = new BYTE[m_pMyModuleData->m_dwSizeOfImage]{};
		ReadProcessMemory(hProc, m_hOriginalMod, ch, m_dwPageSize, 0);
		m_pPEData = new CPEData((IMAGE_DOS_HEADER*)ch);
	}

	{	// 得到各区段信息,以在内存中的镜像尺寸copy到m_pPEData中
		for (DWORD i = 0; i < m_pPEData->m_dwSectionCount; i++)
		{
			DWORD dwStartVA = (m_pPEData->m_pSHT + i)->VirtualAddress + (DWORD)m_hOriginalMod;
			DWORD dwSize = (m_pPEData->m_pSHT + i)->Misc.VirtualSize;

			DWORD dwOldProtect{};
			if (0 == VirtualProtectEx(hProc, (LPVOID)dwStartVA, dwSize, PAGE_EXECUTE_READWRITE, &dwOldProtect))
			{ ::MessageBoxW(0, L"VirtualProtectEx", L"Exception", MB_ICONERROR); }

			BYTE* p = (BYTE*)m_pPEData->m_pDH + (m_pPEData->m_pSHT + i)->VirtualAddress;
			ReadProcessMemory(hProc, (LPCVOID)dwStartVA, (LPVOID)p, dwSize, 0);

			if (0 == VirtualProtectEx(hProc, (LPVOID)dwStartVA, dwSize, dwOldProtect, &dwOldProtect))
			{::MessageBoxW(0, L"VirtualProtectEx", L"Exception", MB_ICONERROR);}
		}
	}

	m_hCopyMod = (HMODULE)m_pPEData->m_pDH;

	{	// 1.初始化m_lc0 ************************************************************
		m_lc0.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	}

	CStringW wcsTemp{};
	GetWindowTextW(wcsTemp);
	wcsTemp.Format(L"%s [0x%08x]", wcsTemp, (DWORD)m_hOriginalMod);
	SetWindowTextW(wcsTemp);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CPEParseDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	DestroyWindow();
	delete this;
	g_pAllWnd[4] = nullptr;

	// CDialogEx::OnClose();
}


void CPEParseDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	// 1.m_lc0尺寸自适应 ************************************************************
	CInterface::InitListCtrl(this, &(this->m_lc0), 2, nullptr, 0);
}


void CPEParseDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnRButtonUp(nFlags, point);
}


void CPEParseDlg::OnRclickIdlc0(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	// 1. ************************************************************
	POINT point{};
	GetCursorPos(&point);

	CMenu* pSubMenu = g_pMainMenu->GetSubMenu(4);
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);


	*pResult = 0;
}


void CPEParseDlg::OnDh()
{
	m_dwCBCurOwner = 0;

	// TODO: 在此添加命令处理程序代码
	// 1.清除列表 ************************************************************
	CInterface::CleanListCtrl(&m_lc0);

	int iCount = m_cb0.GetCount();
	for (int i = 0; i < iCount; i++) { m_cb0.DeleteString(0); }

	// 2.设置列表 ************************************************************
	WCHAR* wcsArrHeaderName[2] = { L"Field", L"Value" };
	CInterface::InitListCtrl(this, &m_lc0, 2, wcsArrHeaderName, 1);

	// 3. ************************************************************
	CStringW wcsTemp{};

	m_lc0.InsertItem(0, L"e_magic");
	wcsTemp.Format(L"0x%04x", m_pPEData->m_pDH->e_magic);
	m_lc0.SetItemText(0, 1, wcsTemp);

	m_lc0.InsertItem(1, L"e_lfanew");
	wcsTemp.Format(L"0x%04x", m_pPEData->m_pDH->e_lfanew);
	m_lc0.SetItemText(1, 1, wcsTemp);
}

void CPEParseDlg::OnNt()
{
	m_dwCBCurOwner = 0;

	// 1.清除列表 ************************************************************
	CInterface::CleanListCtrl(&m_lc0);

	int iCount = m_cb0.GetCount();
	for (int i = 0; i < iCount; i++) { m_cb0.DeleteString(0); }

	// 2.设置列表 ************************************************************
	WCHAR* wcsArrHeaderName[2] = { L"Field", L"Value" };
	CInterface::InitListCtrl(this, &m_lc0, 2, wcsArrHeaderName, 1);

	// 3. ************************************************************
	CStringW wcsTemp{};

	m_lc0.InsertItem(0, L"Signature");
	wcsTemp.Format(L"0x%08x", m_pPEData->m_pNT->Signature);
	m_lc0.SetItemText(0, 1, wcsTemp);
}


void CPEParseDlg::OnFh()
{
	m_dwCBCurOwner = 0;

	// 1.清除列表 ************************************************************
	CInterface::CleanListCtrl(&m_lc0);

	int iCount = m_cb0.GetCount();
	for (int i = 0; i < iCount; i++) { m_cb0.DeleteString(0); }

	// 2.设置列表 ************************************************************
	WCHAR* wcsArrHeaderName[2] = { L"Field", L"Value" };
	CInterface::InitListCtrl(this, &m_lc0, 2, wcsArrHeaderName, 1);

	// 3. ************************************************************
	CStringW wcsTemp{};

	m_lc0.InsertItem(0, L"Machine");
	wcsTemp.Format(L"0x%04x", m_pPEData->m_pFH->Machine);
	m_lc0.SetItemText(0, 1, wcsTemp);

	m_lc0.InsertItem(1, L"NumberOfSections");
	wcsTemp.Format(L"0x%04x", m_pPEData->m_pFH->NumberOfSections);
	m_lc0.SetItemText(1, 1, wcsTemp);

	m_lc0.InsertItem(2, L"SizeOfOptionalHeader");
	wcsTemp.Format(L"0x%04x", m_pPEData->m_pFH->SizeOfOptionalHeader);
	m_lc0.SetItemText(2, 1, wcsTemp);
}


void CPEParseDlg::OnOh()
{
	m_dwCBCurOwner = 0;

	// 1.清除列表 ************************************************************
	CInterface::CleanListCtrl(&m_lc0);

	int iCount = m_cb0.GetCount();
	for (int i = 0; i < iCount; i++) { m_cb0.DeleteString(0); }

	// 2.设置列表 ************************************************************
	WCHAR* wcsArrHeaderName[2] = { L"Field", L"Value" };
	CInterface::InitListCtrl(this, &m_lc0, 2, wcsArrHeaderName, 1);

	// 3. ************************************************************
	CStringW wcsTemp{};

	m_lc0.InsertItem(0, L"Magic");
	wcsTemp.Format(L"0x%04x", m_pPEData->m_pOH->Magic);
	m_lc0.SetItemText(0, 1, wcsTemp);

	m_lc0.InsertItem(1, L"SizeOfCode");
	wcsTemp.Format(L"0x%08x", m_pPEData->m_pOH->SizeOfCode);
	m_lc0.SetItemText(1, 1, wcsTemp);

	m_lc0.InsertItem(2, L"AddressOfEntryPoint");
	wcsTemp.Format(L"0x%08x", m_pPEData->m_pOH->AddressOfEntryPoint + (DWORD)m_hOriginalMod);
	m_lc0.SetItemText(2, 1, wcsTemp);

	m_lc0.InsertItem(3, L"ImageBase");
	wcsTemp.Format(L"0x%08x", m_pPEData->m_pOH->ImageBase);
	m_lc0.SetItemText(3, 1, wcsTemp);

	m_lc0.InsertItem(4, L"SectionAlignment");
	wcsTemp.Format(L"0x%08x", m_pPEData->m_pOH->SectionAlignment);
	m_lc0.SetItemText(4, 1, wcsTemp);

	m_lc0.InsertItem(5, L"FileAlignment");
	wcsTemp.Format(L"0x%08x", m_pPEData->m_pOH->FileAlignment);
	m_lc0.SetItemText(5, 1, wcsTemp);

	m_lc0.InsertItem(6, L"SizeOfImage");
	wcsTemp.Format(L"0x%08x", m_pPEData->m_pOH->SizeOfImage);
	m_lc0.SetItemText(6, 1, wcsTemp);

	m_lc0.InsertItem(7, L"SizeOfHeaders");
	wcsTemp.Format(L"0x%08x", m_pPEData->m_pOH->SizeOfHeaders);
	m_lc0.SetItemText(7, 1, wcsTemp);

	m_lc0.InsertItem(8, L"NumberOfRvaAndSizes");
	wcsTemp.Format(L"0x%08x", m_pPEData->m_pOH->NumberOfRvaAndSizes);
	m_lc0.SetItemText(8, 1, wcsTemp);

	m_lc0.InsertItem(9, L"DataDirectory");
	wcsTemp.Format(L"0x%08x", (DWORD)m_pPEData->m_pOH->DataDirectory + ((DWORD)m_hOriginalMod - (DWORD)m_hCopyMod));
	m_lc0.SetItemText(9, 1, wcsTemp);

	m_lc0.InsertItem(10, L"BaseOfCode");
	wcsTemp.Format(L"0x%08x", m_pPEData->m_pOH->BaseOfCode + (DWORD)m_hOriginalMod);
	m_lc0.SetItemText(10, 1, wcsTemp);
}


void CPEParseDlg::OnDdt()
{
	m_dwCBCurOwner = 0;

	// 1.清除列表 ************************************************************
	CInterface::CleanListCtrl(&m_lc0);

	int iCount = m_cb0.GetCount();
	for (int i = 0; i < iCount; i++) { m_cb0.DeleteString(0); }

	// 2.设置列表 ************************************************************
	WCHAR* wcsArrHeaderName[3] = { L"", L"RVA", L"SIZE" };
	CInterface::InitListCtrl(this, &m_lc0, 3, wcsArrHeaderName, 1);

	// 3. ************************************************************
	CStringW wcsTemp{};

	WCHAR* wcsTN[16] = { L"ET", L"IT", L"RT", L"ExT", L"ST", L"BLT", L"DT", L"CRT", L"GPT", L"TSLT", L"LCT", L"BIT", L"IAT", L"DLIT", L"COMT", L"" };
	
	for (int i = 0; i < 16; i++)
	{
		m_lc0.InsertItem(i, wcsTN[i]);
		wcsTemp.Format(L"0x%08x", m_pPEData->m_pDDT[i].VirtualAddress);
		m_lc0.SetItemText(i, 1, wcsTemp);
		wcsTemp.Format(L"0x%08x", m_pPEData->m_pDDT[i].Size);
		m_lc0.SetItemText(i, 2, wcsTemp);
	}
}


void CPEParseDlg::OnSht()
{
	m_dwCBCurOwner = 0;

	// 1.清除列表 ************************************************************
	CInterface::CleanListCtrl(&m_lc0);

	int iCount = m_cb0.GetCount();
	for (int i = 0; i < iCount; i++) { m_cb0.DeleteString(0); }

	// 2.设置列表 ************************************************************
	WCHAR* wcsArrHeaderName[5] = { L"Name", L"RVA", L"MISC", L"FOA", L"SIZE" };
	CInterface::InitListCtrl(this, &m_lc0, 5, wcsArrHeaderName, 1);

	// 3. ************************************************************
	CStringW wcsTemp{};

	
	for (DWORD i = 0; i < m_pPEData->m_dwSectionCount; i++)
	{
		char strSectionName[9]{};

		IMAGE_SECTION_HEADER* p = m_pPEData->m_pSHT + i;
		memcpy_s(strSectionName, 9, p->Name, 8);
		wcsTemp = strSectionName;
		m_lc0.InsertItem(i, wcsTemp);
		wcsTemp.Format(L"0x%08x", p->VirtualAddress);
		m_lc0.SetItemText(i, 1, wcsTemp);
		wcsTemp.Format(L"0x%08x", p->Misc.VirtualSize);
		m_lc0.SetItemText(i, 2, wcsTemp);
		wcsTemp.Format(L"0x%08x", p->PointerToRawData);
		m_lc0.SetItemText(i, 3, wcsTemp);
		wcsTemp.Format(L"0x%08x", p->SizeOfRawData);
		m_lc0.SetItemText(i, 4, wcsTemp);
	}
}


void CPEParseDlg::OnViewet()
{
	m_dwCBCurOwner = 0;

	if (!m_pPEData->m_pDDT->VirtualAddress) { return; }

	// 1.清除列表 ************************************************************
	CInterface::CleanListCtrl(&m_lc0);

	m_cb0.Clear();
	int iCount = m_cb0.GetCount();
	for (int i = 0; i < iCount; i++) { m_cb0.DeleteString(0); }

	// 2.设置列表 ************************************************************
	WCHAR* wcsArrHeaderName[3] = { L"FunAddr", L"Ordinals(Subscript)", L"FunName"};
	CInterface::InitListCtrl(this, &m_lc0, 3, wcsArrHeaderName, 1);

	// 3.组合框名字 ************************************************************
	CStringW wcsTemp{};

	IMAGE_EXPORT_DIRECTORY* pED = (IMAGE_EXPORT_DIRECTORY*)(m_pPEData->m_pDDT->VirtualAddress + (DWORD)m_hCopyMod);
	char* strName = (char*)(pED->Name + (DWORD)m_hCopyMod);
	wcsTemp = strName;
	m_cb0.InsertString(0, wcsTemp);
	m_cb0.SetCurSel(0);

	// 4.地址 ************************************************************
	DWORD* EAT = (DWORD*)(pED->AddressOfFunctions + (DWORD)m_hCopyMod);
	for (DWORD i = 0; i < pED->NumberOfFunctions; i++)
	{
		wcsTemp.Format(L"0x%08x", *(EAT + i) + (DWORD)m_hOriginalMod);
		m_lc0.InsertItem(i, wcsTemp);
	}

	// 5.函数名 ************************************************************
	WORD* EOT = (WORD*)(pED->AddressOfNameOrdinals + (DWORD)m_hCopyMod);
	DWORD* ENT = (DWORD*)(pED->AddressOfNames + (DWORD)m_hCopyMod);
	for (DWORD i = 0; i < pED->NumberOfNames; i++)
	{
		wcsTemp = (char*)(*(ENT + i) + (DWORD)m_hCopyMod);
		m_lc0.SetItemText(EOT[i], 2, wcsTemp);
	}

	if (m_cb0.GetCount() > 0) { m_cb0.SetCurSel(0); }
	OnSelchangeIdcb0();
}


void CPEParseDlg::OnViewit()
{
	m_dwCBCurOwner = IMPORTTABLE;

	// 1.清除列表 ************************************************************
	CInterface::CleanListCtrl(&m_lc0);

	m_cb0.Clear();
	int iCount = m_cb0.GetCount();
	for (int i = 0; i < iCount; i++) { m_cb0.DeleteString(0); }

	// 2.设置列表 ************************************************************
	WCHAR* wcsArrHeaderName[3] = { L"FunAddr", L"Ordinals(Subscript)", L"FunName" };
	CInterface::InitListCtrl(this, &m_lc0, 3, wcsArrHeaderName, 1);

	// 3.组合框名字 ************************************************************
	CStringW wcsTemp{};

	DWORD dwImportDescriptorCount{};
	IMAGE_IMPORT_DESCRIPTOR* p = (IMAGE_IMPORT_DESCRIPTOR*)((m_pPEData->m_pDDT)[1].VirtualAddress + (DWORD)m_hCopyMod);
	while (p->Name)
	{
		wcsTemp = (char*)(p->Name + (DWORD)m_hCopyMod);
		m_cb0.InsertString(-1, wcsTemp);

		dwImportDescriptorCount++;
		p++;
	}

	if (m_cb0.GetCount() > 0) { m_cb0.SetCurSel(0); }
	OnSelchangeIdcb0();
}


void CPEParseDlg::OnViewrt()
{
	m_dwCBCurOwner = RESOURCETABLE;

	// 1.清除列表 ************************************************************
	CInterface::CleanListCtrl(&m_lc0);

	m_cb0.Clear();
	int iCount = m_cb0.GetCount();
	for (int i = 0; i < iCount; i++){ m_cb0.DeleteString(0); }

	// 2.设置列表 ************************************************************
	WCHAR* wcsArrHeaderName[3] = { L"ID", L"VA", L"SIZE"};
	CInterface::InitListCtrl(this, &m_lc0, 3, wcsArrHeaderName, 1);

	// 3.组合框名字 ************************************************************
	CStringW wcsTemp{};
	// 资源目录
	IMAGE_RESOURCE_DIRECTORY* pRD = (IMAGE_RESOURCE_DIRECTORY*)((m_pPEData->m_pDDT)[2].VirtualAddress + (DWORD)m_hCopyMod);
	DWORD dwTypeCount = pRD->NumberOfIdEntries + pRD->NumberOfNamedEntries;
	IMAGE_RESOURCE_DIRECTORY_ENTRY* pRDE = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pRD + 1);
	for (DWORD i = 0; i < dwTypeCount; i++)
	{
		if ((pRDE + i)->NameIsString)	// 如果是字符串
		{
			IMAGE_RESOURCE_DIR_STRING_U* pRDS = (IMAGE_RESOURCE_DIR_STRING_U*)((pRDE + i)->NameOffset + (DWORD)pRD);
			WCHAR* wcsName = new WCHAR[pRDS->Length + 1]{};
			memcpy_s(wcsName, pRDS->Length + 1, pRDS->NameString, pRDS->Length);
			m_cb0.InsertString(-1, wcsName);
			delete[] wcsName;
		}
		else
		{
			wcsTemp.Format(L"0x%04x", (pRDE + i)->Id);
			m_cb0.InsertString(-1, wcsTemp);
		}
	}

	if (m_cb0.GetCount() > 0) { m_cb0.SetCurSel(0); }
	OnSelchangeIdcb0();
}


void CPEParseDlg::OnViewbrt()
{
	m_dwCBCurOwner = BASERELOCATIONTABLE;

	// 1.清除列表 ************************************************************
	CInterface::CleanListCtrl(&m_lc0);

	int iCount = m_cb0.GetCount();
	for (int i = 0; i < iCount; i++) { m_cb0.DeleteString(0); }

	// 2.设置列表 ************************************************************
	WCHAR* wcsArrHeaderName[2] = { L"VA", L"Type"};
	CInterface::InitListCtrl(this, &m_lc0, 2, wcsArrHeaderName, 2);

	// 3. ************************************************************
	CStringW wcsTemp{};
	
	// 重定位表首指针
	IMAGE_BASE_RELOCATION* pBR = (IMAGE_BASE_RELOCATION*)((m_pPEData->m_pDDT)[5].VirtualAddress + (DWORD)m_hCopyMod);
	DWORD dwSize = (m_pPEData->m_pDDT)[5].Size;

	IMAGE_BASE_RELOCATION* p = pBR;
	DWORD dwCurSize{};
	while (dwCurSize < dwSize)
	{
		wcsTemp.Format(L"0x%08x", p->VirtualAddress + (DWORD)m_hOriginalMod);
		m_cb0.InsertString(-1, wcsTemp);

		dwCurSize += p->SizeOfBlock;
		p = (IMAGE_BASE_RELOCATION*)((BYTE*)p + p->SizeOfBlock);
	}

	if (m_cb0.GetCount() > 0) { m_cb0.SetCurSel(0); }
	OnSelchangeIdcb0();
}


void CPEParseDlg::OnViewtlst()
{
	m_dwCBCurOwner = 0;

	// 1.清除列表 ************************************************************
	CInterface::CleanListCtrl(&m_lc0);

	int iCount = m_cb0.GetCount();
	for (int i = 0; i < iCount; i++) { m_cb0.DeleteString(0); }

	// 2.设置列表 ************************************************************
	WCHAR* wcsArrHeaderName[1] = { L"VA"};
	CInterface::InitListCtrl(this, &m_lc0, 1, wcsArrHeaderName, 2);

	// 3. ************************************************************
	CStringW wcsTemp{};

	if (!(m_pPEData->m_pDDT)[9].VirtualAddress) { return; }	// 没有tls

	IMAGE_TLS_DIRECTORY* pTLS = (IMAGE_TLS_DIRECTORY*)((m_pPEData->m_pDDT)[9].VirtualAddress + (DWORD)m_hCopyMod);
	DWORD* pTLSCallbacks = (DWORD*)(pTLS->AddressOfCallBacks);
	iCount = 0;
	while (*pTLSCallbacks)
	{
		wcsTemp.Format(L"0x%08x", *pTLSCallbacks);
		m_lc0.InsertItem(iCount, wcsTemp);

		pTLSCallbacks++;
	}
}


void CPEParseDlg::OnViewdlt()
{
	// TODO: 在此添加命令处理程序代码
}


void CPEParseDlg::OnSelchangeIdcb0()
{
	if (IMPORTTABLE == m_dwCBCurOwner)
	{
		OnDllChangedInImportTable();
	}
	else if (RESOURCETABLE == m_dwCBCurOwner)
	{
		OnTypeChangedInResourceTable();
	}
	else if (BASERELOCATIONTABLE == m_dwCBCurOwner)
	{
		OnStartVAChangedInRelocationTable();
	}
}
