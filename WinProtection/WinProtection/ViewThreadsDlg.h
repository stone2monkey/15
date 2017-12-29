#pragma once
#include "afxcmn.h"
#include <vector>

using std::vector;
class CMyThreadData;


// CViewThreadsDlg 对话框

class CViewThreadsDlg : public CDialogEx
{
	// Attributions ***************************************************************************************
public:
	HANDLE m_hProc{};			// 进句柄
	int m_iIndexOfProcessinVct{};		// 进程在vector中的索引
	vector<CMyThreadData*>& m_vctMyThreadData;	// 模块vector



	// Methods ********************************************************************************************
public:
	int GetCurThreads(vector<CMyThreadData*>& vctMyThreadData);










	DECLARE_DYNAMIC(CViewThreadsDlg)

public:
	CViewThreadsDlg(int iIndexOfProcessinVct, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CViewThreadsDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDDLG_VIEWTHREADS };
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
	afx_msg void OnFreshenthreads();
};
