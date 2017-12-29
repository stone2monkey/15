#include "stdafx.h"
#include "SingleStepBP.h"
#include "_CGlobal.h"
#include "_MyCustomType.h"
#include "PE.h"
#include "Interface.h"


void CSingleStepBP::AddBP(DWORD dwTID)
{
	// 得到线程句柄
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, 0, dwTID);
	if (!hThread) { assert(0); }

	// 得到线程环境
	CONTEXT ct{};
	ct.ContextFlags = CONTEXT_ALL;
	if (!::GetThreadContext(hThread, &ct)) { assert(0); }

	// 修改TF
	ct.EFlags |= 0b100000000;

	// 设置线程环境
	if (!::SetThreadContext(hThread, &ct)) { assert(0); }

	// 关闭线程句柄
	CloseHandle(hThread);
}

CSingleStepBP::CSingleStepBP()
{
}


CSingleStepBP::~CSingleStepBP()
{
}
