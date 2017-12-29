#pragma once
class CPack
{
	// Attributions ***************************************************************************************
public:
	char* m_szFullFileName{};


	// Methods ********************************************************************************************
public:
	void Init(char* szFullFileName);
	bool OnPack();


public:
	CPack();
	~CPack();
};

