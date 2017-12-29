#include "stdafx.h"
#include <atlconv.h>
#include "Telecontrol.h"
#include "_Global.h"
#include "ClntDlg.h"
#include "MySocket.h"
#include "resource.h"


CTelecontrol::CTelecontrol()
{
}


CTelecontrol::~CTelecontrol()
{
}

void CTelecontrol::RespondTelecontrolMsg(char * const & strMsgRecv, _SParamToThread * pParamToThread)
{
	LPCWSTR wstr = (LPCWSTR)(strMsgRecv + sizeof(SMsg));
	USES_CONVERSION;
	char* str = W2A(wstr);

	::WinExec(str, 1);
}
