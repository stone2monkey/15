
// ServDlg.h : ͷ�ļ�
//

#pragma once
#include "MySocket.h"
#include "_Global.h"
#include "afxcmn.h"


// CServDlg �Ի���
class CServDlg : public CDialogEx
{
// ����
public:
	CServDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERV_DIALOG };
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
