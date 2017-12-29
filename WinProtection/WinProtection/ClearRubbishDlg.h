#pragma once
#include "afxwin.h"
#include <vector>
#include "afxcmn.h"
#include "MyEdit.h"


#define POSTFIX 0
#define SYSTEM 1
#define BROWSER 2
#define VS 3
using std::vector;
// CClearRubbishDlg 对话框

class CClearRubbishDlg : public CDialogEx
{
	// Attributions ***************************************************************************************
public:



	// Methods ********************************************************************************************
public:
	int ClearAllInDir(CStringW& wcsDir);
	int ClearFileWithPostfixSpecified(CStringW& wcsDir);
	int GetPostfix(CStringW& wcsName, CStringW& wcsPostfix);
	int ClearVS(CStringW& wcsDir);

















	DECLARE_DYNAMIC(CClearRubbishDlg)

public:
	CClearRubbishDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CClearRubbishDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDDLG_ClEARUBBISH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	int m_iIndexSelected;
	CComboBox m_cb0;
	CMyEdit m_edt0;
	CListCtrl m_lc0;
	afx_msg void OnSelchangeIdcb0();
	CString m_wcsEdt0;
	afx_msg void OnBnClicked0();
};
