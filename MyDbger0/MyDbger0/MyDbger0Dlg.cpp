
// MyDbger0Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyDbger0.h"
#include "MyDbger0Dlg.h"
#include "afxdialogex.h"
#include "_CGlobal.h"
#include "Interface.h"
#include "ThreadProcOfBug.h"
#include "Cint3BP.h"
#include "HardExecuteBP.h"
#include <TlHelp32.h>
#include "PE.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyDbger0Dlg 对话框



void CMyDbger0Dlg::ResetReg(DWORD dwTID)
{
	// 得到线程句柄
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, 0, dwTID);
	if (!hThread) { assert(0); }

	// 得到线程环境
	CONTEXT ct{};
	ct.ContextFlags = CONTEXT_ALL;
	if (!::GetThreadContext(hThread, &ct)) { assert(0); }

	// 设置Reg并显示
	m_wcsReg = L"";
	m_wcsReg.AppendFormat(L"EAX		%08x\r\n", ct.Eax);
	m_wcsReg.AppendFormat(L"EBX		%08x\r\n", ct.Ebx);
	m_wcsReg.AppendFormat(L"ECX		%08x\r\n", ct.Ecx);
	m_wcsReg.AppendFormat(L"EDX		%08x\r\n\r\n", ct.Edx);

	m_wcsReg.AppendFormat(L"EBP		%08x\r\n", ct.Ebp);
	m_wcsReg.AppendFormat(L"ESP		%08x\r\n", ct.Esp);
	m_wcsReg.AppendFormat(L"ESI		%08x\r\n", ct.Esi);
	m_wcsReg.AppendFormat(L"EDI		%08x\r\n\r\n", ct.Edi);

	m_wcsReg.AppendFormat(L"EIP		%08x\r\n\r\n", ct.Eip);
	m_wcsReg.AppendFormat(L"EFLAGS		%08x\r\n\r\n", ct.EFlags);

	SetDlgItemTextW(IDEDT_REG, m_wcsReg);

	m_dwEip = ct.Eip;

	// 关闭线程句柄
	CloseHandle(hThread);
}

CMyDbger0Dlg::CMyDbger0Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDDLG_MY_DBGER, pParent)
	, m_wcsLog(_T(""))
	, m_wcsReg(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// 初始化全局变量g_pMyDbger0Dlg;
	if (g_pMyDbger0Dlg) { assert(0); }
	g_pMyDbger0Dlg = this;

	// 字体
	if (!m_oFont.CreateFontW(0, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"Fixedsys")) { assert(0); }
}

void CMyDbger0Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDEDT_CMD, m_edtCmd);
	DDX_Text(pDX, IDEDT_LOG, m_wcsLog);
	DDX_Text(pDX, IDEDT_REG, m_wcsReg);
	DDX_Control(pDX, IDLC_DISASM, m_lcDisasm);
	DDX_Control(pDX, IDLC_MEM, m_lcMem);
	DDX_Control(pDX, IDLC_STACK, m_lcStack);
	DDX_Control(pDX, IDLC_MODULE_INFO, m_lcModuleInof);
}

