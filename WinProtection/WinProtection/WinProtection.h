
// WinProtection.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CWinProtectionApp: 
// �йش����ʵ�֣������ WinProtection.cpp
//

class CWinProtectionApp : public CWinApp
{
public:
	CWinProtectionApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CWinProtectionApp theApp;
