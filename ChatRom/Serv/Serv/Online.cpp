#include "stdafx.h"
#include "Online.h"
#include "MySocket.h"
#include "ServDlg.h"
#include "_Global.h"
#include "resource.h"


COnline::COnline()
{
}


COnline::~COnline()
{
}

void COnline::SendOnlineMsg(_SParamToThread * pParamToThread)
{
	// 0. ������߽ṹ
	// uSubMsgCode[0] = QQ;
	SMsg sMsg;
	sMsg.uMsgCode = S_ONLINE_MSGCODE;
	sMsg.uSubMsgCode[0] = pParamToThread->uQQ;

	// 1. ���͸��������ߵ�QQ
	int iLoop = (int)g_vectOnlineQQInfo.size();
	for (int i = 0; i < iLoop; i++)
	{
		pParamToThread->pMySocket->Send(g_vectOnlineQQInfo.at(i).sockConn, (char*)&sMsg, sizeof(SMsg));
	}
}
