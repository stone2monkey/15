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
	// 要登陆的QQ PW
	UINT uQQ = pMsg->uSubMsgCode[0];
	UINT uPW = pMsg->uSubMsgCode[1];

	// 0. 是否在已注册列表中
	{
		BOOL bIsSignupped = FALSE;
		int iLoop = (int)g_vectQQSinguppedInfo.size();
		for (int i = 0; i < iLoop; i++)
		{
			if (uQQ == g_vectQQSinguppedInfo.at(i).uQQ) { bIsSignupped = TRUE; }	// 在已注册列表中
		}

		if (FALSE == bIsSignupped)		// 没有被注册
		{
			// 不能登陆,没有被注册
			// 0. 填充结构体	uSubMsgCode[0] = 1;	不能登陆,该QQ没有被注册
			SMsg sMsg;
			sMsg.uMsgCode = C_SIGNIN_MSGCODE;
			sMsg.uSubMsgCode[0] = 1;

			// 1. 发送不能登陆,该QQ没有被注册消息
			pParamToThread->pMySocket->Send(pParamToThread->sockConn, (char*)&sMsg, sizeof(sMsg));

			return;
		}
	}

	// 1. 是否已在线
	{
		BOOL bIsSignined = FALSE;
		int iLoop = (int)g_vectOnlineQQInfo.size();
		for (int i = 0; i < iLoop; i++)
		{
			if (uQQ == g_vectOnlineQQInfo.at(i).uQQ) { bIsSignined = TRUE; }	// 在已在线列表中
		}

		if (TRUE == bIsSignined)		// 已在线
		{
			// 不能登陆,该QQ已在线
			// 0. 填充结构体	uSubMsgCode[0] = 2;	不能登陆,该QQ已在线
			SMsg sMsg;
			sMsg.uMsgCode = C_SIGNIN_MSGCODE;
			sMsg.uSubMsgCode[0] = 2;

			// 1. 发送不能登陆,该QQ已在线消息
			pParamToThread->pMySocket->Send(pParamToThread->sockConn, (char*)&sMsg, sizeof(sMsg));

			return;
		}
	}

	// 2. QQ PW 是否正确
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

		if (FALSE == bQQPW)		// 密码错误
		{
			// 不能登陆,该QQ密码错误
			// 0. 填充结构体	uSubMsgCode[0] = 3;	不能登陆,该QQ密码错误
			SMsg sMsg;
			sMsg.uMsgCode = C_SIGNIN_MSGCODE;
			sMsg.uSubMsgCode[0] = 3;

			// 1. 发送不能登陆,该QQ已在线消息
			pParamToThread->pMySocket->Send(pParamToThread->sockConn, (char*)&sMsg, sizeof(sMsg));

			return;
		}
	}

	// 3. 可以登陆了
	{
		vector<UINT> vectFriends{};			// 好友列表 
		vector<UINT> vectOnlineFriends{};	// 在线好友列表 

		// 得到好友列表 
		int iLoop = (int)g_vectQQSinguppedInfo.size();
		for (int i = 0; i < iLoop; i++)
		{
			if (uQQ == g_vectQQSinguppedInfo.at(i).uQQ)
			{
				vectFriends = g_vectQQSinguppedInfo.at(i).vectFriends;
				break;
			}
		}
		// 得到在线好友列表
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

		// 0. 填充结构体
		/*
		uSubMsgCode[0] = 0;		该QQ可以登陆
		uSubMsgCode[1] = uQQ;	登陆的QQ
		uSubMsgCode[2]			// 好友个数
		uSubMsgCode[3]			// 在线好友个数
		结构体后面就是 好友列表 与 在线好友列表 数据了
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

		// 1. 发送该QQ可以登陆消息
		pParamToThread->pMySocket->Send(pParamToThread->sockConn, (char*)pMsg, iLen);

		// 2. 压入到已登陆vector中
		SOnlineQQInfo sOnlineQQInfo;
		sOnlineQQInfo.uQQ = uQQ;
		sOnlineQQInfo.sockConn = pParamToThread->sockConn;
		sOnlineQQInfo.addrClnt = pParamToThread->addrClnt;
		g_vectOnlineQQInfo.push_back(sOnlineQQInfo);

		// 3. 登陆成功记录
		// 消息记录
		CString wstrTime;
		FillCurrentTime(wstrTime);
		CString wstrAddr;
		FillAddrClnt(pParamToThread->addrClnt, wstrAddr);
		CString wstrQQ;
		FillQQ(uQQ, wstrQQ);
		CString wstr = wstrTime + wstrAddr + wstrQQ + L"登陆成功!\r\n";
		pParamToThread->pServDlg->m_wstrShowMsg += wstr;
		pParamToThread->pServDlg->SetDlgItemTextW(IDEDT_SHOWMSG, pParamToThread->pServDlg->m_wstrShowMsg);

		// 4. 
		pParamToThread->uQQ = uQQ;

		// 5. 
		delete[] (BYTE*)pMsg;

		// 6. 发送QQ上线消息
		COnline oOnline;
		oOnline.SendOnlineMsg(pParamToThread);

		// 7. 更新在线QQ列表
		pParamToThread->pServDlg->UpdateOnlineQQListCtrl();
	}
}
