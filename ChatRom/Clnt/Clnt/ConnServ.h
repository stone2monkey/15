#pragma once

class CMySocket;

class CConnServ
{
public:
	CConnServ(CString wstrIP, UINT uPort, CMySocket& rMySocket);
	~CConnServ();




// Methods ***********************************************************
public:
	int ConnServ();

// Attributions ******************************************************
public:
	CString m_wstrIP{};
	UINT m_uPort{};
	CMySocket& m_rMySocket;
};

