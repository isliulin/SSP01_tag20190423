//���ݿ�����潻����
#ifndef _DBINTERACTION 
#define _DBINTERACTION
#include "SApi.h"
#include "SFile.h"
#include "db/mdb/SMdb.h"
#include "ManConnect.h"
#include "Rsignal.h"
#include <vector>
typedef int SQLTYPE;
typedef struct sendInfo
{
	SString sTable;
	eMdbTriggerType eType;
	int iTrgRows;
	int iRowSize;
	BYTE *pTrgData;
	SString sDate;
	SString sTime;
public:
	~sendInfo()
	{
		if (pTrgData!=NULL)
		{
			delete pTrgData;
			pTrgData =NULL;
		}
	}
}sendInfo;

struct saveRecord
{
	int mRow;
	int mColum;
	SString Ssql;
	SQLTYPE mType;//������� 0��insert 1:delete 2:update
};
class dbInteraction:public ManConnect
{
public:
	dbInteraction(void);
	~dbInteraction(void);
	static BYTE * testcall(void* cbParam, SString &sTable, eMdbTriggerType eType,int iTrgRows,int iRowSize,BYTE *pTrgData);
	int execSql(SString s_sql);
	bool manageSql(SString s_sql);//����������䣬����treewidget�Ƿ����
	template<class Type>Type getField(SString s_table,Type &l)
	{
		if (!curDb->TestConnect())
		{
			sRet = "����ʧ��";
			return l;
		}
		for (int i = 0;i<i_colum;i++)
		{
			l.push_back(rs.GetColumnName(i).data());
		}
		return l;
	}
	void setDbParam(SString sParam);
	void create(SString sParam="");//�������ݿ�ʵ��
	bool connect();//����
	void add();//������ݿ����Ӷ���������
	void addcmd(SString sTable,int row,int colum,SString sValue,SQLTYPE type,SString sRow);//���ִ���������
	bool isCmdExec();//�ж�����Ƿ�ȫִ��
	void clearRecord()
	{
		recordList.clear();
	}
	void runRecord();//ִ�����������
	void del(int i_idx);//��������ɾ��
	void pointTo(int i_idx);//ָ��ǰʵ��
	SString getValue(int row,int colum){return rs.GetValue(row,colum);}
	int getColum(){return i_colum;};
	int getRow(){return i_row;};
	bool delTable(SString tableName);
	void registCallback(OnMdbClientTrgCallback pFun, void *cbParam, SString sTableName, BYTE iTriggerFlag);
	void removeCallback(OnMdbClientTrgCallback pFun, void *cbParam, SString sTableName, BYTE iTriggerFlag);
	SString update(SString sTablename,SString rowData,int colum)
	{
		SString sRet;
		int i_row;
		SPtrList<stuTableField> l_fields;
		curDb->GetMdbClient()->GetTableFields(sTablename.data(),l_fields);
		SString sSql = "update "+ sTablename + " set ";
		bool b=false;
		for (int i = 0;i<l_fields.count();i++)
		{
			if (i!=colum&&b)
			{
				sSql+= l_fields[i]->name;
				sSql+= "=";
				switch(l_fields[i]->type)
				{
				case 6:
					sSql+="'";
					sSql+=SString::GetIdAttribute(i+1,rowData,",");
					sSql+="'";
					break;
				default:
					sSql+=SString::GetIdAttribute(i+1,rowData,",");
					break;
				}
					if (colum==l_fields.count()-1)
					{
						if (i!=l_fields.count()-2)
						{
							sSql+=" and ";
						}
					}else
					{
						if (i!=l_fields.count()-1)
						{
							sSql+=" and ";
						}
					}
				
			}else if(i==colum&&!b)
			{
				sSql+= l_fields[i]->name;
				sSql+="=";
				sSql+=SString::GetIdAttribute(colum+1,rowData,",")+" where ";
				b=true;
				i=-1;
			}
		}
		i_row = curDb->Execute(sSql);
		if (i_row<0)
		{
			sRet="����ʧ�ܣ� ";
			sRet+=curDb->GetLastError();
		}else
		{

			sRet="���³ɹ���";
			if (i_row>1)
			{
				sRet="���棺�������ظ���¼!";
			}
		}
		return sRet;
	}
	int getTablefields(SString sTable, SPtrList<stuTableField> &TableFields)
	{
		return curDb->GetMdbClient()->GetTableFields(sTable.data(),TableFields);
	}
	CMdbClient * getDbClient()
	{
		return curDb->GetMdbClient();
	}
	void RunSqlFile(SString filename);//����sql�ļ�
	Rsignal * getRsignal(){return Rs;};
	unsigned int getOID(int row)
	{
		return rs.GetRecord(row)->GetOid();
	}
	SString sRet;//�����ʾ��Ϣ
private:
	SMdb *curDb;//��ǰ���ݿ�ָ��
	//ManConnect mc;//���ݿ�������
	int i_colum;//��
	int i_row;//��
	SRecordset rs;//���ݼ�
	SPtrList<stuTableField> TableFields;//�����ֶ���Ϣ
	vector<saveRecord> recordList;//ִ����伯
	OnMdbClientTrgCallback fp;
	Rsignal *Rs;
};


#endif


