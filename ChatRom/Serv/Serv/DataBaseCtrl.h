#pragma once
#include <string>
#include <vector>
using std::string;
using std::vector;

//导入库
//#import "C:\\Program Files\\Common Files\\System\\ado\\msador15.dll" \
//          no_namespace rename("EOF","adoEOF")


#import "C:\\Program Files (x86)\\Common Files\\System\\ado\\msado15.dll" no_namespace rename ("EOF","adoEOF")


//#include "msador15.tli"


//保存表信息
struct _RESULT
{
    vector<string> vec_CloName{};  //列名
    vector<vector<UINT>> vec_Data{};     //数据
};


class CDataBaseCtrl
{
public:
	CDataBaseCtrl();
	~CDataBaseCtrl();

public:
	//连接数据库
	bool LinkDataBase(string server,  //数据IP
		              string DBName,
		              string UseName,
		              string PassWord);

	//发送SQL语句
	bool ExecSqlByConnect(string sql);
	bool ExecSqlByCommand(string sql);
	bool ExecSqlByRecordSet(_RESULT& re, string sql);

	////处理返回的记录集
	//bool ParseRecordSet(_TESULT& re,_RecordsetPtr pRecCordset);

private:
    _ConnectionPtr m_pConnect;

};

