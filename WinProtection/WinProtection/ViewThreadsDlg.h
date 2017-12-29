#pragma once
#include "afxcmn.h"
#include <vector>

using std::vector;
class CMyThreadData;


// CViewThreadsDlg �Ի���

class CViewThreadsDlg : public CDialogEx
{
	// Attributions ***************************************************************************************
public:
	HANDLE m_hProc{};			// �����
	int m_iIndexOfProcessinVct{};		// ������vector�е�����
	vector<CMyThreadData*>& m_vctMyThreadData;	// ģ��vector



	// Methods ********************************************************************************************
public:
	int GetCurThreads(vector<CMyThreadData*>& vctMyThreadData);










	DECLARE_DYNAMIC(CViewThreadsDlg)

public:
	CViewThreadsDlg(int iIndexOfProcessinVct, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CViewThreadsDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDDLG_VIEWTHREADS };
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
	afx_msg void OnFreshenthreads();
};
