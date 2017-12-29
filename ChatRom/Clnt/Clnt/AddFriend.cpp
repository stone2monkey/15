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
	// 0. ���ע��ṹ
	/*
	uSubMsgCode[0] = QQ			�ҵ�QQ
	uSubMsgCode[1]				Ҫ��ӵ�QQ
	*/
	
	SMsg sMsg;
	sMsg.uMsgCode = C_ADDFRIEND_MSGCODE;
	sMsg.uSubMsgCode[0] = g_uQQ;
	sMsg.uSubMsgCode[1] = uQQ;

	// 1. ����
	pParamToThread->pMySocket->Send(pParamToThread->pMySocket->m_sockConn, (char*)&sMsg, sizeof(SMsg));

}

void CAddFriend::RespondAddFriendMsg(char * const & strMsgRecv, _SParamToThread * pParamToThread)
{
	SMsg* pMsg = (SMsg*)strMsgRecv;
	UINT uQQ = pMsg->uSubMsgCode[0];
	UINT uQQAdded = pMsg->uSubMsgCode[1];

	// �Ƿ�������
	if (TRUE == pMsg->uSubMsgCode[2])
	{
		g_vectFriends.push_back(uQQAdded);
        pParamToThread->pClntDlg->UpdateOnlineQQListCtrl();
		pParamToThread->pClntDlg->MessageBoxW(L"��Ӻ��ѳɹ�!");
	}
	else
	{
		pParamToThread->pClntDlg->MessageBoxW(L"��Ӻ���ʧ��,��QQδע��!");
		return;
	}

	// �Ƿ�����
	if (TRUE == pMsg->uSubMsgCode[3])
	{
		g_vectOnlineFriends.push_back(uQQAdded);
		pParamToThread->pClntDlg->UpdateOnlineQQListCtrl();
	}
}
