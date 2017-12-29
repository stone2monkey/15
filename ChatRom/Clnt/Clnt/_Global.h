#pragma once
#include <vector>

using namespace std;
class CClntDlg;
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









// �ͻ���Ҫ�õ��Ľṹ *****************************************************************************************************
typedef struct _SParamToThread				// ���̵߳Ĳ���
{
	CClntDlg* pClntDlg;
	CMySocket* pMySocket;
}SParamToThread;
typedef struct _SMsg						// ��Ϣ�ṹ
{
	UINT uEncode = (UINT)::GetTickCount();				// ������
	UINT uMsgCode{};			// ����Ϣ��
	UINT uSubMsgCode[10]{};		// ����Ϣ��, Ҳ��������������
	UINT uFollowContentByte{};	// �������ݵ��ֽ���,����������ṹ�ֽ�����
}SMsg;







// �ͻ���Ҫ�õ������� *****************************************************************************************************
extern vector<UINT> g_vectFriends;				// �����б� 
extern vector<UINT> g_vectOnlineFriends;			// ���ߺ����б� 
extern UINT g_uQQ;								// ��½��QQ;



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

