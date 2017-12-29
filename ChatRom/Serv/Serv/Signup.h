#pragma once

struct _SParamToThread;

class CSignup
{
public:
	CSignup();
	~CSignup();


	void RespondSignupMsg(char*const& strMsgRecv, _SParamToThread * pParamToThread);
};

