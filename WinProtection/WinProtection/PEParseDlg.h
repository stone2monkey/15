#pragma once
#include "afxcmn.h"
#include "PEData.h"
#include "afxwin.h"

#define IMPORTTABLE 1
#define RESOURCETABLE 2
#define BASERELOCATIONTABLE 3
class CMyModuleData;


// CPEParseDlg 对话框

class CPEParseDlg : public CDialogEx
{
	// Attributions ***************************************************************************************
public:
	HMODULE m_hOriginalMod{}, m_hCopyMod{};
	DWORD m_dwPID{};
	CMyModuleData* m_pMyModuleData{};
	CPEData* m_pPEData{};
	DWORD m_dwPageSize{};

	DWORD m_dwCBCurOwner{};		// ComboBox谁在用



// Methods ********************************************************************************************
public:
	void OnDllChangedInImportTable();
	void OnTypeChangedInResourceTable();
	void OnStartVAChangedInRelocationTable();





	
	DECLARE_DYNAMIC(CPEParseDlg)

public:
	CPEParseDlg(DWORD PID, CMyModuleData* pMyModuleData, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPEParseDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDDLG_PEPARSE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CListCtrl m_lc0;
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRclickIdlc0(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDh();
	afx_msg void OnNt();
	afx_msg void OnFh();
	afx_msg void OnOh();
	afx_msg void OnDdt();
	afx_msg void OnSht();
	afx_msg void OnViewet();
	afx_msg void OnViewit();
	afx_msg void OnViewrt();
	afx_msg void OnViewbrt();
	afx_msg void OnViewtlst();
	afx_msg void OnViewdlt();
	CComboBox m_cb0;
	afx_msg void OnSelchangeIdcb0();
};
