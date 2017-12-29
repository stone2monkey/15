#pragma once

struct _SParamToThread;





class CAddFriend
{
public:
	CAddFriend();
	~CAddFriend();


	void SendAddFriendMsg(UINT uQQ, _SParamToThread* pParamToThread);
	void RespondAddFriendMsg(char*const& strMsgRecv, _SParamToThread * pParamToThread);
};

