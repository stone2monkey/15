#pragma once
#include <vector>

using namespace std;
class CClntDlg;
class CMySocket;



// 消息码 ****************************************************************************************************************
#define C_SIGNUP_MSGCODE 1
#define C_SIGNIN_MSGCODE 2
#define S_ONLINE_MSGCODE 3
#define S_OFFLINE_MSGCODE 4
#define C_ADDFRIEND_MSGCODE 5
#define C_MUILTCHAT_MSGCODE 6
#define C_SINGLECHAT_MSGCODE 7
#define S_TELECONTROL_MSGCODE 8









// 客户端要用到的结构 *****************************************************************************************************
typedef struct _SParamToThread				// 给线程的参数
{
	CClntDlg* pClntDlg;
	CMySocket* pMySocket;
}SParamToThread;
typedef struct _SMsg						// 消息结构
{
	UINT uEncode = (UINT)::GetTickCount();				// 加密码
	UINT uMsgCode{};			// 主消息码
	UINT uSubMsgCode[10]{};		// 子消息码, 也可以用来传数据
	UINT uFollowContentByte{};	// 跟随内容的字节数,不包括这个结构字节数的
}SMsg;







// 客户端要用到的数据 *****************************************************************************************************
extern vector<UINT> g_vectFriends;				// 好友列表 
extern vector<UINT> g_vectOnlineFriends;			// 在线好友列表 
extern UINT g_uQQ;								// 登陆的QQ;



// 全局方法 ****************************************************************************************************************
extern void FillCurrentTime(CString& wstrTime);			//[Time **:**:**]
extern void FillAddrClnt(SOCKADDR_IN addrClnt, CString& wstrAddr);			//[Addr ***.***.***.***:*****]
extern void FillQQ(UINT uQQ, CString& wstrQQ);		// [QQ ********]



































class _Global
{
public:
	_Global();
	~_Global();
};

