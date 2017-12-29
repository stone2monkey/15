#include "stdafx.h"
#include "_CGlobal.h"


vector<CMyProcessData*> g_vctMyProcessData{};
vector<CMyWindowData*> g_vctMyWindowData;
extern CWnd* g_pAllWnd[10]{};
CMenu* g_pMainMenu{};
vector<char*> g_vctWhiteList{}, g_vctBlackList{};









// Methods ********************************************************************************************
void PopupWnd(LPCTSTR lpText)
{
	::MessageBox(0, lpText, _T("Exception"), MB_ICONERROR);
}













_CGlobal::_CGlobal()
{
}


_CGlobal::~_CGlobal()
{
}
