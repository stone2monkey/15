#pragma once
#include "afxcmn.h"
#include <vector>

using std::vector;
class CMyProcessData;




// CViewProcessesDlg �Ի���

class CViewProcessesDlg : public CDialogEx
{
	// Methods ********************************************************************************************
public:
	int GetCurProcesses(vector<CMyProcessData*>& vctMyProcessData);
	BOOL InitWhiteList();





	DECLARE_DYNAMIC(CViewProcessesDlg)

public:
	CViewProcessesDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CViewProcessesDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDDLG_VIEWPROCESSES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	CListCtrl m_lc0;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRclickIdlc0(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnViewthreads();
	afx_msg void OnViewmodules();
	afx_msg void OnFreshenprocesses();
	afx_msg void OnAddtowhitelist();
	afx_msg void OnCheckwhitelist();
};
