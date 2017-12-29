#pragma once
#include "afxcmn.h"
#include <vector>
#include <winsvc.h>

using std::vector;
class CMyServiceData;

// CServiceManagerDlg �Ի���

class CServiceManagerDlg : public CDialogEx
{
	// Attributions ***************************************************************************************
public:
	SC_HANDLE m_hSCM1;
	vector<CMyServiceData*> m_vctMyServiceData{};





	// Methods ********************************************************************************************
	int GetAllServiceInfo();
	// ���·���״̬
	void UpadateServiceStatus();















	DECLARE_DYNAMIC(CServiceManagerDlg)

public:
	CServiceManagerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CServiceManagerDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDDLG_SERVICEMANAGER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CListCtrl m_lc0;
	afx_msg void OnStop();
	afx_msg void OnStartup();
	afx_msg void OnRclickIdlc0(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
