
// WinProtectionDlg.h : 头文件
//

#pragma once
#include "CircleDlg.h"


// CWinProtectionDlg 对话框
class CWinProtectionDlg : public CDialogEx
{
	// Attributions ***************************************************************************************
public:
	CCircleDlg* m_pCircleDlg{};		// 小圆球
	BOOL m_bIsShow{};







	// Methods ********************************************************************************************
// 构造
public:
	CWinProtectionDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WINPROTECTION_DIALOG };
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
	afx_msg void OnBnClickedOk();
	afx_msg void On0Viewprocesses();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnViewallwindows();
	afx_msg void OnClearrubbish();
	afx_msg void OnServicemanager();
	afx_msg void On32827();
	afx_msg void OnViewmd5();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
