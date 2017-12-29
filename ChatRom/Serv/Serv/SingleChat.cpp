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

	// ���Ĺ�ϵ�ڿͻ�������,����������˷���Ϣ�Ϳ�����

	// 0. ���Լ�����Ϣ
	{
		int iLen = sizeof(SMsg) + pMsg->uFollowContentByte;
		pParamToThread->pMySocket->Send(pParamToThread->sockConn, strMsgRecv, iLen);
	}

	// 1. ���Է�����Ϣ
	{
		//1-0. �ҵ��Է�socket
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
