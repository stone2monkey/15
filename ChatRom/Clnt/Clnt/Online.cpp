#include "stdafx.h"
#include "Online.h"
#include "_Global.h"
#include "ClntDlg.h"
#include "MySocket.h"
#include "resource.h"


COnline::COnline()
{
}


COnline::~COnline()
{
}

void COnline::RespondOnlineMsg(char * const & strMsgRecv, _SParamToThread * pParamToThread)
{
	SMsg* pMsg = (SMsg*)strMsgRecv;

	// 0. 如果是自己return;
	{
		if (0 == g_uQQ) { return; }		// 还没有登陆
		if (pMsg->uSubMsgCode[0] == g_uQQ) { return;	}	//是自己上线了
	}

	// 1. 是不是自己好友
	{
		BOOL bIsMyFriend = FALSE;
		int iLoop = g_vectFriends.size();
		for (int i = 0; i < iLoop; i++)
		{
			if (pMsg->uSubMsgCode[0] == g_vectFriends.at(i)) { bIsMyFriend = TRUE; break;}
		}

		if (FALSE == bIsMyFriend) { return; }
	}

	// 2. 是我的好友, 看是否在在线列表中,如果不在,压入
	{
		BOOL bIsInOnlineFriends = FALSE;
		int iLoop = g_vectOnlineFriends.size();
		for (int i = 0; i < iLoop; i++)
		{
			if (pMsg->uSubMsgCode[0] == g_vectOnlineFriends.at(i)) { bIsInOnlineFriends = TRUE; break; }
		}

		if (FALSE == bIsInOnlineFriends) 
		{ 
			g_vectOnlineFriends.push_back(pMsg->uSubMsgCode[0]); 
			pParamToThread->pClntDlg->UpdateOnlineQQListCtrl();
		}
		//else { assert(0); }		// 上线只发送一次,所以如果发送来上线消息时,这个QQ在你的在线列表中,是有问题的
	}
	
}
