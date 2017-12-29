// ClearRubbishDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WinProtection.h"
#include "ClearRubbishDlg.h"
#include "afxdialogex.h"
#include "_CGlobal.h"
#include "MyEdit.h"


// CClearRubbishDlg �Ի���

IMPLEMENT_DYNAMIC(CClearRubbishDlg, CDialogEx)

//****************************************************
// Name: CClearRubbishDlg::ClearAllInDir
// Func: ɾ��Ŀ¼����������(��������Ŀ¼)
// RetV: int
// Args: CStringW & wcsDir
//****************************************************
int CClearRubbishDlg::ClearAllInDir(CStringW & wcsDir)
{
	// 1.�ҿ����� ************************************************************
	CFileFind oFF{};
	BOOL bRes = oFF.FindFile(wcsDir + L"\\*.*");
	if (!bRes) { return 1; }

	// 2.���� ************************************************************
	CStringW wcsNameFound{};
	while (bRes)
	{
		bRes = oFF.FindNextFileW();
		// �õ��ҵ���
		wcsNameFound = oFF.GetFileName();

		// �����ϵ�
		if (oFF.IsDots()) { continue; }

		// �ı�����,����ɾ��
		SetFileAttributesW(oFF.GetFilePath(), FILE_ATTRIBUTE_NORMAL);

		// ɾ��
		if (oFF.IsDirectory())
		{
			ClearAllInDir(oFF.GetFilePath());
			RemoveDirectoryW(oFF.GetFilePath());
		}
		else
		{
			DeleteFileW(oFF.GetFilePath());
		}

		// ��ӡ
		// ����Ϊ0Ϊ����,�������Ϊ����
		m_lc0.InsertItem(0x7fffffff, oFF.GetFilePath());
	}
	oFF.Close();

	return 0;
}

int CClearRubbishDlg::ClearFileWithPostfixSpecified(CStringW & wcsDir)
{
	// 1.�ҿ����� ************************************************************
	CFileFind oFF{};
	BOOL bRes = oFF.FindFile(wcsDir + L"\\*.*");
	if (!bRes) { return 1; }

	// 2.���� ************************************************************
	CStringW wcsNameFound{};
	while (bRes)
	{

		bRes = oFF.FindNextFileW();
		// �õ��ҵ���
		wcsNameFound = oFF.GetFileName();

		// �����ϵ�
		if (oFF.IsDots()) { continue; }

		// �ı�����,����ɾ��
		SetFileAttributesW(oFF.GetFilePath(), FILE_ATTRIBUTE_NORMAL);

		// ������ļ���,traverse
		if (oFF.IsDirectory())
		{
			ClearFileWithPostfixSpecified(oFF.GetFilePath());
		}
		
		// ������ļ�
		//�õ���ǰ��׺
		CStringW wcsPostfix{};
		if (GetPostfix(oFF.GetFilePath(), wcsPostfix)) { continue; }
		wcsPostfix.Insert(wcslen(wcsPostfix), L'?');
		//�����ָ����׺
		UpdateData();
		if (-1 != m_edt0.m_wcsPostfix.Find(wcsPostfix))	// ��׺����
		{
			DeleteFileW(oFF.GetFilePath());

			// ��ӡ
			// ����Ϊ0Ϊ����,�������Ϊ����
			m_lc0.InsertItem(0x7fffffff, oFF.GetFilePath());
		}	
	}
	oFF.Close();

	return 0;
}

//****************************************************
// Name: CClearRubbishDlg::GetPostfix
// Func: �õ�һ���ַ����ĺ�׺
// RetV: int	��Ϊ0ʱ,����ļ�û�к�׺
// Args: CStringW & wcsName
// Args: CStringW & wcsPostfix
//****************************************************
int CClearRubbishDlg::GetPostfix(CStringW & wcsName, CStringW& wcsPostfix)
{
	wcsPostfix = wcsName;
	wcsPostfix.MakeReverse();

	int iCount = wcsPostfix.Replace(L'.', L'\0');
	if (!iCount) { return 1; }

	wcsPostfix.MakeReverse();

	return 0;
}

