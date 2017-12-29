#include "stdafx.h"
#include "MyServiceData.h"


CMyServiceData::CMyServiceData()
{
}


CMyServiceData::~CMyServiceData()
{
	if (!m_lpServiceName)
	{
		delete[] m_lpServiceName;
		m_lpServiceName = nullptr;
	}
}
