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
	// 0.���������Խ��� ************************************************************
	STARTUPINFO SI{};
	SI.cb = sizeof(STARTUPINFO);
	PROCESS_INFORMATION PI{};
	BOOL bProcDbged = CreateProcess(g_pProcDbgedInfo->wcsFullName, 0, 0, 0, 0, DEBUG_ONLY_THIS_PROCESS, 0, 0, &SI, &PI);
	if (!bProcDbged) { assert(0); }

	// �õ�����ID
	g_pProcDbgedInfo->dwPID = PI.dwProcessId;

	// 1.��ѭ�������¼� ************************************************************
	DEBUG_EVENT DE{};
	while (true)
	{
		// 1.0 �ȴ�������ϵͳ���͵����¼� ************************************************************
		if (!WaitForDebugEvent(&DE, -1)) { assert(0); }

		// 1.1 ��������¼� ************************************************************
		BOOL bIsHandled{}; // ������,TRUE������,FALSEû����
		switch (DE.dwDebugEventCode)
		{
			case CREATE_PROCESS_DEBUG_EVENT:
				{
					// ���߳�,����dwTIP
					g_pProcDbgedInfo->dwTID = DE.dwThreadId;
					// PE�Ѿ�������
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

		// 1.2 �ظ�������ϵͳ ************************************************************
		if (bIsHandled)
			ContinueDebugEvent(DE.dwProcessId, DE.dwThreadId, DBG_CONTINUE);
		else
			ContinueDebugEvent(DE.dwProcessId, DE.dwThreadId, DBG_EXCEPTION_NOT_HANDLED);
	}

	return 0;
}

void CThreadProcOfBug::CREATE_PROCESS_DEBUG_EVENT_Handler(DEBUG_EVENT* pDE)
{
	// ��ʼ������EXEģ���PE ******************************************************************

	// CPE
	CPE* pPE = new CPE();
	pPE->Init((DWORD)pDE->u.CreateProcessInfo.lpBaseOfImage, g_pProcDbgedInfo->dwPID);

	// MODUELE_DBGED_INFO
	MODUELE_DBGED_INFO* p = new MODUELE_DBGED_INFO{};
	p->pPE = pPE;

	// ����
	if (g_vctModuleDbgedInfo.size()) { assert(0); }
	g_vctModuleDbgedInfo.push_back(p);
}

BOOL CThreadProcOfBug::EXCEPTION_DEBUG_EVENT_Handler(DEBUG_EVENT* pDE)
{
	// 0.��һ�δ�������쳣 ************************************************************
	static BOOL bIsFirstBP = TRUE;
	if (bIsFirstBP)
	{
		bIsFirstBP = FALSE;
		return FirstExceptionDbgEvtHandler(pDE);
	}

	// 1.�޸��ϵ�(ÿ���쳣�ͼ���Ƿ���Ҫ�޸��Ķϵ�,�ڵ��߳���û������,���߳�Ҫ����߳�id) ************************************************************
	{
		g_pInt3BP->RepairBK();//int3
	}
	//shiqianjukeng

	// 2.�Ƿ��ǵ��������õ��쳣,�õ�vct�е����� ************************************************************
	// 3.�ǵ��������õ��쳣,�޸�����;else,����FALSE,����������;TF���� ************************************************************
	// 4.��������������Ӧ���� ************************************************************
	switch (pDE->u.Exception.ExceptionRecord.ExceptionCode)		// �쳣����
	{
		case EXCEPTION_BREAKPOINT:		// int3
			{
				DWORD dwIndex{};	// ����
				if (g_pInt3BP->SeekBP((DWORD)pDE->u.Exception.ExceptionRecord.ExceptionAddress, dwIndex))
				{
					g_pInt3BP->RepairCode(g_pInt3BP->m_vctMyInt3BK.at(dwIndex)->dwAddr);	// ���ҵĶϵ�,�޸�����,�Ա�ִ��
					g_pInt3BP->RepairEip(pDE->dwThreadId);
					g_pInt3BP->m_vctMyInt3BK.at(dwIndex)->bNeedToRepair = TRUE;	// ���int3�ϵ���Ҫ�޸�

					break;
				}
				else
				{
					return FALSE;
				}
			}
			break;
		case EXCEPTION_SINGLE_STEP:		// TF (0x80000004L == EXCEPTION_SINGLE_STEP) 
			// ����ִ�������־��Ӳ��ִ����TF���õ�.Ҫ���ֵ�����ʲô�쳣;�������Ӳ���쳣,��ô����TF�쳣
			{
				// Ӳ���쳣
				DWORD dwAddr{}, dwBPType{}, dwIndex{};
				int iRes = CInterface::IsHardBP(g_pProcDbgedInfo->dwTID, dwAddr, dwBPType, dwIndex);
				if (1 == iRes)	// HB
				{
					switch (dwBPType)
					{
						case 0:	// Ӳ��ִ�жϵ�(ִ��ǰ����)
							{
								CStringW wcsTemp{};
								wcsTemp.Format(L"Ӳ��ִ�жϵ�:[0x%08x]!", dwAddr);
								::MessageBoxW(g_pMyDbger0Dlg->m_hWnd, wcsTemp, L"", 0);

								g_pHardExecuteBP->RepairHardExecuteCoed(g_pProcDbgedInfo->dwTID, dwIndex);
								g_iWhichTask = iTaskHBRepair;
							}
							break;
						case 1:	// Ӳ��д�ϵ�(ִ�к����)
							{
								CStringW wcsTemp{};
								wcsTemp.Format(L"Ӳ��д�ϵ�:[0x%08x]! [Eip:0x%08x]!", dwAddr, (DWORD)pDE->u.Exception.ExceptionRecord.ExceptionAddress);
								::MessageBoxW(g_pMyDbger0Dlg->m_hWnd, wcsTemp, L"", 0);
							}
							break;
						case 3:	// Ӳ����д�ϵ�(ִ�к����)
							{
								// �ǲ������µĶϵ�
								if (dwAddr == g_pHardReadWriteBP->m_ConditionBP.dwAddr)
								{
									// �鿴��ַ��������
									HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, g_pProcDbgedInfo->dwPID);
									if (!hProc) { assert(0); }
									DWORD dwData{};
									if (!::ReadProcessMemory(hProc, (LPVOID)dwAddr, &dwData, 4, 0)) { assert(0); }

									//
									if (dwData == g_pHardReadWriteBP->m_ConditionBP.dwData)
									{
										CStringW wcsTemp{};
										wcsTemp.Format(L"����Ӳ����д�ϵ�:[[0x%08x] == 0x%08x]! [Eip:0x%08x]!", dwAddr, dwData, (DWORD)pDE->u.Exception.ExceptionRecord.ExceptionAddress);
										::MessageBoxW(g_pMyDbger0Dlg->m_hWnd, wcsTemp, L"", 0);
										break;
									}
									else
										return TRUE;
								}


								CStringW wcsTemp{};
								wcsTemp.Format(L"Ӳ����д�ϵ�:[0x%08x]! [Eip:0x%08x]!", dwAddr, (DWORD)pDE->u.Exception.ExceptionRecord.ExceptionAddress);
								::MessageBoxW(g_pMyDbger0Dlg->m_hWnd, wcsTemp, L"", 0);
							}
							break;
					}
				}
				else if ((0 == iRes) || (-1 == iRes))	//TF
				{
					if (g_iWhichTask == iTaskNon)	// û������
					{
						return TRUE;
					}
					else if (g_iWhichTask == iTaskF9)	// ���������F9����
					{
						g_iWhichTask = iTaskNon;	
						return TRUE;
					}
					else if (g_iWhichTask == iTaskHBRepair)	// Ӳ���ϵ��޸�
					{
						CInterface::RepairHardBP(g_pProcDbgedInfo->dwTID);
						g_iWhichTask = iTaskNon;	
						return TRUE;
					}
					else if (g_iWhichTask == iTaskMBRepair)	// �ڴ�ϵ��޸�
					{
						g_pMemBP->AlterMemProtection();
					}
					else if (g_iWhichTask == iTaskF7)	// ���������F7����
					{
						g_iWhichTask = iTaskNon;	
					}
				}
			}
			break;
		case  EXCEPTION_ACCESS_VIOLATION:
			{
				// ������Ҫ���ڴ�ϵ�
				if (g_pMemBP->m_MyMemBP.dwAddr != pDE->u.Exception.ExceptionRecord.ExceptionInformation[1])
				{
					g_pMemBP->ResumeMemProtection();
					g_iWhichTask = iTaskMBRepair;
					g_pSingleStepBP->AddBP(pDE->dwThreadId);
					return TRUE;
				}

				// ����Ҫ���ڴ�ϵ�
				switch (g_pMemBP->m_MyMemBP.dwType)
				{
					case 0:	// �ڴ���ϵ�
						break;
					case 1:	// �ڴ�д�ϵ�
						break;
					case 8:	// �ڴ�ִ�жϵ�
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



	// 5.���½��� ************************************************************
	g_pMyDbger0Dlg->ResetReg(pDE->dwThreadId);	// Reg�������
	g_pMyDbger0Dlg->m_lcMem.GetMemData(g_pMyDbger0Dlg->m_dwEip, g_pProcDbgedInfo->dwPID);	// mem�������
	CInterface::DisasmListPageChanged((DWORD)pDE->u.Exception.ExceptionRecord.ExceptionAddress, 0);	// ��������
	g_pMyDbger0Dlg->m_lcStack.GetStackData();	// ջ����


	// 6.�ȴ��û�����һ������ʽ F7 F9 ? ************************************************************

	DWORD dwAddr{}, dwBPType{}, dwIndex{};
	int iRes = CInterface::IsHardBP(g_pProcDbgedInfo->dwTID, dwAddr, dwBPType, dwIndex);

	// �򿪿��޸�����BOOL
	g_bCanChangeTask = TRUE;
	//���¼�����
	HANDLE hEvnet = OpenEventW(EVENT_ALL_ACCESS, 0, L"g_hEvent");
	if (hEvnet == NULL) { assert(0); }
	//�ȴ��²�����
	WaitForSingleObject(hEvnet, -1);
	ResetEvent(hEvnet);
	CloseHandle(hEvnet);
	g_bCanChangeTask = FALSE;
	
	// 7.������Ӧ���� ************************************************************
	switch (g_iWhichTask)
	{
		//case iTaskNon:
		//	break;
		case iTaskF7:
			g_pSingleStepBP->AddBP(pDE->dwThreadId);	// �����F7����û�н�����
			g_pMyDbger0Dlg->m_wcsLog = L"TF�ϵ�,�� F7 or F9";
			::SetWindowTextW(g_pMyDbger0Dlg->GetDlgItem(IDEDT_LOG)->m_hWnd, g_pMyDbger0Dlg->m_wcsLog);
			return TRUE;
			break;
		case iTaskF9:	// Ҫ��һ��TF�ϵ�,Ϊ�˽����쳣��,Ȼ���һʱ���޸�,Ϊ��ִ���ƻ��Ķϵ�
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
		// 0.0 ��ʼ��ģ��ĵ��Է���,�õ����Է��ž��
		InitExeSymbol(pDE);

		// 0.1 ��ʼ��g_vctDisasmListItemInfo�����
		// ���g_vctDisasmListItemInfo
		DisasmAllInstruction(g_vctModuleDbgedInfo.at(0)->pPE->m_dwBaseOfCode, g_vctModuleDbgedInfo.at(0)->pPE->m_dwSizeOfCode);
	


		// �������(OEP)
		CInterface::DisasmListPageChanged(g_vctModuleDbgedInfo.at(0)->pPE->m_dwOEP, 0);

		// 0.2 ��OEP��int3�ϵ�
		g_pInt3BP->AddBP(g_vctModuleDbgedInfo.at(0)->pPE->m_dwOEP);

		// 
		g_pMyDbger0Dlg->m_wcsLog = L"int3�ϵ�,�� F7 or F9";
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
	// ���Է��ž���������,����Ϊ�˲��ظ�,����ģ��ĵ�ַ��;
	HANDLE hSymbol = (HANDLE)g_vctModuleDbgedInfo.at(0)->pPE->m_dwMod;
	DWORD dwBaseOfExe = g_vctModuleDbgedInfo.at(0)->pPE->m_dwMod;

	// ��ʼ�����Է���
	if (!::SymInitialize(hSymbol, 0, 0)) { assert(0); }

	// ���ص��Է���
	int iReq = WideCharToMultiByte(CP_ACP, 0, g_pProcDbgedInfo->wcsFullName, -1, 0, 0, 0, 0);
	char* strName = new char[iReq] {};
	WideCharToMultiByte(CP_ACP, 0, g_pProcDbgedInfo->wcsFullName, -1, strName, iReq, 0, 0);
	memcpy(strName + iReq - 5, ".pdb", 5);
	if (!::SymLoadModule64(hSymbol, 0, strName, 0, g_vctModuleDbgedInfo.at(0)->pPE->m_dwMod, 0))
	{ assert(0); }

	//����
	if (!g_vctModuleDbgedInfo.size()) { assert(0); }
	g_vctModuleDbgedInfo.at(0)->hSymbol = hSymbol;


	// ��ʼ�����з��ŵ�ַ
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
	// 1.�޸��ڴ��ַ�������� ************************************************************
	HANDLE hProc{};
	DWORD dwOldProtect{};
	DWORD dwSize = 16;
	CInterface::OpenProcAndAlterPage(hProc, g_pProcDbgedInfo->dwPID, dwAddr, dwSize, dwOldProtect);

	// ����16�ֽ�
	BYTE btRead[16]{};
	ReadProcessMemory(hProc, (LPVOID)dwAddr, btRead, 16, 0);

	// �����
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

	// ������g_vctDisasmListItemInfo
	DISASM_LIST_ITEM_INOF* p = new DISASM_LIST_ITEM_INOF{};
	p->dwAddr = dwAddr;
	p->dwLen = iLen;
	p->pBtOpcode = new BYTE[iLen]{};
	memcpy(p->pBtOpcode, btRead, iLen);
	p->wcsOrginalDisasm = disasm.CompleteInstr;
	p->wcsOrginalDisasm.MakeLower();
	p->wcsDisasmSymboled = p->wcsOrginalDisasm;

	g_vctDisasmListItemInfo.push_back(p);

	// 4.�ָ��ڴ��ַ�������� ************************************************************
	CInterface::ResumePageAndClsHandle(hProc, dwAddr, dwSize, dwOldProtect);
}

void CThreadProcOfBug::DisasmAllInstruction(DWORD dwAddr, DWORD m_dwSizeOfCode) 
{
	// 1.�޸��ڴ��ַ�������� ************************************************************
	HANDLE hProc{};
	DWORD dwOldProtect{};
	CInterface::OpenProcAndAlterPage(hProc, g_pProcDbgedInfo->dwPID, dwAddr, m_dwSizeOfCode, dwOldProtect);

	// �����ģ�����д���ָ��
	int iCount{};
	while (iCount < (int)m_dwSizeOfCode)
	{
		int iLen{};
		DisasmOneInstruction(dwAddr, iLen);
		if (-2 == iLen) return;

		dwAddr += iLen;
		iCount += iLen;
	}
	

	// 4.�ָ��ڴ��ַ�������� ************************************************************
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
