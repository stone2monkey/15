#pragma once

typedef struct MD5VAL_STRUCT
{
	unsigned int a;
	unsigned int b;
	unsigned int c;
	unsigned int d;
} MD5VAL;




class CMD5
{
// Attributions ***************************************************************************************
public:




	// Methods ********************************************************************************************
public:
	//�����ַ�����MD5ֵ(����ָ���������ɺ�������)
	MD5VAL md5(char * str, unsigned int size = 0);

	//���ļ�ָ��,�����MD5ֵ,ֻ����10M��С
	MD5VAL md5File(FILE * fpin);

	//���ؼ��ܺ��MD5�ַ���
	char * md5FileValue(char * fname);

	//MD5�ַ�������,��conv����
	void md5Str(char * input, char * output);

















public:
	CMD5();
	~CMD5();
};

