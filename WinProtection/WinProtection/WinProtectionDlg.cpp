
// WinProtectionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WinProtection.h"
#include "WinProtectionDlg.h"
#include "afxdialogex.h"
#include "_CGlobal.h"
#include "ViewProcessesDlg.h"
#include "ViewAllWindowsDlg.h"
#include "ClearRubbishDlg.h"
#include "ServiceManagerDlg.h"
#include "MD5Dlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWinProtectionDlg 对话框



CWinProtectionDlg::CWinProtectionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_WINPROTECTION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWinProtectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWinProtectionDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CWinProtectionDlg::OnBnClickedOk)
	ON_COMMAND(IDSUBMENU0_VIEWPROCESSES, &CWinProtectionDlg::On0Viewprocesses)
	ON_WM_RBUTTONUP()
	ON_COMMAND(IDSUBMENU0_VIEWALLWINDOWS, &CWinProtectionDlg::OnViewallwindows)
	ON_COMMAND(IDSUBMENU0_CLEARRUBBISH, &CWinProtectionDlg::OnClearrubbish)
	ON_COMMAND(IDSUBMENU0_SERVICEMANAGER, &CWinProtectionDlg::OnServicemanager)
	ON_COMMAND(ID_32827, &CWinProtectionDlg::On32827)
	ON_COMMAND(IDSUBMENU0_VIEWMD5, &CWinProtectionDlg::OnViewmd5)
END_MESSAGE_MAP()


// CWinProtectionDlg 消息处理程序

BOOL CWinProtectionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	{
		m_bIsShow = TRUE;

		// 菜单 
		g_pMainMenu = new CMenu();
		HMENU hMainMenu = LoadMenuW(GetModuleHandleA(NULL), MAKEINTRESOURCEW(IDMENU_0));
		g_pMainMenu->Attach(hMainMenu);

		// 注册热键
		::RegisterHotKey(m_hWnd, 0xa0001, MOD_CONTROL, 0x57);
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CWinProtectionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CWinProtectionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CWinProtectionDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	// CDialogEx::OnOK();
}


void CWinProtectionDlg::On0Viewprocesses()
{
	// TODO: 在此添加命令处理程序代码

	// 1.弹出进程窗口 ************************************************************
	if (g_pAllWnd[0]) 
	{ 
		// SwitchToThisWindow(g_pAllWnd[0]->m_hWnd, 1);
		g_pAllWnd[0]->SetActiveWindow();
		return; 
	}

	g_pAllWnd[0] = (CWnd*)new CViewProcessesDlg();
	CViewProcessesDlg* p = (CViewProcessesDlg*)g_pAllWnd[0];
	p->Create(IDDLG_VIEWPROCESSES, GetDesktopWindow());
	p->ShowWindow(SW_SHOW);
}


void CWinProtectionDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	POINT point1{};
	GetCursorPos(&point1);

	CMenu* pSubMenu = g_pMainMenu->GetSubMenu(0);
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, point1.x, point1.y, this);


	CDialogEx::OnRButtonUp(nFlags, point);
}


void CWinProtectionDlg::OnViewallwindows()
{
	// TODO: 在此添加命令处理程序代码

	// 1.弹出所有窗口 ************************************************************
	if (g_pAllWnd[3])
	{
		// SwitchToThisWindow(g_pAllWnd[3]->m_hWnd, 1);
		g_pAllWnd[3]->SetActiveWindow();
		return;
	}

	g_pAllWnd[3] = (CWnd*)new CViewAllWindowsDlg();
	CViewAllWindowsDlg* p = (CViewAllWindowsDlg*)g_pAllWnd[3];
	p->Create(IDDLG_VIEWPROCESSES, GetDesktopWindow());
	p->ShowWindow(SW_SHOW);
}


void CWinProtectionDlg::OnClearrubbish()
{
	// 1.弹出清理垃圾窗口 ************************************************************
	if (g_pAllWnd[5])
	{
		// SwitchToThisWindow(g_pAllWnd[3]->m_hWnd, 1);
		g_pAllWnd[5]->SetActiveWindow();
		return;
	}

	g_pAllWnd[5] = (CWnd*)new CClearRubbishDlg();
	CClearRubbishDlg* p = (CClearRubbishDlg*)g_pAllWnd[5];
	p->Create(IDDLG_ClEARUBBISH, GetDesktopWindow());
	p->ShowWindow(SW_SHOW);
}


void CWinProtectionDlg::OnServicemanager()
{
	// 1.弹出服务窗口 ************************************************************
	if (g_pAllWnd[6])
	{
		// SwitchToThisWindow(g_pAllWnd[3]->m_hWnd, 1);
		g_pAllWnd[6]->SetActiveWindow();
		return;
	}

	g_pAllWnd[6] = (CWnd*)new CServiceManagerDlg();
	CServiceManagerDlg* p = (CServiceManagerDlg*)g_pAllWnd[6];
	p->Create(IDDLG_SERVICEMANAGER, GetDesktopWindow());
	p->ShowWindow(SW_SHOW);
}


void CWinProtectionDlg::On32827()
{
	// 1.弹出小圆球窗口 ************************************************************
	if (g_pAllWnd[7])
	{
		// SwitchToThisWindow(g_pAllWnd[3]->m_hWnd, 1);
		g_pAllWnd[7]->SetActiveWindow();
		return;
	}

	g_pAllWnd[7] = (CWnd*)new CCircleDlg();
	CCircleDlg* p = (CCircleDlg*)g_pAllWnd[7];
	p->Create(IDDLG_CIRCLE, GetDesktopWindow());
	p->ShowWindow(SW_SHOW);
	p->SetWindowPos(&wndTopMost, 1830, 700, 0, 0, SWP_NOSIZE);
}


void CWinProtectionDlg::OnViewmd5()
{
	// 1.弹出MD5窗口 ************************************************************
	if (g_pAllWnd[8])
	{
		// SwitchToThisWindow(g_pAllWnd[3]->m_hWnd, 1);
		g_pAllWnd[8]->SetActiveWindow();
		return;
	}

	g_pAllWnd[8] = (CWnd*)new CMD5Dlg();
	CMD5Dlg* p = (CMD5Dlg*)g_pAllWnd[8];
	p->Create(IDDLG_MD5, GetDesktopWindow());
	p->ShowWindow(SW_SHOW);
}


BOOL CWinProtectionDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	if ((pMsg->message == WM_HOTKEY) && (pMsg->wParam == 0xa0001))
	{
		if (m_bIsShow)
		{
			m_bIsShow = 0;
			ShowWindow(SW_HIDE);
		}
		else
		{
			m_bIsShow = 1;
			ShowWindow(SW_SHOWNORMAL);
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
