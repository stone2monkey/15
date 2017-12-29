
// ClntDlg.h : ͷ�ļ�
//

#pragma once
#include "MySocket.h"
#include "ConnServ.h"
#include "_Global.h"
#include "afxcmn.h"


// CClntDlg �Ի���
class CClntDlg : public CDialogEx
{
// ����
public:
	CClntDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLNT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_wstrIP;
	UINT m_uPort;
	afx_msg void OnBnClickedConnserv();

	CMySocket m_oMySocket;
	CString m_wstrShowMsg;
//	UINT m_uQQ;
//	UINT m_uPW;
	afx_msg void OnBnClickedLogup();
	static void (WINAPI LoopRecv)(SParamToThread* pParamToThread);
	afx_msg void OnBnClickedLogin();
//	CString m_uQQ;
	UINT m_uPW;
	UINT m_uQQ;
	UINT m_uAddSeek;
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedSeek();
	virtual void OnOK();
	void UpdateOnlineQQListCtrl();
	CListCtrl m_lcOnlineQQ;
	afx_msg void OnBnClickedSendmultichat();
	CString m_wstrMsgSent;
	UINT m_uQQOfRecvMsg;
	afx_msg void OnBnClickedSendsinglechat();
};
