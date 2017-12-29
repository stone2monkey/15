#include "stdafx.h"
#include "Signin.h"
#include "_Global.h"
#include "ClntDlg.h"
#include "MySocket.h"
#include "resource.h"


CSignin::CSignin()
{
}


CSignin::~CSignin()
{
}

void CSignin::SendSigninMsg(UINT uQQ, UINT uPW, _SParamToThread * pParamToThread)
{
	// 0. 填充登陆结构
	// uSubMsgCode[0] = QQ; uSubMsgCode[1] = PW
	SMsg sMsg;
	sMsg.uMsgCode = C_SIGNIN_MSGCODE;
	sMsg.uSubMsgCode[0] = uQQ;
	sMsg.uSubMsgCode[1] = uPW;

	// 1. 发送
	pParamToThread->pMySocket->Send(pParamToThread->pMySocket->m_sockConn, (char*)&sMsg, sizeof(SMsg));

}

void CSignin::RespondSigninMsg(char * const & strMsgRecv, _SParamToThread * pParamToThread)
{
	SMsg* pMsg = (SMsg*)strMsgRecv;		

	// 0. uSubMsgCode[0] = 0;	该QQ可以登陆		
	// 1. uSubMsgCode[0] = 1;	不能登陆,该QQ没有被注册	
	// 2. uSubMsgCode[0] = 2;	不能登陆,该QQ已在线
	// 3. uSubMsgCode[0] = 3;	不能登陆, 该QQ密码错误

	
	// 0. uSubMsgCode[0] = 0;	该QQ可以登陆	
	{
		if (0 == pMsg->uSubMsgCode[0])
		{
			// 0. 填充全局好友列表
			for (int i = 0; (UINT)i < pMsg->uSubMsgCode[2]; i++)
			{
				UINT uQQ = *((UINT*)(pMsg + 1) + (UINT)i);
				g_vectFriends.push_back(uQQ);
			}

			// 1. 填充全局在线好友列表
			for (int i = 0; (UINT)i < pMsg->uSubMsgCode[3]; i++)
			{
				UINT uQQ = *((UINT*)(pMsg + 1) + pMsg->uSubMsgCode[2] + (UINT)i);
				g_vectOnlineFriends.push_back(uQQ);
			}

			// 2. 对话框控件跟进
			pParamToThread->pClntDlg->GetDlgItem(IDEDT_QQ)->EnableWindow(0);
			pParamToThread->pClntDlg->GetDlgItem(IDEDT_PW)->EnableWindow(0);
			pParamToThread->pClntDlg->GetDlgItem(IDBTN_LOGUP)->EnableWindow(0);
			pParamToThread->pClntDlg->GetDlgItem(IDBTN_LOGIN)->EnableWindow(0);
			pParamToThread->pClntDlg->UpdateOnlineQQListCtrl();

			// 3. 成功对话框
			CString wstrTime;
			FillCurrentTime(wstrTime);
			CString wstrAddr;
			FillAddrClnt(pParamToThread->pMySocket->m_addrClnt, wstrAddr);
			CString wstrQQ;
			FillQQ(pMsg->uSubMsgCode[1], wstrQQ);
			CString wstr = wstrTime + wstrAddr + wstrQQ + L"登陆成功!";
            pParamToThread->pClntDlg->SetWindowTextW(wstrQQ + wstrAddr);
            pParamToThread->pClntDlg->MessageBoxW(wstr);

            pParamToThread->pClntDlg->GetDlgItem(IDEDT_QQ)->EnableWindow(0);
            pParamToThread->pClntDlg->GetDlgItem(IDEDT_PW)->EnableWindow(0);
            pParamToThread->pClntDlg->GetDlgItem(IDBTN_LOGUP)->EnableWindow(0);
            pParamToThread->pClntDlg->GetDlgItem(IDBTN_LOGIN)->EnableWindow(0);
            pParamToThread->pClntDlg->GetDlgItem(IDC_EDIT3)->EnableWindow(1);
            pParamToThread->pClntDlg->GetDlgItem(IDBTN_SEEK)->EnableWindow(1);
            pParamToThread->pClntDlg->GetDlgItem(IDBTN_ADD)->EnableWindow(1);

			// 4. 得到全局QQ
			g_uQQ = pMsg->uSubMsgCode[1];


			return;
		}
	}

	// 1. uSubMsgCode[0] = 1;	不能登陆,该QQ没有被注册	
	{
		if (1 == pMsg->uSubMsgCode[0])
		{
			CString wstrTime;
			FillCurrentTime(wstrTime);
			CString wstrAddr;
			FillAddrClnt(pParamToThread->pMySocket->m_addrClnt, wstrAddr);
			CString wstrQQ;
			FillQQ(pMsg->uSubMsgCode[1], wstrQQ);
			CString wstr = wstrTime + wstrAddr + wstrQQ + L"不能登陆,该QQ没有被注册!";
			pParamToThread->pClntDlg->MessageBoxW(wstr);

			return;
		}
	}

	// 2. uSubMsgCode[0] = 2;	不能登陆,该QQ已在线
	{
		if (2 == pMsg->uSubMsgCode[0])
		{
			CString wstrTime;
			FillCurrentTime(wstrTime);
			CString wstrAddr;
			FillAddrClnt(pParamToThread->pMySocket->m_addrClnt, wstrAddr);
			CString wstrQQ;
			FillQQ(pMsg->uSubMsgCode[1], wstrQQ);
			CString wstr = wstrTime + wstrAddr + wstrQQ + L"不能登陆,该QQ已在线";
			pParamToThread->pClntDlg->MessageBoxW(wstr);

			return;
		}
	}

	// 3. uSubMsgCode[0] = 3;	不能登陆, 该QQ密码错误
	{
		if (3 == pMsg->uSubMsgCode[0])
		{
			CString wstrTime;
			FillCurrentTime(wstrTime);
			CString wstrAddr;
			FillAddrClnt(pParamToThread->pMySocket->m_addrClnt, wstrAddr);
			CString wstrQQ;
			FillQQ(pMsg->uSubMsgCode[1], wstrQQ);
			CString wstr = wstrTime + wstrAddr + wstrQQ + L"不能登陆,该QQ密码错误!";
			pParamToThread->pClntDlg->MessageBoxW(wstr);

			return;
		}
	}
}
