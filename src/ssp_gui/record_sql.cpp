#include "record_sql.h"
#include "include_macdef.h"
#include "QMessageBox"
record_sql::record_sql(void)
{
}


record_sql::~record_sql(void)
{
}

bool record_sql::exec(/*SDatabase * database*/)
{
	//BOOLCHECKRETURN(database->TestConnect(),QObject::tr("���ݿ�����ʧ�ܣ�"),);
	SDatabaseOper *pDb = DB;
	for (int i = 0;i<(int)m_updateList.size();i++)
	{
		if(!pDb->ExecuteSQL(m_updateList[i]))
		{
			QMessageBox::information(NULL,"tip",QObject::tr("ִ�и���ʧ��,�������!"));
			m_updateList.clear();
			m_deltList.clear();
			m_insertList.clear();
			return false;
		}
	}
	m_updateList.clear();
	for (int i = 0;i<(int)m_deltList.size();i++)
	{
		if(!pDb->ExecuteSQL(m_deltList[i]))
		{
			QMessageBox::information(NULL,"tip",QObject::tr("ִ��ɾ��ʧ��,�������!"));
			m_deltList.clear();
			m_insertList.clear();
			return false;
		}
	}
	m_deltList.clear();
	for (int i = 0;i<(int)m_insertList.size();i++)
	{
		if (!pDb->ExecuteSQL(m_insertList[i]))
		{
			QMessageBox::information(NULL,"tip",QObject::tr("ִ�����ʧ��,�������!"));
			m_insertList.clear();
			return false;
		}
	}
	m_insertList.clear();
	return true;
}
void record_sql::clear()
{
	m_insertList.clear();
	m_deltList.clear();
	m_updateList.clear();
}
void record_sql::addSql(SString sSql,SQLTYPE type)
{
	switch(type)
	{
	case 0:
		m_insertList.push_back(sSql);
		break;
	case 1:
		m_deltList.push_back(sSql);
		break;
	case 2:
		m_updateList.push_back(sSql);
		break;
	}
}