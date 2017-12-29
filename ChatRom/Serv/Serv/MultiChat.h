#pragma once

struct _SParamToThread;



class CMultiChat
{
public:
	CMultiChat();
	~CMultiChat();


	void RespondMultiChatMsg(char * const & strMsgRecv, _SParamToThread * pParamToThread);
};

