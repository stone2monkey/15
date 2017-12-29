#include "stdafx.h"
#include "SingleStepBP.h"
#include "_CGlobal.h"
#include "_MyCustomType.h"
#include "PE.h"
#include "Interface.h"


void CSingleStepBP::AddBP(DWORD dwTID)
{
	// �õ��߳̾��
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, 0, dwTID);
	if (!hThread) { assert(0); }

	// �õ��̻߳���
	CONTEXT ct{};
	ct.ContextFlags = CONTEXT_ALL;
	if (!::GetThreadContext(hThread, &ct)) { assert(0); }

	// �޸�TF
	ct.EFlags |= 0b100000000;

	// �����̻߳���
	if (!::SetThreadContext(hThread, &ct)) { assert(0); }

	// �ر��߳̾��
	CloseHandle(hThread);
}

CSingleStepBP::CSingleStepBP()
{
}


CSingleStepBP::~CSingleStepBP()
{
}
