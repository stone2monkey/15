#include "stdafx.h"
#include "Signup.h"
#include "_Global.h"
#include "ClntDlg.h"
#include "MySocket.h"

CSignup::CSignup()
{
}


CSignup::~CSignup()
{
}

void CSignup::SendSignupMsg(UINT uQQ, UINT uPW, _SParamToThread * pParamToThread)
{
	// 0. 填充注册结构
	// uSubMsgCode[0] = QQ; uSubMsgCode[1] = PW
	SMsg sMsg;
	sMsg.uMsgCode = C_SIGNUP_MSGCODE;
	sMsg.uSubMsgCode[0] = uQQ;
	sMsg.uSubMsgCode[1] = uPW;

	// 1. 发送
	pParamToThread->pMySocket->Send(pParamToThread->pMySocket->m_sockConn, (char*)&sMsg, sizeof(SMsg));
}

void CSignup::RespondSignupMsg(char * const & strMsgRecv, _SParamToThread * pParamToThread)
{
	SMsg* pMsg = (SMsg*)strMsgRecv;		// uSubMsgCode[0] = 0;  该QQ可以注册		uSubMsgCode[0] = 1;	该QQ已被别人注册	

	// 0.注册成功
	{
		if (0 == pMsg->uSubMsgCode[0])
		{
			CString wstrTime;
			FillCurrentTime(wstrTime);
			CString wstrAddr;
			FillAddrClnt(pParamToThread->pMySocket->m_addrClnt, wstrAddr);
			CString wstrQQ;
			FillQQ(pMsg->uSubMsgCode[1], wstrQQ);
			CString wstr = wstrTime + wstrAddr + wstrQQ + L"注册成功!";
			pParamToThread->pClntDlg->MessageBoxW(wstr);

			return;
		}
	}

	// 1.注册失败
	{
		if (1 == pMsg->uSubMsgCode[0])
		{
			CString wstrTime;
			FillCurrentTime(wstrTime);
			CString wstrAddr;
			FillAddrClnt(pParamToThread->pMySocket->m_addrClnt, wstrAddr);
			CString wstrQQ;
			FillQQ(pMsg->uSubMsgCode[1], wstrQQ);
			CString wstr = wstrTime + wstrAddr + wstrQQ + L"注册失败,该QQ已被别人注册!";
			pParamToThread->pClntDlg->MessageBoxW(wstr);
		}
	}
}
