#pragma once


class CMyModuleData
{
	// Attributions ***************************************************************************************
public:
	WCHAR* m_wcsName{};		// 模块名
	HMODULE m_hMod{};		// 模块在内存的VA,也就是加载基址
	DWORD m_dwBaseOfDll{};
	DWORD m_dwSizeOfImage{};
	DWORD m_dwEntryPoint{};




	// Methods ********************************************************************************************
public:
	CMyModuleData();
	~CMyModuleData();
};

