#include "stdafx.h"
#include "MyTSL.h"
#include <winternl.h>
#include "_CGlobal.h"
#include "MyDbger0Dlg.h"
#pragma comment(lib,"ntdll.lib")

_declspec (thread) int g_i = 0x1234;
void NTAPI tls_callback(PVOID h, DWORD reason, PVOID pv)
{
	if (IsDebuggerPresent())
	{
		ExitProcess(0);
	}
	

	return;
}

#pragma data_seg(".CRT$XLB")
PIMAGE_TLS_CALLBACK p_thread_callback[] = { tls_callback, NULL };
#pragma data_seg()

CMyTSL::CMyTSL()
{
}


CMyTSL::~CMyTSL()
{
}
