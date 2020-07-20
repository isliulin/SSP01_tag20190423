#include "view_datawindow_edit.h"
#include "ssp_gui.h"

const int ref1_sn=4;//snֵ�ڱ������
const int ref2_sn=2;
const int data1_sn=9;
const int data2_sn=9;
#define COL_REF1_SN      4
#define COL_REF2_DW_SN   2
#define COL_REF2_ROW_SN  3
#define COL_DATA1_SN     9
#define COL_DATA2_SN     9
view_datawindow_edit::view_datawindow_edit(QWidget *parent)
	: CBaseView(parent)
{
	ui.setupUi(this);
	m_inputDlg=new inputNum();
	m_countMax=1;
	pDb = DB;
	QStringList header;
	header<<tr("����")<<tr("����")<<tr("����")<<tr("����")<<tr("sn");
	ui.tableWidget_ref1->setColumnCount(5);
	ui.tableWidget_ref1->setHorizontalHeaderLabels(header);
	ui.tableWidget_ref1->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.tableWidget_ref1->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.tableWidget_ref1->horizontalHeader()->setStretchLastSection(true);
	ui.tableWidget_ref1->verticalHeader()->hide();
	ui.tableWidget_ref1->setItemDelegate(new leftTableRefWindowDelegate(this));
	ui.tableWidget_ref1->setColumnWidth(0,100);
	ui.tableWidget_ref1->setColumnWidth(1,100);
	ui.tableWidget_ref1->setColumnWidth(2,100);
	ui.tableWidget_ref1->setColumnWidth(3,100);
	ui.tableWidget_ref1->setColumnWidth(4,100);
	ui.tableWidget_ref1->setColumnHidden(ref1_sn,true);//����sn

	header.clear();
	header<<tr("���")<<tr("����")<<"sn" << "row_sn";
	ui.tableWidget_ref2->setColumnCount(4);
	ui.tableWidget_ref2->setHorizontalHeaderLabels(header);
	ui.tableWidget_ref2->setColumnHidden(ref2_sn,true);
	ui.tableWidget_ref2->setColumnHidden(COL_REF2_ROW_SN,true);
	ui.tableWidget_ref2->setColumnWidth(0,50);
	ui.tableWidget_ref2->setColumnWidth(1,100);
	ui.tableWidget_ref2->setColumnWidth(2,100);
	ui.tableWidget_ref2->setColumnWidth(3,100);
	ui.tableWidget_ref2->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.tableWidget_ref2->setSelectionMode(QAbstractItemView::SingleSelection);
	//ui.tableWidget_ref2->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
	ui.tableWidget_ref2->verticalHeader()->hide();
	

	header.clear();
	header<<tr("����")<<tr("����")<<tr("����")<<tr("����")<<tr("���ݱ任����")<<tr("�������ݼ�������")<<tr("���������")<<tr("�������������")<<tr("��չ����")<<"sn";
	ui.tableWidget_data1->setColumnCount(10);
	ui.tableWidget_data1->setHorizontalHeaderLabels(header);
	ui.tableWidget_data1->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.tableWidget_data1->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.tableWidget_data1->horizontalHeader()->setStretchLastSection(true);
	ui.tableWidget_data1->verticalHeader()->hide();
	ui.tableWidget_data1->setItemDelegate(new leftTableDataWindowDelegate(this));
	ui.tableWidget_data1->setColumnWidth(0,100);
	ui.tableWidget_data1->setColumnWidth(1,50);
	ui.tableWidget_data1->setColumnWidth(2,100);
	ui.tableWidget_data1->setColumnWidth(3,100);
	ui.tableWidget_data1->setColumnWidth(4,100);
	ui.tableWidget_data1->setColumnWidth(5,100);
	ui.tableWidget_data1->setColumnWidth(6,100);
	ui.tableWidget_data1->setColumnWidth(7,100);
	ui.tableWidget_data1->setColumnWidth(8,100);
	ui.tableWidget_data1->setColumnWidth(9,150);
	ui.tableWidget_data1->setColumnHidden(data1_sn,true);//����sn

	header.clear();
	header<<tr("���")<<tr("����")<<tr("����")<<tr("����")<<tr("���")<<tr("��������")<<tr("ת������")<<tr("��չ����")<<tr("���BAK")<<"sn";
	ui.tableWidget_data2->setColumnCount(10);
	ui.tableWidget_data2->setHorizontalHeaderLabels(header);
	ui.tableWidget_data2->setColumnHidden(8,true);
	ui.tableWidget_data2->setColumnHidden(COL_DATA2_SN,true);
	ui.tableWidget_data2->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.tableWidget_data2->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.tableWidget_data2->setColumnWidth(0,50);
	ui.tableWidget_data2->setColumnWidth(1,100);
	ui.tableWidget_data2->setColumnWidth(2,100);
	ui.tableWidget_data2->setColumnWidth(3,50);
	ui.tableWidget_data2->setColumnWidth(4,100);
	ui.tableWidget_data2->setColumnWidth(5,100);
	ui.tableWidget_data2->setColumnWidth(6,200);
	ui.tableWidget_data2->setColumnWidth(7,100);
	ui.tableWidget_data2->setColumnWidth(8,150);	
	ui.tableWidget_data2->setColumnWidth(9,100);	
	//ui.tableWidget_data2->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
	ui.tableWidget_data2->verticalHeader()->hide();
	ui.tableWidget_data2->setItemDelegate(new dataWindowDelegate(this));

	connect(ui.tableWidget_ref1,SIGNAL(currentItemChanged(QTableWidgetItem *,QTableWidgetItem *)),this,SLOT(showRefInfo(QTableWidgetItem *,QTableWidgetItem *)));
	connect(ui.tableWidget_ref1,SIGNAL(itemChanged(QTableWidgetItem *)),this,SLOT(editRef1(QTableWidgetItem *)));

	connect(ui.tableWidget_data1,SIGNAL(currentItemChanged(QTableWidgetItem *,QTableWidgetItem *)),this,SLOT(showDataInfo(QTableWidgetItem *,QTableWidgetItem *)));
	connect(ui.tableWidget_data1,SIGNAL(currentItemChanged(QTableWidgetItem *,QTableWidgetItem *)),this,SLOT(showConInfo(QTableWidgetItem *,QTableWidgetItem *)));
	connect(ui.tableWidget_data1,SIGNAL(itemChanged(QTableWidgetItem *)),this,SLOT(editData1(QTableWidgetItem *)));

	connect(ui.tableWidget_ref2,SIGNAL(currentItemChanged(QTableWidgetItem *,QTableWidgetItem *)),this,SLOT(ref2ItemCilcked(QTableWidgetItem *,QTableWidgetItem *)));
	connect(ui.tableWidget_ref2,SIGNAL(itemChanged(QTableWidgetItem *)),this,SLOT(editRef2(QTableWidgetItem *)));

	connect(ui.tableWidget_data2,SIGNAL(currentItemChanged(QTableWidgetItem *,QTableWidgetItem *)),this,SLOT(data2ItemCilcked(QTableWidgetItem *,QTableWidgetItem *)));
	connect(ui.tableWidget_data2,SIGNAL(itemChanged(QTableWidgetItem *)),this,SLOT(editData2(QTableWidgetItem *)));

	connect(ui.comboBox_data_cls,SIGNAL(currentIndexChanged(QString)),this,SLOT(dataCombo(QString)));
	connect(ui.comboBox_ref_cls,SIGNAL(currentIndexChanged(QString)),this,SLOT(refCombo(QString)));
	
	connect(ui.textEdit_ref,SIGNAL(textChanged()),this,SLOT(editRefText()));
	connect(ui.textEdit_data,SIGNAL(textChanged()),this,SLOT(editDataText()));

	connect(ui.pushButton_add,SIGNAL(clicked()),this,SLOT(onPushButton_add()));
	connect(ui.pushButton_copy,SIGNAL(clicked()),this,SLOT(onPushButton_copy()));
	connect(ui.pushButton_del,SIGNAL(clicked()),this,SLOT(onPushButton_del()));
	connect(ui.pushButton_view,SIGNAL(clicked()),this,SLOT(onPushButton_view()));
	connect(ui.pushButton_import,SIGNAL(clicked()),this,SLOT(onPushButton_import()));
	connect(ui.pushButton_export,SIGNAL(clicked()),this,SLOT(onPushButton_export()));
	connect(ui.pushButton_apply,SIGNAL(clicked()),this,SLOT(onPushButton_apply()));

	connect(ui.pushButton_ref2_add,SIGNAL(clicked()),this,SLOT(onPushButton_ref2_add()));
	connect(ui.pushButton_ref2_del,SIGNAL(clicked()),this,SLOT(onPushButton_ref2_del()));
	connect(ui.pushButton_data2_add,SIGNAL(clicked()),this,SLOT(onPushButton_data2_add()));
	connect(ui.pushButton_data2_del,SIGNAL(clicked()),this,SLOT(onPushButton_data2_del()));
	connect(ui.pushButton_newcol,SIGNAL(clicked()),this,SLOT(onPushButton_newcol()));

	connect(m_inputDlg,SIGNAL(finishInputNum()),this,SLOT(pasteCfg()));

	connect(ui.tabWidget,SIGNAL(currentChanged(int)),this,SLOT(tabChanged(int )));
	connect(ui.checkBox_useCls,SIGNAL(clicked()),this,SLOT(slotOnCheckBoxClick()));
	SQt::SetTableWidgetStyle(ui.tableWidget_ref1);
	SQt::SetTableWidgetStyle(ui.tableWidget_ref2);
	SQt::SetTableWidgetStyle(ui.tableWidget_data1);
	SQt::SetTableWidgetStyle(ui.tableWidget_data2);

	QList<int> widgetSizes1;//splitter������
	widgetSizes1 <<450 << 550;
	ui.splitter->setSizes(widgetSizes1);
	ui.splitter_3->setSizes(widgetSizes1);
	QList<int> widgetSizes2;//splitter������
	widgetSizes2 << 450 << 550;
	ui.splitter_2->setSizes(widgetSizes2);
	ui.splitter_4->setSizes(widgetSizes2);
	viewDwConEdit=new view_dw_condition_edit();
	QHBoxLayout *layout=new QHBoxLayout(this);
	layout->setContentsMargins(0,0,0,0);
	layout->addWidget(viewDwConEdit);
	ui.tab_5->setLayout(layout);
	refreshLeftTable();
	ui.tabWidget->setCurrentIndex(0);

}
void view_datawindow_edit::initRefCombo()
{
	ui.comboBox_ref_cls->blockSignals(true);
	SRecordset refcls;
	pDb->RetrieveRecordset("select distinct cls_name from t_ssp_data_window where dstype=1 and cls_name is not null order by cls_name desc,dw_desc",refcls);
	ui.comboBox_ref_cls->clear();
	ui.comboBox_ref_cls->addItem("ȫ��");
	for(int i=0;i<refcls.GetRows();i++)
	{
		QString tmp=refcls.GetValue(i,0).data();
		if(tmp!="")
		{
			ui.comboBox_ref_cls->addItem(tmp);
		}
	}
	ui.comboBox_ref_cls->blockSignals(false);
}
void view_datawindow_edit::initDataCombo()
{
	ui.comboBox_data_cls->blockSignals(true);
	SRecordset datacls;
	pDb->RetrieveRecordset("select distinct cls_name from t_ssp_data_window where dstype=2 and cls_name is not null order by cls_name",datacls);
	
	ui.comboBox_data_cls->clear();
	ui.comboBox_data_cls->addItem("ȫ��");
	for(int i=0;i<datacls.GetRows();i++)
	{
		QString tmp=datacls.GetValue(i,0).data();
		if(tmp!="")
		{
			ui.comboBox_data_cls->addItem(tmp);
		}
	}
	ui.comboBox_data_cls->blockSignals(false);
}
QTableWidget *view_datawindow_edit::getTableData2()
{
	return ui.tableWidget_data2;
}
QTableWidget *view_datawindow_edit::getTableData1()
{
	return ui.tableWidget_data1;
}
QTableWidget *view_datawindow_edit::getTableRef1()
{
	return ui.tableWidget_ref1;
}
void view_datawindow_edit::getComboxAndCheckState(QString& strComboText, bool& bIsChecked)
{
	strComboText = ui.comboBox_data_cls->currentText();
	bIsChecked = ui.checkBox_useCls->isChecked();
}
QString view_datawindow_edit::getCls()
{
	if(ui.checkBox_useCls->checkState()==Qt::Checked&&ui.comboBox_data_cls->currentText()!="ȫ��")
	{
		QString cls=" and (cls_name is Null or cls_name='"+ui.comboBox_data_cls->currentText()+"')";
		return cls;
	}
	return "";
}
view_datawindow_edit::~view_datawindow_edit()
{
	delete m_inputDlg;
}
// ============================================================================
// function:view_datawindow_edit
// date:2015/08/24
// input:
// output:
// desc:���������ݿ�����ˢ��������
// ============================================================================
void view_datawindow_edit::refreshLeftTable()
{
	setRefBlockSignals(true);
	setDataBlockSignals(true);
	ui.tableWidget_ref1->clearContents();
	ui.tableWidget_ref1->setRowCount(0);

	ui.tableWidget_data1->clearContents();
	ui.tableWidget_data1->setRowCount(0);

	m_rsLeftTable.clear();
	int ret = pDb->RetrieveRecordset("select dw_sn,dstype,name,dbtype,dw_desc,dw_select,transform_type,cross_refname,cross_column,cross_data_column,cls_name,ext_attr from t_ssp_data_window order by cls_name desc,dw_desc",m_rsLeftTable);
	if( ret < 0)
	{
		QMessageBox::warning(this,tr("����"),tr("���ݲ�ѯʧ��"),QMessageBox::Ok);
		return;
	} else if (ret == 0)
	{
		QMessageBox::warning(this,tr("����"),tr("���ݿ�������"),QMessageBox::Ok);
		return;
	}

	for(int i=0;i<m_rsLeftTable.GetRows();i++)
	{	
		if(m_rsLeftTable.GetValue(i,1)=="1")//����Ϊ����ʱ
		{
			int currowcount = ui.tableWidget_ref1->rowCount();
			ui.tableWidget_ref1->insertRow(currowcount);
			QTableWidgetItem *item=new QTableWidgetItem(m_rsLeftTable.GetValue(i,10).data());
			ui.tableWidget_ref1->setItem(currowcount,0,item);

			item=new QTableWidgetItem(m_rsLeftTable.GetValue(i,4).data());
			ui.tableWidget_ref1->setItem(currowcount,3,item);

			item=new QTableWidgetItem(typeTransform(m_rsLeftTable.GetValue(i,3)).data());
			ui.tableWidget_ref1->setItem(currowcount,2,item);

			item=new QTableWidgetItem(m_rsLeftTable.GetValue(i,2).data());
			ui.tableWidget_ref1->setItem(currowcount,1,item);

 			item=new QTableWidgetItem(m_rsLeftTable.GetValue(i,0).data());
 			ui.tableWidget_ref1->setItem(currowcount,ref1_sn,item);
		}
		if(m_rsLeftTable.GetValue(i,1)=="2")//����Ϊ���ݼ�ʱ
		{
			int currowcount = ui.tableWidget_data1->rowCount();
			ui.tableWidget_data1->insertRow(currowcount);
			QTableWidgetItem *item=new QTableWidgetItem(m_rsLeftTable.GetValue(i,10).data());
			ui.tableWidget_data1->setItem(currowcount,0,item);

			item=new QTableWidgetItem(m_rsLeftTable.GetValue(i,4).data());
			ui.tableWidget_data1->setItem(currowcount,3,item);

			item=new QTableWidgetItem(typeTransform(m_rsLeftTable.GetValue(i,3)).data());
			ui.tableWidget_data1->setItem(currowcount,2,item);

			item=new QTableWidgetItem(m_rsLeftTable.GetValue(i,2).data());
			ui.tableWidget_data1->setItem(currowcount,1,item);

			item=new QTableWidgetItem(m_rsLeftTable.GetValue(i,0).data());
			ui.tableWidget_data1->setItem(currowcount,data1_sn,item);
			//new
			item=new QTableWidgetItem(tranftranftype(m_rsLeftTable.GetValue(i,6)).data());
			ui.tableWidget_data1->setItem(currowcount,4,item);

			item=new QTableWidgetItem(m_rsLeftTable.GetValue(i,7).data());
			ui.tableWidget_data1->setItem(currowcount,5,item);

			item=new QTableWidgetItem(m_rsLeftTable.GetValue(i,8).data());
			ui.tableWidget_data1->setItem(currowcount,6,item);

			item=new QTableWidgetItem(m_rsLeftTable.GetValue(i,9).data());
			ui.tableWidget_data1->setItem(currowcount,7,item);
			item=new QTableWidgetItem(m_rsLeftTable.GetValue(i,11).data());
			ui.tableWidget_data1->setItem(currowcount,8,item);		}
	}
	setRefBlockSignals(false);
	setDataBlockSignals(false);

	initRefCombo();
	initDataCombo();
	ui.tableWidget_ref1->resizeColumnsToContents();
	ui.tableWidget_data1->resizeColumnsToContents();
	if (ui.tableWidget_ref1->rowCount() > 0)
	{
		ui.tableWidget_ref1->selectRow(0);
	}
	if (ui.tableWidget_data1->rowCount() > 0)
	{
		ui.tableWidget_data1->selectRow(0);
	}
	
}
//�������Ӱ�ť��Ӧ
void view_datawindow_edit::onPushButton_add()
{
	QString clsName="";
	
	if(ui.tabWidget->currentIndex()==0)//����
	{
		if(ui.comboBox_ref_cls->currentText()!="ȫ��")
			clsName=ui.comboBox_ref_cls->currentText();
		ui.tableWidget_ref1->blockSignals(true);
		int curRow;
		curRow=ui.tableWidget_ref1->rowCount();
		if(ui.tableWidget_ref1->rowCount()==0)
			curRow=0;
		if(curRow==-1)
		{
			ui.tableWidget_ref1->blockSignals(false);
			return;
		}
		int curmaxdwsn = getMaxDwSn();
		if (curmaxdwsn < 0)
		{
			return;
		}
		curmaxdwsn++;

		bool flag=pDb->Execute(SString::toFormat("insert into t_ssp_data_window (dw_sn,dstype,name,dbtype,dw_desc,dw_select,cls_name) values (%d,%d,'',1,'','','%s')",curmaxdwsn,1,clsName.toStdString().data()));
		if(flag==false)
		{
			QMessageBox::warning(this,tr("����"),tr("�����ݿ��������ʧ��"),QMessageBox::Ok);
			ui.tableWidget_ref1->blockSignals(false);
			return;
		}
		ui.tableWidget_ref1->insertRow(curRow);
		QTableWidgetItem *item=new QTableWidgetItem(clsName);
		ui.tableWidget_ref1->setItem(curRow,0,item);
		item=new QTableWidgetItem();
		ui.tableWidget_ref1->setItem(curRow,1,item);
		item=new QTableWidgetItem(tr("�̶�"));
		ui.tableWidget_ref1->setItem(curRow,2,item);
		item=new QTableWidgetItem();
		ui.tableWidget_ref1->setItem(curRow,3,item);
		item=new QTableWidgetItem();
		item->setText(QString::number(curmaxdwsn));
		ui.tableWidget_ref1->setItem(curRow,ref1_sn,item);

		ui.tableWidget_ref1->blockSignals(false);
	}
	if(ui.tabWidget->currentIndex()==1)//���ݼ�
	{
		if(ui.comboBox_data_cls->currentText()!="ȫ��")
			clsName=ui.comboBox_data_cls->currentText();

		ui.tableWidget_data1->blockSignals(true);
		int curRow;
		curRow=ui.tableWidget_data1->rowCount();
		if(ui.tableWidget_data1->rowCount()==0)
			curRow=0;
		if(curRow==-1)
		{
			ui.tableWidget_data1->blockSignals(false);
			return;
		}
		int curmaxdwsn = getMaxDwSn();
		if (curmaxdwsn < 0)
		{
			return;
		}
		curmaxdwsn++;

		bool flag=pDb->Execute(SString::toFormat("insert into t_ssp_data_window (dw_sn,dstype,name,dbtype,dw_desc,dw_select,transform_type,cross_refname,cross_column,cross_data_column,cls_name) values (%d,%d,'',2,'','',0,'',0,0,'%s')",curmaxdwsn,2,clsName.toStdString().data()));
		if(flag==false)
		{
			QMessageBox::warning(this,tr("����"),tr("�����ݿ�������ݼ�ʧ��"),QMessageBox::Ok);
			ui.tableWidget_ref1->blockSignals(false);
			return;
		}
		ui.tableWidget_data1->insertRow(curRow);
		QTableWidgetItem *item=new QTableWidgetItem(clsName);
		ui.tableWidget_data1->setItem(curRow,0,item);
		item=new QTableWidgetItem();
		ui.tableWidget_data1->setItem(curRow,1,item);
		item=new QTableWidgetItem(tr("��ʷ��"));
		ui.tableWidget_data1->setItem(curRow,2,item);
		item=new QTableWidgetItem();
		ui.tableWidget_data1->setItem(curRow,3,item);

		item=new QTableWidgetItem();
		item->setText(tr("���任"));
		ui.tableWidget_data1->setItem(curRow,4,item);
		item=new QTableWidgetItem();
		ui.tableWidget_data1->setItem(curRow,5,item);
		item=new QTableWidgetItem();
		item->setText(QString::number(0));
		ui.tableWidget_data1->setItem(curRow,6,item);
		item=new QTableWidgetItem();
		item->setText(QString::number(0));
		ui.tableWidget_data1->setItem(curRow,7,item);

		item=new QTableWidgetItem();
		item->setText(QString::number(curmaxdwsn));
		ui.tableWidget_data1->setItem(curRow,data1_sn,item);

		ui.tableWidget_data1->blockSignals(false);
	}
}
void view_datawindow_edit::refCombo(QString text)
{
	setRefBlockSignals(true);
	ui.tableWidget_ref1->clearContents();
	ui.tableWidget_ref1->setRowCount(0);
	ui.tableWidget_ref2->clearContents();
	ui.tableWidget_ref2->setRowCount(0);
	ui.textEdit_ref->clear();
	m_rsLeftTable.clear();
	SString sql;
	if(text!="ȫ��")
	{
		sql = SString::toFormat("select dw_sn,dstype,name,dbtype,dw_desc,dw_select,transform_type,cross_refname,\
								cross_column,cross_data_column,cls_name from t_ssp_data_window where cls_name='%s' and dstype=1 or (cls_name='' or cls_name is null) order by cls_name desc,dw_desc",text.toStdString().data());
	} else
	{
		sql = "select dw_sn,dstype,name,dbtype,dw_desc,dw_select,transform_type,cross_refname,\
								cross_column,cross_data_column,cls_name from t_ssp_data_window where dstype=1 order by cls_name desc,dw_desc";
	}
	int ret = pDb->RetrieveRecordset(sql, m_rsLeftTable);
	if(ret < 0)
	{
		QMessageBox::information(this,tr("����"),tr("��ȡ�����ݿ�ʧ��[%1]").arg(sql.data()),QMessageBox::Ok);
		setRefBlockSignals(false);
		return;
	} else if (ret == 0)
	{
		setRefBlockSignals(false);
		return;
	} 

	for(int i=0; i < m_rsLeftTable.GetRows();i++)
	{	
		ui.tableWidget_ref1->insertRow(i);
		QTableWidgetItem *item=new QTableWidgetItem(m_rsLeftTable.GetValue(i,10).data());
		ui.tableWidget_ref1->setItem(i,0,item);

		item=new QTableWidgetItem(m_rsLeftTable.GetValue(i,4).data());
		ui.tableWidget_ref1->setItem(i,3,item);

		item=new QTableWidgetItem(typeTransform(m_rsLeftTable.GetValue(i,3)).data());
		ui.tableWidget_ref1->setItem(i,2,item);

		item=new QTableWidgetItem(m_rsLeftTable.GetValue(i,2).data());
		ui.tableWidget_ref1->setItem(i,1,item);

		item=new QTableWidgetItem(m_rsLeftTable.GetValue(i,0).data());
		ui.tableWidget_ref1->setItem(i,ref1_sn,item);
	}
	setRefBlockSignals(false);
	ui.tableWidget_ref1->resizeColumnsToContents();
	if (ui.tableWidget_ref1->rowCount() > 0)
	{
		ui.tableWidget_ref1->selectRow(0);
	}
}
void view_datawindow_edit::dataCombo(QString text)
{
	setDataBlockSignals(true);
	ui.tableWidget_data1->clearContents();
	ui.tableWidget_data1->setRowCount(0);
	ui.tableWidget_data2->clearContents();
	ui.tableWidget_data2->setRowCount(0);
	ui.textEdit_data->clear();
	m_rsLeftTable.clear();
	SString sql;
	if(text!="ȫ��")
	{
		sql = SString::toFormat("select dw_sn,dstype,name,dbtype,dw_desc,dw_select,transform_type,cross_refname,cross_column,cross_data_column,cls_name,ext_attr from t_ssp_data_window where cls_name='%s' and dstype=2 or (cls_name='' or cls_name is null) order by cls_name desc,dw_desc",text.toStdString().data());
	}
	else
	{
		sql = "select dw_sn,dstype,name,dbtype,dw_desc,dw_select,transform_type,cross_refname,cross_column,cross_data_column,cls_name,ext_attr from t_ssp_data_window where dstype=2 order by cls_name desc,dw_desc";
	}
	int ret = pDb->RetrieveRecordset(sql,m_rsLeftTable);
	if (ret < 0)
	{
		QMessageBox::information(this,tr("����"),tr("��ȡ�����ݿ�ʧ��[%1]").arg(sql.data()),QMessageBox::Ok);
		setDataBlockSignals(false);
		return;
	} else if (ret == 0)
	{
		return;
	} 
	for(int i=0;i<m_rsLeftTable.GetRows();i++)
	{	
		ui.tableWidget_data1->insertRow(i);
		QTableWidgetItem *item=new QTableWidgetItem(m_rsLeftTable.GetValue(i,10).data());
		ui.tableWidget_data1->setItem(i,0,item);

		item=new QTableWidgetItem(m_rsLeftTable.GetValue(i,4).data());
		ui.tableWidget_data1->setItem(i,3,item);

		item=new QTableWidgetItem(typeTransform(m_rsLeftTable.GetValue(i,3)).data());
		ui.tableWidget_data1->setItem(i,2,item);

		item=new QTableWidgetItem(m_rsLeftTable.GetValue(i,2).data());
		ui.tableWidget_data1->setItem(i,1,item);

		item=new QTableWidgetItem(m_rsLeftTable.GetValue(i,0).data());
		ui.tableWidget_data1->setItem(i,data1_sn,item);
		//new
		item=new QTableWidgetItem(tranftranftype(m_rsLeftTable.GetValue(i,6)).data());
		ui.tableWidget_data1->setItem(i,4,item);

		item=new QTableWidgetItem(m_rsLeftTable.GetValue(i,7).data());
		ui.tableWidget_data1->setItem(i,5,item);

		item=new QTableWidgetItem(m_rsLeftTable.GetValue(i,8).data());
		ui.tableWidget_data1->setItem(i,6,item);

		item=new QTableWidgetItem(m_rsLeftTable.GetValue(i,9).data());
		ui.tableWidget_data1->setItem(i,7,item);
		item=new QTableWidgetItem(m_rsLeftTable.GetValue(i,11).data());
		ui.tableWidget_data1->setItem(i,8,item);
	}
	setDataBlockSignals(false);
	ui.tableWidget_data1->resizeColumnsToContents();
	if (ui.tableWidget_data1->rowCount() > 0)
	{
		ui.tableWidget_data1->selectRow(0);
	}
}
//��������Ӱ�ť
void view_datawindow_edit::onPushButton_ref2_add()
{
	// ��һ����ȡ����ǰѡ���ж��󣬻�ȡdw_sn
	int ref1Row = ui.tableWidget_ref1->currentRow();
	if (ref1Row < 0)
	{
		QMessageBox::warning(this,tr("����"),tr("δѡ�������������ѡ���������"),QMessageBox::Ok);
		return;
	}
	QString strDwsn = ui.tableWidget_ref1->item(ref1Row,COL_REF1_SN)->text();
	int newRowsn = 0;
	SRecordset rst;
	SString sql;
	sql = SString::toFormat("select max(row_sn) from t_ssp_data_window_row where dw_sn=%s",strDwsn.toStdString().c_str());
	int iRet = pDb->RetrieveRecordset(sql,rst);
	if (iRet < 0)
	{
		QMessageBox::warning(this,tr("����"),tr("�����ݿ�ִ��ʧ��[%1]").arg(sql.data()),QMessageBox::Ok);
		return;
	} else if (iRet == 0)
	{
		newRowsn = 1;
	} else {
		newRowsn = rst.GetValue(0,0).toInt() + 1;
	}
	//���������ݲ������ݿ�
	sql = SString::toFormat("insert into t_ssp_data_window_row (dw_sn,row_sn,row_id,row_name) values (%s,%d,'','')",strDwsn.toStdString().c_str(), newRowsn);

	bool flag=pDb->Execute(sql);
	if(flag==false)
	{
		QMessageBox::warning(this,tr("����"),tr("�����ݿ�ref����rowʧ��[%1]").arg(sql.data()),QMessageBox::Ok);
		return;
	}
	
	int curRefRow=ui.tableWidget_ref2->rowCount();
	ui.tableWidget_ref2->insertRow(curRefRow);
	ui.tableWidget_ref2->blockSignals(true);
	QTableWidgetItem *item=new QTableWidgetItem();
	ui.tableWidget_ref2->setItem(curRefRow,0,item);
	item=new QTableWidgetItem();
	ui.tableWidget_ref2->setItem(curRefRow,1,item);
	item=new QTableWidgetItem(strDwsn);
	ui.tableWidget_ref2->setItem(curRefRow,2,item);
	item=new QTableWidgetItem(QString::number(newRowsn));
	ui.tableWidget_ref2->setItem(curRefRow,3,item);
	ui.tableWidget_ref2->blockSignals(false);
}
//���ݼ�����Ӱ�ť
void view_datawindow_edit::onPushButton_data2_add()
{
	ui.tableWidget_data2->blockSignals(true);
	int curRow = ui.tableWidget_data2->rowCount();	

	int curdataRow=ui.tableWidget_data1->currentRow();
	if(curdataRow < 0)
	{	
		QMessageBox::warning(this,tr("����"),tr("δѡ���������ʧ��"),QMessageBox::Ok);
		ui.tableWidget_data2->blockSignals(false);
		return;
	}

	SRecordset tmpsets;
	int ret = pDb->RetrieveRecordset(SString::toFormat("select max(col_sn) from t_ssp_data_window_col where dw_sn=%d",ui.tableWidget_data1->item(curdataRow,data1_sn)->text().toInt()),tmpsets);
	int iNewNo = -1;
	if( ret < 0)
	{
		QMessageBox::warning(this,tr("����"),tr("��ѯ���ݱ�����ʧ��"),QMessageBox::Ok);
		ui.tableWidget_data2->blockSignals(false);
		return;
	} else if (ret == 0)
	{
		iNewNo = 1;
	} else {
		iNewNo = tmpsets.GetValue(0,0).toInt()+1;
	}
	
	ui.tableWidget_data2->insertRow(curRow);
	QTableWidgetItem *item=new QTableWidgetItem();
	item->setText(ui.tableWidget_data1->item(curdataRow,data1_sn)->text());//����ѡ���е�sn
	ui.tableWidget_data2->setItem(curRow,COL_DATA2_SN,item);

	item=new QTableWidgetItem();
	item->setText(QString::number(iNewNo));
	//item->setFlags(item->flags()&~Qt::ItemIsEditable);
	ui.tableWidget_data2->setItem(curRow,0,item);
	item=new QTableWidgetItem();
	ui.tableWidget_data2->setItem(curRow,1,item);
	item=new QTableWidgetItem("�ַ���");
	ui.tableWidget_data2->setItem(curRow,2,item);
	item=new QTableWidgetItem();
	item->setCheckState(Qt::Unchecked);
	item->setFlags(item->flags()&~Qt::ItemIsEditable);
	ui.tableWidget_data2->setItem(curRow,3,item);
	item=new QTableWidgetItem();
	ui.tableWidget_data2->setItem(curRow,4,item);
	item=new QTableWidgetItem();
	ui.tableWidget_data2->setItem(curRow,5,item);
	item=new QTableWidgetItem();
	ui.tableWidget_data2->setItem(curRow,6,item);

	item=new QTableWidgetItem();
	ui.tableWidget_data2->setItem(curRow,7,item);
	item=new QTableWidgetItem();//BAK
	item->setText(QString::number(iNewNo));
	ui.tableWidget_data2->setItem(curRow,8,item);

	bool flag=pDb->Execute(SString::toFormat("insert into t_ssp_data_window_col (dw_sn,col_sn,col_name,col_vtype,col_width,ref_name,conv,pkey,isnull,ukey)	values (%s,%d,'','string',0,'','',0,0,0)",ui.tableWidget_data1->item(curdataRow,data1_sn)->text().toStdString().c_str(),iNewNo));
	if(flag==false)
		QMessageBox::warning(this,tr("����"),tr("�����ݿ�dataset����colʧ��"),QMessageBox::Ok);

	for(int i=curRow+1;i<ui.tableWidget_data2->rowCount();i++)
	{
		int num=ui.tableWidget_data2->item(curRow,0)->text().toInt()+1;
		ui.tableWidget_data2->item(curRow,0)->setText(QString::number(num));
	}
	ui.tableWidget_data2->blockSignals(false);
}

