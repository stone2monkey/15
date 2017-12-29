#include "stdafx.h"
#include "AddFriend.h"
#include "_Global.h"
#include "ClntDlg.h"
#include "MySocket.h"
#include "resource.h"


CAddFriend::CAddFriend()
{
}


CAddFriend::~CAddFriend()
{
}

void CAddFriend::SendAddFriendMsg(UINT uQQ, _SParamToThread* pParamToThread)
{
	// 0. 填充注册结构
	/*
	uSubMsgCode[0] = QQ			我的QQ
	uSubMsgCode[1]				要添加的QQ
	*/
	
	SMsg sMsg;
	sMsg.uMsgCode = C_ADDFRIEND_MSGCODE;
	sMsg.uSubMsgCode[0] = g_uQQ;
	sMsg.uSubMsgCode[1] = uQQ;

	// 1. 发送
	pParamToThread->pMySocket->Send(pParamToThread->pMySocket->m_sockConn, (char*)&sMsg, sizeof(SMsg));

}

void CAddFriend::RespondAddFriendMsg(char * const & strMsgRecv, _SParamToThread * pParamToThread)
{
	SMsg* pMsg = (SMsg*)strMsgRecv;
	UINT uQQ = pMsg->uSubMsgCode[0];
	UINT uQQAdded = pMsg->uSubMsgCode[1];

	// 是否可以添加
	if (TRUE == pMsg->uSubMsgCode[2])
	{
		g_vectFriends.push_back(uQQAdded);
        pParamToThread->pClntDlg->UpdateOnlineQQListCtrl();
		pParamToThread->pClntDlg->MessageBoxW(L"添加好友成功!");
	}
	else
	{
		pParamToThread->pClntDlg->MessageBoxW(L"添加好友失败,该QQ未注册!");
		return;
	}

	// 是否在线
	if (TRUE == pMsg->uSubMsgCode[3])
	{
		g_vectOnlineFriends.push_back(uQQAdded);
		pParamToThread->pClntDlg->UpdateOnlineQQListCtrl();
	}
}
