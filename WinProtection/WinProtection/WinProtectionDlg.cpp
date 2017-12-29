
// WinProtectionDlg.cpp : ʵ���ļ�
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


// CWinProtectionDlg �Ի���



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


// CWinProtectionDlg ��Ϣ�������

BOOL CWinProtectionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	{
		m_bIsShow = TRUE;

		// �˵� 
		g_pMainMenu = new CMenu();
		HMENU hMainMenu = LoadMenuW(GetModuleHandleA(NULL), MAKEINTRESOURCEW(IDMENU_0));
		g_pMainMenu->Attach(hMainMenu);

		// ע���ȼ�
		::RegisterHotKey(m_hWnd, 0xa0001, MOD_CONTROL, 0x57);
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CWinProtectionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CWinProtectionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CWinProtectionDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// CDialogEx::OnOK();
}


void CWinProtectionDlg::On0Viewprocesses()
{
	// TODO: �ڴ���������������

	// 1.�������̴��� ************************************************************
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	POINT point1{};
	GetCursorPos(&point1);

	CMenu* pSubMenu = g_pMainMenu->GetSubMenu(0);
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, point1.x, point1.y, this);


	CDialogEx::OnRButtonUp(nFlags, point);
}


void CWinProtectionDlg::OnViewallwindows()
{
	// TODO: �ڴ���������������

	// 1.�������д��� ************************************************************
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
	// 1.���������������� ************************************************************
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
	// 1.�������񴰿� ************************************************************
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
	// 1.����СԲ�򴰿� ************************************************************
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
	// 1.����MD5���� ************************************************************
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
	// TODO: �ڴ����ר�ô����/����û���

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