//���ư�ť
void view_datawindow_edit::onPushButton_copy()
{
	m_inputDlg->show();
}
//���ư�ť��Ӧ
void view_datawindow_edit::pasteCfg()
{
	int num=m_inputDlg->getNum();//���Ƽ���
	if(ui.tabWidget->currentIndex()==0)
	{
		int curRow=ui.tableWidget_ref1->currentRow();
		if(curRow>=0)
		{
			int newDwsn = getMaxDwSn();
			if (newDwsn < 0)
			{
				return;
			}		
			setRefBlockSignals(true);
			for(int i=0;i<num;i++)
			{
				newDwsn ++;
				QString strCurSelRowSn = ui.tableWidget_ref1->item(curRow,COL_REF1_SN)->text();
				//�����ݿ⸴��
				SString sql = SString::toFormat("insert into t_ssp_data_window (dw_sn,dstype,name,dbtype,dw_desc,dw_select,transform_type,cross_refname,cross_column,cross_data_column,cls_name,ext_attr) select %d,dstype,name,dbtype,dw_desc,dw_select,transform_type,cross_refname,cross_column,cross_data_column,cls_name,ext_attr from t_ssp_data_window where dw_sn=%s",
					newDwsn,strCurSelRowSn.toStdString().c_str());
				bool flag=pDb->Execute(sql);
				if(flag==false)
				{
					QMessageBox::warning(this,tr("����"),tr("�����ݿ⸴������ʧ��"),QMessageBox::Ok);
					setRefBlockSignals(false);
					return;
				}
				int icurRowCount = ui.tableWidget_ref1->rowCount();
				//�����
				ui.tableWidget_ref1->insertRow(icurRowCount);
				QTableWidgetItem *item=new QTableWidgetItem();
				QTableWidgetItem *pRef;
				pRef = ui.tableWidget_ref1->item(curRow,0);
				item->setText(pRef==NULL?"":pRef->text());
				ui.tableWidget_ref1->setItem(icurRowCount,0,item);

				item=new QTableWidgetItem();
				pRef = ui.tableWidget_ref1->item(curRow,1);
				item->setText(pRef==NULL?"":pRef->text());
				ui.tableWidget_ref1->setItem(icurRowCount,1,item);

				item=new QTableWidgetItem();
				pRef = ui.tableWidget_ref1->item(curRow,2);
				item->setText(pRef==NULL?"":pRef->text());
				ui.tableWidget_ref1->setItem(icurRowCount,2,item);

				item=new QTableWidgetItem();
				pRef = ui.tableWidget_ref1->item(curRow,3);
				item->setText(pRef==NULL?"":pRef->text());
				ui.tableWidget_ref1->setItem(icurRowCount,3,item);

				
				item=new QTableWidgetItem();
				item->setText(QString::number(newDwsn));
				ui.tableWidget_ref1->setItem(icurRowCount,COL_REF1_SN,item);
			
				//��Ӧ��row����
				sql = SString::toFormat("insert into t_ssp_data_window_row (dw_sn,row_sn,row_id,row_name) select %d,row_sn,row_id,row_name from t_ssp_data_window_row where dw_sn=%s",
					newDwsn,strCurSelRowSn.toStdString().c_str());
				flag=pDb->Execute(sql);
				if(flag==false)
				{
					QMessageBox::warning(this,tr("����"),tr("�����ݿ⸴������ʧ��"),QMessageBox::Ok);
					setRefBlockSignals(false);
					return;
				}
			}
			setRefBlockSignals(false);
		}
	}
	if(ui.tabWidget->currentIndex()==1)//���ݼ�
	{
		int curRow=ui.tableWidget_data1->currentRow();
		if(curRow>=0)
		{
			int newDwsn = getMaxDwSn();
			if (newDwsn < 0)
			{
				return;
			}
			setDataBlockSignals(true);
			for(int i=0;i<num;i++)
			{
				newDwsn++;
				QString strCurSelRowSn = ui.tableWidget_data1->item(curRow,COL_DATA1_SN)->text();
				//�����ݿ⸴��
				SString sql = SString::toFormat("insert into t_ssp_data_window (dw_sn,dstype,name,dbtype,dw_desc,dw_select,transform_type,cross_refname,cross_column,cls_name,cross_data_column,ext_attr) select %d,dstype,name,dbtype,dw_desc,dw_select,transform_type,cross_refname,cross_column,cls_name,cross_data_column,ext_attr from t_ssp_data_window where dw_sn=%s",
					newDwsn,strCurSelRowSn.toStdString().c_str());
				bool flag=pDb->Execute(sql);
				if(flag==false)
				{
					QMessageBox::warning(this,tr("����"),tr("�����ݿ⸴�����ݼ�ʧ��"),QMessageBox::Ok);
					setDataBlockSignals(false);
					return;
				}
				int icurRowCount = ui.tableWidget_data1->rowCount();
				//�����
				ui.tableWidget_data1->insertRow(icurRowCount);
				QTableWidgetItem *item=new QTableWidgetItem();
				QTableWidgetItem *pRef;
				pRef = ui.tableWidget_data1->item(curRow,2);
				item->setText(pRef==NULL?"":pRef->text());
				ui.tableWidget_data1->setItem(icurRowCount,2,item);

				item=new QTableWidgetItem();
				pRef = ui.tableWidget_data1->item(curRow,1);
				item->setText(pRef==NULL?"":pRef->text());
				ui.tableWidget_data1->setItem(icurRowCount,1,item);

				item=new QTableWidgetItem();
				pRef = ui.tableWidget_data1->item(curRow,0);
				item->setText(pRef==NULL?"":pRef->text());
				ui.tableWidget_data1->setItem(icurRowCount,0,item);

				item=new QTableWidgetItem();
				pRef = ui.tableWidget_data1->item(curRow,3);
				item->setText(pRef==NULL?"":pRef->text());
				ui.tableWidget_data1->setItem(icurRowCount,3,item);

				item=new QTableWidgetItem();
				pRef = ui.tableWidget_data1->item(curRow,4);
				item->setText(pRef==NULL?"":pRef->text());
				ui.tableWidget_data1->setItem(icurRowCount,4,item);

				item=new QTableWidgetItem();
				pRef = ui.tableWidget_data1->item(curRow,5);
				item->setText(pRef==NULL?"":pRef->text());
				ui.tableWidget_data1->setItem(icurRowCount,5,item);

				item=new QTableWidgetItem();
				pRef = ui.tableWidget_data1->item(curRow,6);
				item->setText(pRef==NULL?"":pRef->text());
				ui.tableWidget_data1->setItem(icurRowCount,6,item);


				item=new QTableWidgetItem();
				pRef = ui.tableWidget_data1->item(curRow,7);
				item->setText(pRef==NULL?"":pRef->text());
				ui.tableWidget_data1->setItem(icurRowCount,7,item);

				item=new QTableWidgetItem();
				pRef = ui.tableWidget_data1->item(curRow,8);
				item->setText(pRef==NULL?"":pRef->text());
				ui.tableWidget_data1->setItem(icurRowCount,8,item);

				
				item=new QTableWidgetItem();
				item->setText(QString::number(newDwsn));
				ui.tableWidget_data1->setItem(icurRowCount,COL_DATA1_SN,item);
				
				//�����ж���
				sql = SString::toFormat("insert into t_ssp_data_window_col (dw_sn,col_sn,col_name,col_vtype,col_width,ref_name,conv,pkey,isnull,ukey,ext_attr) select %d,col_sn,col_name,col_vtype,col_width,ref_name,conv,pkey,isnull,ukey,ext_attr from t_ssp_data_window_col where dw_sn=%s",
					newDwsn,strCurSelRowSn.toStdString().c_str());
				flag=pDb->Execute(sql);
				if(flag==false)
				{
					QMessageBox::warning(this,tr("����"),tr("�����ݿ⸴���ж���ʧ��").arg(sql.data()),QMessageBox::Ok);
					setRefBlockSignals(false);
					return;
				}
				//������������
				sql = SString::toFormat("insert into t_ssp_data_window_condition (dw_sn,con_sn,con_name,cmp_type,input_type,is_option,ref_name,def_value) select %d,con_sn,con_name,cmp_type,input_type,is_option,ref_name,def_value from t_ssp_data_window_condition where dw_sn=%d",
					newDwsn,strCurSelRowSn.toStdString().c_str());
				flag=pDb->Execute(sql);
				if(flag==false)
				{
					QMessageBox::warning(this,tr("����"),tr("�����ݿ⸴��������������ʧ��").arg(sql.data()),QMessageBox::Ok);
					setRefBlockSignals(false);
					return;
				}
			}
			setDataBlockSignals(false);

		}
	}
}
//ɾ��
void view_datawindow_edit::onPushButton_del()
{
	if(ui.tabWidget->currentIndex()==0)//����
	{
		int curRow;
		if(ui.tableWidget_ref1->rowCount() == 0)
		{
			return;
		}
		curRow=ui.tableWidget_ref1->currentRow();
		if(curRow < 0)
		{
			QMessageBox::warning(this,tr("����"),tr("������û��ѡ�ж�������ѡ�����"),QMessageBox::Ok);
			return;
		}
		
		int i_ret = QMessageBox::warning(this,tr("����"),tr("�Ƿ�ɾ�����õ�")+QString::number(curRow+1)+tr("�У�"),QMessageBox::Yes,QMessageBox::No);
		if (i_ret==QMessageBox::No)
		{
			return;
		}
		SString sn=ui.tableWidget_ref1->item(curRow,ref1_sn)->text().toStdString().data();
		bool flag=pDb->Execute(SString::toFormat("delete from t_ssp_data_window where dw_sn=%s",sn.data()));
		if(flag==false)
		{
			QMessageBox::warning(this,tr("����"),tr("�����ݿ�ɾ������ʧ��"),QMessageBox::Ok);
			return;
		}
		pDb->Execute(SString::toFormat("delete from t_ssp_data_window_row where dw_sn=%s",sn.data()));
		if(flag==false)
		{
			QMessageBox::warning(this,tr("����"),tr("�����ݿ�ɾ�����ö�Ӧ��ʧ��"),QMessageBox::Ok);
			return;
		}
		ui.tableWidget_ref1->blockSignals(true);
		ui.tableWidget_ref1->removeRow(curRow);
		ui.tableWidget_ref1->blockSignals(false);
		QString strCurComText = ui.comboBox_ref_cls->currentText();
		if(strCurComText != "ȫ��")
		{
			if (ui.tableWidget_ref1->rowCount() > 0)
			{
				ui.tableWidget_ref1->selectRow(0);

			} else {
				initRefCombo();
				refCombo("ȫ��");
#if 0
				int iIndex = ui.comboBox_ref_cls->findText(strCurComText);
				if (iIndex < 0)
				{
					ui.comboBox_ref_cls->setCurrentIndex(0);
					
				} else {
					ui.comboBox_ref_cls->setCurrentIndex(iIndex);
				}
#endif
				
			}
		} else {
			if (ui.tableWidget_ref1->rowCount() > 0)
			{
				ui.tableWidget_ref1->selectRow(0);
			} else {
				initRefCombo();			
				ui.textEdit_ref->blockSignals(true);
				ui.textEdit_ref->clear();
				ui.textEdit_ref->blockSignals(false);
				ui.tableWidget_ref2->clearContents();
				ui.tableWidget_ref2->setRowCount(0);
			}
		}
	}
	if(ui.tabWidget->currentIndex()==1)//���ݼ�
	{
		int curRow;
		if(ui.tableWidget_data1->rowCount() == 0)
		{
			return;
		}

		curRow=ui.tableWidget_data1->currentRow();
		if(curRow<0)
		{
			QMessageBox::warning(this,tr("����"),tr("������û��ѡ�ж�������ѡ�����"),QMessageBox::Ok);
			return;
		}
		int i_ret = QMessageBox::warning(this,tr("����"),tr("�Ƿ�ɾ�����ݼ���")+QString::number(curRow+1)+tr("�У�"),QMessageBox::Yes,QMessageBox::No);
		if (i_ret==QMessageBox::No)
		{
			return;
		}

		SString sn=ui.tableWidget_data1->item(curRow,data1_sn)->text().toStdString().data();
		bool flag=pDb->Execute(SString::toFormat("delete from t_ssp_data_window where dw_sn=%s",sn.data()));
		if(flag==false)
		{
			QMessageBox::warning(this,tr("����"),tr("�����ݿ�ɾ�����ݼ�ʧ��"),QMessageBox::Ok);
			return;
		}
		flag = pDb->Execute(SString::toFormat("delete from t_ssp_data_window_col where dw_sn=%s",sn.data()));
		if(flag==false)
		{
			QMessageBox::warning(this,tr("����"),tr("�����ݿ�ɾ�����ݼ�ʧ��"),QMessageBox::Ok);
			return;
		}

		flag = pDb->Execute(SString::toFormat("delete from t_ssp_data_window_condition where dw_sn=%s",sn.data()));
 		if(flag==false)
 		{
 			QMessageBox::warning(this,tr("����"),tr("�����ݿ�ɾ�����ݼ�����ʧ��"),QMessageBox::Ok);
 			return;
 		}
		ui.tableWidget_data1->blockSignals(true);
		ui.tableWidget_data1->removeRow(curRow);
		ui.tableWidget_data1->blockSignals(false);
		QString strCurComText = ui.comboBox_data_cls->currentText();
		if(strCurComText != "ȫ��")
		{
			if (ui.tableWidget_data1->rowCount() > 0)
			{
				ui.tableWidget_data1->selectRow(0);

			} else {
				initDataCombo();
				dataCombo("ȫ��");
				/*int iIndex = ui.comboBox_data_cls->findText(strCurComText);
				if (iIndex < 0)
				{
				ui.comboBox_data_cls->setEditText("ȫ��");
				} else {
				ui.comboBox_data_cls->setCurrentIndex(iIndex);
				}*/
			}
		} else {
			if (ui.tableWidget_data1->rowCount() > 0)
			{
				ui.tableWidget_data1->selectRow(0);
			} else {
				initDataCombo();			
				ui.textEdit_data->blockSignals(true);
				ui.textEdit_data->clear();
				ui.textEdit_data->blockSignals(false);
				ui.tableWidget_data2->clearContents();
				ui.tableWidget_data2->setRowCount(0);
			}
		}
	}
}
//������ɾ��
void view_datawindow_edit::onPushButton_ref2_del()
{
	int curRow = ui.tableWidget_ref2->currentRow();
	if(curRow < 0)
	{
		QMessageBox::warning(this,tr("����"),tr("δѡ������"),QMessageBox::Ok);
		return;
	}
	int i_ret = QMessageBox::warning(this,tr("����"),tr("�Ƿ�ɾ�����ö�Ӧ�е�")+QString::number(curRow+1)+tr("�У�"),QMessageBox::Yes,QMessageBox::No);
	if (i_ret==QMessageBox::No)
	{
		return;
	}

	SString sn=ui.tableWidget_ref2->item(curRow,ref2_sn)->text().toStdString().data();
	QString strTemp = ui.tableWidget_ref2->item(curRow, COL_REF2_ROW_SN)->text();
	SString rowsn = ui.tableWidget_ref2->item(curRow, COL_REF2_ROW_SN)->text().toStdString().c_str();
	bool flag=pDb->Execute(SString::toFormat("delete from t_ssp_data_window_row where dw_sn=%s and row_sn=%s",sn.data(), rowsn.data()));
	if(flag==false)
	{
		QMessageBox::warning(this,tr("����"),tr("�����ݿ�ɾ�����ö�Ӧ��ʧ��"),QMessageBox::Ok);
		ui.tableWidget_ref2->blockSignals(false);
		return;
	}
	ui.tableWidget_ref2->blockSignals(true);
	ui.tableWidget_ref2->removeRow(curRow);
	ui.tableWidget_ref2->blockSignals(false);
	if (ui.tableWidget_ref2->rowCount() <= 0)
	{
		ui.pushButton_ref2_del->setEnabled(false);
	}

}
//���ݼ���ɾ��
void view_datawindow_edit::onPushButton_data2_del()
{
	int curRow;
	ui.tableWidget_data2->blockSignals(true);
	curRow=ui.tableWidget_data2->currentRow();
	if(curRow<0)
	{
		ui.tableWidget_data2->blockSignals(false);
		return;
	}
	int i_ret = QMessageBox::warning(this,tr("����"),tr("�Ƿ�ɾ�����ݼ��е�")+QString::number(curRow+1)+tr("�У�"),QMessageBox::Yes,QMessageBox::No);
	if (i_ret==QMessageBox::No)
	{
		ui.tableWidget_data2->blockSignals(false);
		return;
	}
	//int curdataRow=ui.tableWidget_data2->currentRow();
	SString sn=ui.tableWidget_data2->item(curRow,COL_DATA2_SN)->text().toStdString().data();
	SString ssql=SString::toFormat("delete from t_ssp_data_window_col where dw_sn=%s",sn.data());
	bool flag=pDb->Execute(SString::toFormat("delete from t_ssp_data_window_col where dw_sn=%s and col_sn=%s",sn.data(),ui.tableWidget_data2->item(curRow,0)->text().toStdString().data()));
	if(flag==false)
	{
		QMessageBox::warning(this,tr("����"),tr("�����ݿ�ɾ�����ݼ�������ʧ��"),QMessageBox::Ok);
		ui.tableWidget_data2->blockSignals(false);
		return;
	}

	ui.tableWidget_data2->removeRow(curRow);
	ui.tableWidget_data2->blockSignals(false);
	if (ui.tableWidget_ref2->rowCount() <= 0)
	{
		ui.pushButton_data2_del->setEnabled(false);
	}
}
void view_datawindow_edit::onPushButton_newcol()
{
	int curdataRow=ui.tableWidget_data1->currentRow();
	if(curdataRow<0)
	{	
		QMessageBox::warning(this,tr("����"),tr("δѡ���������ʧ��"),QMessageBox::Ok);
		return;
	}
	QString strDwsn = ui.tableWidget_data1->item(curdataRow,COL_DATA1_SN)->text();
	ui.tableWidget_data2->blockSignals(true);
	if(ui.tableWidget_data2->rowCount()>0)
	{
		if(QMessageBox::No==QMessageBox::warning(this,tr("����"),tr("�Ѵ�����,�Ƿ���գ�"),QMessageBox::Yes,QMessageBox::No))
			return;
		SString ssql=SString::toFormat("delete from t_ssp_data_window_col where dw_sn=%s",strDwsn.toStdString().c_str());
		bool bFlag = pDb->Execute(ssql);
		if (!bFlag)
		{
			QMessageBox::warning(this,tr("����"),tr("ɾ�����ݿ�����ʧ��[%1]").arg(ssql.data()),QMessageBox::Ok);
			ui.tableWidget_data2->blockSignals(false);
			return;
		}
		ui.pushButton_data2_del->setEnabled(false);
		ui.tableWidget_data2->clearContents();
		ui.tableWidget_data2->setRowCount(0);
	}

	QString str=ui.textEdit_data->toPlainText();
	if(!str.contains("select"))
	{
		ui.tableWidget_data2->blockSignals(false);
		return;
	}
	QStringList strList=str.split("from").at(0).split("select").at(1).split(",");
	int num=strList.size();	
	
	for(int i = 0;i < num; i++)
	{
		int no = i + 1;
		//�������ݿ���룬����ɹ����ڱ�����ʾ
		bool flag=pDb->Execute(SString::toFormat("insert into t_ssp_data_window_col (dw_sn,col_sn,col_name,col_vtype,col_width,ref_name,conv,pkey,isnull,ukey)	values (%s,%d,'%s','string',0,'','',0,0,0)",strDwsn.toStdString().c_str(),no,strList.at(i).toStdString().c_str()));
		if(flag==false)
		{
			QMessageBox::warning(this,tr("����"),tr("�����ݿ�dataset����colʧ��"),QMessageBox::Ok);
			ui.tableWidget_data2->blockSignals(false);
			return;
		}
		ui.tableWidget_data2->insertRow(i);
		QTableWidgetItem *item=new QTableWidgetItem();
		item->setText(QString::number(no));
		ui.tableWidget_data2->setItem(i,0,item);
		item=new QTableWidgetItem(strList.at(i));
		ui.tableWidget_data2->setItem(i,1,item);
		item=new QTableWidgetItem("�ַ���");
		ui.tableWidget_data2->setItem(i,2,item);
		item=new QTableWidgetItem();
		item->setCheckState(Qt::Unchecked);
		item->setFlags(item->flags()&~Qt::ItemIsEditable);
		ui.tableWidget_data2->setItem(i,3,item);
		item=new QTableWidgetItem();
		ui.tableWidget_data2->setItem(i,4,item);
		item=new QTableWidgetItem();
		ui.tableWidget_data2->setItem(i,5,item);
		item=new QTableWidgetItem();
		ui.tableWidget_data2->setItem(i,6,item);
		item=new QTableWidgetItem();
		ui.tableWidget_data2->setItem(i,7,item);

		item=new QTableWidgetItem();//BAK
		item->setText(QString::number(no));
		ui.tableWidget_data2->setItem(i,8,item);

		item=new QTableWidgetItem();
		item->setText(strDwsn);//����ѡ���е�sn
		ui.tableWidget_data2->setItem(i,COL_DATA2_SN,item);	
	}

	ui.tableWidget_data2->blockSignals(false);
	if (ui.tableWidget_data2->rowCount() > 0)
	{
		ui.tableWidget_data2->selectRow(0);
		ui.pushButton_data2_del->setEnabled(true);
	}
}
//Ԥ��
void view_datawindow_edit::onPushButton_view()
{
	
	if(ui.tabWidget->currentIndex()==0)
	{
		QMessageBox::warning(this,tr("����"),tr("���ò���Ԥ��"),QMessageBox::Yes);
		return;
	}
	else if(ui.tabWidget->currentIndex()==1)
	{
		
		int curRow=ui.tableWidget_data1->currentRow();
		if(curRow<0)
		{
			QMessageBox::warning(this,tr("����"),tr("δѡ������"),QMessageBox::Ok);
			return;
		}
		if(ui.tableWidget_data2->rowCount()<1)
		{
			QMessageBox::warning(this,tr("����"),tr("��Ϊ��"),QMessageBox::Ok);
			return;
		}

		review reviewDlg(this);	
		view_common *common=new view_common(reviewDlg.widget,ui.tableWidget_data1->item(curRow,1)->text().toStdString().data());
		common->InitWidget(ui.tableWidget_data1->item(curRow,1)->text().toStdString().data());
		view_query *query=new view_query(reviewDlg.widget_2,ui.tableWidget_data1->item(curRow,1)->text().toStdString().data(),false);
		query->InitWidget(ui.tableWidget_data1->item(curRow,1)->text().toStdString().data());
 		reviewDlg.gridLayout_2->addWidget(common);
 		reviewDlg.gridLayout_4->addWidget(query);
		reviewDlg.exec();
	}
	
}
int view_datawindow_edit::getMaxDwSn()
{
	int maxDwsn = -1;
	SString sql = "select max(dw_sn) from t_ssp_data_window";
	SRecordset tempSet;
	int ret = pDb->RetrieveRecordset(sql,tempSet);
	if (ret < 0)
	{
		QMessageBox::warning(this,tr("����"),tr("���ݿ��ѯʧ�ܡ�%1��").arg(sql.data()),QMessageBox::Ok);
		return -1;
	} else if (ret == 0)
	{
		maxDwsn = 0;
	}else{
		maxDwsn = tempSet.GetValue(0,0).toInt();
	}
	return maxDwsn;
}
bool view_datawindow_edit::insertOneRefSetInfo(int maxDwsn, myRef* curRefSet)
{
	SString sql;
	if (curRefSet->dbtype == "1")
	{
		sql = SString::toFormat("insert into t_ssp_data_window \
								(dw_sn,dstype,name,dbtype,dw_desc,dw_select,transform_type,cross_refname,\
								cross_column,cross_data_column,cls_name,ext_attr) values (%d,%d,'%s',%s,'%s',' ',0,' ',0,0,'%s',' ')"
								,maxDwsn, 1, curRefSet->name.data(),curRefSet->dbtype.data(),curRefSet->dw_desc.data(),curRefSet->cls_name.data());
		if (!pDb->Execute(sql))
		{
			QMessageBox::warning(this,tr("����"),tr("�����ݿ��в���ʧ��[%1]").arg(sql.data()),QMessageBox::Ok);
			return false;
		}
		for (int i = 0; i < curRefSet->myRowList.count(); i ++)
		{
			sql = SString::toFormat("insert into t_ssp_data_window_row (dw_sn,row_sn,row_id,row_name) values \
									(%d,%d,'%s','%s')",maxDwsn, i+1,curRefSet->myRowList.at(i)->id.data(),curRefSet->myRowList.at(i)->name.data());
			if (!pDb->Execute(sql))
			{
				QMessageBox::warning(this,tr("����"),tr("�����ݿ��в���ʧ��[%1]").arg(sql.data()),QMessageBox::Ok);
				return false;
			}
		}
	} else {
		sql = SString::toFormat("insert into t_ssp_data_window \
								(dw_sn,dstype,name,dbtype,dw_desc,dw_select,transform_type,cross_refname,\
								cross_column,cross_data_column,cls_name,ext_attr) values (%d,%d,'%s',%s,'%s','%s',0,' ',0,0,'%s',' ')"\
								,maxDwsn, 1, curRefSet->name.data(),curRefSet->dbtype.data(),curRefSet->dw_desc.data(),curRefSet->dw_select.data(),curRefSet->cls_name.data());
		if (!pDb->Execute(sql))
		{
			QMessageBox::warning(this,tr("����"),tr("�����ݿ��в���ʧ��[%1]").arg(sql.data()),QMessageBox::Ok);
			return false;
		}
	}
	return true;
}
bool view_datawindow_edit::handleOneRefNode(int& maxDwsn, myRef* curRefSet)
{
	SString sql = SString::toFormat("select dw_sn from t_ssp_data_window where name='%s' and dstype=1",curRefSet->name.data());
	SRecordset querySet;
	int ret = pDb->RetrieveRecordset(sql,querySet);
	if (ret < 0)
	{
		QMessageBox::warning(this,tr("����"),tr("�����ݿ��ѯʧ��[%1]").arg(sql.data()),QMessageBox::Ok);
		return false;
	} else if (ret == 0)
	{
		maxDwsn ++;
		return insertOneRefSetInfo(maxDwsn,curRefSet);
	} else 
	{
		//�����ֹԭ���ݿ����name�ظ����ǵ�������ݴ����ظ������������ԭ���ݣ���ֹ���������ظ�������������ж�
		for (int i = 0; i < querySet.GetRows(); i++)
		{
			sql = SString::toFormat("delete from t_ssp_data_window where dw_sn=%s",querySet.GetValue(i,0).data());
			if (!pDb->Execute(sql))
			{
				QMessageBox::warning(this,tr("����"),tr("�����ݿ���ɾ��ʧ��[%1]").arg(sql.data()),QMessageBox::Ok);
				return false;
			}
			if (curRefSet->dbtype == "1")
			{
				sql = SString::toFormat("delete from t_ssp_data_window_row where dw_sn=%s",querySet.GetValue(i,0).data());
				if (!pDb->Execute(sql))
				{
					QMessageBox::warning(this,tr("����"),tr("�����ݿ���ɾ��ʧ��[%1]").arg(sql.data()),QMessageBox::Ok);
					return false;
				}
			}
		}
		maxDwsn ++;
		return insertOneRefSetInfo(maxDwsn,curRefSet);
	}
	return true;
}
bool view_datawindow_edit::insertOneDataSetInfo(int maxDwsn, myDataset* curDataSet)
{
	SString sql = SString::toFormat("insert into t_ssp_data_window \
							(dw_sn,dstype,name,dbtype,dw_desc,dw_select,transform_type,cross_refname,\
							cross_column,cross_data_column,cls_name,ext_attr) values (%d,%d,'%s',%d,'%s','%s',%d,'%s',%d,%d,'%s','%s')"\
							,maxDwsn, 2, curDataSet->name.data(),curDataSet->dbtype,curDataSet->dw_desc.data(),curDataSet->dw_select.data()
							,curDataSet->transform_type,curDataSet->cross_refname.data(),curDataSet->cross_column,curDataSet->cross_data_column
							,curDataSet->cls_name.data(),curDataSet->ext_attr.data());
	if (!pDb->Execute(sql))
	{
		QMessageBox::warning(this,tr("����"),tr("�����ݿ��в���ʧ��[%1]").arg(sql.data()),QMessageBox::Ok);
		return false;
	}
	for (int i = 0; i < curDataSet->myColList.count(); i ++)
	{
		sql = SString::toFormat("insert into t_ssp_data_window_col (dw_sn,col_sn,col_name,col_vtype,col_width,ref_name,conv,pkey,isnull,ukey,ext_attr) values \
								(%d,%d,'%s','%s',%d,'%s','%s',%d,%d,%d,'%s')",
								maxDwsn, i+1,curDataSet->myColList.at(i)->col_name.data(), curDataSet->myColList.at(i)->col_vtype.data(),
								curDataSet->myColList.at(i)->col_width,curDataSet->myColList.at(i)->ref_name.data(),curDataSet->myColList.at(i)->conv.data(),
								curDataSet->myColList.at(i)->pkey,curDataSet->myColList.at(i)->isnull,curDataSet->myColList.at(i)->ukey,
								curDataSet->myColList.at(i)->ext_attr.data());
		if (!pDb->Execute(sql))
		{
			QMessageBox::warning(this,tr("����"),tr("�����ݿ��в���ʧ��[%1]").arg(sql.data()),QMessageBox::Ok);
			return false;
		}
	}

	for (int i = 0; i < curDataSet->myConditionList.count(); i++)
	{
		sql = SString::toFormat("insert into t_ssp_data_window_condition (dw_sn,con_sn,con_name,is_option,cmp_type,input_type,ref_name,def_value,dyn_where) values \
								(%d,%d,'%s',%d,%d,%d,'%s','%s','%s')",maxDwsn, i+1, curDataSet->myConditionList.at(i)->con_name.data(),
								curDataSet->myConditionList.at(i)->is_option, curDataSet->myConditionList.at(i)->cmp_type, curDataSet->myConditionList.at(i)->input_type,
								curDataSet->myConditionList.at(i)->ref_name.data(), curDataSet->myConditionList.at(i)->def_value.data(), curDataSet->myConditionList.at(i)->dyn_where.data());
		if (!pDb->Execute(sql))
		{
			QMessageBox::warning(this,tr("����"),tr("�����ݿ��в���ʧ��[%1]").arg(sql.data()),QMessageBox::Ok);
			return false;
		}
	}
	return true;
	
}
bool view_datawindow_edit::handleOneDataNode(int& maxDwsn, myDataset* curDataSet)
{
	SString sql = SString::toFormat("select dw_sn from t_ssp_data_window where name='%s' and dstype=2",curDataSet->name.data());
	SRecordset querySet;
	int ret = pDb->RetrieveRecordset(sql,querySet);
	if (ret < 0)
	{
		QMessageBox::warning(this,tr("����"),tr("�����ݿ��ѯʧ��[%1]").arg(sql.data()),QMessageBox::Ok);
		return false;
	} else if (ret == 0)
	{
		maxDwsn ++;
		return insertOneDataSetInfo(maxDwsn,curDataSet);
	} else 
	{
		//�����ֹԭ���ݿ����name�ظ����ǵ�������ݴ����ظ������������ԭ���ݣ���ֹ���������ظ�������������ж�
		for (int i = 0; i < querySet.GetRows(); i++)
		{
			sql = SString::toFormat("delete from t_ssp_data_window where dw_sn=%s",querySet.GetValue(i,0).data());
			if (!pDb->Execute(sql))
			{
				QMessageBox::warning(this,tr("����"),tr("�����ݿ���ɾ��ʧ��[%1]").arg(sql.data()),QMessageBox::Ok);
				return false;
			}

	
			sql = SString::toFormat("delete from t_ssp_data_window_col where dw_sn=%s",querySet.GetValue(i,0).data());
			if (!pDb->Execute(sql))
			{
				QMessageBox::warning(this,tr("����"),tr("�����ݿ���ɾ��ʧ��[%1]").arg(sql.data()),QMessageBox::Ok);
				return false;
			}

			sql = SString::toFormat("delete from t_ssp_data_window_condition where dw_sn=%s",querySet.GetValue(i,0).data());
			if (!pDb->Execute(sql))
			{
				QMessageBox::warning(this,tr("����"),tr("�����ݿ���ɾ��ʧ��[%1]").arg(sql.data()),QMessageBox::Ok);
				return false;
			}
		}
		maxDwsn ++;
		return insertOneDataSetInfo(maxDwsn,curDataSet);
	}
	return true;
}
//����
void view_datawindow_edit::onPushButton_import()
{
	QString path=QFileDialog::getOpenFileName(this,"���ļ�","./../conf/sys_datawindow.xml","*.xml",0,0);
	//QString path="D:\\softPlat\\trunk\\SSP01\\project\\win32-vc10\\sys_datawindow.xml";
	SXmlConfig xml;
	if(!xml.ReadConfig( path.toStdString().data()))
	{
		LOGERROR("Load Datawindow config error!file=%s",path.toStdString().data());
		return;
	}
	int curMaxdwsn = getMaxDwSn();
	if (curMaxdwsn < 0)
	{
		return;
	}

	SBaseConfig *pDW=&xml;

	if(pDW->GetNodeName() != "data_window")
	{
		LOGERROR("Datawindow root node is not 'data_window'!");
		return;
	}
	SSP_GUI->ShowWaiting();
#if 1
	int nodeCount = pDW->GetChildCount();
	for (int i = 0; i < nodeCount; i++)
	{
		SBaseConfig *psubNode = pDW->GetChildNode(i);
		if (psubNode == NULL)
		{
			continue;
		}
		if (psubNode->GetNodeName() == "reference")
		{
			myRef ref;
			ref.name = psubNode->GetAttribute("name");
			SString strDbType=psubNode->GetAttribute("dbtype");
			if(strDbType == "fixed")
				ref.dbtype = "1";
			else if(strDbType == "db")
				ref.dbtype = "2";
			else if(strDbType == "mdb")
				ref.dbtype = "3";
			else
			{
				LOGWARN("Unknown reference[%s] type=%s!",ref.name.data(),strDbType.data());
				continue;
			}
			ref.dw_desc = psubNode->GetAttribute("dw_desc");
			ref.cls_name = psubNode->GetAttribute("cls_name");
			//��ʱ�̶�����ʱ
			if (ref.dbtype == "1")
			{
				int irowNode = psubNode->GetChildCount("row");
				for (int j = 0; j < irowNode; j++)
				{
					SBaseConfig *pRowNode = psubNode->GetChildNode(j, "row");
					if (pRowNode == NULL)
					{
						//delete pRow;
						continue;
					}
					myRow *pRow = new myRow();	
					pRow->id=pRowNode->GetAttribute("row_id");
					pRow->name=pRowNode->GetAttribute("row_name");
					ref.myRowList.append(pRow);
					ref.myRowList.setAutoDelete(true);
				}
			} else {
				//��sql����п��ܴ��ڵ�ת���ַ�����ת��
				ref.dw_select = psubNode->GetChildNode("select")->GetNodeValue();
				ref.dw_select.replace("&lt;=","<=");//ת��
				ref.dw_select.replace("&gt;",">");//ת��
				//ref.dw_select.replace("\'","\\\'");
				ref.dw_select.replace("'","''");
			}
			//�Ի�ȡ�����ýڵ���д���
			if(!handleOneRefNode(curMaxdwsn, &ref))
			{
				//������ݿ����ʧ�ܣ���Ӧ��ֹͣ��������
				break;
			}
		} else if (psubNode->GetNodeName() == "dataset")
		{
			myDataset curDataSet;
			curDataSet.name = psubNode->GetAttribute("name");
			SString strDbType=psubNode->GetAttribute("dbtype");
			if(strDbType == "db")
				curDataSet.dbtype = 2;
			else if(strDbType == "mdb")
				curDataSet.dbtype = 3;
			else
			{
				LOGWARN("Unknown reference[%s] type=%s!",curDataSet.name.data(),strDbType.data());
				continue;
			}
			curDataSet.dw_desc = psubNode->GetAttribute("dw_desc");
			//curDataSet.dw_select = psubNode->GetAttribute("name");
			curDataSet.transform_type = psubNode->GetAttribute("transform_type").toInt();
			curDataSet.cross_refname = psubNode->GetAttribute("cross_refname");
			curDataSet.cross_column = psubNode->GetAttribute("cross_column").toInt();
			curDataSet.cross_data_column = psubNode->GetAttribute("cross_data_column").toInt();
			curDataSet.cls_name = psubNode->GetAttribute("cls_name");
			curDataSet.ext_attr = psubNode->GetAttribute("ext_attr");
			SBaseConfig * pSubSelNode = psubNode->GetChildNode(0,"select");
			if (pSubSelNode == NULL)
			{
				continue;
			}
			curDataSet.dw_select = pSubSelNode->GetNodeValue();
			curDataSet.dw_select.replace("&lt;","<");//ת��
			curDataSet.dw_select.replace("&gt;",">");//ת��
			curDataSet.dw_select.replace("'","''");
			//��ȡ�ж�������
			SBaseConfig * pSubColNode = psubNode->GetChildNode(0,"columns");
			if (pSubColNode != NULL)
			{
				int icolnum = pSubColNode->GetChildCount("col");
				for (int i = 0; i < icolnum; i++)
				{
					SBaseConfig *pNode = pSubColNode->GetChildNode(i, "col");
					if (pNode == NULL)
					{
						//delete pRow;
						continue;
					}
					myCol *pColSet = new myCol();
					pColSet->col_name = pNode->GetAttribute("col_name");
					pColSet->col_vtype = pNode->GetAttribute("col_vtype");
					pColSet->col_width = pNode->GetAttribute("col_width").toInt();
					pColSet->ref_name = pNode->GetAttribute("ref_name");
					pColSet->conv = pNode->GetAttribute("conv");
					pColSet->pkey = pNode->GetAttribute("pkey").toInt();
					pColSet->isnull = pNode->GetAttribute("isnull").toInt();
					pColSet->ukey=pNode->GetAttribute("ukey").toInt();
					pColSet->ext_attr=pNode->GetAttribute("ext_attr");
					curDataSet.myColList.append(pColSet);
					curDataSet.myColList.setAutoDelete(true);
				}
			}
			
			//��ȡ������������
			SBaseConfig * pSubConditonNode = psubNode->GetChildNode(0,"condition");
			if (pSubConditonNode != NULL)
			{
				int iconditionnum = pSubConditonNode->GetChildCount("con");
				for (int i = 0; i < iconditionnum; i++)
				{
					SBaseConfig *pNode = pSubConditonNode->GetChildNode(i, "con");
					if (pNode == NULL)
					{
						//delete pRow;
						continue;
					}
					myCondition *pConditionSet = new myCondition();
					pConditionSet->con_name = pNode->GetAttribute("con_name");
					pConditionSet->is_option = pNode->GetAttribute("is_option").toInt();
					pConditionSet->cmp_type = pNode->GetAttribute("cmp_type").toInt();
					pConditionSet->input_type = pNode->GetAttribute("input_type").toInt();
					pConditionSet->ref_name = pNode->GetAttribute("ref_name");
					pConditionSet->def_value = pNode->GetAttribute("def_value");
					pConditionSet->dyn_where = pNode->GetAttribute("dyn_where");
					curDataSet.myConditionList.append(pConditionSet);
					curDataSet.myConditionList.setAutoDelete(true);
				}
			}
			
			if (!handleOneDataNode(curMaxdwsn,&curDataSet))
			{
				break;
			}
			
		}
	}
#endif
	
	refreshLeftTable();//ˢ�������
	SSP_GUI->HideWaiting();
}
//����
void view_datawindow_edit::onPushButton_export()
{
	OutputCfg dlg(this);
	dlg.exec();

}
//Ӧ��
void view_datawindow_edit::onPushButton_apply()
{
	SSP_GUI->ShowWaiting();
	SQt::ProcessUiEvents();
	CSsp_DatawindowMgr *pOld = GET_DW_CFG;
	pOld->LoadByDb();
// 	CSsp_DatawindowMgr *pDw = new CSsp_DatawindowMgr();
// 	pDw->SetDbOper(DB);
// 	pDw->SetMdbOper(MDB);
// 	pDw->LoadByDb();
// 	CSsp_DatawindowMgr *pOld = GET_DW_CFG;
// 	CConfigMgr::RemoveConfig(pOld,true);
	SSP_GUI->HideWaiting();
}

