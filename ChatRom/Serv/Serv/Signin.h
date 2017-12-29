#pragma once

struct _SParamToThread;



class CSignin
{
public:
	CSignin();
	~CSignin();

	void RespondSigninMsg(char*const& strMsgRecv, _SParamToThread * pParamToThread);
};

