#include "stdafx.h"
#include <DbgHelp.h>
#include "ThreadProcOfBug.h"
#include "_CGlobal.h"
#include "PE.h"
#include "Interface.h"
#include "Cint3BP.h"
#include "SingleStepBP.h"
#include "MyDbger0Dlg.h"
#include "HardExecuteBP.h"
#include "MemBP.h"
#include "StackListCtrl.h"
#include <Psapi.h>
#include "HardReadWriteBP.h"
#include "resource.h"

#pragma comment(lib, "DbgHelp.lib")



DWORD WINAPI CThreadProcOfBug::ThreadProcOfBug(LPVOID lpThreadParameter)
{
	// 0.创建被调试进程 ************************************************************
	STARTUPINFO SI{};
	SI.cb = sizeof(STARTUPINFO);
	PROCESS_INFORMATION PI{};
	BOOL bProcDbged = CreateProcess(g_pProcDbgedInfo->wcsFullName, 0, 0, 0, 0, DEBUG_ONLY_THIS_PROCESS, 0, 0, &SI, &PI);
	if (!bProcDbged) { assert(0); }

	// 得到进程ID
	g_pProcDbgedInfo->dwPID = PI.dwProcessId;

	// 1.死循环调试事件 ************************************************************
	DEBUG_EVENT DE{};
	while (true)
	{
		// 1.0 等待调试子系统发送调试事件 ************************************************************
		if (!WaitForDebugEvent(&DE, -1)) { assert(0); }

		// 1.1 处理调试事件 ************************************************************
		BOOL bIsHandled{}; // 处理结果,TRUE处理了,FALSE没处理
		switch (DE.dwDebugEventCode)
		{
			case CREATE_PROCESS_DEBUG_EVENT:
				{
					// 单线程,所以dwTIP
					g_pProcDbgedInfo->dwTID = DE.dwThreadId;
					// PE已经被镜像
					CREATE_PROCESS_DEBUG_EVENT_Handler(&DE);
					bIsHandled = TRUE;
				}
				break;
			case CREATE_THREAD_DEBUG_EVENT:
				bIsHandled = TRUE;
				break;
			case EXCEPTION_DEBUG_EVENT:
				bIsHandled = EXCEPTION_DEBUG_EVENT_Handler(&DE);
				break;
			case EXIT_PROCESS_DEBUG_EVENT:
				bIsHandled = TRUE;
				break;
			case EXIT_THREAD_DEBUG_EVENT:
				bIsHandled = TRUE;
				break;
			case LOAD_DLL_DEBUG_EVENT:
				LOAD_DLL_DEBUG_EVENT_Handler(&DE);
				bIsHandled = TRUE;
				break;
			case UNLOAD_DLL_DEBUG_EVENT:
				UNLOAD_DLL_DEBUG_EVENT_Handler(&DE);
				bIsHandled = TRUE;
				break;
			case OUTPUT_DEBUG_STRING_EVENT:
				bIsHandled = TRUE;
				break;
			case RIP_EVENT:
				bIsHandled = TRUE;
				break;
		}

		// 1.2 回复调试子系统 ************************************************************
		if (bIsHandled)
			ContinueDebugEvent(DE.dwProcessId, DE.dwThreadId, DBG_CONTINUE);
		else
			ContinueDebugEvent(DE.dwProcessId, DE.dwThreadId, DBG_EXCEPTION_NOT_HANDLED);
	}

	return 0;
}

void CThreadProcOfBug::CREATE_PROCESS_DEBUG_EVENT_Handler(DEBUG_EVENT* pDE)
{
	// 初始化进程EXE模块的PE ******************************************************************

	// CPE
	CPE* pPE = new CPE();
	pPE->Init((DWORD)pDE->u.CreateProcessInfo.lpBaseOfImage, g_pProcDbgedInfo->dwPID);

	// MODUELE_DBGED_INFO
	MODUELE_DBGED_INFO* p = new MODUELE_DBGED_INFO{};
	p->pPE = pPE;

	// 保存
	if (g_vctModuleDbgedInfo.size()) { assert(0); }
	g_vctModuleDbgedInfo.push_back(p);
}