//ѡ�������е���ɾ������
void view_datawindow_edit::ref2ItemCilcked(QTableWidgetItem *item,QTableWidgetItem *pre)
{
	S_UNUSED(item);
	S_UNUSED(pre);
	int row = ui.tableWidget_ref1->currentRow();
	if (row == -1)
	{
		return;
	}
	QString strDbType = ui.tableWidget_ref1->item(row,2)->text();
	if (strDbType != "�̶�")
	{
		ui.pushButton_ref2_del->setEnabled(false);
	} else {
		ui.pushButton_ref2_del->setEnabled(true);
	}
	
}
//ѡ�����ݼ��е���ɾ������
void view_datawindow_edit::data2ItemCilcked(QTableWidgetItem *item,QTableWidgetItem *pre)
{
	S_UNUSED(item);
	S_UNUSED(pre);
	ui.pushButton_data2_del->setEnabled(true);
}
//�л�������Щ��ť����
void view_datawindow_edit::tabChanged(int num)
{
	if(num==0)
	{
		if(ui.tableWidget_ref1->currentRow()< 0)
		{
			ui.pushButton_copy->setEnabled(false);
			ui.pushButton_del->setEnabled(false);
			ui.pushButton_view->setEnabled(false);
			ui.pushButton_ref2_del->setEnabled(false);
		}
		else
		{
			ui.pushButton_copy->setEnabled(true);
			if (ui.tableWidget_ref1->currentRow() < 0)
			{
				ui.pushButton_del->setEnabled(false);
			} else {
				ui.pushButton_del->setEnabled(true);
			}
			//ui.pushButton_del->setEnabled(true);
			ui.pushButton_view->setEnabled(false);
			if (ui.tableWidget_ref2->currentRow() < 0)
			{
				ui.pushButton_ref2_del->setEnabled(false);
			} else {
				ui.pushButton_ref2_del->setEnabled(true);
			}
		}
		
	}
	else if(num==1)
	{
		if(ui.tableWidget_data1->currentRow() < 0)
		{
			ui.pushButton_copy->setEnabled(false);
			ui.pushButton_del->setEnabled(false);
			ui.pushButton_view->setEnabled(false);
			ui.pushButton_data2_del->setEnabled(false);
		}
		else
		{
			ui.pushButton_copy->setEnabled(true);
			if (ui.tableWidget_data1->currentRow() < 0)
			{
				ui.pushButton_del->setEnabled(false);
				ui.pushButton_view->setEnabled(false);
			} else {
				ui.pushButton_del->setEnabled(true);
				ui.pushButton_view->setEnabled(true);
			}
		
			//ui.pushButton_view->setEnabled(true);
			if(ui.tableWidget_data2->currentRow()< 0)
			{
				ui.pushButton_data2_del->setEnabled(false);
			} else {
				ui.pushButton_data2_del->setEnabled(true);
			}
			viewDwConEdit->refreshDataRef();			
		}
		////if(ui.tableWidget_data2->currentRow()<=0)
		//{
		//	ui.pushButton_data2_del->setEnabled(false);
		//}
	}
}
//�����������������Ҳ���ʾsql����ֵ
void view_datawindow_edit::showRefInfo(QTableWidgetItem *item,QTableWidgetItem *pre)
{
	S_UNUSED(pre);
	setRefBlockSignals(true);

	m_rsLeftTable.clear();
	m_rsRow.clear();
	//ui.tableWidget_ref2->clear();
	ui.textEdit_ref->clear();
	ui.tableWidget_ref2->clearContents();
	ui.tableWidget_ref2->setRowCount(0);

	ui.pushButton_copy->setEnabled(true);
	ui.pushButton_del->setEnabled(true);
	ui.pushButton_view->setEnabled(false);
	ui.pushButton_ref2_del->setEnabled(false);
	//ui.pushButton_ref2_add->setEnabled(true);
	//ui.pushButton_ref2_del->setEnabled(true);

	SString sn=ui.tableWidget_ref1->item(item->row(),ref1_sn)->text().toStdString().data();
	QString dbType=ui.tableWidget_ref1->item(item->row(),2)->text();
	if (dbType == "�̶�")
	{
		ui.pushButton_ref2_add->setEnabled(true);
	} else {
		ui.pushButton_ref2_add->setEnabled(false);
	}

	ui.pushButton_ref2_del->setEnabled(false);

	SString sql = SString::toFormat("select dw_sn,dstype,name,dbtype,dw_desc,dw_select from t_ssp_data_window where dstype=1 and dw_sn=%s order by cls_name desc,dw_desc",sn.data());
	int ret = pDb->RetrieveRecordset(sql, m_rsLeftTable);
	if(ret < 0)
	{
		QMessageBox::information(this,tr("����"),tr("��ȡ�����ݿ�ʧ��[%1]").arg(sql.data()),QMessageBox::Ok);
		setRefBlockSignals(false);
		return;
	} else if (ret == 0)
	{
		setRefBlockSignals(false);
		return;
	} else if (ret > 1)
	{
		QMessageBox::information(this,tr("����"),tr("���ݿ���ڶ���"),QMessageBox::Ok);
		setRefBlockSignals(false);
		return;
	}

	ui.textEdit_ref->setText(m_rsLeftTable.GetValue(0,5).data());
	if (dbType == "�̶�" )
	{
		sql = SString::toFormat("select dw_sn,row_sn,row_id,row_name from t_ssp_data_window_row where dw_sn=%s order by row_sn ",sn.data());
		ret = pDb->RetrieveRecordset(sql, m_rsRow);
		if(ret < 0)
		{
			QMessageBox::information(this,tr("����"),tr("��ȡ�����ݿ�������ʧ��[%1]").arg(sql.data()),QMessageBox::Ok);
			setRefBlockSignals(false);
			return;
		}else if (ret == 0)
		{
			setRefBlockSignals(false);
			return;
		}

		for(int i=0;i<m_rsRow.GetRows();i++)
		{
			ui.tableWidget_ref2->insertRow(i);
			QTableWidgetItem *item=new QTableWidgetItem(m_rsRow.GetValue(i,3).data());//����
			ui.tableWidget_ref2->setItem(i,1,item);

			item=new QTableWidgetItem(m_rsRow.GetValue(i,2).data());
			ui.tableWidget_ref2->setItem(i,0,item);//���

			item=new QTableWidgetItem(m_rsRow.GetValue(i,0).data());
			ui.tableWidget_ref2->setItem(i,2,item);//sn

			item=new QTableWidgetItem(m_rsRow.GetValue(i,1).data());
			ui.tableWidget_ref2->setItem(i,3,item);//row_sn
		}
		//��ԡ��̶�������table��ɱ���
		ui.tableWidget_ref2->setEditTriggers(QAbstractItemView::DoubleClicked);		//�ɱ༭
	} else 
	{
		SRecordset rs;
		//SDatabaseOper *pDb = DB;
		//QStringList header;
		sql = m_rsLeftTable.GetValue(0,5).data();
		bool bNeedReplace = true;
		while (bNeedReplace)
		{
			int iPosStart = sql.find("{");
			if (iPosStart >= 0)
			{
				int iPosEnd =  sql.find("}");
				SString strSub = sql.substr(iPosStart,iPosEnd-iPosStart+1);
				sql.replace(strSub,"");
			} else {
				bNeedReplace = false;
				break;
			}
		}
		ret = pDb->RetrieveRecordset(sql,rs);
		if (ret < 0)
		{
			QMessageBox::information(this,tr("����"),tr("��ȡ�����ݿ�������ʧ��[%1]").arg(sql.data()),QMessageBox::Ok);
			setRefBlockSignals(false);
			return;
		}else if (ret == 0)
		{
			setRefBlockSignals(false);
			return;
		}
		for(int i=0; i < rs.GetRows();i++)
		{
			ui.tableWidget_ref2->insertRow(i);

			QTableWidgetItem *item=new QTableWidgetItem(rs.GetValue(i,0).data());
			item->setFlags(item->flags()&~Qt::ItemIsEditable);
			ui.tableWidget_ref2->setItem(i,0,item);

			item=new QTableWidgetItem(rs.GetValue(i,1).data());
			ui.tableWidget_ref2->setItem(i,1,item);
		}
		ui.tableWidget_ref2->setEditTriggers(QAbstractItemView::NoEditTriggers);		//���ɱ༭
	}

	setRefBlockSignals(false);
	
	ui.tableWidget_ref2->resizeColumnsToContents();
	if (ui.tableWidget_ref2->rowCount() > 0)
	{
		ui.tableWidget_ref2->selectRow(0);
		ui.pushButton_ref2_del->setEnabled(true);
	} else {
		ui.pushButton_ref2_del->setEnabled(false);
	}
}
//���������ݼ��������Ҳ���ʾsql����ֵ
void view_datawindow_edit::showDataInfo(QTableWidgetItem *item,QTableWidgetItem *pre)
{
	S_UNUSED(pre);
	if (item == NULL)
	{
		return;
	}
	int curSelRow = item->row();
	if (curSelRow < 0)
	{
		QMessageBox::information(this,tr("����"),tr("δѡ�����������ѡ���������"),QMessageBox::Ok);
		return;
	}
	setDataBlockSignals(true);
	m_rsLeftTable.clear();
	m_rsCol.clear();

	ui.textEdit_data->clear();
	ui.tableWidget_data2->clearContents();
	ui.tableWidget_data2->setRowCount(0);
	ui.pushButton_data2_del->setEnabled(false);
	SString sn=ui.tableWidget_data1->item(curSelRow,COL_DATA1_SN)->text().toStdString().data();

	SString sql = SString::toFormat("select dw_sn,dstype,name,dbtype,dw_desc,dw_select from t_ssp_data_window where dstype=2 and dw_sn=%s order by cls_name desc,dw_desc",sn.data());
	int ret = pDb->RetrieveRecordset(sql, m_rsLeftTable);
	if(ret < 0)
	{
		QMessageBox::information(this,tr("����"),tr("��ȡ���ݿ�ʧ��[%1]").arg(sql.data()),QMessageBox::Ok);
		setDataBlockSignals(false);
		return;
	} else if (ret == 0)
	{
		setDataBlockSignals(false);
		return;
	} else if (ret > 1)
	{
		QMessageBox::information(this,tr("����"),tr("���ݿ���ڶ���"),QMessageBox::Ok);
		setDataBlockSignals(false);
		return;
	}
	QString strDwSelect = m_rsLeftTable.GetValue(0,5).data();
	ui.textEdit_data->setText(strDwSelect);
	QString strComboText = ui.comboBox_data_cls->currentText();

	sql = SString::toFormat("select dw_sn,col_sn,col_name,col_vtype,col_width,ref_name,conv,pkey,isnull,ukey,ext_attr from t_ssp_data_window_col where dw_sn=%s order by col_sn",sn.data());
	ret = pDb->RetrieveRecordset(sql,m_rsCol);
	if(ret < 0)
	{
		QMessageBox::information(this,tr("����"),tr("���ݿ��ѯʧ��[%1]��").arg(sql.data()),QMessageBox::Ok);
		setDataBlockSignals(false);
		return;
	} else if ( ret == 0)
	{
		setDataBlockSignals(false);
		return;
	}


	SRecordset refNameSets;
	pDb->RetrieveRecordset(SString::toFormat("select name,dw_desc from t_ssp_data_window where dstype=1 order by cls_name desc,dw_desc"),refNameSets);

	for(int i=0;i<m_rsCol.GetRows();i++)
	{
 		ui.tableWidget_data2->insertRow(i);
		item=new QTableWidgetItem(m_rsCol.GetValue(i,1).data());
		//item->setFlags(item->flags()&~Qt::ItemIsEditable);
		ui.tableWidget_data2->setItem(i,0,item);//���

		QTableWidgetItem *item=new QTableWidgetItem(valuetypeTransform(m_rsCol.GetValue(i,3)).data());
		ui.tableWidget_data2->setItem(i,2,item);//����

		item=new QTableWidgetItem(m_rsCol.GetValue(i,2).data());
		ui.tableWidget_data2->setItem(i,1,item);//����

		item=new QTableWidgetItem();
		item->setCheckState(Qt::Unchecked);
		if(m_rsCol.GetValue(i,7)=="1")
			item->setCheckState(Qt::Checked);
		item->setFlags(item->flags()&~Qt::ItemIsEditable);
		ui.tableWidget_data2->setItem(i,3,item);//����

		item=new QTableWidgetItem(m_rsCol.GetValue(i,4).data());
		ui.tableWidget_data2->setItem(i,4,item);//���

		item=new QTableWidgetItem(refnameTransform(m_rsCol.GetValue(i,5),refNameSets).data());
		ui.tableWidget_data2->setItem(i,5,item);//��������

		item=new QTableWidgetItem(convStrToName(m_rsCol.GetValue(i,6)).data());
		ui.tableWidget_data2->setItem(i,6,item);//ת������
		item=new QTableWidgetItem(m_rsCol.GetValue(i,10).data());
		ui.tableWidget_data2->setItem(i,7,item);//ext_attr
		item=new QTableWidgetItem(m_rsCol.GetValue(i,1).data());
		ui.tableWidget_data2->setItem(i,8,item);//
		item=new QTableWidgetItem(m_rsCol.GetValue(i,0).data());
		ui.tableWidget_data2->setItem(i,COL_DATA2_SN,item);//		
	}
	setDataBlockSignals(false);
	ui.tableWidget_data2->resizeColumnsToContents();
	if (ui.tableWidget_data2->rowCount() > 0)
	{
		ui.tableWidget_data2->selectRow(0);
		ui.pushButton_data2_del->setEnabled(true);
	}else{
		ui.pushButton_data2_del->setEnabled(false);
	}
}
void view_datawindow_edit::showConInfo(QTableWidgetItem *item,QTableWidgetItem *pre)
{
	S_UNUSED(pre);
	if (item == NULL)
	{
		return;
	}
	int row=item->row();
	if (row < 0)
	{
		return;
	}
	QString strComBoxText = ui.comboBox_data_cls->currentText();
	bool bCheckState = ui.checkBox_useCls->isChecked() ? true:false;
	viewDwConEdit->refreshTable(ui.tableWidget_data1->item(row,COL_DATA1_SN)->text().toInt(),strComBoxText,bCheckState);
}
//�༭������Ϣ����
void view_datawindow_edit::editRef1(QTableWidgetItem *item)
{
	//ui.tableWidget_ref1->blockSignals(true);
 	int col=item->column();
	int row=item->row();
	bool flag =true;
	if(col==3)
	{
		flag=pDb->Execute(SString::toFormat("update t_ssp_data_window set dw_desc='%s' where dw_sn=%s",\
			ui.tableWidget_ref1->item(row,3)->text().toStdString().c_str(),ui.tableWidget_ref1->item(row,ref1_sn)->text().toStdString().c_str()));
	}
	else if(col==2)
	{
		flag=pDb->Execute(SString::toFormat("update t_ssp_data_window set dbtype=%s where dw_sn=%s",\
			typeTransform(ui.tableWidget_ref1->item(row,2)->text().toStdString().data()).data(),ui.tableWidget_ref1->item(row,ref1_sn)->text().toStdString().c_str()));
	}
	else if(col==1)
	{
		//�ж��µ�ǰ�������Ƿ�����ظ����
		SRecordset tmpset;
		SString sql = SString::toFormat("select name from t_ssp_data_window where name='%s' and dstype=1",
			ui.tableWidget_ref1->item(row,1)->text().toStdString().c_str());
		int ret = pDb->RetrieveRecordset(sql,tmpset);
		if (ret < 0)
		{
			QMessageBox::warning(this,tr("����"),tr("�����ݿ��ѯʧ�ܡ�%1��").arg(sql.data()),QMessageBox::Ok);
			return;
		} else if (ret > 0)
		{
			QMessageBox::warning(this,tr("����"),tr("��ǰ�����ظ������������룡"),QMessageBox::Ok);
			return;
		}
		flag=pDb->Execute(SString::toFormat("update t_ssp_data_window set name='%s' where dw_sn=%s",\
			ui.tableWidget_ref1->item(row,1)->text().toStdString().c_str(),ui.tableWidget_ref1->item(row,ref1_sn)->text().toStdString().c_str()));
	}
	else if(col==0)
	{
		flag=pDb->Execute(SString::toFormat("update t_ssp_data_window set cls_name='%s' where dw_sn=%s",\
			ui.tableWidget_ref1->item(row,0)->text().toStdString().c_str(),ui.tableWidget_ref1->item(row,ref1_sn)->text().toStdString().c_str()));
		if(flag==false)
		{
			QMessageBox::warning(this,tr("����"),tr("�����ݿ��������ʧ��"),QMessageBox::Ok);
			//ui.tableWidget_ref1->blockSignals(false);
			return;
		}
		//���ﱣ������ҳԭ��comboѡ�����Ͳ���
		QString strCurRefComboText = ui.comboBox_ref_cls->currentText();
		initRefCombo();
		ui.comboBox_ref_cls->blockSignals(true);
		ui.comboBox_ref_cls->setEditText(strCurRefComboText);
		ui.comboBox_ref_cls->blockSignals(false);
	}
	if(flag==false)
	{
		QMessageBox::warning(this,tr("����"),tr("�����ݿ��������ʧ��"),QMessageBox::Ok);
		//ui.tableWidget_ref1->blockSignals(false);
		return;
	}
	//ui.tableWidget_ref1->blockSignals(false);
}
//�༭��������Ϣ����
void view_datawindow_edit::editRef2(QTableWidgetItem *item)
{
	ui.tableWidget_ref2->blockSignals(true);
	int col=item->column();
	int row=item->row();
	QString strDwsn=ui.tableWidget_ref2->item(row,ref2_sn)->text();
	QString strRowsn = ui.tableWidget_ref2->item(row,COL_REF2_ROW_SN)->text();

	SString sql;
	if(col==0)
	{
		sql = SString::toFormat("update t_ssp_data_window_row set row_id='%s' where dw_sn=%s and row_sn=%s",
			ui.tableWidget_ref2->item(row,0)->text().toStdString().c_str(),strDwsn.toStdString().c_str(),strRowsn.toStdString().c_str());
	}
	if(col==1)
	{
		sql = SString::toFormat("update t_ssp_data_window_row set row_name='%s' where dw_sn=%s and row_sn=%s",
			ui.tableWidget_ref2->item(row,1)->text().toStdString().c_str(),strDwsn.toStdString().c_str(), strRowsn.toStdString().c_str());
	}
	bool flag=pDb->Execute(sql);
	if(flag==false)
	{
		QMessageBox::warning(this,tr("����"),tr("�����ݿ�������ö�Ӧ��ʧ��[%1]").arg(sql.data()),QMessageBox::Ok);
		ui.tableWidget_ref2->blockSignals(false);
		return;
	}
	ui.tableWidget_ref2->blockSignals(false);
}
//�༭���ݼ���Ϣ����
void view_datawindow_edit::editData1(QTableWidgetItem *item)
{
	int col=item->column();
	int row=item->row();
	//ui.tableWidget_data1->blockSignals(true);
	SString sql;
	if(col==0)
	{
		sql = SString::toFormat("update t_ssp_data_window set cls_name='%s' where dw_sn=%s",\
			ui.tableWidget_data1->item(row,0)->text().toStdString().c_str(),ui.tableWidget_data1->item(row,data1_sn)->text().toStdString().data());
		bool flag=pDb->Execute(sql);
		if(flag==false)
		{
			QMessageBox::warning(this,tr("����"),tr("�����ݿ�������ݼ�ʧ��[%1]").arg(sql.data()),QMessageBox::Ok);
			//ui.tableWidget_data1->blockSignals(false);
			return;
		}

		//���ﱣ������ҳԭ��comboѡ�����Ͳ���
		QString strCurDataComboText = ui.comboBox_data_cls->currentText();
		initDataCombo();
		if (strCurDataComboText != "ȫ��")
		{
			ui.comboBox_ref_cls->setEditText(strCurDataComboText);
		}  else 
		{
			ui.comboBox_ref_cls->blockSignals(true);
			ui.comboBox_ref_cls->setEditText(strCurDataComboText);
			ui.comboBox_ref_cls->blockSignals(false);
		}
		return;
		
	}
	if(col==1)
	{
		SRecordset tmpset;
		sql = SString::toFormat("select name from t_ssp_data_window where name='%s' and dstype=2",
			ui.tableWidget_data1->item(row,1)->text().toStdString().c_str());
		int ret = pDb->RetrieveRecordset(sql,tmpset);
		if (ret < 0)
		{
			QMessageBox::warning(this,tr("����"),tr("�����ݿ��ѯʧ�ܡ�%1��").arg(sql.data()),QMessageBox::Ok);
			return;
		} else if (ret > 0)
		{
			QMessageBox::warning(this,tr("����"),tr("��ǰ�����ظ������������룡"),QMessageBox::Ok);
			return;
		}
		sql = SString::toFormat("update t_ssp_data_window set name='%s' where dw_sn=%s",\
			ui.tableWidget_data1->item(row,1)->text().toStdString().c_str(),ui.tableWidget_data1->item(row,data1_sn)->text().toStdString().data());
	}
	if(col==2)
	{
		sql = SString::toFormat("update t_ssp_data_window set dbtype=%s where dw_sn=%s",\
			typeTransform(ui.tableWidget_data1->item(row,2)->text().toStdString().data()).data(),ui.tableWidget_data1->item(row,data1_sn)->text().toStdString().data());
	}
	if(col==3)
	{
		sql = SString::toFormat("update t_ssp_data_window set dw_desc='%s' where dw_sn=%s",\
			ui.tableWidget_data1->item(row,3)->text().toStdString().c_str(),ui.tableWidget_data1->item(row,data1_sn)->text().toStdString().data());
	}

	if(col==4)
	{
		sql = SString::toFormat("update t_ssp_data_window set transform_type=%s where dw_sn=%s",\
			tranftranftype(ui.tableWidget_data1->item(row,col)->text().toStdString().data()).data(),ui.tableWidget_data1->item(row,data1_sn)->text().toStdString().data());
	}

	if(col==5)
	{
		sql = SString::toFormat("update t_ssp_data_window set cross_refname='%s' where dw_sn=%s",\
			ui.tableWidget_data1->item(row,col)->text().toStdString().c_str(),ui.tableWidget_data1->item(row,data1_sn)->text().toStdString().data());
	}

	if(col==6)
	{
		sql = SString::toFormat("update t_ssp_data_window set cross_column=%s where dw_sn=%s",\
			ui.tableWidget_data1->item(row,col)->text().toStdString().c_str(),ui.tableWidget_data1->item(row,data1_sn)->text().toStdString().data());
	}

	if(col==7)
	{
		sql = SString::toFormat("update t_ssp_data_window set cross_data_column=%s where dw_sn=%s",\
			ui.tableWidget_data1->item(row,col)->text().toStdString().c_str(),ui.tableWidget_data1->item(row,data1_sn)->text().toStdString().data());
	}
	if(col==8)
	{
		sql = SString::toFormat("update t_ssp_data_window set ext_attr='%s' where dw_sn=%s",\
			ui.tableWidget_data1->item(row,col)->text().toStdString().c_str(),ui.tableWidget_data1->item(row,data1_sn)->text().toStdString().data());
		
	}	
	bool flag=pDb->Execute(sql);
	if(flag==false)
	{
		QMessageBox::warning(this,tr("����"),tr("�����ݿ�������ݼ�ʧ��[%1]").arg(sql.data()),QMessageBox::Ok);
		ui.tableWidget_data1->blockSignals(false);
		return;
	}
	ui.tableWidget_data1->resizeColumnsToContents();
}
//�༭���ݼ�����Ϣ����
void view_datawindow_edit::editData2(QTableWidgetItem *item)
{
	QString strTest = item->text();
	int col=item->column();
	int row=item->row();
	ui.tableWidget_data2->blockSignals(true);
	QString sn=ui.tableWidget_data2->item(row,COL_DATA2_SN)->text();
	SRecordset sets;
	SString sql;
	if(col==0)//���
	{
		//�ر�����Ҫ�жϵ�ǰ�޸ĵ�����Ƿ���ڳ�ͻ
		QString oldno=ui.tableWidget_data2->item(row,8)->text();
		sql = SString::toFormat("select * from t_ssp_data_window_col where dw_sn=%s and col_sn=%s",ui.tableWidget_data2->item(row,COL_DATA2_SN)->text().toStdString().data(),item->text().toStdString().c_str());
		SRecordset tempset;
		int ret = pDb->RetrieveRecordset(sql,tempset);
		if (ret < 0)
		{
			QMessageBox::warning(this,tr("����"),tr("��ѯ���ݿ����ݼ�ʧ��[%1]").arg(sql.data()),QMessageBox::Ok);
			ui.tableWidget_data2->item(row,0)->setText(oldno);
			ui.tableWidget_data2->blockSignals(false);
			return;
		} else if ( ret > 0)
		{
			QMessageBox::warning(this,tr("����"),tr("��ǰ����Ŵ��ڳ�ͻ���������޸ģ�"),QMessageBox::Ok);
			ui.tableWidget_data2->item(row,0)->setText(oldno);
			ui.tableWidget_data2->blockSignals(false);
			return;
		}
		sql = SString::toFormat("update t_ssp_data_window_col set col_sn=%d where dw_sn=%s and col_sn=%s",ui.tableWidget_data2->item(row,0)->text().toInt(),ui.tableWidget_data2->item(row,COL_DATA2_SN)->text().toStdString().data(),oldno.toStdString().data());
		bool flag=pDb->Execute(sql);
		if(flag==false)
		{
			QMessageBox::warning(this,tr("����"),tr("�����ݿ�������ݼ�������ʧ��[%1]").arg(sql.data()),QMessageBox::Ok);
			ui.tableWidget_data2->item(row,0)->setText(oldno);
			ui.tableWidget_data2->blockSignals(false);
			return;
		}
		ui.tableWidget_data2->item(row,8)->setText(ui.tableWidget_data2->item(row,0)->text());
	}

	else if(col==2)//����
	{
		QString name=ui.tableWidget_data2->item(row,2)->text();
		sql = SString::toFormat("update t_ssp_data_window_col set col_vtype='%s' where dw_sn=%s and col_sn=%s",\
			valuetypeTransform(ui.tableWidget_data2->item(row,2)->text().toStdString().data()).data(),ui.tableWidget_data2->item(row,COL_DATA2_SN)->text().toStdString().data(),ui.tableWidget_data2->item(row,0)->text().toStdString().data());
	}
	else if(col==1)//����
	{
		sql = SString::toFormat("update t_ssp_data_window_col set col_name='%s' where dw_sn=%s and col_sn=%s",\
			ui.tableWidget_data2->item(row,1)->text().toStdString().c_str(),ui.tableWidget_data2->item(row,COL_DATA2_SN)->text().toStdString().c_str(),ui.tableWidget_data2->item(row,0)->text().toStdString().data());
	}
	else if(col==3)//����
	{
		const char* mkey="0";
		if(ui.tableWidget_data2->item(row,3)->checkState()==Qt::Checked)
			mkey="1";
		QString name=ui.tableWidget_data2->item(row,data2_sn)->text();
		sql = SString::toFormat("update t_ssp_data_window_col set pkey=%s where dw_sn=%s and col_sn=%s",\
			mkey,ui.tableWidget_data2->item(row,data2_sn)->text().toStdString().data(),ui.tableWidget_data2->item(row,0)->text().toStdString().data());
	}
	else if(col==4)//���
	{
		QString strColWidth = ui.tableWidget_data2->item(row,4)->text();
		strColWidth = strColWidth.isEmpty() ? "0":strColWidth;
		sql = SString::toFormat("update t_ssp_data_window_col set col_width=%s where dw_sn=%s and col_sn=%s",\
			strColWidth.toStdString().c_str(),ui.tableWidget_data2->item(row,data2_sn)->text().toStdString().data(),ui.tableWidget_data2->item(row,0)->text().toStdString().data());
		ui.tableWidget_data2->item(row,4)->setText(strColWidth);
	}

	else if(col==5)//������
	{
		SRecordset refNameSets;
		pDb->RetrieveRecordset(SString::toFormat("select name,dw_desc from t_ssp_data_window where dstype=1 order by cls_name desc,dw_desc"),refNameSets);
		sql = SString::toFormat("update t_ssp_data_window_col set ref_name='%s' where dw_sn=%s and col_sn=%s",\
			refnameTransform(ui.tableWidget_data2->item(row,5)->text().toStdString().data(),refNameSets).data(),ui.tableWidget_data2->item(row,data2_sn)->text().toStdString().data(),ui.tableWidget_data2->item(row,0)->text().toStdString().data());
	}

	else if(col==6)//ת������
	{
		sql = SString::toFormat("update t_ssp_data_window_col set conv='%s' where dw_sn=%s and col_sn=%s",\
			convNameToStr(ui.tableWidget_data2->item(row,6)->text().toStdString().data()).data(),ui.tableWidget_data2->item(row,data2_sn)->text().toStdString().data(),ui.tableWidget_data2->item(row,0)->text().toStdString().data());
	}

	else if(col==7)//��չ����
	{
		QString tmp1=ui.tableWidget_data2->item(row,7)->text();
		QString tmp2=ui.tableWidget_data2->item(row,data2_sn)->text();
		QString tmp3=ui.tableWidget_data2->item(row,0)->text();
		sql = SString::toFormat("update t_ssp_data_window_col set ext_attr='%s' where dw_sn=%s and col_sn=%s",\
			ui.tableWidget_data2->item(row,7)->text().toStdString().c_str(),ui.tableWidget_data2->item(row,data2_sn)->text().toStdString().c_str(),ui.tableWidget_data2->item(row,0)->text().toStdString().data());
	}
	bool flag=pDb->Execute(sql);
	if(flag==false)
	{
		QMessageBox::warning(this,tr("����"),tr("�����ݿ����ʧ��[%1]").arg(sql.data()),QMessageBox::Ok);
		ui.tableWidget_data2->blockSignals(false);
		return;
	}
	ui.tableWidget_data2->blockSignals(false);
	ui.tableWidget_data2->resizeColumnsToContents();
}
//����sql�޸�
void view_datawindow_edit::editRefText()
{
	int curRefRow=ui.tableWidget_ref1->currentRow();
	int type = pDb->SelectIntoI(SString::toFormat("select dbtype from t_ssp_data_window where dw_sn=%s",
		ui.tableWidget_ref1->item(curRefRow,ref1_sn)->text().toStdString().c_str()));
	if(type == 1)
		return;

// 	m_rsLeftTable.clear();
// 	pDb->RetrieveRecordset("select dw_sn,dstype,name,dbtype,dw_desc,dw_select from t_ssp_data_window order by cls_name desc,dw_desc",m_rsLeftTable);
// 	
// 	if(m_rsLeftTable.GetValue(curRefRow,3)=="1")
// 		return;
	QString str=ui.textEdit_ref->toPlainText();
	str.replace("'","''");
	/*bool flag=*/pDb->Execute(SString::toFormat("update t_ssp_data_window set dw_select='%s' where dw_sn=%s",\
		str.toStdString().c_str(),ui.tableWidget_ref1->item(curRefRow,ref1_sn)->text().toStdString().c_str()));
// 	if(flag==false)
// 	{
// 		QMessageBox::warning(this,tr("����"),tr("�����ݿ��������SQL���ʧ��"),QMessageBox::Ok);
// 		return;
// 	}
}
//���ݼ�sql�޸�
void view_datawindow_edit::editDataText()
{
	int curdataRow=ui.tableWidget_data1->currentRow();
	QString str=ui.textEdit_data->toPlainText();
	str.replace("'","''");
	/*bool flag=*/pDb->Execute(SString::toFormat("update t_ssp_data_window set dw_select='%s' where dw_sn=%s",\
		str.toStdString().c_str(),ui.tableWidget_data1->item(curdataRow,data1_sn)->text().toStdString().data()));
// 	if(flag==false)
// 	{
// 		QMessageBox::warning(this,tr("����"),tr("�����ݿ�������ݼ�SQL���ʧ��"),QMessageBox::Ok);
// 		return;
// 	}

}
//����Ƿ�Ϊ��
char* view_datawindow_edit::checkNull(char* num)
{
	if(strlen(num)==0)
		num=(char*)"0";
	return num;	
	if(SString::equals((const char*)num,"false"))
		return (char*)"0";
	if(SString::equals((const char*)num,"true"))
		return (char*)"1";
}

