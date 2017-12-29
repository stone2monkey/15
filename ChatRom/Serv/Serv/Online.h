#pragma once

struct _SParamToThread;





class COnline
{
public:
	COnline();
	~COnline();

	void SendOnlineMsg(_SParamToThread* pParamToThread);
};

