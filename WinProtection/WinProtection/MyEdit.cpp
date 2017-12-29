#include "stdafx.h"
#include "MyEdit.h"
#include "Interface.h"


CMyEdit::CMyEdit()
{
}


CMyEdit::~CMyEdit()
{
}
BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


void CMyEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CEdit::OnLButtonDown(nFlags, point);
}


void CMyEdit::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	UpdateData();
	GetWindowTextW(m_wcsPostfix);

	m_wcsFolderSelected = CInterface::SelectFolder();
	SetWindowTextW(m_wcsFolderSelected);


	CEdit::OnLButtonDblClk(nFlags, point);
}


void CMyEdit::OnRButtonDown(UINT nFlags, CPoint point)
{
	CFileDialog oFD(1);
	if (IDOK == oFD.DoModal())
	{
		m_wcsFileSelected = oFD.GetPathName();
		SetWindowTextW(m_wcsFileSelected);
	}

	CEdit::OnRButtonDown(nFlags, point);
}
