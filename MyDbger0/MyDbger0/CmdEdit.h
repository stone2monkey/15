#pragma once
#include "afxwin.h"
class CCmdEdit :
	public CEdit
{
	// Attributions ***************************************************************************************
public:


	// Methods ********************************************************************************************
public:
	void OnSeekSyms();		// 查看调试符号
	void OnSeekMem();		// 查看内存数据
	void OnSetHardWriteBP();// 设置硬件写断点
	void OnSetHardReadWriteBP();// 设置硬件读写断点
	void OnSetConditionHardReadWriteBP();// 设置条件硬件读写断点
	void OnSetMemBP();









public:
	CCmdEdit();
	~CCmdEdit();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

