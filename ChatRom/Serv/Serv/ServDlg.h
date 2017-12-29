
// ServDlg.h : 头文件
//

#pragma once
#include "MySocket.h"
#include "_Global.h"
#include "afxcmn.h"


// CServDlg 对话框
class CServDlg : public CDialogEx
{
// 构造
public:
	CServDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERV_DIALOG };
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
	CMySocket m_oMySocket;
	afx_msg void OnBnClickedStartup();

	static void (WINAPI LoopAccept)(SParamToThread* pParamToThread);
	static void (WINAPI LoopRecv)(SParamToThread* pParamToThread);
	CString m_wstrShowMsg;
	CListCtrl m_lcOnlineQQ;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void UpdateOnlineQQListCtrl();
	virtual void OnOK();
	UINT m_uTelecontrolQQ;
	CString m_wstrTelecontrol;
	afx_msg void OnBnClickedSnedtelecontrol();
};
