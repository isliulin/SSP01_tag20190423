#ifndef SYS_MDBUI_H
#define SYS_MDBUI_H

#define MAX_SHOW_SIZE 100000

#include <QtGui/QMainWindow>
#include <QDate>
#include <QTime>
#include <queue>
#include <QMessageBox>
#include <QFileDialog>
#include <QSettings>
#include <QTextCodec>
#include <QShortCut>
#include <QScrollBar>
#include <QTimer>
#include <QMap>
#include "ui_sys_mdbgui.h"
#include "ManConnect.h"
#include "RClickMenu.h"
#include "enterdlg.h"
#include "dbInteraction.h"
Q_DECLARE_METATYPE(sendInfo)
BYTE * registFunction(void* cbParam, SString &sTable, eMdbTriggerType eType,int iTrgRows,int iRowSize,BYTE *pTrgData);

class sys_mdbgui : public QMainWindow
{
	Q_OBJECT

public:
	sys_mdbgui(QWidget *parent = 0, Qt::WFlags flags = 0);
	~sys_mdbgui();
	bool b_append;
	//���ʹ���ע�ắ���źţ�ֱ�ӵ���setRegistInfo�������̱߳�����
	void sendTrigSignal()
	{
		emit setInfo();
	}
	int gettableFields(SString sTable, SPtrList<stuTableField> &TableFields)
	{
		return dbinter.getTablefields(sTable,TableFields);
	}
	dbInteraction dbinter;
	SPtrList<sendInfo> m_SendInfo;
	

signals:
	void setInfo();//���ô�����ʾ����ź�
private:
	QString m_curTableName;//��ǰ���������ݿ����ƣ�select *,create,drop 3���﷨�õ���
	int m_index;//��ǰ���ݿ�����
	Ui::sys_mdbguiClass ui;
	bool b_save;//�Ƿ񱣴�
	SMdb *m_mdb;
	ManConnect mc;
	Mmenu *r ;
	QTreeWidgetItem * citem;//��ǰѡ�е���Ŀ
	//dbInteraction dbinter;
	SString s_ip;
	bool b_edit;//�ж�tablewidget�Ǳ��񱻱༭
	int m_curRow;//ԭʼ�б�����
	int m_insert;//���ӵ�����
	bool b_connect;
	QScrollBar * bar;
	int m_TabPageRowN;//��ǰ�б�����
	vector<QString> V_item;
	bool b_first;//�Ƿ��һ�δ�
	int l_reg;//ע���¼�ַ�����
	int l_select;//��ѯ��¼�ַ�����
	int l_operate;//������¼�ַ�����
	QTimer *m_timer;//��ʱ��
	struct tableTofield
	{
		SString stable;
		SPtrList<stuTableField> *lf;
		//QMap<int,unsigned int> *oidMap;//oidӳ��
	};
	vector<tableTofield> m_vfields;//����ص�������ֶ���Ϣ
	QMap<QString,int> m_tableMap;//������Ӧ�ֶ���Ϣ��m_vfields���±�
	bool m_blocal;//�Ƿ��Ǳ��ز���
public slots:
void reloadDb(SString sTable,UPDATETYPE type);
void reloadDb();//ˢ�µ�ǰ���ݿ��б�
void refresh(SString s_table);//ˢ�µ�ǰ��������
void RecordHistory(SString sSql,bool b_select);//����sql�����ʷ��¼ 
void tableInit();
void getValue(int m_value);
void setRegistInfo();//���ô�����ʾ���
void on_treeWidget_itemDoubleClicked( QTreeWidgetItem * item, int column );
void on_treeWidget_customContextMenuRequested(QPoint pos);
void on_treeWidget_currentItemChanged ( QTreeWidgetItem * current, QTreeWidgetItem * previous )
{
	isSave();
	citem = current;
	if (citem==NULL)
	{
		r->setType(Mmenu::NOITEM);
		dbinter.pointTo(-1);
		return;
	}
	if(citem->childCount()==0)
	{
		r->setType(Mmenu::CHILDITEM);
		on_treeWidget_itemDoubleClicked(current,0);
	}else
	{
		m_index = ui.treeWidget->indexOfTopLevelItem(current);
		r->setType(Mmenu::FATHERITEM);
		ui.pushButton_insert->setEnabled(false);
		tableInit();
	}
}
void on_action_triggered ( bool checked = false );//ִ�а�ť��Ӧ�¼�
void on_action_runfile_triggered(bool checked = false);//����sql�ļ�
void on_action_5_triggered ( bool checked = false );//˵����ť��Ӧ�¼�
void on_action_7_triggered ( bool checked = false );//������ť��Ӧ�¼�
void on_action_6_triggered ( bool checked = false );//���ע���¼
void on_action_8_triggered ( bool checked = false );//��ղ�ѯ��¼
void on_action_9_triggered ( bool checked = false );//��ղ�����¼
void on_pushButton_insert_pressed();//���밴ť
void on_pushButton_save_pressed();//���水ť
void on_pushButton_del_pressed();//ɾ����ť
void on_tableWidget_itemSelectionChanged();//��ѡ�仯
void connect();//����
void delDb();//ɾ������
void delTable();//ɾ����
void registCallback();//ע���¼�
void removeRegistCall();//ɾ��ע���¼�
void registAllCallback();//ȫ��ע���¼�
void removeAllRegistCall();//ɾ��ȫ��ע���¼�
void getData(SString sParam);
void execSqltoWidget(QString sql);//ִ�в�ѯ��䲢�ڱ�����ʾ����
void updateData(QTableWidgetItem * item)
{
	SString s;
	SString info;
	if (b_edit)
	{
		//�޸�tablewidget�е�ֵ�����µ�����
		//citem->text(0).toStdString();
		int i_row = item->row();
		int i_colum = item->column();

		if (i_row<m_curRow)
		{
			for (int i=0;i<ui.tableWidget->columnCount();i++)
			{
				s+=ui.tableWidget->item(i_row,i)->text().toLocal8Bit().data();
				s+=",";
			}
			//�޸ĵ���
			//info = dbinter.update(citem->text(0).toStdString(),s,i_colum);
			//ui.label->setText(QString::fromLocal8Bit(info.data()));
			//�޸ĵ���

			//һ���޸�
			dbinter.addcmd(m_curTableName.toStdString(),i_row,i_colum,s,2,s);
			ui.pushButton_save->setEnabled(true);
		}
	}
	
}
void isSave();
SString readSeting();
void test()
{
	ui.menu->exec(QPoint(ui.menuBar->geometry().x(),ui.menuBar->geometry().y()+ui.menuBar->geometry().height()));
};
void writeSeting(QString s_ip="127.0.0.1",QString s_port="1400",QString s_user="skt_mdb",QString s_password="skt001@163.com");
private:
	void setShortCut();
};

#endif // SYS_MDBUI_H
