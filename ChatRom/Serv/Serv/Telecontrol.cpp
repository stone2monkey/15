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

	// 0. �������ݵ��ڴ�
	pParamToThread->pServDlg->UpdateData();

	// 1. ȡ����
	CString& wstrTelecontrol = pParamToThread->pServDlg->m_wstrTelecontrol;
	UINT& uTelecontrolQQ = pParamToThread->pServDlg->m_uTelecontrolQQ;

	// 2. �ж�����
	// ��Ч��QQ
	if (0 == uTelecontrolQQ) { pParamToThread->pServDlg->MessageBoxW(L"��Ч��QQ!"); return; }
	// QQû������
	BOOL bIsOnline = FALSE;
	int iLoop = (int)g_vectOnlineQQInfo.size();
	for (int i = 0; i < iLoop; i++)
	{
		if (uTelecontrolQQ == g_vectOnlineQQInfo.at(i).uQQ) { bIsOnline = TRUE; break; }
	}
	if (FALSE == bIsOnline) { pParamToThread->pServDlg->MessageBoxW(L"�����ߵ�QQ,��ôȥԶ��!"); return; }
	// ��Ч��Զ�̴���
	if (L"" == wstrTelecontrol) { pParamToThread->pServDlg->MessageBoxW(L"��Ч��Զ�̴���!"); return; }

	// 3. �ṹ��ϢͷSMsg
	int iLen = sizeof(SMsg) + wstrTelecontrol.GetLength() * 2 + 2;
	SMsg* pMsg = (SMsg*)new BYTE[iLen];
	pMsg->uMsgCode = S_TELECONTROL_MSGCODE;
	pMsg->uFollowContentByte = wstrTelecontrol.GetLength() * 2 + 2;
	::memcpy(pMsg + 1, wstrTelecontrol.GetBuffer(), pMsg->uFollowContentByte);

	// 4. ����
	// 4-0. �ҵ�Ҫ����QQ��socket
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
	// 4-1. ����
	pParamToThread->pMySocket->Send(sockConn, (char*)pMsg, iLen);

	// 5. ��β
	pParamToThread->pServDlg->SetDlgItemTextW(IDEDT_WSTRTELECONTROL, L"");
	delete[](BYTE*)pMsg;
}
