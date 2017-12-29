// CircleDlg.cpp : 实现文件
//

#include "stdafx.h"
#include <Psapi.h>
#include "WinProtection.h"
#include "CircleDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "_CGlobal.h"
#include "Interface.h"


// CCircleDlg 对话框

IMPLEMENT_DYNAMIC(CCircleDlg, CDialogEx)

void CCircleDlg::FillCPUUsageRatio()
{
	FILETIME ftIdleTime{};
	FILETIME ftKernelTime{};
	FILETIME ftUserTime{};

	// 1.得到当前时间 ************************************************************
	if (!::GetSystemTimes(&ftIdleTime, &ftKernelTime, &ftUserTime))
	{::PopupWnd(L"GetSystemTimes");}

	// 2.得到时间数 ************************************************************
	long long llIdle = CInterface::TimeNum(ftIdleTime, m_ftIdleTime);
	long long llUser = CInterface::TimeNum(ftKernelTime, m_ftKernelTime);
	long long llKernel = CInterface::TimeNum(ftUserTime, m_ftUserTime);

	// 3.使用率 ************************************************************
	m_llCPUUsageRatio = (llUser + llKernel) * 100 / (llIdle + llUser + llKernel);

	// 4. ************************************************************
	m_ftIdleTime = ftIdleTime;
	m_ftKernelTime = ftKernelTime;
	m_ftUserTime = ftUserTime;

	// 5.填充内存百分比 ************************************************************
	MEMORYSTATUSEX ms{};
	ms.dwLength = sizeof(MEMORYSTATUSEX);
	if (!::GlobalMemoryStatusEx(&ms))
	{::PopupWnd(L"GlobalMemoryStatusEx");}
	m_dwMemoryRotio = ms.dwMemoryLoad;
}

CCircleDlg::CCircleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDDLG_CIRCLE, pParent)
{

}

CCircleDlg::~CCircleDlg()
{
}

void CCircleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCircleDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// CCircleDlg 消息处理程序


void CCircleDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	// CDialogEx::OnOK();
}


BOOL CCircleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	{
		// 1.设置窗口 ************************************************************
		m_oBmp.LoadBitmapW(IDBMP_0);
		m_oBmp.GetBitmap(&m_bmpInfo);
		CRect rt(0, 0, m_bmpInfo.bmWidth, m_bmpInfo.bmHeight);
		MoveWindow(&rt);

		LONG lStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
		SetWindowLong(m_hWnd, GWL_EXSTYLE, lStyle | WS_EX_LAYERED);

		SetLayeredWindowAttributes(RGB(255, 255, 255), 0, LWA_COLORKEY);

		// 2.初始化时间 ************************************************************
		if (!::GetSystemTimes(&m_ftIdleTime, &m_ftKernelTime, &m_ftUserTime)) 
		{ ::PopupWnd(L"GetSystemTimes"); }

		// 3.设置timer ************************************************************
		SetTimer(0, 1000, 0);
	}


	return TRUE; 
}


void CCircleDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()

	CDC dcMem{};
	dcMem.CreateCompatibleDC(&dc);

	dcMem.SelectObject(m_oBmp);
	dc.BitBlt(0, 0, m_bmpInfo.bmWidth, m_bmpInfo.bmHeight, &dcMem, 0, 0, SRCCOPY);


	CFont myFont;
	myFont.CreateFont(
		20, // nHeight 字体高度  
		10, // nWidth 字体宽度  
		0, // nEscapement 显示角度  
		0, // nOrientation 字体角度  
		FW_NORMAL, // nWeight 字体磅值  
		FALSE, // bItalic 是否倾斜  
		FALSE, // bUnderline 是否斜体  
		0, // cStrikeOut 是否加删除线  
		ANSI_CHARSET, // nCharSet 指定字符集  
		OUT_DEFAULT_PRECIS, // nOutPrecision 指定输出精度  
		CLIP_DEFAULT_PRECIS, // nClipPrecision 指定剪切精度  
		DEFAULT_QUALITY, // nQuality 指定输出质量  
		DEFAULT_PITCH | FF_SWISS, /*nPitchAndFamily 字符族*/_T("宋体")); // 指定字体的字样名  
	CFont* oldFont = dc.SelectObject(&myFont);
	dc.SetTextColor(RGB(0, 0, 0));
	/*    hbr = */
	//HBRUSH hbr =  (HBRUSH)GetStockObject(NULL_BRUSH);
	//dc.SelectObject (hbr);  
	//(HBRUSH)GetStockObject(NULL_BRUSH);
	dc.SetBkMode(TRANSPARENT); //背景模式为透明  
							   //InvalidateRect(NULL,true);
							   //UpdateWindow();

	CStringW wcsTemp{};
	int a = (int)m_llCPUUsageRatio;
	int b = (int)m_dwMemoryRotio;
	wcsTemp.Format(L"%02d  %02d", a, b);
	dc.TextOut(10, 35, wcsTemp);
	dc.SelectObject(oldFont);
}

void CCircleDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	SendMessage(WM_SYSCOMMAND, SC_MOVE | HTCAPTION);

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CCircleDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	FillCPUUsageRatio();
	InvalidateRect(0);

	CDialogEx::OnTimer(nIDEvent);
}


void CCircleDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	DestroyWindow();
	delete this;
	g_pAllWnd[7] = nullptr;

	// CDialogEx::OnClose();
}


void CCircleDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// 清理内存
	DWORD dwArrPID[1000]{}, dwSize = sizeof(dwArrPID), dwNeedSize = 0;
	EnumProcesses(dwArrPID, dwSize, &dwNeedSize);
	for (DWORD i = 0; i < (dwNeedSize / 4); i++)
	{
		HANDLE hProc = OpenProcess(PROCESS_SET_QUOTA, 0, dwArrPID[i]);
		if (!hProc)
		{
			//::PopupWnd(L"OpenProcess");
			continue;
		}

		SetProcessWorkingSetSize(hProc, -1, -1);
		CloseHandle(hProc);
	}

	// 界面变化
	ShowWindow(SW_HIDE);
	Sleep(500);
	ShowWindow(SW_RESTORE);
		

	CDialogEx::OnLButtonDblClk(nFlags, point);
}
