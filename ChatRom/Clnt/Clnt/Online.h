#pragma once

struct _SParamToThread;




class COnline
{
public:
	COnline();
	~COnline();


	void RespondOnlineMsg(char*const& strMsgRecv, _SParamToThread * pParamToThread);

};