BOOL CThreadProcOfBug::EXCEPTION_DEBUG_EVENT_Handler(DEBUG_EVENT* pDE)
{
	// 0.第一次触发这个异常 ************************************************************
	static BOOL bIsFirstBP = TRUE;
	if (bIsFirstBP)
	{
		bIsFirstBP = FALSE;
		return FirstExceptionDbgEvtHandler(pDE);
	}

	// 1.修复断点(每次异常就检查是否有要修复的断点,在单线程中没有问题,多线程要检测线程id) ************************************************************
	{
		g_pInt3BP->RepairBK();//int3
	}
	//shiqianjukeng

	// 2.是否是调试器设置的异常,得到vct中的索引 ************************************************************
	// 3.是调试器设置的异常,修复处理;else,返回FALSE,交给程序处理;TF除外 ************************************************************
	// 4.跟据类型做出相应处理 ************************************************************
	switch (pDE->u.Exception.ExceptionRecord.ExceptionCode)		// 异常分类
	{
		case EXCEPTION_BREAKPOINT:		// int3
			{
				DWORD dwIndex{};	// 索引
				if (g_pInt3BP->SeekBP((DWORD)pDE->u.Exception.ExceptionRecord.ExceptionAddress, dwIndex))
				{
					g_pInt3BP->RepairCode(g_pInt3BP->m_vctMyInt3BK.at(dwIndex)->dwAddr);	// 是我的断点,修复代码,以便执行
					g_pInt3BP->RepairEip(pDE->dwThreadId);
					g_pInt3BP->m_vctMyInt3BK.at(dwIndex)->bNeedToRepair = TRUE;	// 这个int3断点需要修复

					break;
				}
				else
				{
					return FALSE;
				}
			}
			break;
		case EXCEPTION_SINGLE_STEP:		// TF (0x80000004L == EXCEPTION_SINGLE_STEP) 
			// 单步执行这个标志是硬件执行与TF共用的.要区分到低是什么异常;如果不是硬件异常,那么才是TF异常
			{
				// 硬件异常
				DWORD dwAddr{}, dwBPType{}, dwIndex{};
				int iRes = CInterface::IsHardBP(g_pProcDbgedInfo->dwTID, dwAddr, dwBPType, dwIndex);
				if (1 == iRes)	// HB
				{
					switch (dwBPType)
					{
						case 0:	// 硬件执行断点(执行前断下)
							{
								CStringW wcsTemp{};
								wcsTemp.Format(L"硬件执行断点:[0x%08x]!", dwAddr);
								::MessageBoxW(g_pMyDbger0Dlg->m_hWnd, wcsTemp, L"", 0);

								g_pHardExecuteBP->RepairHardExecuteCoed(g_pProcDbgedInfo->dwTID, dwIndex);
								g_iWhichTask = iTaskHBRepair;
							}
							break;
						case 1:	// 硬件写断点(执行后断下)
							{
								CStringW wcsTemp{};
								wcsTemp.Format(L"硬件写断点:[0x%08x]! [Eip:0x%08x]!", dwAddr, (DWORD)pDE->u.Exception.ExceptionRecord.ExceptionAddress);
								::MessageBoxW(g_pMyDbger0Dlg->m_hWnd, wcsTemp, L"", 0);
							}
							break;
						case 3:	// 硬件读写断点(执行后断下)
							{
								// 是不是我下的断点
								if (dwAddr == g_pHardReadWriteBP->m_ConditionBP.dwAddr)
								{
									// 查看地址处的数据
									HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, g_pProcDbgedInfo->dwPID);
									if (!hProc) { assert(0); }
									DWORD dwData{};
									if (!::ReadProcessMemory(hProc, (LPVOID)dwAddr, &dwData, 4, 0)) { assert(0); }

									//
									if (dwData == g_pHardReadWriteBP->m_ConditionBP.dwData)
									{
										CStringW wcsTemp{};
										wcsTemp.Format(L"条件硬件读写断点:[[0x%08x] == 0x%08x]! [Eip:0x%08x]!", dwAddr, dwData, (DWORD)pDE->u.Exception.ExceptionRecord.ExceptionAddress);
										::MessageBoxW(g_pMyDbger0Dlg->m_hWnd, wcsTemp, L"", 0);
										break;
									}
									else
										return TRUE;
								}


								CStringW wcsTemp{};
								wcsTemp.Format(L"硬件读写断点:[0x%08x]! [Eip:0x%08x]!", dwAddr, (DWORD)pDE->u.Exception.ExceptionRecord.ExceptionAddress);
								::MessageBoxW(g_pMyDbger0Dlg->m_hWnd, wcsTemp, L"", 0);
							}
							break;
					}
				}
				else if ((0 == iRes) || (-1 == iRes))	//TF
				{
					if (g_iWhichTask == iTaskNon)	// 没有任务
					{
						return TRUE;
					}
					else if (g_iWhichTask == iTaskF9)	// 在这里结束F9任务
					{
						g_iWhichTask = iTaskNon;	
						return TRUE;
					}
					else if (g_iWhichTask == iTaskHBRepair)	// 硬件断点修复
					{
						CInterface::RepairHardBP(g_pProcDbgedInfo->dwTID);
						g_iWhichTask = iTaskNon;	
						return TRUE;
					}
					else if (g_iWhichTask == iTaskMBRepair)	// 内存断点修复
					{
						g_pMemBP->AlterMemProtection();
					}
					else if (g_iWhichTask == iTaskF7)	// 在这里结束F7任务
					{
						g_iWhichTask = iTaskNon;	
					}
				}
			}
			break;
		case  EXCEPTION_ACCESS_VIOLATION:
			{
				// 不是我要的内存断点
				if (g_pMemBP->m_MyMemBP.dwAddr != pDE->u.Exception.ExceptionRecord.ExceptionInformation[1])
				{
					g_pMemBP->ResumeMemProtection();
					g_iWhichTask = iTaskMBRepair;
					g_pSingleStepBP->AddBP(pDE->dwThreadId);
					return TRUE;
				}

				// 是我要的内存断点
				switch (g_pMemBP->m_MyMemBP.dwType)
				{
					case 0:	// 内存读断点
						break;
					case 1:	// 内存写断点
						break;
					case 8:	// 内存执行断点
						{
							g_pMemBP->ResumeMemProtection();
							g_iWhichTask = iTaskMBRepair;
						}
						break;
				}
			}
			break;
		default:
			return FALSE;
			break;
	}



	// 5.更新界面 ************************************************************
	g_pMyDbger0Dlg->ResetReg(pDE->dwThreadId);	// Reg界面跟进
	g_pMyDbger0Dlg->m_lcMem.GetMemData(g_pMyDbger0Dlg->m_dwEip, g_pProcDbgedInfo->dwPID);	// mem界面跟进
	CInterface::DisasmListPageChanged((DWORD)pDE->u.Exception.ExceptionRecord.ExceptionAddress, 0);	// 反汇编界面
	g_pMyDbger0Dlg->m_lcStack.GetStackData();	// 栈界面


	// 6.等待用户的下一步处理方式 F7 F9 ? ************************************************************

	DWORD dwAddr{}, dwBPType{}, dwIndex{};
	int iRes = CInterface::IsHardBP(g_pProcDbgedInfo->dwTID, dwAddr, dwBPType, dwIndex);

	// 打开可修改任务BOOL
	g_bCanChangeTask = TRUE;
	//打开事件对象
	HANDLE hEvnet = OpenEventW(EVENT_ALL_ACCESS, 0, L"g_hEvent");
	if (hEvnet == NULL) { assert(0); }
	//等待事伯对象
	WaitForSingleObject(hEvnet, -1);
	ResetEvent(hEvnet);
	CloseHandle(hEvnet);
	g_bCanChangeTask = FALSE;
	
	// 7.做出相应处理 ************************************************************
	switch (g_iWhichTask)
	{
		//case iTaskNon:
		//	break;
		case iTaskF7:
			g_pSingleStepBP->AddBP(pDE->dwThreadId);	// 这里的F7任务没有结束的
			g_pMyDbger0Dlg->m_wcsLog = L"TF断点,请 F7 or F9";
			::SetWindowTextW(g_pMyDbger0Dlg->GetDlgItem(IDEDT_LOG)->m_hWnd, g_pMyDbger0Dlg->m_wcsLog);
			return TRUE;
			break;
		case iTaskF9:	// 要来一次TF断点,为了进入异常中,然后第一时间修复,为了执行破坏的断点
			g_pSingleStepBP->AddBP(pDE->dwThreadId);
			return TRUE;
			break;
		

		default:
			assert(0);
	}

	
	// 7. ************************************************************
	// 8. ************************************************************
}

