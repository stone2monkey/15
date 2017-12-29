#pragma once
#include <string>
#include <vector>
using std::string;
using std::vector;

//�����
//#import "C:\\Program Files\\Common Files\\System\\ado\\msador15.dll" \
//          no_namespace rename("EOF","adoEOF")


#import "C:\\Program Files (x86)\\Common Files\\System\\ado\\msado15.dll" no_namespace rename ("EOF","adoEOF")


//#include "msador15.tli"


//�������Ϣ
struct _RESULT
{
    vector<string> vec_CloName{};  //����
    vector<vector<UINT>> vec_Data{};     //����
};


class CDataBaseCtrl
{
public:
	CDataBaseCtrl();
	~CDataBaseCtrl();

public:
	//�������ݿ�
	bool LinkDataBase(string server,  //����IP
		              string DBName,
		              string UseName,
		              string PassWord);

	//����SQL���
	bool ExecSqlByConnect(string sql);
	bool ExecSqlByCommand(string sql);
	bool ExecSqlByRecordSet(_RESULT& re, string sql);

	////�����صļ�¼��
	//bool ParseRecordSet(_TESULT& re,_RecordsetPtr pRecCordset);

private:
    _ConnectionPtr m_pConnect;

};