//����ת��
SString view_datawindow_edit::refnameTransform(SString str,SRecordset &datasets )
{
	if(datasets.GetColumns()!=2)
		return "";
	if(str=="")
		return "";
	for(int i=0;i<datasets.GetRows();i++)
	{
		if(str==datasets.GetValue(i,0))
			return datasets.GetValue(i,1);
		else if(str==datasets.GetValue(i,1))
			return datasets.GetValue(i,0);
	}
	return "";
}
SString view_datawindow_edit::convStrToName(SString str)//����ת��
{
	if(str=="")
		return "";
	else if(str=="soc2time")
		return ("������תΪ����ʱ��(yyyy-MM-dd hh:mm:ss)");
	else if(str=="soc2time2")
		return ("������תΪ����ʱ��(yyyy��MM��dd�� hhʱmm��ss��)");
	else if(str=="soc2time3")
		return ("������תΪ����ʱ��(hh:mm:ss)");
	else if(str=="soc2time4")
		return ("������תΪ����ʱ��(hhʱmm��ss��)");
	else if(str=="soc2date")
		return ("������תΪ����ʱ��(yyyy-MM-dd)");
	else if(str=="soc2date2")
		return ("������תΪ����ʱ��(yyyy��MM��dd��)");
	else if(str=="soc2now")
		return ("ָ��ʱ�䵽����ʱ��Ĳ�");
	else if(str=="int2intk")
		return ("����תΪ1024�ı�����ʾ����дֵ");
	else if(str=="int2thousands")
		return ("������Ϊǧλ�����");
	else if(str=="int2ip")
		return ("����ת��ΪIP��ַ�ַ���");
	return "";
}
SString view_datawindow_edit::convNameToStr(SString name)//����ת��
{
	if(name=="")
		return "";
	else if(name==("������תΪ����ʱ��(yyyy-MM-dd hh:mm:ss)"))
		return "soc2time";
	else if(name==("������תΪ����ʱ��(yyyy��MM��dd�� hhʱmm��ss��)"))
		return "soc2time2";
	else if(name==("������תΪ����ʱ��(hh:mm:ss)"))
		return "soc2time3";
	else if(name==("������תΪ����ʱ��(hhʱmm��ss��)"))
		return "soc2time4";
	else if(name==("������תΪ����ʱ��(yyyy-MM-dd)"))
		return "soc2date";
	else if(name==("������תΪ����ʱ��(yyyy��MM��dd��)"))
		return "soc2date2";
	else if(name=="ָ��ʱ�䵽����ʱ��Ĳ�")
		return ("soc2now");
	else if(name==("����תΪ1024�ı�����ʾ����дֵ"))
		return "int2intk";
	else if(name==("������Ϊǧλ�����"))
		return "int2thousands";
	else if(name==("����ת��ΪIP��ַ�ַ���"))
		return "int2ip";
	return "";
}
//����ת��
SString view_datawindow_edit::valuetypeTransform(SString str)
{
	if(str=="string")
		return "�ַ���";
	if(str=="float")
		return "������";
	if(str=="int")
		return "������";
	if(str=="�ַ���")
		return "string";
	if(str=="������")
		return "float";
	if(str=="������")
		return "int";
	return "";
}
SString view_datawindow_edit::tranftranftype(SString str)
{
	if(str=="0")
		return "���任";
	if(str=="1")
		return "��������ת��";
	if(str=="2")
		return "������ת��";
	if(str=="3")
		return "����任";
	if(str=="���任")
		return "0";
	if(str=="��������ת��")
		return "1";
	if(str=="������ת��")
		return "2";
	if(str=="����任")
		return "3";
	return "";
}
//����ת��
SString view_datawindow_edit::typeTransform(SString str)
{
	if(str=="1")
		return "�̶�";
	if(str=="2")
		return "��ʷ��";
	if(str=="3")
		return "�ڴ��";
	if(str=="fixed")
		return "1";
	if(str=="db")
		return "2";
	if(str=="mdb")
		return "3";
	if(str=="�̶�")
		return "1";
	if(str=="��ʷ��")
		return "2";
	if(str=="�ڴ��")
		return "3";
	return "";
}

