#ifndef VIEW_DATAWINDOW_EDIT_H
#define VIEW_DATAWINDOW_EDIT_H

#include <QWidget>
#include "ui_view_datawindow_edit.h"
#include <QtGui>
#include "ssp_baseview.h"
#include "ssp_database.h"
#include "ssp_datawindow.h"
#include "SSvgWnd.h"
#include "view_common.h"
#include "view_query.h"
#include "view_dw_condition_edit.h"
class inputNum;
class review;
struct myRow
{
	SString name;
	SString id;
};
struct myRef
{
	SString name;
	SString dbtype;
	SString dw_desc;
	SString dw_select;
	SString cls_name;
	SPtrList<myRow> myRowList;
};
struct myCondition
{
	SString con_name;
	int is_option;
	int cmp_type;
	int input_type;
	SString ref_name;
	SString def_value;
	SString dyn_where;
};
struct myCol
{
	SString col_name;
	SString col_vtype;
	int col_width;
	SString ref_name;
	SString conv;
	int pkey;
	int isnull;
	int ukey;
	SString ext_attr;
};
struct myDataset
{	
	int dstype;
	SString name;
	int dbtype;
	SString dw_desc;
	SString dw_select;
	int transform_type;
	SString cross_refname;
	int cross_column;
	int cross_data_column;
	SString cls_name;
	SString ext_attr;
	SPtrList<myCol> myColList;
	SPtrList<myCondition> myConditionList;
};
class view_datawindow_edit : public CBaseView
{
	Q_OBJECT

public:
	view_datawindow_edit(QWidget *parent = 0);
	~view_datawindow_edit();

friend class dataWindowDelegate;
friend class leftTableRefWindowDelegate;
friend class leftTableDataWindowDelegate ;
private:
	SDatabaseOper *pDb;
	void initRefCombo();
	void initDataCombo();
	Ui::view_datawindow_edit ui;
	SRecordset m_rsLeftTable;//������Ϣ
	SRecordset m_rsCol;//���ݼ�����
	SRecordset m_rsRow;//��������
	void refreshLeftTable();//ˢ������
	SString typeTransform(SString str);//ת�������ַ�
	SString valuetypeTransform(SString str);//ת�������ַ�
	SString refnameTransform(SString str,SRecordset &datasets );//ת�����������ַ�
	SString tranftranftype(SString str);
	SString convStrToName(SString str);
	SString convNameToStr(SString name);
	//��ȡt_ssp_data_windows����dw_sn�ֶε����ֵ
	int getMaxDwSn();
	//����һ���������������
	bool handleOneRefNode(int& maxDwsn, myRef* curRefSet);
	//����һ���������������ݵ����ݿ�
	bool insertOneRefSetInfo(int maxDwsn, myRef* curRefSet);
	//����һ����������ݼ�����
	bool handleOneDataNode(int& maxDwsn, myDataset* curDataSet);
	//����һ�����������ݼ����ݵ����ݿ�
	bool insertOneDataSetInfo(int maxDwsn, myDataset* curDataSet);
	QTableWidget  *getTableData2();
	QTableWidget  *getTableData1();
	QTableWidget  *getTableRef1();
	QString getCls();
	void getComboxAndCheckState(QString& strComboText, bool& bIsChecked);

	int m_countMax;//���ڼ�¼sn��ÿ�ζ�ȡ���ֵ��ȷ��snֵΨһ
	char* checkNull(char* num);//����Ƿ�Ϊ��
	inputNum *m_inputDlg;
	view_dw_condition_edit *viewDwConEdit;
private:
	//��������ҳ��table��qtexteditor�ؼ��Ƿ������ź�
	void setRefBlockSignals(bool blcok);
	//�������ݼ�ҳ��table��qtexteditor�ؼ��Ƿ������ź�
	void setDataBlockSignals(bool blcok);

private slots:
	void onPushButton_add();
	void onPushButton_copy();
	void onPushButton_del();
	void onPushButton_view();
	void onPushButton_import();
	void onPushButton_export();
	void onPushButton_apply();

	void onPushButton_ref2_add();
	void onPushButton_ref2_del();
	void onPushButton_data2_add();
	void onPushButton_data2_del();
	void onPushButton_newcol();

