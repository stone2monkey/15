#pragma once
#include "afxcmn.h"
#include <vector>

using std::vector;
class CMyWindowData;


// CViewAllWindowsDlg 对话框

class CViewAllWindowsDlg : public CDialogEx
{
	// Methods ********************************************************************************************
public:
	int GetAllWindows(vector<CMyWindowData*>& vctMyWindowData);














	DECLARE_DYNAMIC(CViewAllWindowsDlg)

public:
	CViewAllWindowsDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CViewAllWindowsDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDDLG_VIEWALLWINDOWS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CListCtrl m_lc0;
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
