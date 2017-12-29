// ViewProcessesDlg.cpp : 实现文件
//

#include "stdafx.h"
#include <Psapi.h>
#include "WinProtection.h"
#include "ViewProcessesDlg.h"
#include "afxdialogex.h"
#include "_CGlobal.h"
#include "Interface.h"
#include "ViewThreadsDlg.h"
#include "ViewModulesDlg.h"
#include "MD5.h"


// CViewProcessesDlg 对话框

IMPLEMENT_DYNAMIC(CViewProcessesDlg, CDialogEx)

int CViewProcessesDlg::GetCurProcesses(vector<CMyProcessData*>& vctMyProcessData)
{
	// 1.遍历进程 ************************************************************
	DWORD dwArrProcsId[1024]{};
	DWORD dwProcsCount{};
	if (0 == EnumProcesses(dwArrProcsId, sizeof(dwArrProcsId), &dwProcsCount)) { ::MessageBoxW(0, L"EnumProcesses 0", L"Exception", MB_ICONERROR); }
	dwProcsCount /= 4;

	// 2.得到进程名
	// GetProcessImageFileNameW()
	for (DWORD i = 0; i <dwProcsCount; i++)
	{
		// 得到进程句柄
		HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, dwArrProcsId[i]);
		if (0 == hProc) { continue; }

		// 得到进程全名
		WCHAR wcsFullProcName[MAX_PATH]{};
		DWORD dwSize = sizeof(wcsFullProcName);
		if (0 == QueryFullProcessImageNameW(hProc, 0, wcsFullProcName,&dwSize)) { continue; }
		// 得到进程名
		WCHAR wcsProcName[MAX_PATH]{};
		CInterface::GetNameFormFullName(wcsProcName, wcsFullProcName);

		// 压入
		CMyProcessData* p = new CMyProcessData();
		wcscpy_s(p->m_wcsName, MAX_PATH, wcsProcName);
		wcscpy_s(p->m_wcsFullName, MAX_PATH, wcsFullProcName);
		p->m_uPID = dwArrProcsId[i];
		g_vctMyProcessData.push_back(p);
	}

	return 0;
}

BOOL CViewProcessesDlg::InitWhiteList()
{
	// 1.打开文件 ************************************************************
	WCHAR* wcsFileName = L"D:\\_Spobt_00\\Documents\\Visual Studio 2017\\Projects\\Rep_WinProtection\\WinProtection\\WinProtection\\res\\WhiteList.txt";
	CFile oFile{};
	if (!oFile.Open(wcsFileName, CFile::modeNoTruncate | CFile::modeRead)) { ::PopupWnd(L"CFile::Open!"); }

	// 2.得到MD5个数 ************************************************************
	ULONGLONG dwNumOfMD5 = oFile.GetLength() / 33;
	if (0 == dwNumOfMD5) { return 1; }

	// 3.清空vector ************************************************************
	DWORD iLoop = g_vctWhiteList.size();
	for (DWORD i = iLoop; i > 0; i--)
	{
		delete[] g_vctWhiteList.at(i - 1);
		g_vctWhiteList.pop_back();
	}

	// 3.压入vector ************************************************************
	for (ULONGLONG i = 0; i < dwNumOfMD5; i++)
	{
		char* strMD5 = new char[33]{};
		oFile.Read(strMD5, 33);
		g_vctWhiteList.push_back(strMD5);
	}

	// 4. ************************************************************
	oFile.Close();

	return 1;
}

CViewProcessesDlg::CViewProcessesDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDDLG_VIEWPROCESSES, pParent)
{

}

CViewProcessesDlg::~CViewProcessesDlg()
{
}

void CViewProcessesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDLC_0, m_lc0);
}


BEGIN_MESSAGE_MAP(CViewProcessesDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CViewProcessesDlg::OnBnClickedOk)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_NOTIFY(NM_RCLICK, IDLC_0, &CViewProcessesDlg::OnRclickIdlc0)
	ON_COMMAND(IDSUBMENU1_VIEWTHREADS, &CViewProcessesDlg::OnViewthreads)
	ON_COMMAND(IDSUBMENU1_VIEWMODULES, &CViewProcessesDlg::OnViewmodules)
	ON_COMMAND(IDSUBMENU1_FRESHENPROCESSES, &CViewProcessesDlg::OnFreshenprocesses)
	ON_COMMAND(IDSUBMENU1_AddToWhiteList, &CViewProcessesDlg::OnAddtowhitelist)
	ON_COMMAND(IDSUBMENU1_CheckWhiteList, &CViewProcessesDlg::OnCheckwhitelist)
END_MESSAGE_MAP()


// CViewProcessesDlg 消息处理程序


void CViewProcessesDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	// CDialogEx::OnOK();
}


void CViewProcessesDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	DestroyWindow();
	delete this;
	g_pAllWnd[0] = nullptr;

	// CDialogEx::OnClose();
}


