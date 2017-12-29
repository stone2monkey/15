// ServiceManagerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include <winsvc.h>
#include "WinProtection.h"
#include "ServiceManagerDlg.h"
#include "afxdialogex.h"
#include "Interface.h"
#include "_CGlobal.h"
#include "_CGlobal.h"
#include "MyServiceData.h"


// CServiceManagerDlg �Ի���

IMPLEMENT_DYNAMIC(CServiceManagerDlg, CDialogEx)

int CServiceManagerDlg::GetAllServiceInfo()
{
	// 1.�ͷ� ************************************************************
	DWORD iLoop = m_vctMyServiceData.size();
	for (DWORD i = iLoop -1; i <= 0; i--)
	{
		delete m_vctMyServiceData.at(i);
		m_vctMyServiceData.push_back(m_vctMyServiceData.at(i));
	}

	// 2.�õ� ************************************************************
	DWORD dwNumOfServiec{}, dwSize{};
	EnumServicesStatusExW(m_hSCM1, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL, 0, 0, &dwSize, &dwNumOfServiec, 0, 0);
	LPENUM_SERVICE_STATUS_PROCESSW pESSP = (LPENUM_SERVICE_STATUS_PROCESSW)new char[dwSize] {};
	EnumServicesStatusExW(m_hSCM1, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL, (BYTE*)pESSP, dwSize, &dwSize, &dwNumOfServiec, 0, 0);

	// 3.������vector ************************************************************
	for (DWORD i = 0; i < dwNumOfServiec; i++)
	{


		CMyServiceData* p = new CMyServiceData();
		int iLen = wcslen((pESSP + i)->lpServiceName);
		WCHAR* pch = new WCHAR[iLen + 1]{};
		::memcpy_s(pch, iLen * 2 + 2, (pESSP + i)->lpServiceName, iLen * 2 + 2);
		p->m_lpServiceName = pch;
		p->m_dwCurrentState = (pESSP + i)->ServiceStatusProcess.dwCurrentState;
		p->m_dwServiceType = (pESSP + i)->ServiceStatusProcess.dwServiceType;

		m_vctMyServiceData.push_back(p);
	}

	return 0;
}



void CServiceManagerDlg::UpadateServiceStatus()
{
	if (!m_hSCM1) { return; }

	// ��ʾ����
	DWORD iLoop = m_vctMyServiceData.size();
	for (DWORD i = 0; i < iLoop; i++)
	{
		CMyServiceData* p = m_vctMyServiceData.at(i);

		// �����i��0����
		m_lc0.InsertItem(i, p->m_lpServiceName);

		// �����i��1����
		CStringW wcsTemp{};
		switch (p->m_dwCurrentState)
		{
			case SERVICE_CONTINUE_PENDING:
				wcsTemp = L"SERVICE_CONTINUE_PENDING";
				break;
			case SERVICE_PAUSE_PENDING:
				wcsTemp = L"SERVICE_PAUSE_PENDING ";
				break;
			case SERVICE_PAUSED:
				wcsTemp = L"SERVICE_PAUSED";
				break;
			case SERVICE_RUNNING:
				wcsTemp = L"SERVICE_RUNNING";
				break;
			case SERVICE_START_PENDING:
				wcsTemp = L"SERVICE_START_PENDING";
				break;
			case SERVICE_STOP_PENDING:
				wcsTemp = L"SERVICE_STOP_PENDING";
				break;
			case SERVICE_STOPPED:
				wcsTemp = L"SERVICE_STOPPED";
				break;
		}
		m_lc0.SetItemText(i, 1, wcsTemp);

		// �����i��2����
		switch (p->m_dwServiceType)
		{
			case SERVICE_FILE_SYSTEM_DRIVER:
				wcsTemp = L"SERVICE_FILE_SYSTEM_DRIVER";
				break;
			case SERVICE_KERNEL_DRIVER:
				wcsTemp = L"SERVICE_KERNEL_DRIVER ";
				break;
			case SERVICE_WIN32_OWN_PROCESS:
				wcsTemp = L"SERVICE_WIN32_OWN_PROCESS";
				break;
			case SERVICE_WIN32_SHARE_PROCESS:
				wcsTemp = L"SERVICE_WIN32_SHARE_PROCESS";
				break;
			default:
				wcsTemp = L"";
		}
		m_lc0.SetItemText(i, 2, wcsTemp);
	}
}


CServiceManagerDlg::CServiceManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDDLG_SERVICEMANAGER, pParent)
{

	// 1.�õ�������ƹ�������� ************************************************************
	m_hSCM1 = OpenSCManagerW(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);
	if (!m_hSCM1) { PopupWnd(L"OpenSCManagerW"); }
}

