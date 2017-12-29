#include "stdafx.h"
#include "MySocket.h"
#include <ws2tcpip.h>


CMySocket::CMySocket()
{
}


CMySocket::~CMySocket()
{
}


//************************************
// Method:    CMySocket::Startup 
// Returns:   int		// 0-没有错误发生 1~7-看源码
// Parameter: LPCWSTR & wstrIP	
// Parameter: UINT uPort
// Parameter: BOOL bServ
//************************************
int CMySocket::Startup(LPCWSTR& wstrIP, UINT uPort, BOOL bServ)
{
	// 0. Serv
	while (bServ)
	{
		// 0-0. whether startupped
		if (0 != m_sockLsn) { return 1; }		// already startup

		// 0-1. set m_addrServ m_addrClnt
		m_addrServ.sin_family = AF_INET;
		InetPtonW(AF_INET, wstrIP, &m_addrServ.sin_addr);
		m_addrServ.sin_port = ::htons(uPort);

		m_addrClnt.sin_family = AF_INET;

		// 0-2. ::WSAStartup
		WSADATA wsaDate;
		int iRes = ::WSAStartup(MAKEWORD(2, 2), &wsaDate);
		if (0 != iRes) { return 2; }		

		// 0-3. ::socket
		m_sockLsn = ::socket(AF_INET, SOCK_STREAM, 0);
		if (INVALID_SOCKET == m_sockLsn) { return 3; }

		// 0-4. ::bind
		iRes = ::bind(m_sockLsn, (SOCKADDR*)&m_addrServ, sizeof(SOCKADDR));
		if (0 != iRes) { return 4; }

		// 0-5. ::listen
		iRes = ::listen(m_sockLsn, BACKLOG);
		if (0 != iRes) { return 5; }

		return 0;	// success
	}

	// 1. Clnt
	while (false == bServ)
	{ 
		// 1-0. whether startupped
		if (0 != m_sockConn) { return 1; }		// already startup

		// 1-1. set m_addrServ
		m_addrServ.sin_family = AF_INET;
		InetPtonW(AF_INET, wstrIP, &m_addrServ.sin_addr);
		m_addrServ.sin_port = ::htons(uPort);

		// 1-2. ::WSAStartup
		WSADATA wsaDate;
		int iRes = ::WSAStartup(MAKEWORD(2, 2), &wsaDate);
		if (0 != iRes) { return 2; }

		// 1-3. ::socket
		m_sockConn = ::socket(AF_INET, SOCK_STREAM, 0);
		if (INVALID_SOCKET == m_sockConn) { return 3; }

		// 1-4. ::connect
		iRes = ::connect(m_sockConn, (SOCKADDR*)&m_addrServ, sizeof(SOCKADDR));
		if (0 != iRes) { return 6; }

		// 1-5. 填充m_addrClnt
		int iLen = sizeof(SOCKADDR);
		::getsockname(m_sockConn, (SOCKADDR*)&m_addrClnt, &iLen);

		return 0;	// success
	}

	// 2. check
	if (bServ) { if (0 == m_sockLsn || INVALID_SOCKET == m_sockLsn) { return 7; } }	//unknow error
	else { if (0 == m_sockConn || INVALID_SOCKET == m_sockConn) { return 7; } }			//unknow error

	return 0;		
}

void CMySocket::ClosAll()
{
	if (0 != m_sockLsn) { ::closesocket(m_sockLsn); }
	if (0 != m_sockConn) { ::closesocket(m_sockConn); }
	::WSACleanup();
}

SOCKET& CMySocket::Accept()
{
	int iLen = sizeof(SOCKADDR);
	m_sockConn = ::accept(m_sockLsn, (SOCKADDR*)&m_addrClnt, &iLen);
	return m_sockConn;
}

int CMySocket::Recv(SOCKET sockConn, char * const & strMsgRecv, int iLen)
{	
	int iRes = ::recv(sockConn, strMsgRecv, iLen, 0);

	// 解密数据
	CEncode oEncode;
	oEncode.Decode(strMsgRecv, iLen);

	return iRes;
}

int CMySocket::Send(SOCKET sockConn, char * const & strMsgSent, int iLen)
{
	// 数据用副本吧,因为外面有用原本,被加密数据就全错了
	char* str = new char[iLen];
	::memcpy(str, strMsgSent, iLen);

	// 加密数据
	CEncode oEncode;
	oEncode.Encode(str, iLen);

	int iRes = ::send(sockConn, str, iLen, 0);

	delete[] str;

	return iRes;
}
