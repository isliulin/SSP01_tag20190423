/**
 *
 * �� �� �� : dbInteraction.h
 * �������� : 2015-7-30 15:50
 * ��    �� : �ұ���
 * �޸����� : 2015-7-30 15:50
 * ��ǰ�汾 : 1.0
 * �������� : ����ض����ݿ��������
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-7-30 	�ұ���������ض����ݿ��������
 *
 **/
#ifndef _DBINTERACTION_H
#define _DBINTERACTION_H
#include <QTreeWidgetItem>
#include <QMap>
#include "db/SDatabase.h"
#include "include_macdef.h"
#include "structDefine.h"
#include "pcellTree.h"
#include "mcombobox.h"
typedef int SQLTYPE;
class dbInteraction
{
public:
	dbInteraction(SDatabaseOper * pDb);
	dbInteraction()
	{
		m_db = NULL;
	}
	~dbInteraction(void);
	void setDb(SDatabaseOper* pDb){m_db = pDb;};
	SDatabaseOper* getDb(){return m_db;}
	//��ȡ���ṹ���е������¼��ڵ�
	//����˵����sTable   ����
	//		    sField   �����ֶ���
	//          sFField  ���ֶ���
	//          sValue   Ҫ�г��Ľڵ㡢��ӦsField
	//          sSelect  Ҫ���ҵ�����
	//
	virtual void addData(SRecordset &rs,SString sData,QTreeWidgetItem *item){S_UNUSED(rs);S_UNUSED(sData);S_UNUSED(item);};
	void GetChildren(SString sTable,SString sField,SString sFField,SString sValue,QTreeWidgetItem*&item);

	//�������͵����ݱ������ݴ洢�����ṹ��
	//����˵����sTable   ����
	//		    sField   �����ֶ���
	//          sFField  ���ֶ���
	//          sValue   Ҫ�г��Ľڵ㡢��ӦsField
	//          sSelect  Ҫ���ҵ�����
	//
	template<class T> void getTreeItem(SString sTable,SString sField,SString sFField,SString sValue,QTreeWidgetItem*&item,T &t_list)
	{
		SRecordset rs;
		SString Ssql = SString::toFormat("select * from %s",sTable.data());
		m_db->Retrieve(Ssql,rs);
		for (int i=0;i<rs.GetColumns();i++)
		{
			if (rs.GetColumnName(i)!=sFField)
			{
				t_list.push_back(rs.GetColumnName(i).data());
			}
		}
		GetChildren(sTable,sField,sFField,sValue,item);
		//return m_tree;
	}
	mCombobox * getRefNum();//��ȡ�����������
	mCombobox * getReportNum();//��ȡ��������
	void setRefIndex(mCombobox *pComb,QString val);//����ֵ��������������õ�����
	void initUicfgAndReportRef();
public:
	SDatabaseOper* m_db;
	SRecordset m_recored;
	QMap<QString,QString> m_uicfgWndMap;//t_ssp_uicfg_wnd
	QMap<QString,QString> m_reportInfoMap;//t_ssp_report_info
};
#endif