// CircleDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include <Psapi.h>
#include "WinProtection.h"
#include "CircleDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "_CGlobal.h"
#include "Interface.h"


// CCircleDlg �Ի���

IMPLEMENT_DYNAMIC(CCircleDlg, CDialogEx)

void CCircleDlg::FillCPUUsageRatio()
{
	FILETIME ftIdleTime{};
	FILETIME ftKernelTime{};
	FILETIME ftUserTime{};

	// 1.�õ���ǰʱ�� ************************************************************
	if (!::GetSystemTimes(&ftIdleTime, &ftKernelTime, &ftUserTime))
	{::PopupWnd(L"GetSystemTimes");}

	// 2.�õ�ʱ���� ************************************************************
	long long llIdle = CInterface::TimeNum(ftIdleTime, m_ftIdleTime);
	long long llUser = CInterface::TimeNum(ftKernelTime, m_ftKernelTime);
	long long llKernel = CInterface::TimeNum(ftUserTime, m_ftUserTime);

	// 3.ʹ���� ************************************************************
	m_llCPUUsageRatio = (llUser + llKernel) * 100 / (llIdle + llUser + llKernel);

	// 4. ************************************************************
	m_ftIdleTime = ftIdleTime;
	m_ftKernelTime = ftKernelTime;
	m_ftUserTime = ftUserTime;

	// 5.����ڴ�ٷֱ� ************************************************************
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


// CCircleDlg ��Ϣ�������


void CCircleDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// CDialogEx::OnOK();
}


BOOL CCircleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	{
		// 1.���ô��� ************************************************************
		m_oBmp.LoadBitmapW(IDBMP_0);
		m_oBmp.GetBitmap(&m_bmpInfo);
		CRect rt(0, 0, m_bmpInfo.bmWidth, m_bmpInfo.bmHeight);
		MoveWindow(&rt);

		LONG lStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
		SetWindowLong(m_hWnd, GWL_EXSTYLE, lStyle | WS_EX_LAYERED);

		SetLayeredWindowAttributes(RGB(255, 255, 255), 0, LWA_COLORKEY);

		// 2.��ʼ��ʱ�� ************************************************************
		if (!::GetSystemTimes(&m_ftIdleTime, &m_ftKernelTime, &m_ftUserTime)) 
		{ ::PopupWnd(L"GetSystemTimes"); }

		// 3.����timer ************************************************************
		SetTimer(0, 1000, 0);
	}


	return TRUE; 
}


void CCircleDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: �ڴ˴������Ϣ����������
					   // ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()

	CDC dcMem{};
	dcMem.CreateCompatibleDC(&dc);

	dcMem.SelectObject(m_oBmp);
	dc.BitBlt(0, 0, m_bmpInfo.bmWidth, m_bmpInfo.bmHeight, &dcMem, 0, 0, SRCCOPY);


	CFont myFont;
	myFont.CreateFont(
		20, // nHeight ����߶�  
		10, // nWidth ������  
		0, // nEscapement ��ʾ�Ƕ�  
		0, // nOrientation ����Ƕ�  
		FW_NORMAL, // nWeight �����ֵ  
		FALSE, // bItalic �Ƿ���б  
		FALSE, // bUnderline �Ƿ�б��  
		0, // cStrikeOut �Ƿ��ɾ����  
		ANSI_CHARSET, // nCharSet ָ���ַ���  
		OUT_DEFAULT_PRECIS, // nOutPrecision ָ���������  
		CLIP_DEFAULT_PRECIS, // nClipPrecision ָ�����о���  
		DEFAULT_QUALITY, // nQuality ָ���������  
		DEFAULT_PITCH | FF_SWISS, /*nPitchAndFamily �ַ���*/_T("����")); // ָ�������������  
	CFont* oldFont = dc.SelectObject(&myFont);
	dc.SetTextColor(RGB(0, 0, 0));
	/*    hbr = */
	//HBRUSH hbr =  (HBRUSH)GetStockObject(NULL_BRUSH);
	//dc.SelectObject (hbr);  
	//(HBRUSH)GetStockObject(NULL_BRUSH);
	dc.SetBkMode(TRANSPARENT); //����ģʽΪ͸��  
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	SendMessage(WM_SYSCOMMAND, SC_MOVE | HTCAPTION);

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CCircleDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	FillCPUUsageRatio();
	InvalidateRect(0);

	CDialogEx::OnTimer(nIDEvent);
}


void CCircleDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	DestroyWindow();
	delete this;
	g_pAllWnd[7] = nullptr;

	// CDialogEx::OnClose();
}


void CCircleDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// �����ڴ�
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

	// ����仯
	ShowWindow(SW_HIDE);
	Sleep(500);
	ShowWindow(SW_RESTORE);
		

	CDialogEx::OnLButtonDblClk(nFlags, point);
}