BOOL CThreadProcOfBug::FirstExceptionDbgEvtHandler(DEBUG_EVENT * pDE)
{
		// 0.0 初始化模块的调试符号,得到调试符号句柄
		InitExeSymbol(pDE);

		// 0.1 初始化g_vctDisasmListItemInfo与界面
		// 填充g_vctDisasmListItemInfo
		DisasmAllInstruction(g_vctModuleDbgedInfo.at(0)->pPE->m_dwBaseOfCode, g_vctModuleDbgedInfo.at(0)->pPE->m_dwSizeOfCode);
	


		// 界面跟进(OEP)
		CInterface::DisasmListPageChanged(g_vctModuleDbgedInfo.at(0)->pPE->m_dwOEP, 0);

		// 0.2 给OEP下int3断点
		g_pInt3BP->AddBP(g_vctModuleDbgedInfo.at(0)->pPE->m_dwOEP);

		// 
		g_pMyDbger0Dlg->m_wcsLog = L"int3断点,请 F7 or F9";
		::SetWindowTextW(g_pMyDbger0Dlg->GetDlgItem(IDEDT_LOG)->m_hWnd, g_pMyDbger0Dlg->m_wcsLog);


		return 1;
}

void CThreadProcOfBug::LOAD_DLL_DEBUG_EVENT_Handler(DEBUG_EVENT* pDE)
{
}

