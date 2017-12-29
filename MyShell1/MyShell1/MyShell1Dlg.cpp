
// MyShell1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyShell1.h"
#include "MyShell1Dlg.h"
#include "afxdialogex.h"
#include "Pack.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyShell1Dlg dialog



CMyShell1Dlg::CMyShell1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MYSHELL1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyShell1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyShell1Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CMyShell1Dlg::OnBnClickedOk)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CMyShell1Dlg message handlers

BOOL CMyShell1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMyShell1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMyShell1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMyShell1Dlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	// CDialogEx::OnOK();
}


void CMyShell1Dlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: Add your message handler code here and/or call default
	char szFullFileName[MAX_PATH];
	if (DragQueryFileA(hDropInfo, 0, szFullFileName, MAX_PATH))
	{
		CPack o;
		o.Init(szFullFileName);
		if (o.OnPack())
			MessageBox(L"加壳成功了!", L"");
		else
			MessageBox(L"Pack faild!", L"Err:");
	}
	CDialogEx::OnDropFiles(hDropInfo);
}