int CClearRubbishDlg::ClearVS(CStringW & wcsDir)
{
	BOOL bProjectName{};	// �ҵ�����
	BOOL bFolderName{};		// �ҵ��빤��ͬ���ļ���
	CStringW wcsProjectName{};	// �ļ���
	CStringW wcsFolderName{};//�ļ��� 

	// 1.�ҿ����� ************************************************************
	CFileFind oFF{};
	BOOL bRes = oFF.FindFile(wcsDir + L"\\*.*");
	if (!bRes) { return 1; }

	// 2.����,�ҵ�����,�õ��ļ���,û�к�׺�� ************************************************************
	CStringW wcsNameFound{};
	while (bRes)
	{
		bRes = oFF.FindNextFileW();
		// �õ��ҵ���
		wcsNameFound = oFF.GetFileName();

		// �����ϵ�
		if (oFF.IsDots()) { continue; }
		if (oFF.IsDirectory()) { continue; }

		// ������ļ�
		//�õ���ǰ��׺
		CStringW wcsPostfix{};
		if (GetPostfix(oFF.GetFilePath(), wcsPostfix)) { continue; }
		//�����ָ����׺
		if (0 == wcsPostfix.Compare(L"sln"))
		{
			wcsProjectName = wcsNameFound;
			wcsFolderName = wcsProjectName;
			wcsFolderName.Replace(L'.', L'\0');
			bProjectName = TRUE;
			break;
		}
	}
	oFF.Close();

	// 3.���ļ��� ************************************************************
	if (!bProjectName) { return 2; }

	CFileFind oFF1{};
	bRes = oFF1.FindFile(wcsDir + L"\\*.*");
	if (!bRes) { return 1; }

	while (bRes)
	{
		bRes = oFF1.FindNextFileW();
		// �õ��ҵ���
		wcsNameFound = oFF1.GetFileName();

		// �����ϵ�
		if (oFF1.IsDots()) { continue; }

		//
		if (oFF1.IsDirectory())
		{
			if (0 == wcsNameFound.Compare(wcsFolderName))
			{
				bFolderName = TRUE;
				break;
			}
		}
	}
	oFF1.Close();

	// 4.û���ҵ�������ͬ���ļ���,�˳� ************************************************************
	if (!(bProjectName && bFolderName)) { return 3; }

	// 5.ɾ������������ͬ���ļ�����,�������ļ� ************************************************************
	CFileFind oFF2{};
	bRes = oFF2.FindFile(wcsDir + L"\\*.*");
	if (!bRes) { return 1; }
	while (bRes)
	{

		bRes = oFF2.FindNextFileW();
		// �õ��ҵ���
		wcsNameFound = oFF2.GetFileName();

		// �����ϵ�
		if (oFF2.IsDots()) { continue; }

		// �ı�����,ʹ��ɾ��
		SetFileAttributesW(oFF2.GetFilePath(), FILE_ATTRIBUTE_NORMAL);

		// ������ļ���,traverse
		if (oFF2.IsDirectory())
		{
			if (0 == wcsFolderName.Compare(wcsNameFound)) { continue; }

			ClearAllInDir(oFF2.GetFilePath());
			RemoveDirectoryW(oFF2.GetFilePath());
			m_lc0.InsertItem(0x7fffffff, oFF2.GetFilePath());
		}
		else  // ������ļ�
		{
			if (0 == wcsProjectName.Compare(wcsNameFound)) { continue; }

			DeleteFileW(oFF2.GetFilePath());
			m_lc0.InsertItem(0x7fffffff, oFF2.GetFilePath());
		}
	}
	oFF2.Close();

	// 6.ɾ��ͬ������Ŀ¼�е�Debug Release x64 ************************************************************
	int iLen = wcslen(wcsFolderName);
	WCHAR* ch = new WCHAR[iLen * 2 + 2]{};
	memcpy_s(ch, iLen * 2 + 2, wcsFolderName, iLen * 2 + 2);
	CStringW w0 = wcsDir + L"\\" + ch + L"\\" + L"Debug";
	CStringW w1 = wcsDir + L"\\" + ch + L"\\" + L"Release";
	CStringW w2 = wcsDir + L"\\" + ch + L"\\" + L"x64";
	ClearAllInDir(w0);
	ClearAllInDir(w1);
	ClearAllInDir(w2);

	if (RemoveDirectoryW(w0))
	{
		m_lc0.InsertItem(0x7fffffff, w0);
	}

	if (RemoveDirectoryW(w1))
	{
		m_lc0.InsertItem(0x7fffffff, w1);
	}

	if (RemoveDirectoryW(w2))
	{
		m_lc0.InsertItem(0x7fffffff, w2);
	}

	return 0;
}


