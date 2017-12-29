#pragma once

struct _SParamToThread;





class COffline
{
public:
	COffline();
	~COffline();


	void RespondOfflineMsg(char*const& strMsgRecv, _SParamToThread * pParamToThread);
};

