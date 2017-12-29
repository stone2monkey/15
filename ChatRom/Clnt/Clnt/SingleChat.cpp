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
	// 0. ȡ������
	// �ı���Ϣ
	CString wstrMsgSent;
	pParamToThread->pClntDlg->GetDlgItemTextW(IDEDT_MSGSENT, wstrMsgSent);
	if (L"" == wstrMsgSent) { return; }
	// ���շ�QQ
	{
		pParamToThread->pClntDlg->UpdateData();
		UINT uQQOfRecvMsg = pParamToThread->pClntDlg->m_uQQOfRecvMsg;
		// QQ����
		if (0 == uQQOfRecvMsg) { return; }
		// ���ߺ����б���û�и�QQ
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
			pParamToThread->pClntDlg->MessageBoxW(L"������Ϣ��QQ���Ѳ�����or����ȷ!");
			return;

		}
	}

	// 1. Ҫ���͵�����
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

	// 3. ����ͷSMsg
	/*
	uSubMsgCode[0]		���ͷ�QQ
	uSubMsgCode[1]		���շ�QQ
	*/
	int iLen = sizeof(SMsg) + wstrMsgSent.GetLength() * 2 + 2;
	SMsg* pMsg = (SMsg*)new BYTE[iLen];
	pMsg->uMsgCode = C_SINGLECHAT_MSGCODE;
	pMsg->uSubMsgCode[0] = g_uQQ;
	pMsg->uSubMsgCode[1] = pParamToThread->pClntDlg->m_uQQOfRecvMsg;
	pMsg->uFollowContentByte = wstrMsgSent.GetLength() * 2 + 2;
	::memcpy(pMsg + 1, wstrMsgSent0, wstrMsgSent.GetLength() * 2 + 2);

	// 4. ����
	pParamToThread->pMySocket->Send(pParamToThread->pMySocket->m_sockConn, (char*)pMsg, iLen);

	// 5. ������Ϣ�ı༭�������ÿ�
	pParamToThread->pClntDlg->m_wstrMsgSent = "";
	pParamToThread->pClntDlg->SetDlgItemTextW(IDEDT_MSGSENT, pParamToThread->pClntDlg->m_wstrMsgSent);

	// 6. 
	delete[](BYTE*)pMsg;
}

void CSingleChat::RespondSingleChatMsg(char * const & strMsgRecv, _SParamToThread * pParamToThread)
{
	SMsg* pMsg = (SMsg*)strMsgRecv;

	// 0. ������Ƿ��ͷ�
	{
		if (g_uQQ == pMsg->uSubMsgCode[0])
		{
			// 0-0. �õ����������
			LPCWSTR wstrFollowContentByte = (LPCWSTR)(pMsg + 1);
			
			// 0-1. �������
			CString wstr = L">>>>>���ķ��ͳɹ�!<<<<<<";
			wstr += wstrFollowContentByte;
			wstr += L"********************************************************\r\n";

			// 0-2. ���뵽��ʾ��Ϣ��
			pParamToThread->pClntDlg->m_wstrShowMsg += wstr;

			// 0-3. �������ݵ���ʾ��
			pParamToThread->pClntDlg->SetDlgItemTextW(IDEDT_SHOWMSG, pParamToThread->pClntDlg->m_wstrShowMsg);
		}
	}

	// 1. ������ǽ��շ�
	if (g_uQQ == pMsg->uSubMsgCode[1])
	{
		// 0-0. �õ����������
		LPCWSTR wstrFollowContentByte = (LPCWSTR)(pMsg + 1);

		// 0-1. �������
		CString wstr = L">>>>>���յ�����!<<<<<<";
		wstr += wstrFollowContentByte;
		wstr += L"********************************************************\r\n";

		// 0-2. ���뵽��ʾ��Ϣ��
		pParamToThread->pClntDlg->m_wstrShowMsg += wstr;

		// 0-3. �������ݵ���ʾ��
		pParamToThread->pClntDlg->SetDlgItemTextW(IDEDT_SHOWMSG, pParamToThread->pClntDlg->m_wstrShowMsg);
	}
}
