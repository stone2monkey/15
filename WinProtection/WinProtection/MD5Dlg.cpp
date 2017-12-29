// MD5Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WinProtection.h"
#include "MD5Dlg.h"
#include "afxdialogex.h"
#include "_CGlobal.h"
#include "MD5.h"
#include "Interface.h"


// CMD5Dlg �Ի���

IMPLEMENT_DYNAMIC(CMD5Dlg, CDialogEx)

//****************************************************
// Name: CMD5Dlg::InitBlackList
// Func: ��ʼ��MD5��g_vctBlackList
// RetV: BOOL
//****************************************************
BOOL CMD5Dlg::InitBlackList()
{
	// 1.���ļ� ************************************************************
	WCHAR* wcsFileName = L"D:\\_Spobt_00\\Documents\\Visual Studio 2017\\Projects\\Rep_WinProtection\\WinProtection\\WinProtection\\res\\BlackList.txt";
	CFile oFile{};
	if (!oFile.Open(wcsFileName, CFile::modeNoTruncate | CFile::modeRead)) {::PopupWnd(L"CFile::Open!");}

	// 2.�õ�MD5���� ************************************************************
	ULONGLONG dwNumOfMD5 = oFile.GetLength() / 33;
	if (0 == dwNumOfMD5) { return 1; }

	// 3.���vector ************************************************************
	DWORD iLoop = g_vctBlackList.size();
	for (DWORD i = iLoop; i > 0; i++)
	{
		delete[] g_vctBlackList.at(i - 1);
		g_vctBlackList.pop_back();
	}

	// 3.ѹ��vector ************************************************************
	for (ULONGLONG i = 0; i < dwNumOfMD5; i++)
	{
		char* strMD5 = new char[33]{};
		oFile.Read(strMD5, 33);
		g_vctBlackList.push_back(strMD5);
	}

	// 4. ************************************************************
	oFile.Close();

	return 1;
}



CMD5Dlg::CMD5Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDDLG_MD5, pParent)
{

}

CMD5Dlg::~CMD5Dlg()
{
}

void CMD5Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDEDT_0, m_myedt0);
}


BEGIN_MESSAGE_MAP(CMD5Dlg, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDBTN_ADDMD5, &CMD5Dlg::OnBnClickedAddmd5)
	ON_BN_CLICKED(IDBTN_0, &CMD5Dlg::OnBnClicked0)
END_MESSAGE_MAP()


// CMD5Dlg ��Ϣ�������




BOOL CMD5Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	{
		InitBlackList();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CMD5Dlg::OnClose()
{
	DestroyWindow();
	delete this;
	g_pAllWnd[8] = nullptr;
}


void CMD5Dlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	// CDialogEx::OnOK();
}


//****************************************************
// Name: CMD5Dlg::OnBnClickedAddmd5
// Func: ���MD5��������
// RetV: void
//****************************************************
void CMD5Dlg::OnBnClickedAddmd5()
{
	CFileDialog oFD(1);
	oFD.DoModal();
	CStringW wcsFullFileName = oFD.GetPathName();
	CStringW wcsFullListName = L"D:\\_Spobt_00\\Documents\\Visual Studio 2017\\Projects\\Rep_WinProtection\\WinProtection\\WinProtection\\res\\BlackList.txt";
	CInterface::AddMD5ToFile(wcsFullFileName, wcsFullListName, this->m_hWnd, g_vctBlackList);
}


//****************************************************
// Name: CMD5Dlg::OnBnClicked0
// Func: ���·���µ��ļ���MD5�Ƿ��ں�������,����ָ���Ĳ���
// RetV: void
//****************************************************
void CMD5Dlg::OnBnClicked0()
{
	CInterface::CheckPathMD5(m_myedt0.m_wcsFolderSelected, this->m_hWnd);
}
