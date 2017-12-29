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
	//计算字符串的MD5值(若不指定长度则由函数计算)
	MD5VAL md5(char * str, unsigned int size = 0);

	//给文件指针,计算出MD5值,只计算10M大小
	MD5VAL md5File(FILE * fpin);

	//返回加密后的MD5字符串
	char * md5FileValue(char * fname);

	//MD5字符串加密,用conv方法
	void md5Str(char * input, char * output);

















public:
	CMD5();
	~CMD5();
};