CServiceManagerDlg::~CServiceManagerDlg()
{
	if (!m_hSCM1) 
	{
		CloseServiceHandle(m_hSCM1);
		m_hSCM1 = 0;
	}
}

void CServiceManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDLC_0, m_lc0);
}


BEGIN_MESSAGE_MAP(CServiceManagerDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CServiceManagerDlg::OnBnClickedOk)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_COMMAND(IDSUBMENU5_Stop, &CServiceManagerDlg::OnStop)
	ON_COMMAND(IDSUBMENU5_Startup, &CServiceManagerDlg::OnStartup)
	ON_NOTIFY(NM_RCLICK, IDLC_0, &CServiceManagerDlg::OnRclickIdlc0)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CServiceManagerDlg ��Ϣ�������


void CServiceManagerDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// CDialogEx::OnOK();
}


BOOL CServiceManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	{
		// 1.��ʼ��m_lc0��ͷ ************************************************************
		WCHAR* wcsArrHeaderName[3] = { L"Name", L"Status" , L"Type"};
		CInterface::InitListCtrl(this, &(this->m_lc0), 3, wcsArrHeaderName, 1);

		// 2. ************************************************************
		GetAllServiceInfo();
		UpadateServiceStatus();
	}


	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CServiceManagerDlg::OnClose()
{
	DestroyWindow();
	delete this;
	g_pAllWnd[6] = nullptr;
}


void CServiceManagerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������

	CInterface::InitListCtrl(this, &(this->m_lc0), 3, nullptr, 0);
}


void CServiceManagerDlg::OnStop()
{
	// �õ���ǰѡ����
	int iIndexSelected = m_lc0.GetSelectionMark();

	// �޸�
	CMyServiceData* p = m_vctMyServiceData.at(iIndexSelected);
	if (SERVICE_STOPPED != p->m_dwCurrentState)
	{
		SC_HANDLE hService = OpenServiceW(m_hSCM1, p->m_lpServiceName, SERVICE_ALL_ACCESS);
		if (!hService) { PopupWnd(L"OpenServiceW"); return; }

		SERVICE_STATUS ss{};
		if (!ControlService(hService, SERVICE_CONTROL_STOP, &ss))	// ʧ��
		{
			DWORD dwErrCode = GetLastError();
			CStringW wcsTemp = L"ControlService";
			wcsTemp.AppendFormat(L" 0x%08x", dwErrCode);
			PopupWnd(wcsTemp);

			CloseServiceHandle(hService);
			return;
		}

		p->m_dwCurrentState = ss.dwCurrentState;
		if (!((SERVICE_STOPPED == p->m_dwCurrentState) || (SERVICE_STOP_PENDING == p->m_dwCurrentState)))
		{
			CStringW wcsTemp = L"ControlService";
			PopupWnd(wcsTemp);
			CloseServiceHandle(hService);
			return;
		}

		m_lc0.SetItemText(iIndexSelected, 1, L"SERVICE_STOPPED");

		CloseServiceHandle(hService);
	}
}


void CServiceManagerDlg::OnStartup()
{
	// �õ���ǰѡ����
	int iIndexSelected = m_lc0.GetSelectionMark();

	// �޸�
	CMyServiceData* p = m_vctMyServiceData.at(iIndexSelected);
	if (SERVICE_STOPPED == p->m_dwCurrentState)
	{
		SC_HANDLE hService = OpenServiceW(m_hSCM1, p->m_lpServiceName, SERVICE_ALL_ACCESS);
		if (!hService) { PopupWnd(L"OpenServiceW"); return; }

		if (!StartServiceW(hService, 0, NULL))	// ʧ��
		{ 
			DWORD dwErrCode = GetLastError();
			CStringW wcsTemp = L"StartServiceW";
			wcsTemp.AppendFormat(L" 0x%08x", dwErrCode);
			PopupWnd(wcsTemp);

			CloseServiceHandle(hService);
			return;
		}

		p->m_dwCurrentState = SERVICE_RUNNING;
		m_lc0.SetItemText(iIndexSelected, 1, L"SERVICE_RUNNING");

		CloseServiceHandle(hService);
	}
}


void CServiceManagerDlg::OnRclickIdlc0(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// 1. ************************************************************
	POINT point{};
	GetCursorPos(&point);

	CMenu* pSubMenu = g_pMainMenu->GetSubMenu(5);
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);



	*pResult = 0;
}


void CServiceManagerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ


	CDialogEx::OnTimer(nIDEvent);
}
