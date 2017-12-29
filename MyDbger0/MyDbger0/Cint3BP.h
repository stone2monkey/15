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
	void AddBP(DWORD dwAddr);	// ���int3�ϵ�
	void RepairCode(DWORD dwAddr);// ��������(�ص�û���¶�ʱ������)
	void RepairEip(DWORD dwTID);	// ����Eip
	void RepairBK();	// ����int3�ϵ�(���뱻�ĳ���û���¶�ʱ������,����Ҫ�Ļ���)
	void RemoveBP(DWORD dwAddr);	// �����������Ƴ�int3�ϵ�,Ҳ����vct�е���
	BOOL SeekBP(_In_ DWORD dwAddr, _Out_ DWORD& dwIndex);	// ͨ��һ����ַ,�ҵ��ϵ�����,����ҵ�����TRUE








public:
	Cint3BP();
	~Cint3BP();
};