CClearRubbishDlg::CClearRubbishDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDDLG_ClEARUBBISH, pParent)
	, m_iIndexSelected(0)
	, m_wcsEdt0(_T(""))
{

}

CClearRubbishDlg::~CClearRubbishDlg()
{
}

void CClearRubbishDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDCB_0, m_iIndexSelected);
	DDX_Control(pDX, IDCB_0, m_cb0);
	DDX_Control(pDX, IDEDT_0, m_edt0);
	DDX_Control(pDX, IDLC_0, m_lc0);
	DDX_Text(pDX, IDEDT_0, m_wcsEdt0);
}


BEGIN_MESSAGE_MAP(CClearRubbishDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CClearRubbishDlg::OnBnClickedOk)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDCB_0, &CClearRubbishDlg::OnSelchangeIdcb0)
	ON_BN_CLICKED(IDBTN_0, &CClearRubbishDlg::OnBnClicked0)
END_MESSAGE_MAP()


// CClearRubbishDlg ��Ϣ�������


void CClearRubbishDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// CDialogEx::OnOK();
}


BOOL CClearRubbishDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	{	// ��ʼ�������б�
		WCHAR* wcsTemp[4] = { L"��������", L"ϵͳ����", L"���������", L"VS����" };
		for (int i = 0; i < 4; i++) { m_cb0.InsertString(-1, wcsTemp[i]); }
		m_cb0.SetCurSel(0);
		m_iIndexSelected = 0;
		OnSelchangeIdcb0();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CClearRubbishDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	DestroyWindow();
	delete this;
	g_pAllWnd[5] = nullptr;

	// CDialogEx::OnClose();
}


void CClearRubbishDlg::OnSelchangeIdcb0()
{
	m_iIndexSelected = m_cb0.GetCurSel();

	switch (m_iIndexSelected)
	{
		case POSTFIX:
			{
				m_edt0.SetCueBanner(L"����������Ҫ����ĺ�׺(�����׺ʱ, ÿ����׺�����\"?\")!˫����ѡ��Ҫ�����Ŀ¼!");
			}break;
		case SYSTEM:
			{
				m_edt0.SetWindowTextW(L"��������ť!");
			}break;
		case BROWSER:
			{
				m_edt0.SetCueBanner(L"��������ť!");
			}break;
		case VS:
			{
				m_edt0.SetCueBanner(L"˫����ѡ��Ҫ�����Ŀ¼!");
			}break;		
	}
}


void CClearRubbishDlg::OnBnClicked0()
{
	switch (m_iIndexSelected)
	{
		case POSTFIX:
			{
				ClearFileWithPostfixSpecified(m_edt0.m_wcsFolderSelected);
			}break;
		case SYSTEM:
			{
				SHQUERYRBINFO rb{};
				rb.cbSize = sizeof(SHQUERYRBINFO);

				SHQueryRecycleBinW(L"C:", &rb);
				HRESULT hRes = SHEmptyRecycleBinW(NULL, NULL, SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI | SHERB_NOSOUND);
			}break;
		case BROWSER:
			{
				CStringW wcsDir[2] = { L"C:\\Users\\_Spobt_00\\AppData\\Local\\Mozilla\\Firefox\\Profiles\\a9m6zchs.default\\cache2",
				L"C:\\Users\\_Spobt_00\\AppData\\Local\\Mozilla\\Firefox\\Profiles\\a9m6zchs.default\\OfflineCache" };
				ClearAllInDir(wcsDir[0]);
				ClearAllInDir(wcsDir[1]);
			}break;
		case VS:
			{
				ClearVS(m_edt0.m_wcsFolderSelected);
			}break;
	}
}