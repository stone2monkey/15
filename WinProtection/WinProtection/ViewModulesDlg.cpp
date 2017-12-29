// ViewModulesDlg.cpp : 实现文件
//

#include "stdafx.h"
#include <Psapi.h>
#include "WinProtection.h"
#include "ViewModulesDlg.h"
#include "afxdialogex.h"
#include "Interface.h"
#include "_CGlobal.h"
#include "PEParseDlg.h"


// CViewModulesDlg 对话框

IMPLEMENT_DYNAMIC(CViewModulesDlg, CDialogEx)

int CViewModulesDlg::GetCurModules(vector<CMyModuleData*>& vctMyModuleData)
{
	// 1.遍历模块 ************************************************************
	HMODULE hArrMods[1024]{};
	DWORD dwModsCount{};
	EnumProcessModules(m_hProc, hArrMods, sizeof(hArrMods), &dwModsCount);
	dwModsCount /= 4;

	// 2.得到模块名 ************************************************************
	for (DWORD i = 0; i < dwModsCount; i++)
	{
		// 得到模块名
		WCHAR wcsModName[MAX_PATH]{};
		if (0 == GetModuleBaseNameW(m_hProc, hArrMods[i], wcsModName, MAX_PATH)) { continue; }

		// 得到更多模块信息
		MODULEINFO mi{};
		if (0 == ::GetModuleInformation(m_hProc, hArrMods[i], &mi, sizeof(MODULEINFO))) { continue; }

		// 压入
		CMyModuleData* p = new CMyModuleData();
		wcscpy_s(p->m_wcsName, MAX_PATH, wcsModName);
		p->m_hMod = (HMODULE)mi.lpBaseOfDll;
		p->m_dwBaseOfDll = (DWORD)mi.lpBaseOfDll;
		p->m_dwSizeOfImage = (DWORD)mi.SizeOfImage;
		p->m_dwEntryPoint = (DWORD)mi.EntryPoint;
		vctMyModuleData.push_back(p);
	}

	return 0;
}

CViewModulesDlg::CViewModulesDlg(int iIndexOfProcessinVct, CWnd* pParent /*=NULL*/) :
	m_vctMyModuleData(g_vctMyProcessData.at(iIndexOfProcessinVct)->m_vctMyModuleData),
CDialogEx(IDDLG_VIEWMODULES, pParent) 
{
	m_iIndexOfProcessinVct = iIndexOfProcessinVct;
	m_hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, g_vctMyProcessData.at(m_iIndexOfProcessinVct)->m_uPID);
}

CViewModulesDlg::~CViewModulesDlg()
{
}

void CViewModulesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDLC_0, m_lc0);
}


BEGIN_MESSAGE_MAP(CViewModulesDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CViewModulesDlg::OnBnClickedOk)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_COMMAND(IDSUBMENU3_FRESHENMODULES, &CViewModulesDlg::OnFreshenmodules)
	ON_COMMAND(IDSUBMENU3_PEPARSE, &CViewModulesDlg::OnPeparse)
	ON_NOTIFY(NM_RCLICK, IDLC_0, &CViewModulesDlg::OnRclickIdlc0)
END_MESSAGE_MAP()


// CViewModulesDlg 消息处理程序


void CViewModulesDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	// CDialogEx::OnOK();
}


BOOL CViewModulesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	{
		// 把窗口标题改成进程名
		CStringW wcsTitleName{};
		GetWindowTextW(wcsTitleName);
		CStringW wcsProcName{};
		wcsProcName.Format(L" [%s]", g_vctMyProcessData[m_iIndexOfProcessinVct]->m_wcsName);
		wcsTitleName += wcsProcName;
		SetWindowTextW(wcsTitleName);


		// 1.初始化m_lc0标头 ************************************************************
		WCHAR* wcsArrHeaderName[5] = { L"Name", L"hMod" , L"BaseOfDll", L"SizeOfImage", L"EntryPoint"};
		CInterface::InitListCtrl(this, &(this->m_lc0), 5, wcsArrHeaderName, 1);

		// 2.得到模块数据,插入m_lc0 ************************************************************
		GetCurModules(m_vctMyModuleData);
		int iLoop = (int)m_vctMyModuleData.size();
		for (int i = 0; i < iLoop; i++)
		{
			m_lc0.InsertItem(i, m_vctMyModuleData.at(i)->m_wcsName);
			CStringW wcsTemp{};
			wcsTemp.Format(L"0x%08x -- 0d%08u", m_vctMyModuleData.at(i)->m_hMod, m_vctMyModuleData.at(i)->m_hMod);
			m_lc0.SetItemText(i, 1, wcsTemp);
			wcsTemp.Format(L"0x%08x -- 0d%08u", m_vctMyModuleData.at(i)->m_dwBaseOfDll, m_vctMyModuleData.at(i)->m_dwBaseOfDll);
			m_lc0.SetItemText(i, 2, wcsTemp);
			wcsTemp.Format(L"0x%08x -- 0d%08u", m_vctMyModuleData.at(i)->m_dwSizeOfImage, m_vctMyModuleData.at(i)->m_dwSizeOfImage);
			m_lc0.SetItemText(i, 3, wcsTemp);
			wcsTemp.Format(L"0x%08x -- 0d%08u", m_vctMyModuleData.at(i)->m_dwEntryPoint, m_vctMyModuleData.at(i)->m_dwEntryPoint);
			m_lc0.SetItemText(i, 4, wcsTemp);
		}

	}


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CViewModulesDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	DestroyWindow();
	delete this;
	g_pAllWnd[2] = nullptr;

	// CDialogEx::OnClose();
}


void CViewModulesDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码

	// 1.m_lc0尺寸自适应 ************************************************************
	CInterface::InitListCtrl(this, &(this->m_lc0), 5, nullptr, 0);

}


void CViewModulesDlg::OnFreshenmodules()
{
	// TODO: 在此添加命令处理程序代码
}


void CViewModulesDlg::OnPeparse()
{
	// TODO: 在此添加命令处理程序代码

	// 1.弹出模块窗口 ************************************************************
	if (g_pAllWnd[4])
	{
		// SwitchToThisWindow(g_pAllWnd[4]->m_hWnd, 1);
		g_pAllWnd[4]->SetActiveWindow();
		return;
	}

	// 通过被选择的项,得到模块句柄, 进程PID
	int iIndexOfProcessinVct = m_lc0.GetSelectionMark();
	CMyModuleData* pMyModuleData = m_vctMyModuleData.at(iIndexOfProcessinVct);
	DWORD dwPID = g_vctMyProcessData.at(m_iIndexOfProcessinVct)->m_uPID;

	g_pAllWnd[4] = (CWnd*)new CPEParseDlg(dwPID, pMyModuleData);
	CPEParseDlg* p = (CPEParseDlg*)g_pAllWnd[4];
	p->Create(IDDLG_PEPARSE, GetDesktopWindow());
	p->ShowWindow(SW_SHOW);
}


void CViewModulesDlg::OnRclickIdlc0(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	// 1. ************************************************************
	POINT point{};
	GetCursorPos(&point);

	CMenu* pSubMenu = g_pMainMenu->GetSubMenu(3);
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);


	*pResult = 0;
}
