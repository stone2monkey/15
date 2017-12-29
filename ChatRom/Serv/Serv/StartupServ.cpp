#include "stdafx.h"
#include "StartupServ.h"
#include "MySocket.h"


CStartupServ::CStartupServ(CString wstrIP, UINT uPort, CMySocket & rMySocket) : m_rMySocket(rMySocket)
{
	m_wstrIP = wstrIP;
	m_uPort = uPort;
}

CStartupServ::~CStartupServ()
{
}

int CStartupServ::Startup()
{
	LPCWSTR wstrIP = m_wstrIP.GetBuffer();
	return m_rMySocket.Startup(wstrIP, m_uPort);
}
