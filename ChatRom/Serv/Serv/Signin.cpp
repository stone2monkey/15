#include "stdafx.h"
#include "Signin.h"
#include "MySocket.h"
#include "ServDlg.h"
#include "_Global.h"
#include "resource.h"
#include "Online.h"


CSignin::CSignin()
{
}


CSignin::~CSignin()
{
}

void CSignin::RespondSigninMsg(char * const & strMsgRecv, _SParamToThread * pParamToThread)
{
	SMsg* pMsg = (SMsg*)strMsgRecv;
	// Ҫ��½��QQ PW
	UINT uQQ = pMsg->uSubMsgCode[0];
	UINT uPW = pMsg->uSubMsgCode[1];

	// 0. �Ƿ�����ע���б���
	{
		BOOL bIsSignupped = FALSE;
		int iLoop = (int)g_vectQQSinguppedInfo.size();
		for (int i = 0; i < iLoop; i++)
		{
			if (uQQ == g_vectQQSinguppedInfo.at(i).uQQ) { bIsSignupped = TRUE; }	// ����ע���б���
		}

		if (FALSE == bIsSignupped)		// û�б�ע��
		{
			// ���ܵ�½,û�б�ע��
			// 0. ���ṹ��	uSubMsgCode[0] = 1;	���ܵ�½,��QQû�б�ע��
			SMsg sMsg;
			sMsg.uMsgCode = C_SIGNIN_MSGCODE;
			sMsg.uSubMsgCode[0] = 1;

			// 1. ���Ͳ��ܵ�½,��QQû�б�ע����Ϣ
			pParamToThread->pMySocket->Send(pParamToThread->sockConn, (char*)&sMsg, sizeof(sMsg));

			return;
		}
	}

	// 1. �Ƿ�������
	{
		BOOL bIsSignined = FALSE;
		int iLoop = (int)g_vectOnlineQQInfo.size();
		for (int i = 0; i < iLoop; i++)
		{
			if (uQQ == g_vectOnlineQQInfo.at(i).uQQ) { bIsSignined = TRUE; }	// ���������б���
		}

		if (TRUE == bIsSignined)		// ������
		{
			// ���ܵ�½,��QQ������
			// 0. ���ṹ��	uSubMsgCode[0] = 2;	���ܵ�½,��QQ������
			SMsg sMsg;
			sMsg.uMsgCode = C_SIGNIN_MSGCODE;
			sMsg.uSubMsgCode[0] = 2;

			// 1. ���Ͳ��ܵ�½,��QQ��������Ϣ
			pParamToThread->pMySocket->Send(pParamToThread->sockConn, (char*)&sMsg, sizeof(sMsg));

			return;
		}
	}

	// 2. QQ PW �Ƿ���ȷ
	{
		BOOL bQQPW = FALSE;
		int iLoop = (int)g_vectQQSinguppedInfo.size();
		for (int i = 0; i < iLoop; i++)
		{
			if (uQQ == g_vectQQSinguppedInfo.at(i).uQQ && uPW == g_vectQQSinguppedInfo.at(i).uPW)
			{
				bQQPW = TRUE;
			}
		}

		if (FALSE == bQQPW)		// �������
		{
			// ���ܵ�½,��QQ�������
			// 0. ���ṹ��	uSubMsgCode[0] = 3;	���ܵ�½,��QQ�������
			SMsg sMsg;
			sMsg.uMsgCode = C_SIGNIN_MSGCODE;
			sMsg.uSubMsgCode[0] = 3;

			// 1. ���Ͳ��ܵ�½,��QQ��������Ϣ
			pParamToThread->pMySocket->Send(pParamToThread->sockConn, (char*)&sMsg, sizeof(sMsg));

			return;
		}
	}

	// 3. ���Ե�½��
	{
		vector<UINT> vectFriends{};			// �����б� 
		vector<UINT> vectOnlineFriends{};	// ���ߺ����б� 

		// �õ������б� 
		int iLoop = (int)g_vectQQSinguppedInfo.size();
		for (int i = 0; i < iLoop; i++)
		{
			if (uQQ == g_vectQQSinguppedInfo.at(i).uQQ)
			{
				vectFriends = g_vectQQSinguppedInfo.at(i).vectFriends;
				break;
			}
		}
		// �õ����ߺ����б�
		iLoop = (int)vectFriends.size();
		int jLoop = (int)g_vectOnlineQQInfo.size();
		for (int i = 0; i < iLoop; i++)
		{
			for (int j = 0; i < jLoop; j++)
			{
				if (vectFriends.at(i) == g_vectOnlineQQInfo.at(j).uQQ)
				{
					vectOnlineFriends.push_back(vectFriends.at(i));
					break;
				}
			}
		}

		// 0. ���ṹ��
		/*
		uSubMsgCode[0] = 0;		��QQ���Ե�½
		uSubMsgCode[1] = uQQ;	��½��QQ
		uSubMsgCode[2]			// ���Ѹ���
		uSubMsgCode[3]			// ���ߺ��Ѹ���
		�ṹ�������� �����б� �� ���ߺ����б� ������
		*/
		int iLen = (int)vectFriends.size() * 4 + (int)vectOnlineFriends.size() * 4 + sizeof(SMsg);
		SMsg* pMsg = (SMsg*)new BYTE[iLen];
		pMsg->uMsgCode = C_SIGNIN_MSGCODE;
		pMsg->uSubMsgCode[0] = 0;
		pMsg->uSubMsgCode[1] = uQQ;
		pMsg->uSubMsgCode[2] = (int)vectFriends.size();
		pMsg->uSubMsgCode[3] = (int)vectOnlineFriends.size();
		if (!vectFriends.empty()){ ::memcpy(pMsg + 1, &vectFriends.at(0), (int)vectFriends.size() * 4); }
		if (!vectOnlineFriends.empty()) { ::memcpy((BYTE*)(pMsg + 1) + (int)vectFriends.size() * 4, &vectOnlineFriends.at(0), (int)vectOnlineFriends.size() * 4); }

		// 1. ���͸�QQ���Ե�½��Ϣ
		pParamToThread->pMySocket->Send(pParamToThread->sockConn, (char*)pMsg, iLen);

		// 2. ѹ�뵽�ѵ�½vector��
		SOnlineQQInfo sOnlineQQInfo;
		sOnlineQQInfo.uQQ = uQQ;
		sOnlineQQInfo.sockConn = pParamToThread->sockConn;
		sOnlineQQInfo.addrClnt = pParamToThread->addrClnt;
		g_vectOnlineQQInfo.push_back(sOnlineQQInfo);

		// 3. ��½�ɹ���¼
		// ��Ϣ��¼
		CString wstrTime;
		FillCurrentTime(wstrTime);
		CString wstrAddr;
		FillAddrClnt(pParamToThread->addrClnt, wstrAddr);
		CString wstrQQ;
		FillQQ(uQQ, wstrQQ);
		CString wstr = wstrTime + wstrAddr + wstrQQ + L"��½�ɹ�!\r\n";
		pParamToThread->pServDlg->m_wstrShowMsg += wstr;
		pParamToThread->pServDlg->SetDlgItemTextW(IDEDT_SHOWMSG, pParamToThread->pServDlg->m_wstrShowMsg);

		// 4. 
		pParamToThread->uQQ = uQQ;

		// 5. 
		delete[] (BYTE*)pMsg;

		// 6. ����QQ������Ϣ
		COnline oOnline;
		oOnline.SendOnlineMsg(pParamToThread);

		// 7. ��������QQ�б�
		pParamToThread->pServDlg->UpdateOnlineQQListCtrl();
	}
}
