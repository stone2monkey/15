
// MyDbger0.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMyDbger0App: 
// �йش����ʵ�֣������ MyDbger0.cpp
//

class CMyDbger0App : public CWinApp
{
public:
	CMyDbger0App();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMyDbger0App theApp;
