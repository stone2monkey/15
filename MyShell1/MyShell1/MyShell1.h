
// MyShell1.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMyShell1App:
// See MyShell1.cpp for the implementation of this class
//

class CMyShell1App : public CWinApp
{
public:
	CMyShell1App();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMyShell1App theApp;