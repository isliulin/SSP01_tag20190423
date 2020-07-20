#ifndef view_dw_condition_edit_H
#define view_dw_condition_edit_H

#include <QWidget>
#include "ui_view_datawindow_edit.h"
#include <QtGui>
#include "ui_view_dw_condition_edit.h"
#include "ssp_baseview.h"
#include "ssp_database.h"
#include "ssp_datawindow.h"
#include "SSvgWnd.h"
class view_dw_condition_edit : public CBaseView
{
	Q_OBJECT

public:
	view_dw_condition_edit(QWidget *parent = 0);
	~view_dw_condition_edit();
	QTableWidget *getTableData();
	void refreshTable(int Sno, QString strDataComBoxText, bool bCheckState);
	SRecordset m_refList;
	//ˢ����������
	void refreshDataRef();
	QMap<QString, QString> m_dataRefMap;
	void setComboTextAndCheckState(QString strComText, bool bIsChecked);
	QString m_strComText;//�������ݼ�-�����ݼ�����combox��Ӧ�ĵ�ǰ������
	bool    m_bIsChecked;//�����������ѡ��checkbox��״̬
private:
	Ui::view_dw_condition_edit ui;
	QTableWidget *m_table;
	QPushButton *addButton;
	QPushButton *removeButton;
	QPushButton *insertButton;
	SDatabaseOper *pDb;
	QMap<QString, QString> m_inputTypeMap;
	QMap<QString, QString> m_cmpTypeMap;
	

	int m_iNo;//dn_sw
	bool insertRow(int rowNum);
	bool insertAppointRow(int rowNum);
	void initTable();
	QString numToconType(int conType);
	QString numToinputType(int inputType);
	//��ʼ������������
	void initBaseData();


private slots:
	void onAddButton();
	void onRemoveButton();
	void onInsertButton();
	void editItem(QTableWidgetItem *item);
	//��Ӧ����ѡ��ĳ�д����¼�
	void slotClickSel(QTableWidgetItem *item,QTableWidgetItem *pre);
};
//////////////////////////////////////////////////////////////////////////

class dwconditionDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	dwconditionDelegate(QObject *parent = 0);

	virtual QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
	virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

private:
	view_dw_condition_edit *parent;
	int curRow;
	int curCol;
	private slots:
		void comboItemChanged(QString text);
};

/////////////////////////////////////////
#endif // view_dw_condition_edit_H
