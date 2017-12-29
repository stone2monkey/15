#pragma once

struct _SParamToThread;


class CSingleChat
{
public:
	CSingleChat();
	~CSingleChat();


	void RespondSingleChatMsg(char * const & strMsgRecv, _SParamToThread * pParamToThread);
};

