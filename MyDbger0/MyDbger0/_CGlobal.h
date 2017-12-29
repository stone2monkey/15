#pragma once
#include <vector>
#include "_MyCustomType.h"

using std::vector;
class CMyDbger0Dlg;
class Cint3BP;
class CSingleStepBP;
class CHardExecuteBP;
class CHardWriteBP;
class CHardReadWriteBP;
class CMemBP;



// Attributions ***************************************************************************************
extern CMyDbger0Dlg* g_pMyDbger0Dlg;		// 主窗对话框指针
extern PROC_DBGED_INFO* g_pProcDbgedInfo;	// 被调试进程信息
extern vector<MODUELE_DBGED_INFO*> g_vctModuleDbgedInfo;	// 被调试进程所有模块的信息
extern int g_iExceptionHandledMethod;		// 过滤后的异常的处理方法
extern vector<DISASM_LIST_ITEM_INOF*> g_vctDisasmListItemInfo;	//反汇编列表数据集
extern DWORD g_dwIndexOfDisasmList;		// 反汇编列表中第0项在vct中的索引,初始化为-1,表示错误.

// 调试符号相关
extern HANDLE g_hSymbol;	// 让它与模块句柄相等吧
extern vector<CStringW> g_vctWcsSymbol;	// 可以解析的符号
extern vector<CStringW> g_vctWcsAddrOfSymbol;	//符号的地址

// 断点
extern Cint3BP* g_pInt3BP;
extern CSingleStepBP* g_pSingleStepBP;
extern CHardExecuteBP* g_pHardExecuteBP;
extern CHardWriteBP* g_pHardWriteBP;
extern CHardReadWriteBP* g_pHardReadWriteBP;
extern CMemBP* g_pMemBP;
extern DWORD g_dwNeedToRepairHardBPAddr;		// 硬件断点要修复地址

// 支持调试线程与主线程的通信
extern HANDLE g_hEvent;		// 线程间通知
extern int g_iWhichTask;	// 哪个任务,用记F7 F9	
extern BOOL g_bCanChangeTask;// 是否可以修改任务



// Methods ********************************************************************************************














































class _CGlobal
{
public:
	_CGlobal();
	~_CGlobal();
};

