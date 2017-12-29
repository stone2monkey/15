#include "stdafx.h"
#include "Offline.h"
#include "_Global.h"
#include "ClntDlg.h"
#include "MySocket.h"
#include "resource.h"


COffline::COffline()
{
}


COffline::~COffline()
{
}

void COffline::RespondOfflineMsg(char * const & strMsgRecv, _SParamToThread * pParamToThread)
{
	SMsg* pMsg = (SMsg*)strMsgRecv;

	// 如果下线QQ在在线好友列表中,就要推出
	int iLoop = g_vectOnlineFriends.size();
	for (int i = 0; i < iLoop; i++)
	{
		if (pMsg->uSubMsgCode[0] == g_vectOnlineFriends.at(i))
		{
			g_vectOnlineFriends.erase(g_vectOnlineFriends.begin() + i);
			pParamToThread->pClntDlg->UpdateOnlineQQListCtrl();
			return;
		}
	}
}
