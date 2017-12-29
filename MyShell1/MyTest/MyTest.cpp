// MyTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>


int main()
{
	::MessageBoxW(::GetConsoleWindow(), L"一二三四事", L"N", 0);

	char* ch = "123456789";
	int i = strlen(ch);
	i = strcmp(ch, "abc");

	HWND hWnd = ::FindWindow(L"", L"");
	hWnd = 0;

	HANDLE hProc = ::GetCurrentProcess();
	hProc = 0;

	return 0;
}

