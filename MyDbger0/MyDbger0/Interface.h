#pragma once
class CInterface
{
	// Attributions ***************************************************************************************
public:
	//****************************************************
	// Name: CInterface::SelectExeDbged
	// Func: ��ʼ��g_pProcDbgedInfo���ַ�����Ϣ
	// Retn: void
	//****************************************************
	static void SelectExeDbged();

	// ��һ���㹻Ȩ�޾��,�޸��ڴ�����
	static void OpenProcAndAlterPage(_Out_ HANDLE& hProc, _In_ DWORD dwPID, DWORD dwAddrAltered, DWORD dwSize, _Out_ DWORD& dwOldProtect);
	// �ָ��ڴ�����,�رվ��
	static void ResumePageAndClsHandle(_In_ HANDLE hProc, DWORD dwAddrAltered, DWORD dwSize, _In_ DWORD dwNewProtect);


	//****************************************************
	// Name: CInterface::BinaryToWChar
	// Func: �Ѷ�������������ֵ,תΪ�ַ���
	// Args: BYTE * pBinary
	// Args: int iSize
	// Args: CStringW & wcsBinaryString
	// Retn: void
	//****************************************************
	static void BinaryToWChar(BYTE* pBinary, int iSize, CStringW& wcsBinaryString);

	//****************************************************
	// Name: CInterface::DisasmListPageChanged
	// Func: ���ݵ�ַ�õ��б��37��,ѡ��������˭��
	// Args: DWORD dwAddr
	// Args: int iSelection
	// Retn: void
	//****************************************************
	static void DisasmListPageChanged(DWORD dwAddr, int iSelection);

	//****************************************************
	// Name: CInterface::IsHardBP
	// Func: ͨ����ַ�鿴�ǲ���Ӳ���ϵ�&TF,�����Ӳ���ϵ㷵�ص�ַ,����,λ��
	// Args: DWORD dwTID
	// Args: DWORD dwAddr
	// Retn: int
	// return -1;	û���ҵ�
	// return 0;	�ҵ���TF�ϵ�
	// return 1;	�ҵ���Ӳ���ϵ�
	//****************************************************
	static int IsHardBP(DWORD dwTID, _Out_ DWORD& dwAddr, _Out_ DWORD& dwBPType, _Out_ DWORD& dwIndex);
	//****************************************************
	// Name: CInterface::RepairHardBP
	// Func: ˼·��,ֻҪDR0~DR3��Ϊ0, ��ȥ�޸�L0~L3(����ɾ��Ӳ���ϵ�ʱ,Ҫ��DR0~DR3�ÿ�)
	// Args: DWORD dwTID
	// Retn: void
	//****************************************************
	static void RepairHardBP(DWORD dwTID);





















public:
	CInterface();
	~CInterface();
};

