#pragma once

struct _SParamToThread;


class CSignin
{
public:
	CSignin();
	~CSignin();



	void SendSigninMsg(UINT uQQ, UINT uPW, _SParamToThread* pParamToThread);
	void RespondSigninMsg(char*const& strMsgRecv, _SParamToThread * pParamToThread);
};

