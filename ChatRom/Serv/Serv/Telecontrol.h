#pragma once

struct _SParamToThread;


class CTelecontrol
{
public:
	CTelecontrol();
	~CTelecontrol();


	void SendTelecontrolMsg(_SParamToThread* pParamToThread);
};

