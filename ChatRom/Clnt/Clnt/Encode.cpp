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
	//���ַ���UINT�����������, ��kye������������

	int iCanEncodeCount = iLen / 4;		// �ɼ��ܵ����ݸ���

	// 0. ���ַ���UINT�����������
	for (int i = 1; i < iCanEncodeCount; i++)
	{
		((UINT*)str)[i] ^= ((UINT*)str)[0];
	}

	// 1. kye������������
	for (int i = 0; i < iCanEncodeCount; i++)
	{
		((UINT*)str)[i] ^= iKey;
	}
}

void CEncode::Decode(char * const & str, int iLen, int iKey)
{
	// ������Key������������, �������ַ���UINT�����������

	int iCanEncodeCount = iLen / 4;		// �ɼ��ܵ����ݸ���

	// 0. kye������������
	for (int i = 0; i < iCanEncodeCount; i++)
	{
		((UINT*)str)[i] ^= iKey;
	}

	// 0. ���ַ���UINT�����������
	for (int i = 1; i < iCanEncodeCount; i++)
	{
		((UINT*)str)[i] ^= ((UINT*)str)[0];
	}
}
