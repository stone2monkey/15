#include "stdafx.h"
#include "MyModuleData.h"


CMyModuleData::CMyModuleData()
{
	m_wcsName = new WCHAR[MAX_PATH]{};
}


CMyModuleData::~CMyModuleData()
{
	if (m_wcsName)
	{
		delete[] m_wcsName;
		m_wcsName = nullptr;
	}
}
