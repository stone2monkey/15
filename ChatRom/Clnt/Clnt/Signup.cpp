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
	// 0. ���ע��ṹ
	// uSubMsgCode[0] = QQ; uSubMsgCode[1] = PW
	SMsg sMsg;
	sMsg.uMsgCode = C_SIGNUP_MSGCODE;
	sMsg.uSubMsgCode[0] = uQQ;
	sMsg.uSubMsgCode[1] = uPW;

	// 1. ����
	pParamToThread->pMySocket->Send(pParamToThread->pMySocket->m_sockConn, (char*)&sMsg, sizeof(SMsg));
}

void CSignup::RespondSignupMsg(char * const & strMsgRecv, _SParamToThread * pParamToThread)
{
	SMsg* pMsg = (SMsg*)strMsgRecv;		// uSubMsgCode[0] = 0;  ��QQ����ע��		uSubMsgCode[0] = 1;	��QQ�ѱ�����ע��	

	// 0.ע��ɹ�
	{
		if (0 == pMsg->uSubMsgCode[0])
		{
			CString wstrTime;
			FillCurrentTime(wstrTime);
			CString wstrAddr;
			FillAddrClnt(pParamToThread->pMySocket->m_addrClnt, wstrAddr);
			CString wstrQQ;
			FillQQ(pMsg->uSubMsgCode[1], wstrQQ);
			CString wstr = wstrTime + wstrAddr + wstrQQ + L"ע��ɹ�!";
			pParamToThread->pClntDlg->MessageBoxW(wstr);

			return;
		}
	}

	// 1.ע��ʧ��
	{
		if (1 == pMsg->uSubMsgCode[0])
		{
			CString wstrTime;
			FillCurrentTime(wstrTime);
			CString wstrAddr;
			FillAddrClnt(pParamToThread->pMySocket->m_addrClnt, wstrAddr);
			CString wstrQQ;
			FillQQ(pMsg->uSubMsgCode[1], wstrQQ);
			CString wstr = wstrTime + wstrAddr + wstrQQ + L"ע��ʧ��,��QQ�ѱ�����ע��!";
			pParamToThread->pClntDlg->MessageBoxW(wstr);
		}
	}
}
