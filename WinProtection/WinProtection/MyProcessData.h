#pragma once
#include <vector>

using std::vector;
class CMyModuleData;
class CMyThreadData;


class CMyProcessData
{
	// Attributions ***************************************************************************************
public:
	WCHAR* m_wcsName{};		// 进程名
	WCHAR* m_wcsFullName{};	// 全进程名
	UINT m_uPID{};			// 进程的PID
	vector<CMyModuleData*> m_vctMyModuleData{};		// 该进程的所有模块
	vector<CMyThreadData*> m_vctMyThreadData{};		// 该进程的所有线程






	// Methods ********************************************************************************************
public:
	CMyProcessData();
	~CMyProcessData();
};

