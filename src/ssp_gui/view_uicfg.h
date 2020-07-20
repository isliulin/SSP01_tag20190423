#ifndef VIEW_UICFG_H
#define VIEW_UICFG_H

#include <QWidget>
#include "ui_view_uicfg.h"
#include "ssp_baseview.h"
#include "ssp_datawindow.h"
#include "mtreewidget.h"
#include "mcombobox.h"
#include "mpushbutton.h"
#include "record_sql.h"
#include "clearData.h"
#include "ssp_gui.h"
#include "extattributedlg.h"

class view_uicfg;
////////////////////  class UicfgWndDelegate  /////////////////////////
class UicfgWndDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	UicfgWndDelegate(QObject *parent, int no);
	~UicfgWndDelegate();

	QWidget * createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const;
	void updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex & index) const;
	void setEditorData(QWidget *editor,const QModelIndex &index) const;
	void setModelData(QWidget *editor,QAbstractItemModel *model,const QModelIndex &index) const;
private:
	view_uicfg *parent;
	SDatabaseOper *pDb;
	int colno;
};
struct stUicfgWnd
{
	QString wnd_sn;
	QString wnd_name;
	QString create_author;
	QString create_time;
	QString modify_author;
	QString modify_time;
	QString wnd_type;
	QString svg_file;
	QString dataset;
	QString ds_cond;
	QString refresh_sec;
	QString cls_name;
};
///��Ӧϵͳ����-�����ܹ���-����̬�������ù��ܵ�
class view_uicfg : public CBaseView
{
	Q_OBJECT

public:
	view_uicfg(QWidget *parent = 0);
	~view_uicfg();
	bool InitWidget(SString sWname);//��ʼ������
public slots:
	void on_pushButton_pressed();//����
	void on_pushButton_2_pressed();//ɾ��
	void on_tableWidget_itemDoubleClicked(QTableWidgetItem * item);
	void on_tableWidget_itemChanged(QTableWidgetItem * item);
	void on_tableWidget_itemSelectionChanged();
	void on_tableWidget_currentItemChanged ( QTableWidgetItem * current, QTableWidgetItem * previous );
	int getMaxIndex();
	void OnPreShow();
	void onPushButton_copy();
	void initCombo();
	//20160815 �ֵ���
public slots:
	void refreshTable(QString strClsName);
public:
	void initTable();
	//ˢ�����ݼ������ƺ�����ӳ���ϵ
	void refreshDataAndReportSetMap();
	//����һ������
	void insertNewRow(stUicfgWnd* stUW,int curRow);
	//��ʼ�����л������ݵ�ӳ���ϵ
	void initDataInfoMap();
	QString getItemValue(int row, int col);
	int	getRowByPushButton(QObject *object);
	void addTableWidgetOperItem(int row);

	bool getCheckBoxStateAndComBoxText(QString& strComBoxText);
	QMap<QString,QString> reportMap;//�������ӳ���ϵ
	QMap<QString,QString> wndTypeMap;//��������ӳ���ϵ
	QMap<QString,QString> dataSetMap;//���ݼ���������ƺ�����ӳ���ϵ

private slots:
	void slotupLoadRowClicked();
	void slotDownLoadRowClicked();
	void slotEditorRowClicked();
private:
	Ui::view_uicfg ui;
	SString m_tableName;//����
	int m_tableRow;//�����ϲ����е�����
	SDatabaseOper *m_dbOper;//���ݴ��ڹ������
	QWidget * m_widget;
	SRecordset* m_Record;
	record_sql m_sqlRecord;//���
	ssp_gui* m_sspgui;
	DynamicSSvgEditorWnd * m_svgEditWnd;//svg�༭�Ի���
	bool eventFilter(QObject *obj, QEvent *event);//�¼�����
};

#endif // VIEW_UICFG_H
