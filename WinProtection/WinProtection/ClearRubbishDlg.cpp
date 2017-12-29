// ClearRubbishDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WinProtection.h"
#include "ClearRubbishDlg.h"
#include "afxdialogex.h"
#include "_CGlobal.h"
#include "MyEdit.h"


// CClearRubbishDlg 对话框

IMPLEMENT_DYNAMIC(CClearRubbishDlg, CDialogEx)

//****************************************************
// Name: CClearRubbishDlg::ClearAllInDir
// Func: 删除目录下所有内容(不包括本目录)
// RetV: int
// Args: CStringW & wcsDir
//****************************************************
int CClearRubbishDlg::ClearAllInDir(CStringW & wcsDir)
{
	// 1.找开查找 ************************************************************
	CFileFind oFF{};
	BOOL bRes = oFF.FindFile(wcsDir + L"\\*.*");
	if (!bRes) { return 1; }

	// 2.遍历 ************************************************************
	CStringW wcsNameFound{};
	while (bRes)
	{
		bRes = oFF.FindNextFileW();
		// 得到找到的
		wcsNameFound = oFF.GetFileName();

		// 不符合的
		if (oFF.IsDots()) { continue; }

		// 改变属性,例能删除
		SetFileAttributesW(oFF.GetFilePath(), FILE_ATTRIBUTE_NORMAL);

		// 删除
		if (oFF.IsDirectory())
		{
			ClearAllInDir(oFF.GetFilePath());
			RemoveDirectoryW(oFF.GetFilePath());
		}
		else
		{
			DeleteFileW(oFF.GetFilePath());
		}

		// 打印
		// 索引为0为倒序,索引最大为正序
		m_lc0.InsertItem(0x7fffffff, oFF.GetFilePath());
	}
	oFF.Close();

	return 0;
}

int CClearRubbishDlg::ClearFileWithPostfixSpecified(CStringW & wcsDir)
{
	// 1.找开查找 ************************************************************
	CFileFind oFF{};
	BOOL bRes = oFF.FindFile(wcsDir + L"\\*.*");
	if (!bRes) { return 1; }

	// 2.遍历 ************************************************************
	CStringW wcsNameFound{};
	while (bRes)
	{

		bRes = oFF.FindNextFileW();
		// 得到找到的
		wcsNameFound = oFF.GetFileName();

		// 不符合的
		if (oFF.IsDots()) { continue; }

		// 改变属性,例能删除
		SetFileAttributesW(oFF.GetFilePath(), FILE_ATTRIBUTE_NORMAL);

		// 如果是文件夹,traverse
		if (oFF.IsDirectory())
		{
			ClearFileWithPostfixSpecified(oFF.GetFilePath());
		}
		
		// 如果是文件
		//得到当前后缀
		CStringW wcsPostfix{};
		if (GetPostfix(oFF.GetFilePath(), wcsPostfix)) { continue; }
		wcsPostfix.Insert(wcslen(wcsPostfix), L'?');
		//如果是指定后缀
		UpdateData();
		if (-1 != m_edt0.m_wcsPostfix.Find(wcsPostfix))	// 后缀对上
		{
			DeleteFileW(oFF.GetFilePath());

			// 打印
			// 索引为0为倒序,索引最大为正序
			m_lc0.InsertItem(0x7fffffff, oFF.GetFilePath());
		}	
	}
	oFF.Close();

	return 0;
}

//****************************************************
// Name: CClearRubbishDlg::GetPostfix
// Func: 得到一个字符串的后缀
// RetV: int	不为0时,这个文件没有后缀
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
	BOOL bProjectName{};	// 找到工程
	BOOL bFolderName{};		// 找到与工程同名文件夹
	CStringW wcsProjectName{};	// 文件名
	CStringW wcsFolderName{};//文件夹 

	// 1.找开查找 ************************************************************
	CFileFind oFF{};
	BOOL bRes = oFF.FindFile(wcsDir + L"\\*.*");
	if (!bRes) { return 1; }

	// 2.遍历,找到工程,得到文件名,没有后缀的 ************************************************************
	CStringW wcsNameFound{};
	while (bRes)
	{
		bRes = oFF.FindNextFileW();
		// 得到找到的
		wcsNameFound = oFF.GetFileName();

		// 不符合的
		if (oFF.IsDots()) { continue; }
		if (oFF.IsDirectory()) { continue; }

		// 如果是文件
		//得到当前后缀
		CStringW wcsPostfix{};
		if (GetPostfix(oFF.GetFilePath(), wcsPostfix)) { continue; }
		//如果是指定后缀
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

	// 3.找文件夹 ************************************************************
	if (!bProjectName) { return 2; }

	CFileFind oFF1{};
	bRes = oFF1.FindFile(wcsDir + L"\\*.*");
	if (!bRes) { return 1; }

	while (bRes)
	{
		bRes = oFF1.FindNextFileW();
		// 得到找到的
		wcsNameFound = oFF1.GetFileName();

		// 不符合的
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

	// 4.没有找到工程与同名文件夹,退出 ************************************************************
	if (!(bProjectName && bFolderName)) { return 3; }

	// 5.删除除工程名与同名文件夹外,其它的文件 ************************************************************
	CFileFind oFF2{};
	bRes = oFF2.FindFile(wcsDir + L"\\*.*");
	if (!bRes) { return 1; }
	while (bRes)
	{

		bRes = oFF2.FindNextFileW();
		// 得到找到的
		wcsNameFound = oFF2.GetFileName();

		// 不符合的
		if (oFF2.IsDots()) { continue; }

		// 改变属性,使能删除
		SetFileAttributesW(oFF2.GetFilePath(), FILE_ATTRIBUTE_NORMAL);

		// 如果是文件夹,traverse
		if (oFF2.IsDirectory())
		{
			if (0 == wcsFolderName.Compare(wcsNameFound)) { continue; }

			ClearAllInDir(oFF2.GetFilePath());
			RemoveDirectoryW(oFF2.GetFilePath());
			m_lc0.InsertItem(0x7fffffff, oFF2.GetFilePath());
		}
		else  // 如果是文件
		{
			if (0 == wcsProjectName.Compare(wcsNameFound)) { continue; }

			DeleteFileW(oFF2.GetFilePath());
			m_lc0.InsertItem(0x7fffffff, oFF2.GetFilePath());
		}
	}
	oFF2.Close();

	// 6.删除同名工程目录中的Debug Release x64 ************************************************************
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


// CClearRubbishDlg 消息处理程序


void CClearRubbishDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	// CDialogEx::OnOK();
}


BOOL CClearRubbishDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	{	// 初始化下拉列表
		WCHAR* wcsTemp[4] = { L"垃圾清理", L"系统垃圾", L"浏览器垃圾", L"VS垃圾" };
		for (int i = 0; i < 4; i++) { m_cb0.InsertString(-1, wcsTemp[i]); }
		m_cb0.SetCurSel(0);
		m_iIndexSelected = 0;
		OnSelchangeIdcb0();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CClearRubbishDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

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
				m_edt0.SetCueBanner(L"单击以输入要清理的后缀(多个后缀时, 每个后缀后紧跟\"?\")!双击以选择要清理的目录!");
			}break;
		case SYSTEM:
			{
				m_edt0.SetWindowTextW(L"单击清理按钮!");
			}break;
		case BROWSER:
			{
				m_edt0.SetCueBanner(L"单击清理按钮!");
			}break;
		case VS:
			{
				m_edt0.SetCueBanner(L"双击以选择要清理的目录!");
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