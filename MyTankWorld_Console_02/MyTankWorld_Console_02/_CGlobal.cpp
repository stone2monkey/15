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
	// ���ù������
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 1;
	cci.bVisible = FALSE;	        // �Ƿ���ʾ���
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
	// ��������
	COORD loc;
	loc.X = x * 2;               // x��ֵ��Wide��2��
	loc.Y = y;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), loc);
	printf(pszChar); //��ӡʱ��Ҫע����Щ�����ַ���ռ�����ֽ�
}