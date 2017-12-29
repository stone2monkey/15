#pragma once
#include <vector>

using std::vector;
class CMyModuleData;
class CMyThreadData;


class CMyProcessData
{
	// Attributions ***************************************************************************************
public:
	WCHAR* m_wcsName{};		// ������
	WCHAR* m_wcsFullName{};	// ȫ������
	UINT m_uPID{};			// ���̵�PID
	vector<CMyModuleData*> m_vctMyModuleData{};		// �ý��̵�����ģ��
	vector<CMyThreadData*> m_vctMyThreadData{};		// �ý��̵������߳�






	// Methods ********************************************************************************************
public:
	CMyProcessData();
	~CMyProcessData();
};