void view_datawindow_edit::slotOnCheckBoxClick()
{
	int row = ui.tableWidget_data1->currentRow();
	if (row < 0)
	{
		QMessageBox::warning(this,tr("����"),tr("��ǰ���ݼ�������ѡ���У�����ѡ���У�"),QMessageBox::Ok);
		return;
	}

	QTableWidgetItem* item = ui.tableWidget_data1->item(row,0);
	if (item)
	{
		QString strComBoxText = ui.comboBox_data_cls->currentText();
		bool bCheckState = ui.checkBox_useCls->isChecked();
		viewDwConEdit->setComboTextAndCheckState(strComBoxText,bCheckState);
	}
		
}
//////////////////////////////////////////////////////////////////////

dataWindowDelegate::dataWindowDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
{
	this->parent=static_cast<view_datawindow_edit*>(parent);
	pDb=DB;
}



QWidget* dataWindowDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	switch(index.column())
	{
	case 2:
		{
			QComboBox* editor = new QComboBox(parent);
			editor->insertItem(0,"�ַ���");
			editor->insertItem(1,"������");
			editor->insertItem(2,"������");
			return editor;
		}
		break;
	case 5:
		{
			SRecordset m_rsLeftTable;
			QString strComText;
			bool bIsChecked;
			SString sql;
			this->parent->getComboxAndCheckState(strComText,bIsChecked);
			if (strComText != "ȫ��" && bIsChecked)
			{
				sql = SString::toFormat("select dw_desc from t_ssp_data_window where dstype=1 and cls_name='%s' order by cls_name desc,dw_desc",strComText.toStdString().c_str());
			} else {
				sql = "select dw_desc from t_ssp_data_window where dstype=1 and dw_desc is not null order by cls_name desc,dw_desc";
			}

			pDb->RetrieveRecordset(sql,m_rsLeftTable);
			QComboBox* editor = new QComboBox(parent);
			editor->insertItem(0,"");
			for(int k=0;k<m_rsLeftTable.GetRows();k++)
			{
				editor->insertItem(k+1,m_rsLeftTable.GetValue(k,0).data());
			}
			return editor;
		}
		break;
	case 6:
		{
			QComboBox* editor = new QComboBox(parent);
			editor->insertItem(0,"");
			editor->insertItem(1,"������תΪ����ʱ��(yyyy-MM-dd hh:mm:ss)");
			editor->insertItem(2,"������תΪ����ʱ��(yyyy��MM��dd�� hhʱmm��ss��)");
			editor->insertItem(3,"������תΪ����ʱ��(hh:mm:ss)");
			editor->insertItem(4,"������תΪ����ʱ��(hhʱmm��ss��)");
			editor->insertItem(5,"������תΪ����ʱ��(yyyy-MM-dd)");
			editor->insertItem(6,"ָ��ʱ�䵽����ʱ��Ĳ�");
			editor->insertItem(7,"������תΪ����ʱ��(yyyy��MM��dd��)");
			editor->insertItem(8,"����תΪ1024�ı�����ʾ����дֵ");
			editor->insertItem(9,"������Ϊǧλ�����");
			editor->insertItem(10,"����ת��ΪIP��ַ�ַ���");
			return editor;
		}
		break;
	}
	QWidget *widget=QStyledItemDelegate::createEditor(parent,option,index);
	return widget;
}

void dataWindowDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	QComboBox* comboEditor = qobject_cast<QComboBox* >(editor);
	if (comboEditor)
	{		
		QString text = index.model()->data(index,Qt::EditRole).toString();
		connect(comboEditor,SIGNAL(currentIndexChanged(QString)),this,SLOT(comboItemChanged(QString)));
		int index1 = comboEditor->findText(text);	

		comboEditor->setCurrentIndex(index1);	
		//int row=index.row();
		//int col=index.column();
	}

	QStyledItemDelegate::setEditorData(editor,index);

}
void dataWindowDelegate::comboItemChanged(QString text)
{
	parent->getTableData2()->currentItem()->setText(text);
}
void dataWindowDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QStyledItemDelegate::setModelData(editor,model,index);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
leftTableRefWindowDelegate::leftTableRefWindowDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
{
	this->parent=static_cast<view_datawindow_edit*>(parent);
}

QWidget* leftTableRefWindowDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	switch(index.column())
	{
	case 2:
		{
			QComboBox* editor = new QComboBox(parent);
			editor->insertItem(0,"�̶�");
			editor->insertItem(1,"��ʷ��");
			editor->insertItem(2,"�ڴ��");
			return editor;
		}
		break;
	}
	QWidget *widget=QStyledItemDelegate::createEditor(parent,option,index);
	return widget;
}

void leftTableRefWindowDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	QComboBox* comboEditor = qobject_cast<QComboBox* >(editor);
	if (comboEditor)
	{		
		QString text = index.model()->data(index,Qt::EditRole).toString();
		connect(comboEditor,SIGNAL(currentIndexChanged(QString)),this,SLOT(comboItemChanged(QString)));
		int index1 = comboEditor->findText(text);	

		comboEditor->setCurrentIndex(index1);	
		//int row=index.row();
		//int col=index.column();
	}

	QStyledItemDelegate::setEditorData(editor,index);

}
void leftTableRefWindowDelegate::comboItemChanged(QString text)
{
	parent->getTableRef1()->currentItem()->setText(text);
}
void leftTableRefWindowDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QStyledItemDelegate::setModelData(editor,model,index);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
leftTableDataWindowDelegate::leftTableDataWindowDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
{
	this->parent=static_cast<view_datawindow_edit*>(parent);
}

QWidget* leftTableDataWindowDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	switch(index.column())
	{
	case 2:
		{
			QComboBox* editor = new QComboBox(parent);
			editor->insertItem(0,"��ʷ��");
			editor->insertItem(1,"�ڴ��");
			return editor;
		}
		break;

	case 4:
		{
			QComboBox* editor = new QComboBox(parent);
			editor->insertItem(0,"���任");
			editor->insertItem(1,"��������ת��");
			editor->insertItem(3,"������ת��");
			editor->insertItem(4,"����任");
			return editor;
		}
		break;
	}
	QWidget *widget=QStyledItemDelegate::createEditor(parent,option,index);
	return widget;
}

void leftTableDataWindowDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	QComboBox* comboEditor = qobject_cast<QComboBox* >(editor);
	if (comboEditor)
	{		
		QString text = index.model()->data(index,Qt::EditRole).toString();
		connect(comboEditor,SIGNAL(currentIndexChanged(QString)),this,SLOT(comboItemChanged(QString)));
		int index1 = comboEditor->findText(text);	
		comboEditor->setCurrentIndex(index1);	
		//int row=index.row();
		//int col=index.column();
	}

	QStyledItemDelegate::setEditorData(editor,index);

}
void leftTableDataWindowDelegate::comboItemChanged(QString text)
{
	parent->getTableData1()->currentItem()->setText(text);
}
void leftTableDataWindowDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QStyledItemDelegate::setModelData(editor,model,index);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
OutputCfg::OutputCfg(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);
	SQt::SetTableWidgetStyle(tableWidget);
	tableWidget->horizontalHeader()->setStretchLastSection(true);
	tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	tableWidget->verticalHeader()->hide();
	connect(pushButton,SIGNAL(clicked()),this,SLOT(onPushButtonPressed()));
	connect(pushButton_2,SIGNAL(clicked()),this,SLOT(onPushButton2Pressed()));
	connect(tableWidget->horizontalHeader(),SIGNAL(sectionDoubleClicked(int )),this, SLOT(DoubleClickedTable(int)));
	pDb = DB;
	SString sql = "select cls_name,dw_sn,dstype,name,dbtype,dw_desc,dw_select,transform_type,cross_refname,cross_column,cross_data_column,ext_attr from t_ssp_data_window order by cls_name desc,dw_desc";
	int ret = pDb->RetrieveRecordset(sql,m_rsLeftTable);
	if (ret < 0)
	{
		QMessageBox::warning(this,tr("����"),tr("�����ݿ��ѯʧ��[%1]").arg(sql.data()),QMessageBox::Ok);
		return;
	}
	sql = "select dw_sn,row_sn,row_id,row_name from t_ssp_data_window_row order by dw_sn,row_sn";
	ret = pDb->RetrieveRecordset(sql,m_rsRow);
	if (ret < 0)
	{
		QMessageBox::warning(this,tr("����"),tr("�����ݿ��ѯʧ��[%1]").arg(sql.data()),QMessageBox::Ok);
		return;
	}
	sql = "select dw_sn,col_sn,col_name,col_vtype,col_width,ref_name,conv,pkey,isnull,ukey,ext_attr from t_ssp_data_window_col order by dw_sn,col_sn";
	ret = pDb->RetrieveRecordset(sql,m_rsCol);
	if (ret < 0)
	{
		QMessageBox::warning(this,tr("����"),tr("�����ݿ��ѯʧ��[%1]").arg(sql.data()),QMessageBox::Ok);
		return;
	}
	sql = "select dw_sn,con_sn,con_name,is_option,cmp_type,input_type,ref_name,def_value,dyn_where from t_ssp_data_window_condition order by dw_sn,con_sn";
	ret = pDb->RetrieveRecordset(sql,m_rsCondition);
	if (ret < 0)
	{
		QMessageBox::warning(this,tr("����"),tr("�����ݿ��ѯʧ��[%1]").arg(sql.data()),QMessageBox::Ok);
		return;
	}
	//����ʼ��
	for(int i=0;i<m_rsLeftTable.GetRows();i++)
	{
		tableWidget->insertRow(tableWidget->rowCount());
		QTableWidgetItem *item=new QTableWidgetItem();
		item->setCheckState(Qt::Unchecked);
		item->setText(m_rsLeftTable.GetValue(i,0).data());
		item->setFlags(item->flags() & ~Qt::ItemIsEditable);
		tableWidget->setItem(tableWidget->rowCount()-1,0,item);

		item=new QTableWidgetItem();
		item->setText(m_rsLeftTable.GetValue(i,1).data());
		
		item->setFlags(item->flags() & ~Qt::ItemIsEditable);
		tableWidget->setItem(tableWidget->rowCount()-1,1,item);

		item=new QTableWidgetItem();
		item->setText(tranftype(m_rsLeftTable.GetValue(i,2)).data());
		item->setFlags(item->flags() & ~Qt::ItemIsEditable);
		tableWidget->setItem(tableWidget->rowCount()-1,2,item);

		item=new QTableWidgetItem();
		item->setText(m_rsLeftTable.GetValue(i,3).data());
		item->setFlags(item->flags() & ~Qt::ItemIsEditable);
		tableWidget->setItem(tableWidget->rowCount()-1,3,item);

		item=new QTableWidgetItem();
		item->setText(tranfdbtype(m_rsLeftTable.GetValue(i,4)).data());
		item->setFlags(item->flags() & ~Qt::ItemIsEditable);
		tableWidget->setItem(tableWidget->rowCount()-1,4,item);

		item=new QTableWidgetItem();
		item->setText(m_rsLeftTable.GetValue(i,5).data());
		item->setFlags(item->flags() & ~Qt::ItemIsEditable);
		tableWidget->setItem(tableWidget->rowCount()-1,5,item);

		item=new QTableWidgetItem();
		item->setText(tranftranftype(m_rsLeftTable.GetValue(i,7)).data());
		item->setFlags(item->flags() & ~Qt::ItemIsEditable);
		tableWidget->setItem(tableWidget->rowCount()-1,6,item);

		item=new QTableWidgetItem();
		item->setText(m_rsLeftTable.GetValue(i,8).data());
		item->setFlags(item->flags() & ~Qt::ItemIsEditable);
		tableWidget->setItem(tableWidget->rowCount()-1,7,item);

		item=new QTableWidgetItem();
		item->setText(m_rsLeftTable.GetValue(i,9).data());
		item->setFlags(item->flags() & ~Qt::ItemIsEditable);
		tableWidget->setItem(tableWidget->rowCount()-1,8,item);

		item=new QTableWidgetItem();
		item->setText(m_rsLeftTable.GetValue(i,10).data());
		item->setFlags(item->flags() & ~Qt::ItemIsEditable);
		tableWidget->setItem(tableWidget->rowCount()-1,9,item);

		item=new QTableWidgetItem();
		item->setText(m_rsLeftTable.GetValue(i,11).data());
		item->setFlags(item->flags() & ~Qt::ItemIsEditable);
		tableWidget->setItem(tableWidget->rowCount()-1,10,item);
	}
}

