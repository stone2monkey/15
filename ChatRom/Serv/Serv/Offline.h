#pragma once

struct _SParamToThread;




class COffline
{
public:
	COffline();
	~COffline();


	void SendOfflineMsg(_SParamToThread* pParamToThread);
};

