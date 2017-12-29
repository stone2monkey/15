#pragma once
class CEncode
{
public:
	CEncode();
	~CEncode();

	void Encode(char*const& str, int iLen, int iKey = 0x12345678);
	void Decode(char*const& str, int iLen, int iKey = 0x12345678);
};

