#include "stdafx.h"
#include "_Global.h"




// 客户端要用到的数据 *****************************************************************************************************
vector<UINT> g_vectFriends{};				// 好友列表 
vector<UINT> g_vectOnlineFriends{};		// 在线好友列表 
UINT g_uQQ = 0;




































_Global::_Global()
{
}


_Global::~_Global()
{
}



void FillCurrentTime(CString & wstrTime)
{
	CTime oTime = CTime::GetCurrentTime();
	int iHour = oTime.GetHour();
	int iMinute = oTime.GetMinute();
	int iSecond = oTime.GetSecond();

	wstrTime.Format(L"[Time %2d:%2d:%2d] ", iHour, iMinute, iSecond);
}

void FillAddrClnt(SOCKADDR_IN addrClnt, CString& wstrAddr)			//[Addr ***.***.***.***:*****]
{
	int iIP1 = addrClnt.sin_addr.S_un.S_un_b.s_b1;
	int iIP2 = addrClnt.sin_addr.S_un.S_un_b.s_b2;
	int iIP3 = addrClnt.sin_addr.S_un.S_un_b.s_b3;
	int iIP4 = addrClnt.sin_addr.S_un.S_un_b.s_b4;
	WORD wPort = ::ntohs(addrClnt.sin_port);

	wstrAddr.Format(L"[Addr %2d.%2d.%2d.%2d:%6d] ", iIP1, iIP2, iIP3, iIP4, wPort);
}

void FillQQ(UINT uQQ, CString& wstrQQ)
{
	wstrQQ.Format(L"[QQ %8d] ", uQQ);
}