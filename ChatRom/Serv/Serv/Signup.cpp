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
	// Ҫע���QQ PW
	UINT uQQ = pMsg->uSubMsgCode[0];
	UINT uPW = pMsg->uSubMsgCode[1];

	// 0. �Ƿ�����ע���б���
	{
		int iLoop = (int)g_vectQQSinguppedInfo.size();
		for (int i = 0; i < iLoop; i++)
		{
			if (uQQ == g_vectQQSinguppedInfo.at(i).uQQ)
			{
				// ����ע���б���
				// 0. ���ṹ��	uSubMsgCode[0] = 1;	��QQ�ѱ�����ע��
				SMsg sMsg;
				sMsg.uMsgCode = C_SIGNUP_MSGCODE;
				sMsg.uSubMsgCode[0] = 1;

				// 1. ���͸�QQ�ѱ�����ע����Ϣ
				pParamToThread->pMySocket->Send(pParamToThread->sockConn, (char*)&sMsg, sizeof(sMsg));

				return;
			}
		}
	}

	// 1. ����ע��
    {
        // 0. ���ṹ��	uSubMsgCode[0] = 0;  ��QQ����ע��
        SMsg sMsg;
        sMsg.uMsgCode = C_SIGNUP_MSGCODE;
        sMsg.uSubMsgCode[0] = 0;
        sMsg.uSubMsgCode[1] = uQQ;

        // 1. ���͸�QQ����ע����Ϣ
        pParamToThread->pMySocket->Send(pParamToThread->sockConn, (char*)&sMsg, sizeof(sMsg));

        // 2. ѹ�뵽��ע��vector��
        SQQSinguppedInfo sQQSinguppedInfo;
        sQQSinguppedInfo.uQQ = uQQ;
        sQQSinguppedInfo.uPW = uPW;
        g_vectQQSinguppedInfo.push_back(sQQSinguppedInfo);

        // 3. д�������ݿ�
        CDataBaseCtrl oDataBaseCtrl;
        oDataBaseCtrl.LinkDataBase("0", "0", "0", "0");
        char str[0x400]{};
        sprintf_s(str, sizeof(str), "insert t_qqsignupped(uQQ, uPW) value (%d, %d);", uQQ, uPW);
        oDataBaseCtrl.ExecSqlByConnect(str);

		// 4. ע��ɹ���¼
		// ��Ϣ��¼
		CString wstrTime;
		FillCurrentTime(wstrTime);
		CString wstrAddr;
		FillAddrClnt(pParamToThread->addrClnt, wstrAddr);
		CString wstrQQ;
		FillQQ(uQQ, wstrQQ);
		CString wstr = wstrTime + wstrAddr + wstrQQ + L"ע��ɹ�!\r\n";
		pParamToThread->pServDlg->m_wstrShowMsg += wstr;
		pParamToThread->pServDlg->SetDlgItemTextW(IDEDT_SHOWMSG, pParamToThread->pServDlg->m_wstrShowMsg);
	}
}
