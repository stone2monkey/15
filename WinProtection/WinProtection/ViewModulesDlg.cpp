// ViewModulesDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include <Psapi.h>
#include "WinProtection.h"
#include "ViewModulesDlg.h"
#include "afxdialogex.h"
#include "Interface.h"
#include "_CGlobal.h"
#include "PEParseDlg.h"


// CViewModulesDlg �Ի���

IMPLEMENT_DYNAMIC(CViewModulesDlg, CDialogEx)

int CViewModulesDlg::GetCurModules(vector<CMyModuleData*>& vctMyModuleData)
{
	// 1.����ģ�� ************************************************************
	HMODULE hArrMods[1024]{};
	DWORD dwModsCount{};
	EnumProcessModules(m_hProc, hArrMods, sizeof(hArrMods), &dwModsCount);
	dwModsCount /= 4;

	// 2.�õ�ģ���� ************************************************************
	for (DWORD i = 0; i < dwModsCount; i++)
	{
		// �õ�ģ����
		WCHAR wcsModName[MAX_PATH]{};
		if (0 == GetModuleBaseNameW(m_hProc, hArrMods[i], wcsModName, MAX_PATH)) { continue; }

		// �õ�����ģ����Ϣ
		MODULEINFO mi{};
		if (0 == ::GetModuleInformation(m_hProc, hArrMods[i], &mi, sizeof(MODULEINFO))) { continue; }

		// ѹ��
		CMyModuleData* p = new CMyModuleData();
		wcscpy_s(p->m_wcsName, MAX_PATH, wcsModName);
		p->m_hMod = (HMODULE)mi.lpBaseOfDll;
		p->m_dwBaseOfDll = (DWORD)mi.lpBaseOfDll;
		p->m_dwSizeOfImage = (DWORD)mi.SizeOfImage;
		p->m_dwEntryPoint = (DWORD)mi.EntryPoint;
		vctMyModuleData.push_back(p);
	}

	return 0;
}

CViewModulesDlg::CViewModulesDlg(int iIndexOfProcessinVct, CWnd* pParent /*=NULL*/) :
	m_vctMyModuleData(g_vctMyProcessData.at(iIndexOfProcessinVct)->m_vctMyModuleData),
CDialogEx(IDDLG_VIEWMODULES, pParent) 
{
	m_iIndexOfProcessinVct = iIndexOfProcessinVct;
	m_hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, g_vctMyProcessData.at(m_iIndexOfProcessinVct)->m_uPID);
}

CViewModulesDlg::~CViewModulesDlg()
{
}

void CViewModulesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDLC_0, m_lc0);
}


BEGIN_MESSAGE_MAP(CViewModulesDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CViewModulesDlg::OnBnClickedOk)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_COMMAND(IDSUBMENU3_FRESHENMODULES, &CViewModulesDlg::OnFreshenmodules)
	ON_COMMAND(IDSUBMENU3_PEPARSE, &CViewModulesDlg::OnPeparse)
	ON_NOTIFY(NM_RCLICK, IDLC_0, &CViewModulesDlg::OnRclickIdlc0)
END_MESSAGE_MAP()


// CViewModulesDlg ��Ϣ�������


void CViewModulesDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// CDialogEx::OnOK();
}


BOOL CViewModulesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	{
		// �Ѵ��ڱ���ĳɽ�����
		CStringW wcsTitleName{};
		GetWindowTextW(wcsTitleName);
		CStringW wcsProcName{};
		wcsProcName.Format(L" [%s]", g_vctMyProcessData[m_iIndexOfProcessinVct]->m_wcsName);
		wcsTitleName += wcsProcName;
		SetWindowTextW(wcsTitleName);


		// 1.��ʼ��m_lc0��ͷ ************************************************************
		WCHAR* wcsArrHeaderName[5] = { L"Name", L"hMod" , L"BaseOfDll", L"SizeOfImage", L"EntryPoint"};
		CInterface::InitListCtrl(this, &(this->m_lc0), 5, wcsArrHeaderName, 1);

		// 2.�õ�ģ������,����m_lc0 ************************************************************
		GetCurModules(m_vctMyModuleData);
		int iLoop = (int)m_vctMyModuleData.size();
		for (int i = 0; i < iLoop; i++)
		{
			m_lc0.InsertItem(i, m_vctMyModuleData.at(i)->m_wcsName);
			CStringW wcsTemp{};
			wcsTemp.Format(L"0x%08x -- 0d%08u", m_vctMyModuleData.at(i)->m_hMod, m_vctMyModuleData.at(i)->m_hMod);
			m_lc0.SetItemText(i, 1, wcsTemp);
			wcsTemp.Format(L"0x%08x -- 0d%08u", m_vctMyModuleData.at(i)->m_dwBaseOfDll, m_vctMyModuleData.at(i)->m_dwBaseOfDll);
			m_lc0.SetItemText(i, 2, wcsTemp);
			wcsTemp.Format(L"0x%08x -- 0d%08u", m_vctMyModuleData.at(i)->m_dwSizeOfImage, m_vctMyModuleData.at(i)->m_dwSizeOfImage);
			m_lc0.SetItemText(i, 3, wcsTemp);
			wcsTemp.Format(L"0x%08x -- 0d%08u", m_vctMyModuleData.at(i)->m_dwEntryPoint, m_vctMyModuleData.at(i)->m_dwEntryPoint);
			m_lc0.SetItemText(i, 4, wcsTemp);
		}

	}


	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CViewModulesDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	DestroyWindow();
	delete this;
	g_pAllWnd[2] = nullptr;

	// CDialogEx::OnClose();
}


void CViewModulesDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������

	// 1.m_lc0�ߴ�����Ӧ ************************************************************
	CInterface::InitListCtrl(this, &(this->m_lc0), 5, nullptr, 0);

}


void CViewModulesDlg::OnFreshenmodules()
{
	// TODO: �ڴ���������������
}


void CViewModulesDlg::OnPeparse()
{
	// TODO: �ڴ���������������

	// 1.����ģ�鴰�� ************************************************************
	if (g_pAllWnd[4])
	{
		// SwitchToThisWindow(g_pAllWnd[4]->m_hWnd, 1);
		g_pAllWnd[4]->SetActiveWindow();
		return;
	}

	// ͨ����ѡ�����,�õ�ģ����, ����PID
	int iIndexOfProcessinVct = m_lc0.GetSelectionMark();
	CMyModuleData* pMyModuleData = m_vctMyModuleData.at(iIndexOfProcessinVct);
	DWORD dwPID = g_vctMyProcessData.at(m_iIndexOfProcessinVct)->m_uPID;

	g_pAllWnd[4] = (CWnd*)new CPEParseDlg(dwPID, pMyModuleData);
	CPEParseDlg* p = (CPEParseDlg*)g_pAllWnd[4];
	p->Create(IDDLG_PEPARSE, GetDesktopWindow());
	p->ShowWindow(SW_SHOW);
}


void CViewModulesDlg::OnRclickIdlc0(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// 1. ************************************************************
	POINT point{};
	GetCursorPos(&point);

	CMenu* pSubMenu = g_pMainMenu->GetSubMenu(3);
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);


	*pResult = 0;
}
