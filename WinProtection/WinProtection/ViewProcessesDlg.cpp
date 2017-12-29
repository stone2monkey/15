// ViewProcessesDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include <Psapi.h>
#include "WinProtection.h"
#include "ViewProcessesDlg.h"
#include "afxdialogex.h"
#include "_CGlobal.h"
#include "Interface.h"
#include "ViewThreadsDlg.h"
#include "ViewModulesDlg.h"
#include "MD5.h"


// CViewProcessesDlg �Ի���

IMPLEMENT_DYNAMIC(CViewProcessesDlg, CDialogEx)

int CViewProcessesDlg::GetCurProcesses(vector<CMyProcessData*>& vctMyProcessData)
{
	// 1.�������� ************************************************************
	DWORD dwArrProcsId[1024]{};
	DWORD dwProcsCount{};
	if (0 == EnumProcesses(dwArrProcsId, sizeof(dwArrProcsId), &dwProcsCount)) { ::MessageBoxW(0, L"EnumProcesses 0", L"Exception", MB_ICONERROR); }
	dwProcsCount /= 4;

	// 2.�õ�������
	// GetProcessImageFileNameW()
	for (DWORD i = 0; i <dwProcsCount; i++)
	{
		// �õ����̾��
		HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, dwArrProcsId[i]);
		if (0 == hProc) { continue; }

		// �õ�����ȫ��
		WCHAR wcsFullProcName[MAX_PATH]{};
		DWORD dwSize = sizeof(wcsFullProcName);
		if (0 == QueryFullProcessImageNameW(hProc, 0, wcsFullProcName,&dwSize)) { continue; }
		// �õ�������
		WCHAR wcsProcName[MAX_PATH]{};
		CInterface::GetNameFormFullName(wcsProcName, wcsFullProcName);

		// ѹ��
		CMyProcessData* p = new CMyProcessData();
		wcscpy_s(p->m_wcsName, MAX_PATH, wcsProcName);
		wcscpy_s(p->m_wcsFullName, MAX_PATH, wcsFullProcName);
		p->m_uPID = dwArrProcsId[i];
		g_vctMyProcessData.push_back(p);
	}

	return 0;
}

BOOL CViewProcessesDlg::InitWhiteList()
{
	// 1.���ļ� ************************************************************
	WCHAR* wcsFileName = L"D:\\_Spobt_00\\Documents\\Visual Studio 2017\\Projects\\Rep_WinProtection\\WinProtection\\WinProtection\\res\\WhiteList.txt";
	CFile oFile{};
	if (!oFile.Open(wcsFileName, CFile::modeNoTruncate | CFile::modeRead)) { ::PopupWnd(L"CFile::Open!"); }

	// 2.�õ�MD5���� ************************************************************
	ULONGLONG dwNumOfMD5 = oFile.GetLength() / 33;
	if (0 == dwNumOfMD5) { return 1; }

	// 3.���vector ************************************************************
	DWORD iLoop = g_vctWhiteList.size();
	for (DWORD i = iLoop; i > 0; i--)
	{
		delete[] g_vctWhiteList.at(i - 1);
		g_vctWhiteList.pop_back();
	}

	// 3.ѹ��vector ************************************************************
	for (ULONGLONG i = 0; i < dwNumOfMD5; i++)
	{
		char* strMD5 = new char[33]{};
		oFile.Read(strMD5, 33);
		g_vctWhiteList.push_back(strMD5);
	}

	// 4. ************************************************************
	oFile.Close();

	return 1;
}

CViewProcessesDlg::CViewProcessesDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDDLG_VIEWPROCESSES, pParent)
{

}

CViewProcessesDlg::~CViewProcessesDlg()
{
}

void CViewProcessesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDLC_0, m_lc0);
}


BEGIN_MESSAGE_MAP(CViewProcessesDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CViewProcessesDlg::OnBnClickedOk)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_NOTIFY(NM_RCLICK, IDLC_0, &CViewProcessesDlg::OnRclickIdlc0)
	ON_COMMAND(IDSUBMENU1_VIEWTHREADS, &CViewProcessesDlg::OnViewthreads)
	ON_COMMAND(IDSUBMENU1_VIEWMODULES, &CViewProcessesDlg::OnViewmodules)
	ON_COMMAND(IDSUBMENU1_FRESHENPROCESSES, &CViewProcessesDlg::OnFreshenprocesses)
	ON_COMMAND(IDSUBMENU1_AddToWhiteList, &CViewProcessesDlg::OnAddtowhitelist)
	ON_COMMAND(IDSUBMENU1_CheckWhiteList, &CViewProcessesDlg::OnCheckwhitelist)
END_MESSAGE_MAP()


// CViewProcessesDlg ��Ϣ�������


void CViewProcessesDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// CDialogEx::OnOK();
}


void CViewProcessesDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	DestroyWindow();
	delete this;
	g_pAllWnd[0] = nullptr;

	// CDialogEx::OnClose();
}