OutputCfg::~OutputCfg()
{

}
void OutputCfg::DoubleClickedTable(int col)
{
	if(col!=0)
		return;
	int rowCnt=tableWidget->rowCount();
	if(tableWidget->item(0,col)->checkState()==Qt::Checked)
		for(int row=0;row<rowCnt;row++) 
			tableWidget->item(row,col)->setCheckState(Qt::Unchecked);
	else
		for(int row=0;row<rowCnt;row++) 
			tableWidget->item(row,col)->setCheckState(Qt::Checked);
}
//���Ϊxml
void OutputCfg::onPushButtonPressed()
{
	QString path=QFileDialog::getSaveFileName(this,"�����ļ�","./../conf/sys_datawindow.xml","*.xml",0,0);
	//QString path="D:\\softPlat\\trunk\\SSP01\\project\\win32-vc10\\sys_datawindow.xml";
	if(path=="")
		return;
	SXmlConfig pCfg;
	pCfg.SetNodeName("data_window");
	int ipcount = 0;
	for(int row=0;row<tableWidget->rowCount();row++)
	{
		if(tableWidget->item(row,0)->checkState()==Qt::Checked)
		{
			SXmlConfig *pCfgChild=new SXmlConfig();
			if(tableWidget->item(row,2)->text()==tr("����"))
			{
				pCfgChild->SetNodeName("reference");
				pCfgChild->SetAttribute("name",tableWidget->item(row,3)->text().toStdString().data());
				pCfgChild->SetAttribute("dbtype",tranfdbtype(tableWidget->item(row,4)->text().toStdString().data()));
				pCfgChild->SetAttribute("dw_desc",tableWidget->item(row,5)->text().toStdString().data());
				pCfgChild->SetAttribute("cls_name",tableWidget->item(row,0)->text().toStdString().data());
				if(tableWidget->item(row,4)->text()=="�̶�")
				{
					for(int j=0;j<m_rsRow.GetRows();j++)
					{
						if(m_rsRow.GetValue(j,0)==tableWidget->item(row,1)->text().toStdString().data())
						{
							SBaseConfig *rowNode=pCfgChild->AddChildNode("row");
							rowNode->SetAttribute("row_name",m_rsRow.GetValue(j,3));
							rowNode->SetAttribute("row_id",m_rsRow.GetValue(j,2));
						}
					}
				}
				else if(tableWidget->item(row,4)->text()=="��ʷ��"||tableWidget->item(row,4)->text()=="�ڴ��")
				{
					SBaseConfig *rowNode=pCfgChild->AddChildNode("select");
					int i=0;
					for(int j=0;j<m_rsLeftTable.GetRows();j++)
					{
						if(m_rsLeftTable.GetValue(j,1)==tableWidget->item(row,1)->text().toStdString().data())
						{
							i=j;
							break;
						}
					}
					SString sql=m_rsLeftTable.GetValue(i,6);
					sql.replace("<","&lt;");//ת��
					sql.replace(">","&gt;");//ת��
					//sql.replace("\'","&apos;");//ת��
					rowNode->SetNodeValue(sql);
				}
				
			}
			if(tableWidget->item(row,2)->text()=="���ݼ�")
			{
				pCfgChild->SetNodeName("dataset");
				pCfgChild->SetAttribute("name",tableWidget->item(row,3)->text().toStdString().data());
				pCfgChild->SetAttribute("dbtype",tranfdbtype(tableWidget->item(row,4)->text().toStdString().data()));
				pCfgChild->SetAttribute("dw_desc",tableWidget->item(row,5)->text().toStdString().data());
				pCfgChild->SetAttribute("transform_type",tranftranftype(tableWidget->item(row,6)->text().toStdString().data()));
				pCfgChild->SetAttribute("cross_refname",tableWidget->item(row,7)->text().toStdString().data());
				pCfgChild->SetAttribute("cross_column",tableWidget->item(row,8)->text().toStdString().data());
				pCfgChild->SetAttribute("cross_data_column",tableWidget->item(row,9)->text().toStdString().data());
				pCfgChild->SetAttribute("cls_name",tableWidget->item(row,0)->text().toStdString().data());
				pCfgChild->SetAttribute("ext_attr",tableWidget->item(row,10)->text().toStdString().data());
				SBaseConfig *rowNode=pCfgChild->AddChildNode("select");
				int i=0;
				for(int j=0;j<m_rsLeftTable.GetRows();j++)
				{
					if(m_rsLeftTable.GetValue(j,1)==tableWidget->item(row,1)->text().toStdString().data())
					{
						i=j;
						break;
					}
				}
				SString sql=m_rsLeftTable.GetValue(i,6);
				sql.replace("<","&lt;");//ת��
				sql.replace(">","&gt;");//ת��
				//sql.replace("\'","&apos;");//ת��
				rowNode->SetNodeValue(sql);
				bool flag=false;
				//�����ж�������Ϣ
				SBaseConfig *rowNodeR=NULL;
				for(int j=0;j<m_rsCol.GetRows();j++)
				{
					if(m_rsCol.GetValue(j,0)==tableWidget->item(row,1)->text().toStdString().data())
					{
						if(flag==false)
						{
							rowNodeR=pCfgChild->AddChildNode("columns");
							flag=true;
						}
						SBaseConfig *rowNode=rowNodeR->AddChildNode("col");
						rowNode->SetAttribute("col_name",m_rsCol.GetValue(j,2));
						rowNode->SetAttribute("col_vtype",m_rsCol.GetValue(j,3));
						rowNode->SetAttribute("col_width",m_rsCol.GetValue(j,4));
						rowNode->SetAttribute("ref_name",m_rsCol.GetValue(j,5));
						rowNode->SetAttribute("conv",m_rsCol.GetValue(j,6));
						rowNode->SetAttribute("pkey",m_rsCol.GetValue(j,7));
						rowNode->SetAttribute("isnull",m_rsCol.GetValue(j,8));
						rowNode->SetAttribute("ukey",m_rsCol.GetValue(j,9));
						rowNode->SetAttribute("ext_attr",m_rsCol.GetValue(j,10));
					}
				}
				//����������������Ϣ
				SBaseConfig *rowNodeC=NULL;
				flag = false;
				for(int j=0;j<m_rsCondition.GetRows();j++)
				{
					if(m_rsCondition.GetValue(j,0)==tableWidget->item(row,1)->text().toStdString().data())
					{
						if(flag==false)
						{
							rowNodeC=pCfgChild->AddChildNode("condition");
							flag=true;
						}
						SBaseConfig *rowNode=rowNodeC->AddChildNode("con");
						rowNode->SetAttribute("con_name",m_rsCondition.GetValue(j,2));
						rowNode->SetAttribute("is_option",m_rsCondition.GetValue(j,3));
						rowNode->SetAttribute("cmp_type",m_rsCondition.GetValue(j,4));
						rowNode->SetAttribute("input_type",m_rsCondition.GetValue(j,5));
						rowNode->SetAttribute("ref_name",m_rsCondition.GetValue(j,6));
						rowNode->SetAttribute("def_value",m_rsCondition.GetValue(j,7));
						rowNode->SetAttribute("dyn_where",m_rsCondition.GetValue(j,8));
					}
				}
			}
			ipcount++;
			pCfg.AddChildNode(pCfgChild);
		}

	}

	if(pCfg.SaveConfig(path.toStdString().data()))
		QMessageBox::information(this,tr("��ʾ"),tr("�����ɹ�"),QMessageBox::Ok);
	else
		QMessageBox::information(this,tr("��ʾ"),tr("����ʧ��"),QMessageBox::Ok);
	close();
}
void OutputCfg::onPushButton2Pressed()
{
	close();
}

void OutputCfg::setupUi(QWidget *OutputCfg)
{
	if (OutputCfg->objectName().isEmpty())
		OutputCfg->setObjectName(QString::fromUtf8("OutputCfg"));
	OutputCfg->resize(451, 375);
	gridLayout_2 = new QGridLayout(OutputCfg);
	gridLayout_2->setSpacing(5);
	gridLayout_2->setContentsMargins(4, 4, 4, 4);
	gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
	verticalLayout = new QVBoxLayout();
	verticalLayout->setSpacing(6);
	verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
	tableWidget = new QTableWidget(OutputCfg);
	if (tableWidget->columnCount() < 11)
		tableWidget->setColumnCount(11);
	QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
	tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
	QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
	tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
	QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
	tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
	QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
	tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
	QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
	tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
	QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
	tableWidget->setHorizontalHeaderItem(5, __qtablewidgetitem5);
	QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
	tableWidget->setHorizontalHeaderItem(6, __qtablewidgetitem6);
	QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
	tableWidget->setHorizontalHeaderItem(7, __qtablewidgetitem7);
	QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
	tableWidget->setHorizontalHeaderItem(8, __qtablewidgetitem8);
	QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
	tableWidget->setHorizontalHeaderItem(9, __qtablewidgetitem9);
	QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
	tableWidget->setHorizontalHeaderItem(10, __qtablewidgetitem10);
	tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
	verticalLayout->addWidget(tableWidget);
	widget = new QWidget(OutputCfg);
	widget->setObjectName(QString::fromUtf8("widget"));
	widget->setMaximumSize(QSize(16777215, 30));
	gridLayout = new QGridLayout(widget);
	gridLayout->setSpacing(4);
	gridLayout->setContentsMargins(0, 0, 0, 0);
	gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
	pushButton = new QPushButton(widget);
	pushButton->setObjectName(QString::fromUtf8("pushButton"));
	QIcon icon;
	icon.addFile(QString::fromUtf8(":/ssp_gui/Resources/export2.gif"), QSize(), QIcon::Normal, QIcon::Off);
	pushButton->setIcon(icon);
	gridLayout->addWidget(pushButton, 0, 0, 1, 1);
	pushButton_2 = new QPushButton(widget);
	pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
	QIcon icon1;
	icon1.addFile(QString::fromUtf8(":/ssp_gui/Resources/del.gif"), QSize(), QIcon::Normal, QIcon::Off);
	pushButton_2->setIcon(icon1);
	gridLayout->addWidget(pushButton_2, 0, 1, 1, 1);
	horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	gridLayout->addItem(horizontalSpacer, 0, 2, 1, 1);
	verticalLayout->addWidget(widget);
	gridLayout_2->addLayout(verticalLayout, 0, 0, 1, 1);
	retranslateUi(OutputCfg);
	QMetaObject::connectSlotsByName(OutputCfg);
	//ȥ���Ի����ϡ�������ť
	Qt::WindowFlags flags=Qt::Dialog;
	flags |=Qt::WindowCloseButtonHint;
	setWindowFlags(flags);
} // setupUi

void OutputCfg::retranslateUi(QWidget *OutputCfg)
{
	OutputCfg->setWindowTitle(tr("ѡ�����"));
	QTableWidgetItem *___qtablewidgetitem0 = tableWidget->horizontalHeaderItem(0);
	___qtablewidgetitem0->setText( tr("����"));
	QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(1);
	___qtablewidgetitem->setText( tr("���"));
	QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(2);
	___qtablewidgetitem1->setText( tr("����"));
	QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(3);
	___qtablewidgetitem2->setText(tr("����"));
	QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(4);
	___qtablewidgetitem3->setText(tr("��������"));
	QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(5);
	___qtablewidgetitem4->setText(tr("����"));
	QTableWidgetItem *___qtablewidgetitem5 = tableWidget->horizontalHeaderItem(6);
	___qtablewidgetitem5->setText( tr("���ݱ任����"));
	QTableWidgetItem *___qtablewidgetitem6 = tableWidget->horizontalHeaderItem(7);
	___qtablewidgetitem6->setText(tr("�������ݼ�������"));
	QTableWidgetItem *___qtablewidgetitem7 = tableWidget->horizontalHeaderItem(8);
	___qtablewidgetitem7->setText(tr("���������"));
	QTableWidgetItem *___qtablewidgetitem8 = tableWidget->horizontalHeaderItem(9);
	___qtablewidgetitem8->setText(tr("�������������"));
	QTableWidgetItem *___qtablewidgetitem9 = tableWidget->horizontalHeaderItem(10);
	___qtablewidgetitem9->setText(tr("��չ����"));
	pushButton->setText(tr("����(&O)"));
	pushButton_2->setText(tr("ȡ��(&C)"));
} // retranslateUi
SString OutputCfg::tranftranftype(SString str)
{
	if(str=="0")
		return "���任";
	if(str=="1")
		return "��������ת��";
	if(str=="2")
		return "������ת��";
	if(str=="3")
		return "����任";
	if(str=="���任")
		return "0";
	if(str=="��������ת��")
		return "1";
	if(str=="������ת��")
		return "2";
	if(str=="����任")
		return "3";
	return "";
}
SString OutputCfg::tranfdbtype(SString str)
{
	if(str=="1")
		return "�̶�";
	if(str=="2")
		return "��ʷ��";
	if(str=="3")
		return "�ڴ��";
	if(str=="�̶�")
		return "fixed";
	if(str=="��ʷ��")
		return "db";
	if(str=="�ڴ��")
		return "mdb";
	return "";
}
SString OutputCfg::tranftype(SString str)
{
	if(str=="1")
		return "����";
	if(str=="2")
		return "���ݼ�";
	if(str=="����")
		return "1";
	if(str=="���ݼ�")
		return "2";
	return "";
}

//////////////////////////////////////////////////
inputNum::inputNum(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);
	connect(pushButton,SIGNAL(clicked()),this,SLOT(onPushButtonPressed()));
	connect(pushButton_2,SIGNAL(clicked()),this,SLOT(onPushButton2Pressed()));
}

inputNum::~inputNum()
{

}
void inputNum::onPushButtonPressed()
{
	emit finishInputNum();
	hide();
}
void inputNum::onPushButton2Pressed()
{
	hide();
}
int inputNum::getNum()
{
	if(lineEdit->text().toInt()>=1)
		return lineEdit->text().toInt();
	else
		return 1;
}
void inputNum::setupUi(QDialog *inputNum)
{
	if (inputNum->objectName().isEmpty())
		inputNum->setObjectName(QString::fromUtf8("inputNum"));
	inputNum->setEnabled(true);
	inputNum->resize(250, 90);
	QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(inputNum->sizePolicy().hasHeightForWidth());
	inputNum->setSizePolicy(sizePolicy);
	inputNum->setMinimumSize(QSize(250, 90));
	inputNum->setMaximumSize(QSize(250, 90));
	gridLayout = new QGridLayout(inputNum);
	gridLayout->setSpacing(4);
	gridLayout->setContentsMargins(4, 4, 4, 4);
	gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
	verticalLayout = new QVBoxLayout();
	verticalLayout->setSpacing(6);
	verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
	horizontalLayout_2 = new QHBoxLayout();
	horizontalLayout_2->setSpacing(6);
	horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
	label = new QLabel(inputNum);
	label->setObjectName(QString::fromUtf8("label"));
	horizontalLayout_2->addWidget(label);
	lineEdit = new QLineEdit(inputNum);
	lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
	horizontalLayout_2->addWidget(lineEdit);
	verticalLayout->addLayout(horizontalLayout_2);
	widget = new QWidget(inputNum);
	widget->setObjectName(QString::fromUtf8("widget"));
	widget->setMaximumSize(QSize(16777215, 30));
	horizontalLayout = new QHBoxLayout(widget);
	horizontalLayout->setSpacing(4);
	horizontalLayout->setContentsMargins(11, 11, 11, 11);
	horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
	horizontalLayout->setContentsMargins(0, 0, 0, 4);
	horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	horizontalLayout->addItem(horizontalSpacer);
	pushButton = new QPushButton(widget);
	pushButton->setObjectName(QString::fromUtf8("pushButton"));
	QIcon icon;
	icon.addFile(QString::fromUtf8(":/ssp_gui/Resources/export2.gif"), QSize(), QIcon::Normal, QIcon::Off);
	pushButton->setIcon(icon);
	horizontalLayout->addWidget(pushButton);
	pushButton_2 = new QPushButton(widget);
	pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
	QIcon icon1;
	icon1.addFile(QString::fromUtf8(":/ssp_gui/Resources/del.gif"), QSize(), QIcon::Normal, QIcon::Off);
	pushButton_2->setIcon(icon1);
	horizontalLayout->addWidget(pushButton_2);
	verticalLayout->addWidget(widget);
	gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);
	retranslateUi(inputNum);
	QMetaObject::connectSlotsByName(inputNum);
	Qt::WindowFlags flags=Qt::Dialog;
	flags |=Qt::WindowCloseButtonHint;
	setWindowFlags(flags);
} // setupUi

void inputNum::retranslateUi(QDialog *inputNum)
{
	inputNum->setWindowTitle(tr("���봰��"));
	label->setText(tr("���븴�Ƹ���"));
	lineEdit->setText(tr("1"));
	pushButton->setText(tr("ȷ��(&O)"));
	pushButton_2->setText(tr("ȡ��(&C)"));
} // retranslateUi
///////////////////////////////////////////////

review::review(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);
	setWindowFlags( this->windowFlags() | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint );
	showMaximized();
}

review::~review()
{

}
void review::setupUi(QDialog *review)
{
	if (review->objectName().isEmpty())
		review->setObjectName(QString::fromUtf8("review"));
	review->resize(594, 342);
	gridLayout = new QGridLayout(review);
	gridLayout->setSpacing(6);
	gridLayout->setContentsMargins(4, 4, 4, 4);
	gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
	tabWidget = new QTabWidget(review);
	tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
	widget = new QWidget();
	widget->setObjectName(QString::fromUtf8("widget"));
	gridLayout_3 = new QGridLayout(widget);
	gridLayout_3->setSpacing(4);
	gridLayout_3->setContentsMargins(4, 4, 4, 4);
	gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
	gridLayout_2 = new QGridLayout();
	gridLayout_2->setSpacing(4);
	gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
	gridLayout_3->addLayout(gridLayout_2, 0, 0, 1, 1);
	tabWidget->addTab(widget, QString());
	widget_2 = new QWidget();
	widget_2->setObjectName(QString::fromUtf8("widget_2"));
	gridLayout_5 = new QGridLayout(widget_2);
	gridLayout_5->setSpacing(4);
	gridLayout_5->setContentsMargins(4, 4, 4, 4);
	gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
	gridLayout_4 = new QGridLayout();
	gridLayout_4->setSpacing(4);
	gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
	gridLayout_5->addLayout(gridLayout_4, 0, 0, 1, 1);
	tabWidget->addTab(widget_2, QString());
	gridLayout->addWidget(tabWidget, 0, 0, 1, 1);
	retranslateUi(review);
	tabWidget->setCurrentIndex(1);
	QMetaObject::connectSlotsByName(review);
} // setupUi

void review::retranslateUi(QDialog *review)
{
	review->setWindowTitle(tr("Ԥ������"));
	tabWidget->setTabText(tabWidget->indexOf(widget), tr("�༭����"));
	tabWidget->setTabText(tabWidget->indexOf(widget_2), tr("��ѯ����"));
} // retranslateUi


//��������ҳ��table��qtexteditor�ؼ��Ƿ������ź�
void view_datawindow_edit::setRefBlockSignals(bool blcok)
{
	ui.tableWidget_ref1->blockSignals(blcok);
	ui.tableWidget_ref2->blockSignals(blcok);
	ui.textEdit_ref->blockSignals(blcok);
}
//�������ݼ�ҳ��table��qtexteditor�ؼ��Ƿ������ź�
void view_datawindow_edit::setDataBlockSignals(bool blcok)
{
	ui.tableWidget_data1->blockSignals(blcok);
	ui.tableWidget_data2->blockSignals(blcok);
	ui.textEdit_data->blockSignals(blcok);
}