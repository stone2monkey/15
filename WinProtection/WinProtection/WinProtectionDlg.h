
// WinProtectionDlg.h : ͷ�ļ�
//

#pragma once
#include "CircleDlg.h"


// CWinProtectionDlg �Ի���
class CWinProtectionDlg : public CDialogEx
{
	// Attributions ***************************************************************************************
public:
	CCircleDlg* m_pCircleDlg{};		// СԲ��
	BOOL m_bIsShow{};







	// Methods ********************************************************************************************
// ����
public:
	CWinProtectionDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WINPROTECTION_DIALOG };
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
	afx_msg void On0Viewprocesses();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnViewallwindows();
	afx_msg void OnClearrubbish();
	afx_msg void OnServicemanager();
	afx_msg void On32827();
	afx_msg void OnViewmd5();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