BOOL CViewProcessesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	{
		// 1.初始化m_lc0标头 ************************************************************
		WCHAR* wcsArrHeaderName[3] = { L"Name", L"FullName", L"PID" };
		CInterface::InitListCtrl(this, &(this->m_lc0), 3, wcsArrHeaderName, 1);

		// 2.得到进程数据,插入m_lc0 ************************************************************
		GetCurProcesses(g_vctMyProcessData);
		int iLoop = (int)g_vctMyProcessData.size();
		for (int i = 0; i < iLoop; i++)
		{
			m_lc0.InsertItem(i, g_vctMyProcessData.at(i)->m_wcsName);
			m_lc0.SetItemText(i, 1, g_vctMyProcessData.at(i)->m_wcsFullName);
			CStringW wcsTemp{};
			wcsTemp.Format(L"0x%08x -- 0d%08u", g_vctMyProcessData.at(i)->m_uPID, g_vctMyProcessData.at(i)->m_uPID);
			m_lc0.SetItemText(i, 2, wcsTemp);
		}

		// 3.初始化白名单 ************************************************************
		InitWhiteList();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CViewProcessesDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码

	// 1.m_lc0尺寸自适应 ************************************************************
	CInterface::InitListCtrl(this, &(this->m_lc0), 3, nullptr, 0);
}


void CViewProcessesDlg::OnRclickIdlc0(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	// 1. ************************************************************
	POINT point{};
	GetCursorPos(&point);

	CMenu* pSubMenu = g_pMainMenu->GetSubMenu(1);
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);

	*pResult = 0;
}


void CViewProcessesDlg::OnViewthreads()
{
	// 1.弹出线程窗口 ************************************************************
	if (g_pAllWnd[1])
	{
		// SwitchToThisWindow(g_pAllWnd[1]->m_hWnd, 1);
		g_pAllWnd[1]->SetActiveWindow();
		return;
	}

	// 被选择的项,也就是进程在vct中的索引
	int iIndexOfProcessinVct = m_lc0.GetSelectionMark();

	g_pAllWnd[1] = (CWnd*)new CViewThreadsDlg(iIndexOfProcessinVct);
	CViewThreadsDlg* p = (CViewThreadsDlg*)g_pAllWnd[1];
	p->Create(IDDLG_VIEWTHREADS, GetDesktopWindow());
	p->ShowWindow(SW_SHOW);
}


void CViewProcessesDlg::OnViewmodules()
{
	// 1.弹出模块窗口 ************************************************************
	if (g_pAllWnd[2])
	{
		// SwitchToThisWindow(g_pAllWnd[2]->m_hWnd, 1);
		g_pAllWnd[2]->SetActiveWindow();
		return;
	}

	// 被选择的项,也就是进程在vct中的索引
	int iIndexOfProcessinVct = m_lc0.GetSelectionMark();

	g_pAllWnd[2] = (CWnd*)new CViewModulesDlg(iIndexOfProcessinVct);
	CViewModulesDlg* p = (CViewModulesDlg*)g_pAllWnd[2];
	p->Create(IDDLG_VIEWMODULES, GetDesktopWindow());
	p->ShowWindow(SW_SHOW);
}


void CViewProcessesDlg::OnFreshenprocesses()
{
	// TODO: 在此添加命令处理程序代码
}


//****************************************************
// Name: CViewProcessesDlg::OnAddtowhitelist
// Func: 添加指定应用文件的MD5到白外单
// RetV: void
//****************************************************
void CViewProcessesDlg::OnAddtowhitelist()
{
	// 被选择的项,也就是进程在vct中的索引
	int iIndexOfProcessinVct = m_lc0.GetSelectionMark();

	// 添加MD5到白名单
	CStringW wcsFullFileName = g_vctMyProcessData.at(iIndexOfProcessinVct)->m_wcsFullName;
	CStringW wcsFullListName = L"D:\\_Spobt_00\\Documents\\Visual Studio 2017\\Projects\\Rep_WinProtection\\WinProtection\\WinProtection\\res\\WhiteList.txt";
	CInterface::AddMD5ToFile(wcsFullFileName, wcsFullListName, this->m_hWnd, g_vctWhiteList);
}


void CViewProcessesDlg::OnCheckwhitelist()
{
	// 被选择的项,也就是进程在vct中的索引
	int iIndexOfProcessinVct = m_lc0.GetSelectionMark();

	// 得到被选择的应用MD5
	CStringW wcsFullFileName = g_vctMyProcessData.at(iIndexOfProcessinVct)->m_wcsFullName;
	char strFullFileName[MAX_PATH]{};
	WideCharToMultiByte(CP_ACP, 0, wcsFullFileName, -1, strFullFileName, MAX_PATH, 0, 0);
	char* strMD5 = new char[33]{};
	CMD5 oMD5{};
	memcpy_s(strMD5, 33, oMD5.md5FileValue(strFullFileName), 33);
	
	// 得到的MD5是否有效
	if (0 == StrCmpA(strMD5, ""))
	{
		::MessageBoxW(m_hWnd, L"获取MD5失败!", L"MD5", 0);
		delete[] strMD5;
		return;
	}

	// MD5是否在名单中
	BOOL bFound = CInterface::IsMD5SpecifiedInMD5List(strMD5, g_vctWhiteList);
	if (!bFound)
	{
		if (IDYES == ::MessageBoxW(m_hWnd, L"应用不在白名单中,是否结束?", L"MD5", MB_YESNO))
		{
			HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, g_vctMyProcessData.at(iIndexOfProcessinVct)->m_uPID);
			if (!hProc) { ::PopupWnd(L"OpenProcess"); }

			::TerminateProcess(hProc, 0);
			m_lc0.DeleteItem(iIndexOfProcessinVct);
			g_vctMyProcessData.erase(g_vctMyProcessData.begin() + iIndexOfProcessinVct);
		}
	}

	// 收尾
	delete[] strMD5;
}

