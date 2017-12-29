#include "stdafx.h"
#include "Offline.h"
#include "MySocket.h"
#include "ServDlg.h"
#include "_Global.h"
#include "resource.h"


COffline::COffline()
{
}


COffline::~COffline()
{
}

void COffline::SendOfflineMsg(_SParamToThread * pParamToThread)
{
	// 0. 填充下线结构
	// uSubMsgCode[0] = QQ;
	SMsg sMsg;
	sMsg.uMsgCode = S_OFFLINE_MSGCODE;
	sMsg.uSubMsgCode[0] = pParamToThread->uQQ;

	// 1. 发送给所有在线的QQ
	int iLoop = (int)g_vectOnlineQQInfo.size();
	for (int i = 0; i < iLoop; i++)
	{
		pParamToThread->pMySocket->Send(g_vectOnlineQQInfo.at(i).sockConn, (char*)&sMsg, sizeof(SMsg));
	}
}
