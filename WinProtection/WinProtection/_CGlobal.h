#pragma once
#include <vector>
#include "MyModuleData.h"
#include "MyProcessData.h"
#include "MyThreadData.h"
#include "MyWindowData.h"

using std::vector;

// Attributions ***************************************************************************************
extern vector<CMyProcessData*> g_vctMyProcessData;	// ���н��̵�����
extern vector<CMyWindowData*> g_vctMyWindowData;	// ���д�������
extern vector<char*> g_vctWhiteList, g_vctBlackList;	// ������,������MD5

// g_pAllWnd[0]		���̴��ڶ���ָ��
// g_pAllWnd[1]		�̴߳��ڶ���ָ��
// g_pAllWnd[2]		ģ�鴰�ڶ���ָ��
// g_pAllWnd[3]		���д��ڶ���ָ��
// g_pAllWnd[4]		PE���ڶ���ָ��
// g_pAllWnd[5]		�����������ڶ���ָ��
// g_pAllWnd[6]		���񴰿ڶ���ָ��
// g_pAllWnd[7]		СԲ�򴰿ڶ���ָ��
// g_pAllWnd[8]		MD5���ڶ���ָ��
extern CWnd* g_pAllWnd[10];							// ���д��ڵ�[����ָ��]����
extern CMenu* g_pMainMenu;						// ȫ�ֲ˵���Դ










// Methods ********************************************************************************************
void PopupWnd(LPCTSTR lpText);

































class _CGlobal
{
public:
	_CGlobal();
	~_CGlobal();
};

