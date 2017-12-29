#pragma once

struct _SParamToThread;


class CAddFriend
{
public:
	CAddFriend();
	~CAddFriend();


	void RespondAddFriendMsg(char*const& strMsgRecv, _SParamToThread* pParamToThread);
};