void CThreadProcOfBug::UNLOAD_DLL_DEBUG_EVENT_Handler(DEBUG_EVENT* pDE)
{ 
}

void CThreadProcOfBug::InitExeSymbol(DEBUG_EVENT * pDE)
{
	// 调试符号句柄是随机的,但是为了不重复,就用模块的地址吧;
	HANDLE hSymbol = (HANDLE)g_vctModuleDbgedInfo.at(0)->pPE->m_dwMod;
	DWORD dwBaseOfExe = g_vctModuleDbgedInfo.at(0)->pPE->m_dwMod;

	// 初始化调试符号
	if (!::SymInitialize(hSymbol, 0, 0)) { assert(0); }

	// 加载调试符号
	int iReq = WideCharToMultiByte(CP_ACP, 0, g_pProcDbgedInfo->wcsFullName, -1, 0, 0, 0, 0);
	char* strName = new char[iReq] {};
	WideCharToMultiByte(CP_ACP, 0, g_pProcDbgedInfo->wcsFullName, -1, strName, iReq, 0, 0);
	memcpy(strName + iReq - 5, ".pdb", 5);
	if (!::SymLoadModule64(hSymbol, 0, strName, 0, g_vctModuleDbgedInfo.at(0)->pPE->m_dwMod, 0))
	{ assert(0); }

	//保存
	if (!g_vctModuleDbgedInfo.size()) { assert(0); }
	g_vctModuleDbgedInfo.at(0)->hSymbol = hSymbol;


	// 初始化所有符号地址
	DWORD dwCount = g_vctWcsSymbol.size();
	for (DWORD i = 0; i < dwCount; i++)
	{
		// variable
		char strSI[sizeof(SYMBOL_INFO) + MAX_SYM_NAME]{};
		SYMBOL_INFO* pSI = (SYMBOL_INFO*)strSI;
		pSI->SizeOfStruct = sizeof(SYMBOL_INFO);
		pSI->MaxNameLen = MAX_SYM_NAME;

		// w to a
		int iReq = WideCharToMultiByte(CP_ACP, 0, g_vctWcsSymbol.at(i), -1, 0, 0, 0, 0);
		char* strName = new char[iReq] {};
		WideCharToMultiByte(CP_ACP, 0, g_vctWcsSymbol.at(i), -1, strName, iReq, 0, 0);

		// seek
		if (!SymFromName(hSymbol, strName, pSI)) { assert(0); }

		// save
		CStringW wcsTemp{};
		wcsTemp.Format(L"%08xh", pSI->Address);
		g_vctWcsAddrOfSymbol.push_back(wcsTemp);

		//end
		delete[] strName;
	}

	delete[] strName;
}

