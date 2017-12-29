#pragma once

class CMySocket;


class CStartupServ
{
public:
	CStartupServ(CString wstrIP, UINT uPort, CMySocket& rMySocket);
	~CStartupServ();



// Methods ***********************************************************
public:
	int Startup();
	
	
	
// Attributions ******************************************************
public:
	CString m_wstrIP{};
	UINT m_uPort{};
	CMySocket& m_rMySocket;
};

