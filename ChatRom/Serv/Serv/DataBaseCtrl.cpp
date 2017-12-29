#include "stdafx.h"
#include "DataBaseCtrl.h"


CDataBaseCtrl::CDataBaseCtrl()
{
	//��ʼ��COM���
	CoInitialize(NULL);
	m_pConnect.CreateInstance(__uuidof(Connection));
}


CDataBaseCtrl::~CDataBaseCtrl()
{
	//�ͷ�COM���
	m_pConnect->Close();
	CoUninitialize();	
}

bool CDataBaseCtrl::LinkDataBase(string server, /*����IP */ 
	                             string DBName, 
	                             string UseName, 
	                             string PassWord)
{
	//����ODBC���ݿ�
	// �򿪿������->������->ODBC ����Դ(64 λ / 32λ) 
	// -> ѡ��(�û�DSN)->���->MySQL ODBC5.3(ANSI/Unicode)Driver -> (�û���,���룬���ݿ�)
     
	 

	try
	{
		//�������ݵ�����
		//�������ϴ����ļ� 1.udl,ֱ�Ӵ� ->�ṩ����->Microsoft OLE DB Provider for ODBC Drivers
		/*_bstr_t strCnn("Provider = MSDASQL.1; Password = 123456; Persist Security Info = True; \
                        User ID = root; Data Source = Tank_Net; Initial Catalog = tankinfo_net");
        */
    
		////��һ�ַ�ʽ
		//char strCnn[MAX_PATH] = {};
		//sprintf_s(strCnn, MAX_PATH, "Provider = MSDASQL.1; Password = %s; Persist Security Info = True; \
        //          User ID = %s; Data Source = %s; Initial Catalog = %s",
		//	            PassWord.c_str(),
		//				UseName.c_str(), 
		//				server.c_str(), 
		//				DBName.c_str());


		//m_pConnect->Open(strCnn,_bstr_t(""),_bstr_t(""), adConnectUnspecified);

		
		//�ڶ��ַ�ʽ
		//ʹ��Open�����ݿ⽨������(ʹ��udl�ļ�)
		m_pConnect->Open(_bstr_t("FILE Name=0.udl"),  //udl�����ļ�
			_bstr_t(""),                     //�˺�
			_bstr_t(""),                     //����
			adConnectUnspecified);           //�������ӵķ�ʽ
		
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
		m_pConnect->Execute(_bstr_t(sql.c_str()), //ִ��SQL���
			                 &Record,             //������Ӱ�������
			                 adCmdText);          //������������ķ���
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

		//ָʾҪ�����ṩ�߷����������ı�
		pComm->CommandText = _bstr_t(sql.c_str());
		//ָʾָ���� Command��Recordset �� Record ����ǰ������ Connection ����
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
		pRecordset->Open(_bstr_t(sql.c_str()),            //SQL ���
			             m_pConnect.GetInterfacePtr(),    //����Դ�����ӵ���Ϣ,����ָ���ж���ķ���
			             adOpenStatic,                    //�α�����
			             adLockOptimistic,                //����������������
			             adCmdText);                      //������������ķ���

		//1.ȡ����
		Fields * pFields = pRecordset->GetFields();
		long lCount      = pFields->GetCount();  //��ȡ�ֶε�����
		if (lCount == 0)
		{
			return false;
		}
            
		for (long i = 0;i<lCount;i++)
		{
			//ȡ��ÿ���ֶε�����
			FieldPtr pFieldPtr = pFields->GetItem(i);
			string colName     = pFieldPtr->GetName();
			re.vec_CloName.push_back(colName);
		}

		//2.��ȡ����(�����α�λ��)
		while (!pRecordset->GetadoEOF())
		{
			vector <UINT> iteme;
			for (long i = 0;i<lCount;i++)
			{
				//ͨ����Ż��ֶ�����ȡ��ǰ���е�����
				_variant_t value = pRecordset->GetCollect(i);
				if (value.vt == VT_NULL)
				{
					iteme.push_back(0);
					continue;
				}
				
				iteme.push_back(value);
			}

			re.vec_Data.push_back(iteme);
			pRecordset->MoveNext();    //�ƶ��α�
		}
	}

	catch (_com_error&e)
	{
		MessageBox(0, e.ErrorMessage(), 0, 0);
		return false;
	}
	return true;
}
