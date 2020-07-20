/**
 *
 * �� �� �� : view_nocommon.h
 * �������� : 2015-7-30 15:35
 * ��    �� : �ұ���
 * �޸����� : 2015-7-30 15:35
 * ��ǰ�汾 : 1.0
 * �������� : �ض�����ʵ��
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-7-30 	�ұ������ض�����ʵ��
 *
 **/

#ifndef VIEW_NOCOMMON_H
#define VIEW_NOCOMMON_H

#include <QWidget>
#include "ui_view_nocommon.h"
#include "ssp_baseview.h"
#include "db/mysql/SMySQL.h"
#include "dbInteraction.h"
#include "include_macdef.h"
#include "structDefine.h"
#include "ssp_datawindow.h"
#include "mtreewidget.h"
#include "mcombobox.h"
#include "record_sql.h"
#include "clearData.h"

class ViewNoCommonPlugin;
class view_nocommon : public CBaseView,public clearData
{
	Q_OBJECT

public:
	view_nocommon(QWidget *parent = 0,SString sFunName="");
	~view_nocommon();
	bool InitWidget(SString sWname);//��ʼ����ͬ�ķ�ͨ�ô���
	void InitDb();//���ݿ��ʼ��
	SString PrepareForFunPoint(SString sTop);//׼�����ܵ����ݱ�(�մ�������)�����ҷ��ض���ڵ�
	void SetAsFunPoint();//��ʼ��Ϊϵͳ���ܵ�
	void SetAsGrtUsr();//��ʼ��Ϊ�û�Ȩ��
	void SetAsGrtGrp();//��ʼ��ΪȺ��Ȩ��
	virtual void OnPreShow();
	virtual void OnPreHide();
	void GetSysFunName(QVector<QString> &funList);

protected:
	virtual void dragEnterEvent(QDragEnterEvent *event);
	virtual void dropEvent(QDropEvent *event);

public slots:
	void OnAddNode(QTreeWidgetItem * node,QTreeWidgetItem *parentNode);
	void OnDelNode(QTreeWidgetItem * node,QTreeWidgetItem *parentNode);
	void OnEditNode(QTreeWidgetItem * node,QTreeWidgetItem *parentNode);
	void OnUpdateNode(QTreeWidgetItem * node,SString sPre,int column);

	void OnListItemChoosed_group(QListWidgetItem *item);
	void OnUpdateCheck_group(QTreeWidgetItem* item,int column);

	void OnListItemChoosed_user(QListWidgetItem *item);
	void OnUpdateCheck_user(QTreeWidgetItem* item,int column);
private:
	Ui::view_nocommon ui;
	SString m_funName;//��������
	SString m_tableName;//����
	SDatabaseOper *m_pOper;//���ݿ��������
	pcell_CTree<SString> *m_tree;
	SRecordset* m_Record;
	record_sql m_sqlRecord;//���
	dbInteraction m_dbInter;
	QListWidgetItem *m_choosedItem;//��ǰ�б�ѡ����
	bool m_bconnect;
	SString m_funTop;//���㹦�ܵ�
	vector<SString> m_funFields;//���ܵ���ֶ�
	ViewNoCommonPlugin *m_viewNoCommonPlugin;
};

#endif // VIEW_NOCOMMON_H
