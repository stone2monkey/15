
// MyDbger0Dlg.cpp : ʵ���ļ�
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


// CMyDbger0Dlg �Ի���



void CMyDbger0Dlg::ResetReg(DWORD dwTID)
{
	// �õ��߳̾��
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, 0, dwTID);
	if (!hThread) { assert(0); }

	// �õ��̻߳���
	CONTEXT ct{};
	ct.ContextFlags = CONTEXT_ALL;
	if (!::GetThreadContext(hThread, &ct)) { assert(0); }

	// ����Reg����ʾ
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

	// �ر��߳̾��
	CloseHandle(hThread);
}

CMyDbger0Dlg::CMyDbger0Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDDLG_MY_DBGER, pParent)
	, m_wcsLog(_T(""))
	, m_wcsReg(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// ��ʼ��ȫ�ֱ���g_pMyDbger0Dlg;
	if (g_pMyDbger0Dlg) { assert(0); }
	g_pMyDbger0Dlg = this;

	// ����
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


// CMyDbger0Dlg ��Ϣ�������

BOOL CMyDbger0Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	{
		// 0.��󻯴��� ************************************************************
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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMyDbger0Dlg::OnPaint()
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
HCURSOR CMyDbger0Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMyDbger0Dlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// CDialogEx::OnOK();
}


BOOL CMyDbger0Dlg::PreTranslateMessage(MSG* pMsg)
{
	 // F3 (��һ������)
	if (WM_KEYDOWN == pMsg->message && VK_F3 == pMsg->wParam)
	{
		// ѡ�񱻵���exe�ļ�(����������õ������Խ��̵��ļ�����Ϣ)
		CInterface::SelectExeDbged();

		// ���û��ȫ�ļ���,˵��������ϢΪ��,һ��Ϊû��ѡ���ļ�.��ִ�е����߳�.
		if (0 == g_pProcDbgedInfo->wcsFullName.Compare(L"")){ return CDialogEx::PreTranslateMessage(pMsg); }

		// ִ�е����߳�
		if (!CreateThread(0, 0, (LPTHREAD_START_ROUTINE)CThreadProcOfBug::ThreadProcOfBug, NULL, 0, 0)) { assert(0); }
	}

	// F7 (TF����)
	if (WM_KEYDOWN == pMsg->message && VK_F7 == pMsg->wParam && g_bCanChangeTask)
	{
		//�޸�����
		g_iWhichTask = iTaskF7;	
		//���¼�����
		HANDLE hEvnet = OpenEventW(EVENT_ALL_ACCESS, 0, L"g_hEvent");
		if (hEvnet == NULL) { assert(0); }
		//�޸��¼�Ϊ���ź�
		SetEvent(hEvnet);
		CloseHandle(hEvnet);
	}

	// F9 (ִ��)
	if (WM_KEYDOWN == pMsg->message && VK_F9 == pMsg->wParam && g_bCanChangeTask)
	{
		g_iWhichTask = iTaskF9;
		//���¼�����
		HANDLE hEvnet = OpenEventW(EVENT_ALL_ACCESS, 0, L"g_hEvent");
		if (hEvnet == NULL) { assert(0); }
		//�޸��¼�Ϊ���ź�
		SetEvent(hEvnet);
		CloseHandle(hEvnet);
	}

	// F2 (���� | ȡ�� int3)
	if (WM_KEYDOWN == pMsg->message && VK_F2 == pMsg->wParam)
	{
		// �õ���ַ
		DWORD dwIndex = m_lcDisasm.GetSelectionMark();
		if (-1 == dwIndex)
		{
			return MessageBoxW(L"û��ѡ��Ҫ��int3�ϵ��λ��!");
		}

		dwIndex += g_dwIndexOfDisasmList;

		DWORD dwAddr = g_vctDisasmListItemInfo.at(dwIndex)->dwAddr;

		// �ϵ��Ƿ����, λ��
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

		// ����,�Ƿ��ɾ,ɾ
		if (bIsExisting)
		{
			g_pInt3BP->RepairCode(dwAddr);
			g_pInt3BP->RemoveBP(dwAddr);
			CStringW wcsTemp{};
			wcsTemp.Format(L"ɾ��int3:0x%08x��ַ���ϵ�ɹ�!", dwAddr);
			MessageBoxW(wcsTemp);
		}

		// ������,���
		if (!bIsExisting)
		{
			g_pInt3BP->AddBP(dwAddr);
			CStringW wcsTemp{};
			wcsTemp.Format(L"���int3:0x%08x��ַ���ϵ�ɹ�!", dwAddr);
			MessageBoxW(wcsTemp);
		}
	}

	// F4 (���� | ȡ�� HE_BP Ӳ��ִ�жϵ�)
	if (WM_KEYDOWN == pMsg->message && VK_F4 == pMsg->wParam)
	{
		// �õ���ַ
		DWORD dwIndex = m_lcDisasm.GetSelectionMark();
		if (-1 == dwIndex)
		{
			return MessageBoxW(L"û��ѡ��Ҫ��[Ӳ��ִ�жϵ�]��λ��!");
		}

		dwIndex += g_dwIndexOfDisasmList;
		DWORD dwAddr = g_vctDisasmListItemInfo.at(dwIndex)->dwAddr;

		// 
		CStringW wcsTemp{};
		CHardExecuteBP o{};
		if (0 == o.AddBP(g_pProcDbgedInfo->dwTID, dwAddr))
		{
			wcsTemp.Format(L"���Ӳ��ִ�жϵ�:0x%08x��ַ���ϵ�ɹ�!", dwAddr);
			MessageBoxW(wcsTemp);
		}
		else if (1 == o.AddBP(g_pProcDbgedInfo->dwTID, dwAddr))
		{
			if (o.RemoveBP(g_pProcDbgedInfo->dwTID, dwAddr))
			{
				wcsTemp.Format(L"ɾ��Ӳ��ִ�жϵ�:0x%08x��ַ���ϵ�ɹ�!", dwAddr);
				MessageBoxW(wcsTemp);
			}
			else { assert(0); }
		}
		else if (2 == o.AddBP(g_pProcDbgedInfo->dwTID, dwAddr))
		{
			wcsTemp.Format(L"Ӳ���ϵ�����,�������,��ַ��Ҳû��Ӳ���ϵ��ɾ:0x%08x!", dwAddr);
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

			// ���ɼ����пؼ�
			m_lcDisasm.ShowWindow(SW_HIDE);
			m_lcMem.ShowWindow(SW_HIDE);
			m_lcStack.ShowWindow(SW_HIDE);
			m_edtCmd.ShowWindow(SW_HIDE);
			m_lcDisasm.ShowWindow(SW_HIDE);
			::ShowWindow(GetDlgItem(IDEDT_REG)->m_hWnd, SW_HIDE);
			::ShowWindow(GetDlgItem(IDEDT_LOG)->m_hWnd, SW_HIDE);	

			// ����ģ��ؼ�
			CRect rt{};
			GetClientRect(&rt);
			m_lcModuleInof.MoveWindow(&rt);


			// ����
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

			// �ɼ����пؼ�
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
