// ViewAllWindowsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WinProtection.h"
#include "ViewAllWindowsDlg.h"
#include "afxdialogex.h"
#include "Interface.h"
#include "_CGlobal.h"


// CViewAllWindowsDlg 对话框

IMPLEMENT_DYNAMIC(CViewAllWindowsDlg, CDialogEx)

int CViewAllWindowsDlg::GetAllWindows(vector<CMyWindowData*>& vctMyWindowData)
{
	// 1.得到桌面窗口 ************************************************************
	CWnd* pDesktopWnd = CWnd::GetDesktopWindow();

	// 2.得到所有窗口数据 ************************************************************
	CWnd* pChild = pDesktopWnd->GetWindow(GW_CHILD);
	while (pChild)
	{
		WCHAR* wcsClassName = new WCHAR[MAX_PATH]{};
		::GetClassNameW(pChild->m_hWnd, wcsClassName, MAX_PATH);

		WCHAR* wcsTitleName = new WCHAR[MAX_PATH]{};
		::GetWindowTextW(pChild->m_hWnd, wcsTitleName, MAX_PATH);

		CMyWindowData* p = new CMyWindowData();
		p->m_wcsClassName = wcsClassName;
		p->m_wcsTitleName = wcsTitleName;
		p->m_hWnd = pChild;
		vctMyWindowData.push_back(p);

		pChild = pChild->GetWindow(GW_HWNDNEXT);
	}

	return 0;
}

CViewAllWindowsDlg::CViewAllWindowsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDDLG_VIEWALLWINDOWS, pParent)
{

}

CViewAllWindowsDlg::~CViewAllWindowsDlg()
{
}

void CViewAllWindowsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDLC_0, m_lc0);
}


BEGIN_MESSAGE_MAP(CViewAllWindowsDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CViewAllWindowsDlg::OnBnClickedOk)
	ON_WM_CLOSE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CViewAllWindowsDlg 消息处理程序


void CViewAllWindowsDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	// CDialogEx::OnOK();
}


BOOL CViewAllWindowsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	{
		// 1.初始化m_lc0标头 ************************************************************
		WCHAR* wcsArrHeaderName[3] = { L"TitleName", L"ClassName", L"hWnd"};
		CInterface::InitListCtrl(this, &(this->m_lc0), 3, wcsArrHeaderName, 1);

		// 2.得到窗口数据,插入m_lc0 ************************************************************
		GetAllWindows(g_vctMyWindowData);
		int iLoop = (int)g_vctMyWindowData.size();
		for (int i = 0; i < iLoop; i++)
		{
			m_lc0.InsertItem(i, g_vctMyWindowData.at(i)->m_wcsTitleName);
			m_lc0.SetItemText(i, 1, g_vctMyWindowData.at(i)->m_wcsClassName);
			CStringW wcsTemp{};
			wcsTemp.Format(L"0x%08x -- 0d%08u", g_vctMyWindowData.at(i)->m_hWnd, g_vctMyWindowData.at(i)->m_hWnd);
			m_lc0.SetItemText(i, 2, wcsTemp);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CViewAllWindowsDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	DestroyWindow();
	delete this;
	g_pAllWnd[3] = nullptr;

	// CDialogEx::OnClose();
}


void CViewAllWindowsDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码

	// 1.m_lc0尺寸自适应 ************************************************************
	CInterface::InitListCtrl(this, &(this->m_lc0), 3, nullptr, 0);

}