void CThreadProcOfBug::DisasmOneInstruction(DWORD dwAddr, int & iLen)
{
	// 1.修改内存地址处的属性 ************************************************************
	HANDLE hProc{};
	DWORD dwOldProtect{};
	DWORD dwSize = 16;
	CInterface::OpenProcAndAlterPage(hProc, g_pProcDbgedInfo->dwPID, dwAddr, dwSize, dwOldProtect);

	// 读出16字节
	BYTE btRead[16]{};
	ReadProcessMemory(hProc, (LPVOID)dwAddr, btRead, 16, 0);

	// 反汇编
	DISASM disasm = { 0 };
	disasm.Archi = 0;
	disasm.EIP = (UIntPtr)btRead;
	disasm.VirtualAddr = dwAddr;

	iLen = Disasm(&disasm);

	if (-1 == iLen) 
	{
		iLen = -2;
		return; 
	}

	// 保存入g_vctDisasmListItemInfo
	DISASM_LIST_ITEM_INOF* p = new DISASM_LIST_ITEM_INOF{};
	p->dwAddr = dwAddr;
	p->dwLen = iLen;
	p->pBtOpcode = new BYTE[iLen]{};
	memcpy(p->pBtOpcode, btRead, iLen);
	p->wcsOrginalDisasm = disasm.CompleteInstr;
	p->wcsOrginalDisasm.MakeLower();
	p->wcsDisasmSymboled = p->wcsOrginalDisasm;

	g_vctDisasmListItemInfo.push_back(p);

	// 4.恢复内存地址处的属性 ************************************************************
	CInterface::ResumePageAndClsHandle(hProc, dwAddr, dwSize, dwOldProtect);
}

void CThreadProcOfBug::DisasmAllInstruction(DWORD dwAddr, DWORD m_dwSizeOfCode) 
{
	// 1.修改内存地址处的属性 ************************************************************
	HANDLE hProc{};
	DWORD dwOldProtect{};
	CInterface::OpenProcAndAlterPage(hProc, g_pProcDbgedInfo->dwPID, dwAddr, m_dwSizeOfCode, dwOldProtect);

	// 反汇编模块所有代码指令
	int iCount{};
	while (iCount < (int)m_dwSizeOfCode)
	{
		int iLen{};
		DisasmOneInstruction(dwAddr, iLen);
		if (-2 == iLen) return;

		dwAddr += iLen;
		iCount += iLen;
	}
	

	// 4.恢复内存地址处的属性 ************************************************************
	CInterface::ResumePageAndClsHandle(hProc, dwAddr, m_dwSizeOfCode, dwOldProtect);
}

void CThreadProcOfBug::MakeAddrToSymbol(CStringW & wcsSymbol)
{
	DWORD dwCount = g_vctWcsAddrOfSymbol.size();
	for (DWORD i = 0; i < dwCount; i++)
	{
		wcsSymbol.Replace(g_vctWcsAddrOfSymbol.at(i), g_vctWcsSymbol.at(i));
	}
}

CThreadProcOfBug::CThreadProcOfBug()
{
}


CThreadProcOfBug::~CThreadProcOfBug()
{
}
