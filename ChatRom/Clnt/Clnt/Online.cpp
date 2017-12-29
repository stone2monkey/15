#include "stdafx.h"
#include "Online.h"
#include "_Global.h"
#include "ClntDlg.h"
#include "MySocket.h"
#include "resource.h"


COnline::COnline()
{
}


COnline::~COnline()
{
}

void COnline::RespondOnlineMsg(char * const & strMsgRecv, _SParamToThread * pParamToThread)
{
	SMsg* pMsg = (SMsg*)strMsgRecv;

	// 0. ������Լ�return;
	{
		if (0 == g_uQQ) { return; }		// ��û�е�½
		if (pMsg->uSubMsgCode[0] == g_uQQ) { return;	}	//���Լ�������
	}

	// 1. �ǲ����Լ�����
	{
		BOOL bIsMyFriend = FALSE;
		int iLoop = g_vectFriends.size();
		for (int i = 0; i < iLoop; i++)
		{
			if (pMsg->uSubMsgCode[0] == g_vectFriends.at(i)) { bIsMyFriend = TRUE; break;}
		}

		if (FALSE == bIsMyFriend) { return; }
	}

	// 2. ���ҵĺ���, ���Ƿ��������б���,�������,ѹ��
	{
		BOOL bIsInOnlineFriends = FALSE;
		int iLoop = g_vectOnlineFriends.size();
		for (int i = 0; i < iLoop; i++)
		{
			if (pMsg->uSubMsgCode[0] == g_vectOnlineFriends.at(i)) { bIsInOnlineFriends = TRUE; break; }
		}

		if (FALSE == bIsInOnlineFriends) 
		{ 
			g_vectOnlineFriends.push_back(pMsg->uSubMsgCode[0]); 
			pParamToThread->pClntDlg->UpdateOnlineQQListCtrl();
		}
		//else { assert(0); }		// ����ֻ����һ��,�������������������Ϣʱ,���QQ����������б���,���������
	}
	
}
