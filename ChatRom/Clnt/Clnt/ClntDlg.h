
// ClntDlg.h : 头文件
//

#pragma once
#include "MySocket.h"
#include "ConnServ.h"
#include "_Global.h"
#include "afxcmn.h"


// CClntDlg 对话框
class CClntDlg : public CDialogEx
{
// 构造
public:
	CClntDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLNT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
