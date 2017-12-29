#include "stdafx.h"
#include "AddFriend.h"
#include "MySocket.h"
#include "ServDlg.h"
#include "_Global.h"
#include "resource.h"


CAddFriend::CAddFriend()
{
}


CAddFriend::~CAddFriend()
{
}

void CAddFriend::RespondAddFriendMsg(char*const& strMsgRecv, _SParamToThread * pParamToThread)
{
	SMsg* pMsg = (SMsg*)strMsgRecv;
	UINT uQQ = pMsg->uSubMsgCode[0];
	UINT uQQAdded = pMsg->uSubMsgCode[1];
	
	// 0. 是否在好友列表中	
	// 客户端已经判断了

	// 1. 是否是已经注册的QQ
	BOOL bIsQQSignupped = FALSE;
	{
		int iLoop = (int)g_vectQQSinguppedInfo.size();
		for (int i = 0; i < iLoop; i++)
		{
			if (uQQAdded == g_vectQQSinguppedInfo.at(i).uQQ) { bIsQQSignupped = TRUE; break; }
		}
	}

	// 1. 是否在线			
	BOOL bIsQQOnline = FALSE;
	int iPosQQAdded = -1;
	{
		int iLoop = (int)g_vectOnlineQQInfo.size();
		for (int i = 0; i < iLoop; i++)
		{
			if (uQQAdded == g_vectOnlineQQInfo.at(i).uQQ) 
			{ 
				bIsQQOnline = TRUE; 
				iPosQQAdded = i;
				break; 
			}
		}

	}

	// 2. 发送消息
	pMsg->uSubMsgCode[2] = bIsQQSignupped;
	pMsg->uSubMsgCode[3] = bIsQQOnline;
	pParamToThread->pMySocket->Send(pParamToThread->sockConn, (char*)pMsg, sizeof(SMsg));

	// 3. 如果可以添加,压入到该QQ好友列表下
	if (bIsQQSignupped)
	{
		int iLoop = (int)g_vectQQSinguppedInfo.size();
		for (int i = 0; i < iLoop; i++)
		{
			if (uQQ == g_vectQQSinguppedInfo.at(i).uQQ)
			{
				g_vectQQSinguppedInfo.at(i).vectFriends.push_back(uQQAdded);    

                // 4. 写出到数据库
                int iCountFreinds = g_vectQQSinguppedInfo.at(i).vectFriends.size();
                CDataBaseCtrl oDataBaseCtrl;
                oDataBaseCtrl.LinkDataBase("0", "0", "0", "0");
                char str[0x400]{};
                sprintf_s(str, sizeof(str), "update t_qqsignupped set ufq%d = %d where uQQ = %d;", iCountFreinds - 1, uQQAdded, uQQ);
                oDataBaseCtrl.ExecSqlByConnect(str);

                break;
			}
		}
	}	

	// 5. 因为是双向添加好友 , 现在要给被添加方发送添加好友消息了
	{
		// 0. 添加方QQ压入到被添加方的QQ好友列表中
		int iLoop = (int)g_vectQQSinguppedInfo.size();
		for (int i = 0; i < iLoop; i++)
		{
			if (uQQAdded == g_vectQQSinguppedInfo.at(i).uQQ)
			{
				g_vectQQSinguppedInfo.at(i).vectFriends.push_back(pMsg->uSubMsgCode[0]);

                // 写出到数据库
                int iCountFreinds = g_vectQQSinguppedInfo.at(i).vectFriends.size();
                CDataBaseCtrl oDataBaseCtrl;
                oDataBaseCtrl.LinkDataBase("0", "0", "0", "0");
                char str[0x400]{};
                sprintf_s(str, sizeof(str), "update t_qqsignupped set ufq%d = %d where uQQ = %d;", iCountFreinds - 1, uQQ, uQQAdded);
                oDataBaseCtrl.ExecSqlByConnect(str);

				break;
			}
		}

		// 1. 如果被添加方不在线,return;
		if (!bIsQQOnline) { return; }

		// 2. 填充SMsg
		// 添加方与被添加方换位
		pMsg->uSubMsgCode[0] = uQQAdded;
		pMsg->uSubMsgCode[1] = uQQ;
		//
		pMsg->uSubMsgCode[2] = 1;		// 注册为真
		pMsg->uSubMsgCode[3] = 1;		// 在线为真

		// 3. 发送消息
		SOCKET sockConn = g_vectOnlineQQInfo.at(iPosQQAdded).sockConn;
		pParamToThread->pMySocket->Send(sockConn, (char*)pMsg, sizeof(SMsg));
	}
}