BEGIN_MESSAGE_MAP(CMyDbger0Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CMyDbger0Dlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CMyDbger0Dlg 消息处理程序

BOOL CMyDbger0Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	{
		// 0.最大化窗口 ************************************************************
		ShowWindow(SW_SHOWMAXIMIZED);

		// 1. ************************************************************
		::CreateEventW(0, 1, 0, L"g_hEvent");

		// 2. ************************************************************
		m_lcDisasm.SetFont(&m_oFont);
		m_lcMem.SetFont(&m_oFont);
		m_lcStack.SetFont(&m_oFont);
		GetDlgItem(IDEDT_REG)->SetFont(&m_oFont);
		GetDlgItem(IDEDT_LOG)->SetFont(&m_oFont);
		GetDlgItem(IDEDT_CMD)->SetFont(&m_oFont);

		// 3. ************************************************************
		// 4. ************************************************************
		// 5. ************************************************************
		// 6. ************************************************************
		// 7. ************************************************************
		// 8. ************************************************************
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMyDbger0Dlg::OnPaint()
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
HCURSOR CMyDbger0Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMyDbger0Dlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	// CDialogEx::OnOK();
}


BOOL CMyDbger0Dlg::PreTranslateMessage(MSG* pMsg)
{
	 // F3 (打开一个进程)
	if (WM_KEYDOWN == pMsg->message && VK_F3 == pMsg->wParam)
	{
		// 选择被调试exe文件(这里面可以拿到被调试进程的文件名信息)
		CInterface::SelectExeDbged();

		// 如果没有全文件名,说明进程信息为空,一般为没有选择文件.不执行调试线程.
		if (0 == g_pProcDbgedInfo->wcsFullName.Compare(L"")){ return CDialogEx::PreTranslateMessage(pMsg); }

		// 执行调试线程
		if (!CreateThread(0, 0, (LPTHREAD_START_ROUTINE)CThreadProcOfBug::ThreadProcOfBug, NULL, 0, 0)) { assert(0); }
	}

	// F7 (TF单步)
	if (WM_KEYDOWN == pMsg->message && VK_F7 == pMsg->wParam && g_bCanChangeTask)
	{
		//修改任务
		g_iWhichTask = iTaskF7;	
		//打开事件对象
		HANDLE hEvnet = OpenEventW(EVENT_ALL_ACCESS, 0, L"g_hEvent");
		if (hEvnet == NULL) { assert(0); }
		//修改事件为有信号
		SetEvent(hEvnet);
		CloseHandle(hEvnet);
	}

	// F9 (执行)
	if (WM_KEYDOWN == pMsg->message && VK_F9 == pMsg->wParam && g_bCanChangeTask)
	{
		g_iWhichTask = iTaskF9;
		//打开事件对象
		HANDLE hEvnet = OpenEventW(EVENT_ALL_ACCESS, 0, L"g_hEvent");
		if (hEvnet == NULL) { assert(0); }
		//修改事件为有信号
		SetEvent(hEvnet);
		CloseHandle(hEvnet);
	}

	// F2 (设置 | 取消 int3)
	if (WM_KEYDOWN == pMsg->message && VK_F2 == pMsg->wParam)
	{
		// 得到地址
		DWORD dwIndex = m_lcDisasm.GetSelectionMark();
		if (-1 == dwIndex)
		{
			return MessageBoxW(L"没有选择要下int3断点的位置!");
		}

		dwIndex += g_dwIndexOfDisasmList;

		DWORD dwAddr = g_vctDisasmListItemInfo.at(dwIndex)->dwAddr;

		// 断点是否存在, 位置
		BOOL bIsExisting{};
		DWORD iLoop = g_pInt3BP->m_vctMyInt3BK.size();
		for (DWORD i = 0; i < iLoop; i++)
		{
			if (dwAddr == g_pInt3BP->m_vctMyInt3BK.at(i)->dwAddr)
			{
				bIsExisting = TRUE;
				break;
			}
		}

		// 存在,是否可删,删
		if (bIsExisting)
		{
			g_pInt3BP->RepairCode(dwAddr);
			g_pInt3BP->RemoveBP(dwAddr);
			CStringW wcsTemp{};
			wcsTemp.Format(L"删除int3:0x%08x地址处断点成功!", dwAddr);
			MessageBoxW(wcsTemp);
		}

		// 不存在,添加
		if (!bIsExisting)
		{
			g_pInt3BP->AddBP(dwAddr);
			CStringW wcsTemp{};
			wcsTemp.Format(L"添加int3:0x%08x地址处断点成功!", dwAddr);
			MessageBoxW(wcsTemp);
		}
	}

	// F4 (设置 | 取消 HE_BP 硬件执行断点)
	if (WM_KEYDOWN == pMsg->message && VK_F4 == pMsg->wParam)
	{
		// 得到地址
		DWORD dwIndex = m_lcDisasm.GetSelectionMark();
		if (-1 == dwIndex)
		{
			return MessageBoxW(L"没有选择要下[硬件执行断点]的位置!");
		}

		dwIndex += g_dwIndexOfDisasmList;
		DWORD dwAddr = g_vctDisasmListItemInfo.at(dwIndex)->dwAddr;

		// 
		CStringW wcsTemp{};
		CHardExecuteBP o{};
		if (0 == o.AddBP(g_pProcDbgedInfo->dwTID, dwAddr))
		{
			wcsTemp.Format(L"添加硬件执行断点:0x%08x地址处断点成功!", dwAddr);
			MessageBoxW(wcsTemp);
		}
		else if (1 == o.AddBP(g_pProcDbgedInfo->dwTID, dwAddr))
		{
			if (o.RemoveBP(g_pProcDbgedInfo->dwTID, dwAddr))
			{
				wcsTemp.Format(L"删除硬件执行断点:0x%08x地址处断点成功!", dwAddr);
				MessageBoxW(wcsTemp);
			}
			else { assert(0); }
		}
		else if (2 == o.AddBP(g_pProcDbgedInfo->dwTID, dwAddr))
		{
			wcsTemp.Format(L"硬件断点满了,不能添加,地址处也没有硬件断点可删:0x%08x!", dwAddr);
			MessageBoxW(wcsTemp);
		}
	}
	// Ctrl + M
	else if ((WM_KEYUP == pMsg->message) && ('M' == pMsg->wParam) && ::GetAsyncKeyState(VK_CONTROL))
	{
		static BOOL bActivity = TRUE;

		if (bActivity)
		{
			bActivity = FALSE;

			// 不可见所有控件
			m_lcDisasm.ShowWindow(SW_HIDE);
			m_lcMem.ShowWindow(SW_HIDE);
			m_lcStack.ShowWindow(SW_HIDE);
			m_edtCmd.ShowWindow(SW_HIDE);
			m_lcDisasm.ShowWindow(SW_HIDE);
			::ShowWindow(GetDlgItem(IDEDT_REG)->m_hWnd, SW_HIDE);
			::ShowWindow(GetDlgItem(IDEDT_LOG)->m_hWnd, SW_HIDE);	

			// 设置模块控件
			CRect rt{};
			GetClientRect(&rt);
			m_lcModuleInof.MoveWindow(&rt);


			// 数据
			CStringW wcsTemp{};

			HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, g_pProcDbgedInfo->dwPID);
			if (INVALID_HANDLE_VALUE == hModuleSnap) { assert(0); }

			MODULEENTRY32W ME{};
			ME.dwSize = sizeof(MODULEENTRY32W);
			BOOL bRes = Module32FirstW(hModuleSnap, &ME);
			if (!bRes) { assert(0); }

			m_lcModuleInof.DeleteAllItems();

			int i{};
			do 
			{
				CPE o;
				o.Init((DWORD)ME.hModule, g_pProcDbgedInfo->dwPID);

				m_lcModuleInof.InsertItem(i, L"");

				m_lcModuleInof.SetItemText(i, 0, ME.szModule);

				wcsTemp.Format(L"0x%08x", o.m_dwMod);
				m_lcModuleInof.SetItemText(i, 1, wcsTemp);

				wcsTemp.Format(L"0x%08x", o.m_dwSizeOfMod);
				m_lcModuleInof.SetItemText(i, 2, wcsTemp);

				wcsTemp.Format(L"0x%08x", o.m_dwBaseOfCode);
				m_lcModuleInof.SetItemText(i, 3, wcsTemp);

				wcsTemp.Format(L"0x%08x", o.m_dwSizeOfCode);
				m_lcModuleInof.SetItemText(i, 4, wcsTemp);

				wcsTemp.Format(L"0x%08x", o.m_dwOEP);
				m_lcModuleInof.SetItemText(i, 5, wcsTemp);

				m_lcModuleInof.SetItemText(i, 6, ME.szExePath);

				i++;
				
			} while (Module32NextW(hModuleSnap, &ME));

			m_lcModuleInof.ShowWindow(SW_SHOW);
		}
		else
		{
			bActivity = TRUE;

			// 可见所有控件
			m_lcDisasm.ShowWindow(SW_SHOW);
			m_lcMem.ShowWindow(SW_SHOW);
			m_lcStack.ShowWindow(SW_SHOW);
			m_edtCmd.ShowWindow(SW_SHOW);
			m_lcDisasm.ShowWindow(SW_SHOW);
			::ShowWindow(GetDlgItem(IDEDT_REG)->m_hWnd, SW_SHOW);
			::ShowWindow(GetDlgItem(IDEDT_LOG)->m_hWnd, SW_SHOW);

			m_lcModuleInof.ShowWindow(SW_HIDE);
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
