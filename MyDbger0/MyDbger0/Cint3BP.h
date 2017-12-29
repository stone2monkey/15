#pragma once
#include <vector>
#include "_MyCustomType.h"
using std::vector;


class Cint3BP
{
	// Attributions ***************************************************************************************
public:
	vector<MY_INT3_BP*> m_vctMyInt3BK{};



	// Methods ********************************************************************************************
public:
	void AddBP(DWORD dwAddr);	// 添加int3断点
	void RepairCode(DWORD dwAddr);// 修正代码(回到没有下断时的数据)
	void RepairEip(DWORD dwTID);	// 修正Eip
	void RepairBK();	// 修正int3断点(代码被改成了没有下断时的数据,所以要改回来)
	void RemoveBP(DWORD dwAddr);	// 管理数据上移除int3断点,也就是vct中弹出
	BOOL SeekBP(_In_ DWORD dwAddr, _Out_ DWORD& dwIndex);	// 通过一个地址,找到断点索引,如果找到返回TRUE








public:
	Cint3BP();
	~Cint3BP();
};

