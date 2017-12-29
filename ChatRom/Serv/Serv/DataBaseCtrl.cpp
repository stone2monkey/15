#include "stdafx.h"
#include "DataBaseCtrl.h"


CDataBaseCtrl::CDataBaseCtrl()
{
	//初始化COM组件
	CoInitialize(NULL);
	m_pConnect.CreateInstance(__uuidof(Connection));
}


CDataBaseCtrl::~CDataBaseCtrl()
{
	//释放COM组件
	m_pConnect->Close();
	CoUninitialize();	
}

bool CDataBaseCtrl::LinkDataBase(string server, /*数据IP */ 
	                             string DBName, 
	                             string UseName, 
	                             string PassWord)
{
	//设置ODBC数据库
	// 打开控制面板->管理工具->ODBC 数据源(64 位 / 32位) 
	// -> 选项(用户DSN)->添加->MySQL ODBC5.3(ANSI/Unicode)Driver -> (用户名,密码，数据库)
     
	 

	try
	{
		//下列数据的由来
		//在桌面上创建文件 1.udl,直接打开 ->提供程序->Microsoft OLE DB Provider for ODBC Drivers
		/*_bstr_t strCnn("Provider = MSDASQL.1; Password = 123456; Persist Security Info = True; \
                        User ID = root; Data Source = Tank_Net; Initial Catalog = tankinfo_net");
        */
    
		////第一种方式
		//char strCnn[MAX_PATH] = {};
		//sprintf_s(strCnn, MAX_PATH, "Provider = MSDASQL.1; Password = %s; Persist Security Info = True; \
        //          User ID = %s; Data Source = %s; Initial Catalog = %s",
		//	            PassWord.c_str(),
		//				UseName.c_str(), 
		//				server.c_str(), 
		//				DBName.c_str());


		//m_pConnect->Open(strCnn,_bstr_t(""),_bstr_t(""), adConnectUnspecified);

		
		//第二种方式
		//使用Open与数据库建立连接(使用udl文件)
		m_pConnect->Open(_bstr_t("FILE Name=0.udl"),  //udl配置文件
			_bstr_t(""),                     //账号
			_bstr_t(""),                     //密码
			adConnectUnspecified);           //建立连接的方式
		
	}
	catch (_com_error& e)
	{
		MessageBox(0,e.ErrorMessage(),0,0);
		return false;
	}
	return true;
}

bool CDataBaseCtrl::ExecSqlByConnect(string sql)
{
	try {
		//VARIANT Record;
		_variant_t Record;
		m_pConnect->Execute(_bstr_t(sql.c_str()), //执行SQL语句
			                 &Record,             //返回受影响的行数
			                 adCmdText);          //解释命令参数的方法
	}
	catch(_com_error& e)
	{
		MessageBox(0, e.ErrorMessage(), 0, 0);
		return false;
	}
	return true;
}

bool CDataBaseCtrl::ExecSqlByCommand(string sql)
{
	//

	try {
		_CommandPtr pComm;
		pComm.CreateInstance(__uuidof(Command));

		_variant_t Record;

		//指示要根据提供者发出的命令文本
		pComm->CommandText = _bstr_t(sql.c_str());
		//指示指定的 Command、Recordset 或 Record 对象当前所属的 Connection 对象。
		pComm->ActiveConnection = m_pConnect;

		pComm->Execute(&Record, NULL, adCmdText);

	}
	catch (_com_error&e)
	{
		MessageBox(0, e.ErrorMessage(), 0, 0);
		return false;
	}
	return true;

}

bool CDataBaseCtrl::ExecSqlByRecordSet(_RESULT& re, string sql)
{
	try
	{
		_RecordsetPtr pRecordset;
		pRecordset.CreateInstance(__uuidof(Recordset));
		pRecordset->Open(_bstr_t(sql.c_str()),            //SQL 语句
			             m_pConnect.GetInterfacePtr(),    //数据源的连接的信息,智能指针中定义的方法
			             adOpenStatic,                    //游标类型
			             adLockOptimistic,                //锁定（并发）类型
			             adCmdText);                      //解释命令参数的方法

		//1.取列名
		Fields * pFields = pRecordset->GetFields();
		long lCount      = pFields->GetCount();  //获取字段的数量
		if (lCount == 0)
		{
			return false;
		}
            
		for (long i = 0;i<lCount;i++)
		{
			//取出每个字段的名字
			FieldPtr pFieldPtr = pFields->GetItem(i);
			string colName     = pFieldPtr->GetName();
			re.vec_CloName.push_back(colName);
		}

		//2.读取数据(根据游标位置)
		while (!pRecordset->GetadoEOF())
		{
			vector <UINT> iteme;
			for (long i = 0;i<lCount;i++)
			{
				//通过序号或字段名获取当前行中的数据
				_variant_t value = pRecordset->GetCollect(i);
				if (value.vt == VT_NULL)
				{
					iteme.push_back(0);
					continue;
				}
				
				iteme.push_back(value);
			}

			re.vec_Data.push_back(iteme);
			pRecordset->MoveNext();    //移动游标
		}
	}

	catch (_com_error&e)
	{
		MessageBox(0, e.ErrorMessage(), 0, 0);
		return false;
	}
	return true;
}
