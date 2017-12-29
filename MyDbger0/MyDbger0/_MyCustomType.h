#pragma once
#include "stdafx.h"

class CPE;


// 进程信息
typedef struct _PROC_DBGED_INFO
{
	CStringW wcsFullName{};
	CStringW wcsPath{};
	CStringW wcsName{};
	CStringW wcsTitle{};
	CStringW wcsExt{};

	DWORD dwPID{};
	DWORD dwTID{};
}PROC_DBGED_INFO;

// 模块信息
typedef struct _MODUELE_DBGED_INFO
{
	CStringW wcsFullName{};
	CStringW wcsPath{};
	CStringW wcsName{};
	CStringW wcsTitle{};
	CStringW wcsExt{};

	CPE* pPE{};

	HANDLE hSymbol{};
}MODUELE_DBGED_INFO;

// 反汇编列表控件每项信息
typedef struct _DISASM_LIST_ITEM_INOF
{
	DWORD dwAddr{};
	BYTE* pBtOpcode{};
	CStringW wcsOrginalDisasm{};
	CStringW wcsDisasmSymboled{};
	CStringW wcsComment{};

	DWORD dwLen{};
}DISASM_LIST_ITEM_INOF;

// 异常类型
const DWORD int3 = 1;
const DWORD TF = 2;
const DWORD HB = 3;
const DWORD HE = 4;
const DWORD HW = 5;
const DWORD ME = 6;
const DWORD MR = 7;
const DWORD MW = 8;

// int3断点类型
typedef struct _MY_INT3_BP
{
	DWORD dwAddr{};
	BYTE btData[1];
	BOOL bNeedToRepair{};	// 如果为真时,表明这个地址的int3断点失效了,要修复
}MY_INT3_BP;

// 用户任务类型
const int iTaskNon = 0;
const int iTaskF7 = 1;
const int iTaskF9 = 2;
const int iTaskHBRepair = 3;
const int iTaskMBRepair = 4;
const int iTaskInt3Repair = 4;


// DR7
typedef struct _MY_REG_DR7
{
	unsigned L0 : 1;
	unsigned G0 : 1;

	unsigned L1 : 1;
	unsigned G1 : 1;

	unsigned L2 : 1;
	unsigned G2 : 1;

	unsigned L3 : 1;
	unsigned G3 : 1;

	unsigned LE : 1;
	unsigned GE : 1;

	unsigned RESERVEL1 : 3;
	unsigned GD : 1;
	unsigned RESERVEL2 : 2;

	unsigned RW0 : 2;
	unsigned LEN0 : 2;

	unsigned RW1 : 2;
	unsigned LEN1 : 2;

	unsigned RW2 : 2;
	unsigned LEN2 : 2;

	unsigned RW3 : 2;
	unsigned LEN3 : 2;

}MY_REG_DR7;

typedef struct _MY_REG_DR6
{
	unsigned B0 : 1;
	unsigned B1 : 1;
	unsigned B2 : 1;
	unsigned B3 : 1;


	unsigned RESERVEL1 : 9;


	
	unsigned BD : 1;
	unsigned BS : 1;
	unsigned BT : 1;


	unsigned RESERVEL2 : 16;

}MY_REG_DR6;

// 内存断点
typedef struct _MY_MEM_BP
{
	DWORD dwAddr{};
	DWORD dwType{};
	DWORD dwOldProtect{};
}MY_MEM_BP;

// 条件硬件读写断点
typedef struct _MY_CONDITION_HRW_BP
{
	DWORD dwAddr{};
	DWORD dwData{};
}MY_CONDITION_HRW_BP;