#include "stdafx.h"
#include "MyProcessData.h"
#include "MyModuleData.h"
#include "MyThreadData.h"


CMyProcessData::CMyProcessData()
{
	WCHAR* wcsTemp = new WCHAR[MAX_PATH * 2]{};
	m_wcsName = wcsTemp;
	m_wcsFullName = m_wcsName + MAX_PATH;
}


CMyProcessData::~CMyProcessData()
{
	if (m_wcsName)
	{
		delete[] m_wcsName;
		m_wcsName = nullptr;
		m_wcsFullName = nullptr;
	}

	int iLoop = (int)m_vctMyThreadData.size();
	for (int i = iLoop-1; i >= 0; i++)
	{
		delete m_vctMyThreadData.at(i);
		m_vctMyThreadData.pop_back();
	}

	iLoop = (int)m_vctMyThreadData.size();
	for (int i = iLoop - 1; i >= 0; i++)
	{
		delete m_vctMyThreadData.at(i);
		m_vctMyThreadData.pop_back();
	}
}
