#pragma once
#include <vector>
#include "DataBaseCtrl.h"


using namespace std;
class CServDlg;
class CMySocket;


// ��Ϣ�� ****************************************************************************************************************
#define C_SIGNUP_MSGCODE 1
#define C_SIGNIN_MSGCODE 2
#define S_ONLINE_MSGCODE 3
#define S_OFFLINE_MSGCODE 4
#define C_ADDFRIEND_MSGCODE 5
#define C_MUILTCHAT_MSGCODE 6
#define C_SINGLECHAT_MSGCODE 7
#define S_TELECONTROL_MSGCODE 8











// �����Ҫ�õ��Ľṹ *****************************************************************************************************
typedef	struct _SQQSinguppedInfo			// ע��QQ����Ϣ QQ PW �����б� 
{
	UINT uQQ = 0;
	UINT uPW = 0;
	vector<UINT> vectFriends{};
}SQQSinguppedInfo;
typedef struct _SOnlineQQInfo				// ���ߵ�QQ����Ϣ QQ SOCKET ADDR
{
	UINT uQQ = 0;
	SOCKET sockConn{};
	SOCKADDR_IN addrClnt{};		
}SOnlineQQInfo;
typedef struct _SParamToThread				// ���̵߳Ĳ���
{
	CServDlg* pServDlg;
	CMySocket* pMySocket;
	SOCKET sockConn = 0;//accept�ɹ���
	SOCKADDR_IN addrClnt{};//accept�ɹ���
	UINT uQQ = 0;
}SParamToThread;
typedef struct _SMsg						// ��Ϣ�ṹ
{
	UINT uEncode = (UINT)::GetTickCount();				// ������
	UINT uMsgCode{};			// ����Ϣ��
	UINT uSubMsgCode[10]{};		// ����Ϣ��, Ҳ��������������
	UINT uFollowContentByte{};	// �������ݵ��ֽ���,����������ṹ�ֽ�����
}SMsg;




// �����Ҫ�õ������� *****************************************************************************************************
extern vector<SQQSinguppedInfo> g_vectQQSinguppedInfo;		// �Ѿ�ע��QQ����Ϣvector
extern vector<SOnlineQQInfo> g_vectOnlineQQInfo;			// ���ߵ�QQ����Ϣvector






// ȫ�ַ��� ****************************************************************************************************************
extern void FillCurrentTime(CString& wstrTime);			//[Time **:**:**]
extern void FillAddrClnt(SOCKADDR_IN addrClnt, CString& wstrAddr);			//[Addr ***.***.***.***:*****]
extern void FillQQ(UINT uQQ, CString& wstrQQ);		// [QQ ********]





































class _Global
{
public:
	_Global();
	~_Global();
};

