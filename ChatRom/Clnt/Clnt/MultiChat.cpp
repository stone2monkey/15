#include "stdafx.h"
#include "MultiChat.h"
#include "_Global.h"
#include "ClntDlg.h"
#include "MySocket.h"
#include "resource.h"


CMultiChat::CMultiChat()
{
}


CMultiChat::~CMultiChat()
{
}

void CMultiChat::SendMultiChatMsg(_SParamToThread * pParamToThread)
{
	// 0. 更新数据到内存
	pParamToThread->pClntDlg->UpdateData();

	// 1. 数据不对,退出
	if (L"" == pParamToThread->pClntDlg->m_wstrMsgSent) { return; }

	// 2. 要发送的数据
	CString wstrTime;
	FillCurrentTime(wstrTime);
	CString wstrAddr;
	FillAddrClnt(pParamToThread->pMySocket->m_addrClnt, wstrAddr);
	CString wstrQQ;
	FillQQ(g_uQQ, wstrQQ);
	CString wstrMsgSent = wstrTime + wstrAddr + wstrQQ + pParamToThread->pClntDlg->m_wstrMsgSent;
	wstrMsgSent += L"\r\n";
	LPCWSTR wstrMsgSent0 = wstrMsgSent.GetBuffer();

	// 3. 数据头SMsg
	int iLen = sizeof(SMsg) + wstrMsgSent.GetLength() * 2 + 2;
	SMsg* pMsg = (SMsg*)new BYTE[iLen];
	pMsg->uMsgCode = C_MUILTCHAT_MSGCODE;
	pMsg->uFollowContentByte = wstrMsgSent.GetLength() * 2 + 2;
	::memcpy(pMsg + 1, wstrMsgSent0, wstrMsgSent.GetLength() * 2 + 2);

	// 4. 发送
	pParamToThread->pMySocket->Send(pParamToThread->pMySocket->m_sockConn, (char*)pMsg, iLen);

	// 5. 发送消息的编辑框数据置空
	pParamToThread->pClntDlg->m_wstrMsgSent = "";
	pParamToThread->pClntDlg->SetDlgItemTextW(IDEDT_MSGSENT, pParamToThread->pClntDlg->m_wstrMsgSent);

	// 6. 
	delete[] (BYTE*)pMsg;
}

void CMultiChat::RespondMultiChatMsg(char * const & strMsgRecv, _SParamToThread * pParamToThread)
{
	SMsg* pMsg = (SMsg*)strMsgRecv;

	// 0. 得到跟随的数据
	LPCWSTR wstrFollowContentByte = (LPCWSTR)(pMsg + 1);

	// 1. 加入到显示消息中
	pParamToThread->pClntDlg->m_wstrShowMsg += wstrFollowContentByte;

	// 2. 更新数据到显示框
	pParamToThread->pClntDlg->SetDlgItemTextW(IDEDT_SHOWMSG, pParamToThread->pClntDlg->m_wstrShowMsg);
}
