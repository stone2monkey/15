// ViewThreadsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include <TlHelp32.h>
#include "WinProtection.h"
#include "ViewThreadsDlg.h"
#include "afxdialogex.h"
#include "_CGlobal.h"
#include "Interface.h"
#include "MyThreadData.h"


// CViewThreadsDlg �Ի���

IMPLEMENT_DYNAMIC(CViewThreadsDlg, CDialogEx)

int CViewThreadsDlg::GetCurThreads(vector<CMyThreadData*>& vctMyThreadData)
{
	HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (INVALID_HANDLE_VALUE == hThreadSnap) { ::MessageBoxW(0, L"CreateToolhelp32Snapshot 0 !", L"Exception", MB_ICONERROR); }

	THREADENTRY32 te{};
	te.dwSize = sizeof(THREADENTRY32);

	if (!Thread32First(hThreadSnap, &te)) { CloseHandle(hThreadSnap); return 0; }

	int iCount = -1;
	do 
	{
		iCount++;
		CMyThreadData* p = new CMyThreadData();
		p->m_uTID = te.th32ThreadID;
		vctMyThreadData.push_back(p);
	} while (Thread32Next(hThreadSnap, &te));

	CloseHandle(hThreadSnap);

	return 0;
}

CViewThreadsDlg::CViewThreadsDlg(int iIndexOfProcessinVct, CWnd* pParent /*=NULL*/) :
	m_vctMyThreadData(g_vctMyProcessData.at(iIndexOfProcessinVct)->m_vctMyThreadData),
	CDialogEx(IDDLG_VIEWTHREADS, pParent)
{
	m_iIndexOfProcessinVct = iIndexOfProcessinVct;
	m_hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, g_vctMyProcessData.at(m_iIndexOfProcessinVct)->m_uPID);

}

CViewThreadsDlg::~CViewThreadsDlg()
{
}

void CViewThreadsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDLC_0, m_lc0);
}


BEGIN_MESSAGE_MAP(CViewThreadsDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CViewThreadsDlg::OnBnClickedOk)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_COMMAND(IDSUBMENU2_FRESHENTHREADS, &CViewThreadsDlg::OnFreshenthreads)
END_MESSAGE_MAP()


// CViewThreadsDlg ��Ϣ�������


void CViewThreadsDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// CDialogEx::OnOK();
}


BOOL CViewThreadsDlg::OnInitDialog()
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
		WCHAR* wcsArrHeaderName[1] = { L"TID" };
		CInterface::InitListCtrl(this, &(this->m_lc0), 1, wcsArrHeaderName, 1);

		// 2.�õ��߳�����,����m_lc0 ************************************************************
		GetCurThreads(m_vctMyThreadData);
		int iLoop = (int)m_vctMyThreadData.size();
		for (int i = 0; i < iLoop; i++)
		{
			CStringW wcsTemp{};
			wcsTemp.Format(L"0x%08x -- 0d%08u", m_vctMyThreadData.at(i)->m_uTID, m_vctMyThreadData.at(i)->m_uTID);
			m_lc0.InsertItem(i, wcsTemp);
		}

	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CViewThreadsDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	DestroyWindow();
	delete this;
	g_pAllWnd[1] = nullptr;

	// CDialogEx::OnClose();
}


void CViewThreadsDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������

	// 1.m_lc0�ߴ�����Ӧ ************************************************************
	CInterface::InitListCtrl(this, &(this->m_lc0), 1, nullptr, 0);
}


void CViewThreadsDlg::OnFreshenthreads()
{
	// TODO: �ڴ���������������
}
