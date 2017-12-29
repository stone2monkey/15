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
extern CMyDbger0Dlg* g_pMyDbger0Dlg;		// �����Ի���ָ��
extern PROC_DBGED_INFO* g_pProcDbgedInfo;	// �����Խ�����Ϣ
extern vector<MODUELE_DBGED_INFO*> g_vctModuleDbgedInfo;	// �����Խ�������ģ�����Ϣ
extern int g_iExceptionHandledMethod;		// ���˺���쳣�Ĵ�����
extern vector<DISASM_LIST_ITEM_INOF*> g_vctDisasmListItemInfo;	//������б����ݼ�
extern DWORD g_dwIndexOfDisasmList;		// ������б��е�0����vct�е�����,��ʼ��Ϊ-1,��ʾ����.

// ���Է������
extern HANDLE g_hSymbol;	// ������ģ������Ȱ�
extern vector<CStringW> g_vctWcsSymbol;	// ���Խ����ķ���
extern vector<CStringW> g_vctWcsAddrOfSymbol;	//���ŵĵ�ַ

// �ϵ�
extern Cint3BP* g_pInt3BP;
extern CSingleStepBP* g_pSingleStepBP;
extern CHardExecuteBP* g_pHardExecuteBP;
extern CHardWriteBP* g_pHardWriteBP;
extern CHardReadWriteBP* g_pHardReadWriteBP;
extern CMemBP* g_pMemBP;
extern DWORD g_dwNeedToRepairHardBPAddr;		// Ӳ���ϵ�Ҫ�޸���ַ

// ֧�ֵ����߳������̵߳�ͨ��
extern HANDLE g_hEvent;		// �̼߳�֪ͨ
extern int g_iWhichTask;	// �ĸ�����,�ü�F7 F9	
extern BOOL g_bCanChangeTask;// �Ƿ�����޸�����



// Methods ********************************************************************************************














































class _CGlobal
{
public:
	_CGlobal();
	~_CGlobal();
};

