#include "stdafx.h"
#include "ConnServ.h"
#include "MySocket.h"


CConnServ::CConnServ(CString wstrIP, UINT uPort, CMySocket & rMySocket) : m_rMySocket(rMySocket)
{
	m_wstrIP = wstrIP;
	m_uPort = uPort;
}


CConnServ::~CConnServ()
{
}

int CConnServ::ConnServ()
{
	LPCWSTR wstrIP = m_wstrIP.GetBuffer();
	return m_rMySocket.Startup(wstrIP, m_uPort, 0);
}
