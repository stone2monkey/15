#include "stdafx.h"
#include "Interface.h"
#include "_CGlobal.h"
#include "MD5.h"


CInterface::CInterface()
{
}


CInterface::~CInterface()
{
}

int CInterface::InitListCtrl(CWnd * pWnd, CListCtrl * pListCtrl, int iColumnCount, WCHAR ** const wcsArrHeaderName, BOOL bInit)
{
	// �õ��п�
	CRect oRect{};
	pWnd->GetClientRect(oRect);
	int iWidth = (oRect.Width()-4) / iColumnCount;

	// ��ʼ�� or �Ը�Ӧ�п�	�б�ؼ�
	if (bInit)	// ��ʼ��
	{
		pListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
		pListCtrl->MoveWindow(0, 0, oRect.Width(), oRect.Height());
		for (int i = 0; i < iColumnCount; i++) { pListCtrl->InsertColumn(i, wcsArrHeaderName[i], LVCFMT_LEFT, iWidth); }
	}
	else // �Ը�Ӧ�п�
	{
		pListCtrl->MoveWindow(0, 0, oRect.Width(), oRect.Height());
		for (int i = 0; i < iColumnCount; i++) { pListCtrl->SetColumnWidth(i, iWidth); }
	}

	return 0;
}

//****************************************************
// Name: CInterface::GetNameFormFullName
// Func: ��һ��ȫ·�����еõ�����
// RetV: int
// Args: WCHAR * const wcsName
// Args: WCHAR * const wcsFullName
//****************************************************
int CInterface::GetNameFormFullName(WCHAR * const wcsName, WCHAR * const wcsFullName)
{
	WCHAR* p = wcsFullName;
	while (*p) { p++; }
	while (*p != '\\') { p--; }
	p++;

	if (0 != wcscpy_s(wcsName, MAX_PATH, p)) { ::MessageBoxW(0, L"wcscpy_s 0", L"Exception", MB_ICONERROR); }

	return 0;
}

//****************************************************
// Name: CInterface::CleanListCtrl
// Func: ���һ������ģʽ�б��������
// RetV: int
// Args: CListCtrl * pListCtrl
//****************************************************
int CInterface::CleanListCtrl(CListCtrl * pListCtrl)
{
	// �����
	pListCtrl->DeleteAllItems();

	// �����
	int iColumnCount = pListCtrl->GetHeaderCtrl()->GetItemCount();
	for (int i = 0; i < iColumnCount; i++) { pListCtrl->DeleteColumn(0); }

	return 0;
}

CStringW CInterface::SelectFolder()
{
	WCHAR szFolderPath[MAX_PATH]{};
	CStringW strFolderPath{};

	BROWSEINFO      sInfo{};
	sInfo.lpszTitle = _T("��ѡ��һ���ļ��У�");


	// ��ʾ�ļ���ѡ��Ի���  
	LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);
	if (lpidlBrowse != NULL)
	{
		// ȡ���ļ�����  
		if (::SHGetPathFromIDList(lpidlBrowse, szFolderPath))
		{
			strFolderPath = szFolderPath;
		}
	}
	if (lpidlBrowse != NULL)
	{
		::CoTaskMemFree(lpidlBrowse);
	}

	return strFolderPath;
}

long long CInterface::TimeNum(FILETIME ft1, FILETIME ft2)
{
	long long  t1 = (long long)ft1.dwHighDateTime << 32 | ft1.dwLowDateTime;
	long long  t2 = (long long)ft2.dwHighDateTime << 32 | ft2.dwLowDateTime;

	return t1 - t2;
}


