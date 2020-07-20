/**
 *
 * �� �� �� : mtreewidget.h
 * �������� : 2015-7-30 15:19
 * ��    �� : �ұ���
 * �޸����� : 2015-7-30 15:19
 * ��ǰ�汾 : 1.0
 * �������� : ��дQTreeWidget������Զ��幦��
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-7-30 	�ұ�������дQTreeWidget
 *
 **/

#ifndef MTREEWIDGET_H
#define MTREEWIDGET_H
#include "pcellTree.h"
#include <QTreeWidget>
#include <QMenu>
#include <QLineEdit>
#include <QMessageBox>
#include <QResizeEvent>
#include "SString.h"
#include "mcombobox.h"
#include "dbInteraction.h"
class mTreeWidget : public QTreeWidget,public dbInteraction
{
	Q_OBJECT

public:
	mTreeWidget(QWidget *parent=0);
	virtual ~mTreeWidget();
	void setManual(bool bManual)
	{
		m_bManual = bManual;
	}

	//�����ṹ������ӵ�treewidget��
	template<class T> void addTree(SString sTable,SString sField,SString sFField,SString sValue,QStringList &ql,T &t_list)
	{
		if (m_bCheck)
		{
			this->setColumnCount(2);
		}else
		{
/*		int i = SString::GetAttributeCount(cTree->getData(),",");
		this->setColumnCount(i);*/	
			this->setHeaderLabels(ql);
		}
		QTreeWidgetItem * citm = new QTreeWidgetItem(this);
		dbInteraction::setDb(DB);
		initUicfgAndReportRef();
		dbInteraction::getTreeItem(sTable,sField,sFField,sValue,citm,t_list);
		this->resizeColumnToContents(0);
}

signals:
	void addNode(QTreeWidgetItem * node,QTreeWidgetItem *parentNode);
	void delNode(QTreeWidgetItem * node,QTreeWidgetItem *parentNode);
	void editNode(QTreeWidgetItem * node,QTreeWidgetItem *parentNode);
	void updateNode(QTreeWidgetItem * node,SString sPre,int column);//sPreΪ�޸�֮ǰ������ֵ�����޸�������ʱ���õ�
	void updateCheck(QTreeWidgetItem * node,int column);
public slots:
	void OnCurrentItemChanged ( QTreeWidgetItem * current, QTreeWidgetItem * previous );
	void onItemChanged(QTreeWidgetItem * item, int column);
	QTreeWidgetItem * chooseItem(int topIndex,QString sText);//���������ҵ���Ӧѡ��
	QTreeWidgetItem * chooseItem(QTreeWidgetItem *topItem,QString sText);
	void addData(SRecordset &rs,SString sData,QTreeWidgetItem *item);//���������
	void onCustomContextMenuRequested(const QPoint &pos);//�Ҽ�����˵�
	void OnItemDoubleClicked(QTreeWidgetItem * item, int column);//˫���¼�
	void setRmenuAndCheck(bool b_rmenu,bool b_check,bool b_edit)//�����Ƿ����Ҽ��˵��͹�ѡ
	{
		m_bRmenu = b_rmenu;
		m_bCheck = b_check;
		m_bEdit = b_edit;
	}
	void emptyCheck(int topIndex);
	void emptyCheck(QTreeWidgetItem * item);//��չ�ѡ
	void OnAddNode();
	void OnDelNode();
	void OnEditNode();
	void finishEdit();
	void oneCellChanged(QTreeWidgetItem* item,int column);//���������ı�
	void setColumnToCheck(QTreeWidgetItem * item,int column);//������ɹ�ѡ
	void setColumnToRef(QTreeWidgetItem * item,int column);//�������combobox
	void setColumnToUicfgOrReportRef(QTreeWidgetItem * item,int column);//����T_SSP_UICFG_WND/T_SSP_REPORT_INFOװ����wnd_sn+name
private slots:
	void slotupLoadRowClicked();
	void slotDownLoadRowClicked();
private:
	int getLevel(QTreeWidgetItem *item);//��ȡ�ڵ��ǵڼ���
	bool eventFilter(QObject *obj, QEvent *event);
private:
	QTreeWidgetItem * m_choosed;
	QMenu * m_rcMenu;
	QAction * m_addNode;
	QAction * m_delNode;
	QAction * m_editNode;
	QTreeWidgetItem * m_citem;
	QTreeWidgetItem * m_pitem;
	bool m_bRmenu;//�Ƿ�����Ҽ��˵�
	bool m_bCheck;//�Ƿ���Թ�ѡ
	bool m_bEdit;//�Ƿ�����޸�
	bool m_addOrUpdate;
	int m_clickCloumn;//��ǰ˫������
	bool m_bManual;//�Ƿ����ֶ���ѡ
	bool m_bresized;
	mCombobox *mpcomb;
	SDatabaseOper *m_dbOper;
	QWidget * widget;
};

#endif // MTREEWIDGET_H
