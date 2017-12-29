#pragma once
#include <vector>
#include "DataBaseCtrl.h"


using namespace std;
class CServDlg;
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











// 服务端要用到的结构 *****************************************************************************************************
typedef	struct _SQQSinguppedInfo			// 注册QQ的信息 QQ PW 好友列表 
{
	UINT uQQ = 0;
	UINT uPW = 0;
	vector<UINT> vectFriends{};
}SQQSinguppedInfo;
typedef struct _SOnlineQQInfo				// 在线的QQ的信息 QQ SOCKET ADDR
{
	UINT uQQ = 0;
	SOCKET sockConn{};
	SOCKADDR_IN addrClnt{};		
}SOnlineQQInfo;
typedef struct _SParamToThread				// 给线程的参数
{
	CServDlg* pServDlg;
	CMySocket* pMySocket;
	SOCKET sockConn = 0;//accept成功后
	SOCKADDR_IN addrClnt{};//accept成功后
	UINT uQQ = 0;
}SParamToThread;
typedef struct _SMsg						// 消息结构
{
	UINT uEncode = (UINT)::GetTickCount();				// 加密码
	UINT uMsgCode{};			// 主消息码
	UINT uSubMsgCode[10]{};		// 子消息码, 也可以用来传数据
	UINT uFollowContentByte{};	// 跟随内容的字节数,不包括这个结构字节数的
}SMsg;




// 服务端要用到的数据 *****************************************************************************************************
extern vector<SQQSinguppedInfo> g_vectQQSinguppedInfo;		// 已经注册QQ的信息vector
extern vector<SOnlineQQInfo> g_vectOnlineQQInfo;			// 在线的QQ的信息vector






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

