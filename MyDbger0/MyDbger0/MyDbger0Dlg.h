
// MyDbger0Dlg.h : ͷ�ļ�
//

#pragma once
#include "DisasmListCtrl.h"
#include "MemListCtrl.h"
#include "StackListCtrl.h"
#include "CmdEdit.h"
#include "afxcmn.h"
#include "ModuleListCtrl.h"



// CMyDbger0Dlg �Ի���
class CMyDbger0Dlg : public CDialogEx
{
// Attributions ***************************************************************************************
public:
	CCmdEdit m_edtCmd;			// �����пؼ���
	CString m_wcsLog;			// ��־�ؼ������ַ���
	CString m_wcsReg;			// �Ĵ����ؼ������ַ���
	DWORD m_dwEip;				// eip
	CDisasmListCtrl m_lcDisasm;	// �����ؼ���
	CMemListCtrl m_lcMem;		// �ڴ�ؼ���
	CStackListCtrl m_lcStack;	// ջ�ؼ���
	CModuleListCtrl m_lcModuleInof;	//ģ��

	CFont m_oFont{};


// Methods ********************************************************************************************
public:
	void ResetReg(DWORD dwTID);	//���üĴ����ؼ������ַ���,����ʾ
	virtual BOOL PreTranslateMessage(MSG* pMsg);




















// ����
public:
	CMyDbger0Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDDLG_MY_DBGER };
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
	afx_msg void OnBnClickedOk();
};