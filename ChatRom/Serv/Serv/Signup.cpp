#include "stdafx.h"
#include "Signup.h"
#include "MySocket.h"
#include "ServDlg.h"
#include "_Global.h"
#include "resource.h"


CSignup::CSignup()
{
}


CSignup::~CSignup()
{
}

void CSignup::RespondSignupMsg(char * const & strMsgRecv, _SParamToThread * pParamToThread)
{
	SMsg* pMsg = (SMsg*)strMsgRecv;
	// 要注册的QQ PW
	UINT uQQ = pMsg->uSubMsgCode[0];
	UINT uPW = pMsg->uSubMsgCode[1];

	// 0. 是否在已注册列表中
	{
		int iLoop = (int)g_vectQQSinguppedInfo.size();
		for (int i = 0; i < iLoop; i++)
		{
			if (uQQ == g_vectQQSinguppedInfo.at(i).uQQ)
			{
				// 在已注册列表中
				// 0. 填充结构体	uSubMsgCode[0] = 1;	该QQ已被别人注册
				SMsg sMsg;
				sMsg.uMsgCode = C_SIGNUP_MSGCODE;
				sMsg.uSubMsgCode[0] = 1;

				// 1. 发送该QQ已被别人注册消息
				pParamToThread->pMySocket->Send(pParamToThread->sockConn, (char*)&sMsg, sizeof(sMsg));

				return;
			}
		}
	}

	// 1. 可以注册
    {
        // 0. 填充结构体	uSubMsgCode[0] = 0;  该QQ可以注册
        SMsg sMsg;
        sMsg.uMsgCode = C_SIGNUP_MSGCODE;
        sMsg.uSubMsgCode[0] = 0;
        sMsg.uSubMsgCode[1] = uQQ;

        // 1. 发送该QQ可以注册消息
        pParamToThread->pMySocket->Send(pParamToThread->sockConn, (char*)&sMsg, sizeof(sMsg));

        // 2. 压入到已注册vector中
        SQQSinguppedInfo sQQSinguppedInfo;
        sQQSinguppedInfo.uQQ = uQQ;
        sQQSinguppedInfo.uPW = uPW;
        g_vectQQSinguppedInfo.push_back(sQQSinguppedInfo);

        // 3. 写出到数据库
        CDataBaseCtrl oDataBaseCtrl;
        oDataBaseCtrl.LinkDataBase("0", "0", "0", "0");
        char str[0x400]{};
        sprintf_s(str, sizeof(str), "insert t_qqsignupped(uQQ, uPW) value (%d, %d);", uQQ, uPW);
        oDataBaseCtrl.ExecSqlByConnect(str);

		// 4. 注册成功记录
		// 消息记录
		CString wstrTime;
		FillCurrentTime(wstrTime);
		CString wstrAddr;
		FillAddrClnt(pParamToThread->addrClnt, wstrAddr);
		CString wstrQQ;
		FillQQ(uQQ, wstrQQ);
		CString wstr = wstrTime + wstrAddr + wstrQQ + L"注册成功!\r\n";
		pParamToThread->pServDlg->m_wstrShowMsg += wstr;
		pParamToThread->pServDlg->SetDlgItemTextW(IDEDT_SHOWMSG, pParamToThread->pServDlg->m_wstrShowMsg);
	}
}
