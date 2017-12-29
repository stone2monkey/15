#pragma once
#include "MyEdit.h"
#include "afxwin.h"


// CMD5Dlg 对话框

class CMD5Dlg : public CDialogEx
{
	// Attributions ***************************************************************************************







	// Methods ********************************************************************************************
public:
	BOOL InitBlackList();
	






















	DECLARE_DYNAMIC(CMD5Dlg)

public:
	CMD5Dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMD5Dlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDDLG_MD5 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	virtual void OnOK();
	afx_msg void OnBnClickedAddmd5();
	CMyEdit m_myedt0;
	afx_msg void OnBnClicked0();
};
