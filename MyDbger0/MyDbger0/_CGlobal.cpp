#include "stdafx.h"
#include "_CGlobal.h"
#include "Cint3BP.h"
#include "SingleStepBP.h"
#include "HardExecuteBP.h"
#include "HardWriteBP.h"
#include "MemBP.h"
#include "HardReadWriteBP.h"


// Attributions ***************************************************************************************
CMyDbger0Dlg* g_pMyDbger0Dlg{};
PROC_DBGED_INFO* g_pProcDbgedInfo{};
vector<MODUELE_DBGED_INFO*> g_vctModuleDbgedInfo{};
int g_iExceptionHandledMethod{};
vector<DISASM_LIST_ITEM_INOF*> g_vctDisasmListItemInfo{};
DWORD g_dwIndexOfDisasmList = -1;


HANDLE g_hSymbol{};	// ������ģ������Ȱ�
vector<CStringW> g_vctWcsSymbol{L"g_i0", L"g_i1", L"Fun"};	// ���Խ����ķ���
vector<CStringW> g_vctWcsAddrOfSymbol{};	//���ŵĵ�ַ

Cint3BP* g_pInt3BP = new Cint3BP();
CSingleStepBP* g_pSingleStepBP = new CSingleStepBP();
CHardExecuteBP* g_pHardExecuteBP = new CHardExecuteBP();
CHardWriteBP* g_pHardWriteBP = new CHardWriteBP();
CHardReadWriteBP* g_pHardReadWriteBP = new CHardReadWriteBP();
CMemBP* g_pMemBP = new CMemBP();
DWORD g_dwNeedToRepairHardBPAddr{};

HANDLE g_hEvent;		// �̼߳�֪ͨ
int g_iWhichTask{};	// �ĸ�����,�ü�F7 F9	
BOOL g_bCanChangeTask{};// �Ƿ�����޸�����


// Methods ********************************************************************************************


















































_CGlobal::_CGlobal()
{
}


_CGlobal::~_CGlobal()
{
}
