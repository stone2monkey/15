#pragma once
#include "afxwin.h"
class CCmdEdit :
	public CEdit
{
	// Attributions ***************************************************************************************
public:


	// Methods ********************************************************************************************
public:
	void OnSeekSyms();		// �鿴���Է���
	void OnSeekMem();		// �鿴�ڴ�����
	void OnSetHardWriteBP();// ����Ӳ��д�ϵ�
	void OnSetHardReadWriteBP();// ����Ӳ����д�ϵ�
	void OnSetConditionHardReadWriteBP();// ��������Ӳ����д�ϵ�
	void OnSetMemBP();









public:
	CCmdEdit();
	~CCmdEdit();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

