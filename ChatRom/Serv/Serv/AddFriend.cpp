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
	
	// 0. �Ƿ��ں����б���	
	// �ͻ����Ѿ��ж���

	// 1. �Ƿ����Ѿ�ע���QQ
	BOOL bIsQQSignupped = FALSE;
	{
		int iLoop = (int)g_vectQQSinguppedInfo.size();
		for (int i = 0; i < iLoop; i++)
		{
			if (uQQAdded == g_vectQQSinguppedInfo.at(i).uQQ) { bIsQQSignupped = TRUE; break; }
		}
	}

	// 1. �Ƿ�����			
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

	// 2. ������Ϣ
	pMsg->uSubMsgCode[2] = bIsQQSignupped;
	pMsg->uSubMsgCode[3] = bIsQQOnline;
	pParamToThread->pMySocket->Send(pParamToThread->sockConn, (char*)pMsg, sizeof(SMsg));

	// 3. ����������,ѹ�뵽��QQ�����б���
	if (bIsQQSignupped)
	{
		int iLoop = (int)g_vectQQSinguppedInfo.size();
		for (int i = 0; i < iLoop; i++)
		{
			if (uQQ == g_vectQQSinguppedInfo.at(i).uQQ)
			{
				g_vectQQSinguppedInfo.at(i).vectFriends.push_back(uQQAdded);    

                // 4. д�������ݿ�
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

	// 5. ��Ϊ��˫����Ӻ��� , ����Ҫ������ӷ�������Ӻ�����Ϣ��
	{
		// 0. ��ӷ�QQѹ�뵽����ӷ���QQ�����б���
		int iLoop = (int)g_vectQQSinguppedInfo.size();
		for (int i = 0; i < iLoop; i++)
		{
			if (uQQAdded == g_vectQQSinguppedInfo.at(i).uQQ)
			{
				g_vectQQSinguppedInfo.at(i).vectFriends.push_back(pMsg->uSubMsgCode[0]);

                // д�������ݿ�
                int iCountFreinds = g_vectQQSinguppedInfo.at(i).vectFriends.size();
                CDataBaseCtrl oDataBaseCtrl;
                oDataBaseCtrl.LinkDataBase("0", "0", "0", "0");
                char str[0x400]{};
                sprintf_s(str, sizeof(str), "update t_qqsignupped set ufq%d = %d where uQQ = %d;", iCountFreinds - 1, uQQ, uQQAdded);
                oDataBaseCtrl.ExecSqlByConnect(str);

				break;
			}
		}

		// 1. �������ӷ�������,return;
		if (!bIsQQOnline) { return; }

		// 2. ���SMsg
		// ��ӷ��뱻��ӷ���λ
		pMsg->uSubMsgCode[0] = uQQAdded;
		pMsg->uSubMsgCode[1] = uQQ;
		//
		pMsg->uSubMsgCode[2] = 1;		// ע��Ϊ��
		pMsg->uSubMsgCode[3] = 1;		// ����Ϊ��

		// 3. ������Ϣ
		SOCKET sockConn = g_vectOnlineQQInfo.at(iPosQQAdded).sockConn;
		pParamToThread->pMySocket->Send(sockConn, (char*)pMsg, sizeof(SMsg));
	}
}
