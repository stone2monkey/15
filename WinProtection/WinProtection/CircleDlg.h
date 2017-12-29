#pragma once


// CCircleDlg �Ի���

class CCircleDlg : public CDialogEx
{
	// Attributions ***************************************************************************************
public:
	CBitmap m_oBmp{};
	BITMAP m_bmpInfo{};

	FILETIME m_ftIdleTime{};
	FILETIME m_ftKernelTime{};
	FILETIME m_ftUserTime{};
	long long m_llCPUUsageRatio{};
	DWORD m_dwMemoryRotio{};


	// Methods ********************************************************************************************
public:
	void FillCPUUsageRatio();























	DECLARE_DYNAMIC(CCircleDlg)

public:
	CCircleDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCircleDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDDLG_CIRCLE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};
