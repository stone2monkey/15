#pragma once


class CMyModuleData
{
	// Attributions ***************************************************************************************
public:
	WCHAR* m_wcsName{};		// ģ����
	HMODULE m_hMod{};		// ģ�����ڴ��VA,Ҳ���Ǽ��ػ�ַ
	DWORD m_dwBaseOfDll{};
	DWORD m_dwSizeOfImage{};
	DWORD m_dwEntryPoint{};




	// Methods ********************************************************************************************
public:
	CMyModuleData();
	~CMyModuleData();
};

