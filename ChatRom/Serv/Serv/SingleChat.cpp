#include "stdafx.h"
#include "SingleChat.h"
#include "MySocket.h"
#include "ServDlg.h"
#include "_Global.h"
#include "resource.h"


CSingleChat::CSingleChat()
{
}


CSingleChat::~CSingleChat()
{
}

void CSingleChat::RespondSingleChatMsg(char * const & strMsgRecv, _SParamToThread * pParamToThread)
{
	SMsg* pMsg = (SMsg*)strMsgRecv;

	// 单聊关系在客户端做的,这里给二个人发消息就可以了

	// 0. 给自己发消息
	{
		int iLen = sizeof(SMsg) + pMsg->uFollowContentByte;
		pParamToThread->pMySocket->Send(pParamToThread->sockConn, strMsgRecv, iLen);
	}

	// 1. 给对方发消息
	{
		//1-0. 找到对方socket
		UINT uQQOfRecvMsg = pMsg->uSubMsgCode[1];
		SOCKET sockConn{};
		int iLoop = (int)g_vectOnlineQQInfo.size();
		for (int i = 0; i < iLoop; i++)
		{
			if (uQQOfRecvMsg == g_vectOnlineQQInfo.at(i).uQQ)
			{
				sockConn = g_vectOnlineQQInfo.at(i).sockConn;
				break;
			}
		}
		if (0 == sockConn)
		{
#ifdef DEBUG
			assert(0);
#endif
		}

		int iLen = sizeof(SMsg) + pMsg->uFollowContentByte;
		pParamToThread->pMySocket->Send(sockConn, strMsgRecv, iLen);
	}
}
