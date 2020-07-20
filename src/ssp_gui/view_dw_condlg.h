#ifndef VIEW_DW_CONDLG_H
#define VIEW_DW_CONDLG_H

#include <QDialog>
#include <QtGui>
#include "ui_view_dw_condlg.h"
#include "ssp_baseview.h"
#include "ssp_database.h"
#include "ssp_datawindow.h"
#include "SSvgWnd.h"
struct stuDwCondition;
class view_dw_condlg : public QDialog
{
	Q_OBJECT

public:
	view_dw_condlg(QWidget *parent = 0);
	~view_dw_condlg();
	bool refreshTable(int Sno,QString &err);
	bool refreshTableByName(SString name,QString &err);
	SPtrList<stuDwCondition>* getListDwCon();
	bool isOk();
	bool getisQueryNecessary();
	bool isConditionEmpty();
	QString getConStr(){return conStr;}
private:
	QMap<int,QString> conMap;//��¼������ѯ�ʶ�Ӧ��ϵ
	Ui::view_dw_condlg ui;
	QTableWidget *m_table;
	QPushButton *selAllButton;
	QPushButton *calAllButton;
	QPushButton *serButton;
	QPushButton *cancelButton;
	void initTable();
	SDatabaseOper *pDb;
	int m_iNo;
	QString conStr;
	SPtrList<stuDwCondition> m_consets;
	QDate getDatebyDef(QString defValue);
	QDateTime getDateTimebyDef(QString defValue);
	bool isOkey;//�ж�ѡ�����ȷ������ȡ��
	bool isQueryNecessary;//�Ƿ��������������
	bool isEmpty;//��ѯ�����Ƿ�Ϊ�գ�
	void closeEvent(QCloseEvent *event);
private slots:
	void onselAllButton();
	void oncalAllButton();
	void onSerButton();
	void onCancelButton();
	void onItemClicked(QTableWidgetItem *item);
	void onInTableItemClicked(QTableWidgetItem *item);
	void dynConditionChanged();
};

#endif // VIEW_DW_CONDLG_H