BOOL CViewProcessesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	{
		// 1.��ʼ��m_lc0��ͷ ************************************************************
		WCHAR* wcsArrHeaderName[3] = { L"Name", L"FullName", L"PID" };
		CInterface::InitListCtrl(this, &(this->m_lc0), 3, wcsArrHeaderName, 1);

		// 2.�õ���������,����m_lc0 ************************************************************
		GetCurProcesses(g_vctMyProcessData);
		int iLoop = (int)g_vctMyProcessData.size();
		for (int i = 0; i < iLoop; i++)
		{
			m_lc0.InsertItem(i, g_vctMyProcessData.at(i)->m_wcsName);
			m_lc0.SetItemText(i, 1, g_vctMyProcessData.at(i)->m_wcsFullName);
			CStringW wcsTemp{};
			wcsTemp.Format(L"0x%08x -- 0d%08u", g_vctMyProcessData.at(i)->m_uPID, g_vctMyProcessData.at(i)->m_uPID);
			m_lc0.SetItemText(i, 2, wcsTemp);
		}

		// 3.��ʼ�������� ************************************************************
		InitWhiteList();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CViewProcessesDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������

	// 1.m_lc0�ߴ�����Ӧ ************************************************************
	CInterface::InitListCtrl(this, &(this->m_lc0), 3, nullptr, 0);
}


void CViewProcessesDlg::OnRclickIdlc0(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// 1. ************************************************************
	POINT point{};
	GetCursorPos(&point);

	CMenu* pSubMenu = g_pMainMenu->GetSubMenu(1);
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);

	*pResult = 0;
}


void CViewProcessesDlg::OnViewthreads()
{
	// 1.�����̴߳��� ************************************************************
	if (g_pAllWnd[1])
	{
		// SwitchToThisWindow(g_pAllWnd[1]->m_hWnd, 1);
		g_pAllWnd[1]->SetActiveWindow();
		return;
	}

	// ��ѡ�����,Ҳ���ǽ�����vct�е�����
	int iIndexOfProcessinVct = m_lc0.GetSelectionMark();

	g_pAllWnd[1] = (CWnd*)new CViewThreadsDlg(iIndexOfProcessinVct);
	CViewThreadsDlg* p = (CViewThreadsDlg*)g_pAllWnd[1];
	p->Create(IDDLG_VIEWTHREADS, GetDesktopWindow());
	p->ShowWindow(SW_SHOW);
}


void CViewProcessesDlg::OnViewmodules()
{
	// 1.����ģ�鴰�� ************************************************************
	if (g_pAllWnd[2])
	{
		// SwitchToThisWindow(g_pAllWnd[2]->m_hWnd, 1);
		g_pAllWnd[2]->SetActiveWindow();
		return;
	}

	// ��ѡ�����,Ҳ���ǽ�����vct�е�����
	int iIndexOfProcessinVct = m_lc0.GetSelectionMark();

	g_pAllWnd[2] = (CWnd*)new CViewModulesDlg(iIndexOfProcessinVct);
	CViewModulesDlg* p = (CViewModulesDlg*)g_pAllWnd[2];
	p->Create(IDDLG_VIEWMODULES, GetDesktopWindow());
	p->ShowWindow(SW_SHOW);
}


void CViewProcessesDlg::OnFreshenprocesses()
{
	// TODO: �ڴ���������������
}


//****************************************************
// Name: CViewProcessesDlg::OnAddtowhitelist
// Func: ���ָ��Ӧ���ļ���MD5�����ⵥ
// RetV: void
//****************************************************
void CViewProcessesDlg::OnAddtowhitelist()
{
	// ��ѡ�����,Ҳ���ǽ�����vct�е�����
	int iIndexOfProcessinVct = m_lc0.GetSelectionMark();

	// ���MD5��������
	CStringW wcsFullFileName = g_vctMyProcessData.at(iIndexOfProcessinVct)->m_wcsFullName;
	CStringW wcsFullListName = L"D:\\_Spobt_00\\Documents\\Visual Studio 2017\\Projects\\Rep_WinProtection\\WinProtection\\WinProtection\\res\\WhiteList.txt";
	CInterface::AddMD5ToFile(wcsFullFileName, wcsFullListName, this->m_hWnd, g_vctWhiteList);
}


void CViewProcessesDlg::OnCheckwhitelist()
{
	// ��ѡ�����,Ҳ���ǽ�����vct�е�����
	int iIndexOfProcessinVct = m_lc0.GetSelectionMark();

	// �õ���ѡ���Ӧ��MD5
	CStringW wcsFullFileName = g_vctMyProcessData.at(iIndexOfProcessinVct)->m_wcsFullName;
	char strFullFileName[MAX_PATH]{};
	WideCharToMultiByte(CP_ACP, 0, wcsFullFileName, -1, strFullFileName, MAX_PATH, 0, 0);
	char* strMD5 = new char[33]{};
	CMD5 oMD5{};
	memcpy_s(strMD5, 33, oMD5.md5FileValue(strFullFileName), 33);
	
	// �õ���MD5�Ƿ���Ч
	if (0 == StrCmpA(strMD5, ""))
	{
		::MessageBoxW(m_hWnd, L"��ȡMD5ʧ��!", L"MD5", 0);
		delete[] strMD5;
		return;
	}

	// MD5�Ƿ���������
	BOOL bFound = CInterface::IsMD5SpecifiedInMD5List(strMD5, g_vctWhiteList);
	if (!bFound)
	{
		if (IDYES == ::MessageBoxW(m_hWnd, L"Ӧ�ò��ڰ�������,�Ƿ����?", L"MD5", MB_YESNO))
		{
			HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, g_vctMyProcessData.at(iIndexOfProcessinVct)->m_uPID);
			if (!hProc) { ::PopupWnd(L"OpenProcess"); }

			::TerminateProcess(hProc, 0);
			m_lc0.DeleteItem(iIndexOfProcessinVct);
			g_vctMyProcessData.erase(g_vctMyProcessData.begin() + iIndexOfProcessinVct);
		}
	}

	// ��β
	delete[] strMD5;
}

