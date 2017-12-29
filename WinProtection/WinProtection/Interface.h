// 这是一个对外提供接口的类
#pragma once
#include <vector>

using std::vector;


class CInterface
{
// Methods ********************************************************************************************
public:
	CInterface();
	~CInterface();

	static int InitListCtrl(CWnd* pWnd, CListCtrl* pListCtrl, int iColumnCount, WCHAR** const wcsArrHeaderName, BOOL bInit);
	static int GetNameFormFullName(WCHAR* const wcsName, WCHAR* const wcsFullName);
	static int CleanListCtrl(CListCtrl* pListCtrl);
	static CStringW SelectFolder();
	static long long TimeNum(FILETIME ft1, FILETIME ft2);
	static void CheckPathMD5(CStringW & wcsDir, HWND hWnd);
	static void AddMD5ToFile(CStringW & wcsFullFileName, CStringW & wcsFullListName, HWND hWnd, vector<char*>& vctList);
	static BOOL IsMD5SpecifiedInMD5List(char*& strMD5, vector<char*>& vctList);
};