	void showRefInfo(QTableWidgetItem *item,QTableWidgetItem *pre);
	void showDataInfo(QTableWidgetItem *item,QTableWidgetItem *pre);
	void showConInfo(QTableWidgetItem *item,QTableWidgetItem *pre);
	void ref2ItemCilcked(QTableWidgetItem *item,QTableWidgetItem *pre);
	void data2ItemCilcked(QTableWidgetItem *item,QTableWidgetItem *pre);
	void editRef1(QTableWidgetItem *item);
	void editRef2(QTableWidgetItem *item);
	void editData1(QTableWidgetItem *item);
	void editData2(QTableWidgetItem *item);
	void editRefText();
	void editDataText();
	void refCombo(QString text);
	void dataCombo(QString text);

	void pasteCfg();
	void tabChanged(int num);
	void slotOnCheckBoxClick();
};
////////////////////////////////////////////////////

//��Ӧϵͳ����-�����ݴ�������-������������ť��Ӧ�����ĶԻ���
class OutputCfg : public QDialog
{
	Q_OBJECT

public:
	OutputCfg(QWidget *parent = 0);
	~OutputCfg();
private:
	QGridLayout *gridLayout_2;
	QVBoxLayout *verticalLayout;
	QTableWidget *tableWidget;
	QWidget *widget;
	QGridLayout *gridLayout;
	QPushButton *pushButton;
	QPushButton *pushButton_2;
	QSpacerItem *horizontalSpacer;
	QGridLayout *gridLayout1;
	QGridLayout *gridLayout2;
private:
	SDatabaseOper *pDb;
	SRecordset m_rsLeftTable;//������Ϣ
	SRecordset m_rsCol;//���ݼ�����
	SRecordset m_rsRow;//��������
	SRecordset m_rsCondition;//�����м�

	SString tranftype(SString str);//����
	SString tranfdbtype(SString str);
	SString tranftranftype(SString str);
	void setupUi(QWidget *OutputCfg);//����
	void retranslateUi(QWidget *OutputCfg);
private slots:
	void onPushButtonPressed();//ȷ��
	void onPushButton2Pressed();//ȡ��
	void DoubleClickedTable(int col);//��ѡȫѡ
};

//////////////////////////////////////////////////////////////////////////

class dataWindowDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	dataWindowDelegate(QObject *parent = 0);

	virtual QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
	virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

private:
	view_datawindow_edit *parent;
	int curRow;
	int curCol;
	SDatabaseOper *pDb;

	private slots:
		void comboItemChanged(QString text);
};

/////////////////////////////////////////

class leftTableRefWindowDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	leftTableRefWindowDelegate(QObject *parent = 0);

	virtual QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
	virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

private:
	view_datawindow_edit *parent;
	int curRow;
	int curCol;
	private slots:
		void comboItemChanged(QString text);
};

/////////////////////////////////////////

class leftTableDataWindowDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	leftTableDataWindowDelegate(QObject *parent = 0);

	virtual QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
	virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

private:
	view_datawindow_edit *parent;
	int curRow;
	int curCol;
	private slots:
		void comboItemChanged(QString text);
};

/////////////////////////////////////////
//��Ӧϵͳ����-�����ݴ�������-�������ơ���ť��Ӧ����������Ի���
class inputNum : public QDialog
{
	Q_OBJECT

public:
	inputNum(QWidget *parent = 0);
	~inputNum();
	int getNum();

private:
	QGridLayout *gridLayout;
	QVBoxLayout *verticalLayout;
	QHBoxLayout *horizontalLayout_2;
	QLabel *label;
	QLineEdit *lineEdit;
	QWidget *widget;
	QHBoxLayout *horizontalLayout;
	QSpacerItem *horizontalSpacer;
	QPushButton *pushButton;
	QPushButton *pushButton_2;
private:
	void setupUi(QDialog *inputNum);
	void retranslateUi(QDialog *inputNum);
signals:
	void finishInputNum();
private slots:
	void onPushButtonPressed();
	void onPushButton2Pressed();
};
/////////////////////////////////////////
//��Ӧϵͳ����-�����ݴ�������-����Ԥ������ť��Ӧ�����ĶԻ���
class review : public QDialog
{
	Q_OBJECT

public:
	review(QWidget *parent = 0);
	~review();
	QGridLayout *gridLayout;
	QTabWidget *tabWidget;
	QWidget *widget;
	QGridLayout *gridLayout_3;
	QGridLayout *gridLayout_2;
	QWidget *widget_2;
	QGridLayout *gridLayout_5;
	QGridLayout *gridLayout_4;
private:
	void setupUi(QDialog *review);
	void retranslateUi(QDialog *review);
};
#endif // VIEW_DATAWINDOW_EDIT_H
