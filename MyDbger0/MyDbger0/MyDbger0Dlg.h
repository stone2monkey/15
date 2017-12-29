
// MyDbger0Dlg.h : 头文件
//

#pragma once
#include "DisasmListCtrl.h"
#include "MemListCtrl.h"
#include "StackListCtrl.h"
#include "CmdEdit.h"
#include "afxcmn.h"
#include "ModuleListCtrl.h"



// CMyDbger0Dlg 对话框
class CMyDbger0Dlg : public CDialogEx
{
// Attributions ***************************************************************************************
public:
	CCmdEdit m_edtCmd;			// 命令行控件类
	CString m_wcsLog;			// 日志控件关联字符串
	CString m_wcsReg;			// 寄存器控件关联字符串
	DWORD m_dwEip;				// eip
	CDisasmListCtrl m_lcDisasm;	// 反汇编控件类
	CMemListCtrl m_lcMem;		// 内存控件类
	CStackListCtrl m_lcStack;	// 栈控件类
	CModuleListCtrl m_lcModuleInof;	//模块

	CFont m_oFont{};


// Methods ********************************************************************************************
public:
	void ResetReg(DWORD dwTID);	//设置寄存器控件关联字符串,并显示
	virtual BOOL PreTranslateMessage(MSG* pMsg);




















// 构造
public:
	CMyDbger0Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDDLG_MY_DBGER };
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
};