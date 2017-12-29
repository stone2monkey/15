#pragma once
#include "afxcmn.h"
#include <vector>

using std::vector;
class CMyModuleData;


// CViewModulesDlg �Ի���

class CViewModulesDlg : public CDialogEx
{
	// Attributions ***************************************************************************************
public:
	HANDLE m_hProc{};			// �����
	int m_iIndexOfProcessinVct{};		// ������vector�е�����
	vector<CMyModuleData*>& m_vctMyModuleData;	// ģ��vector




	// Methods ********************************************************************************************
public:
	int GetCurModules(vector<CMyModuleData*>& vctMyModuleData);












	DECLARE_DYNAMIC(CViewModulesDlg)

public:
	CViewModulesDlg(int iIndexOfProcessinVct, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CViewModulesDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDDLG_VIEWMODULES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
