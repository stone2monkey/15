#include "stdafx.h"
#include "MyWindowData.h"


CMyWindowData::CMyWindowData()
{
}


CMyWindowData::~CMyWindowData()
{
	if (!m_wcsTitleName)
	{
		delete[] m_wcsTitleName;
		m_wcsTitleName = nullptr;
	}

	if (!m_wcsClassName)
	{
		delete[] m_wcsClassName;
		m_wcsClassName = nullptr;
	}
}
