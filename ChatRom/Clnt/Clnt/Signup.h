#pragma once


//struct SParamToThread;
struct _SParamToThread;
//typename SParamToThread;


class CSignup
{
public:
	CSignup();
	~CSignup();


	void SendSignupMsg(UINT uQQ, UINT uPW, _SParamToThread* pParamToThread);
	void RespondSignupMsg(char*const& strMsgRecv, _SParamToThread * pParamToThread);
};