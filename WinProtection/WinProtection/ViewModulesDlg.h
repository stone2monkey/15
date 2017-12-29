#pragma once
#include "afxcmn.h"
#include <vector>

using std::vector;
class CMyModuleData;


// CViewModulesDlg 对话框

class CViewModulesDlg : public CDialogEx
{
	// Attributions ***************************************************************************************
public:
	HANDLE m_hProc{};			// 进句柄
	int m_iIndexOfProcessinVct{};		// 进程在vector中的索引
	vector<CMyModuleData*>& m_vctMyModuleData;	// 模块vector




	// Methods ********************************************************************************************
public:
	int GetCurModules(vector<CMyModuleData*>& vctMyModuleData);












	DECLARE_DYNAMIC(CViewModulesDlg)

public:
	CViewModulesDlg(int iIndexOfProcessinVct, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CViewModulesDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDDLG_VIEWMODULES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CListCtrl m_lc0;
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFreshenmodules();
	afx_msg void OnPeparse();
	afx_msg void OnRclickIdlc0(NMHDR *pNMHDR, LRESULT *pResult);
};
