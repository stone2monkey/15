#include "stdafx.h"
#include "SingleChat.h"
#include "_Global.h"
#include "ClntDlg.h"
#include "MySocket.h"
#include "resource.h"


CSingleChat::CSingleChat()
{
}


CSingleChat::~CSingleChat()
{
}

void CSingleChat::SendSingleChatMsg(_SParamToThread * pParamToThread)
{
	// 0. 取得数据
	// 文本消息
	CString wstrMsgSent;
	pParamToThread->pClntDlg->GetDlgItemTextW(IDEDT_MSGSENT, wstrMsgSent);
	if (L"" == wstrMsgSent) { return; }
	// 接收方QQ
	{
		pParamToThread->pClntDlg->UpdateData();
		UINT uQQOfRecvMsg = pParamToThread->pClntDlg->m_uQQOfRecvMsg;
		// QQ不对
		if (0 == uQQOfRecvMsg) { return; }
		// 在线好友列表中没有该QQ
		BOOL bIsInOnlineFriends = FALSE;
		int iLoop = g_vectOnlineFriends.size();
		for (int i = 0; i < iLoop; i++)
		{
			if (uQQOfRecvMsg == g_vectOnlineFriends.at(i))
			{
				bIsInOnlineFriends = TRUE;
				break;
			}
		}
		if (FALSE == bIsInOnlineFriends)
		{
			pParamToThread->pClntDlg->MessageBoxW(L"接收消息的QQ好友不在线or不正确!");
			return;

		}
	}

	// 1. 要发送的数据
	CString wstrTime;
	FillCurrentTime(wstrTime);
	CString wstrAddr;
	FillAddrClnt(pParamToThread->pMySocket->m_addrClnt, wstrAddr);
	CString wstrQQ;
	FillQQ(g_uQQ, wstrQQ);
	CString temp = L"\r\n";
	temp += wstrTime + wstrAddr + wstrQQ;
	temp += L"--> ";
	FillQQ(pParamToThread->pClntDlg->m_uQQOfRecvMsg, wstrQQ);
	temp += wstrQQ;
	temp += L"\r\n";
	temp += wstrMsgSent;
	wstrMsgSent = temp + L"\r\n";
	LPCWSTR wstrMsgSent0 = wstrMsgSent.GetBuffer();

	// 3. 数据头SMsg
	/*
	uSubMsgCode[0]		发送方QQ
	uSubMsgCode[1]		接收方QQ
	*/
	int iLen = sizeof(SMsg) + wstrMsgSent.GetLength() * 2 + 2;
	SMsg* pMsg = (SMsg*)new BYTE[iLen];
	pMsg->uMsgCode = C_SINGLECHAT_MSGCODE;
	pMsg->uSubMsgCode[0] = g_uQQ;
	pMsg->uSubMsgCode[1] = pParamToThread->pClntDlg->m_uQQOfRecvMsg;
	pMsg->uFollowContentByte = wstrMsgSent.GetLength() * 2 + 2;
	::memcpy(pMsg + 1, wstrMsgSent0, wstrMsgSent.GetLength() * 2 + 2);

	// 4. 发送
	pParamToThread->pMySocket->Send(pParamToThread->pMySocket->m_sockConn, (char*)pMsg, iLen);

	// 5. 发送消息的编辑框数据置空
	pParamToThread->pClntDlg->m_wstrMsgSent = "";
	pParamToThread->pClntDlg->SetDlgItemTextW(IDEDT_MSGSENT, pParamToThread->pClntDlg->m_wstrMsgSent);

	// 6. 
	delete[](BYTE*)pMsg;
}

void CSingleChat::RespondSingleChatMsg(char * const & strMsgRecv, _SParamToThread * pParamToThread)
{
	SMsg* pMsg = (SMsg*)strMsgRecv;

	// 0. 如果我是发送方
	{
		if (g_uQQ == pMsg->uSubMsgCode[0])
		{
			// 0-0. 得到跟随的数据
			LPCWSTR wstrFollowContentByte = (LPCWSTR)(pMsg + 1);
			
			// 0-1. 添加提醒
			CString wstr = L">>>>>单聊发送成功!<<<<<<";
			wstr += wstrFollowContentByte;
			wstr += L"********************************************************\r\n";

			// 0-2. 加入到显示消息中
			pParamToThread->pClntDlg->m_wstrShowMsg += wstr;

			// 0-3. 更新数据到显示框
			pParamToThread->pClntDlg->SetDlgItemTextW(IDEDT_SHOWMSG, pParamToThread->pClntDlg->m_wstrShowMsg);
		}
	}

	// 1. 如果我是接收方
	if (g_uQQ == pMsg->uSubMsgCode[1])
	{
		// 0-0. 得到跟随的数据
		LPCWSTR wstrFollowContentByte = (LPCWSTR)(pMsg + 1);

		// 0-1. 添加提醒
		CString wstr = L">>>>>接收到单聊!<<<<<<";
		wstr += wstrFollowContentByte;
		wstr += L"********************************************************\r\n";

		// 0-2. 加入到显示消息中
		pParamToThread->pClntDlg->m_wstrShowMsg += wstr;

		// 0-3. 更新数据到显示框
		pParamToThread->pClntDlg->SetDlgItemTextW(IDEDT_SHOWMSG, pParamToThread->pClntDlg->m_wstrShowMsg);
	}
}
