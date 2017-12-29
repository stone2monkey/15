#pragma once
#include <vector>
#include "MyModuleData.h"
#include "MyProcessData.h"
#include "MyThreadData.h"
#include "MyWindowData.h"

using std::vector;

// Attributions ***************************************************************************************
extern vector<CMyProcessData*> g_vctMyProcessData;	// 所有进程的数据
extern vector<CMyWindowData*> g_vctMyWindowData;	// 所有窗口数据
extern vector<char*> g_vctWhiteList, g_vctBlackList;	// 白名单,黑名单MD5

// g_pAllWnd[0]		进程窗口对象指针
// g_pAllWnd[1]		线程窗口对象指针
// g_pAllWnd[2]		模块窗口对象指针
// g_pAllWnd[3]		所有窗口对象指针
// g_pAllWnd[4]		PE窗口对象指针
// g_pAllWnd[5]		清理垃圾窗口对象指针
// g_pAllWnd[6]		服务窗口对象指针
// g_pAllWnd[7]		小圆球窗口对象指针
// g_pAllWnd[8]		MD5窗口对象指针
extern CWnd* g_pAllWnd[10];							// 所有窗口的[对象指针]数组
extern CMenu* g_pMainMenu;						// 全局菜单资源










// Methods ********************************************************************************************
void PopupWnd(LPCTSTR lpText);

































class _CGlobal
{
public:
	_CGlobal();
	~_CGlobal();
};

