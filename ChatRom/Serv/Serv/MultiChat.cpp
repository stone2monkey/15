#include "stdafx.h"
#include "MultiChat.h"
#include "MySocket.h"
#include "ServDlg.h"
#include "_Global.h"
#include "resource.h"


CMultiChat::CMultiChat()
{
}


CMultiChat::~CMultiChat()
{
}

void CMultiChat::RespondMultiChatMsg(char * const & strMsgRecv, _SParamToThread * pParamToThread)
{
	SMsg* pMsg = (SMsg*)strMsgRecv;

	// 转发给所有在线的QQ就行了
	int iLen = sizeof(SMsg) + pMsg->uFollowContentByte;
	int iLoop = (int)g_vectOnlineQQInfo.size();
	for (int i = 0; i < iLoop; i++)
	{
		pParamToThread->pMySocket->Send(g_vectOnlineQQInfo.at(i).sockConn, strMsgRecv, iLen);
	}
}
