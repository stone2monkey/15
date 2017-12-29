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
	// ���ǵ����߳�
	static DWORD WINAPI ThreadProcOfBug(LPVOID lpThreadParameter);

	//****************************************************
	// Name: CThreadProcOfBug::CREATE_PROCESS_DEBUG_EVENT_Handler
	// Func: �������б����Խ��̵�EXEģ����, 
	// Retn: void
	//****************************************************
	static void CREATE_PROCESS_DEBUG_EVENT_Handler(DEBUG_EVENT* pDE);

	//****************************************************
	// Name: CThreadProcOfBug::EXCEPTION_DEBUG_EVENT_Handler
	// Func: ��һ���Ǵ�����ϵͳ����ִ��Ȩ������ʱ,��һ���ǳ����Լ�ȥִ��OPE��,����...
	// ����Ĵ�����Ҫ�ȹ���,�õ���Ҫ�Ķϵ�����(F2, F7, F8, HB, HR, HW, MB, MR, MW), Ȼ����һ���¼�ȥ
	// �ȴ��û�������,��ʽ��:���������¼��ں˶���,F7 F8 F9�޸�һ��ȫ�ֱ���,��ʹ����֪����ô��������쳣.
	// �����¼�,����ĵȴ��¼�����,ִ�д���.
	// Retn: void
	//****************************************************
	static BOOL EXCEPTION_DEBUG_EVENT_Handler(DEBUG_EVENT* pDE);

	//****************************************************
	// Name: CThreadProcOfBug::FirstExceptionDbgEvtHandler
	// Func: EXCEPTION_DEBUG_EVENT�ĵ�0���쳣�����¼�����,��֪ͨ:����ϵͳ��ִ��Ȩ�޽�������,��һ����ִ��OPE��
	// Args: DEBUG_EVENT * pDE
	// Retn: BOOL
	//****************************************************
	static BOOL FirstExceptionDbgEvtHandler(DEBUG_EVENT* pDE);

	//****************************************************
	// Name: CThreadProcOfBug::LOAD_DLL_DEBUG_EVENT_Handler
	// Func: ������Եõ�ģ����Ϣ,���ص��Է���
	// Retn: void
	//****************************************************
	static void LOAD_DLL_DEBUG_EVENT_Handler(DEBUG_EVENT* pDE);

	//****************************************************
	// Name: CThreadProcOfBug::UNLOAD_DLL_DEBUG_EVENT_Handler
	// Func: �������෴�Ĺ���
	// Retn: void
	//****************************************************
	static void UNLOAD_DLL_DEBUG_EVENT_Handler(DEBUG_EVENT* pDE);

	static void InitExeSymbol(DEBUG_EVENT* pDE);


	//****************************************************
	// Name: CThreadProcOfBug::AsmOneInstruction
	// Func: �����һ��ָ��
	// Args: DWORD dwAddr
	// Args: int & iLen
	// Retn: void
	//****************************************************
	static void DisasmOneInstruction(DWORD dwAddr, int& iLen);

	//****************************************************
	// Name: CThreadProcOfBug::DisasmAllInstruction
	// Func: �����ģ�����д���ָ��
	// Args: DWORD dwAddr
	// Args: DWORD m_dwSizeOfCode
	// Retn: void
	//****************************************************
	static void DisasmAllInstruction(DWORD dwAddr, DWORD m_dwSizeOfCode);

	//****************************************************
	// Name: CThreadProcOfBug::MakeAddrToSymbol
	// Func: ������һ���ַ���,������ַ���е��ַ�������,�͸���Ϊ���ű��еĶ�Ӧ�ַ���
	// Args: CStringW & wcsSymbol
	// Retn: void
	//****************************************************
	static void MakeAddrToSymbol(CStringW& wcsSymbol);












public:
	CThreadProcOfBug();
	~CThreadProcOfBug();
};

