#pragma once

struct _SParamToThread;


class CMultiChat
{
public:
	CMultiChat();
	~CMultiChat();

	void SendMultiChatMsg(_SParamToThread* pParamToThread);
	void RespondMultiChatMsg(char*const& strMsgRecv, _SParamToThread * pParamToThread);
};

