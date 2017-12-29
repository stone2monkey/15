#pragma once
/*******************************/
#define BEA_ENGINE_STATIC
#define BEA_USE_STDCALL
#include "Bea/headers/BeaEngine.h"
#pragma comment(lib, "Bea/Win32/Lib/BeaEngine.lib")
#pragma comment(linker, "/NODEFAULTLIB:\"crt.lib\"")
/*******************************/
#include <strsafe.h>


class CThreadProcOfBug
{
	// Attributions ***************************************************************************************
public:



	// Methods ********************************************************************************************
public:
	// 这是调试线程
	static DWORD WINAPI ThreadProcOfBug(LPVOID lpThreadParameter);

	//****************************************************
	// Name: CThreadProcOfBug::CREATE_PROCESS_DEBUG_EVENT_Handler
	// Func: 这里是有被调试进程的EXE模块句柄, 
	// Retn: void
	//****************************************************
	static void CREATE_PROCESS_DEBUG_EVENT_Handler(DEBUG_EVENT* pDE);

	//****************************************************
	// Name: CThreadProcOfBug::EXCEPTION_DEBUG_EVENT_Handler
	// Func: 第一次是触发是系统将把执行权给程序时,下一步是程序自己去执行OPE了,所以...
	// 后面的触发就要先过滤,得到想要的断点类型(F2, F7, F8, HB, HR, HW, MB, MR, MW), 然后用一个事件去
	// 等待用户的命令,方式是:主窗创建事件内核对象,F7 F8 F9修改一个全局变量,以使这里知道怎么处理这个异常.
	// 激活事件,这里的等待事件激活,执行处理.
	// Retn: void
	//****************************************************
	static BOOL EXCEPTION_DEBUG_EVENT_Handler(DEBUG_EVENT* pDE);

	//****************************************************
	// Name: CThreadProcOfBug::FirstExceptionDbgEvtHandler
	// Func: EXCEPTION_DEBUG_EVENT的第0个异常调试事件处理,它通知:这是系统把执行权限交给程序,下一步是执行OPE了
	// Args: DEBUG_EVENT * pDE
	// Retn: BOOL
	//****************************************************
	static BOOL FirstExceptionDbgEvtHandler(DEBUG_EVENT* pDE);

	//****************************************************
	// Name: CThreadProcOfBug::LOAD_DLL_DEBUG_EVENT_Handler
	// Func: 这里可以得到模块信息,加载调试符号
	// Retn: void
	//****************************************************
	static void LOAD_DLL_DEBUG_EVENT_Handler(DEBUG_EVENT* pDE);

	//****************************************************
	// Name: CThreadProcOfBug::UNLOAD_DLL_DEBUG_EVENT_Handler
	// Func: 做加载相反的工作
	// Retn: void
	//****************************************************
	static void UNLOAD_DLL_DEBUG_EVENT_Handler(DEBUG_EVENT* pDE);

	static void InitExeSymbol(DEBUG_EVENT* pDE);


	//****************************************************
	// Name: CThreadProcOfBug::AsmOneInstruction
	// Func: 反汇编一条指令
	// Args: DWORD dwAddr
	// Args: int & iLen
	// Retn: void
	//****************************************************
	static void DisasmOneInstruction(DWORD dwAddr, int& iLen);

	//****************************************************
	// Name: CThreadProcOfBug::DisasmAllInstruction
	// Func: 反汇编模块所有代码指令
	// Args: DWORD dwAddr
	// Args: DWORD m_dwSizeOfCode
	// Retn: void
	//****************************************************
	static void DisasmAllInstruction(DWORD dwAddr, DWORD m_dwSizeOfCode);

	//****************************************************
	// Name: CThreadProcOfBug::MakeAddrToSymbol
	// Func: 传进来一个字符串,如果与地址表中的字符串对上,就改它为符号表中的对应字符串
	// Args: CStringW & wcsSymbol
	// Retn: void
	//****************************************************
	static void MakeAddrToSymbol(CStringW& wcsSymbol);












public:
	CThreadProcOfBug();
	~CThreadProcOfBug();
};

