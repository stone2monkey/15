#pragma once

struct _SParamToThread;



class CSingleChat
{
public:
	CSingleChat();
	~CSingleChat();

	void SendSingleChatMsg(_SParamToThread* pParamToThread);
	void RespondSingleChatMsg(char*const& strMsgRecv, _SParamToThread * pParamToThread);
};

