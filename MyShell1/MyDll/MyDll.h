#pragma once
#include <Windows.h>


int GetStrLen(char* str);
bool StrCmp(char* str0, char* str1);
DWORD GetCurMod();
DWORD GetKernel32Mod();
DWORD GetFnAddr(DWORD dwMod, char* strFnName);
LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void NakedFn();
void(__stdcall NonNakedFn)();

void RepairBRT(DWORD dwMod);
void RepairIIT(DWORD dwMod);
void PopWnd(HINSTANCE hInstance);
void DecodeExeTextSec(DWORD* pAddr, DWORD dwXorData, DWORD dwTime);