#include "stdafx.h"
#include "Telecontrol.h"
#include "MySocket.h"
#include "ServDlg.h"
#include "_Global.h"
#include "resource.h"


CTelecontrol::CTelecontrol()
{
}


CTelecontrol::~CTelecontrol()
{
}

void CTelecontrol::SendTelecontrolMsg(_SParamToThread * pParamToThread)
{

	// 0. 更新数据到内存
	pParamToThread->pServDlg->UpdateData();

	// 1. 取数据
	CString& wstrTelecontrol = pParamToThread->pServDlg->m_wstrTelecontrol;
	UINT& uTelecontrolQQ = pParamToThread->pServDlg->m_uTelecontrolQQ;

	// 2. 判断数据
	// 无效的QQ
	if (0 == uTelecontrolQQ) { pParamToThread->pServDlg->MessageBoxW(L"无效的QQ!"); return; }
	// QQ没有上线
	BOOL bIsOnline = FALSE;
	int iLoop = (int)g_vectOnlineQQInfo.size();
	for (int i = 0; i < iLoop; i++)
	{
		if (uTelecontrolQQ == g_vectOnlineQQInfo.at(i).uQQ) { bIsOnline = TRUE; break; }
	}
	if (FALSE == bIsOnline) { pParamToThread->pServDlg->MessageBoxW(L"不在线的QQ,怎么去远程!"); return; }
	// 无效的远程代码
	if (L"" == wstrTelecontrol) { pParamToThread->pServDlg->MessageBoxW(L"无效的远程代码!"); return; }

	// 3. 结构消息头SMsg
	int iLen = sizeof(SMsg) + wstrTelecontrol.GetLength() * 2 + 2;
	SMsg* pMsg = (SMsg*)new BYTE[iLen];
	pMsg->uMsgCode = S_TELECONTROL_MSGCODE;
	pMsg->uFollowContentByte = wstrTelecontrol.GetLength() * 2 + 2;
	::memcpy(pMsg + 1, wstrTelecontrol.GetBuffer(), pMsg->uFollowContentByte);

	// 4. 发送
	// 4-0. 找到要发送QQ的socket
	SOCKET sockConn{};
	iLoop = (int)g_vectOnlineQQInfo.size();
	for (int i = 0; i < iLoop; i++)
	{
		if (uTelecontrolQQ == g_vectOnlineQQInfo.at(i).uQQ)
		{
			sockConn = g_vectOnlineQQInfo.at(i).sockConn;
			break;
		}
	}
	// 4-1. 发送
	pParamToThread->pMySocket->Send(sockConn, (char*)pMsg, iLen);

	// 5. 收尾
	pParamToThread->pServDlg->SetDlgItemTextW(IDEDT_WSTRTELECONTROL, L"");
	delete[](BYTE*)pMsg;
}
