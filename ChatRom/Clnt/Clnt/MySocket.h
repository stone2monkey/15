#pragma once
#include <WinSock2.h>

#pragma comment (lib,"ws2_32.lib")
#define BACKLOG 0xff

class CMySocket
{
public:
	CMySocket();
	~CMySocket();

	// Methods ***********************************************************
	public:
		int Startup(LPCWSTR& wstrIP, UINT uPort, BOOL bServ = true);
		void ClosAll();
		SOCKET& Accept();
		int Recv(SOCKET sockConn, char*const& strMsgRecv, int iLen);
		int Send(SOCKET sockConn, char*const& strMsgSent, int iLen);
	
	protected:
	
	private: 
	
	
	// Attributions ******************************************************
	public:
		SOCKET m_sockLsn = {};			// listen socket
		SOCKET m_sockConn = {};			// connect socket (Server is the socket of ::accept() returned , client is the socket of ::socket() returned!)
		SOCKADDR_IN m_addrServ = {};	// server sockaddr 
		SOCKADDR_IN m_addrClnt = {};	// client sockaddr 
	
	protected:
	
	private:
	
	
	
};

