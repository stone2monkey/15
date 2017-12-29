#include "stdafx.h"
#include "Encode.h"


CEncode::CEncode()
{
}


CEncode::~CEncode()
{
}

void CEncode::Encode(char * const & str, int iLen, int iKey)
{
	//首字符串UINT加密其后数据, 再kye加密所有数据

	int iCanEncodeCount = iLen / 4;		// 可加密的数据个数

	// 0. 首字符串UINT加密其后数据
	for (int i = 1; i < iCanEncodeCount; i++)
	{
		((UINT*)str)[i] ^= ((UINT*)str)[0];
	}

	// 1. kye加密所有数据
	for (int i = 0; i < iCanEncodeCount; i++)
	{
		((UINT*)str)[i] ^= iKey;
	}
}

void CEncode::Decode(char * const & str, int iLen, int iKey)
{
	// 先用首Key解密所有数据, 再用首字符串UINT解密其后数据

	int iCanEncodeCount = iLen / 4;		// 可加密的数据个数

	// 0. kye解密所有数据
	for (int i = 0; i < iCanEncodeCount; i++)
	{
		((UINT*)str)[i] ^= iKey;
	}

	// 0. 首字符串UINT解密其后数据
	for (int i = 1; i < iCanEncodeCount; i++)
	{
		((UINT*)str)[i] ^= ((UINT*)str)[0];
	}
}
