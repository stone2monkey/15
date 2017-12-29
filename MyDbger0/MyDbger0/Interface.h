#pragma once
class CInterface
{
	// Attributions ***************************************************************************************
public:
	//****************************************************
	// Name: CInterface::SelectExeDbged
	// Func: 初始化g_pProcDbgedInfo中字符串信息
	// Retn: void
	//****************************************************
	static void SelectExeDbged();

	// 打开一个足够权限句柄,修改内存属性
	static void OpenProcAndAlterPage(_Out_ HANDLE& hProc, _In_ DWORD dwPID, DWORD dwAddrAltered, DWORD dwSize, _Out_ DWORD& dwOldProtect);
	// 恢复内存属性,关闭句柄
	static void ResumePageAndClsHandle(_In_ HANDLE hProc, DWORD dwAddrAltered, DWORD dwSize, _In_ DWORD dwNewProtect);


	//****************************************************
	// Name: CInterface::BinaryToWChar
	// Func: 把二进制数据字面值,转为字符串
	// Args: BYTE * pBinary
	// Args: int iSize
	// Args: CStringW & wcsBinaryString
	// Retn: void
	//****************************************************
	static void BinaryToWChar(BYTE* pBinary, int iSize, CStringW& wcsBinaryString);

	//****************************************************
	// Name: CInterface::DisasmListPageChanged
	// Func: 根据地址得到列表的37项,选择属性是谁的
	// Args: DWORD dwAddr
	// Args: int iSelection
	// Retn: void
	//****************************************************
	static void DisasmListPageChanged(DWORD dwAddr, int iSelection);

	//****************************************************
	// Name: CInterface::IsHardBP
	// Func: 通过地址查看是不是硬件断点&TF,如果是硬件断点返回地址,类型,位置
	// Args: DWORD dwTID
	// Args: DWORD dwAddr
	// Retn: int
	// return -1;	没有找到
	// return 0;	找到了TF断点
	// return 1;	找到了硬件断点
	//****************************************************
	static int IsHardBP(DWORD dwTID, _Out_ DWORD& dwAddr, _Out_ DWORD& dwBPType, _Out_ DWORD& dwIndex);
	//****************************************************
	// Name: CInterface::RepairHardBP
	// Func: 思路是,只要DR0~DR3不为0, 就去修复L0~L3(所以删除硬件断点时,要把DR0~DR3置空)
	// Args: DWORD dwTID
	// Retn: void
	//****************************************************
	static void RepairHardBP(DWORD dwTID);





















public:
	CInterface();
	~CInterface();
};

