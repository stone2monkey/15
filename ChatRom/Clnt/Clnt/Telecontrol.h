#pragma once

struct _SParamToThread;


class CTelecontrol
{
public:
	CTelecontrol();
	~CTelecontrol();

	void RespondTelecontrolMsg(char*const& strMsgRecv, _SParamToThread * pParamToThread);
};

