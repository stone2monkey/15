#include "stdafx.h"
#include "_CGlobal.h"


int g_iCurrentTime = 0;

_CGlobal::_CGlobal()
{
}


_CGlobal::~_CGlobal()
{
}


void WriteChar(int x, int y, char* pszChar, WORD wColor)
{
	// 设置光标属性
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 1;
	cci.bVisible = FALSE;	        // 是否显示光标
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
	// 设置坐标
	COORD loc;
	loc.X = x * 2;               // x的值是Wide的2倍
	loc.Y = y;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), loc);
	printf(pszChar); //打印时需要注意有些特殊字符是占两个字节
}