void CInterface::CheckPathMD5(CStringW & wcsDir, HWND hWnd)
{
	// 1.�ҿ����� ************************************************************
	CFileFind oFF{};
	BOOL bRes = oFF.FindFile(wcsDir + L"\\*.*");
	if (!bRes) { return; }

	// 2.���� ************************************************************
	CStringW wcsNameFound{};
	while (bRes)
	{

		bRes = oFF.FindNextFileW();
		// �õ��ҵ���
		wcsNameFound = oFF.GetFileName();

		// �����ϵ�
		if (oFF.IsDots()) { continue; }

		// ������ļ���,traverse
		if (oFF.IsDirectory())
		{
			CheckPathMD5(oFF.GetFilePath(), hWnd);
		}

		// ������ļ�
		// 2.�õ�MD5 ************************************************************
		char* strMD5 = new char[33]{};
		CMD5 oMD5{};
		CStringW wcsFullName = oFF.GetFilePath();
		char strFullName[MAX_PATH]{};
		WideCharToMultiByte(CP_ACP, 0, wcsFullName, -1, strFullName, MAX_PATH, 0, 0);
		char* p = oMD5.md5FileValue(strFullName);
		memcpy_s(strMD5, 33, p, 33);

		// 3.MD5�Ƿ���� ************************************************************
		DWORD iLoop = g_vctBlackList.size();
		for (DWORD i = 0; i < iLoop; i++)
		{
			if (0 == StrCmpA(strMD5, g_vctBlackList.at(i)))
			{
				CStringW wcsTemp{};
				wcsTemp.Format(L"%s%s", oFF.GetFilePath(), L"\r\n�ں�������,�Ƿ�ɾ��?");
				if (IDYES == ::MessageBoxW(hWnd, wcsTemp, L"MD5", MB_YESNO))
				{
					CFile::Remove(oFF.GetFilePath());
				}
				break;
			}
		}

		delete[] strMD5;
	}
	oFF.Close();
}

//****************************************************
// Name: CInterface::AddMD5ToFile
// Func: ���ָ���ļ���MD5����Ӧ������
// RetV: void
// Args: CStringW & wcsFullFileName	ָ���ļ���ȫ��
// Args: CStringW & wcsFullListName	ָ��������ȫ��
// Args: HWND hWnd ERRORʱ�����ĸ�
// Args: vector<char * > & vctList  ��ʲô����
//****************************************************
void CInterface::AddMD5ToFile(CStringW & wcsFullFileName, CStringW & wcsFullListName,HWND hWnd, vector<char*>& vctList)
{
	if (0 == wcsFullFileName.Compare(L"")) { return; }

	// 1.�õ�ѡ�����ļ� ************************************************************
	char strFullFileName[MAX_PATH]{};
	WideCharToMultiByte(CP_ACP, 0, wcsFullFileName, -1, strFullFileName, MAX_PATH, 0, 0);
	char strFullListName[MAX_PATH]{};
	WideCharToMultiByte(CP_ACP, 0, wcsFullListName, -1, strFullListName, MAX_PATH, 0, 0);

	// 2.�õ�MD5 ************************************************************
	char* strMD5 = new char[33]{};
	CMD5 oMD5{};
	char* p = oMD5.md5FileValue(strFullFileName);
	memcpy_s(strMD5, 33, p, 33);

	if (0 == StrCmpA(strMD5, ""))
	{
		::MessageBoxW(hWnd, L"��ȡMD5ʧ��!", L"MD5", 0);
		delete[] strMD5;
		return;
	}

	// 3.MD5�Ƿ���� ************************************************************
	DWORD iLoop = vctList.size();
	for (DWORD i = 0; i < iLoop; i++)
	{
		if (0 == StrCmpA(strMD5, vctList.at(i)))
		{
			::MessageBoxW(hWnd, L"��MD5�Ѿ���������,�����ټ�!", L"MD5", 0);
			delete[] strMD5;
			return;
		}
	}

	// 4.����MD5���ļ�,vector ************************************************************
	FILE* pFile{};
	fopen_s(&pFile, strFullListName, "ab");
	fwrite(strMD5, 33, 1, pFile);
	fclose(pFile);

	vctList.push_back(strMD5);
}

//****************************************************
// Name: CInterface::IsMD5SpecifiedInMD5List
// Func: ָ��MD5�Ƿ���������
// RetV: BOOL
// Args: char * & strMD5
// Args: vector<char * > & vctList
//****************************************************
BOOL CInterface::IsMD5SpecifiedInMD5List(char *& strMD5, vector<char*>& vctList)
{
	DWORD iLoop = vctList.size();
	for (DWORD i = iLoop; i >0; i--)
	{
		if (0 == StrCmpA(strMD5, vctList.at(i - 1)))
		{
			return 1;
		}
	}

	return 0;
}
